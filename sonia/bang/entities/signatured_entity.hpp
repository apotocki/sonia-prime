//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <ranges>
#include <boost/container/flat_map.hpp>

#include "sonia/span.hpp"
#include "sonia/optional.hpp"
#include "sonia/small_vector.hpp"
#include "sonia/utility/functional/range_equal.hpp"
#include "sonia/utility/functional/hash/span.hpp"
#include "sonia/bang/terms.hpp"
#include "sonia/bang/semantic_fwd.hpp"

namespace sonia::lang::bang {

//entity signature:  qname, {identifier, [const]entity_identifier}*, {[const]entity_identifier}*

struct field_descriptor
{
    identifier name_;
    entity_identifier entity_id_;
    bool is_const_;

    inline explicit field_descriptor(identifier n, entity_identifier eid = {}, bool is_const_val = false) noexcept
        : name_{ std::move(n) }, entity_id_{ eid }, is_const_{ is_const_val }
    {}

    inline explicit field_descriptor(entity_identifier eid = {}, bool is_const_val = false) noexcept
        : entity_id_{ eid }, is_const_{ is_const_val }
    {}

    inline explicit field_descriptor(identifier n, entity const& e, bool is_const_val = false) noexcept
        : name_{ std::move(n) }, entity_id_{ e.id }, is_const_{ is_const_val }
    {}

    inline explicit field_descriptor(entity const& e, bool is_const_val = false) noexcept
        : entity_id_{ e.id }, is_const_{ is_const_val }
    {}

    inline identifier name() const noexcept { return name_; }
    inline entity_identifier entity_id() const noexcept { return entity_id_; }
    inline bool is_const() const noexcept { return is_const_; }

    inline explicit operator bool() const noexcept { return !!entity_id_; }

    friend inline bool operator== (field_descriptor const& l, field_descriptor const& r) noexcept
    {
        return l.entity_id_ == r.entity_id_ && l.name_ == r.name_ && l.is_const_ == r.is_const_;
    }

    friend inline size_t hash_value(field_descriptor const& v) noexcept
    {
        return hasher{}(v.name_, v.entity_id_, v.is_const_);
    }
};

class entity_signature
{
    boost::container::small_flat_multimap<identifier, uint32_t, 4> named_fields_indices_;
    small_vector<uint32_t, 8> positioned_fields_indices_;
    small_vector<field_descriptor, 8> fields_;

public:
    entity_signature() noexcept = default;

    explicit entity_signature(qname_identifier qid) noexcept : name{ qid } {}
    explicit entity_signature(qname_identifier qid, entity_identifier t) noexcept : name{ qid }, result{ t } {}

    // a functional name that produces the signatured entity
    qname_identifier name;

    // If a function declaration does not have an explicitly declared result type
    // (and should therefore be determined by the function body)
    // the result type doesn't take part in signature identification.
    optional<field_descriptor> result;

    inline bool empty() const noexcept { return fields_.empty(); }
    inline span<const field_descriptor> fields() const noexcept { return fields_; }
    inline field_descriptor const& field(size_t index) const noexcept { return fields_[index]; }
    inline size_t field_count() const noexcept { return fields_.size(); }

    inline span<const std::pair<identifier, uint32_t>> named_fields_indices() const noexcept { return named_fields_indices_; }
    inline span<const uint32_t> positioned_fields_indices() const noexcept { return positioned_fields_indices_; }

    inline auto find_fields(identifier field_name) const noexcept
    {
        return named_fields_indices_.equal_range(field_name);
    }

    inline field_descriptor const* find_field(identifier field_name, size_t * pindex = nullptr) const noexcept
    {
        if (auto it = named_fields_indices_.find(field_name); it != named_fields_indices_.end()) {
            if (pindex) *pindex = it->second; // std::distance(named_fields_indices_.begin(), it);
            return &fields_[it->second];
        }
        return nullptr;
    }

