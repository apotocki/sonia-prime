//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#define SONIA_STD_VARIANT 1
#define SONIA_BOOST_VARIANT 2

#define SONIA_VARIANT_IMPLEMENTATION SONIA_STD_VARIANT

#if SONIA_VARIANT_IMPLEMENTATION == SONIA_STD_VARIANT
#   include <variant>
#   include "sonia/utility/functional/hash.hpp"

namespace sonia {

using std::variant;

template <typename... Ts>
struct match : Ts... { using Ts::operator()...; };
template <typename... Ts> match(Ts...) -> match<Ts...>;

template <typename T> struct is_variant : std::false_type {};
template <typename... Ts> struct is_variant<variant<Ts...>> : std::true_type {};
template <typename T> inline constexpr bool is_variant_v = is_variant<T>::value;

template <typename ... Ts>
struct hash<variant<Ts...>>
{
    inline size_t operator()(variant<Ts...> const& v) const noexcept
    {
        return std::visit([](auto const& val) -> size_t {
            return hash<std::decay_t<decltype(val)>>()(val);
        }, v);
    }
};

}

#include <iosfwd>
namespace std {

template <typename CharT, class TraitsT, typename ... Ts>
requires ((sizeof ... (Ts)) > 0)
inline basic_ostream<CharT, TraitsT>& operator<< (basic_ostream<CharT, TraitsT>& os, std::variant<Ts...> const& v)
{
    std::visit([&os](auto const& val) { os << val; }, v);
    return os;
}

}

#else
#include <type_traits>

#include <boost/variant.hpp>
#include <boost/variant/multivisitors.hpp>

namespace sonia {

using boost::variant;
using boost::recursive_variant_;
using boost::static_visitor;
using boost::apply_visitor;
using boost::get;
using boost::make_recursive_variant;
using boost::recursive_wrapper;

}

#include "sonia/utility/functional/less/variant.hpp"
#include "sonia/utility/streaming/variant.hpp"
#include "sonia/utility/functional/hash/variant.hpp"
#endif

#define SONIA_VARIANT_DECLARED

#include "sonia/utility/functional/equal/variant.hpp"
#include "sonia/utility/functional/less/variant.hpp"
