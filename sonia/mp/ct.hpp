/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <concepts>

#include "arithmetic.hpp"

namespace sonia::mpct
{

// no, we want ADL
// template <typename T, T... args> using limbs = std::integer_sequence<T, args...>;


template <typename T, T... args> struct limbs
{
    using type = limbs;
    static T data[sizeof...(args)];
};

template <typename T, T... args> T limbs<T, args...>::data[sizeof...(args)] = { args ... };

template <typename LimbsT> struct size_method : size_method<typename LimbsT::type> {};
template <typename LimbsT> constexpr size_t size = size_method<LimbsT>::value;
template <typename T, T... args> struct size_method<limbs<T, args...>> { static constexpr size_t value = sizeof ...(args); };

// ////////////////////////////////// front
template <typename LimbsT> struct front_method : front_method <typename LimbsT::type> {};

template <typename LimbsT> constexpr auto front = front_method<LimbsT>::value;

template <typename LimbsT> struct front_method<const LimbsT> : front_method<LimbsT> {};

template <typename T, T h, T... args>
struct front_method<limbs<T, h, args...>> : std::integral_constant<T, h> { };

template <typename T> struct front_method<limbs<T>> : std::integral_constant<T, T{}> {};

// ////////////////////////////////// pop_front
template <typename LimbsT> struct pop_front_method : pop_front_method<typename LimbsT::type> {};

template <typename LimbsT> using pop_front = typename pop_front_method<LimbsT>::type;

template <typename T, T h, T... args>
struct pop_front_method<limbs<T, h, args...>> { using type = limbs<T, args...>; };

template <typename T>
struct pop_front_method<limbs<T>> { using type = limbs<T>; };

// ////////////////////////////////// back
template <typename LimbsT> struct back_method : back_method<typename LimbsT::type> {};

template <typename LimbsT> constexpr auto back = back_method<LimbsT>::value;

template <typename T, T... args>
struct back_method<limbs<T, args...>>
{
    static constexpr T value = (T{ args }, ...);
    using type = std::integral_constant<T, value>;
};

// ////////////////////////////////// slice_limbs
template <typename LimbsT, size_t LimbCount> struct slice_limbs_method : slice_limbs_method<typename LimbsT::type, LimbCount> {};

template <typename LimbsT, size_t LimbCount> using slice_limbs = typename slice_limbs_method<LimbsT, LimbCount>::type;

namespace detail {
    template <typename LimbT, LimbT ...args, size_t ... LimbIs>
    auto slice_limbs_impl(std::index_sequence<LimbIs...>)
    {
        constexpr std::array<LimbT, sizeof ...(args)> ls{ args ... };
        using type = limbs<LimbT, ls[LimbIs] ...>;
        return type{};
    }
}

template <typename T, T ... args, size_t LimbCount> struct slice_limbs_method<limbs<T, args...>, LimbCount>
{
    using type = decltype(detail::slice_limbs_impl<T, args...>(std::make_index_sequence<LimbCount>()));
};

// ////////////////////////////////// pop_back
template <typename LimbsT> struct pop_back_method : pop_back_method<typename LimbsT::type> {};

template <typename LimbsT> using pop_back = typename pop_back_method<LimbsT>::type;

template <typename T, T ... args> struct pop_back_method<limbs<T, args...>>
{
    using type = decltype(detail::slice_limbs_impl<T, args...>(std::make_index_sequence<sizeof ...(args) - 1>()));
};

// ////////////////////////////////// at
template <typename LimbsT, size_t I> struct at_method : at_method<typename LimbsT::type, I> {};
template <typename T, T ... args, size_t I>
requires(sizeof...(args) > I) struct at_method<limbs<T, args...>, I>
{
    static constexpr T value = std::array<T, sizeof ...(args)>{ args ... }[I];
    using type = std::integral_constant<T, value>;
};
template <typename LimbsT, size_t I> constexpr auto at = at_method<LimbsT, I>::value;

// ////////////////////////////////// concat
template <typename LimbsLT, typename LimbsRT> struct concat_method : concat_method<typename LimbsLT::type, typename LimbsRT::type> {};
template <typename LimbsLT, typename LimbsRT> using concat = typename concat_method<LimbsLT, LimbsRT>::type;
template <std::integral T, T... largs, T... rargs>
struct concat_method<limbs<T, largs...>, limbs<T, rargs...>>
{
    using type = limbs<T, largs..., rargs...>;
};

// ////////////////////////////////// BUILD
namespace detail {
    template <typename T, T V, size_t ...Is> auto build_method_impl(std::index_sequence<Is...>)
    {
        return limbs<T, (V + Is * 0)...>{};
    };
}

template <typename T, size_t C, T V = 0> struct build_method 
{
    using type = decltype(detail::build_method_impl<T, V>(std::make_index_sequence<C>()));
};

template <typename T, size_t C, T V> using build = typename build_method<T, C, V>::type;

// ////////////////////////////////// push_back
template <typename LimbsT, auto V> struct push_back_method : push_back_method<typename LimbsT::type, V> {};

template <typename LimbsT, auto V> using push_back = typename push_back_method<LimbsT, V>::type;

template <typename T, T... args, T V>
struct push_back_method<limbs<T, args...>, V> { using type = limbs<T, args..., V>; };

// ////////////////////////////////// push_front
template <typename LimbsT, auto V> struct push_front_method : push_front_method<typename LimbsT::type, V> {};

template <typename LimbsT, auto V> using push_front = typename push_front_method<LimbsT, V>::type;

template <typename T, T... args, T V>
struct push_front_method<limbs<T, args...>, V> { using type = limbs<T, V, args...>; };

// ////////////////////////////////// fold
template <size_t IDX, typename S, template <size_t, typename> class F>
struct fold_index_sequence : fold_index_sequence<IDX - 1, typename F<IDX, S>::type, F> {};

template <typename S, template <size_t, typename> class F>
struct fold_index_sequence<0, S, F> { using type = S; };

// ////////////////////////////////// norm (removing leading 0 limbs; append 0 to empty limbs)
namespace detail {
    template <typename LimbT, LimbT ...args, size_t ... LimbIs>
    constexpr size_t norm_size_impl(std::index_sequence<LimbIs...>)
    {
        constexpr std::array<LimbT, sizeof ...(args)> ls{ args ... };
        size_t sz = sizeof ...(args) - 1;
        for (; sz && !ls[sz]; --sz);
        return sz + 1;
    }
}

template <typename LimbsT> struct norm_method : norm_method<typename LimbsT::type> {};

template <typename T> struct norm_method<limbs<T>> { using type = limbs<T, 0>; };
template <typename T, T ... args> struct norm_method<limbs<T, args...>>
{
    static constexpr size_t sz = detail::norm_size_impl<T, args...>(std::make_index_sequence<sizeof...(args)>());
    using type = slice_limbs<limbs<T, args...>, sz>;
};

template <typename LimbsT> using norm = typename norm_method<LimbsT>::type;

// ////////////////////////////////// resize (slice limbs or append leading zeros)
template <typename LimbsT, size_t SZ> struct resize_method : resize_method<typename LimbsT::type, SZ> {};
template <typename LimbsT, size_t SZ> using resize = typename resize_method<LimbsT, SZ>::type;
template <typename T, T... args, size_t SZ> struct resize_method<limbs<T, args...>, SZ>
    : std::conditional_t<(sizeof ...(args) > SZ), slice_limbs_method<limbs<T, args...>, SZ>, concat_method<limbs<T, args...>, build_method<T, SZ - sizeof...(args)>>>::type
{};

// ////////////////////////////////// slice (given humber of bits)
template <typename LimbsT, size_t BitCount> struct slice_method : slice_method<typename LimbsT::type, BitCount> {};
template <typename LimbsT, size_t BitCount> using slice = typename slice_method<LimbsT, BitCount>::type;

namespace detail {
    template <typename T, size_t BitCount, T ... args> struct slice_impl
    {
        using ls_t = limbs<T, args...>;

        static constexpr auto bits = sizeof(T) * CHAR_BIT;
        static constexpr auto maskbits = BitCount % bits;

        static constexpr T last_limb = std::conditional_t<!maskbits, std::integral_constant<T, 0>, at_method<ls_t, BitCount / bits>>::type::value;
        static constexpr T mask = maskbits ? (T{ 1 } << maskbits) - 1 : -1;
        
        using base_slice_t = slice_limbs<ls_t, BitCount / bits>;
        using type = norm<std::conditional_t<!maskbits, base_slice_t, push_back_method<base_slice_t, last_limb & mask>>>;
    };
}

template <typename T, T... args, size_t BitCount>
struct slice_method<limbs<T, args...>, BitCount>
{
    static constexpr auto bits = sizeof(T) * CHAR_BIT;
    using type = typename std::conditional_t<bits * sizeof...(args) <= BitCount, limbs<T, args...>, detail::slice_impl<T, BitCount, args...>>::type;
};

// ////////////////////////////////// SHIFT LEFT
namespace detail {
    template <size_t ShiftV, typename ShiftedLimbsT, typename LimbsT> struct shift_left_builder
        : shift_left_builder<ShiftV, typename ShiftedLimbsT::type, typename LimbsT::type>
    {};

