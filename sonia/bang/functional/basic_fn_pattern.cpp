//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_fn_pattern.hpp"
#include "value_match_visitor.hpp"
#include "value_type_match_visitor.hpp"
#include "signature_matcher.hpp"

#include <utility>

#include <boost/range/adaptor/reversed.hpp>

#include "sonia/utility/scope_exit.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"

//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/entities/functional_entity.hpp"
#include "sonia/bang/entities/functions/internal_function_entity.hpp"
//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
//#include "sonia/bang/entities/ellipsis/variable_pack_entity.hpp"
#include "sonia/bang/entities/variable_entity.hpp"

#include "sonia/bang/errors/circular_dependency_error.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"


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

parameter_matcher::parameter_matcher(annotated_identifier name, parameter_constraint_modifier_t mod, parameter_constraint_set_t cs) noexcept
    : modifier_{ mod }
    , constraints_ { std::move(cs) }
{
    internal_names_.emplace_back(name.value);
    if (auto const& optexpr = constraints_.type_expression; optexpr) {
        if (unary_expression_t const* pp = get<unary_expression_t>(&*optexpr); pp && pp->op == unary_operator_type::ELLIPSIS) {
            variadic = true;
            syntax_expression_t texpr = std::move(pp->args.front().value());
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
    case parameter_constraint_modifier_t::const_value_type:
        ss << "const "; break;
    case parameter_constraint_modifier_t::mutable_value_type:
        ss << "mut "; break;
    case parameter_constraint_modifier_t::const_value:
        ss << "=> "; break;
    case parameter_constraint_modifier_t::value_type:
        break;
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
    semantic::expression_list_t& expressions;
    functional_binding& binding;

    value_type_constraint_visitor(fn_compiler_context& c, semantic::expression_list_t& el, functional_binding& b)
        : ctx{ c }
        , expressions{ el }
        , binding{ b }
    {}

    result_type operator()(annotated_entity_identifier const& aeid) const
    {
        return aeid.value;
    }

    result_type operator()(annotated_bool const& abv) const
    {
        return ctx.u().get(abv.value ? builtin_eid::true_ : builtin_eid::false_);
    }
    
    result_type operator()(variable_reference const& var) const
    {
        auto res = ct_expression_visitor{ ctx, expressions }(var);
        if (!res) return std::unexpected(res.error());
        return res->value;

        //if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
        //    // check for function parameter
        //    identifier varid = *var.name.value.begin();
        //    if (auto opteids = binding.lookup(varid); opteids) {
        //        THROW_NOT_IMPLEMENTED_ERROR("bind variable_reference");
        //    }
        //}

        //functional const* fl = ctx.lookup_functional(var.name.value);
        //if (!fl) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        //entity_identifier eid = fl->default_entity(ctx);
        //if (!eid) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

        //return eid;
        ////THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_visitor not implemented expression");
    }

    result_type operator()(placeholder const&) const
    {
        return entity_identifier{};
    }

    //result_type operator()(opt_named_syntax_expression_list_t const& nel) const
    //{
    //    return ct_expression_visitor{ ctx }(nel);
    //}

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_visitor not implemented expression");
    }
};

variant<int, parameter_matcher::ignore_t, parameter_matcher::postpone_t, error_storage> parameter_matcher::try_forward_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, syntax_expression_t const& se, functional_binding_set& b, parameter_match_result& mr, semantic::expression_list_t& re) const
{
    if (variadic) return postpone_t{};
    return try_match(caller_ctx, callee_ctx, se, b, mr, re);
}

void parameter_matcher::bind_names(span<const annotated_identifier> names, field_descriptor const& type_or_value, functional_binding& binding) const
{
    if (type_or_value.is_const()) {
        for (auto const& iname : names) {
            binding.emplace_back(iname, type_or_value.entity_id());
        }
    } else {
        for (auto const& iname : names) {
            binding.emplace_back(iname, local_variable{ .type = type_or_value.entity_id(), .is_weak = false });
        }
    }
}

