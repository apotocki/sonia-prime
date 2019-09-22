//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_BIND_COMMAND_HPP
#define SONIA_SERIALIZATION_BIND_COMMAND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "serialization.hpp"
#include "sonia/utility/bind_command.hpp"

namespace sonia::serialization {

template <typename TagT, class BindTupleT, typename R>
class coder<TagT, bind_command_proxy<BindTupleT, R>>
{
    using cmd_type = bind_command_proxy<BindTupleT, R>>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(cmd_type const& val, OutputIteratorT oi) const
    {
        return encode<TagT>(val.btpl_, std::move(oi));
    }
};

}

#endif // SONIA_SERIALIZATION_BIND_COMMAND_HPP
