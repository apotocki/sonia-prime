// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_LOGIC_ERROR_HPP
#define AGNOSTIC_STD_LOGIC_ERROR_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_EXCEPTION
#   include "../exception/exception.hpp"
#endif

namespace std {

class logic_error : public exception
{
public:
    logic_error() noexcept = default;
};

}

#endif // AGNOSTIC_STD_LOGIC_ERROR_HPP
