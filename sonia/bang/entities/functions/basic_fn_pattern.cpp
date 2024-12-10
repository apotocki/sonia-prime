//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_fn_pattern.hpp"

#include <utility>

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/preliminary_type_visitor.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
#include "sonia/bang/entities/functional_entity.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"
#include "sonia/bang/entities/ellipsis/pack_entity.hpp"

#include "sonia/bang/semantic/managed_expression_list.hpp"

//#include "parameter_type_expression_visitor.hpp"

namespace sonia::lang::bang {

class pattern_task_id : public compiler_task_id
{
    functional::pattern const* pattern_;

public:
    inline explicit pattern_task_id(functional::pattern const& p) : pattern_{ &p } {}

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(pattern_task_id);

    bool equal(compiler_task_id const& tid) const noexcept override
    {
        if (pattern_task_id const* pt = dynamic_cast<pattern_task_id const*>(&tid); pt) {
            return pattern_ == pt->pattern_;
        }
        return false;
    }

    size_t hash() const noexcept override { return std::bit_cast<size_t>(pattern_); }
};

parameter_matcher::parameter_matcher(pattern_parameter_descriptor descr) noexcept
    : descriptor_{ std::move(descr) }
{
    if (auto const& optexpr = descriptor_.constraints.type_expression; optexpr) {
        if (bang_parameter_pack_t const* pp = get<bang_parameter_pack_t>(&*optexpr); pp) {
            variadic = true;
            descriptor_.constraints.type_expression = pp->type;
        }
    }
}

error_storage parameter_matcher::build(fn_compiler_context& ctx, basic_fn_pattern const& ptrn)
{
#if 0
    parameter_type_expression_visitor pvis{ ctx, ptrn, *this };
    if (descriptor_.constraints.type_expression) {
        error_storage err = apply_visitor(pvis, *descriptor_.constraints.type_expression);
        if (err) return std::move(err);
    }
    //for (syntax_expression_t const& ce : descriptor_.constraints.concepts) {
    //    error_storage err = apply_visitor(pvis, ce);
    //    if (err) return std::move(err);
    //    concepts.emplace_back(res.value());
    //}
#endif
    return {};
}

bool parameter_matcher::is_pattern() const noexcept
{
    if (is_expresson) return false;
    // to do: check type expression
    return false;
}

// just to evaluate constraint expression
struct value_type_constraint_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& ctx;
    functional_binding_set& binding;

    value_type_constraint_visitor(fn_compiler_context& c, functional_binding_set& b)
        : ctx{ c }, binding{ b }
    {}

    result_type operator()(annotated_entity_identifier const& aeid) const
    {
        return aeid.value;
    }

    result_type operator()(variable_identifier const& var) const
    {
        if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
            // check for function parameter
            identifier varid = *var.name.value.begin();
            if (optional<span<const entity_identifier>> opteids = binding.lookup(varid); opteids) {
                THROW_NOT_IMPLEMENTED_ERROR("bind variable_identifier");
            }
        }

        functional const* fl = ctx.lookup_functional(var.name.value);
        if (!fl) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        entity_identifier eid = fl->default_entity();
        if (!eid) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        return eid;
        //THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_visitor not implemented expression");
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_visitor not implemented expression");
    }
};

struct value_type_constraint_match_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& ctx;
    syntax_expression_t const& expr;
    functional_binding_set& binding;
    mutable int weight = 0;

    value_type_constraint_match_visitor(fn_compiler_context& c, syntax_expression_t const& e, functional_binding_set& b)
        : ctx{ c }, expr { e }, binding{ b }
    {}

    result_type operator()(annotated_qname_identifier const& aqi) const
    {
        functional const& fnl = ctx.u().fregistry().resolve(aqi.value);
        entity_identifier eid = fnl.default_entity();
        if (eid) {
            return this->operator()(eid, aqi.location);
        }
        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr));
    }

    result_type operator()(entity_identifier const& eid, lex::resource_location eidloc = {}) const
    {
        entity const& param_entity = ctx.u().eregistry().get(eid);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            //syntax_expression_t const& expr = expr_it.next_expression();
            expression_visitor evis{ ctx, { pent->element_type(), get_start_location(expr) } };
            auto res = apply_visitor(evis, expr);
            if (!res) return std::unexpected(std::move(res.error()));
        } else {
            BOOST_ASSERT(eid);
            expression_visitor evis{ ctx, { eid, eidloc } };
            auto res = apply_visitor(evis, expr);
            if (!res) {
                return std::unexpected(std::move(res.error()));
            } else if (res.value()) { --weight; }
        }
        return ctx.context_type;

        //GLOBAL_LOG_INFO() << ctx.u().print(eid);
        //GLOBAL_LOG_INFO() << ctx.u().print(expr);
        //THROW_NOT_IMPLEMENTED_ERROR("arg_resolve_visitor(entity_identifier) not implemented expression");
    }

    result_type operator()(variable_identifier const& var) const
    {
        if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
            // check for function parameter
            identifier varid = *var.name.value.begin();
            if (optional<span<const entity_identifier>> opteids = binding.lookup(varid); opteids) {
                // bind result of expression to bind var
                auto res = apply_visitor(expression_visitor{ ctx }, expr);
                if (!res) return std::unexpected(std::move(res.error()));
                THROW_NOT_IMPLEMENTED_ERROR("bind variable_identifier");
            }
            //if (auto it = fd_.varparams().find(varid); it != fd_.varparams().end()) {
            //    return pattern_variable{ varid };
            //}
        }

        functional const* fl = ctx.lookup_functional(var.name.value);
        if (!fl) return std::unexpected(make_error<undeclared_identifier_error>(var.name));
        
        entity_identifier eid = fl->default_entity();
        if (!eid) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        auto res = apply_visitor(expression_visitor{ ctx, annotated_entity_identifier{eid, var.name.location} }, expr);
        if (!res) return std::unexpected(std::move(res.error()));

        weight -= *res ? 1 : 0;
        return ctx.context_type;
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_match_visitor not implemented expression");
    }
};

