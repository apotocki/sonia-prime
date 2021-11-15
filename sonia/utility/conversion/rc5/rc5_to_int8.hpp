/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../converter_facade.hpp"

#include "rc5_tag.hpp"
#include "rc5_algo.hpp"
#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
class rc5_to_int8
    : public converter_facade<
          rc5_to_int8<ProviderT, CustomerT, w, r, b, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , rc5_dstate<w, r, b, ErrorHandlerT>
        , rc5_dstate<w, r, b, ErrorHandlerT>
    >
{
protected:
    using base_type = typename rc5_to_int8::facade_type;
    using state_t = rc5_dstate<w, r, b, ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

    static const unsigned char u = w/8;

private:
    inline static void do_clear(rc5_tag<w, r, b, ErrorHandlerT> const& arg, int8_tag const&, state_t & st)
    {
        st.clear(arg);
    }

    inline static bool do_equal_states(state_t const& s1, state_t const& s2) {
        return s1 == s2;
    }

    ////
    template <class ForwardWritableIterator, class StateT>
    static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        state_t & st = base_type::pstate(s);
        if (st.size < u) { st.pushA(c); }
        else if (st.size < (2 * u - 1)) { st.pushB(c); }
        else {
            st.push_xor_B(c);
            fixed_uint<w> A = st.A;
            fixed_uint<w> B = st.B;
            st.algorithm_.decode(st.A, st.B, st.prevA, st.prevB);
            st.prevA = A; st.prevB = B;
            unsigned int i;
            for (i = 0; i < u; ++i) base_type::customer_put(st.popA(), out, s);
            for (; i < 2 * u; ++i) base_type::customer_put(st.popB(), out, s);
            st.A = st.B = 0;
            assert(st.size == 0);
        }
    }

    template <class ForwardWritableIterator, class StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        state_t & st = base_type::pstate(s);
        if (st.size == 0) return;
        char temp = 0;
        st.error(temp, "Unexpected end of encoded data.");
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        state_t & st = base_type::pstate(s);
        if (st.size > u) { *result = st.popA(); return true; }
        if (st.size > 0) { *result = st.popB(); return true; }

        char buff[2*u];
        const char * pe = base_type::provider_get_range(in, s, buff, buff + 2*u);
        if ( (buff + 2*u) != pe ) {
            if (pe == buff) return false;
            char temp = 0;
            st.error(temp, "Unexpected end of encoded data.");
            return false;
        }

        fixed_uint<w> A, B;
        for (int j = u; --j >= 0;)
        {
            roll_left(A, 8); roll_left(B, 8);
            A += fixed_uint<w>(static_cast<unsigned char>(buff[j]));
            B += fixed_uint<w>(static_cast<unsigned char>(buff[j + u]));
        }

        st.A = A;
        st.B = B;
        st.size = 2 * u;

        /*
        st.A = st.B = 0;
        unsigned int i;
        for (i = 0; i < u; ++i)
        {
            optional<char> v = base_type::provider_get(in, s);
            if (v) {
                st.pushA(v.get());
            } else if (i == 0) {
                return v;
            } else {
                char temp = 0;
                st.error_(temp, st, "Unexpected end of encoded data.");
                return v;
            }
        }

        for (; i < 2*u; ++i)
        {
            optional<char> v = base_type::provider_get(in, s);
            if (v) {
                st.pushB(v.get());
            } else {
                char temp = 0;
                st.error_(temp, st, "Unexpected end of encoded data.");
                return v;
            }
        }
        fixed_uint<w> A = st.A;
        fixed_uint<w> B = st.B;
        */
        st.algorithm_.decode(st.A, st.B, st.prevA, st.prevB);
        st.prevA = A; st.prevB = B;
        *result = st.popA();
        return true;
    }
};

}
