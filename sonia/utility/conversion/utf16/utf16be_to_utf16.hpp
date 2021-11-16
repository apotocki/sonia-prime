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
class utf16be_to_utf16
    : public converter_facade<
          utf16be_to_utf16<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char16_t
        , utf16_state<ErrorHandlerT>
        , integral_state<char16_t, ErrorHandlerT>
    >
{
protected:
    typedef typename utf16be_to_utf16::facade_type base_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf16be_tag<HT> const&, utf16_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char val, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        if (st.cached == 1) {
            char16_t result = (static_cast<char16_t>(st.value) << 8) | static_cast<unsigned char>(val);
            if (result < UTF16_SUR_HIGH_START || result > UTF16_SUR_LOW_END) {
                if (result >= 0xFFFE) { // wrong bom, error
                    st.error(result, "Wrong UTF16 character.");
                } else if (result != 0xFEFF) { // bom
                    base_type::customer_put(result, out, s);
                }
            } else if (result <= UTF16_SUR_HIGH_END) { // found high surrogate
                st.value = result; ++st.cached; return;
            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)result << "'(low surrogate is not expected)";
                st.error(result, serr.str().c_str());
            }
        } else if (st.cached != 3) {
            st.value |= (val & 0xff) << (8*st.cached);
            st.cached++;
            return;    
        } else {
            char16_t trail = (static_cast<char16_t>(val) & 0xff) | (static_cast<char16_t>(st.value >> 8) & 0xff00);
            if (trail >= UTF16_SUR_LOW_START && trail <= UTF16_SUR_LOW_END) {
                base_type::customer_put(static_cast<char16_t>(st.value & 0xFFFF), out, s);
                base_type::customer_put(trail, out, s);
            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)trail << "'(low surrogate is expected)";
                st.error(trail, serr.str().c_str());
            }
        }
        st.reset();
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        // do nothing
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char16_t * result)
    {
        auto & st = base_type::gstate(s);
        if (st.get()) {
            *result = st.get();
            st.get() = 0;
            return true;
        }
        for (;;) {
            char ov, ov1;
            if (!base_type::provider_get(in, s, &ov)) return false;

            if (!base_type::provider_get(in, s, &ov1)) {
                st.error(ov, "Unexpected end of UTF16 data.");
                return false;
            }

            *result = static_cast<char16_t>(ov & 0xff) << 8 | (ov1 & 0xff);
            if (*result < UTF16_SUR_HIGH_START || *result > UTF16_SUR_LOW_END) {
                if (*result != 0xFEFF) { return true; } // sckip BOM
            } else if (*result <= UTF16_SUR_HIGH_END) { // found high surrogate
                if (!(base_type::provider_get(in, s, &ov)) || !(base_type::provider_get(in, s, &ov1))) {
                    st.error(result, "Unexpected end of UTF16 data.");
                    return false;
                }
                st.get() = static_cast<char16_t>(ov & 0xff) << 8 | (ov1 & 0xff);
                if (st.get() >= UTF16_SUR_LOW_START && st.get() <= UTF16_SUR_LOW_END) return true;

                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)st.get() << "'(low surrogate is expected)";
                st.error(st.get(), serr.str().c_str());

            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)*result << "'(low surrogate is not expected)";
                st.error(*result, serr.str().c_str());
            }
        }
    }
};

}
