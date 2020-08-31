// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_FROM_PRIMARY

#include "agnostic/std/type_traits/is_same.hpp"

namespace stdext {

template <class T>
concept is_from_primary = std::is_same_v<typename T::_from_primary, T>;

}

#endif
