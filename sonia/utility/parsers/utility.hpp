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
#include "sonia/utility/algorithm/copy.hpp"

namespace sonia::parsers {

template <typename TokenT>
bool valid(TokenT const& t) noexcept { return true; } /*t.id != 0; };*/

template <typename IteratorT>
bool valid(IteratorT const& b, IteratorT const& e) noexcept
{
    return b != e && valid(*b);
}

template <typename IteratorT>
inline bool alpha(IteratorT & b)
{
    char c = *b;
    if ((c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A)) { // A-Z || a-z
        ++b;
        return true;
    }
    return false;
}

template <typename IteratorT>
inline bool digit(IteratorT & b)
{
    char c = *b;
    if (c >= 0x30 && c <= 0x39) { // 0-9
        ++b;
        return true;
    }
    return false;
}

template <typename IteratorT>
inline bool character(IteratorT & b, char c)
{
    if (*b != c) return false;
    ++b;
    return true;
}

template <typename IteratorT>
inline bool character_from(IteratorT & b, string_view s)
{
    char c = *b;
    for (char tc : s) {
        if (tc == c) {
            ++b;
            return true;
        }
    }
    return false;
}

template <typename IteratorT>
inline bool string(IteratorT & b, IteratorT const& e, string_view s)
{
    IteratorT it = b;
    for (auto sit = s.begin(), seit = s.end(); sit != seit;) {
        if (*it != *sit) return false;
        ++it; ++sit;
        if (!valid(it, e)) {
            if (sit != seit) return false;
            break;
        }
    }
    b = it;
    return true;
}

template <typename CharT>
bool is_hexdigit(CharT c) noexcept
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

template <typename IteratorT>
inline bool hexdig(IteratorT & b)
{
    if (!is_hexdigit(*b)) return false;
    ++b;
    return true;
}

template <typename IteratorT, typename FunctorT>
bool star(IteratorT & b, IteratorT const& e, FunctorT const& ftor)
{
    while (valid(b, e) && ftor(b, e));
    return true;
}

template <typename IteratorT, typename FunctorT>
bool plus(IteratorT & b, IteratorT const& e, FunctorT const& ftor)
{
    if (!valid(b, e) || !ftor(b, e)) return false;
    while (valid(b, e) && ftor(b, e));
    return true;
}

template <typename CharT>
uint8_t get_hexdigit(CharT c) noexcept
{
    if (c >= '0' && c <= '9') return (uint8_t)(c - '0');
    else if (c >= 'a' && c <= 'f') return (uint8_t)(c - 'a' + 10);
    else if (c >= 'A' && c <= 'F') return (uint8_t)(c - 'A' + 10);
    else return 0xff;
}

template <typename IteratorT, typename IntegerT>
bool hexinteger(IteratorT & b, IteratorT const& e, unsigned int mindigits, unsigned int maxdigits, IntegerT& result) noexcept
{
    IteratorT pos = b;
    using char_type = iterator_value_t<IteratorT>;
    result = 0;
    unsigned int dc = 0;
    for (; pos != e && dc < maxdigits; ++pos, ++dc) {
        const char_type c0 = *pos;
        uint8_t v = get_hexdigit(c0);
        if (v != 0xff) {
            result = result * 16 + v;
        } else {
            break;
        }
    }
    if (dc < mindigits) {
        return false;
    }
    b = pos;
    return true;
}

template <typename IteratorT, typename IntegerT>
unsigned int integer(IteratorT & b, IteratorT const& e, unsigned int mindigits, unsigned int maxdigits, IntegerT& result) noexcept
{
    IteratorT pos = b;
    using char_type = iterator_value_t<IteratorT>;
    result = 0;
    unsigned int dc = 0;
    for (; pos != e && dc < maxdigits; ++pos, ++dc) {
        const char_type c = *pos;
        if (c >= '0' && c <= '9') {
            result = result * 10 + (uint8_t)(c - '0');
        } else {
            break;
        }
    }
    if (dc < mindigits) {
        return 0;
    }
    b = pos;
    return dc;
}

template <typename OutputIteratorT>
void to_utf8(uint32_t uchar, OutputIteratorT & oit)
{
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
    template <class, class> class ParserT,
    class ModelT,
    typename IteratorT
>
IteratorT parse(ModelT & model, IteratorT b, IteratorT e)
{
    typedef LexerT<IteratorT> lexer_type;

    lexer_type lexer;
    typename lexer_type::iterator iter = lexer.begin(std::move(b), e), end = lexer.end();

    ParserT<lexer_type, ModelT> parser(model);

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
            tokval = std::string(tok.first, tok.second);
        }
        if (msg.empty()) resultss << "token: '" << tokval << "' is not expected";
        else resultss << msg << ", got: '" << tokval << "'";

        std::string rest;
        copy_not_more(iter->first, e, std::back_inserter(rest), 50);
        resultss << ", stopped at: \"" << rest << "\"";

        throw exception(std::move(resultss.str()));
    }

    if (iter != end) {
        std::string rest;
        copy_not_more(iter->first, e, std::back_inserter(rest), 50);
        throw exception(fmt("parser error: unexpected token, stopped at: \"%1%\"") % rest);
    }

    return std::move(iter->first);
}

}

#endif // SONIA_PARSER_UTILITY_HPP
