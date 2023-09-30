//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/xmlbuilder/xmlbuilder.hpp"
#include "sonia/utility/xmlbuilder/bunch_builder.hpp"
#include "sonia/utility/lua/lua.hpp"

#include "compound.hpp"

namespace sonia {

class templated_bunch 
    : public xmlbuilder::external_builder
{
public:
    templated_bunch();

    void build(string_view xml);


    // external_builder api
    void append_element(span<const xmlbuilder::element> parents, xmlbuilder::element&) override;
    void close_element(span<const xmlbuilder::element> parents, xmlbuilder::element&) override;

    templates::compound& root() { return *root_; }

protected:
    std::string generate_id() const;
    shared_ptr<templates::compound> get_element_by(string_view id) const;
    void set_text(invokation::invokable&, string_view text);

    boost::unordered_map<std::string, shared_ptr<templates::compound>, hasher> elements_;
    shared_ptr<templates::compound> root_;
    
    mutable int id_counter_{ 0 };
};

}
