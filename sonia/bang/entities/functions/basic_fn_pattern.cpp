//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_fn_pattern.hpp"

#include <utility>

#include <boost/range/adaptor/reversed.hpp>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
#include "sonia/bang/entities/functional_entity.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"
#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
#include "sonia/bang/entities/ellipsis/variable_pack_entity.hpp"
#include "sonia/bang/entities/variable_entity.hpp"

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

parameter_matcher::parameter_matcher(annotated_identifier name, parameter_constraint_modifier_t mod, parameter_constraint_set_t cs) noexcept
    : modifier_{ mod }
    , constraints_ { std::move(cs) }
{
    internal_names_.emplace_back(name.value);
    if (auto const& optexpr = constraints_.type_expression; optexpr) {
        if (bang_parameter_pack_t const* pp = get<bang_parameter_pack_t>(&*optexpr); pp) {
            variadic = true;
            syntax_expression_t texpr = std::move(pp->type);
            constraints_.type_expression = std::move(texpr);
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
    switch (modifier_) {
    case parameter_constraint_modifier_t::value_constraint:
        ss << "const "; break;
    case parameter_constraint_modifier_t::typename_constraint:
        ss << "typename "; break;
    }
    if (syntax_expression_t const* pte = get_pointer(constraints_.type_expression); pte) {
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
    functional_binding& binding;

    value_type_constraint_visitor(fn_compiler_context& c, functional_binding& b)
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

// match const or type expressions
struct type_constraint_match_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& caller_ctx;
    fn_compiler_context& callee_ctx;
    syntax_expression_t const& cexpr;
    functional_binding& binding; // to forward it to signature_matcher_visitor
    bool matching_type;

    type_constraint_match_visitor(fn_compiler_context& caller, fn_compiler_context& callee, syntax_expression_t const& ce, functional_binding& b, bool is_type_matching = false)
        : caller_ctx{ caller }, callee_ctx{ callee }, cexpr{ ce }, binding{ b }, matching_type { is_type_matching }
    {}

    result_type operator()(annotated_qname_identifier const& aqi) const
    {
        functional const& fnl = callee_ctx.u().fregistry().resolve(aqi.value);
        return fnl.default_entity(callee_ctx);
        //if (!opteid) return opteid;
        //if (*opteid) {
        //    return this->operator()(*opteid, aqi.location);
        //}

        //return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr));
    }

    result_type operator()(function_call_t const& fc) const
    {
        unit& u = callee_ctx.u();

        ct_expression_visitor sv{ callee_ctx, annotated_entity_identifier{ u.get(builtin_eid::qname) } };
        auto qn_ent_id = apply_visitor(sv, fc.fn_object);
        if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
        qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u.eregistry_get(*qn_ent_id));

        // check if can evaluate signature_pattern_
        auto match = callee_ctx.find(qname_ent.value(), fc);
        if (match) {
            THROW_NOT_IMPLEMENTED_ERROR("type_constraint_match_visitor not implemented expression");
        }

        // can't evaluate signature_pattern_ as a function, consider as a pattern
        // evaluate cexpr in caller context
        auto res = apply_visitor(ct_expression_visitor{ caller_ctx }, cexpr);
        if (!res) return std::unexpected(std::move(res.error()));

        entity const& type_ent = u.eregistry_get(*res);
        entity_signature const* psig = type_ent.signature();
        if (!psig) {
            return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, cexpr));
        }

        if (qname_ent.value() != psig->name()) {
            return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, cexpr));
        }

        size_t arg_index = 0;
        for (auto const& arg : fc.args()) {
            annotated_identifier const* argname = arg.name();
            field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
            if (!pfd) {
                return std::unexpected(make_error<basic_general_error>(fc.location(), "argument pattern mismatch"sv, cexpr));
            }
            BOOST_ASSERT(pfd->is_const());
            signature_matcher_visitor smvis{ callee_ctx, binding, pfd->entity_id() };
            if (auto err = apply_visitor(smvis, arg.value()); err) return std::unexpected(std::move(err));
        }

        return type_ent.id();
    }

    result_type operator()(variable_identifier const& var) const
    {
        auto opteid = callee_ctx.lookup_entity(var.name);
        if (!opteid) return opteid;
        if (!*opteid) {
            if (var.implicit) {
                auto res = apply_visitor(ct_expression_visitor{ caller_ctx }, cexpr);
                if (!res) return std::unexpected(std::move(res.error()));
                identifier varid = *var.name.value.begin();
                binding.emplace_back(annotated_identifier{ varid, var.name.location }, *res);
                return res;
            }
            return std::unexpected(make_error<undeclared_identifier_error>(var.name));
        }

        return apply_visitor(ct_expression_visitor{ caller_ctx, annotated_entity_identifier{ *opteid }, matching_type }, cexpr);
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("type_constraint_match_visitor not implemented expression");
    }
};

