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
class utf32be_to_utf32
    : public converter_facade<
          utf32be_to_utf32<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char32_t
        , utf32_state<ErrorHandlerT>
        , integral_state<void, ErrorHandlerT>
    >
{
protected:
    typedef typename utf32be_to_utf32::facade_type base_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf32be_tag<HT> const&, utf32_tag<ErrorHandlerT> const& t, StateT & st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char val, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        if (st.cached != 3) {
            st.value |= (static_cast<char32_t>(val)&0xff) << (24 - 8*st.cached);
            st.cached++;
            return;
        }
        char32_t result = st.value | (val & 0x000000ff);
        st.reset();
        if (result != 0xfeff) { // skip BOM
            base_type::customer_put(result, out, s);
        }
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        // do nothing
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char32_t * result)
    {
        for (;;) {
            char ov, ov1, ov2, ov3;
            if (!base_type::provider_get(in, s, &ov)) return false;

            if (!base_type::provider_get(in, s, &ov1) ||
                !base_type::provider_get(in, s, &ov2) ||
                !base_type::provider_get(in, s, &ov3))
            {
                base_type::gstate(s).error(result, "Unexpected end of UTF32 data.");
                return false;
            }

            *result = 
                (ov & 0x000000ff) << 24 |
                (ov1 & 0x000000ff) << 16 |
                (ov2 & 0x000000ff) << 8  |
                (ov3 & 0x000000ff);

            return true;
        }
    }
};

}
