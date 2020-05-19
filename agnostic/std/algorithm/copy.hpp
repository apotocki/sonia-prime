// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALGORITHM_COPY_HPP
#define AGNOSTIC_STD_ALGORITHM_COPY_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#   include "../iterator/iterator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ASSIGNABLE
#   include "../type_traits/is_assignable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIAL
#   include "../type_traits/is_trivial.hpp"
#endif

namespace std {

template <class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
    using v0_t = iterator_traits<InputIt>::value_type;
    using r0_t = iterator_traits<InputIt>::reference;
    using v1_t = iterator_traits<OutputIt>::value_type;
    using r1_t = iterator_traits<OutputIt>::reference;

    if constexpr (is_trivial_v<v0_t> && is_trivial_v<v1_t> && is_assignable_v<r1_t, r0_t> 
        && is_pointer_v<InputIt> && is_pointer_v<OutputIt>)
    {
        return static_cast<OutputIt>(memcpy(d_first, first, last - first));
    } else {
        for (; first != last; ++d_first, ++first) {
            *d_first = *first;
        }
        return d_first;
    }
}

}

#endif // AGNOSTIC_STD_ALGORITHM_COPY_HPP
