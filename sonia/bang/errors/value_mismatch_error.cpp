//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "value_mismatch_error.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

struct value_mismatch_expected_printer_visitor : static_visitor<void>
{
    unit const& u_;
    std::ostream& s_;
    explicit value_mismatch_expected_printer_visitor(unit const& u, std::ostream & s) : u_{ u }, s_ { s } {}

    template <typename T>
    inline void operator()(T const& val) const
    {
        s_ << u_.print(val);
    }
};

general_error::string_t value_mismatch_error::description(unit const& u) const noexcept
{
    std::ostringstream ss;
    ss << "value mismatch error, expected: ";
    apply_visitor(value_mismatch_expected_printer_visitor{ u, ss }, expected_);
    return ss.str();
}

}
