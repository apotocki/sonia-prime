//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "pattern_match_error.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

general_error::string_t pattern_match_error::object(unit const& u) const noexcept
{
    std::ostringstream ss;
    pattern_.print(u, ss);
    return ss.str();
}

general_error::string_t pattern_match_error::description(unit const& u) const noexcept
{
    std::ostringstream ss;
    //if (signature_) {
    //    ss << "can't match the function signature: "sv;
    //    ss << u.print(signature_->fn_type);
    //} else {
    ss << "can't match the call";
    //}
    if (reason_) {
        ss << ", caused by: \n" << u.print(*reason_);
    }
    return ss.str();
}

}
