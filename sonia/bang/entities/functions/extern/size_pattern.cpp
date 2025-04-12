//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "size_pattern.hpp"

#include "sonia/bang/entities/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

namespace sonia::lang::bang {

template <typename ArgApplicationT>
error_storage size_pattern::accept_argument(std::nullptr_t, functional_match_descriptor_ptr& pmd, arg_context_type& argctx, ArgApplicationT& v) const
{
    if constexpr (!std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
        return argctx.make_argument_mismatch_error();
    } else {
        if (pmd || argctx.pargname) return argctx.make_argument_mismatch_error();
        fn_compiler_context& ctx = argctx.ctx;
        prepared_call const& call = argctx.call;
        unit& u = ctx.u();

        signatured_entity const* arg_entity = dynamic_cast<signatured_entity const*>(&u.eregistry_get(v));
        if (!arg_entity) {
            return argctx.make_argument_mismatch_error();
        }
        pmd = make_shared<functional_match_descriptor>(u, call.location);
        pmd->result = field_descriptor{ u.make_integer_entity(arg_entity->signature()->positioned_fields_indices().size()).id(), true };
        return {};
    }
}

std::expected<syntax_expression_result_t, error_storage> size_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    return make_result(ctx.u(), md.result.entity_id());
}

template generic_pattern_base<size_pattern>;

}
