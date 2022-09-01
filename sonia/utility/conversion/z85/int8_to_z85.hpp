/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"

#include "z85_tag.hpp"
#include "states.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class int8_to_z85
    : public converter_facade<
          int8_to_z85<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , char
        , z85_put_state
        , z85_enc_get_state
    >
{
protected:
    using base_type = typename int8_to_z85::facade_type;
    using pstate_type = z85_put_state;
    using gstate_type = z85_enc_get_state;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename StateT>
    inline static void do_clear(int8_tag const&, z85_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear();
    }

    ////
    template <class ForwardWritableIterator, class StateT>
    BOOST_FORCEINLINE static void put_tuple(ForwardWritableIterator& out, StateT& s)
    {
        auto& st = base_type::pstate(s);
        char data[5];
        for (int i = 0; i < 5; i++) {
            data[i] = st.tuple % 85;
            st.tuple /= 85;
        }
        int32_t lim = 4 - st.count;
        for (int32_t i = 4; i >= lim; i--) {
            base_type::customer_put(z85constants::alphabet[data[i]], out, s);
        }
    }

    template <class ForwardWritableIterator, class StateT>
    BOOST_FORCEINLINE static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        auto& st = base_type::pstate(s);
        st.tuple |= ((uint32_t)(uint8_t)c) << ((3 - st.count++) * 8);
        if (st.count < 4) return;

        put_tuple(out, s);
        
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
                tuple |= ((uint32_t)(uint8_t)v) << ((3 - st.count++) * 8);
            } while (st.count < 4);

            if (st.count) {
                for (int i = 0; i < 5; i++) {
                    st.data[i] = tuple % 85;
                    tuple /= 85;
                }
                st.cidx = 4;
            } else {
                return false;
            }
        }

        if (st.cidx < 4 - st.count) { return false; }
        *result = z85constants::alphabet[st.data[st.cidx--]];
        if (st.cidx < 4 - st.count) {
            st.clear();
        }
        return true;
    }
};

}
