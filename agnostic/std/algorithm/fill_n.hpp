// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALGORITHM_FILL_N_HPP
#define AGNOSTIC_STD_ALGORITHM_FILL_N_HPP

#pragma once

namespace std {

template <class OutputIt, class Size, class T>
constexpr OutputIt fill_n(OutputIt first, Size count, const T& value)
{
    for (Size i = 0; i < count; i++) {
        *first++ = value;
    }
    return first;
}

}

#endif // AGNOSTIC_STD_ALGORITHM_FILL_N_HPP
