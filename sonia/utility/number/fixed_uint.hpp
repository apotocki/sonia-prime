//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/mpl/min_max.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/reverse.hpp>
#include <boost/mpl/size.hpp>

#include <boost/static_assert.hpp>
#include <boost/integer_traits.hpp>
#include <boost/integer/integer_mask.hpp>

#include <iomanip>
#include <algorithm>
#include <functional>

#include "meta/uint.hpp"
#include "meta/for_each_wi.hpp"

namespace sonia {

// forward declarations
template <std::size_t N, typename TokenT>
class fixed_uint;

//template <std::size_t N1, std::size_t N2>
//bool operator == (fixed_uint<N1> const& lhs, fixed_uint<N2> const& rhs);

template<std::size_t N, typename TokenT>
bool is_odd(fixed_uint<N, TokenT> const& v);

template<std::size_t N, typename TokenT>
std::ostream& operator<<(std::ostream& os, fixed_uint<N, TokenT> const& arg);

///////////////////////////////////
namespace detail {

template <std::size_t sz>
struct uint_printer
{
    std::ostream & os_;
    const uint_fast32_t (&sequence_)[sz];

    bool flag_;
    uint_printer(std::ostream & os, const uint_fast32_t (&seq)[sz])
        : os_(os), sequence_(seq), flag_(false) {}

    void operator() (int N)
    {
        uint_fast32_t val = sequence_[sz - 1 - N];
        if (flag_) {
            os_ << std::setw(8) << val;
        } else if (N == sz - 1) {
            os_ << val;
        } else if (val) {
            os_ << std::setw(8) << std::setfill('0') << val;
            flag_ = true;
        }
    }
};

template <std::size_t sz>
struct uint_setter
{
    uint_fast32_t (&sequence_)[sz];

    uint_setter(uint_fast32_t (&seq)[sz])
        : sequence_(seq) {}

    template <typename elemT>
    void operator() (int index, elemT)
    {
        sequence_[index] = elemT::value;
        //std::cout << std::hex << "seq[" << elemNum::value << "]=" << seqElem::value << "\n";
    }
};

template <unsigned int BV>
struct low_bits_mask_t : boost::low_bits_mask_t<BV> {};

template <>
struct low_bits_mask_t<0> {
    static const unsigned int sig_bits_fast = 0;
};

}


template <std::size_t N, typename TokenT = uint_fast32_t>
class fixed_uint
{
    static const std::size_t bytes_ = (N + 7)/8;
    static const std::size_t token_bytes_ = sizeof(TokenT);
    static const std::size_t token_bits_ = token_bytes_ * 8;
    static const std::size_t token_maxbit_ = token_bits_ - 1;
    static const std::size_t tokens_ = (bytes_ + token_bytes_ - 1) / token_bytes_;

    typedef boost::high_bit_mask_t<token_bits_ - 1>  mask_type_;
    static const TokenT token_mask8000_ = static_cast<TokenT>(mask_type_::high_bit);
    static const TokenT token_mask7fff_ = token_mask8000_ - 1;
    static const TokenT last_token_mask_ = static_cast<TokenT>(boost::low_bits_mask_t<N % token_bits_ == 0 ? token_bits_ : N % token_bits_>::sig_bits);

    TokenT sequence_[tokens_];

public:

    inline fixed_uint()
    {
        std::fill_n(sequence_, tokens_, 0);
    }

    inline fixed_uint(uint_fast32_t val)
    {
        std::fill_n(sequence_, tokens_, 0);
        sequence_[0] = val;
    }

    template <typename seqT>
    inline explicit fixed_uint(mpl::big_uint_<seqT>)
    {
        *this = mpl::big_uint_<seqT>();
    }

    inline fixed_uint(fixed_uint const& arg)
    {
        std::copy(arg.sequence_, arg.sequence_ + tokens_, sequence_);
    }

    template <std::size_t N1>
    inline bool operator != (fixed_uint<N1, TokenT> const& rhs) const
    { return !((*this) == rhs); }

    template <typename seqT>
    inline fixed_uint & operator = (mpl::big_uint_<seqT>)
    {
        using namespace boost::mpl;
        memset(&sequence_, 0, tokens_ * sizeof(uint_fast32_t));
        typedef typename eval_if<
            boost::mpl::less<size<seqT>, int_<tokens_> >
            , identity<seqT>
            , reverse<
                typename fold<
                      seqT
                    , list0<>
                    , eval_if<
                          boost::mpl::less<size<boost::mpl::_1>, int_<tokens_> >
                        , push_front<boost::mpl::_1, boost::mpl::_2>
                        , boost::mpl::_1
                    >
                >::type
            >
        >::type red_seq;
        //std::cout << typeid(red_seq).name() << "\n";
        //typedef zip_view<vector2<range_c<int, 0, size<red_seq>::value>, red_seq> > tempseq;
        mpl::for_each_wi<red_seq>(detail::uint_setter<tokens_>(sequence_));
        return *this;
    }

