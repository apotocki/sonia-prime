//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_ARRAY_VIEW_HPP
#define SONIA_ARRAY_VIEW_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>
#include <functional>
#include <vector>
#include <array>
#include <iterator>

#include <boost/assert.hpp>

#include "cstdint.hpp"
#include "type_traits.hpp"

#include "sonia/utility/functional/range_equal.hpp"
#include "sonia/utility/functional/range_less.hpp"

namespace sonia {

template <typename T>
class array_view
{
public:
    // range resembling
    using value_type = remove_cv_t<T>;
    using const_value_type = add_const_t<T>;
    using pointer = add_pointer_t<T>;
    using reference = add_lvalue_reference_t<T>;
    using const_reference = add_lvalue_reference_t<const_value_type>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = add_pointer_t<T>;
    using const_iterator = add_pointer_t<const_value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = size_type(-1);

    // potentially unsafe
    explicit constexpr array_view(null_t) {}

    constexpr array_view() noexcept : data_(nullptr), size_(0) {}
    constexpr array_view(T * d, size_type sz) noexcept : data_(d), size_(sz) {}

    template <size_t N>
    constexpr array_view(T(&arr)[N]) noexcept : data_(arr), size_(N) {}

    constexpr array_view(T * b, T * e) noexcept
        : data_(b), size_(e - b)
    {
        BOOST_ASSERT(std::less_equal<T*>()(b, e));
    }

    template <typename OtherT>
    constexpr array_view(array_view<OtherT> arr) : data_(arr.begin()), size_(arr.size())
    {
        static_assert(sizeof(T) == sizeof(OtherT));
    }

    constexpr iterator begin() const noexcept { return data_; }
    constexpr iterator end() const noexcept { return data_ + size_; }
    constexpr const_iterator cbegin() const noexcept { return data_; }
    constexpr const_iterator cend() const noexcept { return data_ + size_; }

    reverse_iterator rbegin() const noexcept { return reverse_iterator{end()}; }
    reverse_iterator rend() const noexcept { return reverse_iterator{begin()}; }

    constexpr reference front() const noexcept { return data_[0]; }
    constexpr reference back()  const noexcept { return data_[size_ - 1]; }
    constexpr iterator data() const noexcept { return data_; }

    constexpr bool empty() const noexcept { return !size_; }

    constexpr size_type size() const noexcept { return size_; }

    constexpr array_view& advance_front(std::ptrdiff_t dist)
    {
        size_ -= dist;
        data_ += dist;
        return *this;
    }

    constexpr array_view& advance_back(std::ptrdiff_t dist)
    {
        size_ += dist;
        return *this;
    }

    constexpr void resize(size_type sz)
    {
        size_ = sz;
    }

    constexpr void reset() noexcept { data_ = nullptr; size_ = 0; }

    constexpr reference operator[](size_type ind) const noexcept { return data_[ind]; }

    constexpr explicit operator bool() const noexcept { return !empty(); }

    constexpr bool has(T const * e) const
    {
        return std::less_equal<T const*>()(data_, e) && std::less_equal<T const*>()(e, end());
    }

    constexpr bool is_subset_of(array_view r) const
    {
        return std::less_equal<T*>()(r.data_, data_) && std::less_equal<T*>()(data_, r.end()) &&
            data_ - r.data_ + size_ <= r.size_;
    }

    bool starts_with(array_view prefix) const
    {
        if (size_ < prefix.size()) return false;
        if constexpr (is_trivial_v<T>) {
            return 0 == std::memcmp(data_, prefix.data_, prefix.size() * sizeof(T));
        } else {
            return std::equal(prefix.begin(), prefix.end(), data_);
        }
    }

    bool ends_with(array_view suffix) const
    {
        if (size_ < suffix.size()) return false;
        if constexpr (is_trivial_v<T>) {
            return 0 == std::memcmp(end() - suffix.size(), suffix.data_, suffix.size() * sizeof(T));
        } else {
            return std::equal(suffix.begin(), suffix.end(), end() - suffix.size());
        }
    }

    size_type find(array_view v, size_t pos = 0) const
    {
        BOOST_ASSERT(size_ >= pos);
        const T * it = std::search(data_ + pos, data_ + size_, v.begin(), v.end());
        if (it == data_ + size_) return npos;
        return (it == data_ + size_) ? npos : it - (data_ + pos);
    }

