// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ALIGN_VAL
#   include "agnostic/std/new/align_val.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_NOTHROW
#   include "agnostic/std/new/nothrow.hpp"
#endif

namespace agnostic {

template <typename T>
concept FixedSizeAllocator =
    T::size_value > 0 &&
    std::is_same_v<typename T::value_type, char> &&
    requires(T & a, T const& b) {
        {a.allocate()} -> std::convertible_to<void*>;
        {a.allocate(std::nothrow)} -> std::convertible_to<void*>;
        {a.deallocate(std::declval<void*>())} -> std::same_as<void>;
        {b.is_equal(std::declval<T const&>())} -> std::same_as<bool>;
};

}
