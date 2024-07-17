//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//
#include "sonia/config.hpp"
#include "tests_external.hpp"

#include "sonia/sys/android/streambuf.hpp"

#include <iostream>

#include <boost/bind/placeholders.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <unistd.h>

#include <fstream>
#include <vector>
#include "sonia/filesystem.hpp"

//#define DO_NOT_USE_ASSETS

#ifndef DO_NOT_USE_ASSETS

#include "sonia/utility/iterators/file_region_iterator.hpp"
#include "sonia/utility/iterators/archive_extract_iterator.hpp"

namespace {

void restore_assets(const char* archivepath)
{
    using namespace sonia;
    try {
        fs::path filepath = archivepath;
        fs::path filename = filepath.filename();
        archive_iterator ait = make_archive_extract_iterator(filename.string().c_str(), file_region_iterator<const char>(filepath, 0, 65536));
        ait.impl->set_extraction_depth(2); // xz->tar->
        while (ait.next()) {
            fs::path itempath = ait.name();
            std::vector<fs::path> parts;
            while (!itempath.empty()) {
                parts.emplace_back(itempath.filename());
                itempath = itempath.parent_path();
            }
            if (parts.empty()) continue;
            parts.pop_back();
            while (!parts.empty()) {
                itempath /= parts.back();
                parts.pop_back();
            }
            if (auto parent = itempath.parent_path(); !parent.empty()) {
                fs::create_directories(parent);
            }
            { std::ofstream f{ itempath.string().c_str() }; }
            file_region_iterator<char> fit(itempath, 0, 65536);
            //std::cout << "restored " << itempath << "\n";
            size_t fsz = 0;
            do {
                span<const char> rng = *ait;
                fit.write(rng);
                ++ait;
            } while (!ait.empty());
            fit.close();
        }
    }
    catch (...) {
        std::cout << boost::current_exception_diagnostic_information();
    }
}

}

#endif

extern "C" JNIEXPORT jint JNICALL
Java_com_example_tescppapplication_MainActivity_run(JNIEnv * env, jobject, jstring dir, jstring assetsfile, jstring option)
{
    using namespace sonia::android;

    std::cout.rdbuf(new androidbuf("cout"));
    std::cerr.rdbuf(new androidbuf("cerr"));
    const char* path = env->GetStringUTFChars(dir, NULL);
    std::cout << "provided path: " << path << std::endl;

    if (int r = chdir(path); r == -1) {
        int err = errno;
        std::cout <<  "can't set working dir, error : " << strerror(err) << std::endl;
        return 1;
    }

#ifndef DO_NOT_USE_ASSETS
    const char* assetspath = env->GetStringUTFChars(assetsfile, NULL);
    restore_assets(assetspath);
#endif

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working dir: " << std::string_view{cwd} << std::endl;
    } else {
        std::cout << "getcwd() error" << std::endl;
    }
    const char* optionstr = env->GetStringUTFChars(option, NULL); // e.g. "--run_test=!http_server_filebrowser_test"
    const char* argv[] = { "tests", "--no_color_output", "--log_level=test_suite", optionstr };
    int r = 0;
    try {
        r = run_tests(sizeof(argv)/sizeof(const char*), (char**)argv);
        std::cout.flush();
    } catch (...) {
        std::cout << "ERROR: " << boost::current_exception_diagnostic_information() << std::endl;
    }
    
    delete std::cout.rdbuf(0);
    delete std::cerr.rdbuf(0);
    return r;
}
