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

#include "utf7_tag.hpp"
#include "state.hpp"
#include "constants.hpp"
#include "../utf16/constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT, bool OptV>
class utf7_to_utf16
    : public converter_facade<
          utf7_to_utf16<ProviderT, CustomerT, ErrorHandlerT, OptV>
        , CustomerT
        , ProviderT
        , char
        , char16_t
        , utf7_state<ErrorHandlerT>
        , utf7_state<ErrorHandlerT>
    >
{
protected:
    typedef typename utf7_to_utf16::facade_type base_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf7_tag<OptV, HT> const&, utf16_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char val, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        switch (st.mode)
        {
        case 0:
            if (val != UTF7SHIFT_IN) {
                unsigned char ctype = utf7constants::utf7_matrix[(int)val];
                if (ctype == 1 || (OptV && (ctype == 2))) {
                    base_type::customer_put(static_cast<char16_t>(val), out, s);
                } else {
                    st.error(val, (std::string("Bad input character '") + val + "'").c_str());
                }
                return;
            }
            st.mode = 1;
            return;
        case 1:
            if (val != UTF7SHIFT_OUT) {
                unsigned char v = base64constants::base64_matrix[static_cast<unsigned char>(val)];
                if (v == 0xff) {
                    st.error(val, (std::string("Bad input character '") + val + "'").c_str());
                    return;
                }
                st.put_6b(v);
                st.mode = 2;
            } else {
                base_type::customer_put(static_cast<char16_t>(UTF7SHIFT_IN), out, s);
                st.mode = 0;
            }
            return;
        case 2: // getting first char16_t character
            unsigned char v = base64constants::base64_matrix[static_cast<unsigned char>(val)];
            if (v == 0xff) {
                if (val == UTF7SHIFT_OUT && st.bit < 6) { // getting unshifting character
                    st.bit = 0;
                    st.mode = 0;
                    return;
                }
                st.error(val, (std::string("Bad input character '") + val + "'").c_str());
                return;
            }
            st.put_6b(v);
            if (st.bit >= 16) {
                char16_t r = st.pop_wc();
                base_type::customer_put(r, out, s);
            }
            return;
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
        char val;
        unsigned char v;
        auto & st = base_type::pstate(s);
        for (;;)
        {
            switch (st.mode)
            {
            case 0:
                if (!base_type::provider_get(in, s, &val)) return false;
                if (val != UTF7SHIFT_IN) {
                    unsigned char ctype = utf7constants::utf7_matrix[(int)val];
                    if (ctype == 1 || (OptV && (ctype == 2))) {
                        *result = val;
                        return true;
                    }
                    st.error(val, (std::string("Bad input character '") + val + "'").c_str());
                    continue;
                }
                if (!base_type::provider_get(in, s, &val)) {
                    st.error(val, "Unexpected eof");
                    return false;
                }
                if (val == UTF7SHIFT_OUT) { *result = UTF7SHIFT_IN; return true; }
                st.mode = 1; // bmode
                v = base64constants::base64_matrix[static_cast<unsigned char>(val)];
                if (v == 0xff) {
                    st.error(val, (std::string("Bad input character '") + val + "'").c_str());
                } else {
                    st.put_6b(v);
                }
            case 1:
                for (;;)
                {
                    if (!base_type::provider_get(in, s, &val)) {
                        st.error(val, "Unexpected eof");
                        continue;
                    }
                    v = base64constants::base64_matrix[static_cast<unsigned char>(val)];
                    if (v != 0xff) {
                        st.put_6b(v);
                        if (st.bit >= 16) {
                            *result = st.pop_wc();
                            return true;
                        }
                    } else if (val == UTF7SHIFT_OUT && st.bit < 6) {
                        st.bit = 0;
                        st.mode = 0;
                        break;
                    } else {
                        st.error(val, (std::string("Bad input character '") + val + "'").c_str());
                    }
                }
            }
        }
    }
};

}
