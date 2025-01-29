//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "circular_dependency_error.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

void error_printer_visitor::operator()(circular_dependency_error const& err)
{
    bool first = true;
    for (auto const& e : err.circle_items) {
        if (!first) {
            s_ << "\n -------------- \n";
        }
        else { first = false; }
        e->visit(*this);
    }
}

}
