/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <locale>
#include <sstream>

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"
#include "../integral_state.hpp"
#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class cp_to_utf16
    : public converter_facade<
          cp_to_utf16<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char
        , wchar_t
        , cpstate<ErrorHandlerT>
        , cpstate<ErrorHandlerT>
    >
{
protected:
    using facet_t = std::codecvt<wchar_t, char, std::mbstate_t>;
    using base_type = typename cp_to_utf16::facade_type;
    using cpstate_t = cpstate<ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(cp_tag<HT> const& arg, utf16_tag<ErrorHandlerT> const& t, StateT& st) {
        st.clear(arg);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(char c, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        st.push_char(c);

        const char *pin;
        wchar_t wcharcache[2];
        wchar_t *pout;

        std::codecvt_base::result res = st.facet_->in(
            st.facetstate_,
            st.charcache_, st.charcache_ + st.charcache_length, pin,
            wcharcache, wcharcache + sizeof(wcharcache)/sizeof(wchar_t), pout);

        if (res == std::codecvt_base::error) {
            st.error(c, "Conversion error.");
            return;
        }
        st.shift_charcache(pin);
        const wchar_t *poutstart = wcharcache;
        while (poutstart != pout) {
            base_type::customer_put(*poutstart, out, s);
            poutstart++;
        }
        if (st.charcache_length == cpstate_t::charcache_size) {
            st.error(c, "Internal error: facet demands too much long input sequence.");
        }
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        auto & st = base_type::pstate(s);
        if (st.charcache_length != 0) {
            st.error(st.wcharcache_[0], "Not all input was converted before unshifting.");    
        }
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, wchar_t * result)
    {
        auto & st = base_type::gstate(s);
        if (st.wcharcache_length) {
            *result = st.pop_wchar();
            return true;
        }

        wchar_t *pout = st.wcharcache_;

        for (;;)
        {
            char ov;
            if (!base_type::provider_get(in, s, &ov)) {
                return false;
            }
            st.push_char(ov);

            const char *pin;

            std::codecvt_base::result res = st.facet_->in(
                st.facetstate_,
                st.charcache_, st.charcache_ + st.charcache_length, pin,
                pout, st.wcharcache_ + cpstate_t::wcharcache_size, pout);

            if (res == std::codecvt_base::error) {
                st.error(ov, "Conversion error.");
            }

            st.shift_charcache(pin);
            if (pout != st.wcharcache_) {
                st.wcharcache_start = 0; st.wcharcache_length = pout - st.wcharcache_;
                *result = st.pop_wchar();
                return true;
            }
            if (res == std::codecvt_base::partial) {
                if (st.charcache_length == cpstate_t::charcache_size) {
                    st.error(ov, "Internal error: facet demands too much long input sequence.");
                }
            }
        }
    }
};

}
