//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "union_bit_or_pattern.hpp"

#include <boost/container/flat_set.hpp>

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

class union_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    boost::container::small_flat_set<entity_identifier, 8> types;
};

error_storage union_bit_or_pattern::accept_argument(std::nullptr_t, functional_match_descriptor_ptr& pmd, arg_context_type& argctx, syntax_expression_result_t& er) const
{
    if (argctx.pargname || !er.is_const_result) return argctx.make_argument_mismatch_error();
 
    fn_compiler_context& ctx = argctx.ctx;
    prepared_call const& call = argctx.call;
    unit& u = ctx.u();

    entity const& arg_entity = get_entity(u, er.value());
    if (arg_entity.get_type() != u.get(builtin_eid::typename_)) {
        return argctx.make_argument_mismatch_error();
    }
    if (!pmd) {
        pmd = make_shared<union_match_descriptor>();
    }
    union_match_descriptor& md = static_cast<union_match_descriptor&>(*pmd);

    entity_signature const* psig = arg_entity.signature();
    if (!psig || psig->name != u.get(builtin_qnid::union_)) {
        md.types.insert(arg_entity.id);
        return {};
    }
    // arg_entity is union, copy all its types
    for (auto const& f : psig->fields()) {
        md.types.insert(f.entity_id());
    }
    return {};
}

std::expected<syntax_expression_result_t, error_storage> union_bit_or_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t&, functional_match_descriptor& md) const
{
    auto& umd = static_cast<union_match_descriptor&>(md);
    return make_result(ctx.u(), ctx.u().make_union_type_entity(umd.types).id);
}

template class generic_pattern_base<union_bit_or_pattern>;

} // namespace sonia::lang::bang