std::expected<entity_identifier, error_storage> parameter_matcher::apply_binding(fn_compiler_context& ctx, functional_binding_set& binding) const
{
    switch (descriptor_.modifier) {
        case parameter_constraint_modifier_t::value_type_constraint:
        {
            if (auto const& optexpr = descriptor_.constraints.type_expression; optexpr) {
                value_type_constraint_visitor vtcv{ ctx, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::unexpected(std::move(res.error()));
                return *res;
            } else {
                THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::apply_binding");
            }
            //auto err = apply_visitor(expression_visitor{ ctx }, *descriptor_.constraints.type_expression);
            //if (err) return std::unexpected(std::move(err.error()));
            //return ctx.context_type;
        }
    }
    //}
    THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::apply_binding");
}

variant<std::pair<entity_identifier, int>, parameter_matcher::postpone_t, error_storage> parameter_matcher::try_forward_match(fn_compiler_context& ctx, syntax_expression_t const& se, functional_binding_set& b) const
{
    if (variadic) return postpone_t{};
    auto res = try_match(ctx, se, b);
    if (!res) return std::move(res.error());
    return *res;
}

std::expected<std::pair<entity_identifier, int>, error_storage> parameter_matcher::try_match(fn_compiler_context& ctx, syntax_expression_t const& e, functional_binding_set& binding) const
{
    //if (constraint.which()) { // not empty
        switch (descriptor_.modifier) {
            case parameter_constraint_modifier_t::value_type_constraint:
            {
                if (auto const& optexpr = descriptor_.constraints.type_expression; optexpr) {
                    value_type_constraint_match_visitor vtcv{ ctx, e, binding };
                    auto res = apply_visitor(vtcv, *optexpr);
                    if (!res) return std::unexpected(std::move(res.error()));
                    return std::pair{*res, vtcv.weight};
                } else {
                    expression_visitor evis{ ctx };
                    auto res = apply_visitor(evis, e);
                    if (!res) return std::unexpected(std::move(res.error()));
                    return std::pair{ ctx.context_type, -1 };
                }
            }
            case parameter_constraint_modifier_t::typename_constraint:
            {
                ct_expression_visitor evis{ ctx };
                auto argres = apply_visitor(evis, e);
                if (!argres) return std::unexpected(std::move(argres.error()));

                if (auto const& optexpr = descriptor_.constraints.type_expression; optexpr) {
                    value_type_constraint_visitor vtcv{ ctx, binding };
                    auto res = apply_visitor(vtcv, *optexpr);
                    if (!res) return std::unexpected(std::move(res.error())); // perhaps pattern?
                    if (*res == *argres) return std::pair{ *res, 0 };
                    return std::unexpected(make_error<type_mismatch_error>(get_start_location(e), *argres, *res, get_start_location(*optexpr)));
                }
                THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
            }
        }
    //}
    THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
}

std::expected<std::pair<entity_identifier, int>, error_storage> varnamed_matcher::try_match(fn_compiler_context& ctx, identifier argname, syntax_expression_t const&, functional_binding_set&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("varnamed_matcher::match_forward");
}



//class concrete_named_parameter_impl : public named_parameter_matcher::named_parameter_impl
//{
//public:
//    explicit concrete_named_parameter_impl(pattern_parameter_descriptor descr)
//        : descr_{ std::move(descr) }
//    {
//
//    }
//};

std::expected<std::pair<entity_identifier, int>, error_storage> named_parameter_matcher::try_match(fn_compiler_context& ctx, syntax_expression_t const& e, functional_binding_set& bs) const
{
    return impl_->try_match(ctx, e, bs);
    //THROW_NOT_IMPLEMENTED_ERROR("named_parameter_matcher::try_match");
}

//class named_matcher : public named_parameter_matcher
//{
//    annotated_identifier external_name_;
//    optional<annotated_identifier> internal_name_;
//
//public:
//    explicit named_matcher(annotated_identifier enm, annotated_identifier const* inm)
//        : external_name_{ std::move(enm) }
//    {
//        if (inm) internal_name_.emplace(*inm);
//    }
//};

basic_fn_pattern::basic_fn_pattern(fn_compiler_context& ctx, functional const& fnl, fn_pure_t const& fnd)
    : fnl_{ fnl }, has_varpack_{ 0 }
{
    location_ = fnd.location();

    auto& params = fnd.parameters;
    parameters_.reserve(params.size());

    // auxiliary
    annotated_identifier auxai;
    std::array<char, 16> argname = { '$' };

    for (auto& param : params) {
        parameters_.emplace_back(param.name, param.modifier);
        pattern_parameter_descriptor pdescr{ param.modifier, param.constraints, param.value };
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param.name);
        if (varname) {
            if (!varnamed_matcher_) {
                throw basic_general_error(external_name->location, "Multiple variadic named parameters are not allowed."sv, external_name->value);
            }
            varnamed_matcher_.emplace(*internal_name, std::move(pdescr));
        } else if (!external_name) {
            // auto numeration internal parameter
            //if (!internal_name) {
            //    size_t argindex = matchers_.size();
            //    bool reversed = false;
            //    char* epos = mp::to_string(span{ &argindex, 1 }, argname.data() + 1, reversed);
            //    if (reversed) std::reverse(argname.data() + 1, epos);
            //    auxai = { ctx.u().slregistry().resolve(string_view{ argname.data(), epos }), {} };
            //    internal_name = &auxai;
            //}
            //matchers_.emplace_back(make_shared<parameter_matcher>(*internal_name, std::move(pdescr)));
            matchers_.emplace_back(make_shared<parameter_matcher>(std::move(pdescr)));
        } else {
            auto it = named_matchers_.find(external_name->value);
            if (it != named_matchers_.end()) {
                // change type to to some variadic?
                throw basic_general_error(it->external_name().location, "duplicate parameter name"sv, it->external_name().value);
            }
            named_matchers_.insert(it, named_parameter_matcher { *external_name, internal_name, std::move(pdescr), named_matchers_.size() });
        }
    }

    if (fnd.result) {
        pattern_parameter_descriptor prdescr{ 
            .modifier = parameter_constraint_modifier_t::value_type_constraint,
            .constraints = parameter_constraint_set_t{ .type_expression = *fnd.result },
            .default_value = {}
        };
        //result_matcher_ = make_shared<parameter_matcher>(annotated_identifier{/*result type name*/}, std::move(prdescr));
        result_matcher_ = make_shared<parameter_matcher>(std::move(prdescr));
    }
}

//void basic_fn_pattern::build(fn_compiler_context& ctx) const
//{
//    // gather local variables
//    // local_variables_
//#if 0
//    building_ = 1;
//    for (shared_ptr<parameter_matcher> m : matchers_) {
//        m->build(ctx, *this);
//    }
//    if (result_matcher_) {
//        result_matcher_->build(ctx, *this);
//    }
//    building_ = 0;
//    built_ = 1;
//#endif
//}

