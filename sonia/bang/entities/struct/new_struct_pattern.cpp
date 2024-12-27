//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "new_struct_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors.hpp"

#include "struct_entity.hpp"

//#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> new_struct_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    identifier tpid = ctx.u().get_type_parameter_identifier();

    struct_entity const* pse = nullptr;
    // looking for '__type' parameter
    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        if (pargname && pargname->value == tpid) {
            ct_expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            entity const& some_entity = ctx.u().eregistry().get(*res);
            pse = dynamic_cast<struct_entity const*>(&some_entity);
            if (!pse) return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected a structure"sv, pargname->value));
            break;
        }
    }
    if (!pse) {
        return std::unexpected(make_error<basic_general_error>(lex::resource_location{}, "unmatched parameter"sv, tpid));
    }
    THROW_NOT_IMPLEMENTED_ERROR("new_struct_pattern::try_match");
}

error_storage new_struct_pattern::apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("new_struct_pattern::apply");
}

std::expected<entity_identifier, error_storage> new_struct_pattern::const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("new_struct_pattern::const_apply");
}

std::ostream& new_struct_pattern::print(unit const&, std::ostream& s) const
{
    return s << "new structure";
}

}
