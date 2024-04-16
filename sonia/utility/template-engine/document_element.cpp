//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "document_element.hpp"

namespace sonia::templates {

void document_element::do_registration(registrar_type& mr)
{
    mr.register_property<&document_element::code>("code");

    //mr.register_method<&foreach_element::append>("append");
}

//void document_element::run(sonia::lua::language & lang)
//{
//    run_elements(lang);
//}

}
