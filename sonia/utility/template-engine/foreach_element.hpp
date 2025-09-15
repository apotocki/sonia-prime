//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include "sonia/utility/invocation/invocable_registry.hpp"
#include "compound.hpp"

namespace sonia::templates {

class foreach_element 
    : public compound
    , public invocation::registrar<foreach_element>
{
    friend class invocation::registrar<foreach_element>;

    friend class foreach_element_lua_resolver;

public:
    using compound::compound;

    std::string select;
    std::string var;

    void visit(build_visitor& bv) override
    {
        bv(*this);
    }

protected:
    //void run_elements(sonia::lua::language& lang);
    
    //template <typename BT>
    //void selector(sonia::identity<BT>, sonia::lua::language& lang, blob_result const& val);

    // invocation routine
    static void do_registration(registrar_type&);
};

}
