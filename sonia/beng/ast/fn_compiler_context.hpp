//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <boost/unordered_map.hpp>

#include "sonia/exceptions.hpp"

#include "../unit.hpp"
#include "../semantic.hpp"

namespace sonia::lang::beng {

/*
class expression_scope
{
public:
    explicit expression_scope(unit& u) : u_ {u} {}

    uint32_t new_variable(identifier name, beng_generic_type t, bool is_const)
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
    explicit fn_compiler_context(unit& u, qname ns)
        : unit_{ u }
        , parent_ { nullptr }
        , ns_{ std::move(ns) }
    {
        init();
    }

    fn_compiler_context(fn_compiler_context& parent, qname_view nested)
        : unit_{ parent.unit_ }
        , ns_{ parent.ns() + nested }
        , parent_ { nested.has_prefix(parent.ns()) ? &parent : nullptr }
    {
        init();
    }

    fn_compiler_context(fn_compiler_context const&) = delete;
    fn_compiler_context& operator=(fn_compiler_context const&) = delete;

    qname_view ns() const { return ns_; }
    qname_view base_ns() const { return span{ns_.parts().data(), base_ns_size_}; }

    void pushed_unnamed_ns()
    {
        ns_ = ns_ + unit_.new_identifier();
    }

    void pop_ns()
    {
        assert(ns_.parts().size() > base_ns_size_);
        ns_.truncate(ns_.parts().size() - 1);
    }

    unit& u() const { return unit_; }

    [[noreturn]]
    inline void throw_identifier_redefinition(entity const& e, qname_view qn, lex::resource_location const& loc) const
    {
        throw exception("%1%(%2%,%3%): `%4%`: identifier redefinition, see declaration at %5%(%6%,%7%)"_fmt %
            loc.resource % loc.line % loc.column %
            u().print(qn) %
            e.location().resource % e.location().line % e.location().column);
    }

    [[noreturn]]
    inline void throw_undeclared_identifier(qname_view qn, lex::resource_location const& loc) const
    {
        throw exception("%1%(%2%,%3%): `%4%`: undeclared identifier"_fmt %
            loc.resource % loc.line % loc.column %
            u().print(qn));
    }
    // to do: resolving depends on qname
    shared_ptr<entity> resolve_entity(qname_view name) const
    {
        if (name.is_absolute()) {
            return unit_.eregistry().find(name);
        }
        qname checkns = ns_;
        
        for (size_t sz = checkns.parts().size();;) {
            checkns.append(name);
            shared_ptr<entity> e = unit_.eregistry().find(checkns);
            if (e || !sz) return e;
            --sz;
            checkns.truncate(sz);
        }
    }

    variable_entity const* resolve_variable(qname_view name) const
    {
        if (name.is_absolute()) {
            shared_ptr<entity> e = unit_.eregistry().find(name);
        }
        shared_ptr<entity> e = resolve_entity(name);
        if (auto varptr = dynamic_pointer_cast<variable_entity>(e); varptr) {
            return varptr.get();
        }
        return nullptr;
    }

    //semantic_expression_type build_expression(beng_generic_type const& result_type, expression_t const& e);

    size_t local_variable_count() const { return local_variable_count_; }
    //span<std::pair<variable_entity*, variable_entity*>> captured_variables() { return captured_variables_; }
    size_t allocate_local_variable_index() { return local_variable_count_++; }
    //size_t allocate_captured_variable_index() { return captured_variable_count_++; }
    
    variable_entity& new_position_parameter(size_t paramnum, beng_type t)
    {
        using buff_t = boost::container::small_vector<char, 16>;
        buff_t tailored_param_name = { '$' };
        bool reversed;
        mp::to_string_converter(std::span{ &paramnum, 1 }, std::back_inserter(tailored_param_name), reversed);
        if (reversed) std::reverse(tailored_param_name.begin() + 1, tailored_param_name.end());
        identifier argid = unit_.slregistry().resolve(string_view{ tailored_param_name.data(), tailored_param_name.size() });
        return new_variable(argid, std::move(t), variable_entity::kind::SCOPE_LOCAL);
    }

    variable_entity& new_variable(identifier name, beng_type t, variable_entity::kind k)
    {
        qname var_qname = ns() + name;
        auto ve = sonia::make_shared<variable_entity>(std::move(var_qname), std::move(t), k);
        unit_.eregistry().insert(ve);
        return *ve;
    }

    boost::container::small_vector<std::pair<variable_entity*, variable_entity*>, 16> captured_variables;

    variable_entity& new_captured_variable(identifier name, beng_type t, variable_entity& caption)
    {
        qname var_qname = base_ns() + name;
        auto ve = sonia::make_shared<variable_entity>(std::move(var_qname), std::move(t), variable_entity::kind::LOCAL);
        unit_.eregistry().insert(ve);
        captured_variables.emplace_back(&caption, ve.get());
        return *ve;
    }

    variable_entity& create_captured_variable_chain(variable_entity& v)
    {
        if (!parent_) {
            THROW_INTERNAL_ERROR("can't find fn context for variable: '%1%', current context ns: '%2%'"_fmt %
                u().print(v.name()) % u().print(ns())
            );
        }
        qname_view vardefscope = v.name().parent();
        if (vardefscope.has_prefix(parent_->base_ns())) {
            return new_captured_variable(v.name().back(), v.type(), v);
        } else {
            variable_entity& parentvar = parent_->create_captured_variable_chain(v);
            return new_captured_variable(v.name().back(), v.type(), parentvar);
        }
    }

    /*
    variable_entity const& new_variable(identifier name, beng_generic_type t, bool is_const)
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
            expressions.front() = semantic::push_value{ null_t{} };
        } else if (local_variable_count_) {
            std::vector<semantic_expression_type> prolog;
            prolog.resize(local_variable_count_, semantic::push_value{ null_t{} });
            expressions.front() = std::move(prolog);
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

    //void push_expression(semantic_expression_type e)
    //{
    //    expressions_.emplace_back(std::move(e));
    //}

    //inline span<const semantic_expression_type> expressions() const noexcept { return expressions_; }

    
    std::vector<semantic_expression_type> expressions;
    optional<beng_type> result;

private:
    void init()
    {
        assert(ns_.is_absolute());
        base_ns_size_ = ns_.parts().size();
        expressions.emplace_back(empty_t{}); // reserve for frame initilalization expressions
    }

private:
    
    //boost::container::small_vector<qname, 8> ns_stack_;
    //std::vector<shared_ptr<variable_entity>> variables_;
    //boost::container::small_vector<expression_scope, 8> scope_stack_;
    
};

}

#include "expression_visitor.hpp"

namespace sonia::lang::beng {

//semantic_expression_type fn_compiler_context::build_expression(beng_generic_type const& result_type, expression_t const& e)
//{
//    expression_visitor evis{ *this, &result_type };
//    semantic_expression_pair pair = apply_visitor(evis, e);
//    return std::get<0>(pair);
//}

}