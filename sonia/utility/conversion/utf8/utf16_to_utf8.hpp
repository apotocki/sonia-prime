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

#include "../utf16/constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf16_to_utf8
    : public converter_facade<
          utf16_to_utf8<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char16_t
        , char
        , integral_state<char16_t, ErrorHandlerT>
        , integral_state<char32_t, ErrorHandlerT>>
{
protected:
    using base_type = typename utf16_to_utf8::facade_type ;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf16_tag<HT> const&, utf8_tag<ErrorHandlerT> const& t, StateT & st) {
        st.clear(t.errh_);
    }

    // uchar <= 0xFFFF
    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_put1(char32_t uchar, ForwardWritableIterator& out, StateT& s)
    {
        if ( uchar <= 0x007F ) {
            base_type::customer_put(static_cast<char>(uchar), out, s);
        } else if ( uchar <= 0x07FF ) {
            char c1 = static_cast<char>((uchar & 0x3F) | 0x80);    uchar >>= 6;
            base_type::customer_put(static_cast<char>((uchar & 0x1F) | 0xC0), out, s);
            base_type::customer_put(c1, out, s);
        } else {
            char c2 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
            char c1 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
            base_type::customer_put(static_cast<char>((uchar & 0x0F) | 0xE0), out, s);
            base_type::customer_put(c1, out, s);
            base_type::customer_put(c2, out, s);
        }
    }

    // 0xFFFF < uchar <= 0x10FFFF
    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_put2(char32_t uchar, ForwardWritableIterator& out, StateT& s)
    {
        char c3 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        char c2 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        char c1 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        base_type::customer_put(static_cast<char>((uchar & 0x07) | 0xF0), out, s);
        base_type::customer_put(c1, out, s);
        base_type::customer_put(c2, out, s);
        base_type::customer_put(c3, out, s);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char16_t val, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        if (!st.get())
        {
            if (val < UTF16_SUR_HIGH_START || val > UTF16_SUR_LOW_END) {
                do_put1(val, out, s);
            } else if (val <= UTF16_SUR_HIGH_END) { // found high surrogate
                st.get() = static_cast<uint_least16_t>(val);
            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is not expected)";
                st.error(val, serr.str().c_str());
            }
        } else {
            if (val >= UTF16_SUR_LOW_START && val <= UTF16_SUR_LOW_END) {
                char32_t lead = st.get();
                char32_t trail = val;
                do_put2((lead << 10) + trail + UTF16_SURROGATE_OFFSET, out, s);
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

    // uchar <= 0xFFFF
    inline static bool do_get1(char32_t uchar, char32_t& st, char * result)
    {
        if ( uchar <= 0x7F ) {
            *result = static_cast<char>(uchar);
        } else if ( uchar <= 0x07FF ) {
            st = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
            *result = static_cast<char>((uchar & 0x1F) | 0xC0);
        } else {
            char32_t c2 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
            char32_t c1 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
            st = c1 | (c2 << 8);
            *result = static_cast<char>((uchar & 0x0F) | 0xE0);
        }
        return true;
    }

    // 0xFFFF < uchar <= 0x10FFFF
    inline static bool do_get2(char32_t uchar, char32_t& st, char * result)
    {
        char32_t c3 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
        char32_t c2 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
        char32_t c1 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
        st = c1 | (c2 << 8) | (c3 << 16);
        *result = static_cast<char>((uchar & 0x07) | 0xF0);
        return true;
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        auto & st = base_type::gstate(s);
        for (;;)
        {
            if (!st.get()) {
                char16_t ov, ov1;
                if (!base_type::provider_get(in, s, &ov)) return false;
                if (ov < UTF16_SUR_HIGH_START || ov > UTF16_SUR_LOW_END) {
                    return do_get1(ov, st.get(), result);
                } else if (ov <= UTF16_SUR_HIGH_END) { // found high surrogate
                    if (!base_type::provider_get(in, s, &ov1)) {
                        st.error(result, "Unexpected end of UTF16 data.");
                        return false;
                    }
                    if (ov1 >= UTF16_SUR_LOW_START || ov1 <= UTF16_SUR_LOW_END) {
                        return do_get2((((char32_t)ov) << 10) + (char32_t)ov1 + UTF16_SURROGATE_OFFSET, st.get(), result);
                    } else {
                        st.error(ov1, "Low surrogate is expected.");
                    }
                } else {
                    st.error(ov, "Low surrogate is not expected.");
                }
            } else {
                *result = static_cast<char>(st.get() & 0xFF);
                st.get() >>= 8;
                return true;
            }
        }
    }
};

}
