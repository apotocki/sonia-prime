
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

template< typename Fn, typename ... Args >
future<std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>>
async(Fn && fn, Args ... args) requires (detail::is_launch_policy_v<std::decay_t<Fn>>)
{
    using result_type = std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>;
    packaged_task<result_type(std::decay_t<Args> ...) > pt{ std::forward<Fn>(fn) };
    future<result_type> f{ pt.get_future() };
    fiber{ std::move( pt), std::forward<Args>( args) ... }.detach();
    return f;
}

template< typename Policy, typename Fn, typename ... Args >
future<std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>>
async(Policy policy, Fn&& fn, Args ... args) requires (detail::is_launch_policy_v<Policy>)
{
    using result_type = std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>;
    packaged_task< result_type(std::decay_t<Args> ...) > pt{ std::forward<Fn>(fn) };
    future<result_type> f{ pt.get_future() };
    fiber{ policy, std::move(pt), std::forward<Args>(args) ... }.detach();
    return f;
}

template <typename Policy, typename StackAllocator, typename Fn, typename ... Args>
future<std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>>
async(Policy policy, std::allocator_arg_t, StackAllocator salloc, Fn&& fn, Args ... args) requires (detail::is_launch_policy_v<Policy>)
{
    using result_type = std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>;
    packaged_task<result_type(std::decay_t<Args> ...) > pt{ std::forward<Fn>(fn) };
    future<result_type> f{ pt.get_future() };
    fiber{ policy, std::allocator_arg, salloc, std::move(pt), std::forward<Args>(args) ... }.detach();
    return f;
}

template <typename Policy, typename StackAllocator, typename Allocator, typename Fn, typename ... Args>
future<std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>>
async(Policy policy, std::allocator_arg_t, StackAllocator salloc, Allocator alloc, Fn&& fn, Args ... args) requires (detail::is_launch_policy_v<Policy>)
{
    using result_type = std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args> ...>;
    packaged_task< result_type(std::decay_t<Args> ...) > pt{ std::allocator_arg, alloc, std::forward<Fn>(fn) };
    future<result_type> f{ pt.get_future() };
    fiber{ policy, std::allocator_arg, salloc, std::move(pt), std::forward<Args>(args) ... }.detach();
    return f;
}

}
