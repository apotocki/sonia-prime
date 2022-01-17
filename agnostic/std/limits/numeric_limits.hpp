// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_NUMERIC_LIMITS

#include "float_round_style.hpp"
#include "float_denorm_style.hpp"

namespace std::limits_detail {

struct num_base
{
    static constexpr float_denorm_style has_denorm = denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool is_bounded = false;
    static constexpr bool is_exact = false;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_integer = false;
    static constexpr bool is_modulo = false;
    static constexpr bool is_signed = false;
    static constexpr bool is_specialized = false;
    static constexpr bool tinyness_before = false;
    static constexpr bool traps = false;
    static constexpr float_round_style round_style = round_toward_zero;
    static constexpr int digits = 0;
    static constexpr int digits10 = 0;
    static constexpr int max_digits10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int radix = 0;
};

// base for integer types
struct num_int_base : num_base
{
    static constexpr bool is_bounded = true;
    static constexpr bool is_exact = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_specialized = true;
    static constexpr int radix = 2;
};

// base for floating-point types
struct num_float_base : num_base
{
    static constexpr float_denorm_style has_denorm = denorm_present;
    static constexpr bool has_infinity = true;
    static constexpr bool has_quiet_NaN = true;
    static constexpr bool has_signaling_NaN = true;
    static constexpr bool is_bounded = true;
    static constexpr bool is_iec559 = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_specialized = true;
    static constexpr float_round_style round_style = round_to_nearest;
    static constexpr int radix = FLT_RADIX;
};

}

namespace std {

template <typename T>
class numeric_limits : public limits_detail::num_base
{
public:
    [[nodiscard]] static constexpr T(min)() noexcept { return T(); }
    [[nodiscard]] static constexpr T(max)() noexcept { return T(); }
    [[nodiscard]] static constexpr T lowest() noexcept { return T(); }
    [[nodiscard]] static constexpr T epsilon() noexcept { return T(); }
    [[nodiscard]] static constexpr T round_error() noexcept { return T(); }
    [[nodiscard]] static constexpr T denorm_min() noexcept { return T(); }
    [[nodiscard]] static constexpr T infinity() noexcept { return T(); }
    [[nodiscard]] static constexpr T quiet_NaN() noexcept { return T(); }
    [[nodiscard]] static constexpr T signaling_NaN() noexcept { return T(); }
};

template <typename T> class numeric_limits<const T> : public numeric_limits<T> {};
template <typename T> class numeric_limits<volatile T> : public numeric_limits<T> {};
template <typename T> class numeric_limits<const volatile T> : public numeric_limits<T> {};

template <>
class numeric_limits<bool> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr bool(min)() noexcept { return false; }
    [[nodiscard]] static constexpr bool(max)() noexcept { return true; }
    [[nodiscard]] static constexpr bool lowest() noexcept { return false; }
    [[nodiscard]] static constexpr bool epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr bool round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr bool denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr bool infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr bool quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr bool signaling_NaN() noexcept { return 0; }

    static constexpr int digits = 1;
};

template <>
class numeric_limits<char> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr char(min)() noexcept { return CHAR_MIN; }
    [[nodiscard]] static constexpr char(max)() noexcept { return CHAR_MAX; }
    [[nodiscard]] static constexpr char lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr char epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr char denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr char infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char signaling_NaN() noexcept { return 0; }

    static constexpr bool is_signed = CHAR_MIN != 0;
    static constexpr bool is_modulo = CHAR_MIN == 0;
    static constexpr int digits = 8 - (CHAR_MIN != 0);
    static constexpr int digits10 = 2;
};

template <>
class numeric_limits<signed char> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr signed char(min)() noexcept { return SCHAR_MIN; }
    [[nodiscard]] static constexpr signed char(max)() noexcept { return SCHAR_MAX; }
    [[nodiscard]] static constexpr signed char lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr signed char epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char signaling_NaN() noexcept { return 0; }

    static constexpr bool is_signed = true;
    static constexpr int digits = 7;
    static constexpr int digits10 = 2;
};

template <>
class numeric_limits<unsigned char> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr unsigned char(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char(max)() noexcept { return UCHAR_MAX; }
    [[nodiscard]] static constexpr unsigned char lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr unsigned char epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 8;
    static constexpr int digits10 = 2;
};

#ifdef __cpp_char8_t

template <>
class numeric_limits<char8_t> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr char8_t(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t(max)() noexcept { return UCHAR_MAX; }
    [[nodiscard]] static constexpr char8_t lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr char8_t epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 8;
    static constexpr int digits10 = 2;
};

#endif

