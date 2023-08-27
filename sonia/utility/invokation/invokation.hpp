//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <string>
#include <vector>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/string.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/variadic.hpp"
#include "sonia/shared_ptr.hpp"

#include "sonia/prime_config.hpp"

extern "C" {

enum class blob_type : uint8_t {
    unspecified = 0,     // just byte array if size != 0
    string = 1,          // data points to char*
    ui8 = 2, i8 = 3,
    ui16 = 4, i16 = 5,
    ui32 = 6, i32 = 7,
    ui64 = 9, i64 = 10, // if array, data points to int64*, size of array = size / 8
    flt = 11,           // if array, data points to float*, size of array = size / 4
    blob = 12,           // if array, data points to blob_type*, size of array = size / 16
    boolean = 13,
    function = 14
};

struct blob_result {
    union {
        void const* data;
        uint64_t ui64value;
        int64_t i64value;
        float floatvalue;
        uint32_t ui32value;
        int32_t i32value;
        uint16_t ui16value;
        int16_t i16value;
        uint8_t ui8value;
        int8_t i8value;
    };
    int32_t size; // always in bytes, defined only when data field is used
    uint8_t need_unpin : 7; // defined only when data field is used
    uint8_t is_array : 1; // defined only when data field is used
    blob_type type;
};

}

SONIA_PRIME_API void blob_result_allocate(blob_result *);
SONIA_PRIME_API void blob_result_pin(blob_result *);
SONIA_PRIME_API void blob_result_unpin(blob_result *);

inline bool blob_result_equal(sonia::identity<bool>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::boolean && lhs.i8value == rhs.i8value;
}

inline bool blob_result_equal(sonia::identity<int8_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i8 && lhs.i8value == rhs.i8value;
}

inline bool blob_result_equal(sonia::identity<uint8_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui8 && lhs.ui8value == rhs.ui8value;
}

inline bool blob_result_equal(sonia::identity<int16_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i16 && lhs.i16value == rhs.i16value;
}

inline bool blob_result_equal(sonia::identity<uint16_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui16 && lhs.ui16value == rhs.ui16value;
}

inline bool blob_result_equal(sonia::identity<int32_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i32 && lhs.i32value == rhs.i32value;
}

inline bool blob_result_equal(sonia::identity<uint32_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui32 && lhs.ui32value == rhs.ui32value;
}

inline bool blob_result_equal(sonia::identity<int64_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i64 && lhs.i64value == rhs.i64value;
}

inline bool blob_result_equal(sonia::identity<uint64_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui64 && lhs.ui64value == rhs.ui64value;
}

inline bool blob_result_equal(sonia::identity<float>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::flt && lhs.floatvalue == rhs.floatvalue;
}

inline bool blob_result_equal(sonia::identity<sonia::string_view>, blob_result const& lhs, blob_result const& rhs)
{
    if (rhs.type != blob_type::blob || rhs.size != lhs.size) return false;
    if (lhs.data == rhs.data) return true;
    return std::memcmp(lhs.data, rhs.data, rhs.size) == 0;
}

template <typename T>
inline bool blob_result_equal(sonia::identity<T>, blob_result const& lhs, blob_result const& rhs)
{
    return false;
}

#include "sonia/type_traits.hpp"
#include "sonia/string.hpp"

template <typename T>
inline consteval blob_type blob_type_for()
{
    using type = std::remove_cv_t<T>;
    if constexpr (std::is_same_v<bool, type>) {
        return blob_type::boolean;
    } else if constexpr (std::is_same_v<blob_result, type>) {
        return blob_type::blob;
    } else if constexpr (std::is_integral_v<type>) {
        constexpr bool signed_v = std::is_signed_v<type>;
        if constexpr (sizeof(T) * CHAR_BIT <= 8 && signed_v) return blob_type::i8;
        if constexpr (sizeof(T) * CHAR_BIT <= 8 && !signed_v) return blob_type::ui8;
        if constexpr (sizeof(T) * CHAR_BIT <= 16 && signed_v) return blob_type::i16;
        if constexpr (sizeof(T) * CHAR_BIT <= 16 && !signed_v) return blob_type::ui16;
        if constexpr (sizeof(T) * CHAR_BIT <= 32 && signed_v) return blob_type::i32;
        if constexpr (sizeof(T) * CHAR_BIT <= 32 && !signed_v) return blob_type::ui32;
        return signed_v ? blob_type::i64 : blob_type::ui64;
    } else if constexpr (std::is_floating_point_v<type>) {
        return blob_type::flt;
    } else {
        static_assert(sonia::dependent_false<T>);
    }
    return blob_type::unspecified;
}

