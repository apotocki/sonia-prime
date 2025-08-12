//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "cast_error.hpp"
#include "utility.hpp"

#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

general_error::string_t cast_error::description(unit const& u) const noexcept
{
    std::ostringstream ss;
    ss << "error: cannot cast '"sv;
    u.print_to(ss, src_);
    ss << "' to '"sv;
    u.print_to(ss, dest_);
    ss << "'"sv;
    if (string_view d = apply_visitor(string_resolver_visitor{}, description_); !d.empty()) {
        ss << " "sv << d;
    }
    return ss.str();
}

}
