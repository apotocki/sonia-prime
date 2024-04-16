//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/utility/mvvm/view_model.hpp"
#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/bang/bang.hpp"

namespace sonia {

class bang_view_model
    : public virtual view_model
    , public sonia::lang::bang::language
    , public invokation::registrar<bang_view_model, view_model>
{
    using registrar_type = invokation::registrar<bang_view_model, view_model>::registrar_type;
    friend class invokation::registrar<bang_view_model, view_model>;

public:
    using view_model::view_model;

    // scripting
    void load_file(string_view path);

protected:
    // methods routine
    static void do_registration(registrar_type& mr);
};

}
