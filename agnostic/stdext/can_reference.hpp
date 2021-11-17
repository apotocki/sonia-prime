// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace stdext {

template <typename T> using with_reference = T&;

template <typename T>
concept can_reference = requires { typename with_reference<T>; };

}
