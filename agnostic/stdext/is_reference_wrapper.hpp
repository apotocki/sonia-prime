// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#pragma once

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T>  class reference_wrapper;

}

namespace stdext {

using namespace std;

template <typename T>
struct is_reference_wrapper : false_type {};

template <typename U>
struct is_reference_wrapper<reference_wrapper<U>> : true_type {};

}
