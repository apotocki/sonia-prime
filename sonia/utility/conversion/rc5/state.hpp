/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <boost/assert.hpp>

#include "sonia/cstdint.hpp"

#include "rc5_algo.hpp"

namespace sonia::conversion {

template <unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
struct rc5_state : private ErrorHandlerT
{
    static const unsigned char u = w/8;

    fixed_uint<w> A;
    fixed_uint<w> B;
    
    unsigned int size;
    
    rc5_algo<w, r, b> algorithm_;
    char padding_c_;

    template <typename ArgT>
    inline void clear(ArgT const& arg) {
        A = B = 0;
        size = 0;
        static_cast<ErrorHandlerT&>(*this) = arg.errh_;
        algorithm_ = rc5_algo<w, r, b>(arg.password_);
        padding_c_ = arg.padding_character_;

        for (unsigned int i = 0; i < u; i++) pushA(arg.iv_[i]);
        for (unsigned int i = 0; i < u; i++) pushB(arg.iv_[i + u]);
        size = 0;
    }

    inline bool operator==(rc5_state const& rhs) const
    {
        return size == rhs.size && A == rhs.A && B == rhs.B;
    }

    inline char popA()
    {
        BOOST_ASSERT(size <= 2*u && size > u);
        fixed_uint<w> temp = A; roll_right(temp, (2*u - size--)*8);
        return slice_to<unsigned char>(temp);
    }

    inline char popB()
    {
        BOOST_ASSERT(size > 0 && size <= u);
        fixed_uint<w> temp = B; roll_right(temp, (u - size--)*8);
        return slice_to<unsigned char>(temp);
    }

    inline void pushA(char v)
    {
        BOOST_ASSERT(size < u);
        fixed_uint<w> temp = static_cast<unsigned char>(v); roll_left(temp, 8*size++);
        A += temp;
    }

    inline void push_xor_A(char v)
    {
        BOOST_ASSERT(size < u);
        fixed_uint<w> temp = static_cast<unsigned char>(v); roll_left(temp, 8*size++);
        A ^= temp;
    }

    inline void pushB(char v)
    {
        BOOST_ASSERT(size < 2*u && size >= u);
        fixed_uint<w> temp = static_cast<unsigned char>(v); roll_left(temp, 8*(size++-u));
        B += temp;
    }

    inline void push_xor_B(char v)
    {
        BOOST_ASSERT(size >= u && size < 2 * u);
        fixed_uint<w> temp = static_cast<unsigned char>(v); roll_left(temp, 8*size++);
        B ^= temp;
    }

    inline void error(char c, const char *errmsg) {
        ErrorHandlerT::operator()(c, *this, errmsg);
    }
};

template <unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
struct rc5_dstate : rc5_state<w, r, b, ErrorHandlerT>
{
    fixed_uint<w> prevA;
    fixed_uint<w> prevB;

    template <typename ArgT>
    inline void clear(ArgT const& arg) {
        rc5_state<w, r, b, ErrorHandlerT>::clear(arg);
        prevA = this->A; prevB = this->B;
        this->A = this->B = 0;
    }
};

}
