//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include <boost/context/fiber.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/intrusive/slist.hpp>

#include "sonia/fibers/algo/algorithm.hpp"
#include "sonia/fibers/context.hpp"
#include "sonia/fibers/detail/config.hpp"
#include "sonia/fibers/detail/data.hpp"
#include "sonia/fibers/detail/spinlock.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4251)
#endif

namespace sonia::fibers {

class scheduler_group {
public:
    virtual ~scheduler_group() = default;

    void schedule(context* ctx) noexcept;
};

class scheduler {
public:
    struct timepoint_less {
        bool operator()( context const& l, context const& r) const noexcept {
            return l.tp_ < r.tp_;
        }
    };

    using ready_queue_type = boost::intrusive::list<
        context,
        boost::intrusive::member_hook<
            context, detail::ready_hook, & context::ready_hook_ >,
        boost::intrusive::constant_time_size< false >
    >;

private:
    using sleep_queue_type = boost::intrusive::multiset<
        context,
        boost::intrusive::member_hook<
            context, detail::sleep_hook, & context::sleep_hook_ >,
        boost::intrusive::constant_time_size< false >,
        boost::intrusive::compare< timepoint_less >
    >;

    //using worker_queue_type = boost::intrusive::list<
    //    context,
    //    boost::intrusive::member_hook<
    //        context, detail::worker_hook, & context::worker_hook_ >,
    //    boost::intrusive::constant_time_size< false >
    //>;

    using terminated_queue_type = boost::intrusive::slist<
        context,
        boost::intrusive::member_hook<
            context, detail::terminated_hook, & context::terminated_hook_ >,
        boost::intrusive::linear< true >,
        boost::intrusive::cache_last< true >
    >;

    using remote_ready_queue_type = boost::intrusive::slist<
        context,
        boost::intrusive::member_hook<
            context, detail::remote_ready_hook, & context::remote_ready_hook_ >,
        boost::intrusive::linear< true >,
        boost::intrusive::cache_last< true >
    >;

    // remote ready-queue contains context' signaled by schedulers
    // running in other threads
    detail::spinlock                                            remote_ready_splk_{};
    remote_ready_queue_type                                     remote_ready_queue_{};

    algo::algorithm::ptr_t             algo_;
    // sleep-queue contains context' which have been called
    // scheduler::wait_until()
    sleep_queue_type                                            sleep_queue_{};
    // worker-queue contains all context' mananged by this scheduler
    // except main-context and dispatcher-context
    // unlink happens on destruction of a context
    //worker_queue_type                                           worker_queue_{};
    // terminated-queue contains context' which have been terminated
    terminated_queue_type                                       terminated_queue_{};
    boost::intrusive_ptr< context >                             dispatcher_ctx_{};
    context                                                 *   main_ctx_{ nullptr };
    bool                                                        shutdown_{ false };
    bool direct_foreign_scheduling_{ false };
         
    void release_terminated_() noexcept;

    void remote_ready2ready_() noexcept;

    void sleep2ready_() noexcept;

public:
    scheduler() noexcept;

    scheduler( scheduler const&) = delete;
    scheduler & operator=( scheduler const&) = delete;

    virtual ~scheduler();

    void schedule( context *) noexcept;

    void schedule_from_remote( context *) noexcept;

    boost::context::fiber dispatch() noexcept;

    boost::context::fiber terminate( detail::spinlock_lock &, context *) noexcept;

    void yield( context *) noexcept;

    bool wait_until( context *,
                     std::chrono::steady_clock::time_point const&) noexcept;
    bool wait_until( context *,
                     std::chrono::steady_clock::time_point const&,
                     detail::spinlock_lock &) noexcept;

    void suspend() noexcept;
    void suspend( detail::spinlock_lock &) noexcept;

    bool has_ready_fibers() const noexcept;

    void set_algo( algo::algorithm::ptr_t) noexcept;

    void attach_main_context( context *) noexcept;

    void attach_dispatcher_context( boost::intrusive_ptr< context >) noexcept;

    void attach_worker_context( context *) noexcept;

    void detach_worker_context( context *) noexcept;

    bool has_direct_foreign_scheduling() const noexcept { return direct_foreign_scheduling_; }

    scheduler_group* get_group() const noexcept { return algo_->get_group(); }
};



}

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

