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

#include "state.hpp"

#include "../utf16/constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf8_to_utf16
    : public converter_facade<
          utf8_to_utf16<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char16_t
        , utf8st<ErrorHandlerT>
        , integral_state<uint16_t, ErrorHandlerT>
    >
{
protected:
    using base_type = typename utf8_to_utf16::facade_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf8_tag<HT> const&, utf16_tag<ErrorHandlerT> const& t, StateT & st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char ec, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        char16_t c = static_cast<char16_t>(ec) & 0xff;

        if (!st.num) {
            if (c < 128) {
                base_type::customer_put(c, out, s);
                return;
            } else if ((c & 0xe0) == 0xc0) {
                if (c != 0xc1 && c != 0xc0)
                {
                    st.num++; st.data = c & 0x1f; return;
                }
            } else if ((c & 0xf0) == 0xe0) {
                st.num+=2; st.data = c & 0x0f; return;
            } else if ((c & 0xf8) == 0xf0 && c < 0xf5) {
                st.num+=3; st.data = c & 0x07; return;
            }
        } else if ((c & 0xc0) == 0x80) {
            st.data <<= 6;
            st.data |= (c & 0x3f);
            if (!--st.num) {
                if (st.data < UTF16_SUR_HIGH_START || st.data > UTF16_SUR_LOW_END) {
                    if (st.data <= 0xFFFF) {
                        //if (st.data != UTF16_BOM) {
                            base_type::customer_put(static_cast<char16_t>(st.data), out, s);
                        //}
                    } else {
                        char16_t lead = static_cast<char16_t>(UTF16_LEAD_OFFSET + (st.data >> 10));
                        char16_t trail = static_cast<char16_t>(UTF16_SUR_LOW_START + (st.data & 0x3FF));
                        base_type::customer_put(lead, out, s);
                        base_type::customer_put(trail, out, s);
                    }
                } else {
                    // bad decoded value here
                    std::ostringstream serr;
                    serr << "Wrong decoded value '0x" << std::hex << (uint32_t)st.data << "' (utf16 surrogate)";
                    st.error(c, serr.str().c_str());
                }
            }
            return;
        }
        st.error(ec, (std::string("Wrong utf8 character '") + ec + "'").c_str());
        st.num = 0;
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
        for (;;)
        {
            char ov;
            if (!base_type::provider_get(in, s, &ov)) return false;

            char32_t c = ov & 0xff;
            if (c < 128) {
                *result = static_cast<char16_t>(c);
                return true;
            } else if ((c & 0xe0) == 0xc0) {
                if (c != 0xc1 && c != 0xc0)
                {
                    if (!base_type::provider_get(in, s, &ov)) {
                        st.error(c, "Unexpected eof");
                        return false;
                    }
                    char32_t c1 = ov;
                    if ((c1 & 0xc0) == 0x80) {
                        c &= 0x1F; c <<= 6;
                        *result = static_cast<char16_t>(c | (c1 & 0x3F));
                        return true;
                    }
                    c = c1;
                }
            } else if ((c & 0xf0) == 0xe0) {
                char buff[2];
                if ((buff + 2) != base_type::provider_get_range(in, s, buff, buff + 2)) {
                    st.error(c, "Unexpected eof");
                    return false;
                }

                if ((buff[0] & 0xc0) == 0x80) {
                    if ((buff[1] & 0xc0) == 0x80) {
                        c &= 0x0F; c <<= 6; c |= (buff[0] & 0x3F);
                        c <<= 6; c |= (buff[1] & 0x3F);
                        
                        //if (c == UTF16_BOM) continue;
                        if (c < 0xD800 || c > 0xDFFF) { *result = static_cast<char16_t>(c); return true; }
                        // bad decoded value here
                        std::ostringstream serr;
                        serr << "Wrong decoded value '0x" << std::hex << (uint32_t)c << "' (utf16 surrogate)";
                        st.error(c, serr.str().c_str());
                        continue;
                    } else {
                        c = buff[1];
                    }
                } else {
                    c = buff[0];
                }
            } else if ((c & 0xf8) == 0xf0 && c < 0xf5) {
                char buff[3];
                if ( (buff + 3) != base_type::provider_get_range(in, s, buff, buff + 3)) {
                    st.error(c, "Unexpected eof");
                    return false;
                }

                if ((buff[0] & 0xc0) == 0x80) {
                    if ((buff[1] & 0xc0) == 0x80) {
                        if ((buff[2] & 0xc0) == 0x80) {
                            c &= 0x07; c <<= 6; c |= (buff[0] & 0x3F);
                            c <<= 6; c |= (buff[1] & 0x3F);
                            c <<= 6; c |= (buff[2] & 0x3F);
                            
                            st.get() = static_cast<boost::uint16_t>(UTF16_SUR_LOW_START + (c & 0x3FF));
                            *result = static_cast<char16_t>(UTF16_LEAD_OFFSET + (c >> 10));
                            return true;
                        } else {
                            c = buff[2];
                        }
                    } else {
                        c = buff[1];
                    }
                } else {
                    c = buff[0];
                }
            }
            st.error(c, (std::string("Wrong utf8 character '") + static_cast<char>(c) + "'").c_str());
        }
    }
};

}
