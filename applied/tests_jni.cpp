//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//
#include "sonia/config.hpp"
#include "tests_external.hpp"
#include <jni.h>
#include <android/log.h>

#include <iostream>

#include <boost/bind/placeholders.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/exception/diagnostic_information.hpp>

class androidbuf : public std::streambuf {
public:
    enum { bufsize = 1024 }; // ... or some other suitable buffer size
    androidbuf() { this->setp(buffer, buffer + bufsize - 1); }

private:
    int overflow(int c)
    {
        if (c == traits_type::eof()) {
            sync();
        } else if (epptr() == pptr()) {
            line_.insert(line_.end(), pbase(), pptr());
            line_.push_back(traits_type::to_char_type(c));
            this->setp(buffer, buffer + bufsize);
        }
        return traits_type::not_eof(c);
    }

    int sync()
    {
        if (this->pbase() != this->pptr()) {
            line_.insert(line_.end(), pbase(), pptr());
            line_.push_back(0);
            __android_log_write(ANDROID_LOG_INFO, "std", line_.data());
            line_.clear();
            this->setp(buffer, buffer + bufsize);
        }
        return 0;
    }

    char buffer[bufsize];
    std::vector<char> line_;
};

#include <unistd.h>

extern "C" JNIEXPORT jint JNICALL
Java_com_example_tescppapplication_MainActivity_run(JNIEnv * env, jobject, jstring dir)
{
    std::cout.rdbuf(new androidbuf);
    const char* path = env->GetStringUTFChars(dir, NULL);
    std::cout << "provided path: " << path << std::endl;

    if (int r = chdir(path); r == -1) {
        int err = errno;
        std::cout <<  "can't set working dir, error : " << strerror(err) << std::endl;
        return 1;
    }
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working dir: " << cwd << std::endl;
    } else {
        std::cout << "getcwd() error" << std::endl;
    }
    const char* argv[] = { "tests", "--no_color_output", "--log_level=test_suite" };
    int r = 0;
    try {
        r = run_tests(3, (char**)argv);
    } catch (...) {
        std::cout << "ERROR: " << boost::current_exception_diagnostic_information() << std::endl;
    }
    delete std::cout.rdbuf(0);
    return r;
}
