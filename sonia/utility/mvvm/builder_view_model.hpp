//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/string.hpp"
#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/utility/mvvm/lua_view_model.hpp"

#include "sonia/bang/bang.hpp"

namespace sonia {
 
class builder_view_model 
    : public lua_view_model
    , public sonia::lang::bang::language
{
    using base_t = lua_view_model;

public:
    using lua_view_model::lua_view_model;
};

}
