//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <boost/unordered_set.hpp>

#include "sonia/shared_ptr.hpp"

#include "semantic.hpp"

#include <ranges>

namespace sonia::lang::beng {

class variable_entity : public entity
{
public:
    explicit variable_entity(qname_type name, beng_generic_type t)
        : entity{ std::move(name) }
        , type_{ std::move(t) }
    {}

    inline beng_generic_type const& type() const noexcept { return type_; }

private:
    beng_generic_type type_;
};

class local_variable_entity : public variable_entity
{
public:
    explicit local_variable_entity(qname_type name, beng_generic_type t, bool is_const)
        : variable_entity{ std::move(name), std::move(t) }
        , is_const_ { is_const }
    {}

    inline uint32_t index() const noexcept { return index_; }

    void set_index(size_t idx)
    {
        index_ = static_cast<uint32_t>(idx);
    }
   
private:
    uint32_t index_ : 31;
    uint32_t is_const_ : 1;
};

}
