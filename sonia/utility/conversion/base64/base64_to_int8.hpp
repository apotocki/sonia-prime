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

#include "base64_tag.hpp"
#include "constants.hpp"
#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, char PaddingCV, class ErrorHandlerT>
class base64_to_int8
    : public converter_facade<
          base64_to_int8<ProviderT, CustomerT, PaddingCV, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , bits_state_t<ErrorHandlerT>
        , bits_state_t<ErrorHandlerT>
    >
{
protected:
    using this_type = base64_to_int8;
    using base_type = typename base64_to_int8::facade_type;
    using state_t = bits_state_t<ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    inline static void do_clear(
        base64_tag<PaddingCV, ErrorHandlerT> const& parg,
        int8_tag const&,
        state_t& st)
    {
        st.clear(parg.errh_);
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
        if (c == PaddingCV) { // padding
            if (st.bit == 0) {
                st.error(c, unexpected_char_error_message(c).c_str(), out);
            } else {
                st.bit -= 2;
            }
            return;
        }

        unsigned char v = base64constants::base64_matrix[static_cast<unsigned char>(c)];
        
        if (v == 0xff) {
            st.error(c, unexpected_char_error_message(c).c_str(), out);
            return;
        } else {
            st.cache <<= 6;
            st.cache |= v;
        }

        if (st.bit >= 2) {
            st.bit -= 2;
            base_type::customer_put((st.cache >> st.bit) & 0xFF, out, s);
        } else {
            st.bit += 6;
        }
    }

    template <class ForwardWritableIterator, class StateT>
    static inline void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        // do nothing
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        char c;
        if (!base_type::provider_get(in, s, &c)) return false;

        state_t & st = base_type::gstate(s);
        BOOST_ASSERT(st.bit <= 6);

        if (c == PaddingCV) { // padding
            if (st.bit == 0) {
                st.error(c, unexpected_char_error_message(c).c_str(), in);
            } else if (st.bit == 4) {
                if (!base_type::provider_get(in, s, &c)) {
                    st.error(c, "Unexpected eof");
                } else if (c != PaddingCV) {
                    st.error(c, (unexpected_char_error_message(c) + ", expected padding character.").c_str(), in);
                }
            }
            st.bit = 0;
            return do_get(in, s, result);
        }
        
        unsigned char v = base64constants::base64_matrix[static_cast<unsigned char>(c)];
        
        if (v == 0xff) {
            st.error(c, unexpected_char_error_message(c).c_str(), in);
            return false;
        } else {
            st.cache <<= 6;
            st.cache |= v;
        }

        if (st.bit >= 2) {
            st.bit -= 2;
            *result = (st.cache >> st.bit) & 0xFF;
        } else {
            st.bit = 6;
            if (!base_type::provider_get(in, s, &c)) return false;
            unsigned char v = base64constants::base64_matrix[static_cast<unsigned char>(c)];
            // padding character is error in that place.
            if (v == 0xff) {
                st.error(c, unexpected_char_error_message(c).c_str(), in);
                return false;
            }
            st.cache <<= 6;
            st.cache |= v;
            st.bit = 4;
            *result = (st.cache >> st.bit) & 0xFF;
        }
        return true;
    }
};

}
