//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "main_api.hpp"

#include "sonia/utility/scope_exit.hpp"

const DWORD MS_VC_EXCEPTION=0x406D1388;

namespace sonia::windows {

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void set_thread_name(DWORD dwThreadId, char const* threadName)
{
// is needed and works only on Windows for VS Debugger
#if !defined(__MINGW32__) && !defined(__MINGW64__)
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadId;
    info.dwFlags = 0;

    __try
    {
        RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR),       (ULONG_PTR*)&info );
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
#endif
}

void set_thread_name(boost::thread::id tid, char const* threadName)
{
    unsigned int dwThreadId;
    std::stringstream ss;
    ss << std::hex << tid;
    ss >> dwThreadId;
    set_thread_name(dwThreadId, threadName);
}

std::wstring utf8_to_utf16(string_view str)
{
    std::wstring result;
    result.resize(str.size() + 1);
    int len = MultiByteToWideChar(CP_UTF8, 0, str.begin(), (int)str.size(), result.data(), (int)result.size());
    if (len > result.size()) {
        result.resize((size_t)len);
        len = MultiByteToWideChar(CP_UTF8, 0, str.begin(), (int)str.size(), result.data(), (int)result.size());
        BOOST_ASSERT(len <= result.size());
    }
    return std::move(result);
}

std::string utf16_to_utf8(wstring_view str)
{
    std::string result;
    result.resize(str.size());
    int len = WideCharToMultiByte(CP_UTF8, 0, str.begin(), (int)str.size(), result.data(), (int)result.size(), NULL, NULL);
    if (len > result.size()) {
        result.resize((size_t)len);
        int len = WideCharToMultiByte(CP_UTF8, 0, str.begin(), (int)str.size(), result.data(), (int)result.size(), NULL, NULL);
        BOOST_ASSERT(len <= result.size());
    }
    return std::move(result);
}

std::string error_message(DWORD errcode)
{
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
        while (!buf.empty() && (buf.back() == '\0' || buf.back() == '\r' || buf.back() == '\n')) buf.pop_back();
        return std::string(buf.begin(), buf.end());
    }

    return to_string("unknown error, errcode: %1%"_fmt % errcode);
}

}
