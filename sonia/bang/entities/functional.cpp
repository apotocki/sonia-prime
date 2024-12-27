//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "functional.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/ellipsis/pack_entity.hpp"

#include "sonia/bang/errors/function_call_match_error.hpp"
#include "sonia/bang/errors/pattern_match_error.hpp"

namespace sonia::lang::bang {

void parameter_match_result::append_result(bool variadic, entity_identifier r, se_cont_iterator before_start_it, semantic::expression_list_t& exprs)
{
    set_variadic(variadic);
    set_constexpr(false);
    result.emplace_back(r);

    if (exprs) {
        if (before_start_it == exprs.end()) {
            before_start_it = exprs.begin();
        } else {
            ++before_start_it;
        }
        expressions.emplace_back(before_start_it, exprs.last());
    }
}

void parameter_match_result::append_result(bool variadic, entity_identifier r)
{
    set_variadic(variadic);
    set_constexpr(true);
    result.emplace_back(r);
}

void parameter_match_result::set_constexpr(bool ce_val)
{
    if (mod == (uint8_t)modifier::undefined) {
        mod = (uint8_t)(ce_val ? modifier::is_constexpr : modifier::is_expr);
    } else if (ce_val) {
        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_expr));
        mod |= (uint8_t)modifier::is_constexpr;
    } else {
        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_constexpr));
        mod |= (uint8_t)modifier::is_expr;
    }
}

void parameter_match_result::set_variadic(bool v_val)
{
    if (mod == (uint8_t)modifier::undefined) {
        mod = (uint8_t)(v_val ? modifier::is_variadic : modifier::is_uniadic);
    } else if (v_val) {
        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_uniadic));
        mod |= (uint8_t)modifier::is_variadic;
    } else {
        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_variadic));
        mod |= (uint8_t)modifier::is_uniadic;
    }
}

//void functional_match_descriptor::do_prepare_range(se_cont_iterator& it_before, semantic::managed_expression_list const& exprs, optional<se_rng_t> & rng)
//{
//    if (!exprs) {
//        rng.emplace(exprs.begin(), exprs.end());
//    } else {
//        rng.emplace(it_before, exprs.last());
//        if (rng->first != exprs.end()) ++rng->first;
//        else rng->first = exprs.begin();
//        it_before = rng->second;
//    }
//}

parameter_match_result& functional_match_descriptor::get_match_result(identifier param_name)
{
    auto it = named_matches_.find(param_name);
    if (it == named_matches_.end()) {
        pmrs_.emplace_back();
        named_matches_.emplace_hint(it, param_name, &pmrs_.back());
        return pmrs_.back();
    }
    return *it->second;
}

parameter_match_result& functional_match_descriptor::get_match_result(size_t pos)
{
    while (positional_matches_.size() <= pos) {
        pmrs_.emplace_back();
        positional_matches_.push_back(&pmrs_.back());
    }
    return *positional_matches_[pos];
}

entity_signature functional_match_descriptor::build_signature(unit & u, qname_identifier name)
{
    entity_signature signature{ name };
    for (auto [nm, pmr] : named_matches_) {
        bool is_const = pmr->is_constexpr();
        for (entity_identifier eid : pmr->result) {
            signature.set(nm, field_descriptor{ eid, is_const });
        }
    }
    size_t argnum = 0;
    for (auto pmr : positional_matches_) {
        bool is_const = pmr->is_constexpr();
        for (entity_identifier eid : pmr->result) {
            signature.set(argnum++, field_descriptor{ eid, is_const });
        }
    }
    if (result) {
        signature.set_result(field_descriptor{ result, false });
    }
    return signature;
    
    //pmd->signature.set(pargname->value, field_descriptor{ match->first, false });
    //pmd->signature.set(start_matcher_it - matchers_.begin(), field_descriptor{ pmatch->first, false });
}

void functional_match_descriptor::reset() noexcept
{
    positional_matches_.clear();
    named_matches_.clear();
    pmrs_.clear();
    bindings.reset(0);
    call_expressions.clear();
    result = {};
}

//void functional_match_descriptor::push_named_argument_expressions(identifier argid, se_cont_iterator& it_before, semantic::managed_expression_list const& exprs)
//{
//    optional<se_rng_t> rng;
//    do_prepare_range(it_before, exprs, rng);
//    
//    auto nait = std::lower_bound(named_arguments.begin(), named_arguments.end(), argid, [](auto const& pair, identifier id) { return pair.first < id; });
//    if (nait == named_arguments.end() || nait->first != argid) {
//        named_arguments.emplace(nait, argid, std::initializer_list{ *rng });
//    } else {
//        nait->second.emplace_back(std::move(*rng));
//    }
//}

//void functional_match_descriptor::push_unnamed_argument_expressions(size_t argnum, se_cont_iterator& it_before, semantic::managed_expression_list const& exprs)
//{
//    optional<se_rng_t> rng;
//    do_prepare_range(it_before, exprs, rng);
//    
//    if (unnamed_arguments.size() <= argnum) {
//        unnamed_arguments.resize(argnum + 1);
//    }
//    unnamed_arguments[argnum].emplace_back(std::move(*rng));
//}
//
//span<const functional_match_descriptor::se_rng_t> functional_match_descriptor::get_named_arguments(identifier argid) const noexcept
//{
//    auto nait = std::lower_bound(named_arguments.begin(), named_arguments.end(), argid, [](auto const& pair, identifier id) { return pair.first < id; });
//    if (nait == named_arguments.end() || nait->first != argid) [[unlikely]] return {};
//    return nait->second;
//}
//
//span<const functional_match_descriptor::se_rng_t> functional_match_descriptor::get_position_arguments(size_t pos) const noexcept
//{
//    if (unnamed_arguments.size() <= pos) [[unlikely]] return {};
//    return unnamed_arguments[pos];
//}

