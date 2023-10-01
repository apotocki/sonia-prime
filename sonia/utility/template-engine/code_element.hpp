//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/invokation/invokable.hpp"
#include "compound.hpp"

namespace sonia::templates {

class code_element 
    : public compound
    , public invokation::registrar<code_element>
{
    friend class invokation::registrar<code_element>;

public:
    using compound::compound;

    small_string input;
    std::string code;

    void visit(build_visitor& bv) override
    {
        bv(*this);
    }

protected:
    // invokation routine
    void do_registration(registrar_type&);
};

}
