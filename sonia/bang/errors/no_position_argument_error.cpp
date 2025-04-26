//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "no_position_argument_error.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

general_error::string_t no_position_argument_error::object(unit const& u) const noexcept
{
    return ""sv;
}

general_error::string_t no_position_argument_error::description(unit const& u) const noexcept
{
    return ("no argument at position: %1%"_fmt % arg_index_).str() ;
}

}
