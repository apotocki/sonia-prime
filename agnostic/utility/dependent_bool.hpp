// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace agnostic {

template <typename, bool value> constexpr bool dependent_bool = value;

template <typename> constexpr bool dependent_true = true;

template <typename> constexpr bool dependent_false = false;

}
