//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "sonia/string.hpp"

namespace sonia::services {

class host
{
public:
    virtual ~host() = default;
    virtual void close() = 0;

    virtual std::string_view get_name() const = 0;

    virtual void attach_to_current_thread() = 0;
    virtual void dettach_from_current_thread() = 0;
};

}
