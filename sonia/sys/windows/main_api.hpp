//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#   define UNICODE
#endif
//#define WINVER 0x0600
#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0600
#endif

#include <Windows.h>

#include <boost/thread/thread.hpp>

#include "sonia/string.hpp"

namespace sonia::windows {

void set_thread_name(DWORD dwThreadId, char const* threadName);
void set_thread_name(boost::thread::id tid, char const* threadName);

std::wstring utf8_to_utf16(string_view);
std::string utf16_to_utf8(wstring_view);

std::string error_message(DWORD errcode);

}
