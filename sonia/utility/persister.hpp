//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_PERSISTER_HPP
#define SONIA_UTILITY_PERSISTER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/array_view.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"

namespace sonia {

class persister {
public:
    virtual ~persister() {}

    typedef polymorphic_input_iterator<32, array_view<const uint8_t>, forward_traversal_tag, array_view<const uint8_t>> input_iterator;
    typedef polymorphic_output_iterator<32, array_view<uint8_t>, forward_traversal_tag, array_view<uint8_t>> output_iterator;

    virtual input_iterator reader() const = 0;
    virtual output_iterator writer() = 0;
};

}

#endif // SONIA_UTILITY_PERSISTER_HPP
