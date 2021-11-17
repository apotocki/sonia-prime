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

#include "../utf32/utf32_tag.hpp"
#include "utf8_tag.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf32_to_utf8
    : public converter_facade<
          utf32_to_utf8<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char32_t
        , char
        , integral_state<void, ErrorHandlerT>
        , integral_state<char32_t, ErrorHandlerT>
    >
{
protected:
    using base_type = typename utf32_to_utf8::facade_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf32_tag<HT> const&, utf8_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char32_t uchar, ForwardWritableIterator& out, StateT& s)
    {
        if ( uchar <= 0x007F ) {
            base_type::customer_put(static_cast<char>(uchar), out, s);
        } else if ( uchar <= 0x07FF ) {
            char c1 = static_cast<char>((uchar & 0x3F) | 0x80);    uchar >>= 6;
            base_type::customer_put(static_cast<char>((uchar & 0x1F) | 0xC0), out, s);
            base_type::customer_put(c1, out, s);
        } else if ( uchar <= 0xFFFF ) {
            char c2 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
            char c1 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
            base_type::customer_put(static_cast<char>((uchar & 0x0F) | 0xE0), out, s);
            base_type::customer_put(c1, out, s);
            base_type::customer_put(c2, out, s);
        } else if( uchar <= 0x10FFFF ) {
            char c3 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
            char c2 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
            char c1 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
            base_type::customer_put(static_cast<char>((uchar & 0x07) | 0xF0), out, s);
            base_type::customer_put(c1, out, s);
            base_type::customer_put(c2, out, s);
            base_type::customer_put(c3, out, s);
        } else {
            auto & st = base_type::pstate(s);
            std::ostringstream serr;
            serr << "Wrong utf32 character '" << std::hex << (uint32_t)uchar << "'";
            st.error(uchar, serr.str().c_str());
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
        for (;;)
        {
            if (!st.get()) {
                char32_t uchar;
                if (!base_type::provider_get(in, s, &uchar)) return false;
                if ( uchar <= 0x7F ) {
                    *result = static_cast<char>(uchar);
                    return true;
                } else if ( uchar <= 0x07FF ) {
                    st.get() = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
                    *result = static_cast<char>((uchar & 0x1F) | 0xC0);
                    return true;
                } else if ( uchar <= 0xFFFF ) {
                    char32_t c2 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
                    char32_t c1 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
                    st.get() = c1 | (c2 << 8);
                    *result = static_cast<char>((uchar & 0x0F) | 0xE0);
                    return true;
                } else if ( uchar <= 0x10FFFF ) {
                    char32_t c3 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
                    char32_t c2 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
                    char32_t c1 = static_cast<char32_t>((uchar & 0x3F) | 0x80); uchar >>= 6;
                    st.get() = c1 | (c2 << 8) | (c3 << 16);
                    *result = static_cast<char>((uchar & 0x07) | 0xF0);
                    return true;
                }

                std::ostringstream serr;
                serr << "Wrong utf32 character '" << std::hex << (uint32_t)uchar << "'";
                st.error(uchar, serr.str().c_str());

            } else {
                *result = static_cast<char>(st.get() & 0x000000FF);
                st.get() >>= 8;
                return true;
            }
        }
    }
};

}