struct value_type_constraint_match_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& caller_ctx;
    fn_compiler_context& callee_ctx;
    syntax_expression_t const& expr;
    functional_binding& binding; // to forward it to signature_matcher_visitor
    mutable int weight = 0;

    value_type_constraint_match_visitor(fn_compiler_context& caller, fn_compiler_context& callee, syntax_expression_t const& e, functional_binding& b)
        : caller_ctx{ caller }, callee_ctx{ callee }, expr{ e }, binding{ b }
    {}

    result_type operator()(annotated_qname_identifier const& aqi) const
    {
        functional const& fnl = callee_ctx.u().fregistry().resolve(aqi.value);
        auto opteid = fnl.default_entity(callee_ctx);
        if (!opteid) return opteid;
        if (*opteid) {
            return this->operator()(*opteid, aqi.location);
        }

        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr));
    }

    result_type operator()(entity_identifier const& eid, lex::resource_location eidloc = {}) const
    {
        entity const& param_entity = callee_ctx.u().eregistry_get(eid);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            //syntax_expression_t const& expr = expr_it.next_expression();
            expression_visitor evis{ caller_ctx, { pent->element_type(), get_start_location(expr) } };
            auto res = apply_visitor(evis, expr);
            if (!res) return std::unexpected(std::move(res.error()));
        } else {
            BOOST_ASSERT(eid);
            expression_visitor evis{ caller_ctx, { eid, eidloc } };
            auto res = apply_visitor(evis, expr);
            if (!res) {
                return std::unexpected(std::move(res.error()));
            } else if (res.value()) { --weight; }
        }
        return caller_ctx.context_type;

        //GLOBAL_LOG_INFO() << ctx.u().print(eid);
        //GLOBAL_LOG_INFO() << ctx.u().print(expr);
        //THROW_NOT_IMPLEMENTED_ERROR("arg_resolve_visitor(entity_identifier) not implemented expression");
    }

    result_type operator()(variable_identifier const& var) const
    {
        auto opteid = callee_ctx.lookup_entity(var.name);
        if (!opteid) return opteid;
        if (!*opteid) {
            if (var.implicit) {
                // bind variable
                auto res = apply_visitor(expression_visitor{ caller_ctx }, expr);
                if (!res) return std::unexpected(std::move(res.error()));
                identifier varid = *var.name.value.begin();
                binding.emplace_back(annotated_identifier{ varid, var.name.location }, caller_ctx.context_type);
                return caller_ctx.context_type;
            }
            return std::unexpected(make_error<undeclared_identifier_error>(var.name));
        }

        auto res = apply_visitor(expression_visitor{ caller_ctx, annotated_entity_identifier{*opteid, var.name.location} }, expr);
        if (!res) return std::unexpected(std::move(res.error()));

        weight -= *res ? 1 : 0;
        return caller_ctx.context_type;

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
        unit& u = caller_ctx.u();

        ct_expression_visitor sv{ callee_ctx, annotated_entity_identifier{ u.get(builtin_eid::qname) } };
        auto qn_ent_id = apply_visitor(sv, fc.fn_object);
        if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
        qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u.eregistry_get(*qn_ent_id));

        // check if can evaluate signature_pattern_
        auto match = callee_ctx.find(qname_ent.value(), fc);
        if (match) {
            if (auto result = match->const_apply(callee_ctx); result) {
                expression_visitor evis{ caller_ctx, annotated_entity_identifier{ *result, fc.location() } };
                auto res = apply_visitor(evis, expr);
                if (!res) return std::unexpected(std::move(res.error()));
                weight -= *res ? 1 : 0;
                return caller_ctx.context_type;
            }
        }

        // can't evaluate signature_pattern_ as a function, consider as a pattern
        auto res = apply_visitor(expression_visitor{ caller_ctx }, expr);
        if (!res) return std::unexpected(std::move(res.error()));

        entity const& type_ent = u.eregistry_get(caller_ctx.context_type);
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
            signature_matcher_visitor smvis{ callee_ctx, binding, pfd->entity_id() };
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
        auto res = apply_visitor(expression_visitor{ caller_ctx }, expr);
        if (!res) return std::unexpected(std::move(res.error()));
        weight = -1; // template match
        return caller_ctx.context_type;
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_match_visitor not implemented expression");
    }
};