template <typename T>
blob_result particular_blob_result(T && value);

inline blob_result nil_blob_result()
{
    return blob_result{ nullptr, 0, 0, 0, blob_type::unspecified };
}

inline blob_result function_blob_result(std::string_view value)
{
    return blob_result{ value.data(), static_cast<int32_t>(value.size()), 0, 0, blob_type::function };
}

inline blob_result bool_blob_result(bool value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::boolean };
    result.ui8value = value ? 1 : 0;
    return result;
}

inline blob_result ui8_blob_result(uint8_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::ui8 };
    result.ui8value = value;
    return result;
}

inline blob_result i8_blob_result(int8_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::i8 };
    result.i8value = value;
    return result;
}

inline blob_result ui16_blob_result(uint16_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::ui16 };
    result.ui16value = value;
    return result;
}

inline blob_result i16_blob_result(int16_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::i16 };
    result.i16value = value;
    return result;
}

inline blob_result ui32_blob_result(uint32_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::ui32 };
    result.ui32value = value;
    return result;
}

inline blob_result i32_blob_result(int32_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::i32 };
    result.i32value = value;
    return result;
}

inline blob_result ui64_blob_result(uint64_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::ui64 };
    result.ui64value = value;
    return result;
}

inline blob_result i64_blob_result(int64_t value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::i64 };
    result.i64value = value;
    return result;
}

inline blob_result float_blob_result(float value)
{
    blob_result result{ nullptr, 0, 0, 0, blob_type::flt };
    result.floatvalue = value;
    return result;
}

template <typename CharT, size_t N>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(const CharT(&value)[N])
{
    assert(!value[N - 1]);
    return blob_result{ value, static_cast<int32_t>(N - 1), 0, 0, blob_type::string };
}

template <typename CharT>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(sonia::experimental::basic_string_view<CharT> value)
{
    return blob_result{ value.data(), static_cast<int32_t>(value.size()), 0, 0, blob_type::string };
}

template <typename CharT>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(std::basic_string_view<CharT> value)
{
    return blob_result{ value.data(), static_cast<int32_t>(value.size()), 0, 0, blob_type::string };
}

template <typename CharT>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(std::basic_string<CharT> const& value)
{
    return blob_result{ value.data(), static_cast<int32_t>(value.size()), 0, 0, blob_type::string };
}

template <typename CharT>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(std::basic_string<CharT> && value)
{
    blob_result result{ value.data(), static_cast<int32_t>(value.size()), 0, 0, blob_type::string };
    blob_result_allocate(&result);
    return result;
}

template <typename CharT, size_t ET>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(std::span<CharT, ET> value)
{
    return blob_result{ value.data(), static_cast<int32_t>(value.size()), 0, 0, blob_type::string };
}

template <typename T>
inline blob_result optional_blob_result(sonia::optional<T> const& value)
{
    if (!value) return nil_blob_result();
    return particular_blob_result(*value);
}

template <typename FirstT, typename SecondT>
inline blob_result pair_blob_result(std::pair<FirstT, SecondT> const& value)
{
    blob_result data[2] = { particular_blob_result(value.first), nil_blob_result() };
    try {
        data[1] = particular_blob_result(value.second);
    } catch (...) {
        blob_result_unpin(&data[0]);
        throw;
    }
    blob_result result{ data, static_cast<int32_t>(2 * sizeof(blob_result)), 0, 1, blob_type::blob };
    blob_result_allocate(&result);
    return result;
}

template <size_t ... Is, typename TupleT>
inline blob_result tuple_blob_result(std::index_sequence<Is...>, TupleT const& value)
{
    blob_result data[std::tuple_size_v<TupleT>];
    std::fill(data, data + std::tuple_size_v<TupleT>, nil_blob_result());
    try {
        ([&data, &value] {data[Is] = particular_blob_result(std::get<Is>(value));}(), ...);
    } catch (...) {
        for (blob_result & br : data) {
            blob_result_unpin(&br);
        }
        throw;
    }
    blob_result result{ data, static_cast<int32_t>(std::tuple_size_v<TupleT> *sizeof(blob_result)), 0, 1, blob_type::blob };
    blob_result_allocate(&result);
    return result;
}

template <typename ... ElementsT>
inline blob_result tuple_blob_result(std::tuple<ElementsT...> const& value)
{
    return tuple_blob_result(std::make_index_sequence<sizeof ...(ElementsT)>(), value);
}

