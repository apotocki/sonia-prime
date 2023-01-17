//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <span>

#include "array.hpp"

namespace sonia::serialization {

template <typename TagT, typename T, size_t EV>
class coder<TagT, std::span<T, EV>>
    : public vector_coder<TagT, std::span<T, EV>>
{};

}
