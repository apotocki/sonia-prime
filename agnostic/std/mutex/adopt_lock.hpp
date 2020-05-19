// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ADOPT_LOCK_HPP
#define AGNOSTIC_STD_ADOPT_LOCK_HPP

#pragma once

namespace std {

struct adopt_lock_t { explicit adopt_lock_t() = default; };

}

#endif // AGNOSTIC_STD_ADOPT_LOCK_HPP
