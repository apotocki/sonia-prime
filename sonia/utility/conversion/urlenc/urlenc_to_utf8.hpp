/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../converter_facade.hpp"

namespace sonia::conversion {

namespace detail {

template <class DerivedT, class ProviderT, class CustomerT, class ErrorHandlerT>
class urlenc_to_utf8_impl
    : public converter_facade<
          DerivedT
        , CustomerT
        , ProviderT
        , char
        , char
        , boost::uint_least32_t
        , null_t>
{
protected:
    using this_type =urlenc_to_utf8_impl<DerivedT, ProviderT, CustomerT, ErrorHandlerT>;
    using base_type = converter_facade<
          DerivedT
        , CustomerT
        , ProviderT
        , char
        , char
        , boost::uint_least32_t
        , null_t>;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

public:
    explicit urlenc_to_utf8_impl(utf8_tag<ErrorHandlerT> const& iv) : base_type(), errh_(iv.errh_)
    {}

    template <typename provider_init_tag>
    urlenc_to_utf8_impl(provider_init_tag const& t, utf8_tag<ErrorHandlerT> const& iv) : base_type(t), errh_(iv.errh_)
    {}

private:
    inline void do_clear(null_t const&) const {}
    inline void do_clear(boost::uint_least32_t& st) const { st = 0; }
    inline bool do_equal_states(char32_t const& s1, char32_t const& s2) const { return s1 == s2; }
    inline bool do_equal_states(null_t const& s1, null_t const& s2) const { return true; }

    template <typename ForwardWritableIterator, typename state_t>
    void do_put(char uchar, ForwardWritableIterator& out, state_t& s) const
    {
        throw std::runtime_error("urlenc to utf8 (put) is not implemented");
        /*
        if ( uchar <= 0x007F ) {
            base_type::customer_put(static_cast<char>(uchar), out, s);
        } else if ( uchar <= 0x07FF ) {
            char c1 = static_cast<char>(uchar & 0x3F | 0x80);    uchar >>= 6;
            base_type::customer_put(static_cast<char>(uchar & 0x1F | 0xC0), out, s);
            base_type::customer_put(c1, out, s);
        } else if ( uchar <= 0xFFFF ) {
            char c2 = static_cast<char>(uchar & 0x3F | 0x80); uchar >>= 6;
            char c1 = static_cast<char>(uchar & 0x3F | 0x80); uchar >>= 6;
            base_type::customer_put(static_cast<char>(uchar & 0x0F | 0xE0), out, s);
            base_type::customer_put(c1, out, s);
            base_type::customer_put(c2, out, s);
        } else if( uchar <= 0x10FFFF ) {
            char c3 = static_cast<char>(uchar & 0x3F | 0x80); uchar >>= 6;
            char c2 = static_cast<char>(uchar & 0x3F | 0x80); uchar >>= 6;
            char c1 = static_cast<char>(uchar & 0x3F | 0x80); uchar >>= 6;
            base_type::customer_put(static_cast<char>(uchar & 0x07 | 0xF0), out, s);
            base_type::customer_put(c1, out, s);
            base_type::customer_put(c2, out, s);
            base_type::customer_put(c3, out, s);
        } else {
            std::stringstream serr;
            serr << "Wrong utf32 character '" << std::hex << uchar << "'";
            errh_(uchar, base_type::pstate(s), serr.str().c_str());
        }
        */
    }

    template <typename ForwardWritableIterator, typename state_t>
    inline void do_unshift(ForwardWritableIterator& out, state_t & s) const
    {
        // do nothing
    }

    inline char decode(char v) const
    {
        if (v >= '0' && v <= '9') return v - '0';
        else if (v >= 'a' && v <= 'z') return v - 'a' + 10;
        else if (v >= 'A' && v <= 'Z') return v - 'A' + 10;
        std::stringstream serr;
        serr << "Wrong url encoded character '" << std::hex << v << "'";
        errh_(v, null_t(), serr.str().c_str());
        return 0;
    }

    template <typename ForwardReadableIterator, typename state_t>
    optional<char> do_get(ForwardReadableIterator& in, state_t& s) const
    {
        optional<char> ov = base_type::provider_get(in, s);
        if (!ov) return optional<char>();
        char c0 = ov.get();
        if (c0 == '+') return ' ';
        if (c0 != '%') return c0;
        ov = base_type::provider_get(in, s);
        if (!ov) return optional<char>();
        char c1 = ov.get();
        ov = base_type::provider_get(in, s);
        if (!ov) return optional<char>();
        char c2 = ov.get();
        
        return (decode(c1) << 4) | decode(c2);
    }
private:
    ErrorHandlerT errh_;
};


} // namespace sonia::conversion::detail

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class urlenc_to_utf8
    : public detail::urlenc_to_utf8_impl<urlenc_to_utf8<ProviderT, CustomerT, ErrorHandlerT>, ProviderT, CustomerT, ErrorHandlerT>
{
    typedef urlenc_to_utf8<ProviderT, CustomerT, ErrorHandlerT> this_type;
    typedef detail::urlenc_to_utf8_impl<this_type, ProviderT, CustomerT, ErrorHandlerT> base_type;
public:
    urlenc_to_utf8() 
        : base_type(utf8_tag<ErrorHandlerT>()) {}

    template <typename provider_init_tag>
    urlenc_to_utf8(provider_init_tag const& pv, utf8_tag<ErrorHandlerT> const& tv) 
        : base_type(pv, tv) {}

};

} // namespace sonia::conversion