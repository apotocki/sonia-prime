//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/singleton.hpp"

#include "basic_attribute_resolver.hpp"

namespace sonia::xmlbuilder {

class ui_attribute_resolver : public basic_attribute_resolver, public singleton
{
public:
    ui_attribute_resolver();

protected:
    void setup_view(string_view);
    void setup_field(string_view);
};

}