    fixed_uint& operator ^= (fixed_uint const& rhs)
    {
        for (unsigned int i = 0; i < tokens_; ++i)
        {
            sequence_[i] ^= rhs.sequence_[i];
        }
        return *this;
    }

    fixed_uint operator ~() const
    {
        fixed_uint result;
        for (unsigned int i = 0; i < tokens_; ++i)
        {
            result.sequence_[i] = ~sequence_[i];
        }
        return result;
    }

    template <std::size_t N1>
    fixed_uint& operator -= (fixed_uint<N1, TokenT> const& arg)
    {
        // argument must be lower size
        BOOST_STATIC_ASSERT(N1 <= N);
        fixed_uint<N1, TokenT> temp = ~arg + fixed_uint<N1, TokenT>(1);
        return (*this) += temp;
    }

    template <std::size_t N1>
    fixed_uint& operator += (fixed_uint<N1, TokenT> const& arg)
    {
        // using namespace boost::mpl
        // argument must be lower size
        BOOST_STATIC_ASSERT(N1 <= N);
        //static const std::size_t min = min<int_<tokens_>, int_<fixed_uint<N1, TokenT>::tokens_> >::type::value;
        static const std::size_t min = fixed_uint<N1, TokenT>::tokens_;
        TokenT accum = 0;
        std::size_t i = 0;
        for (i = 0; i < min; ++i)
        {
            TokenT arg0 = sequence_[i];
            TokenT arg1 = arg.sequence_[i];
            TokenT res = (arg0 & token_mask7fff_) + (arg1 & token_mask7fff_) + accum;
            accum = ((arg0 & token_mask8000_) >> token_maxbit_) + ((arg1 & token_mask8000_) >> token_maxbit_) + ((res & token_mask8000_) >> token_maxbit_);
            sequence_[i] = (res & token_mask7fff_) + (accum << token_maxbit_);
            accum >>= 1;
        }
        for (; i < tokens_; ++i)
        {
            TokenT arg0 = sequence_[i];
            TokenT res = (arg0 & token_mask7fff_) + accum;
            accum = ((arg0 & token_mask8000_) >> token_maxbit_) + ((res & token_mask8000_) >> token_maxbit_);
            sequence_[i] = (res & token_mask7fff_) + (accum << token_maxbit_);
            accum >>= 1;
        }
        sequence_[tokens_ - 1] &= last_token_mask_;
        return *this;
    }

    template<typename T>
    T slice_to() const
    {
        static const unsigned int b = boost::integer_traits<T>::digits;
        static const unsigned int u = b / 32;
        static const unsigned int u1 = (b-1) / 32;
        static const unsigned int u2 = b % 32;
        unsigned int i = 0;
        T r = 0;
        for (i = 0; i < (u < tokens_ ? u : tokens_); i += 32)
        {
            T temp = static_cast<T>(sequence_[i]); temp <<= (32 * i);
            r += temp;
        }
        if (u2) {
            r += static_cast<T>((sequence_[i] & detail::low_bits_mask_t<u2>::sig_bits_fast) << (32 * u1));
        }
        return r;
    }

    fixed_uint& roll_left(unsigned int bits)
    {
        return roll_right(N - (bits % N));
    }

    fixed_uint& roll_right(unsigned int bits)
    {
        int dw_shift = (bits / 32) % tokens_;
        int b_shift = bits & 0x1f;

        if (b_shift == 0)
        {
            if (dw_shift != 0)
            {
                uint_fast32_t *cache = reinterpret_cast<uint_fast32_t*>(alloca(sizeof(uint_fast32_t) * dw_shift));

                memcpy(cache, sequence_, dw_shift * sizeof(uint_fast32_t));
                memmove(sequence_, sequence_ + dw_shift, (tokens_ - dw_shift) * sizeof(uint_fast32_t));
                memcpy(sequence_ + tokens_ - dw_shift, cache, dw_shift * sizeof(uint_fast32_t));
            }
        } else {

            uint_fast32_t *cache = reinterpret_cast<uint_fast32_t*>(alloca(sizeof(uint_fast32_t) * (dw_shift + 1)));

            unsigned int i, j;
            for (i = tokens_ - dw_shift - 1, j = 0; i < tokens_; ++i, j++)
            {
                uint_fast32_t a0 = sequence_[(dw_shift + i)%tokens_] >> b_shift;
                uint_fast32_t a1 = sequence_[j] << (32 - b_shift);
                cache[j] = a0 | a1;
                cache[j] = (sequence_[(dw_shift + i)%tokens_] >> b_shift) | (sequence_[j] << (32 - b_shift));
            }

            for (i = 0; i < tokens_ - dw_shift - 1; ++i)
            {
                sequence_[i] = ((sequence_[dw_shift + i] >> b_shift) | (sequence_[dw_shift + i + 1] << (32 - b_shift))) & 0xFFFFFFFF;
            }

            memcpy(sequence_ + tokens_ - dw_shift - 1, cache, (1 + dw_shift) * sizeof(uint_fast32_t));
        }
        sequence_[tokens_ - 1] &= last_token_mask_;
        return *this;
    }

