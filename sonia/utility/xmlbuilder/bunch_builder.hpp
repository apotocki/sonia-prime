//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "xmlbuilder.hpp"
#include "advxmlbuilder.hpp"
#include "sonia/utility/invokation/invokable.hpp"

//#define USE_BASIC_BUILDER

namespace sonia::xmlbuilder {

class bunch_builder 
#ifdef USE_BASIC_BUILDER
    : public basic_external_builder
#else
    : public advanced_external_builder
#endif
{
public:
    class factory
    {
    public:
        virtual ~factory() = default;
        virtual shared_ptr<invokation::invokable> create(string_view type, string_view id) = 0;
        virtual attribute_resolver& get_attribute_resolver() = 0;
    };

    explicit bunch_builder(factory& f) : factory_{f} {}

    void build(string_view xml);

    // external_builder api
    void create(string_view type, string_view id) override;
    void set_text(string_view id, string_view text) override;
    void set_property(string_view id, string_view propname, blob_result const& value) override;
    void set_property_functional(string_view id, string_view propname, string_view code, func_type) override;
    void append(string_view parentid, string_view childid) override;

    shared_ptr<invokation::invokable> get_element_by(string_view id);
    shared_ptr<invokation::invokable> try_get_element_by(string_view id) noexcept;

#ifdef USE_BASIC_BUILDER
    attribute_resolver const& ar() const override final { return factory_.get_attribute_resolver(); };
#endif

protected:
    virtual void do_set_text(invokation::invokable&, string_view text);
    virtual void do_set_property_functional(invokation::invokable&, string_view propname, string_view code, func_type) = 0;
    virtual void do_append(invokation::invokable& parent, shared_ptr<invokation::invokable> child);

    virtual shared_ptr<invokation::invokable> root_element() = 0;

protected:
    boost::unordered_map<std::string, shared_ptr<invokation::invokable>, hasher> elements_;
    factory & factory_;
};

}
