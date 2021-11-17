/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <sstream>

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "state.hpp"
#include "constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf16_to_utf16le
    : public converter_facade<
          utf16_to_utf16le<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char16_t
        , char
        , integral_state<char16_t, ErrorHandlerT>
        , utf16_state<ErrorHandlerT>
    >
{
protected:
    typedef typename utf16_to_utf16le::facade_type base_type;
    typedef integral_state<char16_t, ErrorHandlerT> state16_t;
    typedef integral_state<char, ErrorHandlerT> state8_t;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf16_tag<HT> const&, utf16le_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char16_t val, ForwardWritableIterator& out, StateT& s)
    {
        state16_t & st = base_type::pstate(s);
        if (!st.get())
        {
            if (val < UTF16_SUR_HIGH_START || val > UTF16_SUR_LOW_END) {
                base_type::customer_put(static_cast<char>(val & 0x00FF), out, s);
                base_type::customer_put(static_cast<char>((val >> 8) & 0x00FF), out, s);
            } else if (val <= UTF16_SUR_HIGH_END) { // found high surrogate
                st.get() = val;
            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is not expected)";
                st.error(val, serr.str().c_str());
            }
        } else {
            if (val >= UTF16_SUR_LOW_START && val <= UTF16_SUR_LOW_END) {
                base_type::customer_put(static_cast<char>(st.get() & 0x00FF), out, s);
                base_type::customer_put(static_cast<char>((st.get() >> 8) & 0x00FF), out, s);
                base_type::customer_put(static_cast<char>(val & 0x00FF), out, s);
                base_type::customer_put(static_cast<char>((val >> 8) & 0x00FF), out, s);
                st.get() = 0;
            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is expected)";
                st.error(val, serr.str().c_str());
            }
        }
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
            char16_t ov;
            if (!base_type::provider_get(in, s, &ov)) return false; // eof

            char32_t tmp_result = ov;
            if (tmp_result < UTF16_SUR_HIGH_START || tmp_result > UTF16_SUR_LOW_END) {
                st.cached = 1;
                st.value = (tmp_result >> 8) & 0x00FF;
                *result = static_cast<char>(tmp_result & 0x00FF);
                return true;
            } else if (tmp_result <= UTF16_SUR_HIGH_END) { // found high surrogate
                if (!base_type::provider_get(in, s, &ov)) {
                    st.error(tmp_result, "Unexpected end of UTF16 data.");
                    return false;
                }
                char32_t trail = ov;
                if (trail >= UTF16_SUR_LOW_START || trail <= UTF16_SUR_LOW_END) {
                    st.cached = 3;
                    st.value = ((tmp_result << 8) & 0x00FF0000) | ((trail >> 8) | ((trail & 0xff) << 8));
                    *result = static_cast<char>(tmp_result & 0x00FF);
                    return true;
                } else {
                    st.error(trail, "Low surrogate is expected.");
                }
            } else {
                st.error(result, "Low surrogate is not expected.");
            }
        }
    }
};

}
