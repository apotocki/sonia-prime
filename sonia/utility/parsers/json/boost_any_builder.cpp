//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "boost_any_builder.hpp"

#include <vector>

#include <boost/unordered_map.hpp>

#include "sonia/exceptions.hpp"

namespace sonia { namespace parsers { namespace json {

typedef boost_any_builder::descriptor_type descriptor_type; 
typedef std::vector<boost::any> array_t;
typedef boost::unordered_map<std::string, boost::any> object_t;

boost_any_builder::boost_any_builder()
    : pool_(128)
{

}

boost_any_builder::~boost_any_builder()
{

}

descriptor_type boost_any_builder::build_array(array_view<descriptor_type> av) {
    array_t vec(av.size());
    auto av_it = av.begin(), av_eit = av.end();
    auto dest_it = vec.begin();
    for (; av_it != av_eit; ++av_it, ++dest_it) {
        *dest_it = std::move(*reinterpret_cast<boost::any*>(av_it->second));
        free(*av_it);
    }
    return make_descriptor(std::move(vec));
}

descriptor_type boost_any_builder::build_object(array_view<std::string> names, array_view<descriptor_type> values) {
    BOOST_ASSERT(names.size() == values.size());
    object_t m;
    auto name_it = names.begin();
    auto value_it = values.begin();
    for (; name_it != names.end() && value_it != values.end(); ++name_it, ++value_it) {
        m.insert(std::make_pair(std::move(*name_it), std::move(*reinterpret_cast<boost::any*>(value_it->second)))).second;
        free(*value_it);
    }
    return make_descriptor(std::move(m));
}

void boost_any_builder::free(descriptor_type d) {
    pool_.delete_object(reinterpret_cast<boost::any*>(d.second));
}

shared_ptr<json_value> boost_any_builder::build_json_value(descriptor_type val) {
    auto result = sonia::make_shared<boost_any_value>(std::move(*reinterpret_cast<boost::any*>(val.second)));
    free(val);
    return std::move(result);
}

bool boost_any_value::is_null() const {
    return value_.empty();
}

bool boost_any_value::is_boolean() const {
    return value_.type() == typeid(bool);
}

bool boost_any_value::is_integral() const {
    return value_.type() == typeid(intptr_t);
}

bool boost_any_value::is_floating() const {
    return value_.type() == typeid(float);
}

bool boost_any_value::is_string() const {
    return value_.type() == typeid(std::string);
}

bool boost_any_value::is_object() const {
    return value_.type() == typeid(boost::unordered_map<std::string, boost::any>);
}

bool boost_any_value::is_array() const {
    return value_.type() == typeid(std::vector<boost::any>);
}

optional<bool> boost_any_value::get_boolean() const {
    optional<bool> result;
    bool const* val = boost::any_cast<bool>(&value_);
    if (val) result = *val;
    return result;
}

intptr_t const * boost_any_value::get_integral() const {
    return boost::any_cast<intptr_t>(&value_);
}

float const * boost_any_value::get_floating() const {
    return boost::any_cast<float>(&value_);
}

std::string const* boost_any_value::get_string() const {
    return boost::any_cast<std::string>(&value_);
}

size_t boost_any_value::size() const {
    if (is_array()) {
        return boost::any_cast<array_t>(value_).size();
    } else if (is_object()) {
        return boost::any_cast<object_t>(value_).size();
    } else {
        throw exception("json value is not an array or an object");
    }
}

shared_ptr<json_value> boost_any_value::operator[](size_t idx) const {
    array_t const* arr = boost::any_cast<array_t>(&value_);
    if (!arr) throw exception("json value is not an array");
    if (idx >= arr->size()) {
        throw exception("json array : index is out of bound");
    }
    return sonia::make_shared<boost_any_value>(arr[idx]);
}

shared_ptr<json_value> boost_any_value::operator[](string_view key) const {
    object_t const* m = boost::any_cast<object_t>(&value_);
    if (!m) throw exception("json value is not an object");
    auto it = m->find(key, string_hasher(), string_equal_to());
    if (it != m->cend()) {
        return sonia::make_shared<boost_any_value>(it->second);
    }

    return shared_ptr<json_value>();
}

}}}
