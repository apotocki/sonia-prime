//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//
#include <jni.h>
#ifdef __ANDROID__
#   include <android/log.h>
#else
inline void __android_log_write(...) {}
// dummy
#define ANDROID_LOG_INFO ""
#endif

#include <iostream>
#include <vector>
namespace sonia::android {

class androidbuf : public std::streambuf {
    const char* name_;
public:
    static constexpr size_t bufsize = 1024; // ... or some other suitable buffer size

    explicit androidbuf(const char* name) : name_{ name } { this->setp(buffer, buffer + bufsize - 1); }

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
            __android_log_write(ANDROID_LOG_INFO, name_, line_.data());
            line_.clear();
            this->setp(buffer, buffer + bufsize);
        }
        return 0;
    }

    char buffer[bufsize];
    std::vector<char> line_;
};

}
