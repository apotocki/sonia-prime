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

#include <boost/assert.hpp>

#include "cstdint.hpp"
#include "type_traits.hpp"

#include "sonia/functional/range_equal.hpp"
#include "sonia/functional/range_less.hpp"
#include "sonia/utility/explicit_operator_bool.hpp"

namespace sonia {

template <typename T>
class array_view
{
public:
    // range resembling
    typedef remove_cv_t<T> value_type;
    typedef add_const_t<T> const_value_type;
    typedef add_pointer_t<value_type> pointer;
    typedef add_lvalue_reference_t<T> reference;
    typedef add_lvalue_reference_t<const_value_type> const_reference;
    typedef size_t size_type;
    typedef add_pointer_t<T> iterator;
    typedef add_pointer_t<const_value_type> const_iterator;

    constexpr array_view() noexcept : data_(nullptr), size_(0) {}
    constexpr array_view(T * d, size_type sz) noexcept : data_(d), size_(sz) {}

    template <size_t N>
    constexpr array_view(T(&arr)[N]) : data_(arr), size_(N) {}

    explicit array_view(T * b, T * e) noexcept
        : data_(b), size_(e - b)
    {
        BOOST_ASSERT(std::less_equal<T*>()(b, e));
    }

    template <typename OtherT> 
    array_view(array_view<OtherT> arr) : data_(arr.begin()), size_(arr.size()) {
        static_assert(sizeof(T) == sizeof(OtherT));
    }

    iterator begin() const noexcept { return data_; }
    iterator end() const noexcept { return data_ + size_; }
    const_iterator cbegin() const noexcept { return data_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    reference front() const noexcept { return data_[0]; }
    reference back()  const noexcept { return data_[size_ - 1]; }
    iterator data() const noexcept { return data_; }

    constexpr bool empty() const noexcept { return !size_; }

    constexpr size_type size() const noexcept { return size_; }
    void reset() noexcept { data_ = nullptr; size_ = 0; }

    reference operator[](size_type ind) const noexcept { return data_[ind]; }
    bool operator!() const noexcept { return empty(); }

    BOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL();

    bool is_subset_of(array_view r) const {
        return std::less_equal<T*>()(r.data_, data_) && std::less_equal<T*>()(data_, r.end()) &&
            data_ - r.data_ + size_ <= r.size_;
    }

protected:
    T * data_;
    size_t size_;
};

typedef array_view<uint8_t> byte_array_view;

template <typename T>
bool operator== (array_view<T> const& lhs, array_view<T> const& rhs) {
    return range_equal()(lhs, rhs);
}

template <typename T>
bool operator< (array_view<T> const& lhs, array_view<T> const& rhs) {
    return range_less()(lhs, rhs);
}

template <typename CharT, class TraitsT, typename T>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, array_view<T> arr) {
    os << '[';
    bool first = true;
    for (T const& val : arr) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << val;
    }
    return os << ']';
}

template <typename T, size_t N>
array_view<T> to_array_view(T (&arr)[N]) {
    return array_view<T>(arr, N);
}

template <typename T>
array_view<T> to_array_view(std::vector<T> & v) {
    return array_view<T>(v.empty() ? nullptr : &v.front(), v.size());
}

template <typename T>
array_view<const T> to_array_view(std::vector<T> const& v) {
    return array_view<const T>(v.empty() ? nullptr : &v.front(), v.size());
}

}

#endif // SONIA_ARRAY_VIEW_HPP
