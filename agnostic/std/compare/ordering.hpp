// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ORDERING

namespace std {

class partial_ordering
{
    // less=-1, greater=1, equiv/unordered=0x00
    union {
        struct parts {
            signed char value_ : 6;
            signed char is_unordered_ : 2;
        } parts_;
        signed char full_value_;
    };
public:
    [[nodiscard]] constexpr explicit partial_ordering(const signed char val) noexcept
        : parts_{val, 0}
    {}

    [[nodiscard]] constexpr explicit partial_ordering(const signed char val, const signed char is_unordered) noexcept
        : parts_{val, is_unordered}
    {}

    static const partial_ordering less;
    static const partial_ordering equivalent;
    static const partial_ordering greater;
    static const partial_ordering unordered;

    
    [[nodiscard]] friend constexpr bool operator==(const partial_ordering&, const partial_ordering&) noexcept = default;

    [[nodiscard]] friend constexpr bool operator==(const partial_ordering val, nullptr_t) noexcept { return val.full_value_ == 0; }

    [[nodiscard]] friend constexpr bool operator==(nullptr_t, const partial_ordering val) noexcept { return 0 == val.full_value_; }


    [[nodiscard]] friend constexpr bool operator!=(const partial_ordering val, nullptr_t) noexcept { return val.full_value_ != 0; }

    [[nodiscard]] friend constexpr bool operator!=(nullptr_t, const partial_ordering val) noexcept { return 0 != val.full_value_; }


    [[nodiscard]] friend constexpr bool operator<(const partial_ordering val, nullptr_t) noexcept { return val.parts_.value_ == -1; }

    [[nodiscard]] friend constexpr bool operator>(const partial_ordering val, nullptr_t) noexcept { return val.parts_.value_ == 1; }

    [[nodiscard]] friend constexpr bool operator<=(const partial_ordering val, nullptr_t) noexcept { return val.parts_.value_ == -1 || val.full_value_ == 0; }

    [[nodiscard]] friend constexpr bool operator>=(const partial_ordering val, nullptr_t) noexcept { return val.parts_.value_ == 1|| val.full_value_ == 0; }

    [[nodiscard]] friend constexpr bool operator<(nullptr_t, const partial_ordering val) noexcept { return val.parts_.value_ == 1; }

    [[nodiscard]] friend constexpr bool operator>(nullptr_t, const partial_ordering val) noexcept { return val.parts_.value_ == -1; }

    [[nodiscard]] friend constexpr bool operator<=(nullptr_t, const partial_ordering val) noexcept { return val.parts_.value_ == 1 || val.full_value_ == 0; }

    [[nodiscard]] friend constexpr bool operator>=(nullptr_t, const partial_ordering val) noexcept { return val.parts_.value_ == -1 || val.full_value_ == 0; }


    [[nodiscard]] friend constexpr partial_ordering operator<=>(const partial_ordering val, nullptr_t) noexcept { return val; }

    [[nodiscard]] friend constexpr partial_ordering operator<=>(nullptr_t, const partial_ordering val) noexcept { return partial_ordering{-val.parts_.value_, val.parts_.is_unordered_}; }

private:

};

inline constexpr partial_ordering partial_ordering::less{-1};
inline constexpr partial_ordering partial_ordering::equivalent{0};
inline constexpr partial_ordering partial_ordering::greater{1};
inline constexpr partial_ordering partial_ordering::unordered{0, 1};


class weak_ordering
{
    // less=-1, greater=1, equiv=0x00
public:
    [[nodiscard]] constexpr explicit weak_ordering(const signed char val) noexcept
        : value_(val)
    {}

    static const weak_ordering less;
    static const weak_ordering equivalent;
    static const weak_ordering greater;

    constexpr operator partial_ordering() const noexcept
    {
        return partial_ordering{ value_ };
    }

    [[nodiscard]] friend constexpr bool operator==(const weak_ordering&, const weak_ordering&) noexcept = default;

    [[nodiscard]] friend constexpr bool operator==(const weak_ordering val, nullptr_t) noexcept { return val.value_ == 0; }

    [[nodiscard]] friend constexpr bool operator==(nullptr_t, const weak_ordering val) noexcept { return 0 == val.value_; }


    [[nodiscard]] friend constexpr bool operator!=(const weak_ordering val, nullptr_t) noexcept { return val.value_ != 0; }

    [[nodiscard]] friend constexpr bool operator!=(nullptr_t, const weak_ordering val) noexcept { return 0 != val.value_; }


    [[nodiscard]] friend constexpr bool operator<(const weak_ordering val, nullptr_t) noexcept { return val.value_ == -1; }

