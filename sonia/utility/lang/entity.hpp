//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "qname.hpp"

namespace sonia::lang {

template <typename QnameIdentifierT, typename LocationT>
class entity
{
public:
    using identifier_type = QnameIdentifierT;

    explicit entity(QnameIdentifierT name) : name_{ std::move(name) } {}

    virtual ~entity() = default;

    inline void set_location(LocationT l) { location_ = std::move(l); }

    QnameIdentifierT name() const { return name_; }
    LocationT const& location() const { return location_; }

protected:
    QnameIdentifierT name_;
    LocationT location_;
};

}
