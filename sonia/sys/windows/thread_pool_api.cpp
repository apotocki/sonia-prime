//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sys/windows/windows.hpp"
#include "thread_pool_api.hpp"

#include "sonia/exceptions.hpp"


namespace sonia::windows {

PTP_POOL create_threadpool()
{
    PTP_POOL pool = CreateThreadpool(NULL);
    if (!pool) {
        DWORD err = GetLastError();
        throw exception("CreateThreadpool() failed, error is %1%"_fmt % error_message(err));
    }
    return pool;
}

void close_threadpool(PTP_POOL ptpp) noexcept
{
    CloseThreadpool(ptpp);
}

void set_threadpool_thread_minimum(PTP_POOL pool, size_t val)
{
    BOOST_ASSERT ((std::numeric_limits<DWORD>::max)() >= val);
    if (!SetThreadpoolThreadMinimum(pool, (DWORD)val)) {
        DWORD err = GetLastError();
        throw exception("SetThreadpoolThreadMinimum() failed, error is %1%"_fmt % error_message(err));
    }
}

void set_threadpool_thread_maximum(PTP_POOL pool, size_t val)
{
    BOOST_ASSERT ((std::numeric_limits<DWORD>::max)() >= val);
    SetThreadpoolThreadMaximum(pool, (DWORD)val);
}

void initialize_threadpool_environment(PTP_CALLBACK_ENVIRON pcbe) noexcept
{
#ifndef __MINGW32__
    InitializeThreadpoolEnvironment(pcbe);
#endif
}

void destroy_threadpool_environment(PTP_CALLBACK_ENVIRON pcbe) noexcept
{
#ifndef __MINGW32__
    DestroyThreadpoolEnvironment(pcbe);
#endif
}

void set_threadpool_callback_pool(PTP_CALLBACK_ENVIRON pcbe, PTP_POOL ptpp) noexcept
{
#ifndef __MINGW32__
    SetThreadpoolCallbackPool(pcbe, ptpp);
#endif
}

PTP_CLEANUP_GROUP create_threadpool_cleanup_group()
{
    PTP_CLEANUP_GROUP group = CreateThreadpoolCleanupGroup();
    if (!group) {
        DWORD err = GetLastError();
        throw exception("CreateThreadpoolCleanupGroup() failed, error is %1%"_fmt % error_message(err));
    }
    return group;
}

void close_threadpool_cleanup_group(PTP_CLEANUP_GROUP ptpcg) noexcept
{
    CloseThreadpoolCleanupGroup(ptpcg);
}

void set_threadpool_callback_cleanup_group(PTP_CALLBACK_ENVIRON pcbe, PTP_CLEANUP_GROUP ptpcg, PTP_CLEANUP_GROUP_CANCEL_CALLBACK pfng) noexcept
{
#ifndef __MINGW32__
    SetThreadpoolCallbackCleanupGroup(pcbe, ptpcg, pfng);
#endif
}

void close_threadpool_cleanup_group_members(PTP_CLEANUP_GROUP ptpcg, bool fCancelPendingCallbacks, PVOID pvCleanupContext) noexcept
{
    CloseThreadpoolCleanupGroupMembers(ptpcg, fCancelPendingCallbacks, pvCleanupContext);
}

PTP_TIMER create_threadpool_timer(PTP_TIMER_CALLBACK pfnti, PVOID pv, PTP_CALLBACK_ENVIRON pcbe)
{
#ifndef __MINGW32__
    PTP_TIMER timer = CreateThreadpoolTimer(pfnti, pv, pcbe);
#else
    PTP_TIMER timer = CreateThreadpoolTimer(pfnti, pv, NULL);
#endif
    if (!timer) {
        DWORD err = GetLastError();
        throw exception("CreateThreadpoolTimer() failed, error is %1%"_fmt % error_message(err));
    }
    return timer;
}

void set_threadpool_timer(PTP_TIMER pti, PFILETIME pftDueTime, DWORD msPeriod, DWORD msWindowLength) noexcept
{
    SetThreadpoolTimer(pti, pftDueTime, msPeriod, msWindowLength);
}

void wait_for_threadpool_timer_callbacks(PTP_TIMER pti, bool fCancelPendingCallbacks) noexcept
{
    WaitForThreadpoolTimerCallbacks(pti, fCancelPendingCallbacks);
}

void close_threadpool_timer(PTP_TIMER pti) noexcept
{
     CloseThreadpoolTimer(pti);
}

}
