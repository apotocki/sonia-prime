//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ast_terms.hpp"

namespace sonia::lang::bang {

struct expression_location_visitor : static_visitor<lex::resource_location const&>
{
    expression_location_visitor() = default;

    inline result_type operator()(variable_identifier const& v) const noexcept { return v.name.location; }
    
    //inline result_type operator()(bang_fn_type_t const& b) const noexcept
    //{
    //    return this->operator()(b.arg);
    //}

    //inline result_type operator()(index_expression_t const& ie) const noexcept
    //{
    //    return apply_visitor(*this, ie.base);
    //}

    inline result_type operator()(bang_union_t const& bu) const noexcept
    {
        return apply_visitor(*this, bu.members.front());
    }

    inline result_type operator()(not_empty_expression_t const& me) const noexcept { return apply_visitor(*this, me.value); }
    inline result_type operator()(member_expression_t const& me) const noexcept { return me.start(); }
    //inline result_type operator()(lambda_t const& le) const noexcept { return le.location(); }

    //inline result_type operator()(unary_expression_t const& ue) const noexcept { return ue.start(); }
    //inline result_type operator()(binary_expression_t const& be) const noexcept { return be.start(); }

    //inline result_type operator()(expression_vector_t const& v) const noexcept { return v.start; }
    //inline result_type operator()(function_call_t const& f) const noexcept { return f.start(); }

    template <typename T>
    inline lex::resource_location const& operator()(T const& e) const
    {
        if constexpr (requires{ std::declval<T const&>().location(); }) {
            return e.location();
        } else {
            return e.location;
        }
    }
};

lex::resource_location const& get_start_location(syntax_expression_t const& e)
{
    return apply_visitor(expression_location_visitor{}, e);
}

}
