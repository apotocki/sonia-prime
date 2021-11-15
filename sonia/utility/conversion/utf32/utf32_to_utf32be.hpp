/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf32_to_utf32be
    : public converter_facade<
          utf32_to_utf32be<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char32_t
        , char
        , integral_state<void, ErrorHandlerT>
        , utf32_state<ErrorHandlerT>
    >
{
protected:
    typedef typename utf32_to_utf32be::facade_type base_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf32_tag<HT> const&, utf32be_tag<ErrorHandlerT> const& t, StateT & st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char32_t val, ForwardWritableIterator& out, StateT& s)
    {
        base_type::customer_put(static_cast<char>((val >> 24) & 0x000000FF), out, s);
        base_type::customer_put(static_cast<char>((val >> 16) & 0x000000FF), out, s);
        base_type::customer_put(static_cast<char>((val >> 8) & 0x000000FF), out, s);
        base_type::customer_put(static_cast<char>(val & 0x00000FF), out, s);
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        // do nothing
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        auto & st = base_type::gstate(s);
        for (;;) {
            if (st.cached) {
                --st.cached;
                *result = static_cast<char>(st.value >> (8 * st.cached));
                return true;
            }

            char32_t tmp;
            if (!base_type::provider_get(in, s, &tmp)) return false; // eof
            st.value = tmp;
            st.cached = 3;
            *result = static_cast<char>(tmp >> 24);
            return true;
        }
    }
};

}
