//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SYS_LINUX_SIGMALS_API_HPP
#define SONIA_SYS_LINUX_SIGMALS_API_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <signal.h>
#include <time.h>

namespace sonia::linux {

void timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid);
void timer_delete(timer_t timerid) noexcept;
void timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);

}

#endif // SONIA_SYS_LINUX_SIGMALS_API_HPP
