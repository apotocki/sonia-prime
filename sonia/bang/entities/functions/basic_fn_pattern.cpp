//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_fn_pattern.hpp"

#include <utility>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
#include "sonia/bang/entities/functional_entity.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"
#include "sonia/bang/entities/ellipsis/pack_entity.hpp"

#include "sonia/bang/errors/circular_dependency_error.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

//#include "parameter_type_expression_visitor.hpp"

#include "signature_matcher.hpp"

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

parameter_matcher::parameter_matcher(annotated_identifier name, pattern_parameter_descriptor descr) noexcept
    : descriptor_{ std::move(descr) }
{
    internal_names_.emplace_back(std::move(name));
    if (auto const& optexpr = descriptor_.constraints.type_expression; optexpr) {
        if (bang_parameter_pack_t const* pp = get<bang_parameter_pack_t>(&*optexpr); pp) {
            variadic = true;
            syntax_expression_t texpr = std::move(pp->type);
            descriptor_.constraints.type_expression = std::move(texpr);
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

std::ostream& parameter_matcher::print(unit const& u, std::ostream& ss) const
{
    switch (descriptor_.modifier) {
    case parameter_constraint_modifier_t::value_constraint:
        ss << "const "; break;
    case parameter_constraint_modifier_t::typename_constraint:
        ss << "typename "; break;
    }
    if (syntax_expression_t const* pte = get_pointer(descriptor_.constraints.type_expression); pte) {
        ss << u.print(*pte);
    } else {
        ss << '_';
    }
    if (variadic) {
        ss << " ...";
    }
    return ss;
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
        return ct_expression_visitor{ ctx }(var);

        //if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
        //    // check for function parameter
        //    identifier varid = *var.name.value.begin();
        //    if (auto opteids = binding.lookup(varid); opteids) {
        //        THROW_NOT_IMPLEMENTED_ERROR("bind variable_identifier");
        //    }
        //}

        //functional const* fl = ctx.lookup_functional(var.name.value);
        //if (!fl) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        //entity_identifier eid = fl->default_entity(ctx);
        //if (!eid) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        //return eid;
        ////THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_visitor not implemented expression");
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
    functional_binding_set& binding; // to forward it to signature_matcher_visitor
    mutable int weight = 0;

    value_type_constraint_match_visitor(fn_compiler_context& c, syntax_expression_t const& e, functional_binding_set& b)
        : ctx{ c }, expr{ e }, binding{ b }
    {}

    result_type operator()(annotated_qname_identifier const& aqi) const
    {
        functional const& fnl = ctx.u().fregistry().resolve(aqi.value);
        auto opteid = fnl.default_entity(ctx);
        if (!opteid) return opteid;
        if (*opteid) {
            return this->operator()(*opteid, aqi.location);
        }

        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr));
    }

    result_type operator()(entity_identifier const& eid, lex::resource_location eidloc = {}) const
    {
        entity const& param_entity = ctx.u().eregistry_get(eid);
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
        auto opteid = ctx.lookup_entity(var.name);
        if (!opteid) return opteid;

        auto res = apply_visitor(expression_visitor{ ctx, annotated_entity_identifier{*opteid, var.name.location} }, expr);
        if (!res) return std::unexpected(std::move(res.error()));

        weight -= *res ? 1 : 0;
        return ctx.context_type;

#if 0
        if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
            // check for function parameter
            identifier varid = *var.name.value.begin();
            if (auto opteids = binding.lookup(varid); opteids) {
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
        
        entity_identifier eid = fl->default_entity(ctx);
        if (!eid) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        auto res = apply_visitor(expression_visitor{ ctx, annotated_entity_identifier{eid, var.name.location} }, expr);
        if (!res) return std::unexpected(std::move(res.error()));

        weight -= *res ? 1 : 0;
        return ctx.context_type;
#endif
    }

    result_type operator()(function_call_t const& fc) const
    {
        unit& u = ctx.u();

        ct_expression_visitor sv{ ctx, annotated_entity_identifier{ u.get(builtin_eid::qname) } };
        auto qn_ent_id = apply_visitor(sv, fc.fn_object);
        if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
        qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u.eregistry_get(*qn_ent_id));

        // check if can evaluate signature_pattern_
        auto match = ctx.find(qname_ent.value(), fc);
        if (match) {
            if (auto result = match->const_apply(ctx); result) {
                expression_visitor evis{ ctx, annotated_entity_identifier{ *result, fc.location() } };
                auto res = apply_visitor(evis, expr);
                if (!res) return std::unexpected(std::move(res.error()));
                weight -= *res ? 1 : 0;
                return ctx.context_type;
            }
        }

        // can't evaluate signature_pattern_ as a function, consider as a pattern
        auto res = apply_visitor(expression_visitor{ ctx }, expr);
        if (!res) return std::unexpected(std::move(res.error()));

        entity const& type_ent = u.eregistry_get(ctx.context_type);
        entity_signature const* psig = type_ent.signature();
        if (!psig) {
            return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
        }

        if (qname_ent.value() != psig->name()) {
            return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
        }

        size_t arg_index = 0;
        for (auto const& arg : fc.args()) {
            annotated_identifier const* argname = arg.name();
            field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
            if (!pfd) {
                return std::unexpected(make_error<basic_general_error>(fc.location(), "argument pattern mismatch"sv, expr));
            }
            BOOST_ASSERT(pfd->is_const());
            signature_matcher_visitor smvis{ ctx, binding, pfd->entity_id() };
            if (auto err = apply_visitor(smvis, arg.value()); err) return std::unexpected(std::move(err));
        }

        return type_ent.id();
        
        //signature_matcher_visitor sm{ fc, binding };
        //auto res = sm.try_match(ctx, expr);
        //if (!res) return std::unexpected(std::move(res.error()));
        //weight -= *res ? 1 : 0;
        //return ctx.context_type;

        //// to do: check if can apply fc
        //// if no, consider as a pattern:

        //// is it really a pattern?
        //variable_identifier const* pfn_name = get<variable_identifier>(&fc.fn_object);
        //if (!pfn_name || pfn_name->implicit)
        //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
        //functional const* pfn_fl = ctx.lookup_functional(pfn_name->name.value);
        //if (!pfn_fl) return std::unexpected(make_error<undeclared_identifier_error>(pfn_name->name));

        //expression_visitor evis{ ctx };
        //auto res = apply_visitor(evis, expr);
        //if (!res) return std::unexpected(std::move(res.error()));
        //
        //entity const& type_ent = ctx.u().eregistry_get(ctx.context_type);
        //entity_signature const* psig = type_ent.signature();
        //if (!psig) {
        //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
        //}
        //if (pfn_fl->id() != psig->name()) {
        //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
        //}

        //size_t arg_index = 0;
        //for (auto const& arg: fc.args()) {
        //    annotated_identifier const* argname = arg.name();
        //    field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
        //    if (!pfd) {
        //        return std::unexpected(make_error<basic_general_error>(fc.location(), "argument pattern mismatch"sv, expr));
        //    }
        //}
        ////fc.fn_object // variable_identifier
        //THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_match_visitor(function_call_t) not implemented expression");
    }

    result_type operator()(placeholder const&) const
    {
        auto res = apply_visitor(expression_visitor{ ctx }, expr);
        if (!res) return std::unexpected(std::move(res.error()));
        weight = -1; // template match
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

variant<int, parameter_matcher::postpone_t, error_storage> parameter_matcher::try_forward_match(fn_compiler_context& ctx, syntax_expression_t const& se, functional_binding_set& b, parameter_match_result& mr) const
{
    if (variadic) return postpone_t{};
    auto res = try_match(ctx, se, b, mr);
    if (!res) return std::move(res.error());
    return *res;
}

std::expected<int, error_storage> parameter_matcher::try_match(fn_compiler_context& ctx, syntax_expression_t const& e, functional_binding_set& binding, parameter_match_result& mr) const
{
    //if (constraint.which()) { // not empty
    int weight;
    switch (descriptor_.modifier) {
        case parameter_constraint_modifier_t::value_type_constraint:
        {
            auto last_expr_it = ctx.expressions().last();
            if (auto const& optexpr = descriptor_.constraints.type_expression; optexpr) {
                value_type_constraint_match_visitor vtcv{ ctx, e, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::unexpected(std::move(res.error()));
                mr.append_result(is_variadic(), *res, last_expr_it, ctx.expressions());
                weight = vtcv.weight;
            } else {
                expression_visitor evis{ ctx };
                auto res = apply_visitor(evis, e);
                if (!res) return std::unexpected(std::move(res.error()));
                mr.append_result(is_variadic(), ctx.context_type, last_expr_it, ctx.expressions());
                weight = -1; // unspecified type constraint has less priority than specified one
            }
            break;
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
                if (*res != *argres) {
                    return std::unexpected(make_error<type_mismatch_error>(get_start_location(e), *argres, *res, get_start_location(*optexpr)));
                }
                weight = 0;
            } else {
                weight = -1; // unspecified type constraint has less priority than specified one
            }
            mr.append_result(is_variadic(), *argres);
            for (annotated_identifier const& iname : internal_names_) {
                binding.emplace_back(iname.value, *argres); // bind const value
            }
            break;
            //for (identifier name : internal_names_) {
            //    binding.emplace_back(name, *argres); // bind typename
            //}
            //THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
        }
        case parameter_constraint_modifier_t::value_constraint:
        {
            if (auto const& optexpr = descriptor_.constraints.type_expression; optexpr) {
                value_type_constraint_visitor vtcv{ ctx, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::unexpected(std::move(res.error())); // perhaps pattern?

                ct_expression_visitor evis{ ctx, annotated_entity_identifier{ *res, get_start_location(*optexpr) } };
                auto argres = apply_visitor(evis, e);
                if (!argres) return std::unexpected(std::move(argres.error()));
                mr.append_result(is_variadic(), *argres);
                for (annotated_identifier const& iname : internal_names_) {
                    binding.emplace_back(iname.value, *argres); // bind const value
                }
                weight = 0;
                break;
            }

            THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
        }
    }
    if (mr.internal_names.empty()) {
        mr.internal_names = internal_names_;
    } else {
        BOOST_ASSERT(is_variadic() && std::ranges::equal(mr.internal_names, internal_names_));
    }
    return weight;
}

std::expected<std::pair<entity_identifier, int>, error_storage> varnamed_matcher::try_match(fn_compiler_context& ctx, syntax_expression_t const&, functional_binding_set&) const
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

std::expected<int, error_storage> named_parameter_matcher::try_match(fn_compiler_context& ctx, syntax_expression_t const& e, functional_binding_set& bs, parameter_match_result& mr) const
{
    return impl_->try_match(ctx, e, bs, mr);
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

basic_fn_pattern::basic_fn_pattern(fn_compiler_context& ctx, functional const& fnl, fn_pure const& fnd)
    : fnl_{ fnl }, has_varpack_{ 0 }
{
    location_ = fnd.location();

    auto& params = fnd.parameters;
    parameters_.reserve(params.size());

    // auxiliary
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
            size_t argindex = matchers_.size();
            bool reversed = false;
            char* epos = mp::to_string(span{ &argindex, 1 }, argname.data() + 1, reversed);
            if (reversed) std::reverse(argname.data() + 1, epos);
            identifier nid = ctx.u().slregistry().resolve(string_view{ argname.data(), epos });
            auto m = make_shared<parameter_matcher>(annotated_identifier{ nid }, std::move(pdescr));
            if (internal_name) { m->push_internal_name(*internal_name); }
            matchers_.emplace_back(std::move(m));
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
        result_matcher_ = make_shared<parameter_matcher>(annotated_identifier{/*result type name*/}, std::move(prdescr));
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

//optional<size_t> basic_fn_pattern::get_local_variable_index(identifier id) const
//{
//    auto it = local_variables_.find(id);
//    if (it == local_variables_.end()) return {};
//    return static_cast<size_t>(it - local_variables_.begin());
//}

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

    //compiler_task_tracer::task_guard tg = ctx.try_lock_task(pattern_task_id{ *this });
    //if (!tg) {
    //    return std::unexpected(make_error<circular_dependency_error>(make_error<basic_general_error>(location_, "resolving function signature"sv, fn_qname_id())));
    //}

    parameter_match_result rmatch; // function's return

    // prepare binding
    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
    ctx.push_binding(pmd.get());
    SCOPE_EXIT([&ctx] { ctx.pop_binding(); });

    auto estate = ctx.expressions_state();
    ctx.push_chain(pmd->call_expressions);

    // deal with result match
    if (expected_result_type && result_matcher_ && result_matcher_->is_pattern()) {
        auto res = result_matcher_->try_match(ctx, expected_result_type, pmd->bindings, rmatch);
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

    //semantic::expression_list_t::const_iterator prev_argument_expression_it = pmd->call_expressions.end();

    for (auto const& arg : call.args()) { // { argname, expr }
        annotated_identifier const* pargname = arg.name();
        if (pargname) {
            if (named_parameter_matcher const* nmtch = get_matcher(pargname->value); nmtch) {
                auto match = nmtch->try_match(ctx, arg.value(), pmd->bindings, pmd->get_match_result(pargname->value));
                if (!match) return std::unexpected(std::move(match.error()));
                pmd->weight += *match;
            } else if (varnamed_matcher_) {
                if (auto match = varnamed_matcher_->try_match(ctx, arg.value(), pmd->bindings); !match) {
                    return std::unexpected(std::move(match.error()));
                }
            } else {
                // note: in fact must never be here due to the quick check procedure
                return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
            }
            continue;
        }

        if (start_matcher_it == end_matcher_it) {
            return std::unexpected(result_error(arg));
        }

        // to do: build name identifiers for position matchers
        auto exp = (*start_matcher_it)->try_forward_match(ctx, arg.value(), pmd->bindings, pmd->get_match_result(start_matcher_it - matchers_.begin()));
        if (error_storage* err = get<error_storage>(&exp); err) return std::unexpected(std::move(*err));
        if (auto * pmatchweight = get<int>(&exp); pmatchweight) {
            pmd->weight += *pmatchweight;
            ++start_matcher_it;
            continue;
        }
        BOOST_ASSERT(get<parameter_matcher::postpone_t>(&exp));

        for (auto next_matcher_it = start_matcher_it + 1;;) {
            if (next_matcher_it != end_matcher_it) {
                exp = (*next_matcher_it)->try_forward_match(ctx, arg.value(), pmd->bindings, pmd->get_match_result(next_matcher_it - matchers_.begin()));
                if (auto* pmatchweight = get<int>(&exp); pmatchweight) {
                    pmd->weight += *pmatchweight;
                    start_matcher_it = next_matcher_it; ++start_matcher_it;
                    break;
                }
                if (get<parameter_matcher::postpone_t>(&exp)) {
                    ++next_matcher_it;
                    continue;
                }
            }
            // here: next_matcher_it == end_matcher_it || get<error_storage>(&exp) != null
            std::expected<int, error_storage> res;
            for (;;) {
                --next_matcher_it;
                res = (*next_matcher_it)->try_match(ctx, arg.value(), pmd->bindings, pmd->get_match_result(next_matcher_it - matchers_.begin()));
                if (res) {
                    start_matcher_it = next_matcher_it; // this is a variadic matcher
                    pmd->weight += *res;
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

    size_t argpos = 0;
    for (auto const& [param_name, param_mod] : parameters_) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);
        if (varname) continue;
        if (external_name) {
            parameter_match_result& pmr = pmd->get_match_result(external_name->value);
            if (pmr.is_undefined()) {
                // to do: check default
                return std::unexpected(make_error<basic_general_error>(external_name->location, "unmatched parameter"sv, external_name->value));
            }
        } else {
            parameter_match_result& pmr = pmd->get_match_result(argpos++);
            if (pmr.is_undefined()) {
                // to do: check default
                return std::unexpected(make_error<basic_general_error>(internal_name->location, (std::ostringstream() << "unmatched parameter $"sv << argpos).str()));
            }
        }
    }


    if (!pmd->result && result_matcher_) {
        auto res = result_matcher_->apply_binding(ctx, pmd->bindings);
        if (!res) return std::unexpected(std::move(res.error()));
        pmd->result = *res;
    }
    return pmd;
}

std::ostream& basic_fn_pattern::print(unit const& u, std::ostream& ss) const
{
    size_t posargnum = 0;
    bool first = true;
    ss << '(';
    for (auto const& [param_name, param_mod] : parameters_) {
        if (first) first = false;
        else ss << ", "sv;
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);
        if (varname) {
            ss << "... : "sv;
            varnamed_matcher_->print(u, ss);
        } else if (external_name) {
            ss << u.print(external_name->value) << ": "sv;
            named_parameter_matcher const* nm = get_matcher(external_name->value);
            nm->print(u, ss);
        } else {
            parameter_matcher const& m = *matchers_[posargnum++];
            m.print(u, ss);
        }
    }
    ss << ")->"sv;
    if (result_matcher_) {
        result_matcher_->print(u, ss);
    } else {
        ss << "auto";
    }
    return ss;
}

void basic_fn_pattern::build_scope(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();

    small_vector<small_vector<variable_entity*, 2>, 8 > parameters;

    auto name_appender = [this, &ctx, &parameters](parameter_match_result& pmr) {
        size_t pindex = parameters.size();
        for (annotated_identifier name : pmr.internal_names) {
            qname infn_name = ctx.ns() / name.value;
            functional& fnl = ctx.u().fregistry().resolve(infn_name);
            if (pmr.is_constexpr()) {
                BOOST_ASSERT(pmr.result.size() == 1);
                BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ pmr.result.front(), name.location }));
            } else {
                BOOST_ASSERT(pmr.result.size() == 1);
                auto var_entity = make_shared<variable_entity>(pmr.result.front(), fnl.id(), variable_entity::kind::SCOPE_LOCAL);
                ctx.u().eregistry_insert(var_entity);
                BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ var_entity->id(), name.location }));
                if (parameters.size() == pindex) {
                    parameters.push_back(small_vector<variable_entity*, 2>{ var_entity.get() });
                } else {
                    parameters[pindex].push_back(var_entity.get());
                }
            }
        }
    };

    md.for_each_named_match(name_appender);
    size_t named_param_count = parameters.size();

    md.for_each_positional_match(name_appender);

    for (size_t idx = 0; idx < parameters.size(); ++idx) {
        for (variable_entity* ve : parameters[idx]) {
            ve->set_index(static_cast<intptr_t>(idx) - parameters.size());
        }
    }

    integer_literal_entity smpl{ parameters.size() };
    smpl.set_type(u.get(builtin_eid::integer));
    entity const& numargsent = u.eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<integer_literal_entity>(std::move(smpl));
    });
    functional& numargsfn = u.fregistry().resolve(ctx.ns() / u.get(builtin_id::numargs));
    BOOST_VERIFY(!numargsfn.set_default_entity(annotated_entity_identifier{ numargsent.id() }));
}

