//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_OPTIONAL_HPP
#define SONIA_OPTIONAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <optional>
#include <memory>

namespace sonia {

using std::optional;
using std::in_place;
using std::in_place_t;
using std::nullopt;
using std::nullopt_t;

}

namespace std {

template <typename T> T const * get_pointer(optional<T> const& opt) { return opt ? std::addressof(*opt) : nullptr; }
template <typename T> T * get_pointer(optional<T> & opt) { return opt ? std::addressof(*opt) : nullptr; }

}

#endif // SONIA_OPTIONAL_HPP
