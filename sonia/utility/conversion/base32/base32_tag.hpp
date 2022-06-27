/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../tags.hpp"
#include "constants.hpp"

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct base32_tag : cvt_tag
{
    using customer_tag = base32_tag;
    using provider_tag = base32_tag;
    
    base32_tag(char pc, const char* alphabet, const unsigned char* matrix, ErrorHandlerT const& eh = ErrorHandlerT())
        : padding_character{ pc }
        , alphabet{ alphabet }
        , matrix{ matrix }
        , errh{ eh }
    {}

    char padding_character;
    const char* alphabet;
    const unsigned char* matrix;
    ErrorHandlerT errh;
};

const char default_base32_padding_char = '=';

inline base32_tag<throw_error_handler> base32() {
    return { default_base32_padding_char, base32constants::alphabet, base32constants::matrix };
}

inline base32_tag<throw_error_handler> base32(char pc) {
    return { pc, base32constants::alphabet, base32constants::matrix };
}

template <typename ErrorHandlerT>
inline base32_tag<ErrorHandlerT> base32(ErrorHandlerT const& err) {
    return { default_base32_padding_char, base32constants::alphabet, base32constants::matrix, err };
}

template <typename ErrorHandlerT>
inline base32_tag<ErrorHandlerT> base32(char pc, ErrorHandlerT const& err) {
    return { pc, base32constants::alphabet, base32constants::matrix, err };
}

inline base32_tag<throw_error_handler> base32hex() {
    return { default_base32_padding_char, base32constants::hex_alphabet, base32constants::hex_matrix };
}

inline base32_tag<throw_error_handler> base32hex(char pc) {
    return { pc, base32constants::hex_alphabet, base32constants::hex_matrix };
}

template <typename ErrorHandlerT>
inline base32_tag<ErrorHandlerT> base32hex(ErrorHandlerT const& err) {
    return { default_base32_padding_char, base32constants::hex_alphabet, base32constants::hex_matrix, err };
}

template <typename ErrorHandlerT>
inline base32_tag<ErrorHandlerT> base32hex(char pc, ErrorHandlerT const& err) {
    return { pc, base32constants::hex_alphabet, base32constants::hex_matrix, err };
}

}
