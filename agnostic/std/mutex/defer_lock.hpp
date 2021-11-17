// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_DEFER_LOCK_HPP
#define AGNOSTIC_STD_DEFER_LOCK_HPP

#pragma once

namespace std {

struct defer_lock_t { explicit defer_lock_t() = default; };

}

#endif // AGNOSTIC_STD_DEFER_LOCK_HPP
