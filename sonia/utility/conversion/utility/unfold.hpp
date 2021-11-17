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
#include "sonia/array_view.hpp"
#include "../converter_facade.hpp"

namespace sonia::conversion {

template <class ProviderT, class CustomerT, class T>
class array_unfold
    : public converter_facade<
          array_unfold<ProviderT, CustomerT, T>
        , CustomerT
        , ProviderT
        , array_view<T>
        , T
        , null_t
        , array_view<T>
    >
{
protected:
    using base_type = typename array_unfold::facade_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

private:
    inline static void do_clear(array_tag<T> const& t, type_tag<T> const&, null_t &) { }
    inline static void do_clear(array_tag<T> const& t, type_tag<T> const&, array_view<T> & st) { st = array_view<T>(); }

    template <typename ForwardWritableIterator, typename state_t>
    static void do_put(array_view<T> ec, ForwardWritableIterator& out, state_t& s)
    {
        for (T & elem : ec) {
            base_type::customer_put(elem, out, s);
        }
    }

    template <typename ForwardWritableIterator, typename state_t>
    inline static void do_unshift(ForwardWritableIterator& out, state_t & s)
    {

    }

    template <typename ForwardReadableIterator, typename state_t>
    static bool do_get(ForwardReadableIterator& in, state_t& s, T * result)
    {
        array_view<T> & st = base_type::gstate(s);
        while (st.empty()) {
            array_view<T> ov;
            if (!base_type::provider_get(in, s, &st)) return false;
        }
        *result = st.front();
        st = array_view<T>(st.begin() + 1, st.size() - 1);
        return true;
    }
};

}
