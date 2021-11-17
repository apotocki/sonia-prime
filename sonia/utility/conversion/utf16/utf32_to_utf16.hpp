/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <sstream>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"

#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf32_to_utf16
    : public converter_facade<
          utf32_to_utf16<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char32_t
        , char16_t
        , integral_state<void, ErrorHandlerT>
        , integral_state<uint16_t, ErrorHandlerT>
    >
{
protected:
    typedef typename utf32_to_utf16::facade_type base_type;
    typedef integral_state<void, ErrorHandlerT> pstate_type;
    typedef integral_state<uint16_t, ErrorHandlerT> gstate_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf32_tag<HT> const&, utf16_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char32_t uchar, ForwardWritableIterator& out, StateT& s) 
    {
        pstate_type & st = base_type::pstate(s);
        if (uchar < UTF16_SUR_HIGH_START || (uchar > UTF16_SUR_LOW_END && uchar <= 0xffff)) { 
            base_type::customer_put(static_cast<uint16_t>(uchar), out, s);
        } else {
            if (uchar >= 0x00010000UL && uchar <= 0x0010ffff) {
                uint16_t lead = static_cast<uint16_t>(UTF16_LEAD_OFFSET + (uchar >> 10));
                uint16_t trail = static_cast<uint16_t>(UTF16_SUR_LOW_START + (uchar & 0x3FF));
                base_type::customer_put(lead, out, s);
                base_type::customer_put(trail, out, s);
            } else {
                std::stringstream serr;
                serr << "Wrong utf32 character '" << std::hex << (uint32_t)uchar << "'";
                st.error(uchar, serr.str().c_str());
            }
        }
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        // do nothing
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char16_t * result)
    {
        gstate_type & st = base_type::gstate(s);
        for (;;)
        {
            if (!st.get()) {
                char32_t uchar;
                if (!base_type::provider_get(in, s, &uchar)) return false;
                if (uchar < UTF16_SUR_HIGH_START || (uchar > UTF16_SUR_LOW_END && uchar <= 0xffff)) { 
                    *result = static_cast<char16_t>(uchar);
                    return true;
                } else if (uchar <= 0x0010ffff) {
                    char16_t lead = static_cast<char16_t>(UTF16_LEAD_OFFSET + (uchar >> 10));
                    st.get() = static_cast<char16_t>(UTF16_SUR_LOW_START + (uchar & 0x3FF));
                    *result = lead;
                    return true;
                } else {
                    std::ostringstream serr;
                    serr << "Wrong utf32 character '" << std::hex << (uint32_t)uchar << "'";
                    st.error(uchar, serr.str().c_str());
                    return false;
                }
            } else {
                *result = st.get();
                st.get() = 0;
                return true;
            }
        }
    }
};

}
