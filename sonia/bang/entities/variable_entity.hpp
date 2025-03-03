//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/semantic_fwd.hpp"

namespace sonia::lang::bang {

class extern_variable_entity : public entity
{
public:
    qname_identifier name;

    inline extern_variable_entity(entity_identifier type, qname_identifier nameval) noexcept
        : entity{ }
        , name{ std::move(nameval) }
    {
        set_type(type);
    }

    inline size_t hash() const noexcept override { return hash_value(name); }
    inline bool equal(entity const& rhs) const noexcept
    {
        if (extern_variable_entity const* verhs = dynamic_cast<extern_variable_entity const*>(&rhs); verhs) {
            return verhs->name == name;
        }
        return false;
    }

    void visit(entity_visitor const& v) const override { v(*this); }

    std::ostream& print_to(std::ostream& os, unit const& u) const override;
};

/*
class variable_entity : public entity
{
public:
    enum class kind
    {
        STATIC, LOCAL, SCOPE_LOCAL
    };

    inline variable_entity(entity_identifier type, qname_identifier nameval, kind k) noexcept
        : entity{ }
        , name{ std::move(nameval) }
        , kind_{ k }
    {
        set_type(type);
    }

    qname_identifier name;

    inline kind const& varkind() const noexcept { return kind_; }
    inline bool is_weak() const noexcept { return is_weak_; }

    inline intptr_t index() const noexcept { return index_; }
    inline void set_index(intptr_t val) noexcept { index_ = val; }

    inline void set_weak(bool val = true) noexcept { is_weak_ = val; }

    inline size_t hash() const noexcept override { return hash_value(name); }
    inline bool equal(entity const& rhs) const noexcept
    { 
        if (variable_entity const* verhs = dynamic_cast<variable_entity const*>(&rhs); verhs) {
            return verhs->name == name;
        }
        return false;
    }

    void visit(entity_visitor const& v) const override { v(*this); }

    std::ostream& print_to(std::ostream& os, unit const& u) const override;

private:
    kind kind_;
    intptr_t index_;
    bool is_weak_ = false;
};
*/
/*
class local_variable_entity : public variable_entity
{
public:
    explicit local_variable_entity(qname_type name, bang_type t, bool is_const)
        : variable_entity{ std::move(name), std::move(t) }
        , is_const_ { is_const }
    {}

    inline intptr_t index() const noexcept { return index_; }

    void set_index(intptr_t idx)
    {
        index_ = static_cast<int32_t>(idx);
    }
   
private:
    int32_t index_ : 31;
    int32_t is_const_ : 1;
};
*/

}
