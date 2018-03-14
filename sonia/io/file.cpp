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
#   include <Windows.h>
#else

#endif

namespace sonia { namespace io {

#ifdef BOOST_WINDOWS

string error_message(DWORD errcode)  {
    LPWSTR pBuffer = nullptr;
    DWORD num = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,  
        NULL,
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&pBuffer,
        0,
        NULL);

    if (pBuffer) {
        SCOPE_EXIT([&pBuffer]() { LocalFree(pBuffer); });

        int slength = (int)num + 1;
        int len = WideCharToMultiByte(CP_ACP, 0, pBuffer, slength, 0, 0, 0, 0);
        std::vector<char> buf(len);
        WideCharToMultiByte(CP_ACP, 0, pBuffer, slength, &buf[0], len, 0, 0);
        while (!buf.empty() && (buf.back() == '\r' || buf.back() == '\n')) buf.pop_back();
        return string(buf.begin(), buf.end());
    }

    return "unknown error, errcode: " + boost::lexical_cast<string>(errcode);
}

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
            throw exception("read file error : " + error_message(errcode));
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
    DWORD dwCreationDisposition;

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
