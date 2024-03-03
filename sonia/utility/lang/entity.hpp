//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "qname.hpp"

namespace sonia::lang {

template <typename IdentifierT>
class entity
{
public:
    using identifier_type = IdentifierT;
    using qname_type = qname<IdentifierT>;

    explicit entity(qname_type name) : name_{ std::move(name) } {}

    virtual ~entity() = default;

    qname_view<IdentifierT> name() const { return name_; }

private:
    qname_type name_;
};

}
