//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "parameters.hpp"

#include <algorithm>

namespace sonia::parameters {

void value_descriptor::set_default_json_string(std::string const& jstr)
{
    THROW_NOT_IMPLEMENTED_ERROR("value_descriptor::set_default_json_string");
}

}
