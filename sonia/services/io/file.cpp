//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#ifdef BOOST_WINDOWS
#   include "sonia/utility/windows.hpp"
#else

#endif

#include "file.hpp"

#include <boost/lexical_cast.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/sal.hpp"

namespace sonia { namespace io {

#ifdef BOOST_WINDOWS
#   define NOT_A_FILE_HANDLE nullptr
#   define is_valid(h) h
#   define close_handle(h) CloseHandle(h)
#else
#   define NOT_A_FILE_HANDLE -1
#   define is_valid(h) -1 != h
#   define close_handle(h) ::close(h)
#endif

file::file(file && rhs) : impl_(std::move(rhs.impl_)), path_(rhs.path_), fh_(rhs.fh_) {
    rhs.fh_ = NOT_A_FILE_HANDLE;
}

file::~file() {
    if (is_valid(fh_)) {
        close_handle(fh_);
    }
}

file & file::operator= (file && rhs) {
    if (fh_ != rhs.fh_) {
        close();
        path_ = std::move(rhs.path_);
        fh_ = rhs.fh_;
        impl_ = std::move(rhs.impl_);
        rhs.fh_ = NOT_A_FILE_HANDLE;
    }
    return *this;
}

void file::close() noexcept {
    if (is_valid(fh_)) {
        close_handle(fh_);
        fh_ = NOT_A_FILE_HANDLE;
    }
}

void file::remove() {
    if (is_valid(fh_)) {
        cstring_view cp = path();
        close();
        sonia::sal::delete_file(cp);
    }
}

uint64_t file::size() const {
    try {
        return sonia::sal::file_size(fh_);
    } catch (std::exception const& e) {
        throw exception(e.what(), ", file: ", path());
    }
}

size_t file::read(uint64_t fileoffset, array_view<char> dest) const {
    try {
        return impl_->file_read(fh_, fileoffset, dest);
    } catch (std::exception const& e) {
        throw exception(e.what(), ", file: ", path());
    }
}

size_t file::write(uint64_t fileoffset, array_view<const char> src) {
    try {
        return impl_->file_write(fh_, fileoffset, src);
    } catch (std::exception const& e) {
        throw exception(e.what(), ", file: ", path());
    }
}

#ifdef BOOST_WINDOWS
using namespace sonia::windows;

cstring_view file::path() const {
    if (path_.empty()) {
        try {
            path_ = get_file_name(fh_);
        } catch (...) {
            path_ = "file name is not available";
        }
    }
    return path_;
}

void file::set_size(uint64_t sz) {
    LARGE_INTEGER pos;
    pos.QuadPart = sz;
    LARGE_INTEGER new_pos;
    if (!SetFilePointerEx(fh_, (LARGE_INTEGER)pos, &new_pos, FILE_BEGIN) || !SetEndOfFile(fh_)) {
        DWORD err = GetLastError();
        throw exception("truncate file error : %1%, file: %2%"_fmt % error_message(err) % path());
    }
    if (sz != new_pos.QuadPart) {
        throw exception("truncate error, reached %1% instead of %2%, file: %3%"_fmt % new_pos.QuadPart % sz % path());
    }
}

void file::flush() const {
    if (!FlushFileBuffers((HANDLE)fh_)) {
        DWORD err = GetLastError();
        throw exception("flush file error : %1%, file: %2%"_fmt % error_message(err) % path());
    }
}

#else // else POSIX

cstring_view file::path() const {
    return path_;
}

void file::set_size(uint64_t sz) {
    if (-1 == ftruncate(fh_, (off_t)sz)) {
        int err = errno;
        throw exception("truncate file error : %1%, file: %2%"_fmt % strerror(err) % path());
    }
}

void file::flush() const {
    if (-1 == fsync(fh_)) {
        int err = errno;
        throw exception("flush file error : %1%, file: %2%"_fmt % strerror(err) % path());
    }
}

#endif

}}
