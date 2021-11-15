/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "state.hpp"
#include "utf16_validator.hpp"

namespace sonia::conversion {

namespace detail {

template <class DerivedT, class ProviderT, class CustomerT, class ErrorHandlerT>
class utf16_to_utf16le_impl
    : public utf16_validator<DerivedT, ProviderT, CustomerT, ErrorHandlerT, char, null_t, utf16_state1>
{
protected:
    typedef utf16_to_utf16le_impl<DerivedT, ProviderT, CustomerT, ErrorHandlerT> this_type;
    typedef utf16_validator<DerivedT, ProviderT, CustomerT, ErrorHandlerT, char, null_t, utf16_state1> base_type;

    friend class base_type;
    //friend typename base_type::facade_type;
public:
    template <typename provider_init_tag>
    utf16_to_utf16le_impl(provider_init_tag const& t, utf16le_tag<ErrorHandlerT> const& tag) : base_type(t, tag.errh_)
    {}

private:
    inline void do_clear_impl(null_t& st) const { }
    inline void do_clear_impl(utf16_state1& st) const { st.clear(); }

    inline bool do_equal_states_impl(null_t const& s1, null_t const& s2) const { return true; }
    inline bool do_equal_states_impl(utf16_state1 const& s1, utf16_state1 const& s2) const { return s1 == s2; }

    template <typename ForwardWritableIterator, typename state_t>
    inline void do_put(wchar_t val, ForwardWritableIterator& out, state_t& s) const
    {
        base_type::customer_put(static_cast<char>(val & 0x00FF), out, s);
        base_type::customer_put(static_cast<char>((val >> 8) & 0x00FF), out, s);
    }

    template <typename ForwardWritableIterator, typename state_t>
    inline void do_unshift(ForwardWritableIterator& out, state_t & s) const
    {
        // do nothing
    }

    template <typename ForwardReadableIterator, typename state_t>
    optional<char> do_get(ForwardReadableIterator& in, state_t& s) const
    {
        utf16_state1 & st = base_type::gstate(s).second();

        if (st.cached) {
            char result = st.value; st.clear();
            return result;
        }
        optional<wchar_t> ov = base_type::do_get_impl(in, s);
        if (!ov) return optional<char>(); // eof
        st.value = static_cast<unsigned char>(ov.get() >> 8);
        st.cached = 1;
        return static_cast<char>(ov.get() & 0xff);
    }
};

} // namespace sonia::conversion::detail

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf16_to_utf16le
    : public detail::utf16_to_utf16le_impl<utf16_to_utf16le<ProviderT, CustomerT, ErrorHandlerT>, ProviderT, CustomerT, ErrorHandlerT>
{
    typedef utf16_to_utf16le<ProviderT, CustomerT, ErrorHandlerT> this_type;
    typedef detail::utf16_to_utf16le_impl<this_type, ProviderT, CustomerT, ErrorHandlerT> base_type;
public:
    template <typename provider_init_tag>
    utf16_to_utf16le(provider_init_tag const& pv, utf16le_tag<ErrorHandlerT> const& tv) 
        : base_type(pv, tv) {}

};

} // namespace sonia::conversion
