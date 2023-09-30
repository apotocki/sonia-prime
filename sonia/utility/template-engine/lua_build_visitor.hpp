//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_set.hpp>
#include "sonia/filesystem.hpp"
#include "sonia/utility/lua/lua.hpp"
#include "compound.hpp"

namespace sonia::templates {

class lua_build_visitor 
    : public build_visitor
    , lua::language
{
public:
    
    void operator()(document_element&) override;
    void operator()(foreach_element&) override;
    void operator()(file_template_element&) override;
    void operator()(code_element&) override;

    fs::path template_path;

protected:
    void visit_children(compound&);

    boost::unordered_set<std::string, hasher> installed_ids_;

    
};

}
