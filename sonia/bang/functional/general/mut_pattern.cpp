//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "mut_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

error_storage mut_pattern::accept_argument(std::nullptr_t, functional_match_descriptor_ptr& pmd, arg_context_type& argctx, syntax_expression_result_t& v) const
{
    if (pmd || argctx.pargname)
        return argctx.make_argument_mismatch_error();

    fn_compiler_context& ctx = argctx.ctx;
    prepared_call const& call = argctx.call;
    unit& u = ctx.u();

    signatured_entity const* arg_entity = dynamic_cast<signatured_entity const*>(&get_entity(u, v.value_or_type));
    if (!arg_entity) {
        return argctx.make_argument_mismatch_error();
    }
    pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->result = field_descriptor{ u.make_integer_entity(arg_entity->signature()->positioned_fields_indices().size()).id(), true };
    return {};
}

std::expected<functional_match_descriptor_ptr, error_storage> mut_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    auto call_session = call.new_session();
    size_t arg_index = 0;
    call_session.use(ctx, arg_index, exp);
    return std::unexpected(error_storage{});
}

std::expected<syntax_expression_result_t, error_storage> mut_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t&, functional_match_descriptor& md) const
{
    return make_result(ctx.u(), md.result.entity_id());
}

template generic_pattern_base<mut_pattern>;

}
