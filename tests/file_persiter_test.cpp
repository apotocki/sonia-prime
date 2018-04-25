//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
#define TEST_FOLDER "file_persister_test"

#include "sonia/utility/file_persister.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

using namespace sonia;

#include <iostream>


BOOST_AUTO_TEST_CASE(file_persister_test)
{
    fs::remove_all(TEST_FOLDER);
    typedef std::list<int>::iterator it_t;

    fs::create_directories(TEST_FOLDER);

    file_persister p(TEST_FOLDER "/test.data");

    std::string text("text to persist into file");

    std::copy(text.begin(), text.end(), make_range_dereferencing_iterator(p.writer())).flush();
}
