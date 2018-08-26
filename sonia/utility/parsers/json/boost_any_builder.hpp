//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_BOOS_ANY_BUILDER_HPP
#define SONIA_UTILITY_JSON_BOOS_ANY_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/any.hpp>

#include "sonia/string.hpp"
#include "sonia/utility/object_pool.hpp"

#include "builder.hpp"

namespace sonia { namespace parsers { namespace json {

/*
class boost_any_value : public json_value
{
public:
    explicit boost_any_value(boost::any val) : value_(std::move(val)) {}
    ~boost_any_value() override {}

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

    boost::any const& value() const { return value_; }

private:
    boost::any value_;
};
*/

class boost_any_builder : public parsers::json::builder
{
public:
    boost_any_builder();
    ~boost_any_builder() override;

    descriptor_type build_null() override final { return make_descriptor(); }
    descriptor_type build_boolean(bool val) override final { return make_descriptor(val); }
    descriptor_type build_integer(intptr_t val) override final { return make_descriptor(val); }
    descriptor_type build_double(float val) override final { return make_descriptor(val); }
    descriptor_type build_string(string_view str) override final { return make_descriptor(to_string(str)); }
    descriptor_type build_array(array_view<descriptor_type> av) override final;
    descriptor_type build_object(array_view<std::string> names, array_view<descriptor_type> values) override final;

    void free(descriptor_type d) override final;
    
    shared_ptr<json_value> build_json_value(descriptor_type) override;

private:
    template <typename ... ArgsT>
    descriptor_type make_descriptor(ArgsT&& ... args) {
        auto * pany = pool_.new_object(std::forward<ArgsT>(args) ...);
        return descriptor_type(0, reinterpret_cast<intptr_t>(pany));
    }

    object_pool<boost::any> pool_;
};

}}}

#endif // SONIA_UTILITY_JSON_BOOS_ANY_BUILDER_HPP
