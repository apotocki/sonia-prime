//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <vector>
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"

#include "sonia/utility/iterators/http_chunking_read_input_iterator.hpp"
#include "sonia/utility/iterators/http_chunking_write_input_iterator.hpp"
#include "sonia/utility/iterators/http_form_data_read_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/utility/iterators/buffering_write_input_iterator.hpp"

#include "applied/sonia_test.hpp"

using namespace sonia;

namespace {

using data_t = std::vector<std::vector<char>>;

class chunk_iterator
    : public boost::iterator_facade<
          chunk_iterator
        , array_view<const char>
        , std::input_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<chunk_iterator const*, array_view<const char>>>
    >
{
    friend class boost::iterator_core_access;
    friend class ptr_proxy_wrapper<chunk_iterator const*, array_view<const char>>;
    
    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<chunk_iterator const*, array_view<const char>>>;

    proxy_t dereference() const
    {
        return iterators::make_value_proxy<array_view<const char>>(this);
    }

    array_view<const char> get_dereference() const
    {
        auto const& chunk = (*data_)[pos_];
        auto r = array_view(chunk);
        r.advance_front(offset_);
        return r;
    }

    void set_dereference(array_view<const char> rng)
    {
        BOOST_ASSERT (rng.is_subset_of(array_view((*data_)[pos_])));
        offset_ = static_cast<size_t>(rng.begin() - &(*data_)[pos_].front());
    }

    void increment()
    {
        ++pos_;
        offset_ = 0;
    }

public:
    chunk_iterator(shared_ptr<data_t> data) : data_{std::move(data)} {}

    bool empty() const { return pos_ == data_->size(); }

private:
    shared_ptr<data_t> data_;
    size_t pos_{0};
    size_t offset_{0};
};

class vector_write_output_iterator
    : public boost::iterator_facade<
          vector_write_output_iterator
        , array_view<const char>
        , std::output_iterator_tag
        , array_view<const char> &
    >
{
    friend class boost::iterator_core_access;
    
    bool equal(vector_write_output_iterator const& rhs) const
    {
        return false;
    }

    array_view<const char> & dereference() const
    {
        return buffer_;
    }

    void increment()
    {
        vec_->insert(vec_->end(), buffer_.begin(), buffer_.end());
        buffer_.reset();
    }

    mutable array_view<const char> buffer_;
    std::vector<char> * vec_;

public:
    vector_write_output_iterator(std::vector<char> & vec)
        : vec_{&vec}
    {}

    bool empty() const
    {
        return false;
    }
};

std::vector<char> to_vector(string_view sw) { return std::vector<char>(sw.begin(), sw.end()); }

void test_data(std::vector<std::vector<char>> & data, string_view boundary, string_view expected, string_view expected2)
{
    chunk_iterator it0{make_shared<data_t>(std::move(data))};
    http_form_data_read_iterator<chunk_iterator> fdit{std::move(it0), boundary};
    std::string result;
    while (!fdit.empty()) {
        std::span<const char> c = *fdit;
        result += std::string(c.begin(), c.end());
        ++fdit;
    }
    BOOST_CHECK_EQUAL(result, expected);
    it0 = std::move(fdit.base);
    result.clear();
    while (!it0.empty()) {
        array_view<const char> c = *it0;
        result += std::string(c.begin(), c.end());
        ++it0;
    }
    if (result != expected2) {
        BOOST_CHECK_EQUAL(result, expected2);
    }
}

void test_chunked_data(std::vector<std::vector<char>> & data, string_view expected)
{
    chunk_iterator it0{make_shared<data_t>(std::move(data))};
    http_chunking_read_input_iterator<chunk_iterator> fdit{std::move(it0)};
    std::string result;
    while (!fdit.empty()) {
        range_dereferencing_iterator rdit{std::move(fdit)};
        result.push_back(*rdit);
        ++rdit;
        rdit.flush();
        fdit = std::move(rdit.base);
        if (fdit.empty()) break;
        std::span<const char> c = *fdit;
        result += std::string(c.begin(), c.end());
        ++fdit;
    }
    BOOST_CHECK_EQUAL(result, expected);
    BOOST_CHECK(fdit.base.empty());
}

void test_chunking_data(std::vector<std::vector<char>> & data, size_t buffsz, string_view expected)
{
    std::vector<char> store;
    std::vector<char> buffer(buffsz);
    http_chunking_write_input_iterator it{buffering_write_input_iterator{vector_write_output_iterator(store), buffer}};
    for (auto const& vec : data) {
        range_dereferencing_iterator rdit{std::move(it)};
        rdit = std::copy(vec.begin(), vec.end(), std::move(rdit));
        rdit.fix();
        it = std::move(rdit.base);
    }
    it.close();
    BOOST_CHECK_EQUAL(string_view(array_view(store)), expected);
}

}

