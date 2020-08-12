// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_MUTEX_HPP
#define AGNOSTIC_STD_MUTEX_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ADOPT_LOCK
#   include "mutex/adopt_lock.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DEFER_LOCK
#   include "mutex/defer_lock.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_TRY_TO_LOCK
#   include "mutex/try_to_lock.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_LOCK_GUARD
#   include "mutex/lock_guard.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_LOCK_GUARD
#   include "mutex/lock_guard.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNIQUE_LOCK
#   include "mutex/unique_lock.hpp"
#endif

#endif // AGNOSTIC_STD_MUTEX_HPP
