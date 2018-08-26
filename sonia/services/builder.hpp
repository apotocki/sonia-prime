//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_BUILDER_HPP
#define SONIA_SERVICE_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/parameters/values.hpp"
#include "service.hpp"

namespace sonia { namespace services {

class builder {
public:
    virtual ~builder() {}

    virtual shared_ptr<service> build(sonia::parameters::values const&) const = 0;
};

}}

#endif // SONIA_SERVICE_BUILDER_HPP
