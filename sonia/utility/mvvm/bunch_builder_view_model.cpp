//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bunch_builder_view_model.hpp"

namespace sonia {

shared_ptr<invokation::invokable> bunch_builder_view_model::create(string_view type, string_view id)
{
    auto it = elements_.find(id, hasher{}, string_equal_to{});
    if (it != elements_.end())
        throw exception("A duplicate element identifier '%1%' was found."_fmt % id);
    auto v = factory_.create(type, id);

    elements_.insert(it, { std::string{id}, v });
    return v;
}

shared_ptr<invokation::invokable> bunch_builder_view_model::get_element_by(string_view id) const
{
    if (id.empty()) return root_element();
    auto it = elements_.find(id, hasher{}, string_equal_to{});
    if (it == elements_.end()) throw exception("An element with identifier '%1%' was not found."_fmt % id);
    return it->second;
}

shared_ptr<invokation::invokable> bunch_builder_view_model::try_get_element_by(string_view id) const noexcept
{
    if (id.empty()) return root_element();
    auto it = elements_.find(id, hasher{}, string_equal_to{});
    if (it == elements_.end()) return {};
    return it->second;
}

void bunch_builder_view_model::set_property(string_view id, string_view propname, blob_result const& value)
{
    get_element_by(id)->set_property(propname, value);
}

}
