//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <ranges>
#include <boost/container/small_vector.hpp>

#include "sonia/span.hpp"
#include "sonia/utility/functional/range_equal.hpp"
#include "sonia/utility/functional/hash/pair.hpp"

#include "sonia/bang/terms.hpp"

namespace sonia::lang::bang {

//entity signature:  qname, {identifier, [const]entity_identifier}*, {[const]entity_identifier}*

struct field_descriptor
{
    entity_identifier entity_id_;
    bool is_const_;

    inline entity_identifier entity_id() const noexcept { return entity_id_; }
    inline bool is_const() const noexcept { return is_const_; }

    friend inline bool operator== (field_descriptor const& l, field_descriptor const& r) noexcept
    {
        return l.entity_id_ == r.entity_id_ && l.is_const_ == r.is_const_;
    }

    friend inline size_t hash_value(field_descriptor const& v)
    {
        return hasher{}(v.entity_id_, v.is_const_);
    }
};

class entity_signature
{
    qname_identifier name_;
    boost::container::small_vector<identifier, 4> names_;
    boost::container::small_vector<field_descriptor, 4> fields_;
    field_descriptor result_{{}, false};

public:
    entity_signature() = default;

    explicit entity_signature(qname_identifier qid) : name_{ qid } {}

    inline qname_identifier name() const { return name_; }
    inline void set_name(qname_identifier val) { name_ = val; }

    inline span<const field_descriptor> fields() const noexcept { return fields_; }
    inline span<const identifier> names() const noexcept { return names_; }
    inline const field_descriptor& result() const noexcept { return result_; }
    inline span<const field_descriptor> positioned_fields() const noexcept { return fields().subspan(names_.size()); }
    inline span<const field_descriptor> named_fields() const noexcept { return fields().subspan(0, names_.size()); }

    field_descriptor const* find_field(identifier field_name) const noexcept
    {
        auto it = std::lower_bound(names_.begin(), names_.end(), field_name);
        if (it != names_.end() && *it == field_name) return &fields_[it - names_.begin()];
        return nullptr;
    }

    entity_signature& push(identifier p, field_descriptor fd)
    {
        fields_.insert(fields_.begin() + names_.size(), std::move(fd));
        names_.emplace_back(p);
        return *this;
    }

    inline entity_signature& push(field_descriptor fd)
    {
        fields_.push_back(std::move(fd));
        return *this;
    }

    void set(size_t pos, identifier nm, field_descriptor fd)
    {
        if (fields_.size() <= pos) {
            fields_.resize(pos + 1);
            names_.resize(pos + 1);
        }
        fields_[pos] = std::move(fd);
        names_[pos] = nm;
    }

    void set(size_t pos, field_descriptor fd)
    {
        if (fields_.size() <= pos) {
            fields_.resize(pos + 1);
        }
        fields_[pos] = std::move(fd);
    }

    inline void set_result(field_descriptor fd) { result_ = std::move(fd); }

    void normilize()
    {
        auto names = span{ names_ };
        auto nfields = span{ fields_ }.subspan(0, names.size());
        std::ranges::stable_sort(std::ranges::zip_view(names, nfields), {}, [](auto const& v) { return get<0>(v); });
    }

    inline void reset_fields() noexcept { fields_.clear(); names_.clear(); }

    friend inline bool operator== (entity_signature const& l, entity_signature const& r) noexcept
    {
        return l.name_ == r.name_ && range_equal()(l.fields_, r.fields_) && range_equal()(l.names_, r.names_) && l.result_ == r.result_;
    }

    friend inline size_t hash_value(entity_signature const& v)
    {
        return hasher{}(v.name_, span{ v.names_ }, span{ v.fields_ }, v.result_);
    }
};

#if 0
class entity_signature
{
    qname_identifier name_;
    boost::container::small_vector<std::pair<identifier, field_descriptor>, 2> nfields_;
    boost::container::small_vector<field_descriptor, 2> pfields_;

public:
    entity_signature() = default;

    explicit entity_signature(qname_identifier qid) : name_{ qid } {}

    inline qname_identifier name() const { return name_; }
    inline void set_name(qname_identifier val) { name_ = val; }

    inline span<const std::pair<identifier, field_descriptor>> named_fields() const noexcept { return nfields_; }
    inline span<const field_descriptor> positioned_fields() const noexcept { return pfields_; }

    inline void reset_fields() noexcept { nfields_.clear(); pfields_.clear(); }
    
    entity_signature& push(identifier p, field_descriptor fd)
    {
        nfields_.emplace_back(p, fd);
        return *this;
    }

    entity_signature& push(field_descriptor fd)
    {
        pfields_.push_back(fd);
        return *this;
    }

    void set(size_t pos, identifier p, field_descriptor fd)
    {
        if (nfields_.size() <= pos) nfields_.resize(pos + 1);
        nfields_[pos] = std::pair{ p, fd };
    }

    void set(size_t pos, field_descriptor fd)
    {
        if (pfields_.size() <= pos) pfields_.resize(pos + 1);
        pfields_[pos] = fd;
    }

    void normilize()
    {
        std::stable_sort(nfields_.begin(), nfields_.end(), [](auto const& l, auto const& r) {
            return l.first < r.first;
        });
    }

    field_descriptor const* find_field(identifier field_name) const
    {
        auto it = std::lower_bound(nfields_.begin(), nfields_.end(), field_name,
            [](std::pair<identifier, field_descriptor> const& l, identifier r) { return l.first < r; });
        if (it != nfields_.end() && it->first == field_name) return &it->second;
        return nullptr;
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

#endif
}
