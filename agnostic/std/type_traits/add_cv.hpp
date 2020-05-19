// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ADD_CV_HPP
#define AGNOSTIC_STD_ADD_CV_HPP

#pragma once

namespace std {

template <typename T> struct add_cv
{
    using type = add_volatile_t<add_const_t<T>>;
};

template <typename T> using add_cv_t = typename add_cv<T>::type;

}

#endif // AGNOSTIC_STD_ADD_CV_HPP
