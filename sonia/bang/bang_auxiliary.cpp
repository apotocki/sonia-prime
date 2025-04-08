//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "auxiliary.hpp"

namespace sonia::lang::bang {

entity const& get_entity(unit& u, entity_identifier const& eid)
{
    return u.eregistry_get(eid);
}

}
