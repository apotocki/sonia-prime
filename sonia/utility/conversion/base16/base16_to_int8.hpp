/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <string>

#include <boost/assert.hpp>

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "base16_tag.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class base16_to_int8
    : public converter_facade<
          base16_to_int8<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , integral_state<char, ErrorHandlerT>
        , integral_state<void, ErrorHandlerT>
    >
{
protected:
    using this_type = base16_to_int8;
    using base_type = typename base16_to_int8::facade_type;
    using pstate_type = integral_state<char, ErrorHandlerT>;
    using gstate_type = integral_state<void, ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename StateT>
    inline static void do_clear(base16_tag<ErrorHandlerT> const& parg, int8_tag const&, StateT& st) {
        st.clear(parg.errh_);
    }

    template <class ForwardWritableIterator, class StateT>
    static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);

        char r;
        if (c >= '0' && c <= '9') {
            r = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            r = c - 'A' + 0x0a;
        } else if (c >= 'a' && c <= 'f') {
            r = c - 'a' + 0x0a;
        } else {
            st.error(c, (std::string("Bad input character '") + c + "'").c_str());
            return;
        }

        if (st.get() == 0) {
            st.get() = (r << 4) | 0x0f;
        } else {
            base_type::customer_put((st.get() & 0xF0 | r), out, s);
            st.get() = 0;
        }
    }

    template <class ForwardWritableIterator, class StateT>
    static inline void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        auto c = base_type::pstate(s).get();
        if (c) {
            base_type::pstate(s).error(c, "Unexpected eof");
        }
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        auto & st = base_type::gstate(s);

        char c0, r0;
        if (!base_type::provider_get(in, s, &c0)) return false;

        if (c0 >= '0' && c0 <= '9') {
            r0 = c0 - '0';
        } else if (c0 >= 'A' && c0 <= 'F') {
            r0 = c0 - 'A' + 0x0a;
        } else if (c0 >= 'a' && c0 <= 'f') {
            r0 = c0 - 'a' + 0x0a;
        } else {
            st.error(c0, (std::string("Bad input character '") + c0 + "'").c_str());
            return false;
        }

        char c1, r1;
        if (!base_type::provider_get(in, s, &c1)) {
            st.error(c0, "Unexpected eof");
            return false;
        }
        if (c1 >= '0' && c1 <= '9') {
            r1 = c1 - '0';
        } else if (c1 >= 'A' && c1 <= 'F') {
            r1 = c1 - 'A' + 0x0a;
        } else if (c1 >= 'a' && c1 <= 'f') {
            r1 = c1 - 'a' + 0x0a;
        } else {
            st.error(c1, (std::string("Bad input character '") + c1 + "'").c_str());
            return;
        }

        *result = (c0 << 4) | c1;
        return true;
    }
};

}
