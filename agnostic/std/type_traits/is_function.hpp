// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_FUNCTION_HPP
#define AGNOSTIC_STD_IS_FUNCTION_HPP

#pragma once

namespace std {

template<class>
struct is_function : false_type { };

// specialization for regular functions
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)> : true_type {};

// specialization for variadic functions such as std::printf
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)> : true_type {};

// specialization for function types that have cv-qualifiers
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)const> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)volatile> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)const volatile> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)const> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)volatile> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)const volatile> : true_type {};

// specialization for function types that have ref-qualifiers
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)const&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)volatile&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)const volatile&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)const&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)volatile&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)const volatile&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)&&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)const&&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)volatile&&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT...)const volatile&&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)&&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)const&&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)volatile&&> : true_type {};
template <typename RT, typename... ArgsT>
struct is_function<RT(ArgsT..., ...)const volatile&&> : true_type {};

template <class T> constexpr bool is_function_v = is_function<T>::value;

}

#endif // AGNOSTIC_STD_IS_FUNCTION_HPP