//class argument_matcher
//{
//    basic_fn_pattern const& pattern;
//
//public:
//    explicit inline argument_matcher(basic_fn_pattern const& ptrn) noexcept : pattern{ ptrn } {}
//
//    error_storage setup(pure_call_t const& call)
//    {
//        return {};
//    }
//
//    std::expected<int, error_storage> try_match(fn_compiler_context& ctx, pure_call_t const& call, functional_match_descriptor& md)
//    {
//        for (auto const& arg : call.args()) { // { argname, expr }
//            annotated_identifier const* pargname = arg.name();
//            if (pargname) {
//                if (named_parameter_matcher const* nmtch = pattern.get_matcher(pargname->value); nmtch) {
//                    if (auto err = nmtch->try_match(ctx, arg.value(), md.bindings); err) {
//                        return std::unexpected(std::move(err));
//                    }
//                    continue;
//                } else if (pattern.has_varnamed()) {
//                
//                }
//                
//                //if (it != named_matchers_.end()) continue;
//                //if (has_varnamed_) continue;
//            }
//        }
//        THROW_NOT_IMPLEMENTED_ERROR();
//    }
//};

optional<size_t> basic_fn_pattern::get_local_variable_index(identifier id) const
{
    auto it = local_variables_.find(id);
    if (it == local_variables_.end()) return {};
    return static_cast<size_t>(it - local_variables_.begin());
}

named_parameter_matcher const* basic_fn_pattern::get_matcher(identifier name) const
{
    if (auto it = named_matchers_.find(name); it != named_matchers_.end()) return &*it;
    return nullptr;
}

std::expected<functional_match_descriptor_ptr, error_storage> basic_fn_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const& expected_result_type) const
{
    // quick match check
    for (auto const& arg : call.args()) {
        if (annotated_identifier const* argname = arg.name(); argname) {
            if (varnamed_matcher_) continue;
            auto it = named_matchers_.find(argname->value);
            if (it != named_matchers_.end()) continue;
            return std::unexpected(make_error<basic_general_error>(argname->location, "argument mismatch"sv, argname->value));
        } else if (matchers_.empty()) {
            auto const& argexpr = arg.value();
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
    }

    compiler_task_tracer::task_guard tg = ctx.try_lock_task(pattern_task_id{ *this });
    if (!tg) {
        throw circular_dependency_error({ make_error<basic_general_error>(location_, "resolving function signature"sv, fn_qname_id()) });
    }

    // prepare binding
    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
    pmd->bindings.reset(local_variables_.size());

    auto estate = ctx.expressions_state();
    ctx.push_chain(pmd->call_expressions);

    // deal with result match
    if (expected_result_type && result_matcher_ && result_matcher_->is_pattern()) {
        auto res = result_matcher_->try_match(ctx, expected_result_type, pmd->bindings);
        if (!res) return std::unexpected(std::move(res.error()));
        pmd->result = expected_result_type.value;
    }
    auto start_matcher_it = matchers_.begin(), end_matcher_it = matchers_.end();

    auto result_error = [](opt_named_term<syntax_expression_t> const& arg) {
        if (annotated_identifier const* pargname = arg.name(); pargname) {
            return make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value);
        } else {
            return make_error<basic_general_error>(get_start_location(arg.value()), "argument mismatch"sv, arg.value());
        }
    };

    semantic::expression_list_t::const_iterator prev_argument_expression_it = pmd->call_expressions.end();

    for (auto const& arg : call.args()) { // { argname, expr }
        annotated_identifier const* pargname = arg.name();
        if (pargname) {
            if (named_parameter_matcher const* nmtch = get_matcher(pargname->value); nmtch) {
                auto match = nmtch->try_match(ctx, arg.value(), pmd->bindings);
                if (!match) return std::unexpected(std::move(match.error()));
                pmd->weight += match->second;
                pmd->signature.set(pargname->value, field_descriptor{ match->first, false });
            } else if (varnamed_matcher_) {
                if (auto match = varnamed_matcher_->try_match(ctx, pargname->value, arg.value(), pmd->bindings); !match) {
                    return std::unexpected(std::move(match.error()));
                }
            } else {
                // note: in fact must never be here due to the quick check procedure
                return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
            }
            pmd->push_named_argument_expressions(pargname->value, prev_argument_expression_it, pmd->call_expressions);
            continue;
        }

        if (start_matcher_it == end_matcher_it) {
            return std::unexpected(result_error(arg));
        }

        auto exp = (*start_matcher_it)->try_forward_match(ctx, arg.value(), pmd->bindings);
        if (error_storage* err = get<error_storage>(&exp); err) return std::unexpected(std::move(*err));
        if (auto * pmatch = get<std::pair<entity_identifier, int>>(&exp); pmatch) {
            pmd->push_unnamed_argument_expressions(start_matcher_it - matchers_.begin(), prev_argument_expression_it, pmd->call_expressions);
            pmd->weight += pmatch->second;
            pmd->signature.set(start_matcher_it - matchers_.begin(), field_descriptor{ pmatch->first, false });
            ++start_matcher_it;
            continue;
        }
        BOOST_ASSERT(get<parameter_matcher::postpone_t>(&exp));

        for (auto next_matcher_it = start_matcher_it + 1;;) {
            if (next_matcher_it != end_matcher_it) {
                exp = (*next_matcher_it)->try_forward_match(ctx, arg.value(), pmd->bindings);
                if (auto* pmatch = get<std::pair<entity_identifier, int>>(&exp); pmatch) {
                    pmd->push_unnamed_argument_expressions(next_matcher_it - matchers_.begin(), prev_argument_expression_it, pmd->call_expressions);
                    pmd->weight += pmatch->second;
                    start_matcher_it = next_matcher_it; ++start_matcher_it;
                    break;
                }
                if (get<parameter_matcher::postpone_t>(&exp)) {
                    ++next_matcher_it;
                    continue;
                }
            }
            // here: next_matcher_it == end_matcher_it || get<error_storage>(&exp) != null
            std::expected<std::pair<entity_identifier, int>, error_storage> res;
            for (;;) {
                --next_matcher_it;
                res = (*next_matcher_it)->try_match(ctx, arg.value(), pmd->bindings);
                if (res) {
                    pmd->push_unnamed_argument_expressions(next_matcher_it - matchers_.begin(), prev_argument_expression_it, pmd->call_expressions);
                    start_matcher_it = next_matcher_it; // this is a variadic matcher
                    pmd->weight += res->second;
                    break;
                }
                if (next_matcher_it == start_matcher_it) {
                    return std::unexpected(std::move(res.error()));
                }
            }
            break;
        }
#if 0
        continue;

        auto next_matcher_it = start_matcher_it; ++next_matcher_it;

        while (next_matcher_it != end_matcher_it) {
            exp = (*next_matcher_it)->try_forward_match(ctx, arg.value(), pmd->bindings);
            if (get<error_storage>(&exp)) break;
            if (auto* pmatch = get<std::pair<entity_identifier, int>>(&exp); pmatch) {
                pmd->push_unnamed_argument_expressions(next_matcher_it - matchers_.begin(), prev_argument_expression_it, pmd->call_expressions);
                pmd->weight += pmatch->second;
                start_matcher_it = next_matcher_it; ++start_matcher_it;
                break;
            }
            BOOST_ASSERT(get<parameter_matcher::postpone_t>(&exp));
            ++next_matcher_it;
        }

        std::expected<std::pair<entity_identifier, int>, error_storage> res;
        do {
            --next_matcher_it;
            res = (*next_matcher_it)->try_match(ctx, arg.value(), pmd->bindings);
            if (res) {
                start_matcher_it = next_matcher_it; // this is a variadic matcher
                pmd->weight += res->second;
                break;
            }
        } while (next_matcher_it != start_matcher_it);
        if (!res) {
            return std::unexpected(std::move(res.error()));
        }
#endif
    }

    if (!pmd->result && result_matcher_) {
        auto res = result_matcher_->apply_binding(ctx, pmd->bindings);
        if (!res) return std::unexpected(std::move(res.error()));
        pmd->result = *res;
    }
    return pmd;
}

