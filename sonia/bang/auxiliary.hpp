//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/terms.hpp"

namespace sonia::lang::bang {

class unit;
class entity;

entity const& get_entity(unit&, entity_identifier const&);

inline entity const& get_entity(unit& u, annotated_entity_identifier const& aeid)
{
    return get_entity(u, aeid.value);
}

}
