
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <algorithm>
#include <exception>
#include <memory>
#include <utility>

#include <boost/assert.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/predef.h>

#include "sonia/fibers/detail/config.hpp"
#include "sonia/fibers/detail/disable_overload.hpp"
#include "sonia/fibers/context.hpp"
#include "sonia/fibers/fixedsize_stack.hpp"
#include "sonia/fibers/policy.hpp"
#include "sonia/fibers/properties.hpp"
#include "sonia/fibers/segmented_stack.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4251)
#endif

namespace sonia::fibers {

class fiber {
private:
    friend class context;

    using ptr_t = boost::intrusive_ptr<context>;

    ptr_t       impl_{};

    void start_() noexcept;

public:
    using id = context::id;

    fiber() = default;

    template< typename Fn,
              typename ... Arg,
              typename = detail::disable_overload< fiber, Fn >,
              typename = detail::disable_overload< launch, Fn >,
              typename = detail::disable_overload< std::allocator_arg_t, Fn >
    >
#if BOOST_COMP_GNUC < 50000000
    explicit fiber( Fn && fn, Arg && ... arg) :
#else
    fiber( Fn && fn, Arg ... arg) :
#endif
        fiber{ launch::post,
               std::allocator_arg, default_stack(),
               std::forward< Fn >( fn), std::forward< Arg >( arg) ... } {
    }

    template< typename Fn,
              typename ... Arg,
              typename = detail::disable_overload< fiber, Fn >
    >
#if BOOST_COMP_GNUC < 50000000
    fiber( launch policy, Fn && fn, Arg && ... arg) :
#else
    fiber( launch policy, Fn && fn, Arg ... arg) :
#endif
        fiber{ policy,
               std::allocator_arg, default_stack(),
               std::forward< Fn >( fn), std::forward< Arg >( arg) ... } {
    }

    template< typename StackAllocator,
              typename Fn,
              typename ... Arg
    >
#if BOOST_COMP_GNUC < 50000000
    fiber( std::allocator_arg_t, StackAllocator && salloc, Fn && fn, Arg && ... arg) :
#else
    fiber( std::allocator_arg_t, StackAllocator && salloc, Fn && fn, Arg ... arg) :
#endif
        fiber{ launch::post,
               std::allocator_arg, std::forward< StackAllocator >( salloc),
               std::forward< Fn >( fn), std::forward< Arg >( arg) ... } {
    }

    template< typename StackAllocator,
              typename Fn,
              typename ... Arg
    >
#if BOOST_COMP_GNUC < 50000000
    fiber( launch policy, std::allocator_arg_t, StackAllocator && salloc, Fn && fn, Arg && ... arg) :
#else
    fiber( launch policy, std::allocator_arg_t, StackAllocator && salloc, Fn && fn, Arg ... arg) :
#endif
        impl_{ make_worker_context( policy, std::forward< StackAllocator >( salloc), std::forward< Fn >( fn), std::forward< Arg >( arg) ... ) } {
        start_();
    }

    ~fiber() {
        if ( joinable() ) {
            std::terminate();
        }
    }

    fiber( fiber const&) = delete;
    fiber & operator=( fiber const&) = delete;

    fiber( fiber && other) noexcept :
        impl_{} {
        swap( other);
    }

    fiber & operator=( fiber && other) noexcept {
        if ( joinable() ) {
            std::terminate();
        }
        if ( BOOST_UNLIKELY( this == & other) ) {
            return * this;
        }
        impl_.swap( other.impl_);
        return * this;
    }

    void swap( fiber & other) noexcept {
        impl_.swap( other.impl_);
    }

    id get_id() const noexcept {
        return impl_ ? impl_->get_id() : id();
    }

    bool joinable() const noexcept {
        return nullptr != impl_;
    }

    void join();

    void detach();

    template< typename PROPS >
    PROPS & properties() {
        auto props = impl_->get_properties();
        BOOST_ASSERT_MSG( props, "fiber::properties not set");
        return dynamic_cast< PROPS & >( * props );
    }
};

inline
bool operator<( fiber const& l, fiber const& r) noexcept {
    return l.get_id() < r.get_id();
}

inline
void swap( fiber & l, fiber & r) noexcept {
    return l.swap( r);
}

}

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
