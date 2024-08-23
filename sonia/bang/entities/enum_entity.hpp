//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "../semantic_fwd.hpp"

namespace sonia::lang::bang {

class enum_entity : public entity
{
    qname_identifier name_;

public:
    explicit enum_entity(entity_identifier eid, qname_identifier name) 
        : entity { std::move(eid) }, name_{ name }
    {}

    struct enum_case
    {
        identifier name;
        value_t value;

        template <typename VArgT>
        enum_case(identifier n, VArgT && v) : name{ std::move(n) }, value{ std::forward<VArgT>(v) } {}

        friend inline bool operator==(enum_case const& l, enum_case const& r) { return l.name == r.name; }
        friend inline auto operator<=>(enum_case const& l, enum_case const& r) { return l.name <=> r.name; }
    };

    std::vector<enum_case> cases;

    enum_case const* find(identifier);
};

}
