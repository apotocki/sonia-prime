//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "xmlbuilder_model.hpp"

#include <boost/algorithm/string/trim.hpp>

namespace sonia::xmlbuilder::detail {

void builder_model::on_begin_element()
{
    auto tag = to_string_view(tempstr_);
    stack_.emplace_back();
    stack_.back().name = std::string{tag};
    //GLOBAL_LOG_INFO() << "begin " << tag;
}

void builder_model::on_end_element()
{
    element& e = stack_.back();

    // is attribute tag?
    if (stack_.size() > 1) {
        element& e_parent = stack_[stack_.size() - 2];
        if (e.name.starts_with(e_parent.name) && e.name.size() > e_parent.name.size() && e.name[e_parent.name.size()] == '.') {
            // just skip for now
            return;
        }
    }

    eb.append_element(std::span{ stack_ }.subspan(0, stack_.size() - 1), e);
}

void builder_model::on_close_element()
{
    on_end_element();
    do_close_tag();
}

void builder_model::on_close_tag()
{
    auto tag = to_string_view(tempstr_);
    if (stack_.empty()) {
        throw exception("unexpected closing tag, no closeing expected");
    }
    if (stack_.empty() || stack_.back().name != tag) {
        throw exception("unexpected closing tag, expected </%1%>"_fmt % stack_.back().name);
    }
    do_close_tag();
    //GLOBAL_LOG_INFO() << "end " << tag;
}

void builder_model::do_close_tag()
{
    element& e = stack_.back();
    
    // is attribute tag?
    if (stack_.size() > 1) {
        element& e_parent = stack_[stack_.size() - 2];
        if (e.name.starts_with(e_parent.name) && e.name.size() > e_parent.name.size() && e.name[e_parent.name.size()] == '.') {
            // the element's text is the attribute value
            size_t parent_tag_name_sz = e_parent.name.size();
            string_view attrname{ e.name.data() + parent_tag_name_sz + 1, e.name.size() - parent_tag_name_sz - 1 };

            e_parent.attrs.emplace_back(attrname, e.text);
            /*
            auto tpl = ar(e_parent.name, attrname, e.text);
            if (!std::get<1>(tpl).empty()) {
                e_parent.functionals.emplace_back(std::string{ attrname }, std::move(std::get<1>(tpl)), std::get<2>(tpl));
            } else {
                e_parent.attrs.emplace_back(std::string{ attrname }, std::move(std::get<0>(tpl)));
            }
            */
            stack_.pop_back();
            return;
        }
    }

    eb.close_element(std::span{ stack_ }.subspan(0, stack_.size() - 1), e);
    //eb.append_element(std::span{ stack_ }.subspan(0, stack_.size() - 1), e); //see on_end_element
    stack_.pop_back();
}

void builder_model::on_char_data()
{
    boost::algorithm::trim(tempstr_);
    if (tempstr_.empty()) return;
    
    element& e = stack_.back();
    e.text += string_view(tempstr_.data(), tempstr_.size());
    //GLOBAL_LOG_INFO() << string_view(tempstr_.data(), tempstr_.size());
}

void builder_model::on_attribute_value()
{
    element& e = stack_.back();
    auto attrname = to_string_view(attr_name_);
    auto attrvalue = to_string_view(tempstr_);
    if (attrname == "id"sv) {
        e.id = std::string{attrvalue};
    } else {
        if (attrname.starts_with("xmlns"sv)) [[unlikely]] {
            if (attrname == "xmlns"sv) {
                if (!e.ns.empty()) [[unlikely]] {
                    throw exception("duplicate <%1%> element's namespace definition found"_fmt % e.name);
                }
                e.ns = attrvalue;
                return;
            }
            if (attrname[5] == ':') {
                string_view nsprefix = attrname.substr(6);
                e.prefixes.emplace_back(nsprefix, std::string{ attrvalue });
                return;
            }
        }

        e.attrs.emplace_back(attrname, attrvalue);
    }
}

}

#include "sonia/utility/parsers/xml/lexertl_lexer.hpp"
#include "sonia/utility/parsers/xml/parser.hpp"

void sonia::xmlbuilder::parse(string_view code, external_builder& eb)
{
    sonia::xmlbuilder::detail::builder_model model{ eb };

    sonia::parsers::parse<
        sonia::parsers::xml::lexertl_lexer,
        sonia::parsers::xml::parser
    >(model, code.data(), code.data() + code.size());
}
