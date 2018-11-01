//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_ALGORITHM_PULL_HPP
#define SONIA_ALGORITHM_PULL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

namespace sonia {

template <typename InputIteratorT, typename OutputIteratorT>
InputIteratorT pull(InputIteratorT iit, OutputIteratorT ob, OutputIteratorT oe) {
    for (; ob != oe; ++iit, ++ob) {
        *ob = *iit;
    }
    return std::move(iit);
}

}

#endif // SONIA_ALGORITHM_PULL_HPP
