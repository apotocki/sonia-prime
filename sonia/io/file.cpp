//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "file.hpp"

#include <boost/lexical_cast.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"

#ifdef BOOST_WINDOWS
#   include "sonia/utility/windows.hpp"
#else

#endif

namespace sonia { namespace io {

#ifdef BOOST_WINDOWS



void file_create() {

}

uint64_t file_read(HANDLE fh, uint64_t offset, void * dest, uint64_t sz) {
    OVERLAPPED ovl;
    std::memset(&ovl, 0, sizeof(OVERLAPPED));
    ovl.Offset = (DWORD)(offset & 0xFFFFFFFF);
    ovl.OffsetHigh = (DWORD)(offset >> 32);

    SetLastError(0);
    
    uint64_t rsz = 0;
    for (;;) {
        DWORD sz2read = sz > 0xffffffff ? 0xffffffff : (DWORD)sz;
        void * addr = reinterpret_cast<char*>(dest) + rsz;
        DWORD outrsz;
        if (!ReadFile(fh, addr, sz2read, &outrsz, &ovl)) {
            DWORD errcode = GetLastError();
            throw exception("read file error : " + sonia::windows::error_message(errcode));
        }
        rsz += (uint64_t)outrsz;
        sz -= (uint64_t)outrsz;
        if (outrsz != sz2read || !sz) return rsz;
    }
}
#else // else POSIX


#endif

file::file(boost::filesystem::path const& fp, bool create_if_not_exists) {
#ifdef BOOST_WINDOWS
    // DWORD dwCreationDisposition;

#else

#endif
}

void file::read(uint64_t fileoffset, void * dest, uint64_t sz) {
    if (!sz) return;
    uint64_t rsz = file_read(fh_, fileoffset, dest, sz);
    if (rsz < (uint32_t)sz) {

    }
}

}}
