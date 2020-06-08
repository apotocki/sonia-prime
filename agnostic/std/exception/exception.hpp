// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace std {

class exception
{
public:
    exception() noexcept = default;
    exception(exception const&) noexcept = default;
    virtual ~exception() = default;
    exception& operator=(const exception& other) noexcept = default;

    virtual const char* what() const noexcept = 0;
};

}
