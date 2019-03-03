//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SAL_NET_HPP
#define SONIA_SAL_NET_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia { namespace sal {

#ifdef BOOST_WINDOWS
using socket_type = uintptr_t;
static constexpr socket_type not_initialized_socket_v = (uintptr_t)(~0);
#else
using socket_type = int;
static constexpr socket_type not_initialized_socket_v = -1;
#endif

void close_socket(socket_type) noexcept;

}}

#endif // SONIA_SAL_NET_HPP
