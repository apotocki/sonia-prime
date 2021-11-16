/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "sonia/utility/number/fixed_uint.hpp"

namespace sonia::conversion {

template <
    unsigned char w, unsigned int r, unsigned int b
>
class rc5
{
    static const unsigned char u = w/8;
    static const unsigned char c = (b + u - 1)/u;
    static const unsigned char t = 2*(r + 1);

    using cP_ = mpl::big_uint_hex_<0xb7e1, 0x5163>;
    using cQ_ = mpl::big_uint_hex_<0x9e37, 0x79b9>;

    //using cP_ = mpl::big_uint_hex_<0xb7e1, 0x5162, 0x8aed, 0x2a6b>;
    //using cQ_ = mpl::big_uint_hex_<0x9e37, 0x79b9, 0x7f4a, 0x7c15>;

    static fixed_uint<w> Q;
    static fixed_uint<w> P;
    /*
    typedef typename boost::mpl::fold<
          boost::mpl::range_c<int, 0, t - 1>
        , boost::mpl::list1<P>
        , typename boost::mpl::push_front<
              boost::mpl::_1
            , boost::mpl::plus<boost::mpl::front<boost::mpl::_1>, Q>
        >
    >::type preS_;
    */
    static const unsigned char defaultIV[2*u];
public:
    
    explicit rc5(
        const unsigned char (&password)[b],
        const unsigned char (&iv)[2*u] = rc5::defaultIV)
    {
        unsigned int i, j, n;
        for (i = 0; i < u; i++)
        {
            fixed_uint<w> temp = iv[i]; roll_left(temp, 8*i);
            iv0_ += temp;
            temp = iv[i + u]; roll_left(temp, 8*i);
            iv1_ += temp;
        }
    //explicit rc5(const unsigned char (&password)[b], fixed_uint<w> const& iv0 = 0, fixed_uint<w> const& iv1 = 0)
    //    : iv0_(iv0), iv1_(iv1)
    //{
        
        // init S-table
        S_[0] = P;
        fixed_uint<w> temp = P;
        for (i = 1; i < t; i++)
        {
            temp += Q;
            S_[i] = temp;
        }

        // init L-table
        fixed_uint<w> L[c];
        for (i = 0; i < b; ++i)
        {
            fixed_uint<w> temp = password[i];
            temp.roll_left(8 * i);
            L[i/u] += temp;
        }

        // mixing S
        fixed_uint<w> A, B;
        for (i = 0, j = 0, n = 3 * (t > c ? t : c); n > 0; --n, i = (1+i)%t, j = (1+j)%c)
        {
            S_[i] = A = (S_[i] + A + B).roll_left(3);
            B += A;    L[j] = B = (L[j] + B).roll_left(slice_to<unsigned long>(B));
        }

        /*
        for (int i = 0; i < t; i++)
        {
            std::cout << "S[" << i << "]=" << S_[i] << "\n"; 
        }
        std::cout << "\n";
        for (int i = 0; i < c; i++)
        {
            std::cout << "L[" << i << "]=" << L[i] << "\n"; 
        }
        */
    }

    void encode(fixed_uint<w> & A, fixed_uint<w> & B)
    {
        A ^= iv0_; B ^= iv1_;
        A += S_[0]; B += S_[1];
        //std::cout << "A: " << A << ", B:" << B << "\n";
        for (int i = 1; i <= r; ++i)
        {
            A = (A ^ B).roll_left(slice_to<unsigned long>(B)) + S_[2 * i];
            B = (A ^ B).roll_left(slice_to<unsigned long>(A)) + S_[2 * i + 1];
            //std::cout << "A(" << i << "): " << A << ", B:" << B << "\n";
        }
    }

    void decode(fixed_uint<w> & A, fixed_uint<w> & B)
    {
        for (int i = r; i > 0; --i)
        {
            //std::cout << "A(" << i << "): " << A << ", B:" << B << "\n";
            B = (B - S_[2 * i + 1]).roll_right(slice_to<unsigned long>(A)) ^ A;
            A = (A - S_[2 * i]).roll_right(slice_to<unsigned long>(B)) ^ B;
        }
        A -= S_[0]; B -= S_[1];
        A ^= iv0_; B ^= iv1_;
    }

private:
    fixed_uint<w> S_[t];
    fixed_uint<w> iv0_, iv1_;
};

template <unsigned char w, unsigned int r, unsigned int b>
const unsigned char rc5<w, r, b>::defaultIV[2*u](0);

template <unsigned char w, unsigned int r, unsigned int b>
fixed_uint<w> rc5<w, r, b>::Q = fixed_uint<w>(rc5<w, r, b>::cQ_()) + fixed_uint<w>(is_odd(fixed_uint<w>(rc5<w, r, b>::cQ_())) ? 0 : 1);

template <unsigned char w, unsigned int r, unsigned int b>
fixed_uint<w> rc5<w, r, b>::P = fixed_uint<w>(rc5<w, r, b>::cP_()) + fixed_uint<w>(is_odd(fixed_uint<w>(rc5<w, r, b>::cP_())) ? 0 : 1);

}
