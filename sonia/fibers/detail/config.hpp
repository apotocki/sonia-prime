
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>

#if BOOST_OS_LINUX || BOOST_OS_WINDOWS
# define SONIA_FIBERS_HAS_FUTEX
#endif

#if (!defined(SONIA_FIBERS_HAS_FUTEX) && \
    (defined(SONIA_FIBERS_SPINLOCK_TTAS_FUTEX) || defined(SONIA_FIBERS_SPINLOCK_TTAS_ADAPTIVE_FUTEX)))
# error "futex not supported on this platform"
#endif

#if !defined(SONIA_FIBERS_CONTENTION_WINDOW_THRESHOLD)
# define SONIA_FIBERS_CONTENTION_WINDOW_THRESHOLD 16
#endif

#if !defined(SONIA_FIBERS_RETRY_THRESHOLD)
# define SONIA_FIBERS_RETRY_THRESHOLD 64
#endif

#if !defined(SONIA_FIBERS_SPIN_BEFORE_SLEEP0)
# define SONIA_FIBERS_SPIN_BEFORE_SLEEP0 32
#endif

#if !defined(SONIA_FIBERS_SPIN_BEFORE_YIELD)
# define SONIA_FIBERS_SPIN_BEFORE_YIELD 64
#endif
