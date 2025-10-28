//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

#include "sonia/string.hpp"

namespace sonia::xmlbuilder {

class element
{
public:
    small_string id;
    small_string name;
    std::string text;
    std::string ns;
    std::vector<std::pair<small_string, small_string>> attrs;
    std::vector<std::tuple<small_string, std::string>> prefixes; // [prefix, namespace]
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
    virtual void set_text(element&, string_view text) = 0;
    virtual void set_property(element&, string_view propname, string_view propvalue) = 0;

    std::string generate_id() const;
    void append_element(span<element> parents, element &) override;
    void close_element(span<element> parents, element &) override;

protected:
    mutable int id_counter_{ 0 };
};


void parse(string_view code, external_builder & eb);

}
