//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

#include "sonia/string.hpp"

#include "sonia/utility/invokation/invokation.hpp"

#include "functor_type.hpp"

namespace sonia::xmlbuilder {

class element
{
public:
    small_string id;
    small_string name;
    std::string text;
    std::vector<std::pair<small_string, smart_blob>> attrs;
    std::vector<std::tuple<small_string, small_string, func_type>> functionals; // name, code, functor_type (if code is defined)
};

class attribute_resolver
{
public:
    virtual ~attribute_resolver() = default;

    virtual std::tuple<blob_result, std::string, func_type> operator()(string_view element, string_view attr_name, string_view attr_value) = 0;
};

class external_builder
{
public:
    virtual ~external_builder() = default;
    virtual void append_element(span<element> parents, element&) = 0;
    virtual void close_element(span<element> parents, element&) = 0;
};

class basic_external_builder : public external_builder
{
public:
    virtual void create(string_view type, string_view id) = 0;
    virtual void set_text(string_view id, string_view text) = 0;
    virtual void set_property(string_view id, string_view propname, blob_result const& value) = 0;
    virtual void set_property_functional(string_view id, string_view propname, string_view code, func_type ft) = 0;
    virtual void append(string_view parentid, string_view childid) = 0;
    virtual void append_to_document(string_view childid) {}

    virtual std::string generate_id() const;
    void append_element(span<element> parents, element &) override;
    void close_element(span<element> parents, element &) override;

protected:
    mutable int id_counter_{ 0 };
};


void parse(string_view code, external_builder & eb, attribute_resolver& ar);

}
