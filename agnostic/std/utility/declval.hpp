// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_DECLVAL_HPP
#define AGNOSTIC_STD_DECLVAL_HPP

#pragma once

namespace std {

template <typename T>
add_rvalue_reference_t<T> declval() noexcept;

}

#endif // AGNOSTIC_STD_FORWARD_HPP