    template <std::size_t N1>
    bool operator== (fixed_uint<N1, TokenT> const& rhs) const;

    friend std::ostream& operator<< <N, TokenT>(std::ostream& Os, fixed_uint<N, TokenT> const& arg);
    friend bool is_odd<N>(fixed_uint<N, TokenT> const& v);
};

template <std::size_t N, typename TokenT>
template <std::size_t N1>
inline bool fixed_uint<N, TokenT>::operator== (fixed_uint<N1, TokenT> const& rhs) const
{
    if (!std::equal(
        N > N1 ? rhs.sequence_ : sequence_
        , (N > N1 ? rhs.sequence_ : sequence_) + (N > N1 ? fixed_uint<N1, TokenT>::tokens_ : tokens_)
        , N > N1 ? sequence_ : rhs.sequence_)) return false;

    return ((N > N1 ? sequence_ : rhs.sequence_) + (N > N1 ? tokens_ : fixed_uint<N1, TokenT>::tokens_)) == std::find_if(
        (N > N1 ? sequence_ : rhs.sequence_) + (N > N1 ? fixed_uint<N1, TokenT>::tokens_ : tokens_)
        , (N > N1 ? sequence_ : rhs.sequence_) + (N > N1 ? tokens_ : fixed_uint<N1, TokenT>::tokens_)
        , [](auto arg) { return std::not_equal_to<TokenT>()(0, arg); });
}

template<std::size_t N0, std::size_t N1, typename TokenT>
inline fixed_uint<boost::mpl::max<boost::mpl::size_t<N0>, boost::mpl::size_t<N1> >::type::value, TokenT>
operator + (fixed_uint<N0, TokenT> const& arg0, fixed_uint<N1, TokenT> const& arg1)
{
    fixed_uint<boost::mpl::max<boost::mpl::size_t<N0>, boost::mpl::size_t<N1> >::type::value, TokenT> result = N0 > N1 ? arg0 : arg1;
    result += N0 > N1 ? arg1 : arg0;
    return result;
}

template<std::size_t N0, std::size_t N1, typename TokenT>
inline fixed_uint<boost::mpl::max<boost::mpl::size_t<N0>, boost::mpl::size_t<N1> >::type::value, TokenT>
operator - (fixed_uint<N0, TokenT> const& arg0, fixed_uint<N1, TokenT> const& arg1)
{
    fixed_uint<boost::mpl::max<boost::mpl::size_t<N0>, boost::mpl::size_t<N1> >::type::value, TokenT> result = arg0;
    result -= arg1;
    return result;
}

template <std::size_t N, typename TokenT>
inline fixed_uint<N, TokenT>
operator ^ (fixed_uint<N, TokenT> const& lhs, fixed_uint<N, TokenT> const& rhs)
{
    fixed_uint<N> result = lhs;
    return result ^= rhs;
}

template <std::size_t N, typename TokenT>
inline fixed_uint<N, TokenT>& roll_left(fixed_uint<N, TokenT> & arg, unsigned int bits)
{
    return arg.roll_left(bits);
}

template <std::size_t N, typename TokenT>
inline fixed_uint<N, TokenT>& roll_right(fixed_uint<N, TokenT> & arg, unsigned int bits)
{
    return arg.roll_right(bits);
}

template<std::size_t N, typename TokenT>
std::ostream& operator<<(std::ostream& os, fixed_uint<N, TokenT> const& arg)
{
    os << "0x" << std::hex;// << std::setfill('0');
    boost::mpl::for_each<boost::mpl::range_c<int, 0, fixed_uint<N, TokenT>::tokens_> >(detail::uint_printer<fixed_uint<N, TokenT>::tokens_>(os, arg.sequence_));
    //arg.print(Os);
    return os;
}

template<std::size_t N, typename TokenT>
bool is_odd(fixed_uint<N, TokenT> const& v)
{
    return v.sequence_[0] & 1;
}

template<typename T, std::size_t N, typename TokenT>
T slice_to(fixed_uint<N, TokenT> const& arg)
{
    return arg.template slice_to<T>();
}

}
// sonia::fixed_uint<typename boost::mpl::max<boost::mpl::size_t<N0>, boost::mpl::size_t<N1> >::type::value>

