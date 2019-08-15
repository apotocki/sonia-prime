//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bookkeeper_service.hpp"
#include "sonia/exceptions.hpp"

namespace sonia::services {

bookkeeper_service::bookkeeper_service(bookkeeper_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "file based bookkeeper");
}

void bookkeeper_service::open()
{
    statable_t::open();
    restore();
}

void bookkeeper_service::set(string_view key, json_value const& val)
{
    lock_guard wguard(mtx_);
    auto it = map_.find(key, hasher(), string_equal_to());
    if (it != map_.end()) {
        it->second = val;
    } else {
        map_.insert(it, std::pair{to_string(key), val});
    }
    backup();
}

optional<json_value> bookkeeper_service::get(string_view key) const
{
    shared_lock_guard sguard(mtx_);
    auto it = map_.find(key, hasher(), string_equal_to());
    return (it != map_.end()) ? optional{it->second} : optional<json_value>{};
}

void bookkeeper_service::erase(string_view key)
{
    lock_guard wguard(mtx_);
    auto it = map_.find(key, hasher(), string_equal_to());
    if (it != map_.end()) {
        map_.erase(it);
    }
    backup();
}

bool bookkeeper_service::compare_and_swap(string_view key, json_value const* expected, json_value const* newval)
{
    lock_guard wguard(mtx_);
    auto it = map_.find(key, hasher(), string_equal_to());
    if (it != map_.end()) {
        if (expected && it->second == *expected) {
            if (newval) {
                it->second = *newval;
            } else {
                map_.erase(it);
            }
            backup();
            return true;
        }
    } else if (!expected) {
        if (newval) {
            map_.insert(it, std::pair{to_string(key), *newval});
            backup();
        }
        return true;
    }
    return false;
}

}
