//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_fn_pattern.hpp"

#include <boost/container/small_vector.hpp>

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/preliminary_type_visitor.hpp"
#include "sonia/bang/ast/declaration_visitor.hpp"

//#include "sonia/bang/entities/ellipsis/pack_entity.hpp"
#include "sonia/bang/entities/functional_entity.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"
#include "sonia/bang/entities/ellipsis/pack_entity.hpp"

namespace sonia::lang::bang {

basic_fn_pattern::basic_fn_pattern(fn_compiler_context& ctx, fn_pure_t const& fnd)
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
    optional<expected_result_t> expected_result;

    const_expression_visitor(fn_compiler_context& c, expected_result_t&& er)
        : ctx{ c }
        , expected_result{ std::move(er) }
    {}

    result_type operator()(entity_expression const& ee) const
    {
        if (!expected_result || expected_result->type == ee.id) return {};
        return make_error<type_mismatch_error>(ee.location, ee.id, expected_result->type, expected_result->location);
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
    functional::binding_set_t& binding;
    ExpressionIteratorT& expr_it;
    arg_resolving_by_value_visitor(fn_compiler_context& c, functional::binding_set_t& bnd, ExpressionIteratorT& it)
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
        const_expression_visitor evis{ ctx, expected_result_t{ eid, eidloc } };
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
    functional::binding_set_t& binding;
    ExpressionIteratorT & expr_it;
    mutable int weight = 0;
    arg_resolving_by_value_type_visitor(fn_compiler_context& c, functional::binding_set_t& bnd, ExpressionIteratorT & it)
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
                expression_visitor evis{ ctx, expected_result_t{ pent->element_type(), get_start_location(expr) } };
                auto res = apply_visitor(evis, expr);
                if (!res) break; // skip error
                // do not condider cast's weight inpact for pack_entity cases
                ++pack_size;
                auto neres = expr_it.has_next_expression();
                if (!neres) return std::unexpected(std::move(neres.error()));
                if (!neres.value()) break;
            }
            weight -= pack_size; // put down as pack_entity
            ctx.append_expression(semantic::push_value{ uint64_t{pack_size} });
        } else {
            BOOST_ASSERT(eid);
            expression_visitor evis{ ctx, expected_result_t{ eid, eidloc } };
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
    functional::binding_set_t& binding;

    result_resolving_visitor(fn_compiler_context& c, functional::binding_set_t& bnd)
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


class argument_matcher
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
    explicit argument_matcher(function_descriptor const& fd) : fd_{ fd } {}
    
    error_storage setup(pure_call_t const& call)
    {
        for (auto const& tpl : call.named_args) { // { argname, expr }
            annotated_identifier const& argname = std::get<0>(tpl);
            function_descriptor::named_field const* pnf = fd_.find_named_field(argname.value);
            if (!pnf) {
                return make_error<basic_general_error>(argname.location, "unmatched named argument"sv, argname.value);
            }
            named_.emplace(pnf->ename.value, &std::get<1>(tpl));
        }

        for (function_descriptor::named_field const& nf : fd_.named_fields()) {
            auto it = named_.find(nf.ename.value);
            if (it != named_.end()) continue;
            // to do: treat default value if exists
            return make_error<basic_general_error>(call.location(), "unmatched named parameter"sv, nf.ename.value);
        }

        // initialize argument and parameter iterators
        positioned_ = call.positioned_args;
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

    std::expected<int, error_storage> try_match(fn_compiler_context& ctx, functional::match_descriptor& md)
    {
        int match_weight = 0;
        boost::container::small_vector<fn_compiler_context::expr_vec_t, 8> arg_branches;
        size_t rt_argnum = 0, argnum = 0;

        auto estate = ctx.expressions_state();

        for (;;) {
            // need calling first to prepare cur_name_
            //function_descriptor::generic_field const* pparam = has_next_parameter() ? &next_parameter() : nullptr;
            function_descriptor::generic_field const* pparam = next_parameter();
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
                
                for (annotated_identifier const& ai : pparam->bindings) {
                    auto it = md.bindings.find(ai);
                    if (it == md.bindings.end()) {
                        md.bindings.insert(it, std::pair{ ai, param_entity });
                    }
                    else if (it->second != param_entity) {
                        return std::unexpected(make_error<basic_general_error>(get_start_location(current_expression()), "positioned argument mismatch"sv));
                    }
                }
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

std::expected<int, error_storage> basic_fn_pattern::is_matched(fn_compiler_context& ctx, pure_call_t const& call, functional::match_descriptor& md) const
{
    argument_matcher matcher{ fd_ };
    if (auto err = matcher.setup(call); err) return std::unexpected(std::move(err));

    return matcher.try_match(ctx, md);
    
#if 0
    
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

std::expected<entity_identifier, error_storage> basic_fn_pattern::apply(fn_compiler_context& ctx, functional::match_descriptor& md) const
{
    indirect_signatured_entity smpl{ md.signature };

    entity & e = ctx.u().eregistry().find_or_create(smpl, [this, &ctx, &md]() {
        return build(ctx.u(), md);
    });

    BOOST_ASSERT(dynamic_cast<function_entity*>(&e));
    function_entity & fne = static_cast<function_entity&>(e);
    entity_identifier rt = fne.get_result_type();
    if (!rt) { // need build to resolve result type
        if (building_) {
            throw circular_dependency_error({ make_error<basic_general_error>(location_, "resolving function result type"sv, fd_.id()) });
        }
        building_ = true;
        fne.build(ctx.u());
        building_ = false;
        rt = fne.get_result_type();
    }

    ctx.expressions().push_back(semantic::invoke_function(e.id()));
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

generic_fn_pattern::generic_fn_pattern(fn_compiler_context& ctx, fn_decl_t const& fnd)
    : basic_fn_pattern{ ctx, fnd }
    , body_{ make_shared<std::vector<infunction_statement>>(fnd.body)}
    , kind_{ fnd.kind }
{

}

std::expected<entity_identifier, error_storage> generic_fn_pattern::const_apply(fn_compiler_context& ctx, functional::match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::const_apply");
}

shared_ptr<entity> generic_fn_pattern::build(unit& u, functional::match_descriptor& md) const
{
    auto pife = make_shared<internal_function_entity>(
        fn_qname_id(),
        std::move(md.signature),
        make_shared<internal_function_entity::build_data>(std::move(md.bindings), body_));

    pife->set_inline(kind_ == fn_kind::INLINE);
    if (md.result) {
        pife->build_fn_signature(u, md.result);
    }
    
    return pife;

#if 0

    function_entity& fe = const_cast<function_entity&>(dynamic_cast<function_entity const&>(u.eregistry().get(fn_entity_id_)));
    fe.build(u);
    result_type_ = fe.get_result_type();

    qname_view fn_qname = u.qnregistry().resolve(fn_qname_id_);
    fn_compiler_context fnctx{ u, fn_qname / u.new_identifier() };

    // setup parameters & signature
    entity_signature fnsig{ u.get_fn_qname_identifier() };
    boost::container::small_vector<variable_entity*, 16> params;
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
