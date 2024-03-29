//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

namespace sonia {

struct null_t;
struct empty_t;

template <typename TagT, typename T, typename OutputIteratorT>
OutputIteratorT encode(T const& arg, OutputIteratorT oi);

template <typename TagT, typename T, typename InputIteratorT, typename ArgT>
InputIteratorT decode(InputIteratorT ii, ArgT && arg);

namespace serialization {

template <typename TagT, typename T> class coder;

struct default_t {}; // can be incompatible among systems
struct compressed_t {};
struct ordered_t {}; // compatible among systems
struct le_t {}; // little endian

template <size_t sz, class T> struct sized_t {}; // size modifier for serialization with a given size(in increments) of an external storage
template <size_t sz, class T> struct binary_t {}; // size modifier for serialization with a given binary size

template <std::integral T, size_t sz = sizeof(T)>
requires(sizeof(T) >= sz)
struct sized_integral_t
{
    T & value;
};

template <size_t sz, std::integral T>
inline sized_integral_t<T, sz> sized_integral(T & value) noexcept { return { value }; }

template <typename TagT>
class coder<TagT, null_t>
{
public:
    template <typename T, typename OutputIteratorT>
    OutputIteratorT encode(T const&, OutputIteratorT oi) const noexcept { return std::move(oi); }

    template <typename InputIteratorT, typename T>
    InputIteratorT decode(InputIteratorT ii, T &&) const noexcept { return std::move(ii); }
};

template <typename TagT> class coder<TagT, empty_t> : coder<TagT, null_t> {};

}}

#define SONIA_DECLARE_SERIALIZATION_FRIENDLY \
template <class, class> friend class ::sonia::serialization::coder;
