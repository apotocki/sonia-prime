//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/invokation/invokable.hpp"

namespace sonia::lang::bang {

class external_environment
{
public:
    virtual ~external_environment() = default;

    virtual shared_ptr<invokation::invokable> create(string_view type, string_view id) = 0;
    virtual void set_property(string_view propname, blob_result const& value) = 0;

    //virtual shared_ptr<external_environment> make_shared_environment() = 0;
};

}
