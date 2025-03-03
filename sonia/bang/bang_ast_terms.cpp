//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ast_terms.hpp"

namespace sonia::lang::bang {

struct expression_location_visitor : static_visitor<lex::resource_location const&>
{
    expression_location_visitor() = default;

    template <typename T>
    inline result_type operator()(annotated<T> const& ae) const noexcept { return ae.location; }

    inline result_type operator()(context_value const& v) const noexcept { return v.location; }
    inline result_type operator()(variable_identifier const& v) const noexcept { return v.name.location; }
    inline result_type operator()(annotated_qname const& qn) const noexcept { return qn.location; }
    inline result_type operator()(context_identifier const& ce) const noexcept { return ce.start; }
    inline result_type operator()(placeholder const& ph) const noexcept { return ph.location; }
    inline result_type operator()(not_empty_expression_t const& me) const noexcept { return apply_visitor(*this, me.value); }
    inline result_type operator()(member_expression_t const& me) const noexcept { return me.start(); }
    inline result_type operator()(lambda_t const& le) const noexcept { return le.location(); }
    //inline result_type operator()(annotated_entity_identifier const& ee) const noexcept { return ee.location; }

    inline result_type operator()(unary_expression_t const& ue) const noexcept { return ue.start(); }
    inline result_type operator()(binary_expression_t const& be) const noexcept { return be.start(); }

    inline result_type operator()(expression_vector_t const& v) const noexcept { return v.start; }
    inline result_type operator()(function_call_t const& f) const noexcept { return f.start(); }

    inline result_type operator()(opt_named_syntax_expression_list_t const& sl) const noexcept { return sl.location; }

    inline result_type operator()(new_expression_t const& ne) const noexcept { return ne.location; }

    template <typename T>
    inline lex::resource_location const& operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

lex::resource_location const& get_start_location(syntax_expression_t const& e)
{
    return apply_visitor(expression_location_visitor{}, e);
}

}
