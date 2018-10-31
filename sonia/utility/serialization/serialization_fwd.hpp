//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_SERIALIZATION_FWD_HPP
#define SONIA_UTILITY_SERIALIZATION_FWD_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia {

template <typename TagT, typename T, typename OutputIteratorT>
OutputIteratorT encode(T const& arg, OutputIteratorT oi);

template <typename TagT, typename T, typename InputIteratorT, typename ArgT>
InputIteratorT decode(InputIteratorT ii, ArgT && arg);

namespace serialization {

template <typename TagT, typename T, typename EnableT = void> class coder;

struct default_t {}; // can be incompatible among systems
struct compressed_t {};
struct ordered_t {}; // compatible among systems

}}

#define SONIA_DECLARE_SERIALIZATION_FRIENDLY \
template <class, class, class> friend class ::sonia::serialization::coder;

#endif // SONIA_UTILITY_SERIALIZATION_FWD_HPP
