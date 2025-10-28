//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <string>
#include <iterator>
#include "sonia/utility/parsers/xml/model.hpp"
#include "sonia/utility/xmlbuilder/xmlbuilder.hpp"

namespace sonia::xmlbuilder::detail {

class builder_model : public sonia::parsers::xml::model_base<builder_model>
{
    using base_t = sonia::parsers::xml::model_base<builder_model>;

public:
    explicit builder_model(external_builder& e) : eb{ e } {}

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

    // XML entity decoding helper
    template <typename InputIteratorT, typename OutputIteratorT>
    void decode_xml_entities(InputIteratorT b, InputIteratorT e, OutputIteratorT oit) const;

private:
    void do_close_tag();
    
private:
    external_builder& eb;
    std::vector<element> stack_;
};

}