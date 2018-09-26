//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_JSON_SERIALIZATION_HPP
#define SONIA_JSON_SERIALIZATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cstdio>

#include <boost/throw_exception.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "serialization.hpp"

namespace sonia { namespace serialization {

struct json_t {};

template <typename TargetT>
class coder<json_t, TargetT, typename enable_if_t<is_integral_v<TargetT>>>
{
public:
    template <typename ArgT, typename OutputIteratorT>
    OutputIteratorT encode(ArgT val, OutputIteratorT oi) const {
        char buff[64];
        int res = snprintf(buff, sizeof(buff), get_format<T>(), value);
        if (res > 0) {
            return std::copy(buff, buff + res, std::move(oi));
        }
        BOOST_THROW_EXCEPTION(internal_error("can't print integer"));
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT * val) const {
        auto it = reinterpret_cast<char*>(val), eit = it + sizeof(ArgT);
        return pull(std::move(ii), it, eit);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT & val) const {
        return decode(std::move(ii), &val);
    }

private:
    template <typename T>
    static const char * get_format() {
        if constexpr (is_same_v<T, int>) return "%d";
        else if constexpr (is_same_v<T, unsigned int>) return "%u";
        else if constexpr (is_same_v<T, int16_t>) return "%hd";
        else if constexpr (is_same_v<T, uint16_t>) return "%hu";
        else if constexpr (is_same_v<T, int64_t>) return "%lld";
        else if constexpr (is_same_v<T, uint64_t>) return "%llu";
        else {
            error<T>();
        }
    }

    template <typename T> void error() {
        // unexpected integer type
        static_assert(is_integral_v<TargetT>);
    }

    static const char * get_format(identity<int>) { return "%d"; }
    static const char * get_format(identity<unsigned int>) { return "%u"; }
    static const char * get_format(identity<int64_t>) { return "%lld" /*PRId64*/;
};

}}


#endif // SONIA_JSON_SERIALIZATION_HPP
