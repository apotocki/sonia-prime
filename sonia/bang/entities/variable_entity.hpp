//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <boost/unordered_set.hpp>

#include "sonia/shared_ptr.hpp"

#include "sonia/bang/semantic.hpp"

#include <ranges>

namespace sonia::lang::bang {

class variable_entity : public entity
{
    qname_identifier name_;

public:
    enum class kind
    {
        EXTERN, STATIC, LOCAL, SCOPE_LOCAL
    };

    explicit variable_entity(entity_identifier type, qname_identifier name, kind k)
        : entity{ std::move(type) }
        , name_{ std::move(name) }
        , kind_{ k }
    {}

    qname_identifier name() const { return name_; }

    inline kind const& varkind() const noexcept { return kind_; }
    inline bool is_weak() const { return is_weak_; }

    inline intptr_t index() const noexcept { return index_; }
    void set_index(intptr_t val) { index_ = val; }

    inline void set_weak(bool val = true) { is_weak_ = val; }

    size_t hash() const noexcept override { return hash_value(name_); }
    bool equal(entity const& rhs) const noexcept
    { 
        if (variable_entity const* verhs = dynamic_cast<variable_entity const*>(&rhs); verhs) {
            return verhs->name_ == name_;
        }
        return false;
    }

    void visit(entity_visitor const& v) const override { v(*this); }

private:
    kind kind_;
    intptr_t index_;
    bool is_weak_ = false;
};

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
