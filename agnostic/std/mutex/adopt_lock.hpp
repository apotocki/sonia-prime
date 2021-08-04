// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ADOPT_LOCK

namespace std {

struct adopt_lock_t { explicit adopt_lock_t() = default; };

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <mutex>
#endif