std::expected<entity_identifier, error_storage> basic_fn_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    // fnsig -> fn entity
    // if fn entity exists => all nested qnames(functionals) are defined (e.g. argument variables, function body qname)
    // => all nested qnames shoud be defined in the build procedure
    indirect_signatured_entity smpl{ md.signature };

    entity& e = ctx.u().eregistry().find_or_create(smpl, [this, &ctx, &md]() {
        return build(ctx.u(), md);
    });

    BOOST_ASSERT(dynamic_cast<function_entity*>(&e));
    function_entity& fne = static_cast<function_entity&>(e);
    entity_identifier rt = fne.get_result_type();

    if (!rt) { // we need building to resolve result type
        compiler_task_tracer::task_guard tg = ctx.try_lock_task(entity_task_id{ e });
        if (!tg) {
            throw circular_dependency_error({ make_error<basic_general_error>(location_, "resolving function result type"sv, e.id()) });
        }
        rt = fne.get_result_type();
        if (!rt) {
            fne.build(ctx.u());
            rt = fne.get_result_type();
        }
    }

    // push call expressions in the right order
    ctx.append_expression(semantic::expression_list_t{});
    semantic::expression_list_t& args = get<semantic::expression_list_t&>(ctx.expressions().back());
    size_t argpos = 0;
    for (auto const& [param_name, param_mod] : parameters_) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);

        if (param_mod != parameter_constraint_modifier_t::value_type_constraint) {
            if (!external_name) ++argpos;
            continue;
        }
        
        if (external_name) {
            auto argspan = md.get_named_arguments(external_name->value);
            BOOST_ASSERT(!argspan.empty());
            auto matcherit = named_matchers_.find(external_name->value);
            BOOST_ASSERT(matcherit != named_matchers_.end());
            for (auto rng : argspan) {
                ++rng.second;
                args.splice_back(md.call_expressions, rng.first, rng.second);
            }
            if (matcherit->is_variadic()) {
                ctx.u().push_back_expression(args, semantic::push_value(mp::integer{ argspan.size() }));
            }
        } else {
            auto argspan = md.get_position_arguments(argpos);
            BOOST_ASSERT(!argspan.empty());
            for (auto rng : argspan) {
                ++rng.second;
                args.splice_back(md.call_expressions, rng.first, rng.second);
            }
            BOOST_ASSERT(matchers_.size() > argpos);
            if (matchers_[argpos++]->is_variadic()) {
                ctx.u().push_back_expression(args, semantic::push_value(mp::integer{ argspan.size() }));
            }
        }
    }

    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered
    ctx.u().push_back_expression(args, semantic::invoke_function(e.id()));

    return rt;
}

basic_fn_pattern2::basic_fn_pattern2(fn_compiler_context& ctx, fn_pure_t const& fnd)
{
    unit& u = ctx.u();
    location_ = fnd.location();
    //qname fn_qname = ctx.ns() / fnd.name();
    //fn_qname_id_ = u.fregistry().resolve(fn_qname).id();
    //fd_.set_id(fn_qname_id_);

    if (auto err = ctx.build_function_descriptor(fnd, fd_); err) {
        throw exception(u.print(*err));
    }

    if (fd_.variables().empty() && fd_.varparams().empty()) { // ordinary function
    
    } else { // template

    }
    //THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::basic_fn_pattern");
#if 0
    // now we know if it's a template or a general function
    // to do: implement templates
    
    // if a result type is declared it should be resolved in the declaration context => shouldn't be postponed
    if (fnd.result) {
        result_type_ = apply_visitor(preliminary_type_visitor{ ctx }, *fnd.result);

        entity_signature fnsig{ u.get_fn_qname_identifier() };
        for (auto const& f : fd_.positioned_fields()) {
            if (f.constraint_type == param_constraint_type::value_type_constaint) {
                fnsig.push(f.constraint);
            }
        }
        for (auto const& f : fd_.named_fields()) {
            if (f.constraint_type == param_constraint_type::value_type_constaint) {
                fnsig.push(f.ename.value, f.constraint);
            }
        }
        // create function entity
        auto fn_entity = make_shared<function_entity>(fn_qname_id_, fs_, result_type_);
        fn_entity->set_fn_type(u, fnsig);
        
        u.eregistry().insert(fn_entity); //
        fn_entity->set_body(std::move(fnd.body));
        fn_entity_id_ = fn_entity->id();

        fn_entity->set_const_index(u.allocate_constant_index());
    } else {
        THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern without explicit result type");
    }
#endif
}

struct const_expression_visitor : static_visitor<error_storage>
{
    fn_compiler_context& ctx;
    annotated_entity_identifier expected_result;

