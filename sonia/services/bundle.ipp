//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_BUNDLE_IPP
#define SONIA_SERVICES_BUNDLE_IPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "bundle.hpp"
#include "sonia/exceptions.hpp"

namespace sonia::services {

template <typename ServiceT>
void bundle::install(string_view nm)
{
    auto it = builders_.find(nm, hasher(), string_equal_to());
    if (it == builders_.end()) {
        builders_.insert(it, std::pair(to_string(nm), [](json_object const& props) {
            return make_shared<ServiceT>(props);
        }));
    } else {
        THROW_INTERNAL_ERROR("builder for the service '%1%' is already installed"_fmt % nm);
    }
}

}

#endif // SONIA_SERVICES_BUNDLE_IPP
