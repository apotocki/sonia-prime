//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "xmlbuilder.hpp"

#include <sstream>

#include "sonia/singleton.hpp"
#include "sonia/logger/logger.hpp"

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
    if (parents.size() > 1) { // <some-tag><some-tag.some-attr><e>
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
    
    //if (parents.size() > 1) {
    //    append(parents.back(), e);
    //} else { // parents.size() == 1
    //    append_to_document(e);
    //}
}

void basic_external_builder::close_element(span<element> parents, element& e)
{
    for (auto const& attr_pair : e.attrs) {
        auto const& attrname = attr_pair.first;
        auto const& attrvalue = attr_pair.second;
        set_property(e, attrname, attrvalue);
        /*
        auto tpl = ar()(e.name, attrname, attrvalue);
        if (std::get<0>(tpl).type == blob_type::function) {
            set_property_functional(e.id, attrname, as<string_view>(std::get<0>(tpl)), std::get<1>(tpl));
        } else {
            set_property(e.id, attrname, std::get<0>(tpl));
        }
        */
    }

    // is attribute tag?
    if (!e.text.empty()) {
        set_text(e, e.text);
    }
}

}
