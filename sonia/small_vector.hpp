//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/span.hpp"
#include <boost/container/small_vector.hpp>

namespace sonia {

using boost::container::small_vector;

}

namespace boost::container {

template <typename T, size_t SzV, typename OptionsT>
inline sonia::span<T> to_span(boost::container::small_vector<T, SzV, OptionsT> & vec)
{
    return { vec.data(), vec.size() };
}

template <typename T, size_t SzV, typename OptionsT>
inline sonia::span<const T> to_span(boost::container::small_vector<T, SzV, OptionsT> const& vec)
{
    return { vec.data(), vec.size() };
}

}
