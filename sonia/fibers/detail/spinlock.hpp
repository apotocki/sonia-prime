
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "sonia/fibers/detail/config.hpp"

#if !defined(SONIA_FIBERS_NO_ATOMICS) 
# include <mutex>
# include "sonia/fibers/detail/spinlock_ttas_adaptive.hpp"
# include "sonia/fibers/detail/spinlock_ttas.hpp"
# if defined(SONIA_FIBERS_HAS_FUTEX)
#  include "sonia/fibers/detail/spinlock_ttas_adaptive_futex.hpp"
#  include "sonia/fibers/detail/spinlock_ttas_futex.hpp"
# endif
# if defined(BOOST_USE_TSX)
#  include "sonia/fibers/detail/spinlock_rtm.hpp"
# endif
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace sonia::fibers::detail {

#if defined(SONIA_FIBERS_NO_ATOMICS)
struct spinlock {
    constexpr spinlock() noexcept {}
    void lock() noexcept {}
    void unlock() noexcept {}
};

struct spinlock_lock {
    constexpr spinlock_lock( spinlock &) noexcept {}
    void lock() noexcept {}
    void unlock() noexcept {}
};
#else
# if defined(SONIA_FIBERS_SPINLOCK_STD_MUTEX)
using spinlock = std::mutex;
# elif defined(SONIA_FIBERS_SPINLOCK_TTAS_FUTEX)
#  if defined(BOOST_USE_TSX)
using spinlock = spinlock_rtm< spinlock_ttas_futex >;
#  else
using spinlock = spinlock_ttas_futex;
#  endif
# elif defined(SONIA_FIBERS_SPINLOCK_TTAS_ADAPTIVE_FUTEX)
#  if defined(BOOST_USE_TSX)
using spinlock = spinlock_rtm< spinlock_ttas_adaptive_futex >;
#  else
using spinlock = spinlock_ttas_adaptive_futex;
#  endif
# elif defined(SONIA_FIBERS_SPINLOCK_TTAS_ADAPTIVE)
#  if defined(BOOST_USE_TSX)
using spinlock = spinlock_rtm< spinlock_ttas_adaptive >;
#  else
using spinlock = spinlock_ttas_adaptive;
#  endif
# else
#  if defined(BOOST_USE_TSX)
using spinlock = spinlock_rtm< spinlock_ttas >;
#  else
using spinlock = spinlock_ttas;
#  endif
# endif
using spinlock_lock = std::unique_lock< spinlock >;
#endif

}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
