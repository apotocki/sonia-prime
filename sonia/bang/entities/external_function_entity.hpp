//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/semantic_fwd.hpp"

namespace sonia::lang::bang {

class external_function_entity : public entity
{
    qname_identifier name_;

public:
    explicit external_function_entity(entity_identifier eid, qname_identifier name, size_t fni = -1)
        : entity{ std::move(eid) }
        , name_{ std::move(name) }
        , fn_index{ fni }
    {}

    size_t fn_index;
};

}