    template <size_t ShiftV, std::integral T, T... sargs, T... args>
    struct shift_left_builder<ShiftV, limbs<T, sargs...>, limbs<T, args...>>
    {
        using type = norm<limbs<T, ((args << ShiftV) | (sargs >> (sizeof(T) * CHAR_BIT - ShiftV))) ...>>;
    };
}

template <typename LimbsLT, auto V> struct shift_left_method : shift_left_method<typename LimbsLT::type, V>{};
template <typename LimbsLT, auto V> using shift_left = typename shift_left_method<LimbsLT, V>::type;

template <std::integral T, T... args, auto V>
struct shift_left_method<limbs<T, args...>, V>
{
    using limbs_t = limbs<T, args...>;
    static constexpr auto QR = arithmetic::udiv1<T>(V, sizeof(T) * CHAR_BIT);
    using raw_type = typename detail::shift_left_builder<QR.second, push_front<limbs_t, T(0)>, push_back<limbs_t, T(0)>>::type;
    using type = typename std::conditional_t<!QR.first, raw_type, concat_method<build_method<T, QR.first>, raw_type>>::type;
};

// ////////////////////////////////// SHIFT RIGHT
template <typename LimbsLT, auto V> struct shift_right_method : shift_right_method<typename LimbsLT::type, V> {};
template <typename LimbsLT, auto V> using shift_right = typename shift_right_method<LimbsLT, V>::type;

template <std::integral T, T... args, auto V>
struct shift_right_method<limbs<T, args...>, V>
{
    using limbs_t = limbs<T, args...>;
    static constexpr auto QR = arithmetic::udiv1<T>(V, sizeof(T) * CHAR_BIT);
    using raw_type = typename std::conditional_t<!QR.first, limbs_t,
        std::conditional<(sizeof ...(args) > QR.first), slice_limbs_method<limbs_t, sizeof ...(args) - QR.first>, limbs<T, 0>>
    >::type;
    
