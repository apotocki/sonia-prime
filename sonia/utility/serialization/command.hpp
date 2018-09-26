//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_COMMAND_HPP
#define SONIA_SERIALIZATION_COMMAND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"

#include "serialization.hpp"
#include "type_durable_id.hpp"

#include "sonia/utility//type_durable_id.hpp"
#include "sonia/utility/command.hpp"

namespace sonia { namespace serialization {

template <typename SerializationTagT = compressed_t>
class command_proxy_coder {
public:
    template <class CommandT, typename OutputIteratorT>
    enable_if_t<is_base_of_v<abstract_command_base, CommandT>, OutputIteratorT> encode(CommandT const& cmd, OutputIteratorT oi) const {
        //return encode<SerializationTagT>(cmd, encode<SerializationTagT>(sonia::type::durable_id::get<CommandT>(), std::move(oi)));
        return make_encoder<SerializationTagT>(std::move(oi)) & cmd & sonia::type::durable_id::get<CommandT>();
    }

    template <typename RT, typename ... ArgsT, typename OutputIteratorT>
    OutputIteratorT encode(command<RT, ArgsT ...> const& cmd, OutputIteratorT oi) const {
        return std::move(oi);
        //sonia::type::durable_id cmdid{cmd.get_type_info()};
        //encode(cmdid, std::move(oi))
    }
};

}}

#endif // SONIA_SERIALIZATION_COMMAND_HPP
