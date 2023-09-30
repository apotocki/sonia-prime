//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "basic_attribute_resolver.hpp"
#include "attr_parser.ipp"

#include "sonia/utility/functional/hash/pair.hpp"

namespace sonia::xmlbuilder {

template <typename ParserT>
void basic_attribute_resolver::do_setup_set(string_view element, std::initializer_list<string_view> names)
{
    for (string_view attrname : names) {
        auto it = attr_parsers.find(std::pair{ element, attrname }, hasher{}, string_equal_to{});
        if (it != attr_parsers.end()) {
            throw exception("an attribute parser for '%1%' of '%2%' is already registered"_fmt % attrname % element);
        }
        attr_parsers.insert(it, std::pair{ std::pair{ std::string{ element }, std::string{ attrname } }, in_place_type<ParserT> });
    }
}

}
