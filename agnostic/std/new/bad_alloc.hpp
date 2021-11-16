// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_EXCEPTION
#   include "../exception/exception.hpp"
#endif

namespace std {

class bad_alloc : public exception
{
public:
    bad_alloc() noexcept = default;
    bad_alloc(bad_alloc const&) noexcept = default;
    bad_alloc& operator=(const bad_alloc& other) noexcept = default;

    const char* what() const noexcept override { return "bad_alloc"; }
};

}
