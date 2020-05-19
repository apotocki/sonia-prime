// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_CONDITIONAL_HPP
#define AGNOSTIC_STD_CONDITIONAL_HPP

#pragma once

namespace std {

template <bool CV, typename T, typename F>
struct conditional { using type = T; };

template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

}

#endif // AGNOSTIC_STD_CONDITIONAL_HPP
