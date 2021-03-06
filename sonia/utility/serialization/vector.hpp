//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_VECTOR_HPP
#define SONIA_SERIALIZATION_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>

#include "array.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename T, class AllocT>
class coder<TagT, std::vector<T, AllocT>>
    : public vector_coder<TagT, std::vector<T, AllocT>>
{};

}}

#endif // SONIA_SERIALIZATION_VECTOR_HPP