    const_expression_visitor(fn_compiler_context& c, annotated_entity_identifier && er)
        : ctx{ c }
        , expected_result{ std::move(er) }
    {}

    result_type operator()(annotated_entity_identifier const& ee) const
    {
        if (!expected_result || expected_result.value == ee.value) return {};
        return make_error<type_mismatch_error>(ee.location, ee.value, expected_result.value, expected_result.location);
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("const_expression_visitor not implemented expression");
    }
};

template <typename ExpressionIteratorT>
struct arg_resolving_by_value_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& ctx;
    functional_binding_set& binding;
    ExpressionIteratorT& expr_it;
    arg_resolving_by_value_visitor(fn_compiler_context& c, functional_binding_set& bnd, ExpressionIteratorT& it)
        : ctx{ c }, binding{ bnd }, expr_it{ it }
    {}

    result_type operator()(annotated_qname_identifier const& aqi) const
    {
        functional const& fnl = ctx.u().fregistry().resolve(aqi.value);
        entity_identifier eid = fnl.default_entity();
        if (eid) {
            return this->operator()(eid, aqi.location);
        }
        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr_it.next_expression()));
    }

    result_type operator()(entity_identifier const& eid, lex::resource_location eidloc = {}) const
    {
        BOOST_ASSERT(eid);
        const_expression_visitor evis{ ctx, { eid, eidloc } };
        if (auto err = apply_visitor(evis, expr_it.next_expression()); err) {
            return std::unexpected(std::move(err));
        }
        return eid;
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("arg_resolving_by_value_visitor not implemented expression");
    }
};

template <typename ExpressionIteratorT>
struct arg_resolving_by_value_type_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& ctx;
    functional_binding_set& binding;
    ExpressionIteratorT & expr_it;
    mutable int weight = 0;
    arg_resolving_by_value_type_visitor(fn_compiler_context& c, functional_binding_set& bnd, ExpressionIteratorT & it)
        : ctx{ c }, binding{ bnd }, expr_it{ it }
    {}

    result_type operator()(annotated_qname_identifier const& aqi) const
    {
        functional const& fnl = ctx.u().fregistry().resolve(aqi.value);
        entity_identifier eid = fnl.default_entity();
        if (eid) {
            return this->operator()(eid, aqi.location);
        }
        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr_it.next_expression()));
    }

    result_type operator()(entity_identifier const& eid, lex::resource_location eidloc = {}) const
    {
        entity const& param_entity = ctx.u().eregistry().get(eid);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            unsigned int pack_size = 0;
            for (;;) {
                syntax_expression_t const& expr = expr_it.next_expression();
                expression_visitor evis{ ctx, { pent->element_type(), get_start_location(expr) } };
                auto res = apply_visitor(evis, expr);
                if (!res) break; // skip error
                // do not condider cast's weight inpact for pack_entity cases
                ++pack_size;
                auto neres = expr_it.has_next_expression();
                if (!neres) return std::unexpected(std::move(neres.error()));
                if (!neres.value()) break;
            }
            weight -= pack_size; // put down as pack_entity
            ctx.append_expression(semantic::push_value{ uint64_t{ pack_size } });
        } else {
            BOOST_ASSERT(eid);
            expression_visitor evis{ ctx, { eid, eidloc } };
            auto res = apply_visitor(evis, expr_it.next_expression());
            if (!res) {
                return std::unexpected(std::move(res.error()));
            } else if (res.value()) { --weight; }
        }
        return eid;

        //GLOBAL_LOG_INFO() << ctx.u().print(eid);
        //GLOBAL_LOG_INFO() << ctx.u().print(expr);
        //THROW_NOT_IMPLEMENTED_ERROR("arg_resolve_visitor(entity_identifier) not implemented expression");
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("arg_resolving_by_value_type_visitor not implemented expression");
    }
};

struct result_resolving_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& ctx;
    functional_binding_set& binding;

    result_resolving_visitor(fn_compiler_context& c, functional_binding_set& bnd)
        : ctx{ c }, binding{ bnd }
    {}

    result_type operator()(annotated_qname_identifier const& aqi) const
    {
        functional const& fnl = ctx.u().fregistry().resolve(aqi.value);
        entity_identifier eid = fnl.default_entity();
        if (eid) return eid;
        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, fnl.name()));
    }

    result_type operator()(entity_identifier const& eid) const
    {
        return eid;
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("result_resolving_visitor not implemented expression");
    }
};


#if 0
class argument_matcher2
{
    function_descriptor const& fd_;

    using bound_args_set_t = boost::container::small_flat_multimap<identifier, syntax_expression_t const*, 8>;
    bound_args_set_t named_;
    using positioned_args_set_t = std::span<const syntax_expression_t>;
    positioned_args_set_t positioned_;

    variant<null_t, bound_args_set_t::iterator, positioned_args_set_t::iterator> arg_iterator_;
    identifier cur_name_;
    syntax_expression_t const* cur_expression_ = nullptr;

    using named_fields_t = span<const function_descriptor::named_field>;
    using positioned_fields_t = span<const function_descriptor::positioned_field>;
    variant<null_t, named_fields_t::iterator, positioned_fields_t::iterator> param_iterator_;

    int index_;

public:
    explicit argument_matcher2(function_descriptor const& fd) : fd_{ fd } {}
    
    error_storage setup(pure_call_t const& call)
    {
        for (auto const& arg : call.args()) { // { argname, expr }
            annotated_identifier const* pargname = arg.name();
            if (!pargname) continue;
            function_descriptor::named_field const* pnf = fd_.find_named_field(pargname->value);
            if (!pnf) {
                return make_error<basic_general_error>(pargname->location, "unmatched named argument"sv, arg.value());
            }
            named_.emplace(pnf->ename.value, &arg.value());
        }

        for (function_descriptor::named_field const& nf : fd_.named_fields()) {
            auto it = named_.find(nf.ename.value);
            if (it != named_.end()) continue;
            // to do: treat default value if exists
            return make_error<basic_general_error>(call.location(), "unmatched named parameter"sv, nf.ename.value);
        }

        // initialize argument and parameter iterators
        THROW_NOT_IMPLEMENTED_ERROR();
        //positioned_ = call.positioned_args;

        if (named_.empty()) {
            arg_iterator_ = positioned_.begin();
            param_iterator_ = fd_.positioned_fields().begin();
            BOOST_ASSERT(fd_.named_fields().empty());
        } else {
            arg_iterator_ = named_.begin();
            param_iterator_ = fd_.named_fields().begin();
        }
        cur_name_ = {};
        return {};
    }

