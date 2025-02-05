//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_set_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

class tuple_set_match_descriptor : public functional_match_descriptor
{
    size_t property_index_;

public:
    using functional_match_descriptor::functional_match_descriptor;

    inline size_t property_index() const noexcept { return property_index_; }
    inline void set_property_index(size_t index) noexcept { property_index_ = index; }
};

/**
    inline fn::set(self: tuple(), property: const __identifier @has_property($T), _) -> typeof(self) => implementation defined
*/

std::expected<functional_match_descriptor_ptr, error_storage> tuple_set_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    identifier slfid = u.get(builtin_id::self);
    identifier propid = u.get(builtin_id::property);
    
    entity const* pte = nullptr;
    entity const* ppname = nullptr;
    syntax_expression_t const* pvalue = nullptr;
    shared_ptr<tuple_set_match_descriptor> pmd;
    auto estate = ctx.expressions_state();

    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        if (!pargname && !pvalue) {
            pvalue = &arg.value();
            continue;
        }
        if (pargname->value == slfid && !pte) {
            pmd = make_shared<tuple_set_match_descriptor>(u);
            ctx.push_chain(pmd->call_expressions);
            auto last_expr_it = ctx.expressions().last();
            expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            if (ctx.context_type) {
                entity const& some_entity = ctx.u().eregistry_get(ctx.context_type);
                if (auto psig = some_entity.signature(); psig && psig->name() == u.get(builtin_qnid::tuple)) {
                    pte = &some_entity;
                    pmd->get_match_result(pargname->value).append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
                    continue;
                }
            }
            return std::unexpected(make_error<type_mismatch_error>(pargname->location, ctx.context_type, u.get(builtin_qnid::tuple)));
           //return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected a tuple"sv, pargname->value));
        } else if (pargname->value == propid && !ppname) {
            ct_expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            ppname = &u.eregistry_get(*res);
            /*
            entity const& some_entity = ctx.u().eregistry_get(*res);
            ppname = dynamic_cast<identifier_entity const*>(&some_entity);
            if (ppname) continue;
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected an identifier"sv, pargname->value));
            */
        } else {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, arg.value()));
        }
    }

    if (!pte) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter: `self`"sv));
    } else if (!ppname) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter: `property`"sv));
    } else if (!pvalue) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched value parameter"sv));
    }

    THROW_NOT_IMPLEMENTED_ERROR("tuple_set_pattern::try_match");
#if 0
    entity_signature const* pes = pte->signature();
    BOOST_ASSERT(pes);
    size_t index;
    field_descriptor const* fd;
    if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(ppname); pie) {
        fd = pes->find_field(pie->value(), &index);
    } else if (integer_literal_entity const* pile = dynamic_cast<integer_literal_entity const*>(ppname); pile) {
        fd = pes->find_field((size_t)pile->value(), &index);
    } else {
        return std::unexpected(make_error<basic_general_error>(ppname->location(), "argument mismatch, expected an identifier or index"sv, ppname->id()));
    }
    
    if (!fd) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "no such field"sv, ppname->id()));
    }

    pmd->set_property_index(index);
    BOOST_ASSERT(pes->field_count() > 1 || !index);

    pmd->result = fd->entity_id();
    return pmd;
#endif
}

error_storage tuple_set_pattern::apply(fn_compiler_context& ctx, qname_identifier functional_id, functional_match_descriptor& md) const
{
    unit& u = ctx.u();

    // push call expressions in the right order
    ctx.append_expression(semantic::expression_list_t{});
    semantic::expression_list_t& args = get<semantic::expression_list_t&>(ctx.expressions().back());

    // only one named argument is expected
    md.for_each_named_match([&args, &md](identifier name, parameter_match_result const& mr) {
        for (auto rng : mr.expressions) {
            ++rng.second;
            args.splice_back(md.call_expressions, rng.first, rng.second);
        }
    });

    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered

    if (size_t propindex = static_cast<tuple_set_match_descriptor&>(md).property_index(); propindex) {
        u.push_back_expression(args, semantic::push_value{ propindex });
        u.push_back_expression(args, semantic::invoke_function(u.get(builtin_eid::array_at)));
    }

    ctx.context_type = md.result;
    return {};
}

std::expected<entity_identifier, error_storage> tuple_set_pattern::const_apply(fn_compiler_context& ctx, qname_identifier, functional_match_descriptor& md) const
{
    THROW_NOT_IMPLEMENTED_ERROR("tuple_set_pattern::const_apply");
}

}
