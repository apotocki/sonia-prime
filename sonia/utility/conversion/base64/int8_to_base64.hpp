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

#include "base64_tag.hpp"
#include "constants.hpp"
#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, char PaddingCV, class ErrorHandlerT>
class int8_to_base64
    : public converter_facade<
          int8_to_base64<ProviderT, CustomerT, PaddingCV, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , bits_state_t<ErrorHandlerT>
        , bits_state_t<ErrorHandlerT>
    >
{
protected:
    using base_type = typename int8_to_base64::facade_type;
    using state_t = bits_state_t<ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    inline static void do_clear(
        int8_tag const&,
        base64_tag<PaddingCV, ErrorHandlerT> const& carg,
        state_t& st)
    {
        st.clear(carg.errh_);
    }

    inline static bool do_equal_states(state_t const& s1, state_t const& s2) { return s1 == s2; }

    // aux
    BOOST_FORCEINLINE static char put_c(char c, state_t & st)
    {
        BOOST_ASSERT(st.bit <= 4);
        st.cache <<= 8;
        st.cache |= static_cast<unsigned char>(c);
        st.bit += 2;
        return base64constants::base64_alphabet[(st.cache >> st.bit) & 0x3F];
    }

    ////
    template <class ForwardWritableIterator, class StateT>
    BOOST_FORCEINLINE static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        state_t & st = base_type::pstate(s);
        base_type::customer_put(put_c(c, st), out, s);
        
        if (st.bit == 6) {
            st.bit = 0;
            base_type::customer_put(base64constants::base64_alphabet[st.cache & 0x3F], out, s);
        }
    }

    template <class ForwardWritableIterator, class StateT>
    static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        state_t & st = base_type::pstate(s);
        BOOST_ASSERT(st.bit < 5);
        if (st.bit == 4) {
            st.cache <<= 8;
            base_type::customer_put(base64constants::base64_alphabet[(st.cache >> 6) & 0x3F], out, s);
            base_type::customer_put(PaddingCV, out, s);
            st.bit = 0;
        } else if (st.bit == 2) {
            st.cache <<= 8;
            base_type::customer_put(base64constants::base64_alphabet[(st.cache >> 4) & 0x3F], out, s);
            base_type::customer_put(PaddingCV, out, s);
            base_type::customer_put(PaddingCV, out, s);
            st.bit = 0;
        }
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char* result)
    {
        state_t & st = base_type::gstate(s);
        BOOST_ASSERT(st.bit <= 6);

        if (st.bit == 6) { // getting cached
            st.bit = 0;
            *result = base64constants::base64_alphabet[(st.cache >> st.bit) & 0x3F];
            return true;
        }

        char v;

        if (!base_type::provider_get(in, s, &v)) { // unshifting
            if (st.bit > 0) {
                st.cache <<= 8;
                st.bit -= 6;
                *result = base64constants::base64_alphabet[(st.cache >> (st.bit + 8)) & 0x3F];
                return true;
            } else if (st.bit < 0) { // padding
                st.bit += 2;
                *result = PaddingCV;
                return true;
            }
            return false; // eof
        }
        *result = put_c(v, st);
        return true;
    }
};

}
