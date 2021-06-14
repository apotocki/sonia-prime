//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <optional>
#include <memory>

namespace sonia {

using std::optional;
using std::in_place;
using std::in_place_t;
using std::nullopt;
using std::nullopt_t;

template <class T> struct is_optional : std::false_type {};
template <class T> struct is_optional<optional<T>> : std::true_type{};
template <class T> constexpr bool is_optional_v = is_optional<T>::value;

}

namespace std {

template <typename T> T const * get_pointer(optional<T> const& opt) { return opt ? std::addressof(*opt) : nullptr; }
template <typename T> T * get_pointer(optional<T> & opt) { return opt ? std::addressof(*opt) : nullptr; }

}
