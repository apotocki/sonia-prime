//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one


#include "sonia/config.hpp"
#include "parser.hpp"

#include "sonia/mp/decimal.ipp"
#include "sonia/utility/parsers/utility.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "sonia/mp/integer_view.hpp"

//using namespace sonia;
//using namespace sonia::lang;
using namespace sonia::lang::bang;

#include "bang.tab.hpp"

using YYSTYPE = bang_lang::parser::semantic_type;
using YYLTYPE = bang_lang::parser::location_type;

#include "bang.yy.hpp"

#include "unit.hpp"
#include "sonia/bang/utility/linked_list.ipp"

namespace sonia::lang::bang {

class syntax_expression_resource : public lex::code_resource
{
public:
    managed_syntax_expression_list expressions;
    inline explicit syntax_expression_resource(unit& u) noexcept : expressions{ u } {}
};

class file_resource : public syntax_expression_resource
{
    fs::path path_;

public:
    inline file_resource(unit& u, fs::path p) noexcept
        : syntax_expression_resource{ u }
        , path_{ std::move(p) }
    {}

    small_string description() const override
    {
        auto u8str = path_.generic_u8string();
        return small_string{ reinterpret_cast<char const*>(u8str.data()), u8str.size() };
    }

    inline fs::path const& path() const noexcept { return path_; }
};

class string_resource : public syntax_expression_resource
{
    std::string src_;

public:
    inline explicit string_resource(unit& u, string_view s)
        : syntax_expression_resource{ u }
        , src_{ s }
    {}

    small_string description() const override
    {
        std::ostringstream result;
        result << '`' << src_ << '`';
        return small_string{ result.str() };
    }
};

small_u32string utf8_to_utf32(string_view sv)
{
    boost::container::small_vector<char32_t, 64> result;
    result.reserve(sv.size());
    for (auto it = sv.begin(), eit = sv.end(); it != eit;) {
        char32_t c32;
        if (!parsers::to_utf32(it, eit, c32)) {
            throw exception("wrong utf8 sequence");
        }
        result.push_back(c32);
    }
    return small_u32string{ result.data(), result.size() };
}

shared_ptr<lex::code_resource> parser_context::get_resource() const noexcept
{
    return resource_;
}

identifier parser_context::new_identifier() const
{
    return unit_.new_identifier();
}

annotated_identifier parser_context::make_identifier(annotated_string_view astr) const
{
    return { unit_.slregistry().resolve(astr.value), astr.location };
}

annotated_qname parser_context::make_qname(annotated_string_view astr) const
{
    return annotated_qname{ qname{unit_.slregistry().resolve(astr.value), false}, std::move(astr.location) };
}

//annotated_qname_identifier parser_context::make_qname_identifier(annotated_qname aqn) const
//{
//    return annotated_qname_identifier{ unit_.fregistry().resolve(aqn.value).id(), aqn.location };
//}
//
//annotated_qname_identifier parser_context::make_qname_identifier(annotated_string_view asv, bool is_abs) const
//{
//    return annotated_qname_identifier{ unit_.fregistry().resolve(qname{unit_.slregistry().resolve(asv.value), is_abs}).id(), asv.location };
//}

//identifier parser_context::make_required_identifier(string_view str)
//{
//    return identifier{ env_.iregistry().resolve(str).value, true};
//}

annotated_string parser_context::make_string(annotated_string_view str) const
{
    boost::container::small_vector<char, 128> buff;
    parsers::normilize_json_string(str.value.begin(), str.value.end(), std::back_inserter(buff));
    //return {utf8_to_utf32(str.value), str.location};
    return { small_string{buff.data(), buff.size()}, str.location };
}

//integer_literal parser_context::make_integer_literal(string_view str)
//{
//    auto it = std::find(str.begin(), str.end(), '_');
//    BOOST_ASSERT(it != str.end());
//    return integer_literal{ 
//        sonia::decimal::parse(string_view(str.begin(), it)),
//        qname{make_identifier(string_view(it + 1, str.end()))}
//    };
//}

mp::integer parser_context::make_integer(string_view str) const
{
    return mp::integer(str);
}
//
mp::decimal parser_context::make_decimal(string_view str) const
{
    return mp::decimal(str);
}
//
//u32string parser_context::make_string(string_view str)
//{
//    return utf8_to_utf32(str);
//}

void parser_context::append_error(std::string errmsg)
{
    error_messages_.push_back(std::move(errmsg));
}

syntax_expression_entry& parser_context::push(syntax_expression_t&& e)
{
    return unit_.push_back_expression(resource_->expressions, std::move(e));
}

managed_statement_list parser_context::new_statement_list() const
{
    return managed_statement_list{ unit_ };
}

statement_span parser_context::push(managed_statement_list&& msl)
{
    statement_span result = msl;
    statements_.splice_back(msl);
    BOOST_ASSERT(!msl);
    return result;
}

void parser_context::set_root_statements(managed_statement_list&& sts)
{
    root_statements_ = push(std::move(sts));
}

std::expected<statement_span, std::string> parser_context::parse(fs::path const& f, fs::path const* base_path)
{
    std::vector<char> code;
    try {
        code = unit_.get_file_content(f, base_path);
    } catch (std::exception const& e) {
        return std::unexpected(e.what());
    }
    resource_ = make_shared<file_resource>(unit_, f);
    return parse(string_view { code.data(), code.size() });
}

std::expected<statement_span, std::string> parser_context::parse_string(string_view code)
{
    resource_ = make_shared<string_resource>(unit_, code);
    return parse(code);
}

std::expected<statement_span, std::string> parser_context::parse(string_view code)
{
    auto sc_data = std::make_unique<lex::scanner_data>();

    void* scanner;
    bang_langlex_init_extra(sc_data.get(), &scanner);
    SCOPE_EXIT([scanner]() { bang_langlex_destroy(scanner); });

    std::vector<char> buffer;
    YY_BUFFER_STATE st;

    buffer.resize(code.size() + 2);
    std::copy(code.begin(), code.end(), buffer.begin());
    buffer[code.size()] = 0;
    buffer[code.size() + 1] = 0;

    st = bang_lang_scan_buffer(&buffer[0], buffer.size(), scanner);
    SCOPE_EXIT([st, scanner]() { bang_lang_delete_buffer(st, scanner); });

    bang_langset_lineno(1, scanner);
    bang_langset_column(1, scanner);

    bang_lang::parser p(scanner, *this);

    bool parse_result = true;
    try {
        parse_result = p.parse();
    } catch (std::exception const& e) {
        append_error(e.what());
    }
    if (parse_result || !error_messages_.empty()) {
        for (auto const& l : sc_data->loc_stack) {
            append_error(("%1%:%2%.%3%: error: unterminated /* comment"_fmt
                % get_resource()
                % l.line % l.column).str());
        }

        std::ostringstream ss;
        for (auto const& err : error_messages_) {
            if (&err != &error_messages_.front()) ss << "\n";
            ss << err;
        }
        return std::unexpected(ss.str());
    }
    return root_statements_;
}

}
