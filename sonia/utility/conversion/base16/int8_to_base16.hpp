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
#include "../integral_state.hpp"

#include "base16_tag.hpp"
#include "constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class int8_to_base16
    : public converter_facade<
          int8_to_base16<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , integral_state<void, ErrorHandlerT>
        , integral_state<uint8_t, ErrorHandlerT>
    >
{
protected:
    using base_type = typename int8_to_base16::facade_type;
    using pstate_type = integral_state<void, ErrorHandlerT>;
    using gstate_type = integral_state<uint8_t, ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename StateT>
    inline static void do_clear(int8_tag const&, base16_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear(t.errh_);
    }

    ////
    template <class ForwardWritableIterator, class StateT>
    BOOST_FORCEINLINE static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        base_type::customer_put(base16constants::base16_alphabet[(c >> 4) & 0x0f], out, s);
        base_type::customer_put(base16constants::base16_alphabet[c & 0x0f], out, s);
    }

    template <class ForwardWritableIterator, class StateT>
    static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        // do nothing
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char* result)
    {
        gstate_type & st = base_type::gstate(s);
        if (st.get() != 0) {
            *result = base16constants::base16_alphabet[st.get() & 0x0f];
            st.get() = 0;
            return true;
        }

        char v;
        if (!base_type::provider_get(in, s, &v)) return false;
        *result = base16constants::base16_alphabet[(v >> 4) & 0x0f];
        st.get() = v | 0x0f; // 0xf is just a 'not empty' flag

        return true;
    }
};

}