    using type = typename std::conditional_t<
        !QR.second,
        raw_type,
        detail::shift_left_builder<sizeof(T)* CHAR_BIT - QR.second, raw_type, push_back_method<pop_front_method<raw_type>, T(0)>>
    >::type;
};

// ////////////////////////////////// ZIP
namespace detail {

template <typename LimbsLT, typename LimbsRT, typename LimbIs> struct zip_impl;

template <typename LimbLT, LimbLT ...largs, typename LimbRT, LimbRT ...rargs, size_t ... LimbIs>
struct zip_impl<limbs<LimbLT, largs...>, limbs<LimbRT, rargs...>, std::index_sequence<LimbIs...>>
{
    using zip_limb_t = std::pair<LimbLT, LimbRT>;
    static constexpr std::array<zip_limb_t, sizeof ...(largs)> zls{ zip_limb_t{largs, rargs} ... };
    using type = limbs<zip_limb_t, zls[LimbIs] ...>;
};

}

template <typename LimbsLT, typename LimbsRT> struct zip_method : zip_method<typename LimbsLT::type, typename LimbsRT::type> {};

template <typename LimbsLT, typename LimbsRT> using zip = typename zip_method<LimbsLT, LimbsRT>::type;

template <typename LT, LT... largs, typename RT, RT... rargs>
requires(sizeof...(largs) == sizeof...(rargs))
struct zip_method<limbs<LT, largs...>, limbs<RT, rargs...>>
    : detail::zip_impl<limbs<LT, largs...>, limbs<RT, rargs...>, decltype(std::make_index_sequence<sizeof...(largs)>())>
{};

// ////////////////////////////////// ADD
namespace detail {
template <typename AccumT, typename LimbsLT, typename LimbsRT> struct add_appender;

template <typename AccumT, typename T, T... largs, T... rargs>
struct add_appender<AccumT, limbs<T, largs...>, limbs<T, rargs...>>
{
    using left_limbs_t = limbs<T, largs...>;
    using right_limbs_t = limbs<T, rargs...>;
    static constexpr auto result_pair_t = arithmetic::uadd1c(front<left_limbs_t>, front<right_limbs_t>, AccumT::second_type::value);
    using accum_t = push_back<typename AccumT::first_type, result_pair_t.second>;
    using c_t = std::integral_constant<T, result_pair_t.first>;
    using type = typename add_appender<std::pair<accum_t, c_t>, pop_front<left_limbs_t>, pop_front<right_limbs_t>>::type;
};

template <typename AccumT, std::integral T>
struct add_appender<AccumT, limbs<T>, limbs<T>> : std::conditional_t<
    AccumT::second_type::value,
    push_back<typename AccumT::first_type, AccumT::second_type::value>,
    typename AccumT::first_type>
{};

}

template <typename LimbsLT, typename LimbsRT> struct add_method : add_method<typename LimbsLT::type, typename LimbsRT::type> {};

template <typename LimbsLT, typename LimbsRT> using add = typename add_method<LimbsLT, LimbsRT>::type;

template <typename T, T... largs, T... rargs>
struct add_method<limbs<T, largs...>, limbs<T, rargs...>>
    : detail::add_appender<std::pair<limbs<T>, std::integral_constant<T, 0>>, limbs<T, largs...>, limbs<T, rargs...>>
{};

// ////////////////////////////////// SUB
namespace detail {

template <typename AccumT, typename LimbsLT, typename LimbsRT> struct sub_appender;

template <typename AccumT, std::integral T, T... largs, T... rargs>
struct sub_appender<AccumT, limbs<T, largs...>, limbs<T, rargs...>>
{
    using left_limbs_t = limbs<T, largs...>;
    using right_limbs_t = limbs<T, rargs...>;
    static constexpr auto result_pair_t = arithmetic::usub1c(front<left_limbs_t>, front<right_limbs_t>, AccumT::second_type::value);
    using accum_t = push_back<typename AccumT::first_type, result_pair_t.second>;
    using c_t = std::integral_constant<T, result_pair_t.first>;
    using type = typename sub_appender<std::pair<accum_t, c_t>, pop_front<left_limbs_t>, pop_front<right_limbs_t>>::type;
};

template <typename AccumT, std::integral T>
struct sub_appender<AccumT, limbs<T>, limbs<T>>
{
    using type = norm<typename AccumT::first_type>;
};

}

template <typename LimbsLT, typename LimbsRT> struct sub_method;

template <typename LimbsLT, typename LimbsRT>
using sub = typename sub_method<LimbsLT, LimbsRT>::type;

template <std::integral T, T... largs, T... rargs>
struct sub_method<limbs<T, largs...>, limbs<T, rargs...>>
    : detail::sub_appender<std::pair<limbs<T>, std::integral_constant<T, 0>>, limbs<T, largs...>, limbs<T, rargs...>>
{};

// ////////////////////////////////// CMP
namespace detail {

template <typename LimbsLT, typename LimbsRT> struct comparer_helper
{
    static constexpr int r_ = arithmetic::ucmp1(back<LimbsLT>, back<LimbsRT>);
    using type = typename std::conditional_t<r_ != 0,
        std::integral_constant<int, r_>,
        comparer_helper<pop_back<LimbsLT>, pop_back<LimbsRT>>
    >::type;

    static constexpr int value = type::value;
};

template <std::integral T>
struct comparer_helper<limbs<T>, limbs<T>> : std::integral_constant<int, 0> {};

}

template <typename LimbsLT, typename LimbsRT> struct cmp_method
{
    using left_limbs_t = norm<LimbsLT>;
    using right_limbs_t = norm<LimbsRT>;
    static constexpr size_t left_sz_ = size<left_limbs_t>;
    static constexpr size_t right_sz_ = size<right_limbs_t>;
    using type = std::conditional_t<
        (left_sz_ < right_sz_),
        std::integral_constant<int, -1>,
        std::conditional_t<
            (right_sz_ < left_sz_),
            std::integral_constant<int, 1>,
            detail::comparer_helper<left_limbs_t, right_limbs_t>
        >
    >;
    static constexpr int value = type::value;
};

template <typename LimbsLT, typename LimbsRT>
constexpr int cmp = cmp_method<LimbsLT, LimbsRT>::value;

// ////////////////////////////////// MUL
namespace detail {

template <typename AccumT, typename LimbsLT, auto V> struct mul1_appender;

template <typename AccumT, std::integral T, T... args, T V>
struct mul1_appender<AccumT, limbs<T, args...>, V>
{
    using left_limbs_t = limbs<T, args...>;
    
