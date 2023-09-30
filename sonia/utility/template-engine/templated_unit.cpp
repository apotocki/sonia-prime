//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "templated_unit.hpp"
#include "sonia/utility/parsers/template/parser.hpp"
#include "sonia/utility/parsers/template/lexertl_lexer.hpp"

#include "sonia/logger/logger.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace sonia {

void templated_unit::build(span<const char> code)
{
    result_builder_.str("");
    parsers::parse<
        parsers::tmpl::lexertl_lexer,
        parsers::tmpl::parser
    >(*this, code.data(), code.data() + code.size());

    //GLOBAL_LOG_INFO() << "RESULT: " << result_builder_.str();
}

//void templated_unit::run(string_view prefix)
//{
//    if (!prefix.empty()) {
//        lua::language::append_code(std::string{ prefix });
//    }
//    lua::language::append_code(result());
//}

// model
void templated_unit::write_escaped(string_view s)
{
    for (char c : s) {
        switch (c) {
        case '\r':
            result_builder_ << "\\r";
            break;
        case '\n':
            result_builder_ << "\\n";
            break;
        case '\t':
            result_builder_ << "\\t";
            break;
        case '\\':
            result_builder_ << "\\\\";
            break;
        case '\"':
            result_builder_ << "\\\"";
            break;
        case '\'':
            result_builder_ << "\\'";
            break;
        //case '[':
        //    result_builder_ << "\\[";
        //    break;
        //case ']':
        //    result_builder_ << "\\]";
        //    break;
        default:
            result_builder_ << c;
        }
    }
}

void templated_unit::write_text_line(string_view l, bool eol)
{
    result_builder_ << "writer(\"";
    write_escaped(l);
    if (eol) {
        result_builder_ << "\\n\")\n";
    } else {
        result_builder_ << "\")\n";
    }
}

void templated_unit::commit_text()
{
    string_view text{ tempstr_.data(), tempstr_.size() };

    if (text.empty()) return;
    std::vector<string_view> strs;
    boost::split(strs, text, boost::is_any_of("\n"), boost::token_compress_off);
    if (!strs.empty()) {
        std::span lines{ strs };
        std::string first{ strs.front() };
        if (text_need_trim_left_) {
            boost::trim(first);
            if (first.empty()) {
                lines = lines.subspan(1);
            }
        }
        for (auto sv : lines) {
            bool last = sv.data() == strs.back().data() && sv.size() == strs.back().size();
            while (!sv.empty() && sv.back() == '\r') sv = sv.substr(0, sv.size() - 1);
            write_text_line(sv, !last);
            //GLOBAL_LOG_INFO() << "TEXT LINE: '" << sv << "'";
        }
    }
    //GLOBAL_LOG_INFO() << "TEXT: " << string_view{ tempstr_.data(), tempstr_.size() };
    model::commit_text();
    text_need_trim_left_ = false;
}

void templated_unit::commit_expr()
{
    if (tempstr_.empty()) return;
    result_builder_ << "writer(tostring(" << string_view{ tempstr_.data(), tempstr_.size() } << "))\n";
    //GLOBAL_LOG_INFO() << "EXPR: " << string_view{ tempvar_.data(), tempvar_.size() };
    model::commit_expr();
    text_need_trim_left_ = false;
}

void templated_unit::commit_code()
{
    if (tempstr_.empty()) return;
    result_builder_ << string_view{ tempstr_.data(), tempstr_.size() } << "\n";
    //GLOBAL_LOG_INFO() << "CODE: " << string_view{ tempstr_.data(), tempstr_.size() };
    model::commit_code();
    text_need_trim_left_ = true;
}

}
