//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_FORWARDER_HPP
#define SONIA_FUNCTIONAL_FORWARDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <functional>

namespace sonia {

struct forwarder {
    template <typename T>
    auto&& operator()(T&& val) const { return std::forward<T>(val); }
};

}

#endif // SONIA_FUNCTIONAL_FORWARDER_HPP
