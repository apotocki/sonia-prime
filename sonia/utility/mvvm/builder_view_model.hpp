//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

//#include "sonia/utility/invocation/invocation.hpp"

#include "bang_view_model.hpp"

namespace sonia {
 
class builder_view_model
    : public bang_view_model
    , public invocation::registrar<builder_view_model, bang_view_model>
{
    using registrar_base_t = invocation::registrar<builder_view_model, bang_view_model>;
    using registrar_type = registrar_base_t::registrar_type;
    friend class registrar_base_t;

public:
    class factory
    {
    public:
        virtual ~factory() = default;
        virtual shared_ptr<invocation::invocable> create(string_view type) = 0;
    };

    explicit builder_view_model(factory& f) : factory_{ f } {}

    shared_ptr<invocation::invocable> create(string_view type);

protected:
    // methods routine
    static void do_registration(registrar_type& mr);

    factory& factory_;
};

}