variant<int, parameter_matcher::postpone_t, error_storage> parameter_matcher::try_forward_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, syntax_expression_t const& se, functional_binding_set& b, parameter_match_result& mr) const
{
    if (variadic) return postpone_t{};
    auto res = try_match(caller_ctx, callee_ctx, se, b, mr);
    if (!res) return std::move(res.error());
    return *res;
}

void parameter_matcher::bind_names(span<const annotated_identifier> names, entity_identifier type_or_value, functional_binding& binding) const
{
    switch (modifier_) {
    case parameter_constraint_modifier_t::value_type_constraint:
    {
        shared_ptr<entity> pentity = make_shared<variable_entity>(type_or_value, qname_identifier{}, variable_entity::kind::SCOPE_LOCAL);
        for (auto const& iname : names) {
            binding.emplace_back(iname, pentity);
        }
        break;
    }

    default:
        for (auto const& iname : names) {
            binding.emplace_back(iname, type_or_value);
        }
    }
}

void parameter_matcher::update_binding(unit& u, entity_identifier type_or_value, functional_binding& binding) const
{
    BOOST_ASSERT(!internal_names_.empty());
    annotated_identifier argid = internal_names_.front();
    // all internal names should be bound to the same entity, so we can lookup any of them
    
    if (is_variadic()) {
        // make up an identifier for the pack
        annotated_identifier packargname_id{ u.new_identifier(), argid.location };
        entity_identifier packargname_eid = u.eregistry_find_or_create(packargname_id.value).id();
        entity_signature* psig;

        auto optpack = binding.lookup(argid.value);
        if (!optpack) {
            auto pent = make_shared<basic_signatured_entity>(u.get(builtin_eid::metaobject), entity_signature{}); // empty pack
            psig = pent->signature();
            for (annotated_identifier const& iname : internal_names_) {
                binding.emplace_back(iname, pent);
            }
        } else {
            shared_ptr<basic_signatured_entity> pent = dynamic_pointer_cast<basic_signatured_entity>(*get<shared_ptr<entity>>(&*optpack));
            psig = pent->signature();
        }
        psig->push_back(field_descriptor{ packargname_eid, true });
        bind_names(span<const annotated_identifier>{&packargname_id, 1}, type_or_value, binding);
    } else {
        bind_names(internal_names_, type_or_value, binding);
    }
}

