//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "type_mismatch_error.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

general_error::string_t type_mismatch_error::description(unit const& u) const noexcept
{
    std::ostringstream ss;
    ss << "type mismatch error, expected: "sv;
    apply_visitor(make_functional_visitor<void>([&u, &ss](auto const& val) {
        if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::string> || 
            std::is_same_v<std::decay_t<decltype(val)>, string_view>) {
            ss << val;
        } else {
            ss << u.print(val);
        }
    }), expected_);
        
    return ss.str();
}

}
