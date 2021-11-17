/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../tags.hpp"

namespace sonia::conversion {

namespace detail {

template <unsigned int w>
struct u_provider
{
    static const unsigned char u = w/8;
    static const unsigned char defaultIV[2*u];
};

template <unsigned int w>
const unsigned char u_provider<w>::defaultIV[2*u] = {0};

template <unsigned int b>
struct default_pwd_provider
{
    static const unsigned char value[b];
};

template <unsigned int b>
const unsigned char default_pwd_provider<b>::value[b] = {0};

} // namespace sonia::conversion::detail

template <unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
struct rc5_tag : detail::u_provider<w>, cvt_tag
{
    typedef rc5_tag customer_tag;
    typedef rc5_tag provider_tag;

    using detail::u_provider<w>::u;

    explicit rc5_tag(
          const unsigned char (&password)[b] = detail::default_pwd_provider<b>::value
        , const unsigned char (&iv)[2*u] = detail::u_provider<w>::defaultIV
        , char padding_char = '\0'
        , ErrorHandlerT const& eh = ErrorHandlerT()) 
    : password_(password), iv_(iv), padding_character_(padding_char), errh_(eh) {}
    
    const unsigned char (&password_)[b];
    const unsigned char (&iv_)[2*u];
    const char padding_character_;

    ErrorHandlerT errh_;
};

inline rc5_tag<32, 12, 8, throw_error_handler> rc5(
      const unsigned char (&password)[8] = detail::u_provider<32>::defaultIV
    , const unsigned char (&iv)[8] = detail::u_provider<32>::defaultIV
    , char pch = '\0')
{
    return rc5_tag<32, 12, 8, throw_error_handler>(password, iv, pch);
}

template <unsigned int w, unsigned int r, unsigned int b>
inline rc5_tag<w, r, b, throw_error_handler> rc5(
      const unsigned char (&password)[b]
    , const unsigned char (&iv)[8] = detail::u_provider<w>::defaultIV
    , char pch = '\0')
{
    return rc5_tag<w, r, b, throw_error_handler>(password, iv, pch, throw_error_handler());
}

template <unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
inline rc5_tag<w, r, b, ErrorHandlerT> rc5(
      const unsigned char (&password)[b]
    , const unsigned char (&iv)[8] = detail::u_provider<w>::defaultIV
    , char pch = '\0'
    , ErrorHandlerT const& err = ErrorHandlerT())
{
    return rc5_tag<w, r, b, ErrorHandlerT>(password, iv, pch, err);
}

}
