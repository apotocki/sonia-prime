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

template <typename T, size_t SzV>
struct buffer_state
{
    T buffer[SzV];
    size_t size_;

    inline void clear() {
        size_ = 0;
    }

    template <typename ArgT>
    bool put(ArgT && val) {
        BOOST_ASSERT (size_ < SzV);
        buffer[size_++] = std::forward<ArgT>(val);
        return size_ == SzV;
    }

    array_view<T> get() { return array_view<T>(buffer, size_); }
};

template <class ProviderT, class CustomerT, class T, size_t SzV>
class type_to_buffer
    : public converter_facade<
          type_to_buffer<ProviderT, CustomerT, T, SzV>
        , CustomerT
        , ProviderT
        , T
        , array_view<T>
        , buffer_state<T, SzV>
        , buffer_state<T, SzV>
    >
{
protected:
    using base_type = typename type_to_buffer::facade_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend class identity<base_type>::type;
#endif

private:
    inline static void do_clear(type_tag<T> const&, buffer_tag<T, SzV> const& t, buffer_state<T, SzV> & st) {
        st.clear();
    }

    template <typename ForwardWritableIterator, typename state_t>
    static void do_put(T ec, ForwardWritableIterator& out, state_t& s)
    {
        auto & st = base_type::pstate(s);
        if (st.put(ec)) {
            base_type::customer_put(st.get(), out, s);
            st.size_ = 0;
        }
    }

    template <typename ForwardWritableIterator, typename state_t>
    inline static void do_unshift(ForwardWritableIterator& out, state_t & s)
    {
        auto & st = base_type::pstate(s);
        if (st.size_) {
            base_type::customer_put(st.get(), out, s);
            st.size_ = 0;
        }
    }

    template <typename ForwardReadableIterator, typename state_t>
    static bool do_get(ForwardReadableIterator& in, state_t& s, array_view<T> * result)
    {
        auto & st = base_type::gstate(s);
        T * pos = base_type::provider_get_range(in, s, st.buffer, st.buffer + SzV);
        if (pos == st.buffer) return false;
        *result = array_view<T>(st.buffer, pos);
        return true;
    }
};

}
