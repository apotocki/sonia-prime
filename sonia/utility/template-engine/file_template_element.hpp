//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/invokation/invokable.hpp"
#include "compound.hpp"

namespace sonia::templates {

class file_template_element 
    : public compound
    , public invokation::registrar<file_template_element>
{
    friend class invokation::registrar<file_template_element>;

public:
    using compound::compound;

    small_string src;
    small_string input;
    small_string result;
    std::string code;

    void visit(build_visitor& bv) override
    {
        bv(*this);
    }

protected:
    // invokation routine
    static void do_registration(registrar_type&);
};

}