    static constexpr auto mul_pair_t = arithmetic::umul1(front<left_limbs_t>, V);
    static constexpr auto add_pair_t = arithmetic::uadd1(mul_pair_t.second, AccumT::second_type::value);
    static constexpr auto c_pair_t = arithmetic::uadd1(mul_pair_t.first, add_pair_t.first);
    static_assert(!c_pair_t.first);
    using accum_t = push_back<typename AccumT::first_type, add_pair_t.second>;
    using c_t = std::integral_constant<T, c_pair_t.second>;
    using type = typename mul1_appender<std::pair<accum_t, c_t>, pop_front<left_limbs_t>, V>::type;
};

template <typename AccumT, typename T, T V>
struct mul1_appender<AccumT, limbs<T>, V> : std::conditional_t<
    !!AccumT::second_type::value,
    push_back<typename AccumT::first_type, AccumT::second_type::value>,
    typename AccumT::first_type>
{};

}

template <typename LimbsLT, auto V> struct mul1_method : mul1_method<typename LimbsLT::type, V> {};

template <typename LimbsLT, auto V> using mul1 = typename mul1_method<LimbsLT, V>::type;

template <typename T, T... args> struct mul1_method<limbs<T, args...>, 0> { using type = limbs<T, 0>; };
template <typename T, T... args> struct mul1_method<limbs<T, args...>, 1> { using type = limbs<T, args...>; };

template <typename T, T... args, auto V>
requires(!(V & (V - 1)))
struct mul1_method<limbs<T, args...>, V> : shift_left_method<limbs<T, args...>, arithmetic::consteval_log2<uint64_t>(V)> {};

template <std::integral T, T... args, auto V>
requires(!!(V & (V - 1)))
struct mul1_method<limbs<T, args...>, V>
    : detail::mul1_appender<std::pair<limbs<T>, std::integral_constant<T, 0>>, limbs<T, args...>, V>
{};

namespace detail {

template <typename AccumT, typename LimbsLT, typename LimbsRT> struct mul_summator;

template <typename AccumT, typename LimbsLT, typename T, T... rargs>
struct mul_summator<AccumT, LimbsLT, limbs<T, rargs...>>
{
    using left_limbs_t = limbs<T, rargs...>;
    using term = mul1<LimbsLT, back<left_limbs_t>>;
    using accum_t = add<push_front<AccumT, T{ 0 }>, term>;
    using type = typename mul_summator<accum_t, LimbsLT, pop_back<left_limbs_t>>::type;
};

template <typename AccumT, typename LimbsLT, typename T>
struct mul_summator<AccumT, LimbsLT, limbs<T>> { using type = AccumT; };

}

template <typename LimbsLT, typename LimbsRT> struct mul_method : mul_method<typename LimbsLT::type, typename LimbsRT::type> {};

template <typename LimbsLT, typename LimbsRT> using mul = typename mul_method<LimbsLT, LimbsRT>::type;

template <typename T, T... largs, T... rargs>
struct mul_method<limbs<T, largs...>, limbs<T, rargs...>>
{
    // select greater multiplier as left
    static constexpr size_t lcount = (std::max)(sizeof...(largs), sizeof...(rargs));
    using left_limbs_t = std::conditional_t<lcount == sizeof...(largs), limbs<T, largs...>, limbs<T, rargs...>>;
    using right_limbs_t = std::conditional_t<lcount == sizeof...(largs), limbs<T, rargs...>, limbs<T, largs...>>;
    using type = typename detail::mul_summator<limbs<T>, left_limbs_t, right_limbs_t>::type;
};

// ////////////////////////////////// DIV
namespace detail {

template <typename AccumT, typename LimbsLT, auto V> struct div1_appender;

template <typename AccumT, std::integral T, T... args, T V>
struct div1_appender<AccumT, limbs<T, args...>, V>
{
    using left_limbs_t = limbs<T, args...>;
    static constexpr auto div_pair_t = arithmetic::udiv2by1(AccumT::second_type::value, back<left_limbs_t>, V);
    using accum_t = push_front<typename AccumT::first_type, div_pair_t.first>;
    using c_t = std::integral_constant<T, div_pair_t.second>;
    using type = typename div1_appender<std::pair<accum_t, c_t>, pop_back<left_limbs_t>, V>::type;
};

template <typename AccumT, typename T, T V>
struct div1_appender<AccumT, limbs<T>, V> { using type = AccumT; };

}

template <typename LimbsLT, auto V> struct div1_method : div1_method<typename LimbsLT::type, V> {};
template <typename T, T... args> struct div1_method<limbs<T, args...>, 1> { using type = std::pair<limbs<T, args...>, limbs<T, 0>>; };

//template <typename T, T... args, auto V>
//requires(!(V& (V - 1)))
//struct div1_method<limbs<T, args...>, V>
//{
//    static constexpr auto shift = arithmetic::consteval_log2<uint64_t>(V);
//    using q = shift_right_method<limbs<T, args...>, shift>{};
//    using r = slice<limbs<T, args...>, shift>;
//    using type = std::pair<q, r>;
//};

template <std::integral T, T... args, T V>
struct div1_method<limbs<T, args...>, V>
{
    using left_t = limbs<T, args...>;
    // normalization
    static constexpr int shift = arithmetic::count_leading_zeros(V);
    using norm_left_t = typename std::conditional_t<!shift, left_t, shift_left_method<left_t, shift>>::type;

