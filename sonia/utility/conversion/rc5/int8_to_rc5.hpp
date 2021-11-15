/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <boost/assert.hpp>
#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"

#include "rc5_tag.hpp"
#include "rc5_algo.hpp"
#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
class int8_to_rc5
    : public converter_facade<
          int8_to_rc5<ProviderT, CustomerT, w, r, b, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , rc5_state<w, r, b, ErrorHandlerT>
        , rc5_state<w, r, b, ErrorHandlerT>
    >
{
    static const unsigned char u = w/8;

protected:
    using base_type = typename int8_to_rc5::facade_type;
    using state_t = rc5_state<w, r, b, ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    inline static void do_clear(int8_tag const&, rc5_tag<w, r, b, ErrorHandlerT> const& arg, state_t & st)
    {
        st.clear(arg);
    }

    ////
    template <class ForwardWritableIterator, class StateT>
    static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        state_t & st = base_type::pstate(s);
        if (st.size < u) { st.push_xor_A(c); }
        else if (st.size < (2 * u - 1)) { st.push_xor_B(c); }
        else {
            st.push_xor_B(c);
            st.algorithm_.encode(st.A, st.B, 0, 0);
            unsigned int i;
            for (i = 0; i < u; ++i) base_type::customer_put(st.popA(), out, s);
            for (; i < 2 * u; ++i) base_type::customer_put(st.popB(), out, s);
            BOOST_ASSERT(st.size == 0);
        }
    }

    template <class ForwardWritableIterator, class StateT>
    static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        state_t & st = base_type::pstate(s);
        if (st.size == 0) return;
        unsigned int i = st.size;
        for (; i < u; ++i) st.push_xor_A(st.padding_c_);
        for (; i < 2 * u; ++i) st.push_xor_B(st.padding_c_);
        st.algorithm_.encode(st.A, st.B, 0, 0);
        for (i = 0; i < u; ++i) base_type::customer_put(st.popA(), out, s);
        for (; i < 2 * u; ++i) base_type::customer_put(st.popB(), out, s);
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        state_t & st = base_type::pstate(s);
        if (st.size > u) { *result = st.popA(); return true; }
        if (st.size > 0) { *result = st.popB(); return true; }

        fixed_uint<w> iv0 = st.A;
        fixed_uint<w> iv1 = st.B;
        st.A = st.B = 0;

        unsigned int i;
        for (i = 0; i < u; ++i)
        {
            char v;
            if (base_type::provider_get(in, s, &v)) {
                st.pushA(v);
            } else {
                if (i == 0) return false;
                for (; i < u; ++i) { st.pushA(st.padding_c_); }
                break;
            }
        }

        for (; i < 2*u; ++i)
        {
            char v;
            if (base_type::provider_get(in, s, &v)) {
                st.pushB(v);
            } else {
                for (; i < 2*u; ++i) { st.pushB(st.padding_c_); }
                break;
            }
        }
        st.algorithm_.encode(st.A, st.B, iv0, iv1);
        *result = st.popA();
        return true;
    }
};

}
