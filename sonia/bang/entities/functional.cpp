//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "functional.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"

#include "sonia/bang/entities/prepared_call.hpp"

#include "sonia/bang/errors/function_call_match_error.hpp"
#include "sonia/bang/errors/pattern_match_error.hpp"
#include "sonia/bang/errors/circular_dependency_error.hpp"
#include "sonia/bang/errors/identifier_redefinition_error.hpp"

namespace sonia::lang::bang {

void functional_binding_set::reset() noexcept
{
    binding_.clear();
    binding_names_.clear();
}

functional_binding::value_type const* functional_binding_set::lookup(identifier id) const noexcept
{
    return lookup(id, nullptr);
}

functional_binding::value_type const* functional_binding_set::lookup(identifier id, lex::resource_location const** ppl) const noexcept
{
    auto it = std::lower_bound(binding_names_.begin(), binding_names_.end(), id);
    if (it == binding_names_.end() || *it != id) return nullptr;
    if (ppl) *ppl = &binding_locations_[it - binding_names_.begin()];
    return &binding_[it - binding_names_.begin()];
}

functional_binding::value_type& functional_binding_set::emplace_back(annotated_identifier id, value_type value)
{
    auto it = std::lower_bound(binding_names_.begin(), binding_names_.end(), id.value);
    if (it == binding_names_.end() || *it != id.value) {
        it = binding_names_.emplace(it, id.value);
        auto pos = it - binding_names_.begin();
        binding_locations_.emplace(binding_locations_.begin() + pos, id.location);
        return *binding_.emplace(binding_.begin() + pos, std::move(value));
    } else {
        THROW_INTERNAL_ERROR("functional_binding_set::emplace_back duplicate binding");
        //for (++it; it != binding_names_.end() && *it == id; ++it);
        //binding_.emplace(binding_.begin() + (it - binding_names_.begin()), std::move(value));
    }
}

//void parameter_match_result::append_result(entity_identifier r, se_iterator before_start_it, semantic::expression_list_t& exprs)
//{
//    results.emplace_back(r, semantic::expression_span{});
//    if (exprs) {
//        auto& el = results.back().second;
//        if (before_start_it == exprs.end()) {
//            el = exprs; // semantic::expression_span{ exprs };
//        } else {
//            ++before_start_it;
//            if (before_start_it != exprs.end()) {
//                //semantic::expression_entry* b = static_cast<semantic::expression_entry*>(&*before_start_it.base());
//                //semantic::expression_entry* e = static_cast<semantic::expression_entry*>(&exprs.back_entry());
//                semantic::expression_entry_type* b = &*before_start_it.base();
//                semantic::expression_entry_type* e = &exprs.back_entry();
//                el = semantic::expression_span{ b, e };
//                //el = semantic::expression_span{ &*before_start_it.base(), &exprs.back_entry() };
//            }
//        }
//    }
//}

void parameter_match_result::append_result(entity_identifier type, semantic::expression_span sp)
{
    results.emplace_back(syntax_expression_result_t{ .expressions = std::move(sp), .value_or_type = type, .is_const_result = false });
}

void parameter_match_result::append_const_result(entity_identifier value, semantic::expression_span sp)
{
    results.emplace_back(syntax_expression_result_t{ .expressions = std::move(sp), .value_or_type = value, .is_const_result = true });
}

//void parameter_match_result::set_constexpr(bool ce_val)
//{
//    if (mod == (uint8_t)modifier::undefined) {
//        mod = (uint8_t)(ce_val ? modifier::is_constexpr : modifier::is_expr);
//    } else if (ce_val) {
//        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_expr));
//        mod |= (uint8_t)modifier::is_constexpr;
//    } else {
//        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_constexpr));
//        mod |= (uint8_t)modifier::is_expr;
//    }
//}
//
//void parameter_match_result::set_variadic(bool v_val)
//{
//    if (mod == (uint8_t)modifier::undefined) {
//        mod = (uint8_t)(v_val ? modifier::is_variadic : modifier::is_uniadic);
//    } else if (v_val) {
//        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_uniadic));
//        mod |= (uint8_t)modifier::is_variadic;
//    } else {
//        BOOST_ASSERT(!(mod & (uint8_t)modifier::is_variadic));
//        mod |= (uint8_t)modifier::is_uniadic;
//    }
//}

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

semantic::expression_span functional_match_descriptor::merge_void_spans(semantic::expression_list_t& el) noexcept
{
    semantic::expression_span result;
    auto bit = void_spans.begin(), eit = void_spans.end();
    if (bit == eit) return result;
    result = *bit; ++bit;
    for (; bit != eit; ++bit) {
        result = el.concat(result, *bit);
    }
    return result;
}

parameter_match_result& functional_match_descriptor::get_match_result(identifier param_name)
{
    auto it = named_matches_.find(param_name);
    if (it == named_matches_.end()) {
        pmrs_.emplace_back(param_name, nullptr);
        named_matches_.emplace_hint(it, &pmrs_.back());
        return get<1>(pmrs_.back());
    }
    return get<1>(**it);
}

parameter_match_result& functional_match_descriptor::get_match_result(size_t pos)
{
    while (positional_matches_.size() <= pos) {
        pmrs_.emplace_back();
        positional_matches_.push_back(&get<1>(pmrs_.back()));
    }
    return *positional_matches_[pos];
}

entity_signature functional_match_descriptor::build_signature(unit & u, qname_identifier name)
{
    entity_signature signature{ name };
    for (mr_pair_t const* pnm : named_matches_) {
        for (auto const& ser : get<1>(*pnm).results) {
            signature.emplace_back(get<0>(*pnm), ser.value_or_type, ser.is_const_result);
        }
    }
    //size_t argnum = 0;
    for (auto pmr : positional_matches_) {
        for (auto const& ser : pmr->results) {
            signature.emplace_back(ser.value_or_type, ser.is_const_result);
        }
    }
    if (result.entity_id()) {
        signature.result = field_descriptor{ result };
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
    bindings.reset();
    //call_expressions.clear();
    result = field_descriptor{};
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

entity_identifier functional::default_entity(fn_compiler_context& ctx) const
{
    shared_ptr<entity_resolver> resolver;
    {
        lock_guard lock{ default_entity_mtx_ };
        if (auto* p = get<annotated_entity_identifier>(&default_entity_); p) return p->value; // if not defined, return empty entity_identifier here
        resolver = get<shared_ptr<entity_resolver>>(default_entity_);
    }
    
    compiler_task_tracer::task_guard tg = ctx.try_lock_task(qname_task_id{ id_ });
    if (!tg) {
        throw circular_dependency_error(make_error<basic_general_error>(resolver->location(), "resolving name"sv, id_));
    }

    {
        lock_guard lock{ default_entity_mtx_ };
        if (auto * p = get<annotated_entity_identifier>(&default_entity_); p) return p->value;
    }
        
    auto deid = resolver->const_resolve(ctx);
    if (!deid) deid.error()->rethrow(ctx.u());
    {
        lock_guard lock{ default_entity_mtx_ };
        default_entity_ = annotated_entity_identifier{ *deid, resolver->location() };
    }

    return *deid;
}

void functional::set_default_entity(annotated_entity_identifier e)
{
    lock_guard lock{ default_entity_mtx_ };
    if (auto* p = get<annotated_entity_identifier>(&default_entity_); !p || *p) {
        throw identifier_redefinition_error(
            annotated_qname_identifier{ id(), e.location },
            p ? p->location : get<shared_ptr<entity_resolver>>(default_entity_)->location());
    }
    default_entity_ = std::move(e);
}

void functional::set_default_entity(shared_ptr<entity_resolver> e)
{
    lock_guard lock{ default_entity_mtx_ };
    if (auto* p = get<annotated_entity_identifier>(&default_entity_); !p || *p) {
        throw identifier_redefinition_error(
            annotated_qname_identifier{ id(), e->location() },
            p ? p->location : get<shared_ptr<entity_resolver>>(default_entity_)->location());
    }
    default_entity_ = std::move(e);
}

struct expression_stack_checker
{
#ifndef NDEBUG
    size_t branch_count;
    size_t brach_size;

    inline expression_stack_checker(fn_compiler_context& ctx) noexcept : branch_count{ ctx.expressions_branch() }, brach_size{ ctx.expressions().size() } {}
    
    inline bool check(fn_compiler_context& ctx) noexcept
    {
        return branch_count == ctx.expressions_branch() && brach_size == ctx.expressions().size();
    }
#else
    inline expression_stack_checker(fn_compiler_context&) noexcept {}
    inline bool check(fn_compiler_context&) noexcept { return true; }
#endif
};

std::expected<functional::match, error_storage> functional::find(fn_compiler_context& ctx, pure_call_t const& call, semantic::expression_list_t& ael, annotated_entity_identifier const& expected_result) const
{
    alt_error err;
    mp::decimal major_weight = 0;
    int minor_weight = 0;
    using alternative_t = std::pair<pattern const*, functional_match_descriptor_ptr>;
    small_vector<alternative_t, 2> alternatives;

    expression_stack_checker expr_stack_state{ ctx };

    prepared_call pcall{ call, ael };
    if (auto err = pcall.prepare(ctx); err) return std::unexpected(std::move(err));

    for (auto const& p : patterns_) {
        auto cmp = major_weight <=> p->get_weight();
        if (cmp == std::strong_ordering::greater) continue;

        auto match_descriptor = p->try_match(ctx, pcall, expected_result);

        BOOST_ASSERT(expr_stack_state.check(ctx));

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
                minor_weight = match_weight;
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
            return std::unexpected(make_error<function_call_match_error>(annotated_qname_identifier{ id_, call.location }, nullptr));
        }
        return std::unexpected(make_error<alt_error>(std::move(err)));
    }
    if (alternatives.size() > 1) {
        std::vector<ambiguity_error::alternative> as;
        for (alternative_t const& a : alternatives) {
            as.emplace_back(get<0>(a)->location(), get<1>(a)->build_signature(ctx.u(), id_));
        }
        return std::unexpected(make_error<ambiguity_error>(annotated_qname_identifier{ id_, call.location }, std::move(as)));
    }
    auto [ptrn, md] = alternatives.front();
    return match{ ptrn, ael, std::move(md) };
}

//error_storage functional::pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
//{
//    auto res = apply(ctx, md);
//    if (!res) return std::move(res.error());
//
//    return apply_visitor(make_functional_visitor<error_storage>([&ctx, &md](auto && eid_or_el) -> error_storage {
//        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_el)>, entity_identifier>) {
//            ctx.append_expression(semantic::push_value{ eid_or_el });
//            entity const& e = ctx.u().eregistry_get(eid_or_el);
//            //if (e.get_type() == ctx.u().get(builtin_eid::typename_)) {
//            //  ctx.context_type = eid_or_el;
//            //} else {
//                ctx.context_type = e.get_type();
//            //}
//        } else {
//            ctx.expressions().splice_back(eid_or_el);
//        }
//        return error_storage{};
//    }), *res);
//}

//std::expected<entity_identifier, error_storage> functional::pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
//{
//    auto res = apply(ctx, md);
//    if (!res) return std::unexpected(res.error());
//    using result_t = std::expected<entity_identifier, error_storage>;
//    return apply_visitor(make_functional_visitor<result_t>([&ctx](auto & v) -> result_t {
//        if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
//            return v;
//        } else {
//            if (ctx.context_type == ctx.u().get(builtin_eid::void_)) {
//                BOOST_ASSERT(v.empty());
//                return ctx.context_type;
//            }
//            THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor::operator()(opt_named_syntax_expression_list_t const&)");
//        }
//    }), *res);
//}

std::expected<syntax_expression_t const*, error_storage> try_match_single_unnamed(fn_compiler_context& ctx, prepared_call const& call)
{
    unit& u = ctx.u();
    syntax_expression_t const* matched_arg = nullptr;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        if (pargname) {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, unexpected argument"sv, *pargname));
        }
        syntax_expression_t const& e = arg.value();
        if (matched_arg) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(e), "argument mismatch, unexpected argument"sv, e));
        }
        matched_arg = &e;
    }

    if (!matched_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv));
    }

    return matched_arg;
}

}
