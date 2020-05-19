// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_OUT_OF_RANGE_HPP
#define AGNOSTIC_STD_OUT_OF_RANGE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_LOGIC_ERROR
#   include "logic_error.hpp"
#endif

namespace std {

class out_of_range : public logic_error
{
public:
    out_of_range() noexcept = default;
};

}

#endif // AGNOSTIC_STD_OUT_OF_RANGE_HPP
