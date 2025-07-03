//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "expression_property_visitor.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

namespace sonia::lang::bang {

expression_property_visitor::result_type expression_property_visitor::operator()(variable_reference const& v) const
{
    if (!v.implicit || v.name.value.size() != 1) return expression_analysis_result::this_is_not_pattern();
    identifier paramname = *v.name.value.begin();
    if (auto it = std::lower_bound(parameters.begin(), parameters.end(), paramname); it != parameters.end() && *it == paramname) {
        return expression_analysis_result::this_is_pattern();
    }
    if (expression_variables.find(paramname) != expression_variables.end()) {
        return expression_analysis_result::this_is_pattern();
    }
    auto res = ctx.lookup_entity(v.name);
    if (entity_identifier const* pent = get<entity_identifier>(&res); pent && !*pent) {
        // this is a new expression variable
        expression_variables.insert(paramname);
        return expression_analysis_result::this_is_pattern();
    }
    return expression_analysis_result::this_is_not_pattern();
    
}

expression_property_visitor::result_type expression_property_visitor::operator()(pure_call_t const& call) const
{
    result_type rt = expression_analysis_result::this_is_not_pattern();
    bool can_be_a_pattern = false;
    for (auto const& arg : call.args) {
        auto res = apply_visitor(*this, arg.value());
        if (res.is_pattern) {
            can_be_a_pattern = true;
        } else if (res.use_parameters) { // the argument is not a pattern and uses parameters => the call can not be a pattern
            return res;
        } // else : the argument is not a pattern and does not use parameters => we can continue checking
        rt.use_parameters = rt.use_parameters | res.use_parameters;
    }
    rt.is_pattern = can_be_a_pattern;
    return rt;
}

expression_property_visitor::result_type expression_property_visitor::operator()(member_expression_t const& me) const
{
    // member expressions are not patterns
    result_type rt = expression_analysis_result::this_is_not_pattern();
    rt.use_parameters = apply_visitor(*this, me.object).use_parameters;
    if (!rt.use_parameters) {
        rt.use_parameters = apply_visitor(*this, me.property).use_parameters;
    }
    return rt;
}

expression_property_visitor::result_type expression_property_visitor::operator()(unary_expression_t const& ue) const
{
    result_type rt = (*this)(static_cast<pure_call_t const&>(ue));
    rt.is_pattern = false;
    return rt;
}

expression_property_visitor::result_type expression_property_visitor::operator()(binary_expression_t const& be) const
{
    result_type rt = (*this)(static_cast<pure_call_t const&>(be));
    rt.is_pattern = false;
    return rt;
}

expression_property_visitor::result_type expression_property_visitor::operator()(function_call_t const& fe) const
{
    // function calls can be patterns
    result_type rt = (*this)(static_cast<pure_call_t const&>(fe));
    if (!rt.is_pattern && rt.use_parameters) return rt;
    result_type fnobjrt = apply_visitor(*this, fe.fn_object);
    if (!fnobjrt.is_pattern && fnobjrt.use_parameters) return fnobjrt;
    rt.use_parameters = rt.use_parameters | fnobjrt.use_parameters;
    rt.is_pattern = rt.is_pattern | fnobjrt.is_pattern;
    return rt;
}

}
