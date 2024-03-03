//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "code_element.hpp"

namespace sonia::templates {

void code_element::do_registration(registrar_type& mr)
{
    mr.register_property<&code_element::code>("$");
    mr.register_property<&code_element::input>("input");
}

}
