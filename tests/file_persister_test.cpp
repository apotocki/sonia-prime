//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <filesystem>

namespace fs = std::filesystem;
#define TEST_FOLDER "file_persister_test"

#include "sonia/utility/file_persister.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

#include "applied/sonia_test.hpp"

using namespace sonia;

#include <iostream>

#include "sonia/utility/iterators/file_region_iterator.hpp"

void file_persister_test()
{
    fs::remove_all(TEST_FOLDER);

    fs::create_directories(TEST_FOLDER);

    file_persister p(TEST_FOLDER "/folder1/folder2/test.data");

    std::string text("text to persist into file");

    p.write([&text](file_persister::output_iterator it) {
        std::copy(text.begin(), text.end(), range_dereferencing_iterator(std::move(it))).flush();
    });
    
    std::string result;
    p.read([&result](file_persister::input_iterator it) {
		for (auto rdit = range_dereferencing_iterator(std::move(it)); !rdit.empty(); ++rdit) {
            result.push_back(*rdit);
        }
    });

    BOOST_CHECK_EQUAL(result, text);
}

void file_persister_test_registrar()
{
    register_test(BOOST_TEST_CASE(&file_persister_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(file_persister_test_registrar)
#endif
