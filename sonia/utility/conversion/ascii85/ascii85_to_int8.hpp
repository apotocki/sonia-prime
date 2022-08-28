/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <string>

#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "ascii85_tag.hpp"
#include "states.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class ascii85_to_int8
    : public converter_facade<
          ascii85_to_int8<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , inherited_integral_state<ascii85_put_state, ErrorHandlerT>
        , inherited_integral_state<ascii85_dec_get_state, ErrorHandlerT>
    >
{
protected:
    using this_type = ascii85_to_int8;
    using base_type = typename ascii85_to_int8::facade_type;
    using pstate_type = inherited_integral_state<ascii85_put_state, ErrorHandlerT>;
    using gstate_type = inherited_integral_state<ascii85_dec_get_state, ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename StateT>
    inline static void do_clear(ascii85_tag<ErrorHandlerT> const& parg, int8_tag const&, StateT& st)
    {
        st.clear(parg.errh_);
        st.y_abbr = parg.y_abbr;
    }

    static std::string unexpected_char_error_message(char c)
    {
        std::ostringstream errss;
        errss << "Unexpected character code 0x" << std::hex << (uint32_t)(uint8_t)c;
        return errss.str();
    }

    constexpr static uint32_t powers[5] = { 85 * 85 * 85 * 85, 85 * 85 * 85, 85 * 85, 85, 1 };

    template <class ForwardWritableIterator, class StateT>
    static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r') return;
        
        auto& st = base_type::pstate(s);
        if (!st.count) {
            if (c == 'z') {
                for (int i = 0; i < 4; ++i) {
                    base_type::customer_put(0, out, s);
                }
                return;
            }
            if (c == 'y' && st.y_abbr) {
                for (int i = 0; i < 4; ++i) {
                    base_type::customer_put(' ', out, s);
                }
                return;
            }
        }
        if (c < '!' || c > 0x75) {
            st.error(c, unexpected_char_error_message(c).c_str(), out);
            return;
        }

        st.tuple += (c - '!') * powers[st.count++];

        if (st.count < 5) return;

        for (int i = 0; i < 4; ++i) {
            base_type::customer_put(st.tuple >> ((3 - i) * 8), out, s);
        }
        st.clear();
        return;
    }

    template <class ForwardWritableIterator, class StateT>
    static inline void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        auto st = base_type::pstate(s);
        if (st.count) {
            st.tuple += powers[st.count - 1];
        }
        for (int i = 1; i < st.count; ++i) {
            base_type::customer_put(st.tuple >> ((4 - i) * 8), out, s);
        }
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        auto & st = base_type::gstate(s);
        
        if (!st.count) {
            st.tuple = 0;
            do {
                char c;
                if (!base_type::provider_get(in, s, &c)) break;
                if (!st.count) {
                    if (c == 'z') {
                        st.count = 3;
                        *result = 0;
                        return true;
                    }
                    if (c == 'y' && st.y_abbr) {
                        st.tuple = 0x20202020;
                        st.count = 3;
                        *result = 0x20;
                        return true;
                    }
                }
                if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r') continue;
                if (c < '!' || c > 0x75) {
                    st.error(c, unexpected_char_error_message(c).c_str());
                    continue;
                }

                st.tuple += (c - '!') * powers[st.count++];
            } while (st.count < 5);

            if (st.count < 5) {
                st.tuple += powers[st.count - 1];
            }
            st.cidx = 1;
        }

        if (!st.count) return false;
        *result = st.tuple >> ((4 - st.cidx++) * 8);
        if (st.cidx >= st.count) {
            st.clear();
        }
        return true;
    }
};

}