    std::expected<bool, error_storage> has_next_expression() const
    {
        if (positioned_args_set_t::iterator const* p = get<positioned_args_set_t::iterator>(&arg_iterator_); p) {
            return !cur_name_ && *p != positioned_.end();
        }
        bound_args_set_t::iterator const& nit = get<bound_args_set_t::iterator>(arg_iterator_);
        if (cur_name_) {
            if (nit == named_.end()) return false;
            return nit->first == cur_name_;
        }
        if (nit != named_.end()) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(*nit->second), "unmatched argument"sv, nit->first));
        }
        return !positioned_.empty();
    }

    syntax_expression_t const& next_expression()
    {
        if (positioned_args_set_t::iterator * p = get<positioned_args_set_t::iterator>(&arg_iterator_); p) {
            BOOST_ASSERT(*p != positioned_.end());
            cur_expression_ = std::addressof(*(*p)++);
        } else {
            bound_args_set_t::iterator& nit = get<bound_args_set_t::iterator>(arg_iterator_);
            if (nit == named_.end()) {
                auto pit = positioned_.begin();
                BOOST_ASSERT(pit != positioned_.end());
                cur_expression_ = std::addressof(*pit);
                arg_iterator_ = ++pit;
            } else {
                if (!cur_name_) {
                    cur_name_ = nit->first;
                } else {
                    BOOST_ASSERT(nit->first == cur_name_);
                }
                cur_expression_ = std::addressof(*nit->second);
                ++nit;
            }
        }
        return *cur_expression_;
    }

    syntax_expression_t const& current_expression() const
    {
        BOOST_ASSERT(cur_expression_);
        return *cur_expression_;
    }

    //bool has_next_parameter() const
    //{
    //    if (positioned_fields_t::iterator const* p = get<positioned_fields_t::iterator>(&param_iterator_); p)
    //        return *p != fd_.positioned_fields().end();
    //    
    //    //named_fields_t::iterator const& nit = get<named_fields_t::iterator>(param_iterator_);
    //    BOOST_ASSERT(get<named_fields_t::iterator>(param_iterator_) != fd_.named_fields().end());
    //    return true;
    //}

    function_descriptor::generic_field const* next_parameter()
    {
        if (positioned_fields_t::iterator * p = get<positioned_fields_t::iterator>(&param_iterator_); p) {
            if (*p != fd_.positioned_fields().end()) {
                cur_name_ = {};
                return std::addressof(*(*p)++);
            }
            return nullptr;
        }
        named_fields_t::iterator& nit = get<named_fields_t::iterator>(param_iterator_);
        cur_name_ = nit->ename.value;
        function_descriptor::generic_field const& result = *nit++;
        
        if (nit == fd_.named_fields().end()) {
            param_iterator_ = fd_.positioned_fields().begin();
        }
        return &result;
    }

    std::expected<int, error_storage> try_match(fn_compiler_context& ctx, functional_match_descriptor& md)
    {
        int match_weight = 0;
        small_vector<fn_compiler_context::expr_vec_t, 8> arg_branches;
        size_t rt_argnum = 0, argnum = 0;

        auto estate = ctx.expressions_state();

        for (;;) {
            // need calling first to prepare cur_name_
            //function_descriptor::generic_field const* pparam = has_next_parameter() ? &next_parameter() : nullptr;
            function_descriptor::generic_field const* pparam = next_parameter(); // returns null if no more parameters
            auto res = has_next_expression();
            if (!res) return std::unexpected(std::move(res.error()));
            if (!res.value()) {
                if (!pparam) break; // matchng finished
                THROW_NOT_IMPLEMENTED_ERROR("default parameter value");
            }
            if (!pparam) {
                return std::unexpected(make_error<basic_general_error>(get_start_location(next_expression()), "unmatched argument"sv));
            }
            
            entity_identifier param_entity;
            bool is_runtime_argument = false;
            if (pparam->constraint) {
                std::expected<entity_identifier, error_storage> res;
                switch (pparam->constraint_type) {
                case parameter_constraint_modifier_t::value_type_constraint:
                {
                    if (arg_branches.size() <= rt_argnum) arg_branches.resize(rt_argnum + 1);
                    ctx.push_chain(arg_branches[rt_argnum]);
                    is_runtime_argument = true;
                    
                    arg_resolving_by_value_type_visitor arvis{ ctx, md.bindings, *this };
                    res = apply_visitor(arvis, *pparam->constraint);
                    match_weight += arvis.weight;
                    break;
                }
                case parameter_constraint_modifier_t::typename_constraint:
                case parameter_constraint_modifier_t::value_constraint:
                {
                    arg_resolving_by_value_visitor arvis{ ctx, md.bindings, *this };
                    res = apply_visitor(arvis, *pparam->constraint);
                    break;
                }
                default:
                    THROW_INTERNAL_ERROR("unknown constraint type");
                }
                if (!res.has_value()) return std::unexpected(std::move(res.error()));
                param_entity = res.value();
            } else {
                if (arg_branches.size() <= rt_argnum) arg_branches.resize(rt_argnum + 1);
                ctx.push_chain(arg_branches[rt_argnum]);
                is_runtime_argument = true;

                expression_visitor evis{ ctx, nullptr };
                if (auto res = apply_visitor(evis, next_expression()); !res) {
                    return std::unexpected(std::move(res.error()));
                }
                param_entity = ctx.context_type;
            }

            if (param_entity) { // entity was resolved
                if (cur_name_) {
                    md.signature.set(argnum, cur_name_, { param_entity, !is_runtime_argument });
                } else {
                    md.signature.set(argnum, { param_entity, !is_runtime_argument });
                }
                THROW_NOT_IMPLEMENTED_ERROR();
#if 0
                for (annotated_identifier const& ai : pparam->bindings) {
                    auto it = md.bindings.find(ai.value);
                    if (it == md.bindings.end()) {
                        md.bindings.insert(it, std::pair{ ai, param_entity });
                    }
                    else if (it->second != param_entity) {
                        return std::unexpected(make_error<basic_general_error>(get_start_location(current_expression()), "positioned argument mismatch"sv));
                    }
                }
#endif
            } // else can't resolve due to the lack of matched parameters

            estate.restore();
            if (is_runtime_argument) ++rt_argnum;
            ++argnum;
        }

        // to do: check all resolved
        if (pattern_expression_t const* retexpr = fd_.result_type(); retexpr) {
            result_resolving_visitor arvis{ ctx, md.bindings };
            auto res = apply_visitor(arvis, *retexpr);
            if (!res.has_value()) return std::unexpected(std::move(res.error()));
            md.result = res.value();
            md.signature.set_result({ res.value(), false });
        }

        for (auto& branch : arg_branches) {
            std::move(branch.begin(), branch.end(), std::back_inserter(ctx.expressions()));
        }

        md.signature.normilize(); // ?? already sorted?
        estate.detach();
        return match_weight;
    }

    //inline bool empty() const noexcept { return !named_ || named_->size() + positioned_.size() >= index_; }

    //syntax_expression_t const& operator*() const
    //{
    //    if (index < named_->size()) {

    //    }
    //}
};
#endif