    [[nodiscard]] friend constexpr bool operator>(const weak_ordering val, nullptr_t) noexcept { return val.value_ == 1; }

    [[nodiscard]] friend constexpr bool operator<=(const weak_ordering val, nullptr_t) noexcept { return val.value_ <= 0; }

    [[nodiscard]] friend constexpr bool operator>=(const weak_ordering val, nullptr_t) noexcept { return val.value_ >= 0; }

    [[nodiscard]] friend constexpr bool operator<(nullptr_t, const weak_ordering val) noexcept { return val.value_ == 1; }

    [[nodiscard]] friend constexpr bool operator>(nullptr_t, const weak_ordering val) noexcept { return val.value_ == -1; }

    [[nodiscard]] friend constexpr bool operator<=(nullptr_t, const weak_ordering val) noexcept { return val.value_ >= 0; }

    [[nodiscard]] friend constexpr bool operator>=(nullptr_t, const weak_ordering val) noexcept { return val.value_ <= 0; }


    [[nodiscard]] friend constexpr weak_ordering operator<=>(const weak_ordering val, nullptr_t) noexcept { return val; }

    [[nodiscard]] friend constexpr weak_ordering operator<=>(nullptr_t, const weak_ordering val) noexcept { return weak_ordering{ -val.value_ }; }

private:
    signed char value_;
};

inline constexpr weak_ordering weak_ordering::less{ -1 };
inline constexpr weak_ordering weak_ordering::equivalent{ 0 };
inline constexpr weak_ordering weak_ordering::greater{ 1 };

class strong_ordering
{
    // less=-1, greater=1, equiv=0x00
public:
    [[nodiscard]] constexpr explicit strong_ordering(const signed char val) noexcept
        : value_(val)
    {}

    static const strong_ordering less;
    static const strong_ordering equal;
    static const strong_ordering equivalent;
    static const strong_ordering greater;

    constexpr operator partial_ordering() const noexcept
    {
        return partial_ordering{ value_ };
    }

    constexpr operator weak_ordering() const noexcept
    {
        return weak_ordering{ value_ };
    }

    [[nodiscard]] friend constexpr bool operator==(const strong_ordering&, const strong_ordering&) noexcept = default;

    [[nodiscard]] friend constexpr bool operator==(const strong_ordering val, nullptr_t) noexcept { return val.value_ == 0; }

    [[nodiscard]] friend constexpr bool operator==(nullptr_t, const strong_ordering val) noexcept { return 0 == val.value_; }


    [[nodiscard]] friend constexpr bool operator!=(const strong_ordering val, nullptr_t) noexcept { return val.value_ != 0; }

    [[nodiscard]] friend constexpr bool operator!=(nullptr_t, const strong_ordering val) noexcept { return 0 != val.value_; }


    [[nodiscard]] friend constexpr bool operator<(const strong_ordering val, nullptr_t) noexcept { return val.value_ == -1; }

    [[nodiscard]] friend constexpr bool operator>(const strong_ordering val, nullptr_t) noexcept { return val.value_ == 1; }

    [[nodiscard]] friend constexpr bool operator<=(const strong_ordering val, nullptr_t) noexcept { return val.value_ <= 0; }

    [[nodiscard]] friend constexpr bool operator>=(const strong_ordering val, nullptr_t) noexcept { return val.value_ >= 0; }

    [[nodiscard]] friend constexpr bool operator<(nullptr_t, const strong_ordering val) noexcept { return val.value_ == 1; }

    [[nodiscard]] friend constexpr bool operator>(nullptr_t, const strong_ordering val) noexcept { return val.value_ == -1; }

    [[nodiscard]] friend constexpr bool operator<=(nullptr_t, const strong_ordering val) noexcept { return val.value_ >= 0; }

    [[nodiscard]] friend constexpr bool operator>=(nullptr_t, const strong_ordering val) noexcept { return val.value_ <= 0; }


    [[nodiscard]] friend constexpr strong_ordering operator<=>(const strong_ordering val, nullptr_t) noexcept { return val; }

    [[nodiscard]] friend constexpr strong_ordering operator<=>(nullptr_t, const strong_ordering val) noexcept { return strong_ordering{ -val.value_ }; }

private:
    signed char value_;
};

inline constexpr strong_ordering strong_ordering::less{ -1 };
inline constexpr strong_ordering strong_ordering::equal{ 0 };
inline constexpr strong_ordering strong_ordering::equivalent{ 0 };
inline constexpr strong_ordering strong_ordering::greater{ 1 };

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <compare>
#endif
