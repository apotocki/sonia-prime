//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_ALGORITHM_COPY_HPP
#define SONIA_ALGORITHM_COPY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

namespace sonia {

template <typename ForwardInputIteratorT, typename ForwardOutputIteratorT>
ForwardOutputIteratorT copy_not_more(
    ForwardInputIteratorT b, ForwardInputIteratorT e,
    ForwardOutputIteratorT oit,
    size_t n)
{
    for (; b != e && n; --n) {
        *oit = *b;
        ++oit; ++b;
    }
    return std::move(oit);
}

}

#endif // SONIA_ALGORITHM_COPY_HPP