std::expected<functional_match_descriptor_ptr, error_storage> basic_fn_pattern2::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern2::try_match");
#if 0
    argument_matcher2 matcher{ fd_ };
    if (auto err = matcher.setup(call); err) return std::unexpected(std::move(err));

    return matcher.try_match(ctx, md);
    

    
    //arg_branches.resize(call.named_args.size() + call.positioned_args.size());
    size_t argnum = 0;

    size_t param_pos = 0;
    size_t param_count = fd_.positioned_fields().size() + fd_.named_fields().size();

    for (auto const& pair: bind_args) {
        if (arg_branches.size() <= argnum) arg_branches.resize(argnum + 1);
        ctx.push_chain(arg_branches[argnum]);
    }
    THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::is_matched, named argument");

    auto psp = fd_.positioned_fields();
    auto posarg = psp.begin();
    argnum = call.named_args.size(); // to do: change to actual (possible increased) number of named params (take into account named param packs) 

    for (auto expr_it = call.positioned_args.begin(), expr_eit = call.positioned_args.end(); expr_it != expr_eit;) {
        if (posarg == psp.end()) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(*expr_it), "positioned argument mismatch"sv));
        }

        entity_identifier param_entity;
        if (posarg->constraint) {
            std::expected<entity_identifier, error_storage> res;
            switch (posarg->constraint_type) {
                case parameter_constraint_modifier_t::value_type_constraint:
                {
                    if (arg_branches.size() <= argnum) arg_branches.resize(argnum + 1);
                    ctx.push_chain(arg_branches[argnum]);

                    arg_resolving_by_value_type_visitor arvis{ ctx, md.bindings, expr_it, expr_eit };
                    res = apply_visitor(arvis, *posarg->constraint);
                    match_weight += arvis.weight;
                    break;
                }
                case parameter_constraint_modifier_t::typename_constraint:
                case parameter_constraint_modifier_t::value_constraint:
                {
                    arg_resolving_by_value_visitor arvis{ ctx, md.bindings, expr_it, expr_eit };
                    res = apply_visitor(arvis, *posarg->constraint);
                    break;
                }
                default:
                    THROW_INTERNAL_ERROR("unknown constraint type");
            }
            if (!res.has_value()) return std::unexpected(std::move(res.error()));
            param_entity = res.value();
        } else {
            if (arg_branches.size() <= argnum) arg_branches.resize(argnum + 1);
            ctx.push_chain(arg_branches[argnum]);

            expression_visitor evis{ ctx, nullptr };
            if (auto res = apply_visitor(evis, *expr_it); !res) {
                return std::unexpected(std::move(res.error()));
            }
            param_entity = ctx.context_type;
            ++expr_it;
        }

        if (param_entity) { // entity was resolved
            md.signature.set(argnum, {param_entity, false}); // to do: treat const and typename
            for (annotated_identifier const& ai : posarg->bindings) {
                auto it = md.bindings.find(ai);
                if (it == md.bindings.end()) {
                    md.bindings.insert(it, std::pair{ ai, param_entity });
                } else if (it->second != param_entity) {
                    return std::unexpected(make_error<basic_general_error>(get_start_location(*expr_it), "positioned argument mismatch"sv));
                }
            }
        } // else can't resolve due to lack of matched parameters

        estate.restore();

        ++posarg;
        ++argnum;
    }

    // to do: check all resolved
    if (pattern_expression_t const* retexpr = fd_.result_type(); retexpr) {
        result_resolving_visitor arvis{ ctx, md.bindings };
        auto res = apply_visitor(arvis, *retexpr);
        if (!res.has_value()) return std::unexpected(std::move(res.error()));
        md.result = res.value();
    }

    for (auto & branch : arg_branches) {
        std::move(branch.begin(), branch.end(), std::back_inserter(ctx.expressions()));
    }
    
    //{
    //    span<pattern_expression_t const> cs = posarg->concepts;
    //    for (pattern_expression_t const& pe : cs) {
    //        arg_resolving_visitor arv{ ctx, *expr_it };
    //        auto res = apply_visitor(arv, cs);
    //        if (!res.has_value()) return std::move(res.error());
    //        entity_identifier param_entity = res.value();
    //    }
    //}

    // to do: check concepts
    md.signature.normilize();
    estate.detach();
    return match_weight;
#endif
}

std::expected<entity_identifier, error_storage> basic_fn_pattern2::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    indirect_signatured_entity smpl{ md.signature };

    entity & e = ctx.u().eregistry().find_or_create(smpl, [this, &ctx, &md]() {
        return build(ctx.u(), md);
    });

    BOOST_ASSERT(dynamic_cast<function_entity*>(&e));
    function_entity & fne = static_cast<function_entity&>(e);
    entity_identifier rt = fne.get_result_type();
    if (!rt) { // need build to resolve result type
        compiler_task_tracer::task_guard tg = ctx.try_lock_task(entity_task_id{ e });
        if (!tg) {
            throw circular_dependency_error({ make_error<basic_general_error>(location_, "resolving function result type"sv, fd_.id()) });
        }
        rt = fne.get_result_type();
        if (!rt) {
            fne.build(ctx.u());
            rt = fne.get_result_type();
        }
    }

    ctx.append_expression(semantic::invoke_function(e.id()));
    ctx.pop_chain(); // pop function call chain
    return rt;

    //THROW_NOT_IMPLEMENTED_ERROR("generic_fn_pattern::apply");
