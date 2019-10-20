//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_XML_LEXER_HPP
#define SONIA_UTILITY_XML_LEXER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/parsers/lexer.hpp"

#include "lexems.hpp"

namespace sonia::parsers::xml {

template <typename DerivedT, typename IteratorT>
class lexertl_lexer_base : public lexer_base<DerivedT, IteratorT, true>
{
protected:
    using lexertl_lexer_base_t = lexertl_lexer_base;

public:
    template <typename RulesT>
    static void build_rules(RulesT & rules)
    {
        rules.push_state("IN_TAG");
        rules.push_state("IN_DECL");
        rules.push_state("DOCTYPE");
        rules.push("INITIAL,IN_DECL,DOCTYPE,IN_TAG", "[\\x20\\x9\\xd\\xa]+", ID_SPACE, ".");

        rules.push("INITIAL", "<\\?xml[\\x20\\x9\\xd\\xa]+version[\\x20\\x9\\xd\\xa]*=[\\x20\\x9\\xd\\xa]*", ID_XML_DECL_BEGIN, ">IN_DECL");
        rules.push("IN_DECL", "(\\\"\\d\\.\\d\\\")|('\\d\\.\\d')", ID_VERSION_NUM, ".");
        rules.push("IN_DECL", "[\\x20\\x9\\xd\\xa]+encoding[\\x20\\x9\\xd\\xa]*=[\\x20\\x9\\xd\\xa]*", ID_ENCODING, ".");
        rules.push("IN_DECL", "\\\"[A-Za-z][A-Za-z\\d\\._\\-]*\\\"", ID_ENC_NAME, ".");
        rules.push("IN_DECL", "'[A-Za-z][A-Za-z\\d\\._\\-]*'", ID_ENC_NAME, ".");
        rules.push("IN_DECL", "[\\x20\\x9\\xd\\xa]+standalone[\\x20\\x9\\xd\\xa]*=[\\x20\\x9\\xd\\xa]*", ID_SDECL, ".");
        rules.push("IN_DECL", "[\\x20\\x9\\xd\\xa]*\\?>", ID_XML_DECL_END, "<");

        rules.push("INITIAL", "<!--.{+}[\r\n]*?-->", ID_COMMENT, ".");
        rules.push("INITIAL", "<\\?([^Xx\\d]|([Xx][^Mm])|([Xx][Mm][^Ll]))\\w{+}[\\x20\\x9\\xd\\xa]*?\\?>", ID_PI, ".");
        rules.push("INITIAL", "<!\\[CDATA\\[.{+}[\r\n]*?\\]\\]>", ID_CDATA, ".");

        rules.push("INITIAL", "<!DOCTYPE[\\x20\\x9\\xd\\xa]+", ID_DOCTYPE_BEGIN, ">DOCTYPE");
        rules.push("DOCTYPE", "SYSTEM", ID_SYSTEM, ".");
        rules.push("DOCTYPE", "PUBLIC", ID_PUBLIC, ".");
        rules.push("DOCTYPE", "\\[", ID_OPEN_SQUARE_BRACKET, ".");
        rules.push("DOCTYPE", "\\]", ID_CLOSE_SQUARE_BRACKET, ".");
        rules.push("DOCTYPE,IN_TAG", "[:A-Za-z_][:A-Za-z_\\d\\-\\.]*", ID_NAME, "."); // to do: add other ranges
        rules.push("DOCTYPE,IN_TAG", ">", ID_CLOSE_BROKET, "<");

        rules.push("INITIAL", "<", ID_OPEN_BROKET, ">IN_TAG");
        rules.push("INITIAL", "<\\/", ID_ETAG, ">IN_TAG");
        rules.push("IN_TAG", "[\\x20\\x9\\xd\\xa]*=[\\x20\\x9\\xd\\xa]*", ID_EQ, ".");
        rules.push("IN_TAG", "\\\"[^\\r\\n\\\"]*\\\"", ID_STRING, ".");
        rules.push("IN_TAG", "'[^'\\r\\n]*'", ID_STRING, ".");
        rules.push("IN_TAG", "\\/>", ID_CLOSE_EMPTY_TAG, "<");
        
        rules.push("INITIAL", "[^<]+", ID_CHAR_DATA, ".");
    }
};

template <typename IteratorT>
class lexertl_lexer : public lexertl_lexer_base<lexertl_lexer<IteratorT>, IteratorT>
{};

}

#endif // SONIA_UTILITY_XML_LEXER_HPP
