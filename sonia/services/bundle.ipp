//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_BUNDLE_IPP
#define SONIA_SERVICES_BUNDLE_IPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "bundle.hpp"

namespace sonia { namespace services {

template <typename ServiceT>
void bundle::install(string_view nm) {
    auto it = builders_.find(nm, hasher(), string_equal_to());
    if (it == builders_.end()) {
        builders_.insert(it, std::make_pair(to_string(nm), [](json_object const&) {
            return make_shared<ServiceT>();
        }));
    } else {
        throw internal_error("builder for the service '%1%' is already installed"_fmt % nm);
    }
}

}}

#endif // SONIA_SERVICES_BUNDLE_IPP
