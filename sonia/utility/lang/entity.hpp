//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "qname.hpp"

namespace sonia::lang {

template <typename IdentifierT, typename LocationT>
class entity
{
public:
    using identifier_type = IdentifierT;

    explicit entity(IdentifierT id = {}) : id_{ std::move(id) } {}

    virtual ~entity() = default;

    inline void set_location(LocationT l) { location_ = std::move(l); }

    IdentifierT id() const { return id_; }
    LocationT const& location() const { return location_; }

    void set_id(IdentifierT idval) { id_ = idval; }

protected:
    IdentifierT id_;
    LocationT location_;
};

}
