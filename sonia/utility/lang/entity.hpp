//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

namespace sonia::lang {

template <typename IdentifierT, typename LocationT>
class entity
{
public:
    using identifier_type = IdentifierT;

    inline explicit entity(IdentifierT id = {}) noexcept
        : id_{ std::move(id) }
    {}

    virtual ~entity() = default;

    inline LocationT const& location() const noexcept { return location_; }
    inline void set_location(LocationT l) noexcept { location_ = std::move(l); }

    inline IdentifierT id() const noexcept { return id_; }
    inline void set_id(IdentifierT idval) noexcept { id_ = idval; }

protected:
    IdentifierT id_;
    LocationT location_;
};

}
