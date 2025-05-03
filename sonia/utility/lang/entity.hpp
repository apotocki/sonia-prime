//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#ifdef SONIA_LANG_DEBUG
#   include <string>
#endif

namespace sonia::lang {

template <typename IdentifierT, typename LocationT>
class entity
{
public:
#ifdef SONIA_LANG_DEBUG
    std::string debug_name;
#endif
    using identifier_type = IdentifierT;
    using location_type = LocationT;

    inline explicit entity(IdentifierT id = {}) noexcept
        : id{ std::move(id) }
    {}

    virtual ~entity() = default;

    identifier_type id;
    LocationT location;
};

}
