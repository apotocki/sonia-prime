
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//#include <algorithm>
//#include <memory>
//#include <type_traits>
//#include <utility>

#include "sonia/fibers/future/future.hpp"
#include "sonia/fibers/future/packaged_task.hpp"
#include "sonia/fibers/policy.hpp"

namespace sonia::fibers {

//#if defined(BOOST_MSVC) && (_MSC_VER >= 1911 && _MSVC_LANG >= 201703)
//template <typename>
//struct result_of;
//template <typename F, typename... Args>
//struct result_of<F(Args...)> : std::invoke_result<F, Args...> {};
//#else
//using std::result_of;
//#endif

template <typename>
struct result_of;
template <typename F, typename... Args>
struct result_of<F(Args...)> : std::invoke_result<F, Args...> {};

template< typename Fn, typename ... Args >
future<std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>>
async( Fn && fn, Args ... args) requires (detail::is_launch_policy<std::decay_t<Fn>>::value)
{
    using result_type = std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>;
    packaged_task< result_type( typename std::decay< Args >::type ... ) > pt{
            std::forward< Fn >( fn) };
    future< result_type > f{ pt.get_future() };
    fiber{ std::move( pt), std::forward< Args >( args) ... }.detach();
    return f;
}
/*
template< typename Fn, typename ... Args >

future<
    typename result_of<
        typename std::enable_if<
            ! detail::is_launch_policy< typename std::decay< Fn >::type >::value,
            typename std::decay< Fn >::type
        >::type( typename std::decay< Args >::type ... )
    >::type
>
async( Fn && fn, Args ... args) {
    typedef typename result_of<
        typename std::decay< Fn >::type( typename std::decay< Args >::type ... )
    >::type     result_type;

    packaged_task< result_type( typename std::decay< Args >::type ... ) > pt{
        std::forward< Fn >( fn) };
    future< result_type > f{ pt.get_future() };
    fiber{ std::move( pt), std::forward< Args >( args) ... }.detach();
    return f;
}
*/
template< typename Policy, typename Fn, typename ... Args >
future<
    typename result_of<
        typename std::enable_if<
            detail::is_launch_policy< Policy >::value,
            typename std::decay< Fn >::type
        >::type( typename std::decay< Args >::type ...)
    >::type
>
async( Policy policy, Fn && fn, Args ... args) {
    typedef typename result_of<
        typename std::decay< Fn >::type( typename std::decay< Args >::type ... )
    >::type     result_type;

    packaged_task< result_type( typename std::decay< Args >::type ... ) > pt{
        std::forward< Fn >( fn) };
    future< result_type > f{ pt.get_future() };
    fiber{ policy, std::move( pt), std::forward< Args >( args) ... }.detach();
    return f;
}

template< typename Policy, typename StackAllocator, typename Fn, typename ... Args >
future<
    typename result_of<
        typename std::enable_if<
            detail::is_launch_policy< Policy >::value,
            typename std::decay< Fn >::type
        >::type( typename std::decay< Args >::type ... )
    >::type
>
async( Policy policy, std::allocator_arg_t, StackAllocator salloc, Fn && fn, Args ... args) {
    typedef typename result_of<
        typename std::decay< Fn >::type( typename std::decay< Args >::type ... )
    >::type     result_type;

    packaged_task< result_type( typename std::decay< Args >::type ... ) > pt{
        std::forward< Fn >( fn) };
    future< result_type > f{ pt.get_future() };
    fiber{ policy, std::allocator_arg, salloc,
        std::move( pt), std::forward< Args >( args) ... }.detach();
    return f;
}

template< typename Policy, typename StackAllocator, typename Allocator, typename Fn, typename ... Args >
future<
    typename result_of<
        typename std::enable_if<
            detail::is_launch_policy< Policy >::value,
            typename std::decay< Fn >::type
        >::type( typename std::decay< Args >::type ... )
    >::type
>
async( Policy policy, std::allocator_arg_t, StackAllocator salloc, Allocator alloc, Fn && fn, Args ... args) {
    typedef typename result_of<
        typename std::decay< Fn >::type( typename std::decay< Args >::type ... )
    >::type     result_type;

    packaged_task< result_type( typename std::decay< Args >::type ... ) > pt{
        std::allocator_arg, alloc, std::forward< Fn >( fn) };
    future< result_type > f{ pt.get_future() };
    fiber{ policy, std::allocator_arg, salloc,
        std::move( pt), std::forward< Args >( args) ... }.detach();
    return f;
}

}