#if 0
    if (!result_type_) { // the result type should be inferred by the function content
        if (building_) {
            throw circular_dependency_error({ make_error<basic_general_error>(location_, "resolving function result type"sv, fd_.id()) });
        }
        const_cast<basic_fn_pattern*>(this)->build(ctx.u());
    }
    BOOST_ASSERT(fn_entity_id_);
    ctx.expressions().push_back(semantic::invoke_function(fn_entity_id_));
    return result_type_;
    //THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::apply");
#endif
}

generic_fn_pattern::generic_fn_pattern(fn_compiler_context& ctx, functional const& fnl, fn_decl_t const& fnd)
    : basic_fn_pattern{ ctx, fnl, fnd }
    , body_{ make_shared<std::vector<infunction_statement>>(fnd.body)}
    , kind_{ fnd.kind }
{

}

std::expected<entity_identifier, error_storage> generic_fn_pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::const_apply");
}

shared_ptr<entity> generic_fn_pattern::build(unit& u, functional_match_descriptor& md) const
{
    qname_view fnqn = fn_qname();
    qname fn_ns = fnqn / u.new_identifier();
    //fn_compiler_context fnctx{ u, fnqn / u.new_identifier() };

    auto append_local_var = [&u](qname_view varname, entity_identifier eid) {
        functional& varfn = u.fregistry().resolve(std::move(varname));
        BOOST_ASSERT(!varfn.default_entity());
        auto var_entity = make_shared<variable_entity>(eid, varfn.id(), variable_entity::kind::SCOPE_LOCAL);
        u.eregistry().insert(var_entity);
        varfn.set_default_entity(var_entity->id());
        return var_entity;
    };
    std::array<char, 16> argname = { '$' };

    size_t posarg_num = 0, paramnum = 0;
    size_t paramcount = 0; // +fnctx.captured_variables.size();
    for (auto const& f : md.signature.positioned_fields()) {
        if (!f.is_const()) ++paramcount;
    }
    for (auto const& [_, f] : md.signature.named_fields()) {
        if (!f.is_const()) ++paramcount;
    }
    for (auto const& [param_name, param_mod] : parameters_) {
        if (param_mod != parameter_constraint_modifier_t::value_type_constraint) continue;
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);
        if (external_name) { // named parameter
            field_descriptor const* fd = md.signature.find_field(external_name->value);
            BOOST_ASSERT(fd);
            auto var_entity = append_local_var(fn_ns / (internal_name ? internal_name->value : external_name->value), fd->entity_id());
            var_entity->set_index(paramnum - paramcount); ++paramnum;
        } else {
            field_descriptor const* fd = md.signature.find_field(posarg_num);
            BOOST_ASSERT(fd);

            bool reversed = false;
            char* epos = mp::to_string(span{ &posarg_num, 1 }, argname.data() + 1, reversed);
            if (reversed) std::reverse(argname.data() + 1, epos);
            identifier posargid = u.slregistry().resolve(string_view{ argname.data(), epos });
            auto var_entity = append_local_var(fn_ns / posargid, fd->entity_id());
            var_entity->set_index(paramnum - paramcount);

            if (internal_name) {
                var_entity = append_local_var(fn_ns / internal_name->value, fd->entity_id());
                var_entity->set_index(paramnum - paramcount);
            }
            
            ++posarg_num;
            ++paramnum;
        }
    }

    auto pife = make_shared<internal_function_entity>(
        std::move(fn_ns),
        std::move(md.signature),
        make_shared<internal_function_entity::build_data>(std::move(md.bindings), body_),
        u);

    pife->set_inline(kind_ == fn_kind::INLINE);

    
    //if (md.result) {
    //    pife->build_fn_signature(u, md.result);
    //}
    
    return pife;

#if 0

    function_entity& fe = const_cast<function_entity&>(dynamic_cast<function_entity const&>(u.eregistry().get(fn_entity_id_)));
    fe.build(u);
    result_type_ = fe.get_result_type();

    qname_view fn_qname = u.qnregistry().resolve(fn_qname_id_);
    fn_compiler_context fnctx{ u, fn_qname / u.new_identifier() };

    // setup parameters & signature
    entity_signature fnsig{ u.get_fn_qname_identifier() };
    small_vector<variable_entity*, 16> params;
    std::array<char, 16> argname = { '$' };
    size_t argindex = 0;
    for (auto const& f : fs_->positioned_fields()) {
        if (f.constraint_type == param_constraint_type::value_constaint) {
            THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with value constraints");
        }
        else if (f.constraint_type == param_constraint_type::const_constraint) {
            THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with const constraints");
        }
        BOOST_ASSERT(f.constraint_type == param_constraint_type::type_constaint);
        fnsig.push(f.constraint);
        identifier fname;
        if (f.iname) {
            fname = f.iname->value;
        }
        else {
            bool reversed = false;
            char* epos = mp::to_string(span{ &argindex, 1 }, argname.data() + 1, reversed);
            if (reversed) std::reverse(argname.data() + 1, epos);
            fname = u.slregistry().resolve(string_view{ argname.data(), epos });
        }
        qname_identifier param_qname_id = u.qnregistry().resolve(fnctx.ns() / fname);
        ++argindex;

        auto var_entity = make_shared<variable_entity>(f.constraint, param_qname_id, variable_entity::kind::SCOPE_LOCAL);
        u.eregistry().insert(var_entity);
        functional& param_fnl = u.fregistry().resolve(param_qname_id);
        param_fnl.set_default_entity(var_entity->id());
        params.emplace_back(var_entity.get());
    }
    for (auto const& f : fs_->named_fields()) {
        fnsig.push(f.ename.value, f.constraint);
        THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t with named fields");
    }

    function_entity& fe = const_cast<function_entity&>(dynamic_cast<function_entity const&>(u.eregistry().get(fn_entity_id_)));

    declaration_visitor dvis{ fnctx };
    for (infunction_declaration_t& d : fe.body()) {
        apply_visitor(dvis, d);
    }
    fnctx.finish_frame();

    result_type_ = fnctx.result;
    entity_identifier fntypeid = build_fn_type(u, fnsig);
    fe.set_type(fntypeid);

    intptr_t paramnum = 0;
    size_t paramcount = params.size() + fnctx.captured_variables.size();
    for (variable_entity* var : params) {
        var->set_index(paramnum - paramcount);
        ++paramnum;
    }
    for (auto [from, tovar] : fnctx.captured_variables) {
        tovar->set_index(paramnum - paramcount);
        ++paramnum;
    }
#endif
}

}
