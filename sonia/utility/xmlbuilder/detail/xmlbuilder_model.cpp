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

    eb.append_element(std::span{ stack_ }.first(stack_.size() - 1), e);
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
        throw exception("unexpected closing tag, no closing expected");
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

    eb.close_element(std::span{ stack_ }.first(stack_.size() - 1), e);
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

template <typename InputIteratorT, typename OutputIteratorT>
void builder_model::decode_xml_entities(InputIteratorT b, InputIteratorT e, OutputIteratorT oit) const
{
    auto it = b;
    while (it != e) {
        if (*it == '&') {
            auto entity_start = it;
            ++it;
            if (it == e) {
                // Incomplete entity, just copy the &
                *oit = '&';
                ++oit;
                break;
            }

            // Find the end of the entity
            auto entity_end = it;
            while (entity_end != e && *entity_end != ';') {
                ++entity_end;
            }

            if (entity_end == e) {
                // Incomplete entity, copy as is
                while (entity_start != e) {
                    *oit = *entity_start;
                    ++oit;
                    ++entity_start;
                }
                break;
            }

            // Parse the entity
            std::string entity_name(it, entity_end);
            if (entity_name == "lt") {
                *oit = '<';
                ++oit;
            }
            else if (entity_name == "gt") {
                *oit = '>';
                ++oit;
            }
            else if (entity_name == "amp") {
                *oit = '&';
                ++oit;
            }
            else if (entity_name == "quot") {
                *oit = '"';
                ++oit;
            }
            else if (entity_name == "apos") {
                *oit = '\'';
                ++oit;
            }
            else if (!entity_name.empty() && entity_name[0] == '#') {
                // Numeric character reference
                bool is_hex = false;
                size_t start_pos = 1;
                if (entity_name.size() > 1 && (entity_name[1] == 'x' || entity_name[1] == 'X')) {
                    is_hex = true;
                    start_pos = 2;
                }

                if (start_pos < entity_name.size()) {
                    try {
                        uint32_t code_point;
                        if (is_hex) {
                            code_point = std::stoul(entity_name.substr(start_pos), nullptr, 16);
                        }
                        else {
                            code_point = std::stoul(entity_name.substr(start_pos), nullptr, 10);
                        }

                        // Convert Unicode code point to UTF-8
                        if (code_point <= 0x7F) {
                            *oit = static_cast<char>(code_point);
                            ++oit;
                        }
                        else if (code_point <= 0x7FF) {
                            *oit = static_cast<char>(0xC0 | (code_point >> 6));
                            ++oit;
                            *oit = static_cast<char>(0x80 | (code_point & 0x3F));
                            ++oit;
                        }
                        else if (code_point <= 0xFFFF) {
                            *oit = static_cast<char>(0xE0 | (code_point >> 12));
                            ++oit;
                            *oit = static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                            ++oit;
                            *oit = static_cast<char>(0x80 | (code_point & 0x3F));
                            ++oit;
                        }
                        else if (code_point <= 0x10FFFF) {
                            *oit = static_cast<char>(0xF0 | (code_point >> 18));
                            ++oit;
                            *oit = static_cast<char>(0x80 | ((code_point >> 12) & 0x3F));
                            ++oit;
                            *oit = static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                            ++oit;
                            *oit = static_cast<char>(0x80 | (code_point & 0x3F));
                            ++oit;
                        }
                        // Invalid code points are silently ignored
                    }
                    catch (...) {
                        // Invalid numeric entity, copy as is
                        while (entity_start != entity_end + 1) {
                            *oit = *entity_start;
                            ++oit;
                            ++entity_start;
                        }
                    }
                }
                else {
                    // Empty numeric entity, copy as is
                    while (entity_start != entity_end + 1) {
                        *oit = *entity_start;
                        ++oit;
                        ++entity_start;
                    }
                }
            }
            else {
                // Unknown entity, copy as is
                while (entity_start != entity_end + 1) {
                    *oit = *entity_start;
                    ++oit;
                    ++entity_start;
                }
            }

            it = entity_end + 1; // Skip past the ';'
        }
        else {
            *oit = *it;
            ++oit;
            ++it;
        }
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
