//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "variables.hpp"

#include "sonia/exceptions.hpp"

namespace sonia { namespace parameters {

variable variables::operator[](string_view varname) const {
    auto it = map_.find(varname, string_hasher(), string_equal_to());
    if (it == map_.cend()) {
        throw internal_error(fmt("parameter variable '%1%' doesn't exist") % varname);
    }
    return variable(it->second);
}

bool variables::is_available(string_view valname) const {
   return map_.cend() != map_.find(valname, string_hasher(), string_equal_to());
}

void variables::set_variable(string_view varname, boost::any val) {
    auto it = map_.find(varname, string_hasher(), string_equal_to());
    if (it == map_.cend()) {
        map_.insert(it, std::make_pair(to_string(varname), std::move(val)));
    } else {
        throw internal_error(fmt("found duplicated assignment for '%1%' variable") % varname);
    }
}

}}
