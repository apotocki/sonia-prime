//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <algorithm>
#include <boost/container/small_vector.hpp>

#include "sonia/span.hpp"
#include "sonia/utility/functional/range_equal.hpp"

#include "agnostic/std/algorithm/lexicographical_compare_three_way.hpp"

namespace sonia::lang {

template <typename IdentifierT>
class qname
{
public:
    using identifier_type = IdentifierT;

    friend bool operator== (qname const& l, qname const& r)
    {
        return l.is_absolute() == r.is_absolute() && range_equal()(l.parts_, r.parts_);
    }

    friend auto operator<=>(qname const& lhs, qname const& rhs)
    {
        assert(lhs.is_absolute() == rhs.is_absolute());
        return std::lexicographical_compare_three_way(
            lhs.parts_.begin(), lhs.parts_.end(),
            rhs.parts_.begin(), rhs.parts_.end());
    }

    qname() : absolute_{ true } {}

    explicit qname(IdentifierT pid, bool is_abs = true)
        : absolute_{ is_abs }
    {
        parts_.emplace_back(pid);
    }

    qname(span<const IdentifierT> ids, bool is_absolute)
        : parts_{ ids.begin(), ids.end() }
        , absolute_{ is_absolute }
    {}

    void append(IdentifierT pid)
    {
        parts_.emplace_back(pid);
    }

    void append(span<const IdentifierT> ids)
    {
        parts_.insert(parts_.end(), ids.begin(), ids.end());
    }

    inline void set_relative(bool val = true) noexcept { absolute_ = !val; }
    inline void set_absolute(bool val = true) noexcept { absolute_ = val; }

    inline bool is_relative() const noexcept { return !absolute_; }
    inline bool is_absolute() const noexcept { return absolute_; }

    qname& operator+= (qname const& r)
    {
        if (r.is_absolute()) {
            parts_.clear();
            absolute_ = true;
        }
        parts_.insert(parts_.end(), r.parts_.begin(), r.parts_.end());
        return *this;
    }

    qname& operator+= (IdentifierT r)
    {
        parts_.emplace_back(r);
        return *this;
    }

    span<const IdentifierT> parts() const
    {
        return span<const IdentifierT>{parts_.data(), parts_.size()};
    }

    IdentifierT local_name() const { return parts_.back(); }

    void truncate(size_t sz)
    {
        parts_.resize(sz);
    }

    bool has_prefix(span<const IdentifierT> sp) const
    {
        if (parts_.size() < sp.size()) return false;
        return std::ranges::equal(parts().subspan(0, sp.size()), sp);
    }

    auto begin() const { return parts_.begin(); }
    auto end() const { return parts_.end(); }

private:
    boost::container::small_vector<IdentifierT, 4> parts_;
    bool absolute_;
};

template <typename IdentifierT>
inline size_t hash_value(qname<IdentifierT> const& v)
{
    return hasher{}(v.parts(), v.is_absolute());
}

class qname_identifier
{
    uint32_t value_ : 31;
    uint32_t absolute_ : 1;

public:
    qname_identifier() : value_{0x7fffffffu}, absolute_{1u} {}

    qname_identifier(size_t idvalue, bool is_absolute) : value_{ static_cast<uint32_t>(idvalue) }, absolute_{ is_absolute ? 1u : 0 } {}

    friend inline bool operator== (qname_identifier const& l, qname_identifier const& r)
    {
        return l.value_ == r.value_;
    }

    friend inline auto operator<=>(qname_identifier const& l, qname_identifier const& r)
    {
        return uint32_t(l.value_) <=> uint32_t(r.value_);
    }

    inline bool is_relative() const noexcept { return !absolute_; }
    inline bool is_absolute() const noexcept { return !!absolute_; }
    uint32_t raw() const { return value_; }
};

inline size_t hash_value(qname_identifier const& v)
{
    return hasher{}(v.raw());
}

template <typename IdentifierT>
class qname_view : public span<const IdentifierT>
{
    using base_t = span<const IdentifierT>;
    using qname_t = qname<IdentifierT>;

public:
    qname_view() = default;
    qname_view(qname_t const& qn) 
        : base_t{ qn.parts() }
        , absolute_{ qn.is_absolute() }
    {}
    qname_view(span<const IdentifierT> sp, bool isabs = true)
        : base_t{ sp }
        , absolute_{ isabs }
    {}
    qname_view(qname_view const&) = default;
    qname_view& operator=(qname_view const&) = default;

    qname_view parent() const
    {
        assert(base_t::size());
        return qname_view{span{base_t::data(), base_t::size() - 1}, absolute_ };
    }

    inline bool is_relative() const noexcept { return !absolute_; }
    inline bool is_absolute() const noexcept { return absolute_; }

    friend bool operator== (qname_view const& l, qname_view const& r)
    {
        return l.is_absolute() == r.is_absolute() && range_equal()(l, r);
    }

    friend auto operator<=>(qname_view const& l, qname_view const& r)
    {
        assert(l.is_absolute() == r.is_absolute());
        return std::lexicographical_compare_three_way(
            l.begin(), l.end(),
            r.begin(), r.end());
    }

    bool has_prefix(span<const IdentifierT> sp) const
    {
        if (base_t::size() < sp.size()) return false;
        return std::ranges::equal(base_t::subspan(0, sp.size()), sp);
    }

private:
    bool absolute_;
};

template <typename IdentifierT>
inline qname<IdentifierT> operator+ (qname<IdentifierT> const& base, IdentifierT leaf)
{
    qname<IdentifierT> result{ base };
    result.append(leaf);
    return std::move(result);
}

template <typename IdentifierT>
inline qname<IdentifierT> operator+ (qname<IdentifierT> const& base, qname_view<IdentifierT> leaf)
{
    if (leaf.is_absolute()) {
        return { leaf, true };
    }
    qname<IdentifierT> result{ base };
    result.append(leaf);
    return std::move(result);
}

template <typename IdentifierT>
inline qname<IdentifierT> operator+ (qname<IdentifierT> const& base, qname<IdentifierT> const& leaf)
{
    return base + (qname_view<IdentifierT>)leaf;
}

template <typename IdentifierT>
inline qname<IdentifierT> operator+ (qname_view<IdentifierT> base, IdentifierT leaf)
{
    return qname<IdentifierT>{ base, base.is_absolute() } + leaf;
}

template <typename IdentifierT>
inline qname<IdentifierT> operator+ (qname_view<IdentifierT> base, qname_view<IdentifierT> leaf)
{
    return qname<IdentifierT>{ base, base.is_absolute() } + leaf;
}

}
