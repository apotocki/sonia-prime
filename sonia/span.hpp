//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <span>
#include <boost/iterator_adaptors.hpp>

namespace sonia {

using std::span;

template <typename T, size_t EVL, size_t EVR>
inline bool operator== (std::span<T, EVL> l, std::span<T, EVR> r)
{
    return l.data() == r.data() && l.size() == r.size();
}

template  <typename T, size_t EV>
inline T* data_end(std::span<T, EV> s)
{
    return s.data() + s.size();
}

template  <typename T, size_t EV>
inline std::span<T> advance_back(std::span<T, EV> s, std::ptrdiff_t dist)
{
    return { s.data(), s.size() + dist };
}

// is r a subset of the l
template <typename T, size_t EVL, size_t EVR>
inline bool is_subset_of(std::span<T, EVL> l, std::span<T, EVR> r)
{
    return std::less_equal<T*>()(r.data(), l.data()) && std::less_equal<T*>()(l.data(), data_end(r)) &&
        l.data() - r.data() + l.size() <= r.size();
}

template <class T>
struct is_span : std::false_type {};

template <typename T, size_t E>
struct is_span<std::span<T, E>> : std::true_type {};

template <typename T>
constexpr bool is_span_v = is_span<T>::value;

}

#include <boost/range/range_fwd.hpp>

namespace boost {

template <typename T, size_t EV>
struct range_iterator<const sonia::span<T, EV>, void>
{
    using type = typename sonia::span<T, EV>::iterator;
};

/*
template <class T, size_t EV>
struct range_mutable_iterator<std::span<T, EV>>
{
    using type = typename std::span<T, EV>::iterator;
};
*/

}

#define SONIA_SPAN_DECLARED

#include "sonia/utility/functional/hash/span.hpp"
#include "sonia/utility/streaming/span.hpp"
