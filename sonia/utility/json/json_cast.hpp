//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <filesystem>
#include "sonia/exceptions.hpp"
#include "sonia/optional.hpp"
#include "value.hpp"

namespace sonia {

template <typename T, typename Enable = void>
struct json_cast_impl {
    T operator()(json_value const& jv) const {
        throw exception("Can't convert from json %1% to %2%"_fmt % to_string(jv) % typeid(T).name());
    }
};

template <>
struct json_cast_impl<bool> {
    bool operator()(json_value const& jv) const {
        return jv.get_bool();
    }
};

template <typename T>
struct json_cast_impl<T, enable_if_t<is_integral_v<T>>> {
    T operator()(json_value const& jv) const {
        return (T)jv.get_number();
    }
};

template <typename T>
struct json_cast_impl<T, enable_if_t<is_floating_point_v<T>>> {
    T operator()(json_value const& jv) const {
        return (T)jv.get_number();
    }
};

template <typename T>
struct json_cast_impl<optional<T>> {
    optional<T> operator()(json_value const& jv) const {
        return json_cast_impl<T>()(jv);
    }
};

template <>
struct json_cast_impl<std::string> {
    std::string operator()(json_value const& jv) const {
        return to_string(jv.get_string());
    }
};

template <>
struct json_cast_impl<std::filesystem::path> {
    std::filesystem::path operator()(json_value const& jv) const {
        return to_string(jv.get_string());
    }
};

template <>
struct json_cast_impl<json_value> {
    json_value operator()(json_value const& jv) const {
        return jv;
    }
};

template <>
struct json_cast_impl<json_object> {
    json_object operator()(json_value const& jv) const {
        return jv.get_object();
    }
};

template <typename T>
T json_cast(json_value const& val) {
    return json_cast_impl<T>()(val);
}

}
