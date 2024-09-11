//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <boost/unordered_map.hpp>

#include "sonia/exceptions.hpp"

#include "../unit.hpp"
#include "../semantic.hpp"

#include "sonia/utility/invocation/invocation.hpp"

#include "sonia/bang/entities/const_entity.hpp"

namespace sonia::lang::bang {

/*
class expression_scope
{
public:
    explicit expression_scope(unit& u) : u_ {u} {}

    uint32_t new_variable(identifier name, bang_generic_type t, bool is_const)
    {
        auto it = variables.find(name);
        if (it != variables.end()) {
            throw exception("the variable '%1%' is already defined in the scope"_fmt % u_.print(name));
        }
        variable_descriptor& var = u_.new_variable();
        var.name = name;
        var.is_const = is_const ? 1 : 0;
        variables.insert(it, std::pair{ name, var.index });
        return var.index;
    }

    uint32_t const* resolve_variable(identifier name) const
    {
        auto it = variables.find(name);
        return it != variables.end() ? &it->second : nullptr;
    }

private:
    unit& u_;
    boost::unordered_map<identifier, uint32_t> variables;
};
*/

class fn_compiler_context
{
    unit& unit_;
    fn_compiler_context* parent_;
    qname ns_;
    size_t base_ns_size_;
    size_t local_variable_count_ = 0;
    
public:
    using expr_vec_t = std::vector<semantic::expression_t>;

    fn_compiler_context(unit& u, qname ns)
        : unit_{ u }
        , parent_{ nullptr }
        , ns_{ std::move(ns) }
    {
        init();
    }

    fn_compiler_context(fn_compiler_context& parent, qname_view nested)
        : unit_{ parent.unit_ }
        , ns_{ parent.ns() / nested }
        , parent_{ nested.has_prefix(parent.ns()) ? &parent : nullptr }
    {
        init();
    }

    fn_compiler_context(fn_compiler_context const&) = delete;
    fn_compiler_context& operator=(fn_compiler_context const&) = delete;

    qname_view ns() const { return ns_; }
    qname_view base_ns() const { return span{ns_.parts().data(), base_ns_size_}; }

    void pushed_unnamed_ns()
    {
        ns_ = ns_ / unit_.new_identifier();
    }

    void pop_ns()
    {
        assert(ns_.parts().size() > base_ns_size_);
        ns_.truncate(ns_.parts().size() - 1);
    }

    unit& u() const { return unit_; }

    //std::expected<int, error_storage> build_fieldset(fn_pure_t const&, patern_fieldset_t&);
    error_storage build_function_descriptor(fn_pure_t const& pure_decl, function_descriptor& fds);

    functional const* lookup_functional(qname_view name) const;

    // to do: resolving depends on qname
    shared_ptr<entity> resolve_entity(qname_identifier name) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("resolve_entity");
#if 0
        if (name.is_absolute()) {
            return unit_.eregistry().find(name);
        }
        qname checkns = ns_;
        qname_view name_qn = unit_.qnregistry().resolve(name);
        for (size_t sz = checkns.parts().size();;) {
            checkns.append(name_qn);
            shared_ptr<entity> e = unit_.eregistry().find(unit_.qnregistry().resolve(checkns));
            if (e || !sz) return e;
            --sz;
            checkns.truncate(sz);
        }
#endif
    }