std::expected<int, error_storage> parameter_matcher::try_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, syntax_expression_t const& e, functional_binding& binding, parameter_match_result& mr) const
{
    //if (constraint.which()) { // not empty
    int weight;
    switch (modifier_) {
        case parameter_constraint_modifier_t::value_type_constraint:
        {
            entity_identifier vtype_result;
            auto last_expr_it = caller_ctx.expressions().last();
            if (auto const& optexpr = constraints_.type_expression; optexpr) {
                value_type_constraint_match_visitor vtcv{ caller_ctx, callee_ctx, e, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::unexpected(std::move(res.error()));
                vtype_result = *res;
                mr.append_result(is_variadic(), vtype_result, last_expr_it, caller_ctx.expressions());
                weight = vtcv.weight;
            } else {
                expression_visitor evis{ caller_ctx };
                auto res = apply_visitor(evis, e);
                if (!res) return std::unexpected(std::move(res.error()));
                vtype_result = caller_ctx.context_type;
                mr.append_result(is_variadic(), vtype_result, last_expr_it, caller_ctx.expressions());
                weight = -1; // unspecified type constraint has less priority than specified one
            }
            
            update_binding(caller_ctx.u(), vtype_result, binding);
            break;
        }
        case parameter_constraint_modifier_t::typename_constraint:
        {
            ct_expression_visitor evis{ caller_ctx };
            auto argres = apply_visitor(evis, e);
            if (!argres) return std::unexpected(std::move(argres.error()));

            if (auto const& optexpr = constraints_.type_expression; optexpr) {
                type_constraint_match_visitor vtcv{ caller_ctx, callee_ctx, annotated_entity_identifier{ *argres, get_start_location(e) }, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::unexpected(std::move(res.error())); // perhaps pattern?
                //if (*res != *argres) {
                //    return std::unexpected(make_error<type_mismatch_error>(get_start_location(e), *argres, *res, get_start_location(*optexpr)));
                //}
                weight = 0;
            } else {
                weight = -1; // unspecified type constraint has less priority than specified one
            }
            mr.append_result(is_variadic(), *argres);
            update_binding(caller_ctx.u(), *argres, binding); // bind const value
            break;
            //for (identifier name : internal_names_) {
            //    binding.emplace_back(name, *argres); // bind typename
            //}
            //THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
        }
        case parameter_constraint_modifier_t::value_constraint:
        {
            if (auto const& optexpr = constraints_.type_expression; optexpr) {
                value_type_constraint_visitor vtcv{ callee_ctx, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::unexpected(std::move(res.error())); // perhaps pattern?

                ct_expression_visitor evis{ caller_ctx, annotated_entity_identifier{ *res, get_start_location(*optexpr) } };
                auto argres = apply_visitor(evis, e);
                if (!argres) return std::unexpected(std::move(argres.error()));
                mr.append_result(is_variadic(), *argres);
                update_binding(caller_ctx.u(), *argres, binding); // bind const value
                weight = 0;
                break;
            }

            THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
        }
    }
    
    return weight;
}

class varnamed_parameter_binding : public functional_binding
{
    functional_binding& binding_;
    annotated_identifier const& argname_;

public:
    varnamed_parameter_binding(annotated_identifier const& argname, functional_binding& b) 
        : binding_{ b }, argname_{ argname }
    {}

    inline annotated_identifier const& argname() const noexcept { return argname_; }
    inline functional_binding& base() const noexcept { return binding_; }

    inline optional<value_type> lookup(identifier id) const noexcept override final
    {
        return binding_.lookup(id);
    }

    inline void emplace_back(annotated_identifier id, value_type value) override final
    {
        binding_.emplace_back(std::move(id), std::move(value));
    }
};

std::expected<int, error_storage> varnamed_parameter_matcher::try_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, annotated_identifier argname, syntax_expression_t const& argexpr, functional_binding& binding, parameter_match_result& mr) const
{
    varnamed_parameter_binding vb{ argname, binding };
    auto r = parameter_matcher::try_match(caller_ctx, callee_ctx, argexpr, vb, mr);
    if (!r) return r;
    return *r - 1; // due to variadic nature
}

void varnamed_parameter_matcher::update_binding(unit& u, entity_identifier type_or_value, functional_binding& binding) const
{
    BOOST_ASSERT(internal_names_.size() == 1);
    annotated_identifier name = internal_names_.front();
    BOOST_ASSERT(name);

    varnamed_parameter_binding& vb = static_cast<varnamed_parameter_binding&>(binding);
    annotated_identifier argid = vb.argname();
    entity_identifier argname_eid = u.eregistry_find_or_create(argid.value).id();
    
    auto optval = binding.lookup(name.value);
    if (!optval) {
        entity_signature sig{};
        sig.push_back(field_descriptor{ argname_eid, true });
        binding.emplace_back(name, make_shared<basic_signatured_entity>(u.get(builtin_eid::metaobject), std::move(sig)));
    } else {
        // push argname_eid to signature only if it is not already there
        shared_ptr<basic_signatured_entity> pentity = dynamic_pointer_cast<basic_signatured_entity>(*get<shared_ptr<entity>>(&*optval));
        BOOST_ASSERT(pentity);
        entity_signature& sig = *pentity->signature();
        if (sig.positioned_fields().end() == std::find_if(sig.positioned_fields().begin(), sig.positioned_fields().end(),
            [argname_eid](field_descriptor const& fd) { return fd.entity_id() == argname_eid; })) {
            sig.push_back(field_descriptor{ argname_eid, true });
        }
    }

    if (is_variadic()) {
        // make up an identifier for the pack
        annotated_identifier packargname_id { u.new_identifier(), name.location };
        entity_identifier packargname_eid = u.eregistry_find_or_create(packargname_id.value).id();
        entity_signature* psig;
        
        auto optpack = binding.lookup(argid.value);
        if (!optpack) {
            auto pent = make_shared<basic_signatured_entity>(u.get(builtin_eid::metaobject), entity_signature{}); // empty pack
            psig = pent->signature();
            binding.emplace_back(argid, std::move(pent));
        } else {
            shared_ptr<basic_signatured_entity> pent = dynamic_pointer_cast<basic_signatured_entity>(*get<shared_ptr<entity>>(&*optpack));
            psig = pent->signature();
        }
        psig->push_back(field_descriptor{ packargname_eid, true });
        bind_names(span<const annotated_identifier>{&packargname_id, 1}, type_or_value, vb.base());
    } else {
        bind_names(span<const annotated_identifier>{&argid, 1}, type_or_value, vb.base());
    }
}

//std::expected<std::pair<entity_identifier, error_storage>, error_storage> varnamed_matcher::try_match(fn_compiler_context& ctx, annotated_identifier const& name, syntax_expression_t const& s, functional_match_descriptor& md) const
//{
//    return parameter_matcher::try_match(ctx, s, md.bindings, md.get_match_result(name.value));
//    THROW_NOT_IMPLEMENTED_ERROR("varnamed_matcher::match_forward");
//}

//std::expected<int, error_storage> named_parameter_matcher::try_match(fn_compiler_context& ctx, syntax_expression_t const& e, functional_binding_set& bs, parameter_match_result& mr) const
//{
//    return impl_->try_match(ctx, e, bs, mr);
//    //THROW_NOT_IMPLEMENTED_ERROR("named_parameter_matcher::try_match");
//}

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

basic_fn_pattern::basic_fn_pattern(functional const& fnl) noexcept
    : fnl_{ fnl }, has_varpack_{ 0 }
{ }

error_storage basic_fn_pattern::init(fn_compiler_context& ctx, fn_pure_t const& fnd)
{
    location_ = fnd.location();

    auto& params = fnd.parameters;
    parameters_.reserve(params.size());

    // auxiliary
    std::array<char, 16> argname = { '$' };

    for (auto& param : params) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param.name);

        parameters_.emplace_back(
            external_name ? *external_name : annotated_identifier{},
            std::initializer_list<annotated_identifier>{},
            param.value,
            param.modifier,
            varname);
        
        if (varname) {
            BOOST_ASSERT(internal_name);
            if (varnamed_matcher_) {
                return make_error<basic_general_error>(external_name->location, "Multiple variadic named parameters are not allowed."sv, external_name->value);
            }
            varnamed_matcher_.emplace(*internal_name, param.modifier, param.constraints);
            parameters_.back().inames.emplace_back(*internal_name);
        } else if (!external_name) {
            // auto numeration internal parameter
            size_t argindex = matchers_.size();
            bool reversed = false;
            char* epos = mp::to_string(span{ &argindex, 1 }, argname.data() + 1, reversed);
            if (reversed) std::reverse(argname.data() + 1, epos);
            identifier nid = ctx.u().slregistry().resolve(string_view{ argname.data(), epos });
            annotated_identifier generated_internal_name{ nid, /* to do: get location of constraint */ };
            auto m = make_shared<parameter_matcher>(generated_internal_name, param.modifier, param.constraints);
            if (internal_name) { 
                m->push_internal_name(*internal_name);
                parameters_.back().inames.emplace_back(*internal_name);
            }
            parameters_.back().inames.emplace_back(generated_internal_name);
            matchers_.emplace_back(std::move(m));
        } else {
            parameters_.back().inames.emplace_back(internal_name ? *internal_name : *external_name);
            auto it = named_matchers_.find(external_name->value);
            if (it != named_matchers_.end()) {
                // change type to to some variadic?
                return make_error<basic_general_error>(it->external_name().location, "duplicate parameter name"sv, it->external_name().value);
            }
            named_matchers_.emplace_hint(it, *external_name, internal_name, param.modifier, param.constraints);
        }
    }

    if (fnd.result) {
        result_constraints_.emplace(parameter_constraint_set_t{ .type_expression = *fnd.result });
    }

    return {};
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

    // prepare binding
    fn_compiler_context callee_ctx{ ctx, fnl_.name() };
    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
    callee_ctx.push_binding(pmd->bindings);
    callee_ctx.push_chain(pmd->call_expressions); // for default value expressions
    if (varnamed_matcher_) { pmd->weight -= 1; }
    //SCOPE_EXIT([&ctx] { ctx.pop_binding(); }); //no need, temporary context

    auto estate = ctx.expressions_state();
    ctx.push_chain(pmd->call_expressions);

    
    // deal with result match
    if (expected_result_type && result_constraints_) {
        if (auto const& optexpr = result_constraints_->type_expression; optexpr) {
            type_constraint_match_visitor vtcv{ ctx, callee_ctx, expected_result_type, pmd->bindings };
            auto res = apply_visitor(vtcv, *optexpr);
            if (!res) return std::unexpected(std::move(res.error()));
            pmd->result = expected_result_type.value;
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern : match not a type result");
        }
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
                auto match = nmtch->try_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(pargname->value));
                if (!match) return std::unexpected(std::move(match.error()));
                pmd->weight += *match;
            } else if (varnamed_matcher_) {
                auto match = varnamed_matcher_->try_match(ctx, callee_ctx , *pargname, arg.value(), pmd->bindings, pmd->get_match_result(pargname->value));
                if (!match) return std::unexpected(std::move(match.error()));
                pmd->weight += *match;
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
        auto exp = (*start_matcher_it)->try_forward_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(start_matcher_it - matchers_.begin()));
        if (error_storage* err = get<error_storage>(&exp); err) return std::unexpected(std::move(*err));
        if (auto * pmatchweight = get<int>(&exp); pmatchweight) {
            pmd->weight += *pmatchweight;
            ++start_matcher_it;
            continue;
        }
        BOOST_ASSERT(get<parameter_matcher::postpone_t>(&exp));

        for (auto next_matcher_it = start_matcher_it + 1;;) {
            if (next_matcher_it != end_matcher_it) {
                exp = (*next_matcher_it)->try_forward_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(next_matcher_it - matchers_.begin()));
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
                res = (*next_matcher_it)->try_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(next_matcher_it - matchers_.begin()));
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
    }

    size_t argpos = 0;
    for (parameter_descriptor const& pd : parameters_) {
        //auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);
        if (pd.is_varnamed) continue;
        if (pd.ename) {
            parameter_match_result& pmr = pmd->get_match_result(pd.ename.value);
            if (pmr.is_undefined()) {
                if (pd.default_value) {
                    named_parameter_matcher const* nmtch = get_matcher(pd.ename.value);
                    BOOST_ASSERT(nmtch);
                    auto match = nmtch->try_match(callee_ctx, callee_ctx, *pd.default_value, pmd->bindings, pmr);
                    if (!match) return std::unexpected(std::move(match.error()));
                    pmd->weight += *match;
                    continue;
                }
                return std::unexpected(make_error<basic_general_error>(pd.ename.location, "unmatched parameter"sv, pd.ename.value));
            }
        } else {
            parameter_match_result& pmr = pmd->get_match_result(argpos++);
            if (pmr.is_undefined()) {
                // to do: check default
                return std::unexpected(make_error<basic_general_error>(pd.inames.front().location, (std::ostringstream() << "unmatched parameter $"sv << argpos).str()));
            }
        }
    }


    if (!pmd->result && result_constraints_) {
        if (auto const& optexpr = result_constraints_->type_expression; optexpr) {
            value_type_constraint_visitor vtcv{ ctx, pmd->bindings };
            auto res = apply_visitor(vtcv, *optexpr);
            if (!res) return std::unexpected(std::move(res.error()));
            pmd->result = *res;
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern : match not a type result");
        }
    }
    return pmd;
}

