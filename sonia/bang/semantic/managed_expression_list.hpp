//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/semantic.hpp"

namespace sonia::lang::bang { class unit; }

namespace sonia::lang::bang::semantic {

class managed_expression_list : public expression_list_t
{
    unit & u_;

public:
    inline explicit managed_expression_list(unit & u) noexcept : u_{ u } {}
    managed_expression_list(managed_expression_list const&) = delete;
    managed_expression_list& operator= (managed_expression_list const&) = delete;

    managed_expression_list(managed_expression_list &&) = default;
    managed_expression_list& operator= (managed_expression_list &&) = default;

    void clear(expression_list_t&) noexcept;

    ~managed_expression_list();
};

}
