//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bunch_builder.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::xmlbuilder {

void bunch_builder::build(string_view xml)
{
    parse(xml, *this, factory_.get_attribute_resolver());
}

shared_ptr<invokation::invokable> bunch_builder::get_element_by(string_view id)
{
    if (id.empty()) return root_element();
    auto it = elements_.find(id, hasher{}, string_equal_to{});
    if (it == elements_.end()) throw exception("An element with identifier '%1%' was not found."_fmt % id);
    return it->second;
}

shared_ptr<invokation::invokable> bunch_builder::try_get_element_by(string_view id) noexcept
{
    if (id.empty()) return root_element();
    auto it = elements_.find(id, hasher{}, string_equal_to{});
    if (it == elements_.end()) return {};
    return it->second;
}

void bunch_builder::create(string_view type, string_view id)
{
    auto it = elements_.find(id, hasher{}, string_equal_to{});
    if (it != elements_.end()) throw exception("A duplicate element identifier '%1%' was found."_fmt % id);
    auto v = factory_.create(type, id);

    elements_.insert(it, { std::string{id}, std::move(v) });
}

void bunch_builder::set_text(string_view id, string_view text)
{
    do_set_text(*get_element_by(id), text);
}

void bunch_builder::do_set_text(invokation::invokable& obj, string_view text)
{
    obj.set_property("$", string_blob_result(text));
}

void bunch_builder::set_property(string_view id, string_view propname, blob_result const& value)
{
    if (value.type == blob_type::string) {
        string_view vstr = as<string_view>(value);
        std::vector<blob_result> objects;
        SCOPE_EXIT([&objects] {
            for (auto & obj : objects) { blob_result_unpin(&obj); }
        });
        while (!vstr.empty()) {
            if (vstr.size() < 2 || vstr.front() != '$' || vstr[1] == '$') {
                for (auto& obj : objects) { blob_result_unpin(&obj); }
                objects.clear();
                break;
            }
            size_t pos = vstr.find(',');
            if (pos == vstr.npos) pos = vstr.size();
            string_view eid = vstr.substr(1, pos - 1);
            objects.push_back(object_blob_result(get_element_by(eid)));
            //objects.push_back(object_blob_result<invokation::wrapper_object<shared_ptr<invokation::invokable>>>(get_element_by(eid)));
            vstr = vstr.substr(pos);
            if (!vstr.empty()) vstr = vstr.substr(1); // skip comma
        }
        if (!objects.empty()) {
            if (objects.size() == 1) {
                get_element_by(id)->set_property(propname, objects.front());
            } else {
                get_element_by(id)->set_property(propname, array_blob_result(span{objects}));
            }
            return;
        }
    }
    get_element_by(id)->set_property(propname, value);
}

void bunch_builder::set_property_functional(string_view id, string_view propname, string_view code, func_type ft)
{
    do_set_property_functional(*get_element_by(id), propname, code, ft);
}

void bunch_builder::append(string_view parentid, string_view childid)
{
    do_append(*get_element_by(parentid), get_element_by(childid));
    //if (view_ptr pview = dynamic_pointer_cast<view>(it->second), cview = dynamic_pointer_cast<view>(it2->second); cview && pview) {
    //    pview->add_subview(cview);
    //}
    //view_ptr cview = it->second;
    //pview->add_subview(cview);
}

void bunch_builder::do_append(invokation::invokable& parent, shared_ptr<invokation::invokable> child)
{
    smart_blob sb = object_blob_result(std::move(child));
    parent.invoke("append", { sb.get() });
}

void bunch_builder::append_to_document(string_view childid)
{

}

}
