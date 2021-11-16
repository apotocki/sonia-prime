/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <sstream>
#include "../converter_facade.hpp"
#include "state.hpp"

namespace sonia::conversion {

namespace detail {

template <class DerivedT, class ProviderT, class CustomerT, class ErrorHandlerT>
class utf8_to_xmlenc_impl
    : public converter_facade<
          DerivedT
        , CustomerT
        , ProviderT
        , char
        , char
        , null_t
        , null_t>
{
protected:
    typedef utf8_to_xmlenc_impl<DerivedT, ProviderT, CustomerT, ErrorHandlerT> this_type;
    typedef converter_facade<
          DerivedT
        , CustomerT
        , ProviderT
        , char
        , char
        , null_t
        , null_t> base_type;

#if defined(BOOST_MSVC) || defined (BOOST_INTEL)
    friend identity_t<base_type>;
#else
    friend base_type;
#endif

public:
    explicit utf8_to_xmlenc_impl(xmlenc_tag<ErrorHandlerT> const& iv) : base_type(), errh_(iv.errh_)
    {}

    template <typename provider_init_tag>
    utf8_to_xmlenc_impl(provider_init_tag const& t, xmlenc_tag<ErrorHandlerT> const& iv) : base_type(t), errh_(iv.errh_)
    {}

private:
    inline void do_clear(null_t const&) const {}
    //inline void do_clear(utf8st_t& st) const { st.clear(); }
    //inline bool do_equal_states(utf8st_t const& s1, utf8st_t const& s2) const { return s1 == s2; }
    inline bool do_equal_states(null_t const& s1, null_t const& s2) const { return true; }

    template <typename ForwardWritableIterator, typename state_t>
    void do_put(char ec, ForwardWritableIterator& out, state_t& s) const
    {
        static const char ampenc[] = "&amp;";
        static const char ltenc[] = "&lt;";
        static const char gtenc[] = "&gt;";
        static const char aposenc[] = "&apos;";
        static const char quotenc[] = "&quot;";

        switch (ec)
        {
        case '&':
            base_type::customer_put(boost::make_iterator_range(ampenc, ampenc + sizeof(ampenc) - 1), out, s);
            break;
        case '<':
            base_type::customer_put(boost::make_iterator_range(ltenc, ltenc + sizeof(ltenc) - 1), out, s);
            break;
        case '>':
            base_type::customer_put(boost::make_iterator_range(gtenc, gtenc + sizeof(gtenc) - 1), out, s);
            break;
        case '\'':
            base_type::customer_put(boost::make_iterator_range(aposenc, aposenc + sizeof(aposenc) - 1), out, s);
            break;
        case '\"':
            base_type::customer_put(boost::make_iterator_range(quotenc, quotenc + sizeof(quotenc) - 1), out, s);
            break;
        default:
            base_type::customer_put(ec, out, s);
        }
    }

    template <typename ForwardWritableIterator, typename state_t>
    inline void do_unshift(ForwardWritableIterator& out, state_t & s) const
    {
        // do nothing
    }

    template <typename ForwardReadableIterator, typename state_t>
    optional<char> do_get(ForwardReadableIterator& in, state_t& s) const
    {
        throw std::runtime_error("utf8toxmlenc(get) not implemented yet");
        //return base_type::provider_get(in, s);
        /*
        for (;;)
        {
            optional<char> ov = 
            if (!ov) return optional<char32_t>();

            char32_t c = ov.get() & 0xff;
            if (c < 128) {
                return c;
            } else if ((c & 0xe0) == 0xc0) {
                if (c != 0xc1 && c != 0xc0)
                {
                    ov = base_type::provider_get(in, s);
                    if (!ov) {
                        errh_(c, base_type::gstate(s), "Unexpected eof");
                        return optional<char32_t>();
                    }
                    char32_t c1 = ov.get();
                    if ((c1 & 0xc0) == 0x80) {
                        c &= 0x1F; c <<= 6;
                        return c | (c1 & 0x3F);
                    }
                    c = c1;
                }
            } else if ((c & 0xf0) == 0xe0) {
                char buff[2];
                if ((buff + 2) != base_type::provider_get_range(in, s, buff, buff + 2)) {
                    errh_(c, base_type::gstate(s), "Unexpected eof");
                    return optional<char32_t>();
                }

                if ((buff[0] & 0xc0) == 0x80) {
                    if ((buff[1] & 0xc0) == 0x80) {
                        c &= 0x0F; c <<= 6; c |= (buff[0] & 0x3F);
                        c <<= 6; c |= (buff[1] & 0x3F);
                        
                        if (c == 0xFEFF ) continue;
                        if (c < 0xD800 || c > 0xDFFF) return c;
                        // bad decoded value here
                        std::stringstream serr;
                        serr << "Wrong decoded value '0x" << std::hex << c << "' (utf16 surrogate)";
                        errh_(c, base_type::gstate(s), serr.str().c_str());
                        continue;
                    } else {
                        c = buff[1];
                    }
                } else {
                    c = buff[0];
                }
            } else if ((c & 0xf8) == 0xf0 && c < 0xf5) {
                char buff[3];
                if ( (buff + 3) != base_type::provider_get_range(in, s, buff, buff + 3)) {
                    errh_(c, base_type::gstate(s), "Unexpected eof");
                    return optional<char32_t>();
                }

                if ((buff[0] & 0xc0) == 0x80) {
                    if ((buff[1] & 0xc0) == 0x80) {
                        if ((buff[2] & 0xc0) == 0x80) {
                            c &= 0x07; c <<= 6; c |= (buff[0] & 0x3F);
                            c <<= 6; c |= (buff[1] & 0x3F);
                            c <<= 6; c |= (buff[2] & 0x3F);
                            return c;
                        } else {
                            c = buff[2];
                        }
                    } else {
                        c = buff[1];
                    }
                } else {
                    c = buff[0];
                }
            }
            errh_(c, base_type::gstate(s), (std::string("Wrong utf8 character '") + static_cast<char>(c) + "'").c_str());
        }
        */
    }
    
private:
    ErrorHandlerT errh_;
};

} // namespace sonia::conversion::detail

template <class ProviderT, class CustomerT, class ErrorHandlerT>
class utf8_to_xmlenc
    : public detail::utf8_to_xmlenc_impl<utf8_to_xmlenc<ProviderT, CustomerT, ErrorHandlerT>, ProviderT, CustomerT, ErrorHandlerT>
{
    typedef utf8_to_xmlenc<ProviderT, CustomerT, ErrorHandlerT> this_type;
    typedef detail::utf8_to_xmlenc_impl<this_type, ProviderT, CustomerT, ErrorHandlerT> base_type;
public:

    utf8_to_xmlenc() 
        : base_type(xmlenc_tag<ErrorHandlerT>()) {}

    template <typename provider_init_tag>
    utf8_to_xmlenc(provider_init_tag const& pv, xmlenc_tag<ErrorHandlerT> const& tv) 
        : base_type(pv, tv) {}

};

} // namespace sonia::conversion
