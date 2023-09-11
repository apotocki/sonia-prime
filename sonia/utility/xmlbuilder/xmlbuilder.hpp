//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

#include "sonia/string.hpp"
#include "sonia/variant.hpp"

#include "sonia/utility/invokation/invokation.hpp"

namespace sonia::xmlbuilder {

class external_builder
{
public:
    virtual void create(string_view type, string_view id) = 0;
    virtual void set_text(string_view id, string_view text) = 0;
    virtual void set_property(string_view id, string_view propname, blob_result const& value) = 0;
    virtual void set_property_functional(string_view id, string_view propname, string_view code, bool no_return) = 0;
    virtual void append(string_view parentid, string_view childid) = 0;
};


void parse(string_view code, external_builder & eb);

}
