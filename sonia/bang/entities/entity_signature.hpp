//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/container/small_vector.hpp>

#include "sonia/span.hpp"
#include "sonia/utility/functional/range_equal.hpp"

#include "sonia/bang/terms.hpp"

namespace sonia::lang::bang {

//entity signature:  qname, {identifier, entity_identifier}*, {entity_identifier}*

class entity_signature
{
    qname_identifier name_;
    boost::container::small_vector<identifier, 2> params_;
    boost::container::small_vector<entity_identifier, 2> entities_;

public:
    entity_signature() = default;

    explicit entity_signature(qname_identifier qid) : name_{ qid } {}

    inline qname_identifier name() const { return name_; }

    entity_signature& push(identifier p, entity_identifier e)
    {
        entities_.insert(entities_.begin() + params_.size(), e);
        params_.push_back(p);
        return *this;
    }

    entity_signature& push(entity_identifier e)
    {
        entities_.push_back(e);
        return *this;
    }

    friend inline bool operator== (entity_signature const& l, entity_signature const& r) noexcept
    {
        return l.name_ == r.name_ && range_equal()(l.params_, r.params_) && range_equal()(l.entities_, r.entities_);
    }

    friend inline size_t hash_value(entity_signature const& v)
    {
        return hasher{}(v.name_, span{ v.params_ }, span{ v.entities_ });
    }
};

}