template <typename T, size_t EV>
inline blob_result array_blob_result(std::span<T, EV> arr, bool pinned = false)
{
    return blob_result{ arr.data(), static_cast<int32_t>(arr.size() * sizeof(T)), (uint8_t)(pinned ? 1 : 0), 1, blob_type_for<T>() };
}

template <typename T, size_t N>
inline blob_result array_blob_result(T(&arr)[N])
{
    return blob_result{ arr, static_cast<int32_t>(N * sizeof(T)), 0, 1, blob_type_for<T>() };
}

/*
template <size_t N>
inline blob_result array_blob_result(blob_result(&arr)[N])
{
    return blob_result{ arr, static_cast<int32_t>(N * sizeof(blob_result)), 0, 1, blob_type::blob };
}


*/

//template <size_t N>
//inline blob_result array_blob_result(int64_t(&arr)[N])
//{
//     return blob_result{ arr, static_cast<int32_t>(N * sizeof(int64_t)), 0, 1, blob_type::i64 };
//}
//
//template <size_t N>
//inline blob_result array_blob_result(uint64_t(&arr)[N])
//{
//    return blob_result{ arr, static_cast<int32_t>(N * sizeof(uint64_t)), 0, 1, blob_type::ui64 };
//}



template <typename ArgT>
inline blob_result particular_blob_result(ArgT && value)
{
    using T = sonia::remove_cvref_t<ArgT>;
    if constexpr (std::is_same_v<T, bool>) return bool_blob_result(value);
    else if constexpr (std::is_same_v<T, uint8_t>) return ui8_blob_result(value);
    else if constexpr (std::is_same_v<T, int8_t>) return i8_blob_result(value);
    else if constexpr (std::is_same_v<T, uint16_t>) return ui16_blob_result(value);
    else if constexpr (std::is_same_v<T, int16_t>) return i16_blob_result(value);
    else if constexpr (std::is_same_v<T, uint32_t> || (std::is_integral_v<T> && !std::is_signed_v<T> && sizeof(T) == 4)) return ui32_blob_result(value);
    else if constexpr (std::is_same_v<T, int32_t> || (std::is_integral_v<T> && std::is_signed_v<T> && sizeof(T) == 4)) return i32_blob_result(value);
    else if constexpr (std::is_same_v<T, uint64_t> || (std::is_integral_v<T> && !std::is_signed_v<T> && sizeof(T) == 8)) return ui64_blob_result(value);
    else if constexpr (std::is_same_v<T, int64_t> || (std::is_integral_v<T> && std::is_signed_v<T> && sizeof(T) == 8)) return i64_blob_result(value);
    else if constexpr (std::is_same_v<T, float>) return float_blob_result(value);
    else if constexpr (std::is_same_v<T, std::string_view>) return string_blob_result(value);
    else if constexpr (std::is_same_v<ArgT&&, std::string const&>) return string_blob_result(std::forward<ArgT>(value));
    else if constexpr (std::is_same_v<ArgT&&, std::string&&>) return string_blob_result(std::forward<ArgT>(value));
    else if constexpr (std::is_same_v<T, blob_result>) return value;
    else if constexpr (sonia::is_template_instance_v<sonia::optional, T>) return optional_blob_result(std::forward<ArgT>(value));
    else if constexpr (sonia::is_template_instance_v<std::pair, T>) return pair_blob_result(std::forward<ArgT>(value));
    else if constexpr (sonia::is_template_instance_v<std::tuple, T>) return tuple_blob_result(std::forward<ArgT>(value));
    else {
        static_assert(sonia::dependent_false<T>);
        return nil_blob_result();
    }
}

template <typename FT>
auto blob_result_type_selector(blob_result b, FT&& ftor)
{
    switch (b.type)
    {
    case blob_type::boolean:
        return ftor(sonia::identity<bool>(), b);
    case blob_type::i8:
        return ftor(sonia::identity<int8_t>(), b);
    case blob_type::ui8:
        return ftor(sonia::identity<uint8_t>(), b);
    case blob_type::i16:
        return ftor(sonia::identity<int16_t>(), b);
    case blob_type::ui16:
        return ftor(sonia::identity<uint16_t>(), b);
    case blob_type::i32:
        return ftor(sonia::identity<int32_t>(), b);
    case blob_type::ui32:
        return ftor(sonia::identity<uint32_t>(), b);
    case blob_type::i64:
        return ftor(sonia::identity<int64_t>(), b);
    case blob_type::ui64:
        return ftor(sonia::identity<uint64_t>(), b);
    case blob_type::flt:
        return ftor(sonia::identity<float>(), b);
    case blob_type::string:
        return ftor(sonia::identity<sonia::string_view>(), b);
    case blob_type::blob:
        return ftor(sonia::identity<blob_result>(), b);
    case blob_type::unspecified:
    default:
        return ftor(sonia::identity<void>(), b);
    }
}