void http_form_data_read_iterator_test()
{
    string_view boundary("\r\n----boundary--");
    
    using data_t = std::vector<std::vector<char>>;

    data_t d0{
        to_vector("some data"),
        to_vector(boundary),
        to_vector("fin")
    };
    test_data(d0, boundary, "some data", "fin");

    data_t d1{
        to_vector(std::string("some data1") + to_string(boundary) + "fin")
    };
    test_data(d1, boundary, "some data1", "fin");

    data_t d2{
        to_vector("some data2\r"),
        to_vector("\n----boundary--f"),
        to_vector("in")
    };
    test_data(d2, boundary, "some data2", "fin");

    data_t d3{
        to_vector("some data3\r"),
        to_vector("\n-"),
        to_vector("---boundary--"),
        to_vector("fin")
    };
    test_data(d3, boundary, "some data3", "fin");

    data_t d4{
        to_vector("some"),
        to_vector(" data4\r"),
        to_vector("\n-"),
        to_vector("---boundary-"),
        to_vector("-fin"),
    };
    test_data(d4, boundary, "some data4", "fin");

    data_t d5{
        to_vector("some data5\r\ndata"),
        to_vector("\r\n-"),
        to_vector("---boundary--"),
        to_vector("other data"),
    };
    test_data(d5, boundary, "some data5\r\ndata", "other data");

    data_t d6{
        to_vector("some"),
        to_vector(" data6\r"),
        to_vector("\n-"),
        to_vector("---boundary-"),
        to_vector("oops\r\n-"),
        to_vector("---boundary--fin"),
    };
    test_data(d6, boundary, "some data6\r\n----boundary-oops", "fin");

    data_t d7{
        to_vector(std::string("some data7\r\n--")),
        to_vector(std::string("--boundary!\r\n--")),
        to_vector(std::string("--boundary--")),
        to_vector(std::string("other data")),
    };
    test_data(d7, boundary, "some data7\r\n----boundary!", "other data");

    data_t d8{
        to_vector(std::string("some data8\r\n--")),
        to_vector(std::string("--boundary!\r\n----boundary--")),
        to_vector(std::string("other data")),
    };
    test_data(d8, boundary, "some data8\r\n----boundary!", "other data");
}

void http_chunking_read_iterator_test()
{
    using data_t = std::vector<std::vector<char>>;

    data_t d0{
        to_vector("9\r\nsome data\r\n0\r\n\r\n")
    };
    test_chunked_data(d0, "some data");

    data_t d1{
        to_vector("0009\r"),
        to_vector("\nsome data\r"),
        to_vector("\n00000\r\n\r"),
        to_vector("\n"),
    };
    test_chunked_data(d1, "some data");

    data_t d2{
        to_vector("0009\r"),
        to_vector("\nsome "),
        to_vector("data"),
        to_vector("\r\n00000\r\n\r"),
        to_vector("\n"),
    };
    test_chunked_data(d2, "some data");
}

void http_chunking_write_iterator_test()
{
    data_t d0{
        to_vector("some data")
    };
    test_chunking_data(d0, 20, "9\r\nsome data\r\n0\r\n\r\n");
    test_chunking_data(d0, 21, "09\r\nsome data\r\n0\r\n\r\n");

    data_t d1{
        to_vector("some"),
        to_vector("data")
    };
    test_chunking_data(d1, 20, "8\r\nsomedata\r\n0\r\n\r\n");

    data_t d2{
        to_vector("some data "),
        to_vector("some data ")
    };
    test_chunking_data(d2, 20, "F\r\nsome data some \r\n5\r\ndata \r\n0\r\n\r\n");
}

void http_iterators_test_registrar()
{
    register_test(BOOST_TEST_CASE(&http_form_data_read_iterator_test));
    register_test(BOOST_TEST_CASE(&http_chunking_read_iterator_test));
    register_test(BOOST_TEST_CASE(&http_chunking_write_iterator_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(http_iterators_test_registrar)
#endif
