/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "sonia/type_traits.hpp"
#include "../converter_facade.hpp"
#include "../integral_state.hpp"

#include "constants.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf32_simple_folding
    : public converter_facade<
          utf32_simple_folding<ProviderT, CustomerT, ErrorHandlerT>
        , CustomerT
        , ProviderT
        , char32_t
        , char32_t
        , integral_state<void, ErrorHandlerT>
        , integral_state<void, ErrorHandlerT>
    >
{
protected:
    typedef typename utf32_simple_folding::facade_type base_type;

#if defined(BOOST_MSVC)
    friend identity<base_type>::type;
#else
    friend base_type;
#endif

private:
    template <typename HT, typename StateT>
    inline static void do_clear(utf32_tag<HT> const&, utf32sfold_tag<ErrorHandlerT> const& t, StateT & st) {
        st.clear(t.errh_);
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_put(char32_t val, ForwardWritableIterator& out, StateT& s)
    {
        base_type::customer_put(cvt(val), out, s);
    }

    template <typename ForwardWritableIterator, typename StateT>
    inline static void do_unshift(ForwardWritableIterator& out, StateT & s)
    {
        // do nothing
    }

    template <typename ForwardReadableIterator, typename StateT>
    static bool do_get(ForwardReadableIterator& in, StateT& s, char32_t * result)
    {
        if (!base_type::provider_get(in, s, result)) return false;
        *result = cvt(*result);
        return true;
    }

    static char32_t cvt(char32_t val)
    {
        std::map<char32_t, char32_t> const& cmap = utf32constants::get_simple_folding_map();
        std::map<char32_t, char32_t>::const_iterator v = cmap.find(val);
        return (v != cmap.end()) ? v->second : val;
    }
};

}
