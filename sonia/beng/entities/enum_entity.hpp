//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/beng/ast/terms.hpp"

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
