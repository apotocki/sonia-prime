// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_FLOAT_DENORM_STYLE

namespace std {

enum float_denorm_style {
    denorm_indeterminate = -1,
    denorm_absent = 0,
    denorm_present = 1
};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <limits>
#endif