inline std::ostream& operator<<(std::ostream& os, blob_result const& b)
{
    if (b.type == blob_type::unspecified && !b.data) {
        return os << "nil";
    }
    if (b.is_array) {
        os << "[";
        blob_result_type_selector(b, [&os](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
            using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;
            for (int32_t i = 0; i < b.size / sizeof(fstype); ++i) {
                os << (i ? "," : "");
                if constexpr (std::is_same_v<std::string_view, fstype>) {
                    blob_result const* pelem = reinterpret_cast<blob_result const*>(b.data) + i;
                    os << *pelem;
                } else {
                    os << particular_blob_result(*(reinterpret_cast<fstype const *>(b.data) + i));
                    //fstype const *pelem = reinterpret_cast<fstype const *>(b.data) + i;
                    //os << *pelem;
                }
            }
        });
        return os << "]";
    }
    switch (b.type)
    {
    case blob_type::unspecified:
        if (!b.data) return os << "nil";
        return os << "unspecified";
    case blob_type::boolean:
        return os << (b.i8value ? "true" : "false");
    case blob_type::i8:
        return os << (int)b.i8value << ":i8";
    case blob_type::ui8:
        return os << (int)b.ui8value << ":ui8";
    case blob_type::i16:
        return os << (int)b.i16value << ":i16";
    case blob_type::ui16:
        return os << (int)b.ui16value << ":ui16";
    case blob_type::i32:
        return os << b.i32value << ":i32";
    case blob_type::ui32:
        return os << b.ui32value << ":ui32";
    case blob_type::i64:
        return os << b.i64value << ":i64";
    case blob_type::ui64:
        return os << b.ui64value << ":ui64";
    case blob_type::flt:
        return os << b.floatvalue << ":float";
    case blob_type::string:
        return os << '"' << sonia::string_view(reinterpret_cast<const char*>(b.data), b.size) << '"';
    case blob_type::blob:
        return os << "blob";
    case blob_type::function:
        return os << "function";
    }
    return os;
}

template <typename T>
struct from_blob
{
    T operator()(blob_result val) const {
        using namespace sonia;
        static_assert(dependent_false<T>);
        THROW_INTERNAL_ERROR("can't convert blob %1% to type %2%"_fmt % val % typeid(T).name());
    }
};

template <typename T>
struct from_blob<sonia::optional<T>>
{
    sonia::optional<T> operator()(blob_result val) const {
        using namespace sonia;
        if (val.type == blob_type::unspecified && !val.data) {
            return nullopt;
        }
        return from_blob<T>{}(val);
    }
};

template <>
struct from_blob<bool>
{
    bool operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::boolean) {
            return !!val.i8value;
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to bool"_fmt % val);
    }
};

template <>
struct from_blob<blob_result>
{
    blob_result operator()(blob_result val) const { return val; }
};

template <std::integral T>
struct from_blob<T>
{
    T operator()(blob_result val) const
    {
        using namespace sonia;
        switch (val.type)
        {
        case blob_type::boolean:
            return (T)val.i8value;
        case blob_type::i8:
            return (T)val.i8value;
        case blob_type::ui8:
            return (T)val.ui8value;
        case blob_type::i16:
            return (T)val.i16value;
        case blob_type::ui16:
            return (T)val.ui16value;
        case blob_type::i32:
            return (T)val.i32value;
        case blob_type::ui32:
            return (T)val.ui32value;
        case blob_type::i64:
            return (T)val.i64value;
        case blob_type::ui64:
            return (T)val.ui64value;
        default:
            THROW_INTERNAL_ERROR("can't convert blob %1% to %2%"_fmt % val % typeid(T).name());
        }
    }
};

/*
template <typename CharT>
struct from_blob<sonia::basic_string_view<CharT>>
{
    using view_t = sonia::basic_string_view<CharT>;
    view_t operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::string) {
            return view_t{ reinterpret_cast<const CharT*>(val.data), (size_t)val.size };
        }
        throw exception("can't convert blob %1% to basic_string_view"_fmt % val);
    }
};
*/

template <typename CharT>
requires (sizeof(CharT) == 1)
struct from_blob<std::span<CharT>>
{
    using span_t = std::span<CharT>;
    span_t operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::string ||
            (val.is_array && (val.type == blob_type::unspecified || val.type == blob_type::i8 || val.type == blob_type::ui8)))
        {
            return span_t{ reinterpret_cast<CharT*>(val.data), (size_t)val.size };
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::span<CharT>"_fmt % val);
        //THROW_INTERNAL_ERROR("can't convert blob %1% to std::span<%2%>"_fmt % val % typeid(CharT).name());
    }
};

