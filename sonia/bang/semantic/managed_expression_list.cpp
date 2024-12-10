//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "managed_expression_list.hpp"

#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang::semantic {

void managed_expression_list::clear(expression_list_t & l) noexcept
{
    while (l) {
        entry_type&& e = l.pop_back();
        //GLOBAL_LOG_INFO() << "removing:\n" << u_.print(e.value);
        if (semantic::expression_list_t* pl = get<semantic::expression_list_t>(&e.value); pl) {
            l.splice_back(*pl, pl->begin(), pl->end());
        }
        u_.release(std::move(e));
    }
}

managed_expression_list::~managed_expression_list()
{
    clear(*this);
}

}
