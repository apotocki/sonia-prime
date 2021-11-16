/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>
#include <sstream>

#include "sonia/type_traits.hpp"

#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf16_to_utf32
    : public converter_facade<
          utf16_to_utf32<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char16_t
        , char32_t
        , integral_state<char16_t, ErrorHandlerT>
        , integral_state<void, ErrorHandlerT>
    >
{
protected:
    typedef typename utf16_to_utf32::facade_type base_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

    typedef integral_state<char16_t, ErrorHandlerT> state16_t;
    typedef integral_state<char16_t, ErrorHandlerT> state32_t;

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf16_tag<HT> const&, utf32_tag<ErrorHandlerT> const& t, StateT & st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char16_t val, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        if (!st.get())
        {
            if (val < UTF16_SUR_HIGH_START || val > UTF16_SUR_LOW_END) {
                base_type::customer_put(static_cast<char32_t>(val), out, s);
            } else if (val <= UTF16_SUR_HIGH_END) { // found high surrogate
                st.get() = val;
            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is not expected)";
                st.error(val, serr.str().c_str());
            }
        } else {
            if (val >= UTF16_SUR_LOW_START && val <= UTF16_SUR_LOW_END) {
                char32_t lead = st.get();
                char32_t trail = val;
                base_type::customer_put(static_cast<char32_t>((lead << 10) + trail + UTF16_SURROGATE_OFFSET), out, s);
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
    static bool do_get(ForwardReadableIterator& in, StateT& s, char32_t * result)
    {
        auto & st = base_type::gstate(s);
        for (;;) {
            char16_t ov, trail;
            if (!base_type::provider_get(in, s, &ov)) return false;

            if (ov < UTF16_SUR_HIGH_START || ov > UTF16_SUR_LOW_END) {
                *result = ov;
                return true;
            } else if (ov <= UTF16_SUR_HIGH_END) { // found high surrogate
                if (!base_type::provider_get(in, s, &trail)) {
                    st.error(ov, "Unexpected end of UTF16 data.");
                    return false;
                }
                if (trail >= UTF16_SUR_LOW_START || trail <= UTF16_SUR_LOW_END) {
                    *result = (((char32_t)ov) << 10) + (char32_t)trail + UTF16_SURROGATE_OFFSET;
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
