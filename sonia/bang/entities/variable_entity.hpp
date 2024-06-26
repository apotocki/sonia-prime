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
public:
    enum class kind
    {
        EXTERN, STATIC, LOCAL, SCOPE_LOCAL
    };

    explicit variable_entity(qname_identifier name, bang_type t, kind k)
        : entity{ std::move(name) }
        , type_{ std::move(t) }
        , kind_{ k }
    {}

    inline bang_type const& type() const noexcept { return type_; }
    inline kind const& varkind() const noexcept { return kind_; }
    inline bool is_weak() const { return is_weak_; }

    inline intptr_t index() const noexcept { return index_; }
    void set_index(intptr_t val) { index_ = val; }

    inline void set_weak(bool val = true) { is_weak_ = val; }

private:
    bang_type type_;
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
