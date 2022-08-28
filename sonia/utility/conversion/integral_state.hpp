/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>

namespace sonia::conversion {

template <typename T, typename ErrorHandlerT>
struct integral_state : private ErrorHandlerT
{
    T value_;

    template <typename ArgT>
    inline void clear(ArgT && arg) {
        static_cast<ErrorHandlerT&>(*this) = std::forward<ArgT>(arg);
        value_ = 0;
    }

    template <typename VT>
    inline void error(VT&& v, const char *errmsg) {
        ErrorHandlerT::operator()(std::forward<VT>(v), *this, errmsg);
    }

    template <typename VT, typename IteratorT>
    inline void error(VT&& v, const char *errmsg, IteratorT & it) {
        ErrorHandlerT::operator()(std::forward<VT>(v), *this, errmsg, it);
    }

    inline T const& get() const { return value_; }
    inline T & get() { return value_; }

    inline bool operator== (integral_state const& rhs) const { return value_ == rhs.value_; }

    inline void reset() { value_ = 0; }
};

template <typename ErrorHandlerT>
struct integral_state<void, ErrorHandlerT> : private ErrorHandlerT
{
    template <typename ArgT>
    inline void clear(ArgT && arg) {
        static_cast<ErrorHandlerT&>(*this) = std::forward<ArgT>(arg);
    }

    template <typename VT>
    inline void error(VT&& v, const char *errmsg) {
        ErrorHandlerT::operator()(std::forward<VT>(v), *this, errmsg);
    }

    template <typename VT, typename IteratorT>
    inline void error(VT&& v, const char *errmsg, IteratorT & it) {
        ErrorHandlerT::operator()(std::forward<VT>(v), *this, errmsg, it);
    }

    inline bool operator== (integral_state const& rhs) const { return true; }

    inline void reset() {}
};

template <typename T, typename ErrorHandlerT>
struct inherited_integral_state : public T, private ErrorHandlerT
{
    using T::clear;

    template <typename ArgT>
    inline void clear(ArgT&& arg) {
        static_cast<ErrorHandlerT&>(*this) = std::forward<ArgT>(arg);
        T::clear();
    }

    template <typename VT>
    inline void error(VT&& v, const char* errmsg) {
        ErrorHandlerT::operator()(std::forward<VT>(v), *this, errmsg);
    }

    template <typename VT, typename IteratorT>
    inline void error(VT&& v, const char* errmsg, IteratorT& it) {
        ErrorHandlerT::operator()(std::forward<VT>(v), *this, errmsg, it);
    }

    inline T const& get() const { return *this; }
    inline T& get() { return *this; }

    inline bool operator== (inherited_integral_state const& rhs) const { return get() == rhs.get(); }
};

}