function_descriptor::named_field const* function_descriptor::find_named_field(identifier name) const
{
    auto it = std::lower_bound(nfields_.begin(), nfields_.end(), name,
        [](named_field const& l, identifier r) { return l.ename.value < r; });
    if (it != nfields_.end() && it->ename.value == name) return &*it;
    return nullptr;
}

std::expected<functional::match, error_storage> functional::find(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const& expected_result) const
{
    alt_error err;
    mp::decimal major_weight = 0;
    int minor_weight = 0;
    using alternative_t = std::pair<pattern const*, functional_match_descriptor_ptr>;
    small_vector<alternative_t, 2> alternatives;

    for (auto const& p : patterns_) {
        auto cmp = major_weight <=> p->get_weight();
        if (cmp == std::strong_ordering::greater) continue;
        //auto temp_pmd = make_shared<functional_match_descriptor>{ ctx.u() };
        //temp_md.signature = entity_signature{ id_ };
        //ctx.push_chain(temp_md.call_expressions);
        auto match_descriptor = p->try_match(ctx, call, expected_result);
        if (!match_descriptor) {
            if (alternatives.empty()) { // no need to store errors if a match exists
                //err.alternatives.emplace_back(
                //    make_error<function_call_match_error>(annotated_qname_identifier{ id_, call.location() }, sig, std::move(optres))
                //);
                err.alternatives.emplace_back(make_error<pattern_match_error>(*p, std::move(match_descriptor.error())));
            }
        } else {
            int match_weight = (**match_descriptor).weight;
            if (cmp == std::strong_ordering::less || minor_weight < match_weight) {
                major_weight = p->get_weight();
                minor_weight = (**match_descriptor).weight;
                alternatives.clear();
                alternatives.emplace_back(p.get(), std::move(*match_descriptor));
            } else if (minor_weight == match_weight) { // cmp == std::strong_ordering::equal
                alternatives.emplace_back(p.get(), std::move(*match_descriptor));
            } // else skip less weighted alternatives
        }
    }

    if (alternatives.empty()) {
        if (err.alternatives.size() == 1) {
            return std::unexpected(std::move(err.alternatives.front()));
        }
        if (err.alternatives.empty()) {
            return std::unexpected(make_error<function_call_match_error>(annotated_qname_identifier{ id_, call.location() }, nullptr));
        }
        return std::unexpected(make_error<alt_error>(std::move(err)));
    }
    if (alternatives.size() > 1) {
        std::vector<ambiguity_error::alternative> as;
        for (alternative_t const& a : alternatives) {
            as.emplace_back(get<0>(a)->location(), get<1>(a)->build_signature(ctx.u(), id_));
        }
        return std::unexpected(make_error<ambiguity_error>(annotated_qname_identifier{ id_, call.location() }, std::move(as)));
    }
    auto [ptrn, md] = alternatives.front();
    return match{ id_, ptrn, std::move(md) };
}

std::expected<functional_match_descriptor_ptr, error_storage> fieldset_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::is_matched");
#if 0
    for (auto const& tpl : call.named_args) { // {argname, expr, exprloc}
        THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::is_matched, named argument");
    }
    fieldset_t const& fs = get_fieldset();
    auto psp = fs.positioned_fields();
    auto posarg = psp.begin();

    for (auto expr_it = call.positioned_args.begin(), expr_eit = call.positioned_args.end(); expr_it != expr_eit;) {
        if (posarg == psp.end()) {
            return make_error<basic_general_error>(get_start_location(*expr_it), "positioned argument mismatch"sv);
        }
        entity_identifier param_type = posarg->constraint;
        entity const& param_entity = ctx.u().eregistry().get(param_type);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            uint64_t pack_size = 0;
            //ctx.append_expression(semantic::push_value{ uint64_t{pack_size} });
            //auto pack_size_expr_pointer = ctx.current_expressions_pointer();
            for (;;) {
                expression_visitor evis{ ctx, expected_result_t{ pent->element_type(), get_start_location(*expr_it) } };
                if (auto opterr = apply_visitor(evis, *expr_it); opterr) {
                    ++posarg;
                    if (posarg == psp.end()) return std::move(opterr);
                    break;
                }
                ++pack_size;
                if (++expr_it == expr_eit) {
                    ++posarg;
                    break;
                }
            }
            ctx.append_expression(semantic::push_value{ uint64_t{pack_size} });
            //ctx.set_expression(pack_size_expr_pointer, semantic::push_value{ uint64_t{pack_size} });
        } else {
            expression_visitor evis{ ctx, expected_result_t{ param_type, get_start_location(*expr_it) } };
            if (auto opterr = apply_visitor(evis, *expr_it); opterr) return std::move(opterr);
            ++expr_it;
            ++posarg;
        }
    }
    while (posarg != psp.end()) {
        entity_identifier param_type = posarg->constraint;
        entity const& param_entity = ctx.u().eregistry().get(param_type);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            ctx.append_expression(semantic::push_value{ uint64_t{0} }); // set empty argument pack
            ++posarg;
            continue;
        }
        return make_error<basic_general_error>(call.location(), "positioned argument mismatch"sv);
    }
    // to do: replace expressions according signature
    return {};
#endif
}

error_storage functional::pattern::apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    auto r = const_apply(ctx, fid, md);
    if (!r) return std::move(r.error());
    ctx.append_expression(semantic::push_value{*r});
    entity const& e = ctx.u().eregistry().get(*r);
    ctx.context_type = e.get_type();
    return {};
}

std::expected<entity_identifier, error_storage> fieldset_pattern::const_apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor&) const
{
    // , std::span<semantic::expression_type> args
    THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::const_apply");
}

}