void parameter_matcher::update_binding(unit& u, field_descriptor const& type_or_value, functional_binding& binding) const
{
    BOOST_ASSERT(!internal_names_.empty());
    annotated_identifier argid = internal_names_.front();
    // all internal names should be bound to the same entity, so we can lookup any of them
    
    if (is_variadic()) {
        // make up an identifier for the pack
        annotated_identifier packargname_id{ u.new_identifier(), argid.location };
        entity_identifier packargname_eid = u.make_identifier_entity(packargname_id.value).id;
        entity_signature* psig;

        auto optpack = binding.lookup(argid.value);
        if (!optpack) {
            auto pent = make_shared<basic_signatured_entity>(entity_signature{ u.get(builtin_qnid::metaobject) }); // empty pack
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

variant<int, parameter_matcher::ignore_t, error_storage> parameter_matcher::try_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, syntax_expression_t const& e, functional_binding& binding, parameter_match_result& mr, semantic::expression_list_t& re) const
{
    unit& u = caller_ctx.u();
    //if (constraint.which()) { // not empty
    int weight;
    switch (modifier_) {
        case parameter_constraint_modifier_t::value_type:
        case parameter_constraint_modifier_t::mutable_value_type:
        {
            field_descriptor type_or_value;
            
            if (auto const& optexpr = constraints_.type_expression; optexpr) {
                value_type_match_visitor vtcv{ caller_ctx, callee_ctx, re, e, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::move(res.error());
                auto& ser = *res;
                if (ser.is_const_result) {
                    if (modifier_ != parameter_constraint_modifier_t::mutable_value_type) {
                        mr.append_result(ser);
                        type_or_value = field_descriptor{ ser.value(), true };
                    } else {
                        semantic::managed_expression_list l{ u };
                        u.push_back_expression(l, semantic::push_value(ser.value()));
                        type_or_value = field_descriptor{ get_entity(u, ser.value()).get_type(), false };
                        mr.append_result(type_or_value.entity_id(), l);
                        re.splice_back(l);
                    }
                } else {
                    if (ser.type() == u.get(builtin_eid::void_)) return parameter_matcher::ignore_t{};
                    mr.append_result(ser.type(), ser.expressions);
                    type_or_value = field_descriptor{ ser.type(), false };
                }
                weight = vtcv.weight;
            } else {
                auto last_expr_it = caller_ctx.expressions().last();
                auto res = apply_visitor(base_expression_visitor{ caller_ctx, re }, e);
                if (!res) return std::move(res.error());
                mr.append_result(res->first);
                type_or_value = field_descriptor{ res->first.value_or_type, res->first.is_const_result };
                weight = -1; // unspecified type constraint has less priority than specified one
            }
            
            update_binding(caller_ctx.u(), type_or_value, binding);
            break;
        }
        
        case parameter_constraint_modifier_t::const_value_type:
        {
            if (auto const& optexpr = constraints_.type_expression; optexpr) {
                value_type_constraint_visitor vtcv{ callee_ctx, re, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::move(res.error()); // perhaps pattern?

                ct_expression_visitor evis{ caller_ctx, re, annotated_entity_identifier{ *res, get_start_location(*optexpr) } };
                auto argres = apply_visitor(evis, e);
                if (!argres) return std::move(argres.error());
                mr.append_result(*argres);
                update_binding(caller_ctx.u(), field_descriptor{ argres->value, true }, binding); // bind const value
                weight = 0;
                break;
            }

            THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
        }
        case parameter_constraint_modifier_t::const_value:
        {
            ct_expression_visitor evis{ caller_ctx, re };
            auto argres = apply_visitor(evis, e);
            if (!argres) return std::move(argres.error());

            if (auto const& optexpr = constraints_.type_expression; optexpr) {
                value_match_visitor vtcv{ caller_ctx, callee_ctx, re, annotated_entity_identifier{ argres->value, get_start_location(e) }, binding };
                auto res = apply_visitor(vtcv, *optexpr);
                if (!res) return std::move(res.error()); // perhaps pattern?
                //if (*res != *argres) {
                //    return std::unexpected(make_error<type_mismatch_error>(get_start_location(e), *argres, *res, get_start_location(*optexpr)));
                //}
                weight = 0;
            } else {
                weight = -1; // unspecified type constraint has less priority than specified one
            }
            mr.append_result(*argres);
            update_binding(caller_ctx.u(), field_descriptor{ argres->value, true }, binding); // bind const value
            break;
            //for (identifier name : internal_names_) {
            //    binding.emplace_back(name, *argres); // bind typename
            //}
            //THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher::try_match");
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

    inline value_type const* lookup(identifier id) const noexcept override final
    {
        return binding_.lookup(id);
    }

    inline value_type& emplace_back(annotated_identifier id, value_type value) override final
    {
        return binding_.emplace_back(std::move(id), std::move(value));
    }
};

variant<int, parameter_matcher::ignore_t, error_storage> varnamed_parameter_matcher::try_match(fn_compiler_context& caller_ctx, fn_compiler_context& callee_ctx, annotated_identifier argname, syntax_expression_t const& argexpr, functional_binding& binding, parameter_match_result& mr, semantic::expression_list_t& re) const
{
    varnamed_parameter_binding vb{ argname, binding };
    auto r = parameter_matcher::try_match(caller_ctx, callee_ctx, argexpr, vb, mr, re);
    using result_t = variant<int, parameter_matcher::ignore_t, error_storage>;
    return apply_visitor(make_functional_visitor<result_t>([&r](auto& v) -> result_t {
        if constexpr (std::is_same_v<int, std::decay_t<decltype(v)>>) {
            return v - 1; // due to variadic nature
        } else {
            return std::move(v);
        }
    }), r);
}

void varnamed_parameter_matcher::update_binding(unit& u, field_descriptor const& type_or_value, functional_binding& binding) const
{
    BOOST_ASSERT(internal_names_.size() == 1);
    annotated_identifier name = internal_names_.front();
    BOOST_ASSERT(name);

    varnamed_parameter_binding& vb = static_cast<varnamed_parameter_binding&>(binding);
    annotated_identifier argid = vb.argname();
    entity_identifier argname_eid = u.make_identifier_entity(argid.value).id;
    
    auto optval = binding.lookup(name.value);
    if (!optval) {
        entity_signature sig{ u.get(builtin_qnid::metaobject), u.get(builtin_eid::metaobject) };
        sig.push_back(field_descriptor{ argname_eid, true });
        binding.emplace_back(name, make_shared<basic_signatured_entity>(std::move(sig)));
    } else {
        // push argname_eid to signature only if it is not already there
        shared_ptr<basic_signatured_entity> pentity = dynamic_pointer_cast<basic_signatured_entity>(*get<shared_ptr<entity>>(&*optval));
        BOOST_ASSERT(pentity);
        entity_signature& sig = *pentity->signature();
        if (sig.fields().end() == std::find_if(sig.fields().begin(), sig.fields().end(),
            [argname_eid](field_descriptor const& fd) { return fd.entity_id() == argname_eid; })) {
            sig.push_back(field_descriptor{ argname_eid, true });
        }
    }

    if (is_variadic()) {
        // make up an identifier for the pack
        annotated_identifier packargname_id { u.new_identifier(), name.location };
        entity_identifier packargname_eid = u.make_identifier_entity(packargname_id.value).id;
        entity_signature* psig;
        
        auto optpack = binding.lookup(argid.value);
        if (!optpack) {
            auto pent = make_shared<basic_signatured_entity>(entity_signature{ u.get(builtin_qnid::metaobject) }); // empty pack
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

error_storage basic_fn_pattern::init(fn_compiler_context& ctx, parameter_list_t const& parameters)
{
    parameters_.reserve(parameters.size());

    // auxiliary
    std::array<char, 16> argname = { '$' };

    for (auto& param : parameters) {
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
            auto m = sonia::make_shared<parameter_matcher>(generated_internal_name, param.modifier, param.constraints);
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

    return {};
}

error_storage basic_fn_pattern::init(fn_compiler_context& ctx, fn_pure_t const& fnd)
{
    location_ = fnd.location();

    auto err = init(ctx, fnd.parameters);

    if (err) return err;

    if (fnd.result) {
        result_constraints.emplace(parameter_constraint_set_t{ .type_expression = *fnd.result }, parameter_constraint_modifier_t::value_type);
    }

    return err;
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

std::expected<functional_match_descriptor_ptr, error_storage> basic_fn_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& expected_result_type) const
{
    // quick match check
    for (auto const& arg : call.args) {
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
    auto pmd = make_shared<functional_match_descriptor>();
    callee_ctx.push_binding(pmd->bindings);
    
    if (varnamed_matcher_) { pmd->weight -= 1; }
    //SCOPE_EXIT([&ctx] { ctx.pop_binding(); }); //no need, temporary context

    auto estate = ctx.expressions_state();
    

    
    // deal with result match and only when the result value is not a constant
    if (expected_result_type && result_constraints && get<1>(*result_constraints) != parameter_constraint_modifier_t::const_value) {
        if (auto const& optexpr = get<0>(*result_constraints).type_expression; optexpr) {
            value_match_visitor vtcv{ ctx, callee_ctx, call.expressions, expected_result_type, pmd->bindings };
            auto res = apply_visitor(vtcv, *optexpr);
            if (!res) return std::unexpected(std::move(res.error()));
            pmd->result = field_descriptor { expected_result_type.value };
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

    for (auto const& arg : call.args) { // { argname, expr }
        annotated_identifier const* pargname = arg.name();
        if (pargname) {
            if (named_parameter_matcher const* nmtch = get_matcher(pargname->value); nmtch) {
                auto match = nmtch->try_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(pargname->value), call.expressions);
                if (auto* err = get<error_storage>(&match); err) return std::unexpected(std::move(*err));
                if (get<parameter_matcher::ignore_t>(&match)) continue;
                pmd->weight += get<int>(match);
            } else if (varnamed_matcher_) {
                auto match = varnamed_matcher_->try_match(ctx, callee_ctx , *pargname, arg.value(), pmd->bindings, pmd->get_match_result(pargname->value), call.expressions);
                if (auto* err = get<error_storage>(&match); err) return std::unexpected(std::move(*err));
                if (auto const* pignore = get<parameter_matcher::ignore_t>(&match); pignore) continue;
                pmd->weight += get<int>(match);
            } else {
                // note: in fact must never be here due to the quick check procedure
                return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
            }
            continue;
        }

        if (start_matcher_it == end_matcher_it) {
            return std::unexpected(result_error(arg));

            //auto res = apply_visitor(base_expression_visitor{ ctx }, arg.value());
            //if (!res) return std::unexpected(result_error(arg));
            //auto& [el, reid] = res->first;
            //if (el && reid == ctx.u().get(builtin_eid::void_)) continue; // ignore void
        }

        // to do: build name identifiers for position matchers?
        auto exp = (*start_matcher_it)->try_forward_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(start_matcher_it - matchers_.begin()), call.expressions);
        if (error_storage* err = get<error_storage>(&exp); err) return std::unexpected(std::move(*err));
        if (get<parameter_matcher::ignore_t>(&exp)) continue;
        if (auto * pmatchweight = get<int>(&exp); pmatchweight) {
            pmd->weight += *pmatchweight;
            ++start_matcher_it;
            continue;
        }
        BOOST_ASSERT(get<parameter_matcher::postpone_t>(&exp));

        for (auto next_matcher_it = start_matcher_it + 1;;) {
            if (next_matcher_it != end_matcher_it) {
                exp = (*next_matcher_it)->try_forward_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(next_matcher_it - matchers_.begin()), call.expressions);
                if (auto* pmatchweight = get<int>(&exp); pmatchweight) {
                    pmd->weight += *pmatchweight;
                    start_matcher_it = next_matcher_it; ++start_matcher_it;
                    break;
                }
                if (get<parameter_matcher::ignore_t>(&exp))
                    break;
                if (get<parameter_matcher::postpone_t>(&exp)) {
                    ++next_matcher_it;
                    continue;
                }
            }
            // here: next_matcher_it == end_matcher_it || get<error_storage>(&exp) != null
            variant<int, parameter_matcher::ignore_t, error_storage> res;
            for (;;) {
                --next_matcher_it;
                res = (*next_matcher_it)->try_match(ctx, callee_ctx, arg.value(), pmd->bindings, pmd->get_match_result(next_matcher_it - matchers_.begin()), call.expressions);
                
                BOOST_ASSERT(!get<parameter_matcher::ignore_t>(&res)); // how can it be?
                    
                if (int const* w = get<int>(&res); w) {
                    if (get<parameter_matcher::ignore_t>(&exp))
                    start_matcher_it = next_matcher_it; // this is a variadic matcher
                    pmd->weight += *w;
                    break;
                }
                if (next_matcher_it == start_matcher_it) {
                    return std::unexpected(std::move(get<error_storage>(res)));
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
            if (!pmr) {
                if (pd.default_value) {
                    named_parameter_matcher const* nmtch = get_matcher(pd.ename.value);
                    BOOST_ASSERT(nmtch);
                    auto match = nmtch->try_match(callee_ctx, callee_ctx, *pd.default_value, pmd->bindings, pmr, call.expressions);
                    if (error_storage* err = get<error_storage>(&match); err) return std::unexpected(std::move(*err));
                    pmd->weight += get<int>(match);
                    continue;
                }
                return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv, pd.ename.value, pd.ename.location));
            }
        } else {
            parameter_match_result& pmr = pmd->get_match_result(argpos++);
            if (!pmr) {
                // to do: check default
                
                return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv, pd.inames.front().value, pd.inames.front().location));
            }
        }
    }


    if (!pmd->result.entity_id() && result_constraints) {
        if (auto const& optexpr = get<0>(*result_constraints).type_expression; optexpr) {
            value_type_constraint_visitor vtcv{ ctx, call.expressions, pmd->bindings };
            auto res = apply_visitor(vtcv, *optexpr);
            if (!res) return std::unexpected(std::move(res.error()));
            pmd->result = field_descriptor{ *res, get<1>(*result_constraints) == parameter_constraint_modifier_t::const_value };
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
    
    if (result_constraints) {
        if (get<1>(*result_constraints) == parameter_constraint_modifier_t::const_value) {
            ss << ")=>"sv;
        } else {
            ss << ")->"sv;
        }
        ss << u.print(*get<0>(*result_constraints).type_expression);
    } else {
        ss << ")->auto"sv;
    }
    return ss;
}

void basic_fn_pattern::build_scope(unit& u, functional_match_descriptor& md, internal_function_entity& fent) const
{
    auto argument_appender = [&u, &md, &fent](annotated_identifier name) {
        functional_binding::value_type const* bsp = md.bindings.lookup(name.value);
        BOOST_ASSERT(bsp);
        if (local_variable const* plv = get<local_variable>(bsp); plv) {
            local_variable lv = *plv;
            fent.push_argument(name, std::move(lv));
        }
    };

    // first process only variables
    for (parameter_descriptor const& pd : parameters_)
    {
        BOOST_ASSERT(!pd.ename || pd.inames.size() == 1);
        BOOST_ASSERT(pd.ename || !pd.inames.empty());

        if (pd.modifier != parameter_constraint_modifier_t::value_type && pd.modifier != parameter_constraint_modifier_t::mutable_value_type) continue;

        if (!pd.is_varnamed) {
            for (annotated_identifier name : pd.inames) {
                argument_appender(name);
            }
            continue;
        }

        functional_binding::value_type const* optpackent = md.bindings.lookup(pd.inames.front().value);
        BOOST_ASSERT(optpackent);
        entity_ptr const* packent = get<entity_ptr>(&*optpackent);
        BOOST_ASSERT(packent && *packent);
        BOOST_ASSERT((*packent)->signature() && (*packent)->signature()->name == u.get(builtin_qnid::metaobject));
        entity_signature const* psig = (*packent)->signature();
        BOOST_ASSERT(psig);
        BOOST_ASSERT(psig->named_fields_indices().empty());
        for (auto const& f : boost::adaptors::reverse(psig->fields())) {
            entity const& packargid = u.eregistry_get(f.entity_id());
            BOOST_ASSERT(packargid.get_type() == u.get(builtin_eid::identifier));
            identifier_entity const& packargid_ent = static_cast<identifier_entity const&>(packargid);
            parameter_match_result& pmr = md.get_match_result(packargid_ent.value());
            for (auto const& ser : pmr.results) {
                if (!ser.is_const_result) {
                    // to do: fix variadic arguments with the same name
                    argument_appender(annotated_identifier{ packargid_ent.value(), pd.inames.front().location });
                }
            }
        }
    }

    // bind consts
    md.bindings.for_each([&u, &fent](identifier name, lex::resource_location const& loc, functional_binding::value_type & value) {
        entity_identifier eid = apply_visitor(make_functional_visitor<entity_identifier>([&u](auto const& e) {
            if constexpr (std::is_same_v<std::decay_t<decltype(e)>, entity_identifier>) {
                return e;
            } else if constexpr (std::is_same_v<std::decay_t<decltype(e)>, entity_ptr>) {
                if (e->id) return e->id;
                //if (e->get_type() == u.get(builtin_eid::metaobject)) {
                //    return u.eregistry_find_or_create(*e, [&e]() { return std::move(e); }).id();
                //}
                return u.eregistry_find_or_create(*e, [&e]() { return std::move(e); }).id;
            } else { // else skip variables
                return entity_identifier{};
            }
        }), value);

        if (eid) {
            qname infn_name = fent.name() / name;
            functional& fnl = u.fregistry_resolve(infn_name);
            fnl.set_default_entity(annotated_entity_identifier{ eid, loc });
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

// return appended argument expressions count
std::pair<semantic::expression_span, size_t> basic_fn_pattern::apply_mut_argument(parameter_match_result& pmr, semantic::expression_list_t& el) const
{
    std::pair<semantic::expression_span, size_t> result{ semantic::expression_span{}, 0 };
    size_t non_constexpr_count = 0;
    for (auto & ser : pmr.results) {
        if (!ser.is_const_result) {
            ++get<1>(result);
            get<0>(result) = el.concat(get<0>(result), ser.expressions);
        }
    }
    return result;
}

// return appended argument expressions count
std::pair<semantic::expression_span, size_t> basic_fn_pattern::apply_any_argument(unit& u, parameter_match_result& pmr, semantic::expression_list_t& el) const
{
    std::pair<semantic::expression_span, size_t> result{ semantic::expression_span{}, 0 };
    for (auto &ser : pmr.results) {
        get<0>(result) = el.concat(get<0>(result), ser.expressions);
        if (ser.is_const_result) {
            u.push_back_expression(el, get<0>(result), semantic::push_value(ser.value()));
        }
        ++get<1>(result);
    }
    return result;
}

std::pair<semantic::expression_span, size_t> basic_fn_pattern::apply_argument(unit&, parameter_match_result& pmr, semantic::expression_list_t& el) const
{
    return apply_mut_argument(pmr, el);
}

std::pair<semantic::expression_span, size_t>
basic_fn_pattern::apply_arguments(fn_compiler_context& ctx, functional_match_descriptor& md, semantic::expression_list_t& exprs) const
{
    unit& u = ctx.u();
    std::pair<semantic::expression_span, size_t> result{ semantic::expression_span{}, 0 };
    
    // push call expressions in the right order

    size_t& argcount = result.second;
    size_t argpos = 0;
    for (parameter_descriptor const& pd : parameters_) {

        // skip typename arguments
        if (pd.modifier != parameter_constraint_modifier_t::mutable_value_type && pd.modifier != parameter_constraint_modifier_t::value_type) {
            if (!pd.ename) ++argpos;
            continue;
        }
        
        if (!pd.is_varnamed) {
            parameter_match_result& pmr = pd.ename ?
                md.get_match_result(pd.ename.value) : md.get_match_result(argpos++);
            auto rpair = apply_argument(u, pmr, exprs);
            get<0>(result) = exprs.concat(get<0>(result), get<0>(rpair));
            argcount += get<1>(rpair);
            continue;
        }
        
        functional_binding::value_type const* optpackent = md.bindings.lookup(pd.inames.front().value);
        BOOST_ASSERT(optpackent);
        entity_ptr const* packent = get<entity_ptr>(&*optpackent);
        BOOST_ASSERT(packent && *packent);
        BOOST_ASSERT((*packent)->get_type() == ctx.u().get(builtin_eid::metaobject));
        entity_signature const* psig = (*packent)->signature();
        BOOST_ASSERT(psig);
        BOOST_ASSERT(psig->named_fields_indices().empty());
        for (auto const& f : psig->fields()) {
            entity const& packargid = u.eregistry_get(f.entity_id());
            BOOST_ASSERT(packargid.get_type() == ctx.u().get(builtin_eid::identifier));
            identifier_entity const& packargid_ent = static_cast<identifier_entity const&>(packargid);
            parameter_match_result& pmr = md.get_match_result(packargid_ent.value());
            auto rpair = apply_argument(u, pmr, exprs);
            get<0>(result) = exprs.concat(get<0>(result), get<0>(rpair));
            argcount += get<1>(rpair);
        }
    }

    return result;
}

//error_storage runtime_fn_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
//{
//    unit& u = ctx.u();
//    // fnsig -> fn entity
//    // if fn entity exists => all nested qnames(functionals) are defined (e.g. argument variables, function body qname)
//    // => all nested qnames shoud be defined in the build procedure
//    entity_signature sig = md.build_signature(u, fnl_.id());
//    indirect_signatured_entity smpl{ sig };
//
//    entity& e = u.eregistry_find_or_create(smpl, [this, &ctx, &md, &sig]() {
//        return build(ctx, md, std::move(sig));
//    });
//
//    BOOST_ASSERT(dynamic_cast<function_entity*>(&e));
//    function_entity& fne = static_cast<function_entity&>(e);
//    entity_identifier rt = fne.get_result_type();
//
//    if (!rt) { // we need building to resolve result type
//        compiler_task_tracer::task_guard tg = ctx.try_lock_task(entity_task_id{ e });
//        if (!tg) {
//            throw circular_dependency_error({ make_error<basic_general_error>(location_, "resolving function result type"sv, e.id()) });
//        }
//        rt = fne.get_result_type();
//        if (!rt) {
//            fne.build(ctx.u());
//            rt = fne.get_result_type();
//        }
//    }
//
//    apply_arguments(ctx, md);
//
//    u.push_back_expression(ctx.expressions(), semantic::invoke_function(e.id()));
//
//    ctx.context_type = rt;
//    return {};
//}

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
        functional const& fnl = ctx.u().fregistry_resolve(aqi.value);
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
        functional const& fnl = ctx.u().fregistry_resolve(aqi.value);
        entity_identifier eid = fnl.default_entity(ctx);
        if (eid) {
            return this->operator()(eid, aqi.location);
        }
        return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr_it.next_expression()));
    }

    result_type operator()(entity_identifier const& eid, lex::resource_location eidloc = {}) const
    {
        entity const& param_entity = ctx.u().eregistry_get(eid);
        /*
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
        */
            BOOST_ASSERT(eid);
            expression_visitor evis{ ctx, { eid, eidloc } };
            auto res = apply_visitor(evis, expr_it.next_expression());
            if (!res) {
                return std::unexpected(std::move(res.error()));
            } else if (res.value()) { --weight; }
        //}
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
        functional const& fnl = ctx.u().fregistry_resolve(aqi.value);
        entity_identifier eid = fnl.default_entity(ctx);
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

shared_ptr<entity> generic_fn_pattern::build(fn_compiler_context& ctx, functional_match_descriptor& md, entity_signature&& signature) const
{
    qname_view fnqn = fn_qname();
    qname fn_ns = fnqn / ctx.u().new_identifier();

    auto pife = make_shared<internal_function_entity>(
        std::move(fn_ns),
        std::move(signature),
        /*std::move(md.bindings),*/ body_);

    build_scope(ctx.u(), md, *pife);

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
    

    pife->set_inline(kind_ == fn_kind::INLINE);

    return pife;
}

std::expected<syntax_expression_result_t, error_storage> basic_fn_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& exprs, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    if (md.result && md.result.is_const()) {
        return syntax_expression_result_t{ .value_or_type = md.result.entity_id(), .is_const_result = true };
    }
    // fnsig -> fn entity
    // if fn entity exists => all nested qnames(functionals) are defined (e.g. argument variables, function body qname)
    // => all nested qnames should be defined in the build procedure
    entity_signature sig = md.build_signature(u, fnl_.id());
    indirect_signatured_entity smpl{ sig };

    entity& e = u.eregistry_find_or_create(smpl, [this, &ctx, &md, &sig]() {
        return build(ctx, md, std::move(sig));
    });

    BOOST_ASSERT(dynamic_cast<function_entity*>(&e));
    function_entity& fne = static_cast<function_entity&>(e);
    
    if (!fne.result) { // we need building to resolve result type
        compiler_task_tracer::task_guard tg = ctx.try_lock_task(entity_task_id{ e });
        if (!tg) return std::unexpected(
            make_error<circular_dependency_error>(make_error<basic_general_error>(location_, "resolving function result type"sv, e.id))
        );
        if (!fne.result) {
            BOOST_ASSERT(dynamic_cast<internal_function_entity*>(&e));
            static_cast<internal_function_entity&>(e).build(u);
        }
    }
    
    BOOST_ASSERT(fne.result);

    if (fne.result.is_const()) {
        return syntax_expression_result_t{ .value_or_type = fne.result.entity_id(), .is_const_result = true };
    }

    auto rpair = apply_arguments(ctx, md, exprs);
    u.push_back_expression(exprs, get<0>(rpair), semantic::invoke_function(e.id));

    return syntax_expression_result_t{ .expressions = std::move(get<0>(rpair)), .value_or_type = fne.result.entity_id(), .is_const_result = false };
}

}
