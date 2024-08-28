//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/container/small_vector.hpp>

#include "sonia/span.hpp"
#include "sonia/utility/functional/range_equal.hpp"
#include "sonia/utility/functional/hash/pair.hpp"

#include "sonia/bang/terms.hpp"

namespace sonia::lang::bang {

//entity signature:  qname, {identifier, entity_identifier}*, {entity_identifier}*

class entity_signature
{
    qname_identifier name_;
    boost::container::small_vector<std::pair<identifier, entity_identifier>, 2> nfields_;
    boost::container::small_vector<entity_identifier, 2> pfields_;

public:
    entity_signature() = default;

    explicit entity_signature(qname_identifier qid) : name_{ qid } {}

    inline qname_identifier name() const { return name_; }
    inline void set_name(qname_identifier val) { name_ = val; }

    entity_signature& push(identifier p, entity_identifier e)
    {
        nfields_.emplace_back(p, e);
        return *this;
    }

    entity_signature& push(entity_identifier e)
    {
        pfields_.push_back(e);
        return *this;
    }

    void normilize()
    {
        std::sort(nfields_.begin(), nfields_.end(), [](auto const& l, auto const& r) {
            return l.first < r.first;
        });
    }

    friend inline bool operator== (entity_signature const& l, entity_signature const& r) noexcept
    {
        return l.name_ == r.name_ && range_equal()(l.nfields_, r.nfields_) && range_equal()(l.pfields_, r.pfields_);
    }

    friend inline size_t hash_value(entity_signature const& v)
    {
        return hasher{}(v.name_, span{ v.nfields_ }, span{ v.pfields_ });
    }
};

}
