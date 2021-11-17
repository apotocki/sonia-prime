// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "agnostic/std/utility/declval.hpp"

namespace stdext {

// returns result type of ternary operator
template <typename X, typename Y>
using cond_res = decltype(false ? std::declval<X(&)()>()() : std::declval<Y(&)()>()());

}
