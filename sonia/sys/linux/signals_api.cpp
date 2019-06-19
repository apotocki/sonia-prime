//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "signals_api.hpp"

#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia::linux {

void timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid)
{
    int r = ::timer_create(clockid, sevp, timerid);
    if (r != 0) {
        int err = errno;
        throw exception("timer_create() failed, error is %1%"_fmt % strerror(err));
    }
}

void timer_delete(timer_t timerid) noexcept
{
    int r = ::timer_delete(timerid);
    if (r != 0) {
        int err = errno;
        GLOBAL_LOG_ERROR() << "timer_delete() filed, error is " << strerror(err);
    }
}

void timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec *old_value)
{
    int r = ::timer_settime(timerid, flags, new_value, old_value);
    if (r != 0) {
        int err = errno;
        throw exception("timer_settime() failed, error is %1%"_fmt % strerror(err));
    }
}

}
