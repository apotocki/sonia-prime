//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/small_vector.hpp"
#include "sonia/bang/ast_terms.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;

class prepared_call
{
public:
    lex::resource_location location;
    small_vector<named_expression_t, 8> args;

    inline prepared_call(lex::resource_location loc) noexcept
        : location{ std::move(loc) }
    {}

    explicit prepared_call(pure_call_t const&);

    error_storage prepare(fn_compiler_context&);
};

}
