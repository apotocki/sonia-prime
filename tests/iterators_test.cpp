//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
#define TEST_FOLDER "iterator_test"

#include "sonia/string.hpp"
#include "sonia/utility/iterators/file_region_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

#include "sonia/utility/optional_inheritor.hpp"

using namespace sonia;

class test_cls
    : public polymorphic_clonable
    , public polymorphic_movable
{
public:
    ~test_cls() {}
};

BOOST_AUTO_TEST_CASE (iterators_test)
{
    fs::remove_all(TEST_FOLDER);

    typedef file_region_iterator<const char> file_iterator_t;
    typedef file_region_iterator<char> file_write_iterator_t;

    typedef range_dereferencing_iterator<file_write_iterator_t> write_iterator_t;

    // create the file
    {
        fs::create_directories(TEST_FOLDER);
        std::ofstream f(TEST_FOLDER "/test.dat");
    }

    file_write_iterator_t fout(TEST_FOLDER "/test.dat");
    write_iterator_t it(std::move(fout));
    std::string content = "this is a content";
    std::copy(content.begin(), content.end(), std::move(it)).flush();
    BOOST_CHECK_EQUAL(content.size(), fs::file_size(TEST_FOLDER "/test.dat"));

    {
        std::ofstream f(TEST_FOLDER "/test1.dat");
        file_write_iterator_t fout2(TEST_FOLDER "/test1.dat");
        *fout2 = string_view(content);
    }
    BOOST_CHECK_EQUAL(content.size(), fs::file_size(TEST_FOLDER "/test1.dat"));

    file_iterator_t fin("data/sparqldaf184.n3", 0, 65536);
    file_iterator_t bit = fin;
    uint64_t sz = 0;
    while (!fin.empty()) {
        sz += fin->size();
        ++fin;
    }
    BOOST_CHECK_EQUAL(sz, fs::file_size("data/sparqldaf184.n3"));

    typedef optional_inheritor<test_cls, 8> opt_t;
    opt_t opt;
    opt_t opt2 = std::move(opt);
}
