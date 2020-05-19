// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_PAIR_HPP
#define AGNOSTIC_STD_PAIR_HPP

#pragma once

namespace std {

template <typename T1, typename T2> struct pair
{
    using first_type = T1;
    using second_type = T2;

#if _HAS_CONDITIONAL_EXPLICIT
    // pair();
    template <
        typename UT1 = T1,
        typename UT2 = T2,
        enable_if_t<is_default_constructible_v<UT1> && is_default_constructible_v<UT2>, int> = 0
    >
    constexpr explicit(!is_implicitly_default_constructible_v<UT1> || !is_implicitly_default_constructible_v<UT2>)
        pair() noexcept(
            is_nothrow_default_constructible_v<UT1> && is_nothrow_default_constructible_v<UT2>
        ) : first(), second() {}

    // pair(T1 const& x, T2 const& y);
    template <
        typename UT1 = T1,
        typename UT2 = T2,
        enable_if_t<is_copy_constructible_v<UT1> && is_copy_constructible_v<UT2>, int> = 0
    >
    constexpr explicit(!is_convertible_v<UT1 const&, T1> || !is_convertible_v<UT2 const&, T2>)
        pair(T1 const& x, T2 const& y) noexcept(
            is_nothrow_copy_constructible_v<UT1> && is_nothrow_copy_constructible_v<UT2>
        ) : first(x), second(y) {}

    // pair(UT1 && x, UT2 & y);
    template <
        typename UT1,
        typename UT2,
        enable_if_t<is_constructible_v<first_type, UT1&&> && is_constructible_v<second_type, UT2&&>, int> = 0
    >
    constexpr explicit(!is_convertible_v<UT1&&, first_type> || !is_convertible_v<UT2&&, second_type>)
        pair(UT1 && x, UT2 && y) noexcept(
            is_nothrow_constructible_v<first_type, UT1&&> && is_nothrow_constructible_v<second_type, UT2&&>
        ) : first(forward<UT1>(x)), second(forward<UT2>(y)) {}

    // pair(pair<U1, U2> const& p)
    template <
        typename UT1,
        typename UT2,
        enable_if_t<is_constructible_v<first_type, UT1 const&> && is_constructible_v<second_type, UT2 const&>, int> = 0
    >
    constexpr explicit(!is_convertible_v<UT1 const&, first_type> || !is_convertible_v<UT2 const&, second_type>)
        pair(pair<UT1, UT2> const& p) noexcept(
            is_nothrow_constructible_v<first_type, UT1 const&> && is_nothrow_constructible_v<second_type, UT2 const&>
        ) : first(p.first), second(p.second) {}

    // pair(pair<U1, U2> && p)
    template <
        typename UT1,
        typename UT2,
        enable_if_t<is_constructible_v<first_type, UT1&&> && is_constructible_v<second_type, UT2&&>, int> = 0
    >
    constexpr explicit(!is_convertible_v<UT1&&, first_type> || !is_convertible_v<UT2&&, second_type>)
        pair(pair<UT1, UT2> && p) noexcept(
            is_nothrow_constructible_v<first_type, UT1&&> && is_nothrow_constructible_v<second_type, UT2&&>
        ) : first(move(p.first)), second(move(p.second)) {}

#else
    // pair();
    template <
        typename UT1 = T1,
        typename UT2 = T2,
        enable_if_t<
            (is_default_constructible_v<UT1> && is_default_constructible_v<UT2>) &&
            (!is_implicitly_default_constructible_v<UT1> || !is_implicitly_default_constructible_v<UT2>)
        , int> = 0
    >
    constexpr explicit pair() noexcept(
            is_nothrow_default_constructible_v<UT1> && is_nothrow_default_constructible_v<UT2>
        ) : first(), second() {}

    template <
        typename UT1 = T1,
        typename UT2 = T2,
        enable_if_t<
            is_default_constructible_v<UT1> && is_default_constructible_v<UT2> &&
            is_implicitly_default_constructible_v<UT1> && is_implicitly_default_constructible_v<UT2>
        , int> = 0
    >
    constexpr pair() noexcept(
            is_nothrow_default_constructible_v<UT1> && is_nothrow_default_constructible_v<UT2>
        ) : first(), second() {}

    // pair(T1 const& x, T2 const& y);
    template <
        typename UT1 = T1,
        typename UT2 = T2,
        enable_if_t<
            is_copy_constructible_v<UT1 > && is_copy_constructible_v<UT2> &&
            (!is_convertible_v<UT1 const&, T1> || !is_convertible_v<UT2 const&, T2>)
        , int> = 0
    >
    constexpr explicit pair(T1 const& x, T2 const& y) noexcept(
            is_nothrow_copy_constructible_v<UT1> && is_nothrow_copy_constructible_v<UT2>
        ) : first(x), second(y) {}

    template <
        typename UT1 = T1,
        typename UT2 = T2,
        enable_if_t<
            is_copy_constructible_v<UT1 > && is_copy_constructible_v<UT2> &&
            is_convertible_v<UT1 const&, T1> && is_convertible_v<UT2 const&, T2>
        , int> = 0
    >
    constexpr pair(T1 const& x, T2 const& y) noexcept(
            is_nothrow_copy_constructible_v<UT1> && is_nothrow_copy_constructible_v<UT2>
        ) : first(x), second(y) {}

    // pair(UT1 && x, UT2 & y);
    template <
        typename UT1,
        typename UT2,
        enable_if_t<
            is_constructible_v<first_type, UT1&&> && is_constructible_v<second_type, UT2&&> &&
            (!is_convertible_v<UT1&&, first_type> || !is_convertible_v<UT2&&, second_type>)
        , int> = 0
    >
    constexpr explicit pair(UT1&& x, UT2&& y) noexcept(
            is_nothrow_constructible_v<first_type, UT1&&> && is_nothrow_constructible_v<second_type, UT2&&>
        ) : first(forward<UT1>(x)), second(forward<UT2>(y)) {}


    template <
        typename UT1,
        typename UT2,
        enable_if_t<
            is_constructible_v<first_type, UT1&&> && is_constructible_v<second_type, UT2&&> &&
            is_convertible_v<UT1&&, first_type> && is_convertible_v<UT2&&, second_type>
        , int> = 0
    >
    constexpr pair(UT1&& x, UT2&& y) noexcept(
            is_nothrow_constructible_v<first_type, UT1&&> && is_nothrow_constructible_v<second_type, UT2&&>
        ) : first(forward<UT1>(x)), second(forward<UT2>(y)) {}

    // pair(pair<U1, U2> const& p)
    template <
        typename UT1,
        typename UT2,
        enable_if_t<
            is_constructible_v<first_type, UT1 const&> && is_constructible_v<second_type, UT2 const&> &&
            (!is_convertible_v<UT1 const&, first_type> || !is_convertible_v<UT2 const&, second_type>)
        , int> = 0
    >
    constexpr explicit pair(pair<UT1, UT2> const& p) noexcept(
            is_nothrow_constructible_v<first_type, UT1 const&> && is_nothrow_constructible_v<second_type, UT2 const&>
        ) : first(p.first), second(p.second) {}

    template <
        typename UT1,
        typename UT2,
        enable_if_t<
            is_constructible_v<first_type, UT1 const&>&& is_constructible_v<second_type, UT2 const&> &&
            is_convertible_v<UT1 const&, first_type> && is_convertible_v<UT2 const&, second_type>
        , int> = 0
    >
    constexpr pair(pair<UT1, UT2> const& p) noexcept(
            is_nothrow_constructible_v<first_type, UT1 const&> && is_nothrow_constructible_v<second_type, UT2 const&>
        ) : first(p.first), second(p.second) {}

    // pair(pair<U1, U2> && p)
    template <
        typename UT1,
        typename UT2,
        enable_if_t<
            is_constructible_v<first_type, UT1&&> && is_constructible_v<second_type, UT2&&> &&
            (!is_convertible_v<UT1&&, first_type> || !is_convertible_v<UT2&&, second_type>)
        , int> = 0
    >
    constexpr explicit pair(pair<UT1, UT2>&& p) noexcept(
            is_nothrow_constructible_v<first_type, UT1&&> && is_nothrow_constructible_v<second_type, UT2&&>
        ) : first(move(p.first)), second(move(p.second)) {}

    template <
        typename UT1,
        typename UT2,
        enable_if_t<
            is_constructible_v<first_type, UT1&&> && is_constructible_v<second_type, UT2&&> &&
            is_convertible_v<UT1&&, first_type> && is_convertible_v<UT2&&, second_type>
        , int> = 0
    >
    constexpr pair(pair<UT1, UT2>&& p) noexcept(
            is_nothrow_constructible_v<first_type, UT1&&> && is_nothrow_constructible_v<second_type, UT2&&>
        ) : first(move(p.first)), second(move(p.second)) {}
#endif

    //template <typename... Args1, typename... Args2>
    //constexpr pair(std::piecewise_construct_t,
    //    std::tuple<Args1...> first_args,
    //    std::tuple<Args2...> second_args);

    pair(pair const& p) = default;
    
    pair(pair && p) = default;

    ~pair() = default;

    constexpr pair& operator=(pair const& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }

    template <typename U1, typename U2>
    constexpr pair& operator=(pair<U1, U2> const& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }

    constexpr pair& operator=(pair&& other) noexcept(is_nothrow_move_assignable_v<T1> && is_nothrow_move_assignable_v<T2>)
    {
        first = std::move(other.first);
        second = std::move(other.second);
        return *this;
    }

    template <typename U1, typename U2>
    constexpr pair& operator=(pair<U1, U2> && other) noexcept(is_nothrow_move_assignable_v<T1>&& is_nothrow_move_assignable_v<T2>)
    {
        first = std::move(other.first);
        second = std::move(other.second);
        return *this;
    }

    T1 first;
    T2 second;

};

template <typename T1, typename T2>
pair(T1, T2)->pair<T1, T2>;

}

#endif // AGNOSTIC_STD_PAIR_HPP