    template <typename OffsT>
    constexpr array_view subview(OffsT offset) const;
    
    template <typename OffsT>
    constexpr array_view subview(OffsT offset, size_type sz) const;

protected:
    T * data_;
    size_t size_;
};

using byte_array_view = array_view<uint8_t>;

template <class T> struct is_array_view : false_type {};
template <class T> struct is_array_view<array_view<T>> : true_type {};
template <class T> constexpr bool is_array_view_v = is_array_view<T>::value;

template <typename LT, typename RT>
bool operator== (array_view<LT> const& lhs, array_view<RT> const& rhs)
{
    return range_equal()(lhs, rhs);
}

template <typename LT, typename RT>
bool operator!= (array_view<LT> const& lhs, array_view<RT> const& rhs)
{
    return !range_equal()(lhs, rhs);
}

template <typename T>
bool operator< (array_view<T> const& lhs, array_view<T> const& rhs)
{
    return range_less()(lhs, rhs);
}

template <typename T>
array_view<T> to_array_view(array_view<T> av) { return av; }

template <typename T, size_t N>
array_view<T> to_array_view(T(&arr)[N])
{
    return array_view<T>(arr, N);
}

template <typename T, class AllocatorT>
array_view<T> to_array_view(std::vector<T, AllocatorT> & v)
{
    return array_view<T>(v.empty() ? nullptr : &v.front(), v.size());
}

template <typename T, class AllocatorT>
array_view<const T> to_array_view(std::vector<T, AllocatorT> const& v)
{
    return array_view<const T>(v.empty() ? nullptr : &v.front(), v.size());
}

template <typename T, size_t SzV>
array_view<T> to_array_view(std::array<T, SzV> & v)
{
    return array_view<T>(SzV ? &v.front() : nullptr, SzV);
}

template <typename T, size_t SzV>
array_view<const T> to_array_view(std::array<T, SzV> const& v)
{
    return array_view<const T>(SzV ? &v.front() : nullptr, SzV);
}

template <typename ArrayT, typename OffsT>
constexpr auto to_subview(ArrayT && arr, OffsT offset)
{
    auto b = std::forward<ArrayT>(arr).begin();
    auto size = std::forward<ArrayT>(arr).size();
    //BOOST_ASSERT((size_type)std::abs(offset) <= sz);
    if constexpr (is_signed_v<OffsT>)
    {
        if (offset >= 0) {
            return array_view{b + offset, (size_t)(size - offset)};
        } else {
            return array_view{b + size + offset, (size_t)(-offset)};
        }
    } else {
        return array_view{b + offset, size - offset};
    }
}

template <typename ArrayT, typename OffsT>
constexpr auto to_subview(ArrayT && arr, OffsT offset, size_t sz)
{
    auto b = std::forward<ArrayT>(arr).begin();
    auto size = std::forward<ArrayT>(arr).size();

    if constexpr (is_signed_v<OffsT>)
    {
        if (offset >= 0) {
            return array_view{b + offset, sz};
        } else {
            return array_view{b + size + offset, sz};
        }
    } else {
        return array_view{b + offset, sz};
    }
}

template <typename T>
template <typename OffsT>
constexpr array_view<T> array_view<T>::subview(OffsT offset) const { return to_subview(*this, offset); }

template <typename T>
template <typename OffsT>
constexpr array_view<T> array_view<T>::subview(OffsT offset, size_type sz) const { return to_subview(*this, offset, sz); }

}

#include "sonia/utility/functional/hash.hpp"

namespace sonia {

template <typename T>
size_t hash_value(array_view<T> arr)
{
    size_t hash_value = hash_init_value();
    if constexpr (sizeof(T) == 1 && is_integral_v<T>)
    {
        const uint8_t * it = (uint8_t const*)(arr.begin());
        const uint8_t * eit = (uint8_t const*)(arr.end());
        for (; it != eit; ++it) {
            hash_value ^= static_cast<size_t>(*it);
            hash_value *= hash_prime_value();
        }
    } else {
        for (T const& item : arr) {
            hash_combine(hash_value, item);
        }
    }
    return hash_value;
}

}

#endif // SONIA_ARRAY_VIEW_HPP
