//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "file_template_element.hpp"

namespace sonia::templates {

void file_template_element::do_registration(registrar_type& mr)
{
    register_property<&file_template_element::src>("src");
    register_property<&file_template_element::input>("input");
    register_property<&file_template_element::result>("result");
    register_property<&file_template_element::code>("$");
}

}
