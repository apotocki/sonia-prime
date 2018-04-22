//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_APPLICATION_APPLICATION_HPP
#define SONIA_APPLICATION_APPLICATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>

namespace sonia {

class application {
public:
    application();
    int open(int argc, char const* argv[], std::istream * cfgstream = nullptr);
};

}

#endif // APPLICATION_
