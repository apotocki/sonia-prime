// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_EQUALITY

#include "agnostic/std/compare/ordering.hpp"

namespace std {

class weak_equality
{
public:
    [[nodiscard]] constexpr explicit weak_equality(char value) noexcept
        : value_(value)
    {}

    static const weak_equality equivalent;
    static const weak_equality nonequivalent;

    [[nodiscard]] friend constexpr bool operator==(const weak_equality&, const weak_equality&) noexcept = default;

    [[nodiscard]] friend constexpr bool operator==(const weak_equality val, nullptr_t) noexcept { return val.value_ == 0; }

    [[nodiscard]] friend constexpr bool operator==(nullptr_t, const weak_equality val) noexcept { return 0 == val.value_; }

    [[nodiscard]] friend constexpr bool operator!=(const weak_equality val, nullptr_t) noexcept { return val.value_ != 0; }

    [[nodiscard]] friend constexpr bool operator!=(nullptr_t, const weak_equality val) noexcept { return 0 != val.value_; }

    [[nodiscard]] friend constexpr weak_equality operator<=>(const weak_equality val, nullptr_t) noexcept { return val; }

    [[nodiscard]] friend constexpr weak_equality operator<=>(nullptr_t, const weak_equality val) noexcept { return val; }


private:
    char value_;
};

inline constexpr weak_equality weak_equality::equivalent{ 0 };
inline constexpr weak_equality weak_equality::nonequivalent{ 1 };

class strong_equality
{
public:
    [[nodiscard]] constexpr explicit strong_equality(char value) noexcept
        : value_(value)
    {}

    static const strong_equality equivalent;
    static const strong_equality equal;
    static const strong_equality nonequivalent;
    static const strong_equality nonequal;

    constexpr operator weak_equality() const noexcept
    {
        return weak_equality{value_};
    }

    [[nodiscard]] friend constexpr bool operator==(const strong_equality&, const strong_equality&) noexcept = default;

    [[nodiscard]] friend constexpr bool operator==(const strong_equality val, nullptr_t) noexcept { return val.value_ == 0; }

    [[nodiscard]] friend constexpr bool operator==(nullptr_t, const strong_equality val) noexcept { return 0 == val.value_; }

    [[nodiscard]] friend constexpr bool operator!=(const strong_equality val, nullptr_t) noexcept { return val.value_ != 0; }

    [[nodiscard]] friend constexpr bool operator!=(nullptr_t, const strong_equality val) noexcept { return 0 != val.value_; }

    [[nodiscard]] friend constexpr strong_equality operator<=>(const strong_equality val, nullptr_t) noexcept { return val; }

    [[nodiscard]] friend constexpr strong_equality operator<=>(nullptr_t, const strong_equality val) noexcept { return val; }


private:
    char value_;
};

inline constexpr strong_equality strong_equality::equivalent{ 0 };
inline constexpr strong_equality strong_equality::equal{ 0 };
inline constexpr strong_equality strong_equality::nonequivalent{ 1 };
inline constexpr strong_equality strong_equality::nonequal{ 1 };

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <compare>
#endif
