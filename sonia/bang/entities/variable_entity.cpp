//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "variable_entity.hpp"

#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

std::ostream& variable_entity::print_to(std::ostream& os, unit const& u) const
{
    return os << "VAR "sv << u.print(name);
}

}
