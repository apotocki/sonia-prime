// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace agnostic {

struct uninitialized_t { explicit uninitialized_t() = default; };

static const uninitialized_t uninitialized{  };

}