std::ostream& basic_fn_pattern::print(unit const& u, std::ostream& ss) const
{
    size_t posargnum = 0;
    bool first = true;
    ss << '(';
    for (parameter_descriptor const& pd : parameters_) {
        if (first) first = false;
        else ss << ", "sv;
        //auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param_name);
        if (pd.is_varnamed) {
            ss << "... : "sv;
            varnamed_matcher_->print(u, ss);
        } else if (pd.ename) {
            ss << u.print(pd.ename.value) << ": "sv;
            named_parameter_matcher const* nm = get_matcher(pd.ename.value);
            nm->print(u, ss);
        } else {
            parameter_matcher const& m = *matchers_[posargnum++];
            m.print(u, ss);
        }
    }
    ss << ")->"sv;
    if (result_constraints_) {
        ss << u.print(*result_constraints_->type_expression);
    } else {
        ss << "auto";
    }
    return ss;
}

void basic_fn_pattern::build_scope(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    intptr_t argindex = 0;

    auto scope_appender = [&ctx, &md, &argindex](annotated_identifier name) {
        unit& u = ctx.u();
        qname infn_name = ctx.ns() / name.value;
        functional& fnl = ctx.u().fregistry().resolve(infn_name);

        optional<functional_binding::value_type> bsp = md.bindings.lookup(name.value);
        BOOST_ASSERT(bsp);
        apply_visitor(make_functional_visitor<void>([&u, &fnl, &name, &argindex](auto const& e) {
            if constexpr (std::is_same_v<std::decay_t<decltype(e)>, entity_ptr>) {
                // e is entity_ptr
                if (variable_entity* pve = dynamic_cast<variable_entity*>(e.get()); pve) {
                    pve->set_index(argindex);
                    pve->name = fnl.id();
                
                    if (!pve->id()) { // need register if has no id
                        u.eregistry_insert(e);
                    }
                    BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ e->id(), name.location }));
                } else {
                    THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::build_scope unknown entity");
                }
            } // else skip not variable entities
        }), *bsp);
    };

    // first process only variables in reverse order
    for (parameter_descriptor const& pd : boost::adaptors::reverse(parameters_))
    {
        BOOST_ASSERT(!pd.ename || pd.inames.size() == 1);
        BOOST_ASSERT(pd.ename || !pd.inames.empty());

        if (pd.modifier != parameter_constraint_modifier_t::value_type_constraint) continue;

        if (!pd.is_varnamed) {
            --argindex;
            for (annotated_identifier name : pd.inames) {
                scope_appender(name);
            }
            continue;
        }

        auto optpackent = md.bindings.lookup(pd.inames.front().value);
        BOOST_ASSERT(optpackent);
        entity_ptr* packent = get<entity_ptr>(&*optpackent);
        BOOST_ASSERT(packent && *packent);
        BOOST_ASSERT((*packent)->get_type() == ctx.u().get(builtin_eid::metaobject));
        entity_signature const* psig = (*packent)->signature();
        BOOST_ASSERT(psig);
        BOOST_ASSERT(psig->named_fields().empty());
        for (auto const& f : boost::adaptors::reverse(psig->positioned_fields())) {
            entity const& packargid = u.eregistry_get(f.entity_id());
            BOOST_ASSERT(packargid.get_type() == ctx.u().get(builtin_eid::identifier));
            identifier_entity const& packargid_ent = static_cast<identifier_entity const&>(packargid);
            --argindex;
            scope_appender(annotated_identifier{ packargid_ent.value(), pd.inames.front().location });
        }
    }

    // bind consts
    md.bindings.for_each([&ctx](identifier name, lex::resource_location const& loc, functional_binding::value_type & value) {
        unit& u = ctx.u();

        entity_identifier eid = apply_visitor(make_functional_visitor<entity_identifier>([&u](auto const& e) {
            if constexpr (std::is_same_v<std::decay_t<decltype(e)>, entity_identifier>) {
                return e;
            } else {
                // skip variables
                if (variable_entity* pve = dynamic_cast<variable_entity*>(e.get()); pve) { return entity_identifier{}; }
                
                if (auto eid = e->id(); eid) return eid;
                if (e->get_type() == u.get(builtin_eid::metaobject)) {
                    //shared_ptr<basic_signatured_entity> pentity = dynamic_pointer_cast<basic_signatured_entity>(e);
                    u.eregistry_insert(e);
                    return e->id();
                }
                entity& ent = *e;
                // need register
                THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::build_scope unknown entity");
            }
        }), value);

        if (eid) {
            qname infn_name = ctx.ns() / name;
            functional& fnl = ctx.u().fregistry().resolve(infn_name);
            BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ eid, loc }));
        }
    });

    //if (external_name) { // ordinary named parameter
    //    BOOST_ASSERT(internal_names.size() == 1);
    //    scope_appender(internal_names.front());
    //    
    //} else { // positional parameter, can have multiple internal names
    //    BOOST_ASSERT(!internal_names.empty());
    //    for (annotated_identifier name : internal_names) {
    //        scope_appender(name);
    //    }
    //}
    /*
    if (param_mod == parameter_constraint_modifier_t::value_type_constraint) {
        // variadic ?
        //auto var_entity = make_shared<variable_entity>(pmr.result.front(), fnl.id(), variable_entity::kind::SCOPE_LOCAL);

        entity_ptr const* ppe = get<entity_ptr>(&*bsp); // should be variable entity
        BOOST_ASSERT(ppe);
        ctx.u().eregistry_insert(*ppe);
        BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ (*ppe)->id(), name.location }));
    } else {
        apply_visitor(make_functional_visitor<void>([&fnl, name](auto const& e) {
            if constexpr (std::is_same_v<std::decay_t<decltype(e)>, entity_ptr>) {
                entity& ent = *e;
                if (!ent.id()) {
                    // need register
                }
            } else {
                BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ e, name.location }));
            }
            //BOOST_ASSERT(e.is_variable());
            //auto const& ve = static_cast<variable_entity const&>(e);
            //BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ ve.id(), internal_names.front().location }));
        }), *bsp);
    }
    */
    /*
    if (param_mod == parameter_constraint_modifier_t::value_type_constraint) {
    parameter_match_result& pmr = md.get_match_result(external_name->value);
    BOOST_ASSERT(!pmr.is_undefined());
            
    if (pmr.is_constexpr()) {
        BOOST_ASSERT(pmr.result.size() == 1);
        BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ pmr.result.front(), name.location }));
    } else {
        BOOST_ASSERT(pmr.result.size() == 1);
        auto var_entity = make_shared<variable_entity>(pmr.result.front(), fnl.id(), variable_entity::kind::SCOPE_LOCAL);
        ctx.u().eregistry_insert(var_entity);
        BOOST_VERIFY(!fnl.set_default_entity(annotated_entity_identifier{ var_entity->id(), name.location }));
    }
    */

