//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>
#include "sonia/variant.hpp"

#include "sonia/bang/entities/functional.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

namespace sonia::lang::bang {

class basic_fn_pattern;
class genetic_parameter_matcher;

class parameter_type_expression_visitor : public static_visitor<error_storage>
{
    fn_compiler_context& ctx;
    basic_fn_pattern const& pattern;
    genetic_parameter_matcher& matcher;

public:
    explicit parameter_type_expression_visitor(fn_compiler_context& c, basic_fn_pattern const& ptrn, genetic_parameter_matcher& m) //, function_descriptor& fd)
        : ctx{ c }, pattern{ ptrn }, matcher{ m }
    {}

    result_type operator()(annotated_entity_identifier const&);
    result_type operator()(variable_reference const&);
    result_type operator()(bang_parameter_pack_t const&);
    

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("parameter_type_expression_visitor()");
    }

private:
    inline unit& u() const noexcept { return ctx.u(); }
};

}
