// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace std {

struct nothrow_t { explicit nothrow_t() = default; };

extern const nothrow_t nothrow;

}
