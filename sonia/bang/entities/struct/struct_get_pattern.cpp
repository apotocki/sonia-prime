//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_get_pattern.hpp"
#include "struct_entity.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

//class struct_get_match_descriptor : public functional_match_descriptor
//{
//    size_t property_index_;
//    size_t fields_count_;
//
//public:
//    using functional_match_descriptor::functional_match_descriptor;
//
//    inline size_t property_index() const noexcept { return property_index_; }
//    inline size_t fields_count() const noexcept { return fields_count_; }
//
//    inline void set_property_index(size_t index) noexcept { property_index_ = index; }
//    inline void set_fields_count(size_t index) noexcept { fields_count_ = index; }
//};

/**
    inline fn::get(self: $T @structure, property: const __identifier @has_property($T)) =>
        get(self: as_tuple(self), property : property);
*/

std::expected<functional_match_descriptor_ptr, error_storage> struct_get_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    identifier slfid = u.get(builtin_id::self);
    identifier propid = u.get(builtin_id::property);
    
    struct_entity const* pse = nullptr;
    entity const* ppname = nullptr;
    shared_ptr<tuple_get_match_descriptor> pmd;
    auto estate = ctx.expressions_state();
    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        if (!pargname) {
            auto const& argexpr = arg.value();
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
        if (pargname->value == slfid && !pse) {
            pmd = make_shared<tuple_get_match_descriptor>(u);
            ctx.push_chain(pmd->call_expressions);
            auto last_expr_it = ctx.expressions().last();
            expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            if (ctx.context_type) {
                entity const& some_entity = ctx.u().eregistry_get(ctx.context_type);
                pse = dynamic_cast<struct_entity const*>(&some_entity);
                if (pse) {
                    pmd->get_match_result(pargname->value).append_result(ctx.context_type, last_expr_it, ctx.expressions());
                    continue;
                }
            }
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument type mismatch: a structure was expected."sv, pargname->value));
            //return std::unexpected(make_error<type_mismatch_error>(pargname->location, ctx.context_type, u.get(builtin_qnid::tuple)));
        } else if (pargname->value == propid && !ppname) {
            ct_expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            ppname = &u.eregistry_get(*res);
        } else {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
        }
    }

    if (!pse) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter: `self`"sv));
    } else if (!ppname) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter: `property`"sv));
    }

    auto uteid = pse->underlying_tuple_eid(ctx);
    if (!uteid) return std::unexpected(std::move(uteid.error()));
    entity const& utplent = u.eregistry_get(*uteid);
    
    return check_match(std::move(pmd), call, utplent, *ppname);

    //entity_signature const* ptplsig = utplent.signature();
    //BOOST_ASSERT(ptplsig);

    //size_t index;
    //field_descriptor const* fd;
    //if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(ppname); pie) {
    //    fd = ptplsig->find_field(pie->value(), &index);
    //} else if (integer_literal_entity const* pile = dynamic_cast<integer_literal_entity const*>(ppname); pile) {
    //    fd = ptplsig->find_field((size_t)pile->value(), &index);
    //} else {
    //    return std::unexpected(make_error<basic_general_error>(ppname->location(), "argument mismatch, expected an identifier or index"sv, ppname->id()));
    //}
    //
    //if (!fd) {
    //    return std::unexpected(make_error<basic_general_error>(call.location(), "no such field"sv, ppname->id()));
    //}

    //pmd->set_property_index(index);
    //pmd->set_fields_count(ptplsig->field_count());
    //BOOST_ASSERT(ptplsig->field_count() > 1 || !index);

    //pmd->result = *fd;
    //return pmd;
}

//error_storage struct_get_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
//{
//    unit& u = ctx.u();
//
//    // push call expressions in the right order
//    semantic::expression_list_t& exprs = ctx.expressions();
//
//    // only one named argument is expected
//    md.for_each_named_match([&exprs, &md](identifier name, parameter_match_result const& mr) {
//        for (auto rng : mr.expressions) {
//            ++rng.second;
//            exprs.splice_back(md.call_expressions, rng.first, rng.second);
//        }
//    });
//
//    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered
//    
//    struct_get_match_descriptor& smd = static_cast<struct_get_match_descriptor&>(md);
//    if (smd.fields_count() > 1) {
//        u.push_back_expression(exprs, semantic::push_value{ smd.property_index() });
//        u.push_back_expression(exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));
//    }
//    
//    ctx.context_type = md.result.entity_id();
//    return {};
//}

}
