// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_UTILITY_CEILING_HPP
#define AGNOSTIC_UTILITY_CEILING_HPP

#pragma once

namespace agnostic {

template <size_t val, size_t acc>
constexpr size_t ceiling_v = ((val + acc - 1) / acc) * acc;

}

#endif // AGNOSTIC_UTILITY_CEILING_HPP