template <>
class numeric_limits<char16_t> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr char16_t(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t(max)() noexcept { return USHRT_MAX; }
    [[nodiscard]] static constexpr char16_t lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr char16_t epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 16;
    static constexpr int digits10 = 4;
};

template <>
class numeric_limits<char32_t> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr char32_t(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t(max)() noexcept { return UINT_MAX; }
    [[nodiscard]] static constexpr char32_t lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr char32_t epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 32;
    static constexpr int digits10 = 9;
};

#if defined(_NATIVE_WCHAR_T_DEFINED) && defined(WCHAR_MIN) && defined(WCHAR_MAX)

template <>
class numeric_limits<wchar_t> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr wchar_t(min)() noexcept { return WCHAR_MIN; }
    [[nodiscard]] static constexpr wchar_t(max)() noexcept { return WCHAR_MAX; }
    [[nodiscard]] static constexpr wchar_t lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr wchar_t epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 16;
    static constexpr int digits10 = 4;
};
#endif

template <>
class numeric_limits<short> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr short(min)() noexcept { return SHRT_MIN; }
    [[nodiscard]] static constexpr short(max)() noexcept { return SHRT_MAX; }
    [[nodiscard]] static constexpr short lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr short epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr short round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr short denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr short infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr short quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr short signaling_NaN() noexcept { return 0; }

    static constexpr bool is_signed = true;
    static constexpr int digits = 15;
    static constexpr int digits10 = 4;
};

template <>
class numeric_limits<int> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr int(min)() noexcept { return INT_MIN; }
    [[nodiscard]] static constexpr int(max)() noexcept { return INT_MAX; }
    [[nodiscard]] static constexpr int lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr int epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr int round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr int denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr int infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr int quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr int signaling_NaN() noexcept { return 0; }

    static constexpr bool is_signed = true;
    static constexpr int digits = 31;
    static constexpr int digits10 = 9;
};

template <>
class numeric_limits<long> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr long(min)() noexcept { return LONG_MIN; }
    [[nodiscard]] static constexpr long(max)() noexcept { return LONG_MAX; }
    [[nodiscard]] static constexpr long lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr long epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr long round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr long denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr long infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr long quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr long signaling_NaN() noexcept { return 0; }

    static_assert(sizeof(int) == sizeof(long), "LLP64 assumption");
    static constexpr bool is_signed = true;
    static constexpr int digits = 31;
    static constexpr int digits10 = 9;
};

template <>
class numeric_limits<long long> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr long long(min)() noexcept { return LLONG_MIN; }
    [[nodiscard]] static constexpr long long(max)() noexcept { return LLONG_MAX; }
    [[nodiscard]] static constexpr long long lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr long long epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr long long round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr long long denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr long long infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr long long quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr long long signaling_NaN() noexcept { return 0; }

    static constexpr bool is_signed = true;
    static constexpr int digits = 63;
    static constexpr int digits10 = 18;
};

template <>
class numeric_limits<unsigned short> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr unsigned short(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short(max)() noexcept { return USHRT_MAX; }
    [[nodiscard]] static constexpr unsigned short lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr unsigned short epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 16;
    static constexpr int digits10 = 4;
};

template <>
class numeric_limits<unsigned int> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr unsigned int(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int(max)() noexcept { return UINT_MAX; }
    [[nodiscard]] static constexpr unsigned int lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr unsigned int epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 32;
    static constexpr int digits10 = 9;
};

template <>
class numeric_limits<unsigned long> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr unsigned long(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long(max)() noexcept { return ULONG_MAX; }
    [[nodiscard]] static constexpr unsigned long lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr unsigned long epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long signaling_NaN() noexcept { return 0; }

    static_assert(sizeof(unsigned int) == sizeof(unsigned long), "LLP64 assumption");
    static constexpr bool is_modulo = true;
    static constexpr int digits = 32;
    static constexpr int digits10 = 9;
};

template <>
class numeric_limits<unsigned long long> : public limits_detail::num_int_base
{
public:
    [[nodiscard]] static constexpr unsigned long long(min)() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long(max)() noexcept { return ULLONG_MAX; }
    [[nodiscard]] static constexpr unsigned long long lowest() noexcept { return (min)(); }
    [[nodiscard]] static constexpr unsigned long long epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long round_error() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long denorm_min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long quiet_NaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long signaling_NaN() noexcept { return 0; }

    static constexpr bool is_modulo = true;
    static constexpr int digits = 64;
    static constexpr int digits10 = 19;
};