    using unshifted_type = typename detail::div1_appender<std::pair<limbs<T>, std::integral_constant<T, 0>>, norm_left_t, (V << shift)>::type;

    using qtype = norm<typename unshifted_type::first_type>;
    using rtype = std::integral_constant<T, (unshifted_type::second_type::value >> shift)>;
};

template <typename LimbsLT, auto V> using div1 = typename div1_method<LimbsLT, V>::qtype;

template <typename LimbsLT, typename LimbsRT> struct div_method : div_method<typename LimbsLT::type, typename LimbsRT::type> {};
template <typename LimbsLT, typename LimbsRT> using div = typename div_method<LimbsLT, LimbsRT>::type;

template <std::integral T, T... largs, T... rargs>
struct div_method<limbs<T, largs...>, limbs<T, rargs...>>
{

};

// ////////////////////////////////// POW
template <typename LimbsT, auto a> struct pow_method: pow_method<typename LimbsT::type, a> {};

template <typename LimbsT, auto a> using pow = typename pow_method<LimbsT, a>::type;

template <typename T, T... args>
struct pow_method<limbs<T, args...>, 0> { using type = limbs<T, 1>; };

template <typename T, T... args>
struct pow_method<limbs<T, args...>, 1> { using type = limbs<T, args...>; };

template <typename T, T... args, auto a>
struct pow_method<limbs<T, args...>, a> : mul_method<pow<limbs<T, args...>, a / 2>, pow<limbs<T, args...>, (a + 1) / 2>> {};

namespace literals {

inline constexpr unsigned int char_to_digit(char val)
{
    assert(val >= '0' && val <= '9');
    return (unsigned int)val - '0';
}

template <auto Base, typename AccumT, unsigned int ... args>
struct mul_base_plus_method;

template <std::integral T, T... accum, T Base, unsigned int a0, unsigned int ... args>
struct mul_base_plus_method<Base, limbs<T, accum...>, a0, args...> : 
    mul_base_plus_method<Base, add<mul1<limbs<T, accum...>, Base>, limbs<T, a0>>, args...>
{};

template <std::integral T, T... accum, T Base>
struct mul_base_plus_method<Base, limbs<T, accum...>> { using type = limbs<T, accum...>; };

template <char... Chars> constexpr auto operator"" _W()
{
    return typename mul_base_plus_method<uint64_t(10), limbs<uint64_t>, char_to_digit(Chars) ...>::type{};
}

}

template <uint64_t... args> constexpr limbs<uint64_t, args...> W = {};
template <uint64_t... args> constexpr limbs<uint64_t, args...> W64 = {};
template <uint32_t... args> constexpr limbs<uint32_t, args...> W32 = {};
template <uint16_t... args> constexpr limbs<uint16_t, args...> W16 = {};
template <uint8_t... args> constexpr limbs<uint8_t, args...> W8 = {};

template <std::integral T, T... largs, T... rargs>
inline constexpr auto operator+ (limbs<T, largs...> const&, limbs<T, rargs...>  const&)
{
    using result_type = add<limbs<T, largs...>, limbs<T, rargs...>>;
    return result_type{};
}

template <std::integral T, T... largs, T... rargs>
inline constexpr auto operator- (limbs<T, largs...> const&, limbs<T, rargs...> const&)
{
    using result_type = sub<limbs<T, largs...>, limbs<T, rargs...>>;
    return result_type{};
}

template <std::integral T, T... largs, T rarg>
inline constexpr auto operator* (limbs<T, largs...> const&, limbs<T, rarg> const&)
{
    using result_type = mul1<limbs<T, largs...>, rarg>;
    return result_type{};
}

template <std::integral T, T... largs, T... rargs>
inline constexpr auto operator* (limbs<T, largs...> const&, limbs<T, rargs...> const&)
{
    using result_type = mul<limbs<T, largs...>, limbs<T, rargs...>>;
    return result_type{};
}

template <std::integral T, T... largs, T rarg>
inline constexpr auto operator/ (limbs<T, largs...> const&, limbs<T, rarg> const&)
{
    using result_type = div1<limbs<T, largs...>, rarg>;
    return result_type{};
}

template <std::integral T, T... largs, T... rargs>
inline constexpr auto operator/ (limbs<T, largs...> const&, limbs<T, rargs...> const&)
{
    using result_type = mul<limbs<T, largs...>, limbs<T, rargs...>>;
    return result_type{};
}

template <std::integral T, T... largs, T rarg>
inline constexpr auto upow(limbs<T, largs...> const&, limbs<T, rarg> const&)
{
    using result_type = pow<limbs<T, largs...>, rarg>;
    return result_type{};
}

template <std::integral T, T... largs, T rarg>
inline constexpr auto operator<< (limbs<T, largs...> const&, limbs<T, rarg> const&)
{
    using result_type = shift_left<limbs<T, largs...>, rarg>;
    return result_type{};
}

template <std::integral T, T... largs, T rarg>
inline constexpr auto operator>> (limbs<T, largs...> const&, limbs<T, rarg> const&)
{
    using result_type = shift_right<limbs<T, largs...>, rarg>;
    return result_type{};
}

template <std::integral T, T... args>
inline constexpr auto normalize(limbs<T, args...> const&)
{
    using result_type = norm<limbs<T, args...>>;
    return result_type{};
}

template <std::integral T, T... args>
inline constexpr size_t getsize(limbs<T, args...> const&)
{
    return sizeof...(args);
}

#if 0
template <size_t N, std::unsigned_integral T = uint64_t>
struct big_uint : std::array<T, N> {};

template <size_t N, typename T>
constexpr auto shift_right(big_uint<N, T> a, size_t k)
{
    big_uint<N, T> res{};

    if (k == 0U) return a;

    for (auto i = 0U; i < N - 1; ++i) {
        res[i] = (a[i] >> k) | (a[i + 1] << (std::numeric_limits<T>::digits - k));
    }
    res[N - 1] = (a[N - 1] >> k);
    return res;
}

template <size_t N, typename T>
constexpr auto shift_left(big_uint<N, T> a, size_t k)
{
    if (k == 0U) return detail::pad<1>(a);

    big_uint<N + 1, T> res{};

    res[0] = (a[0] << k);

    for (auto i = 1U; i < N; ++i) {
        res[i] = (a[i] << k) | (a[i - 1] >> (std::numeric_limits<T>::digits - k));
    }

    res[N] = a[N - 1] >> (std::numeric_limits<T>::digits - k);
    return res;
}

namespace detail {

template <typename T = uint64_t, size_t L = 0, char... Chars> //, std::size_t... Is>
constexpr auto
chars_to_big_int(std::integer_sequence<char, Chars...>)
{
    constexpr size_t len = sizeof...(Chars);
    constexpr size_t N = (std::max)(L, 1 + (10 * len) / (3 * std::numeric_limits<T>::digits));
    std::array<char, len> digits{ Chars... };
    big_uint<N, T> num{ 0 };
    big_uint<N, T> power_of_ten{ 1 };

    for (int i = len - 1; i >= 0; --i) {
        num = add_ignore_carry(num, partial_mul<N>(big_uint<1, T>{static_cast<T>(digits[i]) - 48}, power_of_ten));
        power_of_ten = partial_mul<N>(big_uint<1, T>{static_cast<T>(10)}, power_of_ten);
    }
    return num;
}

}

template <typename T, char... Chars> constexpr auto generic_limb_literal()
{
    constexpr size_t len = sizeof...(Chars);
    constexpr size_t N = 1 + (10 * len) / (3 * std::numeric_limits<T>::digits);
    auto num = detail::chars_to_integer_seq<T>(std::integer_sequence<char, Chars...>{}, std::make_index_sequence<N>{});
    constexpr auto L = detail::tight_length(num) + (to_big_int(num) == big_uint<1, T>{});
    return detail::take_first(num, std::make_index_sequence<L>{});
}

template <char... Chars> constexpr auto operator"" _Z()
{
    return generic_limb_literal<uint64_t, Chars...>();
}
#endif
}
