//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <functional>
#include "sonia/utility/parsers/utility.hpp"

#include "lexems.hpp"

namespace sonia::parsers::xml {

template <class LexerT, class ModelT>
class parser
{
    using iterator = typename LexerT::iterator;
    using token_type = typename LexerT::token_type;

public:
    explicit parser(ModelT & mdl) : mdl_{mdl} {}

    void parse(iterator & b, iterator const& e) const;

private:
    bool prolog(iterator & b, iterator const& e) const;
    bool xml_decl(iterator & b, iterator const& e) const;
    bool misc(iterator & b, iterator const& e) const;
    bool doctypedecl(iterator & b, iterator const& e) const;
    bool extenal_id(iterator & b, iterator const& e) const;
    bool int_subset(iterator & b, iterator const& e) const;

    bool element(iterator & b, iterator const& e) const;
    bool attribute(iterator & b, iterator const& e) const;
    bool content(iterator & b, iterator const& e) const;
    //static void throw_unexpected_eof();
    //static bool ws(iterator & b, iterator const& e);

    //void skip_ws(iterator & b, iterator const& e) const;

    ModelT & mdl_;
};

}

// Implementation
namespace sonia::parsers::xml {

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::xml_decl(iterator & b, iterator const& e) const
{
    if (b->id != ID_XML_DECL_BEGIN) return false;
    ++b;
    if (!valid(b, e) || b->id != ID_VERSION_NUM)  {
        throw exception("VersionNum is expected");
    }
    mdl_.put_string(b->first, b->second);
    mdl_.on_version();
    ++b;
    if (valid(b, e) && b->id == ID_ENCODING) {
        ++b;
        if (!valid(b, e) || b->id != ID_ENC_NAME) {
            throw exception("EncName is expected");
        }
        mdl_.put_string(b->first, b->second);
        mdl_.on_encoding();
        ++b;
    }
    if (valid(b, e) && b->id == ID_SDECL) {
        ++b;
        if (!valid(b, e) || b->id != ID_ENC_NAME) throw exception("SDDecl value is expected");
        mdl_.put_string(b->first, b->second);
        mdl_.on_standalone();
        ++b;
    }

    if (!valid(b, e) || b->id != ID_XML_DECL_END) {
        throw exception("The end of XMLDecl is expected");
    }
    ++b;
    return true;
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::misc(iterator & b, iterator const& e) const
{
    if (!valid(b, e)) return false;
    if (b->id == ID_COMMENT || b->id == ID_SPACE) {
        ++b;
        return true;
    }
    if (b->id == ID_PI) {
        mdl_.on_pi(b->first, b->second);
        ++b;
        return true;
    }
    return false;
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::extenal_id(iterator & b, iterator const& e) const
{
    if (b->id != ID_SPACE) return false;
    if (++b; !valid(b, e)) return false;
    if (b->id == ID_SYSTEM) {
        if (++b; !valid(b, e) || b->id != ID_SPACE) throw exception("SystemLiteral is expected");
        if (++b; !valid(b, e) || b->id != ID_STRING) throw exception("SystemLiteral is expected");
        mdl_.put_string(b->first, b->second);
        mdl_.on_system_literal();
    } else if (b->id == ID_PUBLIC) {
        if (++b; !valid(b, e) || b->id != ID_SPACE) throw exception("PubidLiteral is expected");
        if (++b; !valid(b, e) || b->id != ID_STRING) throw exception("PubidLiteral is expected");
        mdl_.put_string(b->first, b->second);
        mdl_.on_pubid_literal();
        if (++b; !valid(b, e) || b->id != ID_SPACE) throw exception("SystemLiteral is expected");
        if (++b; !valid(b, e) || b->id != ID_STRING) throw exception("SystemLiteral is expected");
        mdl_.put_string(b->first, b->second);
        mdl_.on_system_literal();
    } else {
        return false;
    }
    ++b;
    return true;
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::int_subset(iterator & b, iterator const& e) const
{
    if (b->id != ID_OPEN_SQUARE_BRACKET) return false;
    ++b;
    if (!valid(b, e)) throw exception("expected ']'");
    // to do: implementation
    // [28b]   	intSubset	   ::=   	(markupdecl | DeclSep)*
    // [29]   	markupdecl	   ::=   	elementdecl | AttlistDecl | EntityDecl | NotationDecl | PI | Comment
    if (b->id != ID_CLOSE_SQUARE_BRACKET) throw exception("expected ']'");
    ++b;
    return true;
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::doctypedecl(iterator & b, iterator const& e) const
{
    if (!valid(b, e) || b->id != ID_DOCTYPE_BEGIN) return false;
    if (++b; !valid(b, e) || (b->id != ID_NAME && b->id != ID_SYSTEM && b->id != ID_PUBLIC)) {
        throw exception("DOCTYPE Name is expected");
    }
    mdl_.put_string(b->first, b->second, 0);
    mdl_.on_start_doctype();
    ++b;
    if (!valid(b, e)) throw exception("expected '>'");
    extenal_id(b, e);
    if (!valid(b, e)) throw exception("expected '>'");
    if (b->id == ID_SPACE) {
        if (++b; !valid(b, e)) throw exception("expected '>'");
    }
    int_subset(b, e);
    if (!valid(b, e)) throw exception("expected '>'");
    if (b->id == ID_SPACE) {
        if (++b; !valid(b, e)) throw exception("expected '>'");
    }
    if (b->id != ID_CLOSE_BROKET) throw exception("expected '>'");
    ++b;
    return true;
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::prolog(iterator & b, iterator const& e) const
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    if (!valid(b, e) || (!xml_decl(b, e) && !mdl_.is_xml_decl_optional()) ) return false;

    star(b, e, std::bind(&parser::misc, this, _1, _2));
    if (valid(b, e) && doctypedecl(b, e)) {
        star(b, e, std::bind(&parser::misc, this, _1, _2));
    }
    return true;
}

template <class LexerT, class ModelT>
void parser<LexerT, ModelT>::parse(iterator & b, iterator const& e) const
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    if (!valid(b, e) || !prolog(b, e)) {
        throw exception("expected XMLDecl");
    }
    if (!valid(b, e) || !element(b, e)) {
        throw exception("expected element");
    }
    star(b, e, std::bind(&parser::content, this, _1, _2));
    star(b, e, std::bind(&parser::misc, this, _1, _2));
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::element(iterator & b, iterator const& e) const
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    if (b->id != ID_OPEN_BROKET) return false;
    if (++b; !valid(b, e) || b->id != ID_NAME) {
        throw exception("element name is expected");
    }
    mdl_.put_string(b->first, b->second, 0);
    mdl_.on_begin_element();
    ++b;
    star(b, e, std::bind(&parser::attribute, this, _1, _2)); // also eats the space
    if (!valid(b, e)) throw exception("'/>' is expected");
    if (b->id == ID_CLOSE_EMPTY_TAG) {
        mdl_.on_close_element();
        ++b;
        return true;
    } else if (b->id == ID_CLOSE_BROKET) {
        mdl_.on_end_element();
        ++b;
    } else {
        throw exception("'/>' is expected");
    }
    return true;
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::attribute(iterator & b, iterator const& e) const
{
    if (b->id != ID_SPACE) return false;
    if (++b; !valid(b, e) || b->id != ID_NAME) return false;
    mdl_.put_string(b->first, b->second, 0);
    mdl_.on_attribute_name();
    if (++b; !valid(b, e) || b->id != ID_EQ) {
        throw exception("'=' is expected");
    }
    if (++b; !valid(b, e) || b->id != ID_STRING) {
        throw exception("attribute value is expected");
    }
    mdl_.put_string(b->first, b->second);
    mdl_.on_attribute_value();
    ++b;
    return true;    
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::content(iterator & b, iterator const& e) const
{
    if (b->id == ID_SPACE || b->id == ID_CHAR_DATA) {
        mdl_.put_string(b->first, b->second, 0);
        mdl_.on_char_data();
    } else if (b->id == ID_CDATA) {
        mdl_.put_cdata_string(b->first, b->second);
    } else if (b->id == ID_PI) {
        mdl_.on_pi(b->first, b->second);
    } else if (b->id == ID_COMMENT) {
        
    } else if (b->id == ID_ETAG) {
        if (++b; !valid(b, e) || b->id != ID_NAME) {
            throw exception("ETag name is expected");
        }
        mdl_.put_string(b->first, b->second, 0);
        mdl_.on_close_tag();
        if (++b; !valid(b, e)) throw exception("'>' is expected");
        if (b->id == ID_SPACE) {
            ++b;
            if (!valid(b, e)) throw exception("'>' is expected");
        }
        if (b->id != ID_CLOSE_BROKET) throw exception("'>' is expected");
    } else if (element(b, e)) {
        return true;
    } else {
        return false;
    }
    ++b;
    return true;
}

#if 0
template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::ws(iterator & b, iterator const& e)
{
    auto tokid = b->id;
    if (tokid == ID_SPACE || tokid == ID_LINE_COMMENT || tokid == ID_COMMENT) {
        ++b;
        return true;
    }
    return false;
}

template <class LexerT, class ModelT>
void parser<LexerT, ModelT>::skip_ws(iterator & b, iterator const& e) const
{
    star(b, e, &this->ws);
}

template <class LexerT, class ModelT>
void parser<LexerT, ModelT>::throw_unexpected_eof()
{
    throw exception("unexpected eof");
}
#endif

}
