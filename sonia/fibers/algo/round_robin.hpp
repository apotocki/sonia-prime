//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <condition_variable>
#include <chrono>
#include <mutex>

#include "sonia/fibers/algo/algorithm.hpp"
#include "sonia/fibers/context.hpp"
#include "sonia/fibers/detail/config.hpp"
#include "sonia/fibers/scheduler.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4251)
#endif

namespace sonia::fibers::algo {

class round_robin : public algorithm {
private:
    using rqueue_type = scheduler::ready_queue_type;

    rqueue_type                 rqueue_{};
    std::mutex                  mtx_{};
    std::condition_variable     cnd_{};
    bool                        flag_{ false };

public:
    round_robin() = default;

    round_robin( round_robin const&) = delete;
    round_robin & operator=( round_robin const&) = delete;

    void awakened( context *) noexcept override;

    context * pick_next() noexcept override;

    bool has_ready_fibers() const noexcept override;

    void suspend_until( std::chrono::steady_clock::time_point const&) noexcept override;

    void notify() noexcept override;
};

}

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
