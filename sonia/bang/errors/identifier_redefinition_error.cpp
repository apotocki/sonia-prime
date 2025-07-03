//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "identifier_redefinition_error.hpp"
#include "utility.hpp"

namespace sonia::lang::bang {

general_error::string_t identifier_redefinition_error::object(unit const& u) const noexcept
{
    return apply_visitor(printer_resolver_visitor{ u }, name_);
}

}
