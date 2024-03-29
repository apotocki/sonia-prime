
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

#include "sonia/fibers/detail/config.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace sonia::fibers {

enum class launch
{
    dispatch,
    post
};

namespace detail {

template <typename Fn>
struct is_launch_policy : public std::false_type {};

template <>
struct is_launch_policy<sonia::fibers::launch> : public std::true_type {};

template <typename Fn>
constexpr bool is_launch_policy_v = is_launch_policy<Fn>::value_type;

}

}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
