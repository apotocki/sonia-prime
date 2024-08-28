//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/semantic_fwd.hpp"

namespace sonia::lang::bang {

class pack_entity : public type_entity
{
    entity_identifier pack_element_type_;

public:
    explicit pack_entity(qname_identifier eqnm, entity_identifier elem, entity_identifier type)
        : type_entity{ type, std::move(entity_signature{ eqnm }.push(elem)) }, pack_element_type_{ elem }
    {}

    entity_identifier element_type() const { return pack_element_type_; }

    void visit(entity_visitor const& v) const override { v(*this); }
};

}