error_storage runtime_fn_pattern::apply(fn_compiler_context& ctx, qname_identifier functional_id, functional_match_descriptor& md) const
{
    // fnsig -> fn entity
    // if fn entity exists => all nested qnames(functionals) are defined (e.g. argument variables, function body qname)
    // => all nested qnames shoud be defined in the build procedure
    entity_signature sig = md.build_signature(ctx.u(), functional_id);
    indirect_signatured_entity smpl{ sig };

    entity& e = ctx.u().eregistry_find_or_create(smpl, [this, &ctx, &md, &sig]() {
        return build(ctx, md, std::move(sig));
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
    semantic::managed_expression_list args{ ctx.u() };
    
    size_t argpos = 0;
    for (auto const& [param_name, param_mod] : parameters_) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);

        if (param_mod != parameter_constraint_modifier_t::value_type_constraint) {
            if (!external_name) ++argpos;
            continue;
        }
        
        parameter_match_result& pmr = external_name ? 
            md.get_match_result(external_name->value) : md.get_match_result(argpos++);
        
        for (auto rng : pmr.expressions) {
            ++rng.second;
            args.splice_back(md.call_expressions, rng.first, rng.second);
        }
        if (pmr.is_variadic()) {
            ctx.u().push_back_expression(args, semantic::push_value(mp::integer{ pmr.result.size() }));
        }
    }

    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered
    ctx.u().push_back_expression(args, semantic::invoke_function(e.id()));
    ctx.append_expression(ctx.store_semantic_expressions(std::move(args)));
    ctx.context_type = rt;
    return {};
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
        entity_identifier eid = fnl.default_entity(ctx);
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
        entity_identifier eid = fnl.default_entity(ctx);
        if (eid) {
            return this->operator()(eid, aqi.location);
        }
        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr_it.next_expression()));
    }

    result_type operator()(entity_identifier const& eid, lex::resource_location eidloc = {}) const
    {
        entity const& param_entity = ctx.u().eregistry_get(eid);
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
        auto opteid = fnl.default_entity(ctx);
        if (!opteid || *opteid) return opteid;
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


generic_fn_pattern::generic_fn_pattern(fn_compiler_context& ctx, functional const& fnl, fn_decl_t const& fnd)
    : runtime_fn_pattern{ ctx, fnl, fnd }
    , body_{ fnd.body }
    , kind_{ fnd.kind }
{ }

std::expected<entity_identifier, error_storage> generic_fn_pattern::const_apply(fn_compiler_context& ctx, qname_identifier, functional_match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("generic_fn_pattern::const_apply");
}

shared_ptr<entity> generic_fn_pattern::build(fn_compiler_context& ctx, functional_match_descriptor& md, entity_signature&& signature) const
{
    qname_view fnqn = fn_qname();
    qname fn_ns = fnqn / ctx.u().new_identifier();

    fn_compiler_context ent_ctx{ ctx, fn_ns };
    build_scope(ent_ctx, md);

#if 0
    auto append_local_var = [&u](qname_view varname, entity_identifier eid, lex::resource_location loc) {
        functional& varfn = u.fregistry().resolve(std::move(varname));
        auto var_entity = make_shared<variable_entity>(eid, varfn.id(), variable_entity::kind::SCOPE_LOCAL);
        u.eregistry_insert(var_entity);
        BOOST_VERIFY(!varfn.set_default_entity(annotated_entity_identifier{ var_entity->id(), std::move(loc) }));
        return var_entity;
    };

    auto append_local_const = [&u](qname_view varname, annotated_entity_identifier aeid) {
        functional& constfn = u.fregistry().resolve(std::move(varname));
        BOOST_VERIFY(!constfn.set_default_entity(aeid));
    };

    std::array<char, 16> argname = { '$' };

    size_t posarg_num = 0, paramnum = 0;
    size_t paramcount = 0; // +fnctx.captured_variables.size();
    for (auto const& f : signature.positioned_fields()) {
        if (!f.is_const()) ++paramcount;
    }
    for (auto const& [_, f] : signature.named_fields()) {
        if (!f.is_const()) ++paramcount;
    }
    for (auto const& [param_name, param_mod] : parameters_) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);
        if (external_name) { // named parameter
            field_descriptor const* fd = signature.find_field(external_name->value);
            BOOST_ASSERT(fd);
            qname infn_name = fn_ns / ((internal_name && internal_name->value) ? internal_name->value : external_name->value);
            if (!fd->is_const()) {
                BOOST_ASSERT(param_mod == parameter_constraint_modifier_t::value_type_constraint);
                auto var_entity = append_local_var(infn_name, fd->entity_id(), external_name->location);
                var_entity->set_index(paramnum - paramcount);
                ++paramnum;
            } else {
                append_local_const(infn_name, annotated_entity_identifier{ fd->entity_id(), external_name->location });
            }
        } else {
            field_descriptor const* fd = signature.find_field(posarg_num);
            BOOST_ASSERT(fd);

            bool reversed = false;
            char* epos = mp::to_string(span{ &posarg_num, 1 }, argname.data() + 1, reversed);
            if (reversed) std::reverse(argname.data() + 1, epos);
            identifier posargid = u.slregistry().resolve(string_view{ argname.data(), epos });
            qname qposarg = fn_ns / posargid;

            if (!fd->is_const()) {
                BOOST_ASSERT(param_mod == parameter_constraint_modifier_t::value_type_constraint);
                auto var_entity = append_local_var(qposarg, fd->entity_id(), {});
                var_entity->set_index(paramnum - paramcount);

                if (internal_name && internal_name->value) {
                    var_entity = append_local_var(fn_ns / internal_name->value, fd->entity_id(), internal_name->location);
                    var_entity->set_index(paramnum - paramcount);
                }
                ++paramnum;
            } else {
                append_local_const(qposarg, annotated_entity_identifier{ fd->entity_id() });
                if (internal_name && internal_name->value) {
                    append_local_const(fn_ns / internal_name->value, annotated_entity_identifier{ fd->entity_id(), internal_name->location });
                }
            }
            ++posarg_num;
        }
    }

    argname[1] = '$';
    identifier numargid = u.slregistry().resolve(string_view{ argname.data(), 2 });

    integer_literal_entity smpl{ posarg_num };
    smpl.set_type(u.get(builtin_eid::integer));
    entity const& numargent = u.eregistry_find_or_create(smpl, [&smpl]() {
        return make_shared<integer_literal_entity>(std::move(smpl));
    });
    functional& numargfn = u.fregistry().resolve(fn_ns / numargid);
    BOOST_VERIFY(!numargfn.set_default_entity(annotated_entity_identifier{ numargent.id() }));
#endif
    auto pife = make_shared<internal_function_entity>(
        ctx.u(),
        qname{ ent_ctx.ns() },
        std::move(signature),
        /*std::move(md.bindings),*/ body_);

    pife->set_inline(kind_ == fn_kind::INLINE);

    return pife;
}

}