    variable_entity const* resolve_variable(qname_identifier name) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("resolve_variable");
#if 0
        if (name.is_absolute()) {
            shared_ptr<entity> e = unit_.eregistry().find(name);
        }
        shared_ptr<entity> e = resolve_entity(name);
        if (auto varptr = dynamic_pointer_cast<variable_entity>(e); varptr) {
            return varptr.get();
        }
        return nullptr;
#endif
    }

    //semantic::expression_type build_expression(bang_generic_type const& result_type, expression_t const& e);

    size_t local_variable_count() const { return local_variable_count_; }
    //span<std::pair<variable_entity*, variable_entity*>> captured_variables() { return captured_variables_; }
    size_t allocate_local_variable_index() { return local_variable_count_++; }
    //size_t allocate_captured_variable_index() { return captured_variable_count_++; }
    
    //void new_const_entity(string_view name, shared_ptr<const_entity> ent)
    //{
    //    identifier nid = unit_.slregistry().resolve(name);
    //    qname var_qname = ns() / nid;
    //    qname_identifier qname_id = unit_.qnregistry().resolve(var_qname);
    //    functional& varfn = unit_.fregistry().resolve(qname_id);
    //    varfn.set_default_entity(ent->id());
    //}

    //variable_entity& new_position_parameter(size_t paramnum, entity_identifier t)
    //{
    //    using buff_t = boost::container::small_vector<char, 16>;
    //    buff_t tailored_param_name = { '$' };
    //    bool reversed;
    //    mp::to_string(std::span{ &paramnum, 1 }, std::back_inserter(tailored_param_name), reversed);
    //    if (reversed) std::reverse(tailored_param_name.begin() + 1, tailored_param_name.end());
    //    identifier argid = unit_.slregistry().resolve(string_view{ tailored_param_name.data(), tailored_param_name.size() });
    //    return new_variable(argid, std::move(t), variable_entity::kind::SCOPE_LOCAL);
    //}

    variable_entity& new_variable(annotated_identifier name, entity_identifier t, variable_entity::kind k);

    boost::container::small_vector<std::pair<variable_entity*, variable_entity*>, 16> captured_variables;

    variable_entity& new_captured_variable(identifier name, entity_identifier t, variable_entity& caption)
    {
        THROW_NOT_IMPLEMENTED_ERROR("fn_compiler_context new_captured_variable");
#if 0
        qname var_qname = base_ns() / name;
        auto ve = sonia::make_shared<variable_entity>(u().qnregistry().resolve(var_qname), std::move(t), variable_entity::kind::LOCAL);
        ve->set_weak(caption.is_weak());
        unit_.eregistry().insert(ve);
        captured_variables.emplace_back(&caption, ve.get());
        return *ve;
#endif
    }

    variable_entity& create_captured_variable_chain(variable_entity& v)
    {
        if (!parent_) {
            THROW_INTERNAL_ERROR("can't find fn context for variable: '%1%', current context ns: '%2%'"_fmt %
                u().print(v.name()) % u().print(ns())
            );
        }
        qname_view name_qv = u().fregistry().resolve(v.name()).name();
        qname_view vardefscope = name_qv.parent();
        if (vardefscope.has_prefix(parent_->base_ns())) {
            return new_captured_variable(name_qv.back(), v.get_type(), v);
        } else {
            variable_entity& parentvar = parent_->create_captured_variable_chain(v);
            return new_captured_variable(name_qv.back(), v.get_type(), parentvar);
        }
    }

    /*
    variable_entity const& new_variable(identifier name, bang_generic_type t, bool is_const)
    {
        qname var_qname = ns + name;
        auto ve = sonia::make_shared<local_variable_entity>(std::move(var_qname), std::move(t), is_const);
        unit_.eregistry().insert(ve);
        ve->set_index(variables_.size());
        variables_.emplace_back(std::move(ve));
        return *variables_.back();
    }
    */

    void finish_frame()
    {
        if (local_variable_count_ == 1) {
            expressions_.front() = semantic::push_value{ null_t{} };
        } else if (local_variable_count_) {
            std::vector<semantic::expression_t> prolog;
            prolog.resize(local_variable_count_, semantic::push_value{ null_t{} });
            expressions_.front() = std::move(prolog);
        }
    }

    //local_variable_entity const& get_variable(size_t index) const
    //{
    //    return *variables_[index];
    //}

    //local_variable_entity const& new_variable(shared_ptr<local_variable_entity> ve)
    //{
    //    ve->set_index(variables_.size());
    //    variables_.emplace_back(std::move(ve));
    //    return *variables_.back();
    //}

    //void push_expression(semantic::expression_type e)
    //{
    //    expressions_.emplace_back(std::move(e));
    //}

    //inline span<const semantic::expression_type> expressions() const noexcept { return expressions_; }

    expr_vec_t& expressions() { return *expr_stack_.back(); }
    expr_vec_t& expressions(size_t branch_offset) { return *expr_stack_[expr_stack_.size() - branch_offset - 1]; }
    size_t expressions_branch() const { return expr_stack_.size(); }
    void append_expression(semantic::expression_t && e)
    {
        expressions().emplace_back(std::move(e));
    }

    std::pair<size_t, size_t> current_expressions_pointer() const
    {
        return { expr_stack_.size() - 1, expr_stack_.back()->size() - 1 };
    }

    void set_expression(std::pair<size_t, size_t> ep, semantic::expression_t&& e)
    {
        (*expr_stack_[ep.first])[ep.second] = std::move(e);
    }

    void push_chain(expr_vec_t& chain_vec)
    {
        expr_stack_.emplace_back(&chain_vec);
    }

    void pop_chain()
    {
        expr_stack_.pop_back();
    }

    void collapse_chains(size_t branch = 1)
    {
        expr_stack_.resize(branch);
    }
    //semantic::conditional<semantic::expression_type> * current_chain()
    //{
    //    if (expr_stack_.size() <= 1) return nullptr;
    //    auto& maybecond = expressions(1).back();
    //    return get<semantic::conditional<semantic::expression_type>>(&maybecond);
    //}

    friend class expressions_stack_state;

    class expressions_state_type
    {
        fn_compiler_context * pctx_;
        size_t cursize_;
        size_t stack_size_;
        entity_identifier cur_type_;

    public:
        expressions_state_type(fn_compiler_context& ctx)
            : pctx_{&ctx}, cursize_{ ctx.expressions().size() }, stack_size_{ ctx.expr_stack_.size() }, cur_type_{ ctx.context_type }
        {}
        expressions_state_type(expressions_state_type const&) = delete;
        expressions_state_type(expressions_state_type && rhs)
            : pctx_{ rhs.pctx_ }, cursize_{ rhs.cursize_ }, stack_size_{ rhs.stack_size_ }, cur_type_{ rhs.cur_type_ }
        {
            rhs.pctx_ = nullptr;
        }
        expressions_state_type& operator=(expressions_state_type const&) = delete;
        expressions_state_type& operator=(expressions_state_type &&) = delete;
        ~expressions_state_type()
        {
            if (pctx_) {
                restore();
            }
        }

        void restore()
        {
            pctx_->expr_stack_.resize(stack_size_);
            pctx_->expressions().resize(cursize_);
            pctx_->context_type = cur_type_;
        }

        void detach()
        {
            pctx_ = nullptr;
        }

        void restore_and_detach()
        {
            restore();
            detach();
        }
    };

    expressions_state_type expressions_state() { return expressions_state_type{*this}; }

    entity_identifier result;
    entity_identifier accum_result;
    entity_identifier context_type;

    void accumulate_result_type(entity_identifier t)
    {
        if (!accum_result) {
            accum_result = std::move(t);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("compiler context: accumulate_result_type");
#if 0
            accum_result = make_union_type(*accum_result, &t);
#endif
        }
    }

    entity_identifier compute_result_type()
    {
        if (result) { return result; }
        else if (!accum_result) {
            accum_result = unit_.get_void_entity_identifier();
        }
        return accum_result;
    }

private:
    void init()
    {
        assert(ns_.is_absolute());
        base_ns_size_ = ns_.parts().size();
        expressions_.emplace_back(empty_t{}); // reserve for frame initilalization expressions
        expr_stack_.emplace_back(&expressions_);
    }

private:
    expr_vec_t expressions_;
    std::vector<expr_vec_t*> expr_stack_;

    //boost::container::small_vector<qname, 8> ns_stack_;
    //std::vector<shared_ptr<variable_entity>> variables_;
    //boost::container::small_vector<expression_scope, 8> scope_stack_;
    
};

}

#include "expression_visitor.hpp"

namespace sonia::lang::bang {

//semantic::expression_type fn_compiler_context::build_expression(bang_generic_type const& result_type, expression_t const& e)
//{
//    expression_visitor evis{ *this, &result_type };
//    semantic_expression_pair pair = apply_visitor(evis, e);
//    return std::get<0>(pair);
//}

}