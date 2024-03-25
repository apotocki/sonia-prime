//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one


#include "sonia/config.hpp"
#include "parser.hpp"

#include "sonia/mp/decimal.ipp"
#include "sonia/utility/parsers/utility.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "sonia/mp/integer_view.hpp"

//#include "maat/lang/environment.hpp"

//using namespace sonia;
//using namespace sonia::lang;
using namespace sonia::lang::beng;

#include "beng.tab.hpp"

using YYSTYPE = beng_lang::parser::semantic_type;
using YYLTYPE = beng_lang::parser::location_type;

#include "beng.yy.hpp"

#include "unit.hpp"

namespace sonia::lang::beng {

/*
expression_list handle_call_op(expression_list& x, expression_list& y, operator_type op)
{

    x.insert(x.end(), std::make_move_iterator(y.begin()), std::make_move_iterator(y.end()));
    x.push_back(op);
    return std::move(x);
    
}
*/

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

void parser_context::push_ns(qname qn)
{
    if (qn.is_absolute()) {
        ns_stack_.push_back(std::move(qn));
    } else {
        ns_stack_.push_back(ns_stack_.back() + qn);
    }
}

void parser_context::pop_ns()
{
    ns_stack_.pop_back();
}

annotated_identifier parser_context::make_identifier(annotated_string_view astr)
{
    return { unit_.slregistry().resolve(astr.value), astr.location };
}

//identifier parser_context::make_required_identifier(string_view str)
//{
//    return identifier{ env_.iregistry().resolve(str).value, true};
//}

small_u32string parser_context::make_string(string_view str)
{
    return utf8_to_utf32(str);
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

int parser_context::make_int(string_view str)
{
    mp::basic_integer<uint64_t, 1> val(str);
    if (val < (std::numeric_limits<int>::min)() || val > (std::numeric_limits<int>::max)()) {
        throw exception("can't convert to int: '%1%'"_fmt % str);
    }
    return (int)val;
}

decimal parser_context::make_numeric(string_view str)
{
    return sonia::decimal::parse(str);
}
//
//u32string parser_context::make_string(string_view str)
//{
//    return utf8_to_utf32(str);
//}

void parser_context::set_declarations(declaration_set_t ds)
{
    declarations_ = std::move(ds);
}

void parser_context::append_error(std::string errmsg)
{
    error_messages_.push_back(std::move(errmsg));
}

void parser_context::parse(fs::path const& f)
{
    auto code = unit_.get_file_content(f, resource_stack_.empty() ? nullptr : &resource_stack_.back());
    resource_stack_.emplace_back(f);
    SCOPE_EXIT([this]{ resource_stack_.pop_back(); });
    parse(string_view { code.data(), code.size() });
}

void parser_context::parse(string_view code)
{
    auto sc_data = std::make_unique<lex::scanner_data>();

    void* scanner;
    beng_langlex_init_extra(sc_data.get(), &scanner);
    SCOPE_EXIT([scanner]() { beng_langlex_destroy(scanner); });

    std::vector<char> buffer;
    YY_BUFFER_STATE st;

    buffer.resize(code.size() + 2);
    std::copy(code.begin(), code.end(), buffer.begin());
    buffer[code.size()] = 0;
    buffer[code.size() + 1] = 0;

    st = beng_lang_scan_buffer(&buffer[0], buffer.size(), scanner);
    SCOPE_EXIT([st, scanner]() { beng_lang_delete_buffer(st, scanner); });

    beng_langset_lineno(1, scanner);
    beng_langset_column(1, scanner);

    beng_lang::parser p(scanner, *this);

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
        throw exception(ss.str());
    }
}

}
