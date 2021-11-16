// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace agnostic {

struct in_place_capacity_tag{};

template <size_t SzV> struct in_place_capacity
{
    using tag = in_place_capacity_tag;
    static constexpr size_t value = SzV;
};

}
