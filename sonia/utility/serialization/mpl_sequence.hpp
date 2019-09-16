//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_MPL_SEQUENCE_HPP
#define SONIA_SERIALIZATION_MPL_SEQUENCE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/fusion/include/at_c.hpp>
#include "sonia/mpl/sequence.hpp"
#include "serialization.hpp"
#include "tuple.hpp"

namespace sonia::serialization {

template <typename TagT, typename SequenceT>
class coder<TagT, SequenceT, enable_if_t<is_mpl_sequence_v<SequenceT> && !is_template_instance_v<std::tuple, SequenceT>>> 
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(SequenceT const& val, OutputIteratorT oi) const
    {
        return do_encode(std::make_index_sequence<sonia::mpl::size_v<SequenceT>>(), val, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, SequenceT & val) const
    {
        return do_decode(std::make_index_sequence<sonia::mpl::size_v<SequenceT>>(), std::move(ii), val);
    }

private:
    template <size_t ... I, typename OutputIteratorT>
    static OutputIteratorT do_encode(std::index_sequence<I ...>, SequenceT const& val, OutputIteratorT oi)
    {
        return (encoder<TagT, OutputIteratorT>(std::move(oi)) & ... & at_c<I>(val));
    }

    template <size_t ... I, typename InputIteratorT>
    static InputIteratorT do_decode(std::index_sequence<I ...>, InputIteratorT ii, SequenceT & val)
    {
        return (decoder<TagT, InputIteratorT>(std::move(ii)) & ... & at_c<I>(val));
    }
};

}

#endif // SONIA_SERIALIZATION_MPL_SEQUENCE_HPP
