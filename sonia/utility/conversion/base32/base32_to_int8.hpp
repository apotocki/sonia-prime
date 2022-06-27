/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <boost/assert.hpp>

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"

#include "base32_tag.hpp"
#include "constants.hpp"
#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class base32_to_int8
    : public converter_facade<
          base32_to_int8<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , ubits_state_t<ErrorHandlerT>
        , ubits_state_t<ErrorHandlerT>
    >
{
protected:
    using this_type = base32_to_int8;
    using base_type = typename base32_to_int8::facade_type;
    using state_t = ubits_state_t<ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    inline static void do_clear(
        base32_tag<ErrorHandlerT> const& parg,
        int8_tag const&,
        state_t& st)
    {
        st.alphabet = nullptr;
        st.matrix = parg.matrix;
        st.padding_character = parg.padding_character;
        st.clear(parg.errh);
    }

    static std::string unexpected_char_error_message(char c)
    {
        std::ostringstream errss;
        errss << "Unexpected character code 0x" << std::hex << (uint32_t)(uint8_t)c;
        return errss.str();
    }

    template <class ForwardWritableIterator, class StateT>
    static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        if (c == st.padding_character) { // padding
            if (st.bit % 8 == 0) {
                st.error(c, unexpected_char_error_message(c).c_str(), out);
            } else {
                st.bit += 5;
            }
            return;
        }

        unsigned char v = st.matrix[static_cast<unsigned char>(c)];
        
        if (v == 0xff) {
            st.error(c, unexpected_char_error_message(c).c_str(), out);
            return;
        } else {
            st.cache <<= 5;
            st.cache |= v;
        }

        if (st.bit > 2) {
            st.bit -= 3;
            base_type::customer_put((st.cache >> st.bit) & 0xFF, out, s);
        } else {
            st.bit += 5;
        }
    }

    template <class ForwardWritableIterator, class StateT>
    static inline void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        auto& st = base_type::pstate(s);
        if (st.bit % 8 != 0) {
            st.error(0, "EOF, but the padding character was expected.", out);
        }
        // do nothing
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        char c;
        if (!base_type::provider_get(in, s, &c)) return false;

        state_t & st = base_type::gstate(s);
        BOOST_ASSERT(st.bit <= 6);

        if (c == st.padding_character) { // padding
            if (st.bit % 8 == 0) {
                st.error(c, unexpected_char_error_message(c).c_str(), in);
            } else {
                for (;;) {
                    st.bit += 5;
                    if ((st.bit % 8) == 0) break;
                    if (!base_type::provider_get(in, s, &c)) {
                        st.error(c, "EOF, but the padding character was expected.", in);
                    } else if (c != st.padding_character) {
                        st.error(c, (unexpected_char_error_message(c) + ", but the padding character was expected.").c_str(), in);
                    }
                }
            }
            return false;
        }
        
        unsigned char v = st.matrix[static_cast<unsigned char>(c)];
        
        if (v == 0xff) {
            st.error(c, unexpected_char_error_message(c).c_str(), in);
            return false;
        } else {
            st.cache <<= 5;
            st.cache |= v;
        }

        if (st.bit > 2) {
            st.bit -= 3;
            *result = (st.cache >> st.bit) & 0xFF;
        } else {
            st.bit += 2; // +=5 -> 5, 6, 7; -=3 -> 2, 3, 4
            if (!base_type::provider_get(in, s, &c)) return false;
            unsigned char v = st.matrix[static_cast<unsigned char>(c)];
            // padding character is error in that place.
            if (v == 0xff) {
                st.error(c, unexpected_char_error_message(c).c_str(), in);
                return false;
            }
            st.cache <<= 5;
            st.cache |= v;
            *result = (st.cache >> st.bit) & 0xFF;
        }
        return true;
    }
};

}
