//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "beng.hpp"

namespace sonia::lang::beng {

class enum_entity : public entity
{
public:
    explicit enum_entity(qname_type name) : entity { std::move(name) } {}

    struct enum_case
    {
        identifier name;
        value_t value;
        friend inline bool operator==(enum_case const& l, enum_case const& r) { return l.name == r.name; }
        friend inline auto operator<=>(enum_case const& l, enum_case const& r) { return l.name <=> r.name; }
    };

    std::vector<enum_case> cases;

    enum_case const* find(identifier);
};

}

namespace sonia::lang::beng {

enum_entity::enum_case const* enum_entity::find(identifier id)
{
    auto it = std::ranges::lower_bound(cases, id, {}, [](auto const& e) { return e.name; });
    return it == cases.end() ? nullptr : &*it;
}

}

