//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "parameter_type_expression_visitor.hpp"
#include "basic_fn_pattern.hpp"

namespace sonia::lang::bang {

#if 0
struct parameter_pack_element_type_visitor : static_visitor<std::expected<pattern_expression_t, error_storage>>
{
    fn_compiler_context& ctx;
    syntax_expression_t const& element_type_expression_;

    explicit parameter_pack_element_type_visitor(fn_compiler_context& c, syntax_expression_t const& et)
        : ctx{ c }
        , element_type_expression_{ et }
    {}

    inline unit& u() const noexcept { return ctx.u(); }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("parameter_pack_element_type_visitor not implemented expression");
    }

    result_type operator()(annotated_qname_identifier const& aqi)
    {
        functional const& fnl = ctx.u().fregistry().resolve(aqi.value);
        entity_identifier element_type = fnl.default_entity();
        if (!element_type) {
            return std::unexpected(make_error<basic_general_error>(
                aqi.location, //get_start_location(element_type_expression_),
                "identifier is not a type"sv));
        }
        return this->operator()(element_type);
    }

    result_type operator()(entity_identifier eid)
    {
        functional& ellipsis_fnl = u().fregistry().resolve(u().get_ellipsis_qname_identifier());
        named_expression_list_t ellipsis_args;
        ellipsis_args.emplace_back(entity_expression{ eid });
        pure_call_t ellipsis_call{ lex::resource_location{}, std::move(ellipsis_args) };
        functional::match_descriptor md;
        auto ptrn = ellipsis_fnl.find(ctx, ellipsis_call, md);
        if (!ptrn.has_value()) {
            return std::unexpected(std::move(ptrn.error()));
        }
        auto r = ptrn.value()->const_apply(ctx, md);
        if (!r.has_value()) {
            return std::unexpected(std::move(r.error()));
        }
        return r.value();
    }
};
#endif

parameter_type_expression_visitor::result_type parameter_type_expression_visitor::operator()(annotated_entity_identifier const& ee)
{
    matcher.constraint = entity_identifier{ ee.value };
    return {};
}

parameter_type_expression_visitor::result_type parameter_type_expression_visitor::operator()(variable_identifier const& var)
{
    if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) { // $? or local_name
        // check for function parameter
        identifier varid = *var.name.value.begin();
        if (optional<size_t> idx = pattern.get_local_variable_index(varid); idx) {
            matcher.constraint = pattern_local_variable{ *idx };
            return {};
        }
        if (var.implicit) {
            return make_error<undeclared_identifier_error>(var.name);
        }
    }

    functional const* f = ctx.lookup_functional(var.name.value);
    if (f) {
        matcher.constraint = annotated_qname_identifier{ f->id(), var.name.location };
        return {};
    }

    return make_error<undeclared_identifier_error>(var.name);
}

parameter_type_expression_visitor::result_type parameter_type_expression_visitor::operator()(bang_parameter_pack_t const& ppack)
{
    result_type err = apply_visitor(*this, ppack.type);
    if (err) return std::move(err);
    matcher.variadic = true;
    return {};
    // parameter_pack_element_type_visitor vis{ ctx, ppack.type };
    ///return apply_visitor(vis, elem.value());
}

}
