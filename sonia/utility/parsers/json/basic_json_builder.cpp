//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_builder.hpp"

#include "sonia/exceptions.hpp"

namespace sonia { namespace parsers { namespace json {

typedef basic_builder::descriptor_type descriptor_type; 

/*
basic_value::basic_value(descriptor_type dt, bool is_owner)
    : value_(dt), is_owner_(is_owner)
{}

basic_value::~basic_value() {
    if (is_owner_) {
        basic_builder::do_free(value_);
    }
}

bool basic_value::is_null() const {
    return value_.first == (int)basic_builder::object_type::null;
}

bool basic_value::is_boolean() const {
    return value_.first == (int)basic_builder::object_type::boolean;
}

bool basic_value::is_integral() const {
    return value_.first == (int)basic_builder::object_type::integer;
}

bool basic_value::is_floating() const {
    return value_.first == (int)basic_builder::object_type::floating;
}

bool basic_value::is_string() const {
    return value_.first == (int)basic_builder::object_type::string;
}

bool basic_value::is_object() const {
    return value_.first == (int)basic_builder::object_type::object;
}

bool basic_value::is_array() const {
    return value_.first == (int)basic_builder::object_type::array;
}

optional<bool> basic_value::get_boolean() const {
    return is_boolean() ? optional<bool>(!!value_.second) : optional<bool>();
}

intptr_t const* basic_value::get_integral() const {
    return is_integral() ? (intptr_t const*)value_.second : (intptr_t const*)nullptr;
}

float const* basic_value::get_floating() const {
    return is_floating() ? (float const*)value_.second : (float const*)nullptr;
}

std::string const* basic_value::get_string() const {
    return is_string() ? (std::string const*)value_.second : (std::string const*)nullptr;
}

size_t basic_value::size() const {
    if (is_array()) {
        return ((array_t const*)value_.second)->size();
    } else if (is_object()) {
        return ((map_t const*)value_.second)->size();
    } else {
        throw exception("json value is not an array or an object");
    }
}

shared_ptr<json_value> basic_value::operator[](size_t idx) const {
    if (!is_array()) throw exception("json value is not an array");
    array_t const& arr = *(array_t const*)value_.second;
    if (idx >= arr.size()) {
        throw exception("json array : index is out of bound");
    }
    return make_shared<basic_value>(arr[idx], false);
}

shared_ptr<json_value> basic_value::operator[](string_view key) const {
    if (!is_object()) throw exception("json value is not an object");
    map_t const& m = *(map_t const*)value_.second;
    auto it = m.find(key, string_hasher(), string_equal_to());
    if (it != m.end()) {
        return make_shared<basic_value>(it->second, false);
    }

    return shared_ptr<json_value>();
}
*/

descriptor_type basic_builder::build_array(array_view<descriptor_type> av) {
    return make_descriptor(object_type::array, (intptr_t)new array_t(av.begin(), av.end()));
}

descriptor_type basic_builder::build_object(array_view<std::string> names, array_view<descriptor_type> values) {
    BOOST_ASSERT(names.size() == values.size());
    map_t m;
    auto name_it = names.begin();
    auto value_it = values.begin();
    for (; name_it != names.end() && value_it != values.end(); ++name_it, ++value_it) {
        if (!m.insert(map_t::value_type(std::move(*name_it), *value_it)).second) {
            free(*value_it);
        }
    }
    return make_descriptor(object_type::object, (intptr_t)new map_t(std::move(m)));
}

void basic_builder::free(descriptor_type d) {
    do_free(d);
}

void basic_builder::do_free(descriptor_type d) {
    switch((object_type)d.first) {
    case object_type::null:
    case object_type::boolean:
    case object_type::integer:
    case object_type::floating:
        return;
    case object_type::string:
        delete (string_t*)d.second;
        return;
    case object_type::array:
    {
        array_t * arr = (array_t*)d.second;
        for (descriptor_type e : *arr) {
            do_free(e);
        }
        delete arr;
        return;
    }
    case object_type::object:
    {
        map_t * m = (map_t*)d.second;
        for (auto & e : *m) {
            do_free(e.second);
        }
        delete m;
        return;
    }
    default:
        throw exception("unknow descriptor to delete");
    }
}

json_value basic_builder::build_json_value(descriptor_type val) {
    switch (val.first)
    {
    default:
        break;
    }
    //return make_shared<basic_value>(val, true);
}

}}}
