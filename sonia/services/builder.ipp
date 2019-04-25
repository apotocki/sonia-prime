//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_BUILDER_IPP
#define SONIA_SERVICE_BUILDER_IPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "builder.hpp"

namespace sonia::services {

template <class ServT, class ConfT>
shared_ptr<service> basic_builder<ServT, ConfT>::build(service_configuration const& scfg)
{
    ConfT cfg;
    parameters_.apply(scfg.parameters, &cfg);
    auto r = sonia::make_shared<ServT>(cfg);
    service_access::set_layer(*r, scfg.layer);
    return r;
}

}

#endif // SONIA_SERVICE_BUILDER_IPP
