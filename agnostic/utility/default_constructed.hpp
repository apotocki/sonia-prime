// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace agnostic {

struct default_constructed_t { explicit default_constructed_t() = default; };

static const default_constructed_t default_constructed{};

}
