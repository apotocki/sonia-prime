//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#include <vector>
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"

#include "sonia/utility/iterators/http_form_data_read_iterator.hpp"
#include "sonia/utility/iterators/empty_check_iterator.hpp"

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
    template <class, class> friend class ptr_proxy_wrapper;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<chunk_iterator const*, array_view<const char>>>;

    proxy_t dereference() const
    {
        return iterators::make_value_proxy<array_view<const char>>(this);
    }

    array_view<const char> get_dereference() const
    {
        auto const& chunk = (*data_)[pos_];
        auto r = to_array_view(chunk);
        r.advance_front(offset_);
        return r;
    }

    void set_dereference(array_view<const char> rng)
    {
        BOOST_ASSERT (rng.is_subset_of(to_array_view((*data_)[pos_])));
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

std::vector<char> to_vector(string_view sw) { return std::vector<char>(sw.begin(), sw.end()); }

void test_data(std::vector<std::vector<char>> & data, string_view boundary, string_view expected, string_view expected2)
{
    chunk_iterator it0{make_shared<data_t>(std::move(data))};
    http_form_data_read_iterator<chunk_iterator> fdit{std::move(it0), boundary};
    std::string result;
    while (!fdit.empty()) {
        array_view<const char> c = *fdit;
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

}

BOOST_AUTO_TEST_CASE(http_form_data_read_iterator_test)
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
