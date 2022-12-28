//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/variant.hpp>

#include "serialization.hpp"

namespace sonia::serialization {

template <typename TagT, typename IteratorT>
struct variant_encoder_visitor : boost::static_visitor<IteratorT>
{
    explicit variant_encoder_visitor(IteratorT it) : it_{it} {}

    template <typename T>
    IteratorT operator()(T const& val) const
    {
        return sonia::encode<TagT>(val, std::move(it_));
    }
     
    IteratorT it_;
};

template <typename TagT, typename IteratorT>
struct variant_decoder_visitor : boost::static_visitor<IteratorT>
{
    explicit variant_decoder_visitor(IteratorT it) : it_{it} {}

    template <typename T>
    IteratorT operator()(T & val) const
    {
        return decode<TagT, T>(it_, val);
    }

    mutable IteratorT it_;
};

template <typename TagT, typename ... ArgsT>
class coder<TagT, boost::variant<ArgsT ...>>
{
    using value_t = boost::variant<ArgsT ...>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(value_t const& value, OutputIteratorT oi) const
    {
        return boost::apply_visitor(variant_encoder_visitor<TagT, OutputIteratorT>(std::move(oi)), value);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, value_t & value) const
    {
        return boost::apply_visitor(variant_decoder_visitor<TagT, InputIteratorT>(std::move(ii)), value);
    }
};

}
