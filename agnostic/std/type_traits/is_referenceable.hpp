// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#pragma once

#include "agnostic/std/type_traits/integral_constant.hpp"

#ifndef DO_NOT_USE_AGNOSTIC_IS_OBJECT
#   include "is_object.hpp"
#endif
#ifndef DO_NOT_USE_AGNOSTIC_IS_REFERENCE
#   include "is_reference.hpp"
#endif

namespace stdext {

using namespace std;

template <typename T> struct is_referenceable : bool_constant<is_object_v<T> || is_reference<T>> {};

template <typename RT, typename... Args> struct is_referenceable<RT(Args...)> : true_type {};
template <typename RT, typename... Args> struct is_referenceable<RT(Args..., ...)> : true_type {};

}
