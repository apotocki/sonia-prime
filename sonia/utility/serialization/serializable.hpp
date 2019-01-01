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

class serializable
{
public:
    using output_impl_type = proxying_iterator_polymorphic<array_view<char>>;
    using input_impl_type = iterator_polymorphic<array_view<const char>>;

    // note: range_write_iterator and range_read_iterator are reference iterators

    using range_write_iterator = wrapper_iterator<
        output_impl_type*,
        array_view<char>,
        std::output_iterator_tag,
        array_view<char>
    > ;

    using range_read_iterator = wrapper_iterator<
        input_impl_type*,
        const array_view<const char>,
        std::output_iterator_tag,
        const array_view<const char>
    >;

    virtual ~serializable() {}

    virtual void serialize(range_write_iterator) const = 0;
    virtual void deserialize(range_read_iterator) const = 0;
};

}

#endif // SONIA_SERIALIZATION_SERIALIZABLE_HPP
