//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "sonia/utility/windows.hpp"

namespace sonia { namespace sal {

void set_thread_name(sonia::thread::id tid, string_view name) {
    sonia::windows::set_thread_name(tid, (&name.back() + 1) ? to_string(name).c_str() : name.begin());
}

void set_thread_name(sonia::thread::id tid, std::string const& name) {
    sonia::windows::set_thread_name(tid, name.c_str());
}

}}
