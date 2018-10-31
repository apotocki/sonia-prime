//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_SERIALIZABLE_HPP
#define SONIA_SERIALIZATION_SERIALIZABLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/array_view.hpp"
#include "sonia/reference_wrapper.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"

namespace sonia {

class serializable {
public:
    typedef iterator_polymorphic_impl<array_view<char>> output_impl_type;
    typedef iterator_polymorphic_impl<array_view<const char>> input_impl_type;

    // note: range_write_iterator and range_read_iterator are reference iterators

    typedef wrapper_output_iterator<
        iterator_ptr_wrappee_adapter<output_impl_type, std::input_iterator_tag>,
        array_view<char>,
        std::output_iterator_tag,
        array_view<char>
    > range_write_iterator;

    typedef wrapper_output_iterator<
        iterator_ptr_wrappee_adapter<input_impl_type, std::input_iterator_tag>,
        array_view<const char>,
        std::output_iterator_tag,
        array_view<const char>
    > range_read_iterator;

    virtual ~serializable() {}

    virtual void serialize(range_write_iterator) const = 0;
    virtual void deserialize(range_read_iterator) const = 0;
};

}

#endif // SONIA_SERIALIZATION_SERIALIZABLE_HPP
