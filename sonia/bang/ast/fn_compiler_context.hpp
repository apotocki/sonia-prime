//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
//#include <boost/unordered_map.hpp>

//#include <boost/container/flat_set.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/small_vector.hpp"

#include "sonia/bang/errors.hpp"
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/unit.hpp"

#include "sonia/utility/invocation/invocation.hpp"

#include "sonia/bang/entities/variable_entity.hpp"
#include "sonia/bang/entities/functional.hpp"

#include "sonia/utility/lang/compiler.hpp"
#include "sonia/bang/utility/linked_list.ipp"

namespace sonia::lang::bang {

class entity_task_id : public compiler_task_id
{
    entity const& entity_;

public:
    inline explicit entity_task_id(entity const& e) : entity_{e} {}

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(entity_task_id);

    bool equal(compiler_task_id const& tid) const noexcept override
    {
        if (entity_task_id const* et = dynamic_cast<entity_task_id const*>(&tid); et) {
            return entity_.equal(et->entity_);
        }
        return false;
    }

    size_t hash() const noexcept override { return entity_.hash(); }
};

class qname_task_id : public compiler_task_id
{
    qname_identifier qnmid_;

public:
    inline explicit qname_task_id(qname_identifier qnmid) : qnmid_{ qnmid } {}

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(qname_task_id);

    bool equal(compiler_task_id const& tid) const noexcept override
    {
        if (qname_task_id const* et = dynamic_cast<qname_task_id const*>(&tid); et) {
            return qnmid_ == et->qnmid_;
        }
        return false;
    }

    size_t hash() const noexcept override { return hash_value(qnmid_); }
};

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
    compiler_worker_id worker_id_;
    small_vector<functional_binding const*, 4> bindings_;
    functional_binding_set locals_;

public:
    fn_compiler_context(unit& u, qname ns = {});

    fn_compiler_context(fn_compiler_context& parent, qname_view nested);

    fn_compiler_context(fn_compiler_context const&) = delete;
    fn_compiler_context& operator=(fn_compiler_context const&) = delete;

    ~fn_compiler_context();

    qname_view ns() const { return ns_; }
    qname_view base_ns() const { return span{ns_.parts().data(), base_ns_size_}; }

    optional<variant<entity_identifier, local_variable const&>> get_bound(identifier) const noexcept;
    inline void push_binding(functional_binding const& binding)
    {
        bindings_.push_back(std::addressof(binding));
    }

    inline void pop_binding() { bindings_.pop_back(); }

    compiler_task_tracer::task_guard try_lock_task(compiler_task_id const&);

    void pushed_unnamed_ns();

    void pop_ns()
    {
        assert(ns_.parts().size() > base_ns_size_);
        ns_.truncate(ns_.parts().size() - 1);
    }

    inline unit& u() const noexcept { return unit_; }

    //std::expected<int, error_storage> build_fieldset(fn_pure const&, patern_fieldset_t&);
    //error_storage build_function_descriptor(fn_pure const& pure_decl, function_descriptor& fds);

    functional const* lookup_functional(qname_view) const;
    std::expected<qname_identifier, error_storage> lookup_qname(annotated_qname const&) const;
    variant<entity_identifier, local_variable const&> lookup_entity(annotated_qname const&);
    std::expected<functional::match, error_storage> find(builtin_qnid, pure_call_t const&, semantic::expression_list_t&, annotated_entity_identifier const& expected_result = annotated_entity_identifier{});
    std::expected<functional::match, error_storage> find(qname_identifier, pure_call_t const&, semantic::expression_list_t&, annotated_entity_identifier const& expected_result = annotated_entity_identifier{});
    
#if 0
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
#endif
    //semantic::expression_type build_expression(bang_generic_type const& result_type, expression_t const& e);

    size_t local_variable_count() const { return local_variable_count_; }
    //span<std::pair<variable_entity*, variable_entity*>> captured_variables() { return captured_variables_; }
    intptr_t allocate_local_variable_index() { return static_cast<intptr_t>(local_variable_count_++); }
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

    local_variable& new_variable(annotated_identifier, entity_identifier type);
    void new_constant(annotated_identifier, entity_identifier);

#if 0
    small_vector<std::pair<variable_entity*, variable_entity*>, 16> captured_variables;

    variable_entity& new_captured_variable(identifier name, entity_identifier t, variable_entity& caption)
    {
        THROW_NOT_IMPLEMENTED_ERROR("fn_compiler_context new_captured_variable");
#if 0
        qname var_qname = base_ns() / name;
        auto ve = sonia::make_shared<variable_entity>(u().qnregistry().resolve(var_qname), std::move(t), variable_entity::kind::LOCAL);
        ve->set_weak(caption.is_weak());
        unit_.eregistry_insert(ve);
        captured_variables.emplace_back(&caption, ve.get());
        return *ve;
#endif
    }

    variable_entity& create_captured_variable_chain(variable_entity& v);
#endif
    /*
    variable_entity const& new_variable(identifier name, bang_generic_type t, bool is_const)
    {
        qname var_qname = ns + name;
        auto ve = sonia::make_shared<local_variable_entity>(std::move(var_qname), std::move(t), is_const);
        unit_.eregistry_insert(ve);
        ve->set_index(variables_.size());
        variables_.emplace_back(std::move(ve));
        return *variables_.back();
    }
    */

    void finish_frame();

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

    semantic::expression_list_t& expressions() { return *expr_stack_.back(); }
    semantic::expression_list_t& expressions(size_t branch_offset) { return *expr_stack_[expr_stack_.size() - branch_offset - 1]; }
    size_t expressions_branch() const { return expr_stack_.size(); }
    void append_expression(semantic::expression&&);
    
    semantic::expression_span store_semantic_expressions(semantic::managed_expression_list&&);
    semantic::managed_expression_list& expression_store() { return expression_store_; }

    //std::pair<size_t, expression_list_t::iterator> current_expressions_pointer() const
    //{
    //    return { expr_stack_.size() - 1, expr_stack_.back()->size() - 1 };
    //}

    //void set_expression(std::pair<size_t, expression_list_t::iterator> ep, semantic::expression_t&& e)
    //{
    //    (*expr_stack_[ep.first])[ep.second] = std::move(e);
    //}

    void push_chain(semantic::expression_list_t& chain_vec)
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
        expressions_state_type(fn_compiler_context&) noexcept;
            
        expressions_state_type(expressions_state_type const&) = delete;
        expressions_state_type(expressions_state_type&&) noexcept;
        expressions_state_type& operator=(expressions_state_type const&) = delete;
        expressions_state_type& operator=(expressions_state_type &&) = delete;
        ~expressions_state_type();

        void restore();

        void detach() noexcept;
        void restore_and_detach();
    };

    inline expressions_state_type expressions_state() noexcept { return expressions_state_type{*this}; }

    entity_identifier result;
    entity_identifier accum_result;
    entity_identifier context_type;

    void accumulate_result_type(entity_identifier t);
    entity_identifier compute_result_type();

private:
    void init();

private:
    semantic::expression_list_t root_expressions_;
    small_vector<semantic::expression_list_t*, 8> expr_stack_;
    semantic::managed_expression_list expression_store_;
};

}
