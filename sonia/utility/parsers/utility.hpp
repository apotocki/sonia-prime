//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_PARSER_UTILITY_HPP
#define SONIA_PARSER_UTILITY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <sstream>

#include <boost/assert.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/algorithm/copy.hpp"

namespace sonia { namespace parsers {

template <typename TokenT>
bool valid(TokenT const& t) noexcept {
    return t.id != 0;
};

template <typename IteratorT>
bool valid(IteratorT const& b, IteratorT const& e) noexcept {
    return b != e && valid(*b);
}

template <typename IteratorT, typename FunctorT>
bool star(IteratorT & b, IteratorT const& e, FunctorT const& ftor) {
    // prerequisites: valid(b, e) == true
    BOOST_ASSERT(valid(b, e));
    while (ftor(b, e) && valid(b, e));
    return true;
}

template <typename IteratorT, typename FunctorT>
bool plus(IteratorT & b, IteratorT const& e, FunctorT const& ftor) {
    // prerequisites: valid(b, e) == true
    BOOST_ASSERT(valid(b, e));
    if (!ftor(b, e)) return false;
    while (valid(b, e) && ftor(b, e));
    return true;
}

template <typename CharT>
bool is_hexdigit(CharT c) noexcept {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

template <typename CharT>
uint8_t hexdigit(CharT c) noexcept {
    if (c >= '0' && c <= '9') return (uint8_t)(c - '0');
    else if (c >= 'a' && c <= 'f') return (uint8_t)((c - 'a') + 10);
    else return (uint8_t)(c - 'A' + 10);
}

template <typename IteratorT, typename IntegerT>
bool hexdigit(IteratorT & first, IteratorT const& last, int maxdigits, IntegerT& result) noexcept {
    typedef iterator_value_t<IteratorT> char_type;
    result = 0;
    for (; first != last && maxdigits > 0; ++first, --maxdigits) {
        const char_type c0 = *first;
        if (!is_hexdigit(c0)) return false;
        result = result * 16 + hexdigit(c0);
    }
    return true;
}

template <typename OutputIteratorT>
void to_utf8(uint32_t uchar, OutputIteratorT & oit) {
    if (uchar <= 0x007F) {
        *oit = static_cast<char>(uchar); ++oit;
    } else if (uchar <= 0x07FF) {
        const char c1 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        *oit = static_cast<char>((uchar & 0x1F) | 0xC0); ++oit;
        *oit = c1; ++oit;
    } else if ( uchar <= 0xFFFF ) {
        const char c2 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        const char c1 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        *oit = static_cast<char>((uchar & 0x0F) | 0xE0); ++oit;
        *oit = c1; ++oit;
        *oit = c2; ++oit;
    } else if( uchar <= 0x10FFFF ) {
        const char c3 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        const char c2 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        const char c1 = static_cast<char>((uchar & 0x3F) | 0x80); uchar >>= 6;
        *oit = static_cast<char>((uchar & 0x07) | 0xF0); ++oit;
        *oit = c1; ++oit;
        *oit = c2; ++oit;
        *oit = c3; ++oit;
    } else {
        BOOST_THROW_EXCEPTION(exception(fmt("Wrong utf32 character '%#x'") % uchar));
    }
}

template <
    template <class> class LexerT,
    template <class> class ParserT,
    class ModelT,
    typename IteratorT
>
void parse(ModelT & model, IteratorT b, IteratorT e)
{
    typedef LexerT<IteratorT> lexer_type;

    lexer_type lexer;
    typename lexer_type::iterator iter = lexer.begin(std::move(b), e), end = lexer.end();

    ParserT<lexer_type> parser(model);

    try {
        parser.parse(iter, end);
    } catch (exception const& err) {
        std::ostringstream resultss;
        resultss << "parsing error : ";
        std::string msg(err.what()), tokval;
        if (iter == end) {
            tokval = "end of input";
        } else {
            auto const& tok = *iter;
            tokval = std::string(tok.first, tok.eoi);
        }
        if (msg.empty()) resultss << "token: '" << tokval << "' is not expected";
        else resultss << msg << ", got: '" << tokval << "'";

        throw exception(std::move(resultss.str()));
    }

    if (iter != end) {
        std::string rest;
        copy_not_more(iter->first, e, std::back_inserter(rest), 50);
        throw exception(fmt("parser error: unexpected token, stopped at: \"%1%\"") % rest);
    }
}

}}

#endif // SONIA_PARSER_UTILITY_HPP
