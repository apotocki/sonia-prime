/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <locale>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "state.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf16_to_cp
    : public converter_facade<
          utf16_to_cp<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , wchar_t
        , char
        , cpstate_wc<ErrorHandlerT>
        , cpstate<ErrorHandlerT>
    >
{
protected:
    using facet_t = std::codecvt<wchar_t, char, std::mbstate_t>;
    using base_type = typename utf16_to_cp::facade_type;
    using cpstate_t = cpstate<ErrorHandlerT>;
    using cpstate_wc_t = cpstate_wc<ErrorHandlerT>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf16_tag<ErrorHandlerT> const&, cp_tag<HT> const& arg, StateT& st) {
        st.clear(arg);
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_put(wchar_t ec, ForwardWritableIterator& out, StateT& s)
    {
        auto & st = base_type::pstate(s);
        st.push_wchar(ec);

        const wchar_t *pin;
        char charcache[MB_LEN_MAX];
        char *pout;

        std::codecvt_base::result res = st.facet_->out(
            st.facetstate_,
            st.wcharcache_, st.wcharcache_ + st.wcharcache_length, pin,
            charcache, charcache + sizeof(charcache), pout);

        if (res == std::codecvt_base::error) {
            st.error(ec, "Conversion error.");
            return;
        }
        st.shift_wcharcache(pin);
        const char *poutstart = charcache;
        while (poutstart != pout) {
            base_type::customer_put(*poutstart, out, s);
            poutstart++;
        }
        if (st.wcharcache_length == cpstate_t::wcharcache_size) {
            st.error(ec, "Internal error: facet demands too much long input sequence.");
        }
    }

    template <typename ForwardWritableIterator, typename StateT>
    static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        auto & st = base_type::pstate(s);
        if (st.wcharcache_length == 0) {
            char charcache[MB_LEN_MAX];
            char *pout;

            std::codecvt_base::result res = st.facet_->unshift(
                st.facetstate_,
                charcache, charcache + MB_LEN_MAX, pout
            );

            const char *poutstart = charcache;
            while (poutstart != pout) {
                base_type::customer_put(*poutstart, out, s);
                poutstart++;
            }
            return;
        }
        st.error(st.wcharcache_[0], "Not all input was converted before unshifting.");
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char * result)
    {
        auto & st = base_type::gstate(s);
        if (st.charcache_length) {
            *result = st.pop_char();
            return true;
        }

        char *pout = st.charcache_;

        for (;;)
        {
            wchar_t ov;
            if (!base_type::provider_get(in, s, &ov)) {
                if (st.wcharcache_length == 0)
                {
                    std::codecvt_base::result res = st.facet_->unshift(
                        st.facetstate_,
                        st.charcache_, st.charcache_ + cpstate_t::charcache_size, pout
                    );

                    if (st.charcache_ != pout) {
                        st.charcache_start = 0; st.charcache_length = pout - st.charcache_;
                        *result = st.pop_char();
                        return true;
                    }
                    return false;
                }
                st.error(st.wcharcache_[0], "Not all input was converted before unshifting.");
                return false;
            }

            st.push_wchar(ov);

            const wchar_t *pin;
            
            std::codecvt_base::result res = st.facet_->out(
                st.facetstate_,
                st.wcharcache_, st.wcharcache_ + st.wcharcache_length, pin,
                pout, st.charcache_ + cpstate_t::charcache_size, pout);

            if (res == std::codecvt_base::error) {
                st.error(ov, "Conversion error.");
            }
            st.shift_wcharcache(pin);
            if (pout != st.charcache_) {
                st.charcache_start = 0; st.charcache_length = pout - st.charcache_;
                *result = st.pop_char();
                return true;
            }
            if (res == std::codecvt_base::partial) {
                if (st.wcharcache_length == cpstate_t::wcharcache_size) {
                    st.error(ov, "Internal error: facet demands too much long input sequence.");
                }
            }
        }
    }
};

}
