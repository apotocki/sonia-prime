//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "assign_error.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

general_error::string_t assign_error::object(unit const& u) const noexcept
{
    return u.print(lhs_);
}

general_error::string_t assign_error::description(unit const& u) const noexcept
{
    return "not a lvalue expression"sv;
}

}
