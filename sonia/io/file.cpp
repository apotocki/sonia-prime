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

namespace sonia { namespace io {

#ifdef BOOST_WINDOWS
using namespace sonia::windows;

file::~file() {
    if (fh_) {
        CloseHandle((HANDLE)fh_);
    }
}

void file::close() {
    if (fh_) {
        CloseHandle((HANDLE)fh_);
        fh_ = nullptr;
    }
}

uint64_t file::size() const {
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx((HANDLE)fh_, &fileSize)) {
        DWORD err = GetLastError();
        throw exception("get file size error : %1%, file: %2%"_fmt % error_message(err) % get_file_name((HANDLE)fh_));
    }
    return (((uint64_t)fileSize.HighPart) << 32) | fileSize.LowPart;
}

void file::set_size(uint64_t sz) {
    LARGE_INTEGER pos;
    pos.QuadPart = sz;
    LARGE_INTEGER new_pos;
    if (!SetFilePointerEx((HANDLE)fh_, (LARGE_INTEGER)pos, &new_pos, FILE_BEGIN) || !SetEndOfFile((HANDLE)fh_)) {
        DWORD err = GetLastError();
        throw exception("truncate file error : %1%"_fmt % error_message(err));
    }
    if (sz != new_pos.QuadPart) {
        throw exception("truncate error, reached %1% instead of %2%, file: %3%"_fmt % new_pos.QuadPart % sz % get_file_name((HANDLE)fh_));
    }
}

void file::remove() {
    if (fh_) {
        std::string path = get_file_name((HANDLE)fh_);
        close();
        delete_file(path);
    }
}

#else // else POSIX


#endif

size_t file::read(uint64_t fileoffset, array_view<char> dest) {
    return impl_->file_read(fh_, fileoffset, dest);
}

size_t file::write(uint64_t fileoffset, array_view<const char> src) {
    return impl_->file_write(fh_, fileoffset, src);
}

}}
