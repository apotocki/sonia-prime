/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <sstream>
#include <boost/assert.hpp>

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"

#include "state.hpp"
#include "constants.hpp"
#include "../utf16/constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT, bool OptV>
class utf16_to_utf7
    : public converter_facade<
          utf16_to_utf7<ProviderT, CustomerT, ErrorHandlerT, OptV>
        , CustomerT
        , ProviderT
        , char16_t
        , char
        , utf7_state<ErrorHandlerT>
        , utf7_state<ErrorHandlerT>
    >
{
protected:
    using base_type = typename utf16_to_utf7::facade_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf16_tag<HT> const&, utf7_tag<OptV, ErrorHandlerT> const& t, StateT& st) {
        st.clear(t.errh_);
    }
    
    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char16_t val, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        if (!st.wcached) {
            if (val < 128) {
                unsigned char ctype = utf7constants::utf7_matrix[val];
                if (ctype == 1 || (OptV && (ctype == 2))) {
                    if (!st.shifting) {
                        base_type::customer_put(static_cast<char>(val), out, s);
                        return;
                    }
                    if (st.bit > 0) {
                        base_type::customer_put(st.unshift(), out, s);
                    }
                    base_type::customer_put(UTF7SHIFT_OUT, out, s);
                    base_type::customer_put(static_cast<char>(val), out, s);
                    st.shifting = 0;
                    return;
                } else if (!st.shifting) {
                    base_type::customer_put(UTF7SHIFT_IN, out, s);
                    if (val == UTF7SHIFT_IN) {
                        base_type::customer_put(UTF7SHIFT_OUT, out, s);
                        return;
                    }
                }
            } else if (val < UTF16_SUR_HIGH_START || val > UTF16_SUR_LOW_END) {
                if (!st.shifting) { base_type::customer_put(UTF7SHIFT_IN, out, s); }
            } else if (val <= UTF16_SUR_HIGH_END) { // found high surrogate
                st.wcached = val;
                st.shifting = 1;
                return;
            } else {
                std::ostringstream serr;
                serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is not expected)";
                st.error(val, serr.str().c_str());
                return;
            }
            base_type::customer_put(st.put_wc_pop(val), out, s);
            while (st.bit >= 6) {
                base_type::customer_put(st.pop(), out, s);
            }
            st.shifting = 1;
        } else if (val >= UTF16_SUR_LOW_START && val <= UTF16_SUR_LOW_END) {
            base_type::customer_put(st.put_wc_pop(st.wcached), out, s);
            while (st.bit >= 6) {
                base_type::customer_put(st.pop(), out, s);
            }
            base_type::customer_put(st.put_wc_pop(val), out, s);
            while (st.bit >= 6) {
                base_type::customer_put(st.pop(), out, s);
            }
            st.wcached = 0;
        } else {
            std::ostringstream serr;
            serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is expected)";
            st.error(val, serr.str().c_str());
        }
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        auto & st = base_type::pstate(s);
        if (st.wcached) {
            st.error(st.wcached, "Not all input was converted before unshifting."); 
        }
        if (st.bit > 0) {
            base_type::customer_put(st.unshift(), out, s);
        }
        if (st.shifting) {
            base_type::customer_put(UTF7SHIFT_OUT, out, s);
        }
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        auto & st = base_type::gstate(s);
        
        char16_t val;

        for (;;)
        {
            switch (st.mode)
            {
            case 0: // no wcached to output, loading next input wchar
                if (!base_type::provider_get(in, s, &val)) {
                    if (st.shifting) {// unshifting if necessary
                        if (st.bit > 0) { 
                            st.mode = 1; // send UTF7SHIFT_OUT, then eof
                            *result = st.unshift();
                            return true;
                        }
                        st.shifting = 0;
                        st.mode = 2; // eof
                        *result = UTF7SHIFT_OUT;
                        return true;
                    }
                    st.mode = 2; // eof
                    return false; // eof
                }
                if (val < 128) {
                    unsigned char ctype = utf7constants::utf7_matrix[val];
                    if (ctype == 1 || (OptV && (ctype == 2))) {
                        if (!st.shifting) { *result = static_cast<char>(val); return true; } // remains the same mode
                        // b-mode
                        st.wcached = val;
                        if (st.bit > 0) {
                            st.mode = 3; // send UTF7SHIFT_OUT, then d-char
                            *result = st.unshift();
                            return true;
                        }
                        st.mode = 4; // send d-char
                        *result = UTF7SHIFT_OUT;
                        return true;
                    } else if (!st.shifting) {
                        if (val == UTF7SHIFT_IN) { // encoding '+' as '+-'
                            st.wcached = UTF7SHIFT_OUT;
                            st.mode = 4;
                        } else {
                            st.put_wc(val);
                            st.shifting = 1;
                            st.mode = 5; // b-mode
                        }
                        *result = UTF7SHIFT_IN;
                        return true;
                    } else {
                        st.mode = 5; // b-mode
                        *result = st.put_wc_pop(val);
                        return true;
                    }
                } else if (val < UTF16_SUR_HIGH_START || val > UTF16_SUR_LOW_END) {
                    if (!st.shifting) {
                        st.put_wc(val);
                        st.shifting = 1;
                        st.mode = 5; // b-mode
                        *result = UTF7SHIFT_IN;
                        return true;
                    }
                    st.mode = 5; // b-mode
                    *result = st.put_wc_pop(val);
                    return true;
                } else if (val <= UTF16_SUR_HIGH_END) { // found high surrogate
                    char16_t temp = val;
                    if (!base_type::provider_get(in, s, &val)) {
                        st.error(temp, "Unexpected EOF."); 
                        if (st.bit > 0) { 
                            st.mode = 1; // send UTF7SHIFT_OUT, then eof
                            *result = st.unshift();
                            return true;
                        }
                        st.shifting = 0;
                        st.mode = 2; // eof
                        *result = UTF7SHIFT_OUT;
                        return true;
                    }
                    if (val >= UTF16_SUR_LOW_START && val <= UTF16_SUR_LOW_END) {
                        st.wcached = val;
                        st.mode = 6; // b-mode + cached
                        if (!st.shifting) {
                            st.put_wc(temp);
                            st.shifting = 1;
                            *result = UTF7SHIFT_IN;
                            return true;
                        }
                        *result = st.put_wc_pop(temp);
                        return true;
                    } else {
                        std::ostringstream serr;
                        serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is expected)";
                        st.error(val, serr.str().c_str());
                    }
                } else {
                    std::ostringstream serr;
                    serr << "Wrong utf16 character '" << std::hex << (uint16_t)val << "'(low surrogate is not expected)";
                    st.error(val, serr.str().c_str());
                }
                break;
            case 1:
                BOOST_ASSERT(st.bit == 0);
                st.shifting = 0;
                st.mode = 2; // eof
                *result = UTF7SHIFT_OUT;
                return true;
            case 2:
                return false; // eof
            case 3:
                BOOST_ASSERT(st.bit == 0 && st.shifting);
                st.shifting = 0;
                st.mode = 4; // send d-char
                *result = UTF7SHIFT_OUT;
                return true;
            case 4: // writing direct char to output from wcached
                BOOST_ASSERT(!st.shifting);
                st.mode = 0; // to prime mode
                *result = static_cast<char>(st.wcached);
                return true;
            case 5:
                if (st.bit >= 6) { *result = st.pop(); return true; }
                st.mode = 0;
                break;
            case 6:
                if (st.bit >= 6) { *result = st.pop(); return true; }
                st.mode = 5;
                *result = st.put_wc_pop(st.wcached);
                return true;
            }
        }
    }
};

}
