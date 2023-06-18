//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/filesystem.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/optional.hpp"
#include "value.hpp"

namespace sonia {

template <typename T>
struct json_cast_impl
{
    T operator()(json_value const& jv) const
    {
        throw exception("Can't convert from json %1% to %2%"_fmt % to_string(jv) % typeid(T).name());
    }
};

template <>
struct json_cast_impl<bool>
{
    bool operator()(json_value const& jv) const
    {
        return jv.get_bool();
    }
};

template <std::integral T>
struct json_cast_impl<T>
{
    T operator()(json_value const& jv) const
    {
        return (T)jv.get_number();
    }
};

template <std::floating_point T>
struct json_cast_impl<T>
{
    T operator()(json_value const& jv) const
    {
        return (T)jv.get_number();
    }
};

template <typename T>
struct json_cast_impl<optional<T>>
{
    optional<T> operator()(json_value const& jv) const
    {
        return json_cast_impl<T>()(jv);
    }
};

template <typename CharT>
requires (sizeof(CharT) == 1)
struct json_cast_impl<std::basic_string<CharT>>
{
    std::basic_string<CharT> operator()(json_value const& jv) const
    {
        auto sv = jv.get_string();
        return std::basic_string<CharT>(std::basic_string_view<CharT>(reinterpret_cast<const CharT*>(sv.data()), sv.size()));
    }
};

template <>
struct json_cast_impl<fs::path>
{
    fs::path operator()(json_value const& jv) const
    {
        auto sv = jv.get_string();
        return std::u8string_view{reinterpret_cast<char8_t const*>(sv.data()), sv.size()};
    }
};

template <>
struct json_cast_impl<json_value>
{
    json_value operator()(json_value const& jv) const
    {
        return jv;
    }
};

template <>
struct json_cast_impl<json_object>
{
    json_object operator()(json_value const& jv) const
    {
        return jv.get_object();
    }
};

template <typename T>
T json_cast(json_value const& val)
{
    return json_cast_impl<T>()(val);
}

}