#ifndef FLT_TRUE_MIN
#   define FLT_TRUE_MIN 1.401298464e-45F
#endif

template <>
class numeric_limits<float> : public limits_detail::num_float_base
{
public:
    [[nodiscard]] static constexpr float(min)() noexcept { return FLT_MIN; }
    [[nodiscard]] static constexpr float(max)() noexcept { return FLT_MAX; }
    [[nodiscard]] static constexpr float lowest() noexcept { return -(max)(); }
    [[nodiscard]] static constexpr float epsilon() noexcept { return FLT_EPSILON; }
    [[nodiscard]] static constexpr float round_error() noexcept { return 0.5F; }
    [[nodiscard]] static constexpr float denorm_min() noexcept { return FLT_TRUE_MIN; }
    [[nodiscard]] static constexpr float infinity() noexcept { return __builtin_huge_valf(); }
    [[nodiscard]] static constexpr float quiet_NaN() noexcept { return __builtin_nanf("0"); }
    [[nodiscard]] static constexpr float signaling_NaN() noexcept { return __builtin_nansf("1"); }

    static constexpr int digits = FLT_MANT_DIG;
    static constexpr int digits10 = FLT_DIG;
    static constexpr int max_digits10 = 9;
    static constexpr int max_exponent = FLT_MAX_EXP;
    static constexpr int max_exponent10 = FLT_MAX_10_EXP;
    static constexpr int min_exponent = FLT_MIN_EXP;
    static constexpr int min_exponent10 = FLT_MIN_10_EXP;
};

#ifndef DBL_TRUE_MIN
#   define DBL_TRUE_MIN 4.9406564584124654e-324
#endif

template <>
class numeric_limits<double> : public limits_detail::num_float_base
{
public:
    [[nodiscard]] static constexpr double(min)() noexcept { return DBL_MIN; }
    [[nodiscard]] static constexpr double(max)() noexcept { return DBL_MAX; }
    [[nodiscard]] static constexpr double lowest() noexcept { return -(max)(); }
    [[nodiscard]] static constexpr double epsilon() noexcept { return DBL_EPSILON; }
    [[nodiscard]] static constexpr double round_error() noexcept { return 0.5; }
    [[nodiscard]] static constexpr double denorm_min() noexcept { return DBL_TRUE_MIN; }
    [[nodiscard]] static constexpr double infinity() noexcept { return __builtin_huge_val(); }
    [[nodiscard]] static constexpr double quiet_NaN() noexcept { return __builtin_nan("0"); }
    [[nodiscard]] static constexpr double signaling_NaN() noexcept { return __builtin_nans("1"); }

    static constexpr int digits = DBL_MANT_DIG;
    static constexpr int digits10 = DBL_DIG;
    static constexpr int max_digits10 = 17;
    static constexpr int max_exponent = DBL_MAX_EXP;
    static constexpr int max_exponent10 = DBL_MAX_10_EXP;
    static constexpr int min_exponent = DBL_MIN_EXP;
    static constexpr int min_exponent10 = DBL_MIN_10_EXP;
};

#ifndef LDBL_TRUE_MIN
#   define LDBL_TRUE_MIN DBL_TRUE_MIN
#endif

template <>
class numeric_limits<long double> : public limits_detail::num_float_base
{
public:
    [[nodiscard]] static constexpr long double(min)() noexcept { return LDBL_MIN; }
    [[nodiscard]] static constexpr long double(max)() noexcept { return LDBL_MAX; }
    [[nodiscard]] static constexpr long double lowest() noexcept { return -(max)(); }
    [[nodiscard]] static constexpr long double epsilon() noexcept { return LDBL_EPSILON; }
    [[nodiscard]] static constexpr long double round_error() noexcept { return 0.5L; }
    [[nodiscard]] static constexpr long double denorm_min() noexcept { return LDBL_TRUE_MIN; }
    [[nodiscard]] static constexpr long double infinity() noexcept { return __builtin_huge_val(); }
    [[nodiscard]] static constexpr long double quiet_NaN() noexcept { return __builtin_nan("0"); }
    [[nodiscard]] static constexpr long double signaling_NaN() noexcept { return __builtin_nans("1"); }

    static constexpr int digits = LDBL_MANT_DIG;
    static constexpr int digits10 = LDBL_DIG;
    static constexpr int max_digits10 = 17;
    static constexpr int max_exponent = LDBL_MAX_EXP;
    static constexpr int max_exponent10 = LDBL_MAX_10_EXP;
    static constexpr int min_exponent = LDBL_MIN_EXP;
    static constexpr int min_exponent10 = LDBL_MIN_10_EXP;
};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <limits>
#endif