template <typename T>
requires (sizeof(T) > 1)
struct from_blob<std::span<T>>
{
    using span_t = std::span<T>;
    span_t operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type_for<T>() && val.is_array) {
            return span_t{ reinterpret_cast<T*>(val.data), (size_t)val.size / sizeof(T) };
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::span<%2%>"_fmt % val % typeid(T).name());
    }
};

template <typename CharT, typename TraitsT>
struct from_blob<std::basic_string_view<CharT, TraitsT>>
{
    using view_t = std::basic_string_view<CharT, TraitsT>;
    view_t operator()(blob_result const& val) const
    {
        using namespace sonia;
        if (val.type == blob_type::string) {
            return view_t{ reinterpret_cast<const CharT*>(val.data), (size_t)val.size };
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::basic_string_view"_fmt % val);
    }
};

template <typename CharT, typename CharTraitsT>
struct from_blob<std::basic_string<CharT, CharTraitsT>>
{
    using str_t = std::basic_string<CharT, CharTraitsT>;
    str_t operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::string) {
            return str_t{ reinterpret_cast<const CharT*>(val.data), (size_t)val.size };
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::basic_string"_fmt % val);
    }
};

template <typename ... Ts>
struct from_blob<std::tuple<Ts...>>
{
    using tuple_t = std::tuple<Ts...>;
    tuple_t operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::blob && val.is_array) {
            return this->operator()(std::make_index_sequence<sizeof ...(Ts)>{}, reinterpret_cast<blob_result const*>(val.data));
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::tuple"_fmt % val);
    }

    template <size_t ... Is>
    tuple_t operator()(std::index_sequence<Is...>, blob_result const* data) const
    {
        return { from_blob<std::tuple_element_t<Is, tuple_t>>()(data[Is]) ... };
    }
};

template <typename T>
T from_blob_at(size_t index, std::span<const blob_result> vals)
{
    if (index >= vals.size()) {
        if constexpr (std::is_same_v<T, blob_result>) {
            return array_blob_result(std::span<const blob_result>{}); // to be interpreted as no value
        } else {
            return T{};
        }
    }
    return from_blob<T>{}(vals[index]);
}

template <typename ... Ts, size_t ... Is>
std::tuple<Ts...> from_blobs(std::index_sequence<Is...>, std::span<const blob_result> vals)
{
    return std::tuple<Ts...>{ from_blob_at<sonia::variadic::type_at_t<Is, Ts...>>(Is, vals) ... };
}

template <typename ... Ts>
std::tuple<Ts...> from_blobs(std::span<const blob_result> vals)
{
    return from_blobs<Ts...>(std::make_index_sequence<sizeof ...(Ts)>{}, vals);
}

inline bool operator== (blob_result const& lhs, blob_result const& rhs)
{
    return blob_result_type_selector(lhs, [&rhs](auto id, blob_result const& lhs) { return blob_result_equal(id, lhs, rhs); });
}

inline bool operator!= (blob_result const& lhs, blob_result const& rhs)
{
    return !(lhs == rhs);
}

namespace sonia {

class smart_blob : blob_result
{
public:
    smart_blob() : blob_result{ nil_blob_result() } {}
    smart_blob(blob_result && b) : blob_result{ b }
    {}

    smart_blob(smart_blob const& rhs)
        : blob_result { rhs }
    {
        blob_result_pin(this);
    }

    smart_blob(smart_blob && rhs) noexcept
        : blob_result{ rhs }
    {
        static_cast<blob_result&>(rhs) = nil_blob_result();
    }

    smart_blob& operator= (smart_blob const& rhs)
    {
        smart_blob tmp{ rhs };
        swap(tmp);
        return *this;
    }

    smart_blob& operator= (smart_blob && rhs)
    {
        smart_blob tmp{ std::move(rhs) };
        swap(tmp);
        return *this;
    }

    inline void swap(smart_blob & rhs) noexcept
    {
        std::swap(static_cast<blob_result&>(*this), static_cast<blob_result&>(rhs));
    }

    ~smart_blob()
    {
        blob_result_unpin(this);
    }

    blob_result const& operator*() const { return *this; }
    blob_result const& get() const { return *this; }
    blob_result const* operator->() const { return this; }

    void allocate()
    {
        blob_result_allocate(this);
    }

    blob_result detach()
    {
        smart_blob tmp;
        swap(tmp);
        return tmp;
    }
};

}
