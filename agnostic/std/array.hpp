// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ARRAY_HPP
#define AGNOSTIC_STD_ARRAY_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR
#   include "iterator/reverse_iterator.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_FILL_N
#   include "algorithm/fill_n.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_ITER_SWAP
#   include "algorithm/iter_swap.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_SWAPPABLE
#   include "type_traits/is_nothrow_swappable.hpp"
#endif

namespace std {

template <class T, size_t N>
struct array
{
    // types
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr void fill(const T& u)
    {
        fill_n(data_, N, u);
    }

    constexpr void swap(array& rhs) noexcept(is_nothrow_swappable_v<T>)
    {
        iterator first1 = data_, last1 = data_ + N;
        iterator first2 = rhs.data_;
        for (; first1 != last1; ++first1, ++first2) {
            iter_swap(first1, first2);
        }
    }

    // iterators
    [[nodiscard]] constexpr iterator               begin() noexcept { return data_; }
    [[nodiscard]] constexpr const_iterator         begin() const noexcept { return data_; }
    [[nodiscard]] constexpr iterator               end() noexcept { return data_ + N; }
    [[nodiscard]] constexpr const_iterator         end() const noexcept { return data_ + N; }

    [[nodiscard]] constexpr reverse_iterator       rbegin() noexcept { return reverse_iterator(end()); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    [[nodiscard]] constexpr reverse_iterator       rend() noexcept { return reverse_iterator(begin()); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    [[nodiscard]] constexpr const_iterator         cbegin() const noexcept { return data_; }
    [[nodiscard]] constexpr const_iterator         cend() const noexcept { return data_ + N; }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // capacity
    [[nodiscard]] constexpr bool empty() const noexcept { return !N; }
    [[nodiscard]] constexpr size_type size() const noexcept { return N; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return N; }

    // element access
    [[nodiscard]] constexpr reference       operator[](size_type n) noexcept { return data_[n]; }
    [[nodiscard]] constexpr const_reference operator[](size_type n) const noexcept { return data_[n]; }
    [[nodiscard]] constexpr reference       at(size_type n);
    [[nodiscard]] constexpr const_reference at(size_type n) const;
    [[nodiscard]] constexpr reference       front() noexcept { return *data_; }
    [[nodiscard]] constexpr const_reference front() const noexcept { return *data_; }
    [[nodiscard]] constexpr reference       back() noexcept { return data_[N - 1]; }
    [[nodiscard]] constexpr const_reference back() const noexcept { return data_[N - 1]; }

    [[nodiscard]] constexpr T* data() noexcept { return data_; }
    [[nodiscard]] constexpr const T* data() const noexcept { return data_; }

    T data_[N];
};

template<class T, class... U>
array(T, U...)->array<T, 1 + sizeof...(U)>;

}

#ifndef DO_NOT_USE_AGNOSTIC_OUT_OF_RANGE
#include "stdexcept/out_of_range.hpp"
#endif

namespace std {

template <class T, size_t N>
constexpr typename array<T, N>::reference array<T, N>::at(size_type n)
{
    if (N <= n)
        throw out_of_range();
    return data_[n];
}

template <class T, size_t N>
constexpr typename array<T, N>::const_reference array<T, N>::at(size_type n) const
{
    if (N <= n)
        throw out_of_range();
    return data_[n];
}

}

#endif // AGNOSTIC_STD_ARRAY_HPP
