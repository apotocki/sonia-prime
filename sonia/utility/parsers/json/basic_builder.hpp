//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_BASIC_BUILDER_HPP
#define SONIA_UTILITY_JSON_BASIC_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/unordered_map.hpp>

#include "sonia/string.hpp"
#include "builder.hpp"

namespace sonia { namespace parsers { namespace json {

class basic_value : public json_value
{
    typedef builder::descriptor_type descriptor_type;
    typedef std::vector<descriptor_type> array_t;
    typedef boost::unordered_map<std::string, descriptor_type> map_t;

public:
    basic_value(descriptor_type, bool is_owner);
    ~basic_value() override;

    bool is_null() const override final;
    bool is_boolean() const override final;
    bool is_integral() const override final;
    bool is_floating() const override final;
    bool is_string() const override final;
    bool is_object() const override final;
    bool is_array() const override final;

    optional<bool> get_boolean() const override final;
    intptr_t const * get_integral() const override final;
    float const * get_floating() const override final;
    std::string const* get_string() const override final;
    
    size_t size() const override final;
    shared_ptr<json_value> operator[](size_t) const override final;
    shared_ptr<json_value> operator[](string_view) const override final;

private:
    descriptor_type value_;
    bool is_owner_;
};

class basic_builder : public parsers::json::builder
{
    friend class basic_value;

    typedef std::string string_t;
    typedef std::vector<descriptor_type> array_t;
    typedef boost::unordered_map<std::string, descriptor_type> map_t;

    enum class object_type {
        null = 0,
        boolean,
        integer,
        floating,
        string,
        array,
        object
    };

public:
    ~basic_builder() override {}

    descriptor_type build_null() override { return make_descriptor(object_type::null, 0); }
    descriptor_type build_boolean(bool val) override { return make_descriptor(object_type::boolean, (intptr_t)(val ? 1 : 0)); }
    descriptor_type build_integer(intptr_t val) override { return make_descriptor(object_type::integer, val); }
    descriptor_type build_double(float val) override { return make_descriptor(object_type::floating, *reinterpret_cast<intptr_t*>(&val)); }
    descriptor_type build_string(string_view str) override { return make_descriptor(object_type::string, (intptr_t)new string_t(to_string(str))); }
    descriptor_type build_array(array_view<descriptor_type> av) override;
    descriptor_type build_object(array_view<std::string> names, array_view<descriptor_type> values) override;

    void free(descriptor_type d) override;
    
    shared_ptr<json_value> build_json_value(descriptor_type) override;

private:
    static void do_free(descriptor_type d);

    descriptor_type make_descriptor(object_type t, intptr_t ptr) {
        return descriptor_type((int)t, ptr);
    }
};

}}}

#endif // SONIA_UTILITY_JSON_BASIC_BUILDER_HPP
