//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include "sonia/utility/invocation/invocable.hpp"
#include "compound.hpp"

namespace sonia::templates {

class document_element 
    : public compound
    , public invocation::registrar<document_element>
{
    friend class invocation::registrar<document_element>;

    friend class foreach_element_lua_resolver;

public:
    using compound::compound;

    std::string code;
    
    void visit(build_visitor& bv) override
    {
        bv(*this);
    }

protected:
    // invocation routine
    static void do_registration(registrar_type&);
};

}
