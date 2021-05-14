//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <cstdio>

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/karma_uint.hpp>
#include <boost/spirit/include/karma_generate.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "serialization.hpp"

namespace sonia::serialization {

struct json_t {};

template <typename T>
class coder<json_t, T, enable_if_t<
    is_template_instance_v<basic_string_view, T> ||
    is_template_instance_v<basic_cstring_view, T>>>
{
    using type = T;
    using elem_t = typename T::value_type;

public:
    template <typename OutputIteratorT>
    static void encode_elem(elem_t c, OutputIteratorT& oi)
    {
        if constexpr (is_trivial_v<elem_t> && sizeof(elem_t) == 1) {
            switch (c) {
            case 0: break;
            case '"':
                *oi = '\\'; ++oi;
                *oi = '"'; ++oi;
                break;
            case '\\':
                *oi = '\\'; ++oi;
                *oi = '\\'; ++oi;
                break;

            default:
                if (c < 0x1f) {
                    *oi = '\\'; ++oi;
                    *oi = 'u'; ++oi;
                    *oi = '0'; ++oi;
                    *oi = '0'; ++oi;
                    if (c < 16) {
                        *oi = '0'; ++oi;
                    }
                    
                    boost::spirit::karma::generate(oi, boost::spirit::hex, static_cast<unsigned int>(c));
                }
                else {
                    *oi = c; ++oi;
                }
            }
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    }

    template <typename OutputIteratorT>
    OutputIteratorT encode(T const& value, OutputIteratorT oi) const
    {
        for (auto c : value) {
            encode_elem(c, oi);
        }
        return std::move(oi);
    }
};

template <typename TargetT>
class coder<json_t, TargetT, typename enable_if_t<is_integral_v<TargetT>>>
{
public:
    template <typename ArgT, typename OutputIteratorT>
    OutputIteratorT encode(ArgT val, OutputIteratorT oi) const
    {
        char buff[64];
        int res = snprintf(buff, sizeof(buff), get_format<ArgT>(), val);
        if (res > 0) {
            return std::copy(buff, buff + res, std::move(oi));
        }
        THROW_INTERNAL_ERROR("can't print integer");
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT * val) const
    {
        auto it = reinterpret_cast<char*>(val), eit = it + sizeof(ArgT);
        return pull(std::move(ii), it, eit);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT & val) const
    {
        return decode(std::move(ii), &val);
    }

private:
    template <typename T>
    static const char * get_format()
    {
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

    template <typename T> void error()
    {
        // unexpected integer type
        static_assert(is_integral_v<T>);
    }

    static const char * get_format(identity<int>) { return "%d"; }
    static const char * get_format(identity<unsigned int>) { return "%u"; }
    static const char * get_format(identity<int64_t>) { return "%lld" /*PRId64*/; }
};

}
