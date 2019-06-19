//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <Windows.h>
#include <threadpoolapiset.h>

namespace sonia::windows {

PTP_POOL create_threadpool();
void close_threadpool(PTP_POOL) noexcept;

void set_threadpool_thread_minimum(PTP_POOL, size_t);
void set_threadpool_thread_maximum(PTP_POOL, size_t);

void initialize_threadpool_environment(PTP_CALLBACK_ENVIRON) noexcept;
void destroy_threadpool_environment(PTP_CALLBACK_ENVIRON) noexcept;
void set_threadpool_callback_pool(PTP_CALLBACK_ENVIRON, PTP_POOL ptpp = nullptr) noexcept;

PTP_CLEANUP_GROUP create_threadpool_cleanup_group();
void close_threadpool_cleanup_group(PTP_CLEANUP_GROUP) noexcept;
void set_threadpool_callback_cleanup_group(PTP_CALLBACK_ENVIRON, PTP_CLEANUP_GROUP, PTP_CLEANUP_GROUP_CANCEL_CALLBACK) noexcept;
void close_threadpool_cleanup_group_members(PTP_CLEANUP_GROUP, bool fCancelPendingCallbacks, PVOID pvCleanupContext) noexcept;

PTP_TIMER create_threadpool_timer(PTP_TIMER_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON pcbe = nullptr);
void set_threadpool_timer(PTP_TIMER, PFILETIME pftDueTime, DWORD msPeriod, DWORD msWindowLength) noexcept;
void wait_for_threadpool_timer_callbacks(PTP_TIMER, bool fCancelPendingCallbacks) noexcept;
void close_threadpool_timer(PTP_TIMER) noexcept;

}
