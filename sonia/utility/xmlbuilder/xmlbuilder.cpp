//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "xmlbuilder.hpp"

#include "sonia/utility/parsers/xml/model.hpp"
#include "sonia/utility/parsers/xml/lexertl_lexer.hpp"
#include "sonia/utility/parsers/xml/parser.hpp"

#include "sonia/singleton.hpp"
#include "sonia/logger/logger.hpp"

#include <boost/algorithm/string/trim.hpp>

namespace sonia::xmlbuilder {

std::string basic_external_builder::generate_id() const
{
    std::ostringstream namess;
    namess << "_id" << id_counter_++;
    return namess.str();
}

void basic_external_builder::append_element(span<element> parents, element & e)
{
    if (parents.empty()) return; // skip root

    if (e.id.empty()) {
        e.id = generate_id();
    }
    
    create(e.name, e.id);

    // is parent an attribute tag?
    if (parents.size() > 1) {
        element & e_parent = parents.back();
        element const& e_snd_parent = parents[parents.size() - 2];
        if (e_parent.name.starts_with(e_snd_parent.name) && e_parent.name.size() > e_snd_parent.name.size() && e_parent.name[e_snd_parent.name.size()] == '.') {
            if (!e_parent.text.empty()) {
                e_parent.text.push_back(',');
            }
            e_parent.text.push_back('$');
            e_parent.text.append(e.id);
            return;
        }
    }
    
    if (parents.size() > 1) {
        append(parents.back().id, e.id);
    } else { // parents.size() == 1
        append_to_document(e.id);
    }
}

void basic_external_builder::close_element(span<element> parents, element& e)
{
    // is attribute tag?
    if (!e.text.empty()) {
        set_text(e.id, e.text);
    }

    for (auto const& attr : e.attrs) {
        set_property(e.id, attr.first, *attr.second);
    }
    for (auto const& func : e.functionals) {
        set_property_functional(e.id, std::get<0>(func), std::get<1>(func), std::get<2>(func));
    }
}

class builder_model : public sonia::parsers::xml::model_base<builder_model>
{
    using base_t = sonia::parsers::xml::model_base<builder_model>;

public:
    explicit builder_model(external_builder& e, attribute_resolver& a) : eb{e}, ar{a} {}

    bool is_xml_decl_optional() const { return true; }

    void on_begin_element();
    void on_end_element();// on '>'
    void on_close_element(); // on '/>'
    void on_close_tag();// on '</' name

    void on_char_data(); // element's text

    template <typename IteratorT>
    void put_cdata_string(IteratorT b, IteratorT e)
    {
        base_t::put_cdata_string(std::move(b), std::move(e));
        on_char_data();
    }

    void on_attribute_value();

private:
    void do_close_tag();

private:
    external_builder& eb;
    attribute_resolver& ar;
    std::vector<element> stack_;
};

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

            auto tpl = ar(e_parent.name, attrname, e.text);
            if (!std::get<1>(tpl).empty()) {
                e_parent.functionals.emplace_back(std::string{ attrname }, std::move(std::get<1>(tpl)), std::get<2>(tpl));
            } else {
                e_parent.attrs.emplace_back(std::string{ attrname }, std::move(std::get<0>(tpl)));
            }
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
        auto tpl = ar(e.name, attrname, attrvalue);
        //auto tpl = attr_type_mapping::parse(attrname, attrvalue, eb);
        if (!std::get<1>(tpl).empty()) {
            e.functionals.emplace_back(std::string{attrname}, std::move(std::get<1>(tpl)), std::get<2>(tpl));
        } else {
            e.attrs.emplace_back(std::string{attrname}, std::move(std::get<0>(tpl)));
        }
    }
}

//////////////////////////////////////////////////////////////////

void parse(string_view code, external_builder& eb, attribute_resolver& ar)
{
    builder_model model{ eb, ar };

    sonia::parsers::parse<
        sonia::parsers::xml::lexertl_lexer,
        sonia::parsers::xml::parser
    >(model, code.data(), code.data() + code.size());

    //return { std::move(model.vars), std::move(model.result) };

}


/*

create_component(typename, id)
set_component_property(component id, property name, property value)
append(parent component id, child component id)

*/

}
