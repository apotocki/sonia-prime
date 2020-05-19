// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_REMOVE_CV_HPP
#define AGNOSTIC_STD_REMOVE_CV_HPP

#pragma once

namespace std {

template <typename T> struct remove_cv { using type = remove_volatile_t<remove_const_t<T>>; };
template <typename T> using remove_cv_t = typename remove_cv<T>::type;

}

#endif // AGNOSTIC_STD_REMOVE_CV_HPP
