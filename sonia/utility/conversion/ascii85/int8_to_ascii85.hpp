/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"

#include "ascii85_tag.hpp"
#include "states.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class int8_to_ascii85
    : public converter_facade<
          int8_to_ascii85<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , ascii85_put_state
        , ascii85_enc_get_state
    >
{
protected:
    using base_type = typename int8_to_ascii85::facade_type;
    using pstate_type = ascii85_put_state;
    using gstate_type = ascii85_enc_get_state;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename StateT>
    inline static void do_clear(int8_tag const&, ascii85_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear();
        st.y_abbr = t.y_abbr;
    }

    ////
    template <class ForwardWritableIterator, class StateT>
    BOOST_FORCEINLINE static void put_tuple(ForwardWritableIterator& out, StateT& s)
    {
        auto& st = base_type::pstate(s);
        char data[5];
        for (int i = 0; i < 5; i++) {
            data[i] = st.tuple % 85 + '!';
            st.tuple /= 85;
        }
        int32_t lim = 4 - st.count;
        for (int32_t i = 4; i >= lim; i--) {
            base_type::customer_put(data[i], out, s);
        }
    }

    template <class ForwardWritableIterator, class StateT>
    BOOST_FORCEINLINE static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        auto& st = base_type::pstate(s);
        st.tuple |= ((uint32_t)c) << ((3 - st.count++) * 8);
        if (st.count < 4) return;

        if (st.tuple == 0) {
            base_type::customer_put('z', out, s);
        } else if (st.tuple == 0x20202020 && st.y_abbr) {
            base_type::customer_put('y', out, s);
        } else {
            put_tuple(out, s);
        }
        st.clear();
    }

    template <class ForwardWritableIterator, class StateT>
    static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        auto& st = base_type::pstate(s);
        if (st.count) {
            put_tuple(out, s);
        }
    }

    template <class ForwardReadableIterator, class StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char* result)
    {
        auto & st = base_type::gstate(s);
        uint32_t tuple = 0;
        if (!st.count) {
            do {
                char v;
                if (!base_type::provider_get(in, s, &v)) break;
                tuple |= ((uint32_t)v) << ((3 - st.count++) * 8);
            } while (st.count < 4);

            if (tuple == 0 && st.count == 4) {
                *result = 'z';
                st.clear();
                return true;
            } else if (st.y_abbr && tuple == 0x20202020 && st.count == 4) {
                *result = 'y';
                st.clear();
                return true;
            } else if (st.count) {
                for (int i = 0; i < 5; i++) {
                    st.data[i] = tuple % 85 + '!';
                    tuple /= 85;
                }
                st.cidx = 4;
            } else {
                return false;
            }
        }

        if (st.cidx < 4 - st.count) { return false; }
        *result = st.data[st.cidx--];
        if (st.cidx < 4 - st.count) {
            st.clear();
        }
        return true;
    }
};

}
