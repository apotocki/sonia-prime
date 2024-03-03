//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_attribute_resolver.ipp"
#include "sonia/utility/functional/hash/pair.hpp"

namespace sonia::xmlbuilder {

std::tuple<blob_result, func_type> basic_attribute_resolver::operator()(string_view element, string_view attr_name, string_view attr_value) const
{
    for (;;) {
        auto it = std::find(attr_name.begin(), attr_name.end(), '.');
        if (it == attr_name.end()) break;
        string_view attr{ attr_name.data(), (size_t)(it - attr_name.begin()) };
        auto mit = attr_elem_mapper.find(std::pair{ element, attr }, hasher{}, string_equal_to{});
        if (mit == attr_elem_mapper.end()) {
            throw exception("an attribute mapping for '%1%' of element '%2%' is not registered to parse value '%3%'"_fmt % attr % element % attr_value);
        }
        element = mit->second;
        attr_name = attr_name.substr(attr.size() + 1);
    }

    auto it = attr_parsers.find(std::pair{ element, attr_name }, hasher{}, string_equal_to{});
    if (it == attr_parsers.end()) {
        throw exception("an attribute parser for '%1%' of element '%2%' is not registered to parse value '%3%'"_fmt % attr_name % element % attr_value);
    }
    return it->second->parse(attr_value);
}

void basic_attribute_resolver::setup_fset(string_view element, std::initializer_list<string_view> names)
{
    do_setup_set<functional_attr_parser>(element, names);
}

void basic_attribute_resolver::setup_map(string_view element, string_view attrname, string_view mapped_element)
{
    auto it = attr_elem_mapper.find(std::pair{ element, attrname }, hasher{}, string_equal_to{});
    if (it != attr_elem_mapper.end()) {
        throw exception("an attribute mapping for '%1%' of '%2%' is already registered"_fmt % attrname % element);
    }
    attr_elem_mapper.insert(it, std::pair{ std::pair{ std::string{ element }, std::string{ attrname } }, std::string{ mapped_element } });
}

}
