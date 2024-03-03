//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <boost/unordered_map.hpp>

#include "unit.hpp"
#include "sonia/exceptions.hpp"
#include "semantic.hpp"

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

class compiler_context
{
public:
    explicit compiler_context(unit& u)
        : unit_{ u }
    {
        ns_stack_.emplace_back();
        //scope_stack_.emplace_back(unit_);
    }

    unit& u() const { return unit_; }
    qname const& ns() const { return ns_stack_.back(); }

    void set_ns(qname qn)
    {
        ns_stack_.clear();
        ns_stack_.emplace_back(std::move(qn));
    }

    void push_ns(qname qn)
    {
        ns_stack_.emplace_back(std::move(qn));
    }

    void pop_ns()
    {
        ns_stack_.pop_back();
    }

    shared_ptr<entity> resolve_entity(qname_view name) const
    {
        if (name.is_absolute()) {
            return unit_.eregistry().find(name);
        }
        qname checkns = ns();
        
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
        shared_ptr<entity> e = resolve_entity(name);
        if (auto varptr = dynamic_pointer_cast<variable_entity>(e); varptr) {
            return varptr.get();
        }
        return nullptr;
    }

    //semantic_expression_type build_expression(beng_generic_type const& result_type, expression_t const& e);

    variable_entity const& new_variable(identifier name, beng_generic_type t, bool is_const)
    {
        qname var_qname = ns() + name;
        auto ve = sonia::make_shared<local_variable_entity>(std::move(var_qname), std::move(t), is_const);
        unit_.eregistry().insert(ve);
        return unit_.new_variable(ve);
    }

    //void push_expression(semantic_expression_type e)
    //{
    //    expressions_.emplace_back(std::move(e));
    //}

    //inline span<const semantic_expression_type> expressions() const noexcept { return expressions_; }

    std::vector<semantic_expression_type> expressions;

private:
    unit& unit_;
    boost::container::small_vector<qname, 8> ns_stack_;
    //boost::container::small_vector<expression_scope, 8> scope_stack_;
    
};

}

#include "expression_visitor.hpp"

namespace sonia::lang::beng {

//semantic_expression_type compiler_context::build_expression(beng_generic_type const& result_type, expression_t const& e)
//{
//    expression_visitor evis{ *this, &result_type };
//    semantic_expression_pair pair = apply_visitor(evis, e);
//    return std::get<0>(pair);
//}

}