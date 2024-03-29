
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>

#include "sonia/fibers/detail/disable_overload.hpp"
#include "sonia/fibers/exceptions.hpp"
#include "sonia/fibers/future/detail/task_base.hpp"
#include "sonia/fibers/future/detail/task_object.hpp"
#include "sonia/fibers/future/future.hpp"

namespace sonia::fibers {

template< typename Signature >
class packaged_task;

template< typename R, typename ... Args >
class packaged_task< R( Args ... ) > {
private:
    using ptr_type = typename detail::task_base< R, Args ... >::ptr_type;

    bool            obtained_{ false };
    ptr_type        task_{};

public:
    packaged_task() = default;

    template< typename Fn,
              typename = detail::disable_overload< packaged_task, Fn >
    >
    explicit packaged_task( Fn && fn) : 
        packaged_task{ std::allocator_arg,
                       std::allocator< packaged_task >{},
                       std::forward< Fn >( fn)  } {
    }

    template< typename Fn,
              typename Allocator
    >
    explicit packaged_task( std::allocator_arg_t, Allocator const& alloc, Fn && fn) {
        using object_type = detail::task_object<
            typename std::decay< Fn >::type, Allocator, R, Args ...
        >;
        using traits_type = std::allocator_traits<
            typename object_type::allocator_type
        >;
        using ptrait_type = boost::pointer_traits< typename traits_type::pointer >;

        typename object_type::allocator_type a{ alloc };
        typename traits_type::pointer ptr{ traits_type::allocate( a, 1) };
        typename ptrait_type::element_type* p = boost::to_address(ptr);
        try {
            traits_type::construct( a, p, a, std::forward< Fn >( fn) );
        } catch (...) {
            traits_type::deallocate( a, ptr, 1);
            throw;
        }
        task_.reset(p);
    }

    ~packaged_task() {
        if ( task_ && obtained_) {
            task_->owner_destroyed();
        }
    }

    packaged_task( packaged_task const&) = delete;
    packaged_task & operator=( packaged_task const&) = delete;

    packaged_task( packaged_task && other) noexcept :
        obtained_{ other.obtained_ },
        task_{ std::move( other.task_)  } {
        other.obtained_ = false;
    }

    packaged_task & operator=( packaged_task && other) noexcept {
        if ( BOOST_LIKELY( this != & other) ) {
            packaged_task tmp{ std::move( other) };
            swap( tmp);
        }
        return * this;
    }

    void swap( packaged_task & other) noexcept {
        std::swap( obtained_, other.obtained_);
        task_.swap( other.task_);
    }

    bool valid() const noexcept {
        return nullptr != task_.get();
    }

    future< R > get_future() {
        if ( obtained_) {
            throw future_already_retrieved{};
        }
        if ( BOOST_UNLIKELY( ! valid() ) ) {
            throw packaged_task_uninitialized{};
        }
        obtained_ = true;
        return future< R >{
             boost::static_pointer_cast< detail::shared_state< R > >( task_) };
    }

    void operator()( Args ... args) {
        if ( BOOST_UNLIKELY( ! valid() ) ) {
            throw packaged_task_uninitialized{};
        }
        task_->run( std::forward< Args >( args) ... );
    }

    void reset() {
        if ( BOOST_UNLIKELY( ! valid() ) ) {
            throw packaged_task_uninitialized{};
        }
        packaged_task tmp;
        tmp.task_ = task_;
        task_ = tmp.task_->reset();
        obtained_ = false;
    }
};

template< typename Signature >
void swap( packaged_task< Signature > & l, packaged_task< Signature > & r) noexcept {
    l.swap( r);
}

}
