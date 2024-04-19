//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "builder_view_model.hpp"
#include "sonia/utility/invocation/invocable.hpp"

namespace sonia {

class bunch_builder_view_model 
    : public builder_view_model
    , public lang::bang::external_environment
{
public:
    class factory
    {
    public:
        virtual ~factory() = default;
        virtual shared_ptr<invocation::invocable> create(string_view type, string_view id) = 0;
    };

    explicit bunch_builder_view_model(factory& f) : factory_{ f } {}

    shared_ptr<invocation::invocable> create(string_view type, string_view id) override;
    void set_property(string_view propname, blob_result const& value) override;
    
    virtual shared_ptr<invocation::invocable> root_element() const = 0;

    shared_ptr<invocation::invocable> get_element_by(string_view id) const;
    shared_ptr<invocation::invocable> try_get_element_by(string_view id) const noexcept;

protected:
    boost::unordered_map<std::string, shared_ptr<invocation::invocable>, hasher> elements_;
    factory& factory_;
};

}
