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
#include "sonia/utility/explicit_operator_bool.hpp"

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
    using iterator = add_pointer_t<T>;
    using const_iterator = add_pointer_t<const_value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // potentially unsafe
    array_view(null_t) {}

    constexpr array_view() noexcept : data_(nullptr), size_(0) {}
    constexpr array_view(T * d, size_type sz) noexcept : data_(d), size_(sz) {}

    template <size_t N>
    constexpr array_view(T(&arr)[N]) noexcept : data_(arr), size_(N) {}

    constexpr explicit array_view(T * b, T * e) noexcept
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
    
    constexpr void advance_front(std::ptrdiff_t dist)
    {
        size_ -= dist;
        data_ += dist;
    }

    constexpr void advance_back(std::ptrdiff_t dist)
    {
        size_ += dist;
    }

    constexpr void reset() noexcept { data_ = nullptr; size_ = 0; }

    constexpr reference operator[](size_type ind) const noexcept { return data_[ind]; }
    constexpr bool operator!() const noexcept { return empty(); }

    BOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL();

    constexpr bool has(T const * e) const
    {
        return std::less_equal<T const*>()(data_, e) && std::less<T const*>()(e, end());
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

protected:
    T * data_;
    size_t size_;
};

using byte_array_view = array_view<uint8_t>;

template <class T> struct is_array_view : false_type {};
template <class T> struct is_array_view<array_view<T>> : true_type {};
template <class T> constexpr bool is_array_view_v = is_array_view<T>::value;

template <typename T>
bool operator== (array_view<T> const& lhs, array_view<T> const& rhs)
{
    return range_equal()(lhs, rhs);
}

template <typename T>
bool operator< (array_view<T> const& lhs, array_view<T> const& rhs)
{
    return range_less()(lhs, rhs);
}

template <typename T>
array_view<T> to_array_view(array_view<T> av) { return av; }

template <typename T, size_t N>
array_view<T> to_array_view(T (&arr)[N])
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

}

#endif // SONIA_ARRAY_VIEW_HPP
