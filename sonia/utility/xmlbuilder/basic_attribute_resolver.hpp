//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "xmlbuilder.hpp"
#include "attr_parser.hpp"

namespace sonia::xmlbuilder {

class basic_attribute_resolver : public attribute_resolver
{
public:
    std::tuple<blob_result, std::string, func_type> operator()(string_view element, string_view attr_name, string_view attr_value) override;

    template <typename AttrT>
    void setup_set(string_view element, std::initializer_list<string_view> names)
    {
        do_setup_set<particular_attr_parser<AttrT>>(element, names);
    }

    void setup_fset(string_view element, std::initializer_list<string_view> names);

    void setup_map(string_view element, string_view attrname, string_view mapped_element);

private:
    template <typename ParserT>
    void do_setup_set(string_view element, std::initializer_list<string_view> names);

    boost::unordered_map<std::pair<std::string, std::string>, automatic_attr_parser_t, hasher> attr_parsers;
    boost::unordered_map<std::pair<std::string, std::string>, std::string, hasher> attr_elem_mapper;
};

}