    inline field_descriptor const* find_field(size_t index, size_t* pindex = nullptr) const noexcept
    {
        if (index < positioned_fields_indices_.size()) {
            uint32_t offs = positioned_fields_indices_[index];
            if (pindex) *pindex = offs;
            return &fields_[offs];
        }
        return nullptr;
    }

    field_descriptor const* get_field(size_t index) const noexcept
    {
        if (index < fields_.size()) {
            return &fields_[index];
        }
        return nullptr;
    }

    void push_back(field_descriptor fd)
    {
        if (!fd.name()) {
            positioned_fields_indices_.push_back(static_cast<uint32_t>(fields_.size()));
        } else {
            named_fields_indices_.emplace(fd.name(), static_cast<uint32_t>(fields_.size()));
        }
        fields_.push_back(std::move(fd));
    }

    void emplace_back(identifier n, entity_identifier eid, bool is_const_val)
    {
        if (n) {
            named_fields_indices_.emplace(n, static_cast<uint32_t>(fields_.size()));
        } else {
            positioned_fields_indices_.push_back(static_cast<uint32_t>(fields_.size()));
        }
        fields_.emplace_back(n, std::move(eid), is_const_val);
    }

    inline void emplace_back(identifier n, entity const& e, bool is_const_val)
    {
        emplace_back(std::move(n), e.id, is_const_val);
    }

    inline void emplace_back(identifier field_name, field_descriptor fd)
    {
        emplace_back(std::move(field_name), std::move(fd.entity_id()), fd.is_const());
    }

    void emplace_back(entity_identifier eid, bool is_const_val)
    {
        positioned_fields_indices_.push_back(static_cast<uint32_t>(fields_.size()));
        fields_.emplace_back(identifier{}, std::move(eid), is_const_val);
    }

    inline void emplace_back(entity const& e, bool is_const_val)
    {
        emplace_back(e.id, is_const_val);
    }

    inline void reset_fields() noexcept
    {
        fields_.clear();
        named_fields_indices_.clear();
        positioned_fields_indices_.clear();
    }

    friend inline bool operator== (entity_signature const& l, entity_signature const& r) noexcept
    {
        return l.name == r.name &&
            range_equal()(l.fields_, r.fields_) &&
            l.result == r.result;
    }

    friend inline size_t hash_value(entity_signature const& v) noexcept
    {
        return hasher{}(v.name, span{ v.fields_ }, v.result);
    }
};

// entities whose identities are based on their signatures
class signatured_entity : public entity
{
public:
    using entity::entity;

    inline size_t hash() const noexcept override final
    {
        return hash_value(*signature());
    }

    inline bool equal(signatured_entity const& rhs) const noexcept
    {
        return *rhs.signature() == *signature();
    }

    bool equal(entity const& rhs) const noexcept override final
    {
        entity_signature const* prhssig = rhs.signature();
        if (!prhssig) return false;
        return *signature() == *prhssig;

        //if (signatured_entity const* pr = dynamic_cast<signatured_entity const*>(&rhs); pr) {
        //    return equal(*pr);
        //}
        //return false;
    }

    std::ostream& print_to(std::ostream&, unit const&) const override;
};

// auxiliary type for entity lookups by a signature
struct indirect_signatured_entity : signatured_entity
{
    entity_signature& sig_;

public:
    explicit indirect_signatured_entity(entity_signature& s) : sig_{ s } {}

    inline entity_signature const* signature() const noexcept override final { return &sig_; }
};

struct basic_signatured_entity : signatured_entity
{
    entity_signature sig_;

    basic_signatured_entity() = default;

    inline explicit basic_signatured_entity(entity_signature&& sgn) noexcept
        : sig_{ std::move(sgn) }
    {}

    inline void set_signature(entity_signature&& sgn) { sig_ = std::move(sgn); }

    entity_signature const* signature() const noexcept override final { return &sig_; }
    entity_signature * signature() noexcept { return &sig_; }

    entity_identifier get_type() const noexcept override
    {
        return sig_.result.value_or(field_descriptor{}).entity_id();
    }
};

}