#if 0
    small_vector<small_vector<variable_entity*, 2>, 8> parameters;

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
#endif
}

size_t basic_fn_pattern::apply_arguments(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();

    // push call expressions in the right order
    semantic::expression_list_t& exprs = ctx.expressions();

    size_t argcount = 0;
    auto expression_appender = [&u, &exprs, &argcount](functional_match_descriptor& md, parameter_match_result& pmr) {
        for (auto rng : pmr.expressions) {
            ++rng.second;
            exprs.splice_back(md.call_expressions, rng.first, rng.second);
            ++argcount;
        }
        if (pmr.is_variadic()) {
            u.push_back_expression(exprs, semantic::push_value(mp::integer{ pmr.result.size() }));
        }
    };

    size_t argpos = 0;
    for (parameter_descriptor const& pd : parameters_) {

        if (pd.modifier != parameter_constraint_modifier_t::value_type_constraint) {
            if (!pd.ename) ++argpos;
            continue;
        }
        
        if (!pd.is_varnamed) {
            parameter_match_result& pmr = pd.ename ?
                md.get_match_result(pd.ename.value) : md.get_match_result(argpos++);
            expression_appender(md, pmr);
            continue;
        }
        
        auto optpackent = md.bindings.lookup(pd.inames.front().value);
        BOOST_ASSERT(optpackent);
        entity_ptr* packent = get<entity_ptr>(&*optpackent);
        BOOST_ASSERT(packent && *packent);
        BOOST_ASSERT((*packent)->get_type() == ctx.u().get(builtin_eid::metaobject));
        entity_signature const* psig = (*packent)->signature();
        BOOST_ASSERT(psig);
        BOOST_ASSERT(psig->named_fields().empty());
        for (auto const& f : psig->positioned_fields()) {
            entity const& packargid = u.eregistry_get(f.entity_id());
            BOOST_ASSERT(packargid.get_type() == ctx.u().get(builtin_eid::identifier));
            identifier_entity const& packargid_ent = static_cast<identifier_entity const&>(packargid);
            parameter_match_result& pmr = md.get_match_result(packargid_ent.value());
            expression_appender(md, pmr);
        }
    }

    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered
    return argcount;
}

error_storage runtime_fn_pattern::apply(fn_compiler_context& ctx, qname_identifier functional_id, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    // fnsig -> fn entity
    // if fn entity exists => all nested qnames(functionals) are defined (e.g. argument variables, function body qname)
    // => all nested qnames shoud be defined in the build procedure
    entity_signature sig = md.build_signature(u, functional_id);
    indirect_signatured_entity smpl{ sig };

    entity& e = u.eregistry_find_or_create(smpl, [this, &ctx, &md, &sig]() {
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

    apply_arguments(ctx, md);

    u.push_back_expression(ctx.expressions(), semantic::invoke_function(e.id()));

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
            THROW_NOT_IMPLEMENTED_ERROR("arg_resolving_by_value_type_visitor(pack_entity) not implemented expression");
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


generic_fn_pattern::generic_fn_pattern(functional const& fnl)
    : runtime_fn_pattern{ fnl }
    , kind_{ fn_kind::DEFAULT }
{ }

error_storage generic_fn_pattern::init(fn_compiler_context& ctx, fn_decl_t const& fnd)
{
    body_ = fnd.body;
    kind_ = fnd.kind;
    return runtime_fn_pattern::init(ctx, fnd);
}

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
