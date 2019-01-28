//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/windows.hpp"
#include "sonia/sal.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia { namespace sal {

namespace winapi = sonia::windows;

void set_thread_name(sonia::thread::id tid, string_view name)
{
    winapi::set_thread_name(tid, (&name.back() + 1) ? to_string(name).c_str() : name.begin());
}

void set_thread_name(sonia::thread::id tid, std::string const& name)
{
    winapi::set_thread_name(tid, name.c_str());
}

typedef sonia::services::bundle*(get_bundle_fn)();

shared_ptr<sonia::services::bundle> load_bundle(string_view name)
{
#if !defined(__MINGW32__) && !defined(__MINGW64__)
    std::string libname = to_string(name) + ".dll";
#else
    std::string libname = "lib" + to_string(name) + ".dll";
#endif
    HMODULE lib_handle = LoadLibraryA(libname.c_str());
    if (!lib_handle) {
        DWORD err = GetLastError();
        throw internal_error("Cannot load bundle: %1%\n%2%"_fmt % libname % winapi::error_message(err));
    }
    SCOPE_EXIT([&lib_handle]() { FreeLibrary(lib_handle); });

    get_bundle_fn* fn_handle = (get_bundle_fn*)GetProcAddress(lib_handle, "get_bundle");
    if (!fn_handle) {
        DWORD err = GetLastError();
        throw internal_error("Cannot load symbol 'get_bundle' in %1% module\n%2%"_fmt % libname % winapi::error_message(err));
    }
    shared_ptr<sonia::services::bundle> result = shared_ptr<sonia::services::bundle>(fn_handle());
    result->set_handle(lib_handle);
    lib_handle = NULL; // unpin
    return std::move(result);
}

uint64_t file_size(HANDLE h)
{
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(h, &fileSize)) {
        DWORD err = GetLastError();
        throw exception("get file size error : %1%"_fmt % winapi::error_message(err));
    }
    return (((uint64_t)fileSize.HighPart) << 32) | fileSize.LowPart;
}

void delete_file(cstring_view path)
{
    std::wstring wfname = winapi::utf8_to_utf16(path);
    winapi::delete_file(wfname.c_str(), path.c_str());
}

}}
