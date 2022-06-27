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

#include "base32_tag.hpp"
#include "constants.hpp"
#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class int8_to_base32
    : public converter_facade<
          int8_to_base32<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , ubits_state_t<ErrorHandlerT>
        , ubits_state_t<ErrorHandlerT>
    >
{
protected:
    using base_type = typename int8_to_base32::facade_type;
    using state_t = ubits_state_t<ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    inline static void do_clear(
        int8_tag const&,
        base32_tag<ErrorHandlerT> const& carg,
        state_t& st)
    {
        st.alphabet = carg.alphabet;
        st.matrix = nullptr;
        st.padding_character = carg.padding_character;
        st.clear(carg.errh);
    }

    inline static bool do_equal_states(state_t const& s1, state_t const& s2) { return s1 == s2; }

    // aux
    BOOST_FORCEINLINE static char put_c(char c, state_t & st)
    {
        BOOST_ASSERT(st.bit <= 7);
        st.cache <<= 8;
        st.cache |= static_cast<unsigned char>(c);
        st.bit += 3;
        return st.alphabet[(st.cache >> st.bit) & 0x1F];
    }

    ////
    template <class ForwardWritableIterator, class StateT>
    BOOST_FORCEINLINE static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        state_t & st = base_type::pstate(s);
        base_type::customer_put(put_c(c, st), out, s);
        
        if (st.bit >= 5) {
            st.bit -= 5;
            base_type::customer_put(st.alphabet[(st.cache >> st.bit) & 0x1F], out, s);
        }
    }

    template <class ForwardWritableIterator, class StateT>
    static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        state_t & st = base_type::pstate(s);
        BOOST_ASSERT(st.bit <= 7);
        if (!st.bit) return;
        base_type::customer_put(put_c(0, st), out, s);
        while (st.bit) {
            if (st.bit >= 5) {
                base_type::customer_put(st.padding_character, out, s);
                st.bit -= 5;
            } else {
                st.bit += 8;
            }
        }
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char* result)
    {
        state_t & st = base_type::gstate(s);
        //BOOST_ASSERT(st.bit <= 6);

        // 0, 3, 6->1, 4, 7->2, 5->0
        if (st.bit >= 5 && st.bit < 8) { // getting cached
            st.bit -= 5;
            *result = st.alphabet[(st.cache >> st.bit) & 0x1F];
            return true;
        }

        char v;

        if (!base_type::provider_get(in, s, &v)) { // unshifting
            if (st.bit && st.bit < 5) {
                st.cache <<= 8;
                st.bit += 3;
                *result = st.alphabet[(st.cache >> st.bit) & 0x1F];
                // 4 ->(4) 8 + 3
                // 7 ->(3) 8 + 2
                // 5 ->(1) 8 + 0
                // 6 ->(6) 8 + 5
                // 100 -> 011  -1 -> 011
                // 111 -> 010  -1 -> 110
                // 101 -> 000  -1 -> 100
                // 110 -> 101  -1 -> 101
                // st.bit = 8 + (st.bit - 1) - ((st.bit & 1) << 2);
                st.bit = 7 + st.bit - ((st.bit & 1) << 2);
                /*
                if (st.bit == 4) st.bit = 11;
                else if (st.bit == 7) st.bit = 10;
                else if (st.bit == 5) st.bit = 8;
                else {
                    BOOST_ASSERT(st.bit == 6);
                    st.bit = 13;
                }
                */
                return true;
            } else if (st.bit - 8 >= 0) { // padding
                BOOST_ASSERT(st.bit > 7);
                *result = st.padding_character;
                if (st.bit == 8) st.bit = 0;
                else --st.bit;
                return true;
            }
            return false; // eof
        }
        *result = put_c(v, st);
        return true;
    }
};

}
