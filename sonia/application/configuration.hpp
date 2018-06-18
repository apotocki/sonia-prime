//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_APPLICATION_CONFIGURATION_HPP
#define SONIA_APPLICATION_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/any.hpp>

#include "sonia/string.hpp"

namespace sonia {

class config_node {
public:
    virtual ~config_node() {}

    virtual boost::any operator[](string_view) const = 0;
};

}

#endif // SONIA_APPLICATION_CONFIGURATION_HPP
