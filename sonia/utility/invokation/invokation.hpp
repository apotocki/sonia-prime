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
#include "sonia/utility/number/float16.hpp"
#include "sonia/prime_config.hpp"

namespace sonia::invokation { class invokable; }

extern "C" {

// X XX XXXXX-type
// | |- subtype
// |- is array

enum class blob_type : uint8_t {
    nil = 0,
    boolean = 1,
    c8 = 2,
    ui8 = 3, i8 = 4,
    ui16 = 5, i16 = 6,
    ui32 = 7, i32 = 8,
    ui64 = 9, i64 = 10,
    flt16 = 11,
    flt32 = 12,
    flt64 = 13,
    //flt80 = 14,
    object = 15, // pointer to object unowned if need_unpin = 0
    //weakobject = 0x10, //pointer to object?
    //blob = 0x13, // used only as a decayed tuple type
    is_array = 0x80,
    string = 0x82, // isarray | c8
    tuple = 0x93, // isarray | 0x13
    // flt128, bigint, decimal as array types
    function = 0xb4, // like the string
    error = 0xc4, // like the string
    
};

/*
struct blob_result {
    union {
        void const* data;
        sonia::invokation::invokable* object;
        uint64_t ui64value;
        int64_t i64value;
        sonia::float16 f16value;
        float_t f32value;
        double_t f64value;
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
//*/

//#pragma pack(push)
//#pragma pack(1)
#pragma pack(push, 1)
struct alignas(8) blob_result
{
    union {
        struct {
            union {
                void const* data;
                sonia::invokation::invokable* object;
                uint64_t ui64value;
                int64_t i64value;
                sonia::float16 f16value;
                float_t f32value;
                double_t f64value;
                uint32_t ui32value;
                int32_t i32value;
                uint16_t ui16value;
                int16_t i16value;
                uint8_t ui8value;
                int8_t i8value;
            };
            uint32_t size;
            uint16_t reserved;
        } bp; // base_plane
        uint8_t ui8array[14];
    };
    uint8_t inplace_size : 4;
    uint8_t need_unpin : 4;
    blob_type type;
};
#pragma pack(pop)

static_assert(sizeof(blob_result) == 16);

typedef void(*on_invoke_cv_result_setter)(void*, blob_result*, uint32_t); // cookie, results, result count

SONIA_PRIME_API void blob_result_allocate(blob_result *);
SONIA_PRIME_API void blob_result_pin(blob_result *);
SONIA_PRIME_API void blob_result_unpin(blob_result *);

}

inline blob_type operator | (blob_type l, blob_type r) noexcept
{
    return (blob_type)((uint8_t)l | (uint8_t)r);
}

inline auto operator <=> (blob_type l, blob_type r) noexcept
{
    return ((uint8_t)l) <=> ((uint8_t)r);
}

inline bool is_basic_integral(blob_type val) noexcept { return val >= blob_type::ui8 && val <= blob_type::i64; }

inline bool is_nil(blob_result const& val) noexcept { return val.type == blob_type::nil; }

inline bool is_array(blob_result const& val) noexcept { return (((uint8_t)val.type) & 0x80) != 0; }
inline bool contains_string(blob_result const& val) noexcept
{
    return val.type == blob_type::string || val.type == blob_type::error || val.type == blob_type::function;
}

template <typename T>
inline const T* data_of(blob_result const& val) noexcept
{
    return val.inplace_size ? reinterpret_cast<T const*>(val.ui8array) : reinterpret_cast<T const*>(val.bp.data);
}

template <typename T>
inline T* mutable_data_of(blob_result const& val) noexcept
{
    return const_cast<T*>(data_of<T>(val));
}

template <typename T>
inline size_t array_size_of(blob_result const& val) noexcept
{
    size_t raw_sz = (val.inplace_size ? (size_t)val.inplace_size : (size_t)val.bp.size);
    BOOST_ASSERT(0 == raw_sz % sizeof(T));
    return raw_sz / sizeof(T);
}

/*
inline bool blob_result_equal(sonia::identity<bool>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::boolean && lhs.bp.i8value == rhs.bp.i8value;
}

inline bool blob_result_equal(sonia::identity<int8_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i8 && lhs.bp.i8value == rhs.bp.i8value;
}

inline bool blob_result_equal(sonia::identity<uint8_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui8 && lhs.bp.ui8value == rhs.bp.ui8value;
}

inline bool blob_result_equal(sonia::identity<int16_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i16 && lhs.bp.i16value == rhs.bp.i16value;
}

inline bool blob_result_equal(sonia::identity<uint16_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui16 && lhs.bp.ui16value == rhs.bp.ui16value;
}

inline bool blob_result_equal(sonia::identity<int32_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i32 && lhs.bp.i32value == rhs.bp.i32value;
}

inline bool blob_result_equal(sonia::identity<uint32_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui32 && lhs.bp.ui32value == rhs.bp.ui32value;
}

inline bool blob_result_equal(sonia::identity<int64_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::i64 && lhs.bp.i64value == rhs.bp.i64value;
}

inline bool blob_result_equal(sonia::identity<uint64_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::ui64 && lhs.ui64value == rhs.ui64value;
}

inline bool blob_result_equal(sonia::identity<sonia::float16>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::flt16 && lhs.f16value == rhs.f16value;
}


inline bool blob_result_equal(sonia::identity<float_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::flt32 && lhs.f32value == rhs.f32value;
}

inline bool blob_result_equal(sonia::identity<double_t>, blob_result const& lhs, blob_result const& rhs)
{
    return rhs.type == blob_type::flt64 && lhs.f64value == rhs.f64value;
}

inline bool blob_result_equal(sonia::identity<sonia::string_view>, blob_result const& lhs, blob_result const& rhs)
{
    if (rhs.type != blob_type::blob || rhs.size != lhs.size) return false;
    if (lhs.data == rhs.data) return true;
    return std::memcmp(lhs.data, rhs.data, rhs.size) == 0;
}
*/

template <typename T>
inline bool blob_result_equal(sonia::identity<T>, blob_result const& lhs, blob_result const& rhs)
{
    return false;
}

template <std::integral T>
inline bool blob_result_equal(sonia::identity<T>, blob_result const& lhs, blob_result const& rhs)
{
    return as<T>(lhs) == as<T>(rhs);
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
        return blob_type::tuple;
    } else if constexpr (std::is_integral_v<type>) {
        constexpr bool signed_v = std::is_signed_v<type>;
        if constexpr (sizeof(T) * CHAR_BIT <= 8 && signed_v) return blob_type::i8;
        if constexpr (sizeof(T) * CHAR_BIT <= 8 && !signed_v) return blob_type::ui8;
        if constexpr (sizeof(T) * CHAR_BIT <= 16 && signed_v) return blob_type::i16;
        if constexpr (sizeof(T) * CHAR_BIT <= 16 && !signed_v) return blob_type::ui16;
        if constexpr (sizeof(T) * CHAR_BIT <= 32 && signed_v) return blob_type::i32;
        if constexpr (sizeof(T) * CHAR_BIT <= 32 && !signed_v) return blob_type::ui32;
        return signed_v ? blob_type::i64 : blob_type::ui64;
    } else if constexpr (std::is_same_v<type, sonia::float16> || (std::is_floating_point_v<type> && sizeof(type) == 2)) {
        return blob_type::flt16;
    } else if constexpr (std::is_floating_point_v<type> && sizeof(type) == 4) {
        return blob_type::flt32;
    } else if constexpr (std::is_floating_point_v<type> && sizeof(type) == 8) {
        return blob_type::flt64;
    } else {
        static_assert(sonia::dependent_false<T>);
    }
    return blob_type::nil;
}

template <typename T>
blob_result particular_blob_result(T && value);

inline blob_result make_blob_result(blob_type bt, void const* data = nullptr, uint32_t size = 0)
{
    return blob_result { data, /*size*/ size, /*reserved*/ 0, /*inplace_size*/ 0, /*need_unpin*/ 0, bt };
}

inline blob_result nil_blob_result()
{
    return make_blob_result(blob_type::nil);
}

inline blob_result function_blob_result(std::string_view value)
{
    blob_result result = make_blob_result(blob_type::function, value.data());
    result.bp.size = static_cast<uint32_t>(value.size());
    return result;
}

inline blob_result object_blob_result(sonia::invokation::invokable* value)
{
    blob_result result = make_blob_result(blob_type::object);
    result.bp.object = value;
    return result;
}

inline blob_result bool_blob_result(bool value)
{
    blob_result result = make_blob_result(blob_type::boolean);
    result.bp.ui8value = value ? 1 : 0;
    return result;
}

inline blob_result ui8_blob_result(uint8_t value)
{
    blob_result result = make_blob_result(blob_type::ui8);
    result.bp.ui8value = value;
    return result;
}

inline blob_result i8_blob_result(int8_t value)
{
    blob_result result = make_blob_result(blob_type::i8);
    result.bp.i8value = value;
    return result;
}

inline blob_result char_blob_result(char value)
{
    blob_result result = make_blob_result(blob_type::c8);
    result.bp.i8value = value;
    return result;
}

inline blob_result ui16_blob_result(uint16_t value)
{
    blob_result result = make_blob_result(blob_type::ui16);
    result.bp.ui16value = value;
    return result;
}

inline blob_result i16_blob_result(int16_t value)
{
    blob_result result = make_blob_result(blob_type::i16);
    result.bp.i16value = value;
    return result;
}

inline blob_result ui32_blob_result(uint32_t value)
{
    blob_result result = make_blob_result(blob_type::ui32);
    result.bp.ui32value = value;
    return result;
}

inline blob_result i32_blob_result(int32_t value)
{
    blob_result result = make_blob_result(blob_type::i32);
    result.bp.i32value = value;
    return result;
}

inline blob_result ui64_blob_result(uint64_t value)
{
    blob_result result = make_blob_result(blob_type::ui64);
    result.bp.ui64value = value;
    return result;
}

inline blob_result i64_blob_result(int64_t value)
{
    blob_result result = make_blob_result(blob_type::i64);
    result.bp.i64value = value;
    return result;
}

inline blob_result f16_blob_result(sonia::float16 value)
{
    blob_result result = make_blob_result(blob_type::flt16);
    result.bp.f16value = value;
    return result;
}

inline blob_result f32_blob_result(float_t value)
{
    blob_result result = make_blob_result(blob_type::flt32);
    result.bp.f32value = value;
    return result;
}

inline blob_result f64_blob_result(double_t value)
{
    blob_result result = make_blob_result(blob_type::flt64);
    result.bp.f64value = value;
    return result;
}

template <typename CharT, size_t N>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(const CharT(&value)[N], blob_type t = blob_type::string)
{
    assert(!value[N - 1]);
    blob_result result = make_blob_result(t, value);
    result.bp.size = static_cast<uint32_t>(N - 1);
    return result;
}

template <typename T>
requires (sonia::is_string_v<std::remove_cvref_t<T>> && sizeof(typename std::remove_cvref_t<T>::value_type) == 1)
inline blob_result string_blob_result(T && value, blob_type t = blob_type::string)
{
    blob_result result = make_blob_result(t, value.data(), static_cast<uint32_t>(value.size()));
    if constexpr (std::is_rvalue_reference_v<T&&> && sonia::is_template_instance_v<std::basic_string, T>) {
        blob_result_allocate(&result);
        //static_assert(sonia::dependent_false<T>);
    }
    return result;
}

template <typename CharT, size_t ET>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(std::span<CharT, ET> value, blob_type t = blob_type::string)
{
    return make_blob_result(t, value.data(), static_cast<uint32_t>(value.size()));
}

template <typename ArgT>
inline blob_result error_blob_result(ArgT && arg)
{
    return string_blob_result(std::forward<ArgT>(arg), blob_type::error);
}

template <typename T>
inline blob_result optional_blob_result(sonia::optional<T> const& value)
{
    if (!value) return nil_blob_result();
    return particular_blob_result(*value);
}

template <typename T, size_t EV>
requires(std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<blob_result, std::remove_cv_t<T>>)
inline blob_result array_blob_result(std::span<T, EV> arr)
{
    return make_blob_result(blob_type_for<T>() | blob_type::is_array, arr.data(), static_cast<uint32_t>(arr.size() * sizeof(T)));
}

template <typename T, size_t N>
requires(std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<blob_result, std::remove_cv_t<T>>)
inline blob_result array_blob_result(T(&arr)[N])
{
    return make_blob_result(blob_type_for<T>() | blob_type::is_array, arr, static_cast<uint32_t>(N * sizeof(T)));
}

template <typename FirstT, typename SecondT>
inline blob_result pair_blob_result(std::pair<FirstT, SecondT> const& value)
{
    if constexpr (std::is_same_v<FirstT, SecondT>) {
        FirstT data[2] = {value.first, value.second};
        blob_result result = array_blob_result(data);
        blob_result_allocate(&result);
        return result;
    } else {
        blob_result data[2] = { particular_blob_result(value.first), nil_blob_result() };
        try {
            data[1] = particular_blob_result(value.second);
        } catch (...) {
            blob_result_unpin(&data[0]);
            throw;
        }
        blob_result result = make_blob_result( blob_type::tuple, data, static_cast<uint32_t>(2 * sizeof(blob_result)));
        blob_result_allocate(&result);
        return result;
    }
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
    blob_result result = make_blob_result(blob_type::tuple, data, static_cast<uint32_t>(std::tuple_size_v<TupleT> * sizeof(blob_result)));
    blob_result_allocate(&result);
    return result;
}

template <typename ... ElementsT>
inline blob_result tuple_blob_result(std::tuple<ElementsT...> const& value)
{
    return tuple_blob_result(std::make_index_sequence<sizeof ...(ElementsT)>(), value);
}

template <typename ArgT>
inline blob_result particular_blob_result(ArgT && value)
{
    using T = sonia::remove_cvref_t<ArgT>;
    if constexpr (std::is_same_v<T, bool>) return bool_blob_result(value);
    else if constexpr (std::is_same_v<T, uint8_t>) return ui8_blob_result(value);
    else if constexpr (std::is_same_v<T, int8_t>) return i8_blob_result(value);
    else if constexpr (std::is_same_v<T, char>) return char_blob_result(value);
    else if constexpr (std::is_same_v<T, uint16_t>) return ui16_blob_result(value);
    else if constexpr (std::is_same_v<T, int16_t>) return i16_blob_result(value);
    else if constexpr (std::is_same_v<T, uint32_t> || (std::is_integral_v<T> && !std::is_signed_v<T> && sizeof(T) == 4)) return ui32_blob_result(value);
    else if constexpr (std::is_same_v<T, int32_t> || (std::is_integral_v<T> && std::is_signed_v<T> && sizeof(T) == 4)) return i32_blob_result(value);
    else if constexpr (std::is_same_v<T, uint64_t> || (std::is_integral_v<T> && !std::is_signed_v<T> && sizeof(T) == 8)) return ui64_blob_result(value);
    else if constexpr (std::is_same_v<T, int64_t> || (std::is_integral_v<T> && std::is_signed_v<T> && sizeof(T) == 8)) return i64_blob_result(value);
    else if constexpr (std::is_same_v<T, sonia::float16>) return f16_blob_result(value);
    else if constexpr (std::is_same_v<T, float_t>) return f32_blob_result(value);
    else if constexpr (std::is_same_v<T, double_t>) return f64_blob_result(value);
    else if constexpr (sonia::is_string_v<T>) return string_blob_result(std::forward<ArgT>(value));
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
auto blob_type_selector(blob_result const& b, FT&& ftor)
{
    switch (b.type) {
    case blob_type::tuple:
        return ftor(sonia::identity<blob_result>(), b);
    case blob_type::string:
    case blob_type::error:
        return ftor(sonia::identity<char>(), b);
        default:
            break;
    }
    blob_type decayed_type = (blob_type)(((uint8_t)b.type) & 0x7f);
    switch (decayed_type)
    {
    case blob_type::boolean:
        return ftor(sonia::identity<bool>(), b);
    case blob_type::i8:
        return ftor(sonia::identity<int8_t>(), b);
    case blob_type::ui8:
        return ftor(sonia::identity<uint8_t>(), b);
    case blob_type::c8:
        return ftor(sonia::identity<char>(), b);
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
    case blob_type::flt16:
        return ftor(sonia::identity<sonia::float16>(), b);
    case blob_type::flt32:
        return ftor(sonia::identity<float_t>(), b);
    case blob_type::flt64:
        return ftor(sonia::identity<double_t>(), b);
    case blob_type::tuple:
        return ftor(sonia::identity<blob_result>(), b);
    default:
        break;
    }
    return ftor(sonia::identity<void>(), b);
}

inline std::ostream& operator<<(std::ostream& os, blob_result const& b)
{
    if (b.type == blob_type::nil) {
        return os << "nil";
    }
    if (is_array(b) && !contains_string(b)) {
        os << "[";
        blob_type_selector(b, [&os](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            if constexpr (std::is_void_v<type>) { os << "unknown"; }
            else {
                using fstype = std::conditional_t<std::is_same_v<type, bool>, uint8_t, type>;
                fstype const* begin_ptr = data_of<fstype>(b);
                for (auto* p = begin_ptr, *e = begin_ptr + array_size_of<fstype>(b); p != e; ++p) {
                    os << ((p != begin_ptr) ? "," : "");
                    os << particular_blob_result((type)*p);
                }
            }
        });
        return os << "]";
    }
    switch (b.type)
    {
    case blob_type::nil:
        return os << "nil";
    case blob_type::boolean:
        return os << (b.bp.i8value ? "true" : "false");
    case blob_type::c8:
        return os << '\'' << (char)b.bp.i8value<< '\'';
    case blob_type::i8:
        return os << (int)b.bp.i8value << ":i8";
    case blob_type::ui8:
        return os << (int)b.bp.ui8value << ":ui8";
    case blob_type::i16:
        return os << (int)b.bp.i16value << ":i16";
    case blob_type::ui16:
        return os << (int)b.bp.ui16value << ":ui16";
    case blob_type::i32:
        return os << b.bp.i32value << ":i32";
    case blob_type::ui32:
        return os << b.bp.ui32value << ":ui32";
    case blob_type::i64:
        return os << b.bp.i64value << ":i64";
    case blob_type::ui64:
        return os << b.bp.ui64value << ":ui64";
    case blob_type::flt16:
        return os << b.bp.f16value << ":f16";
    case blob_type::flt32:
        return os << b.bp.f32value << ":f32";
    case blob_type::flt64:
        return os << b.bp.f64value << ":f64";
    case blob_type::string:
        return os << '"' << sonia::string_view{data_of<char>(b), array_size_of<char>(b)} << '"';
    case blob_type::function:
        return os << "function";
    case blob_type::object:
        return os << "object";
    case blob_type::error:
        return os << "error: " << sonia::string_view{ data_of<char>(b), array_size_of<char>(b) };
    default:
        return os << "unknown";
    }
}

inline std::ostream& print_type(std::ostream& os, blob_result const& b)
{
    if (b.type == blob_type::nil) {
        return os << "nil";
    } else if (b.type == blob_type::string) {
        return os << "string";
    } else if (b.type == blob_type::error) {
        return os << "error";
    }

    if (is_array(b)) {
        os << "[";
    }
    blob_type decayed_type = (blob_type)(((uint8_t)b.type) & 0x7f);
    switch (decayed_type)
    {
    case blob_type::boolean:
        os << "bool"; break;
    case blob_type::i8:
        os << "i8"; break;
    case blob_type::ui8:
        os << "ui8"; break;
    case blob_type::i16:
        os << "i16"; break;
    case blob_type::ui16:
        os << "ui16"; break;
    case blob_type::i32:
        os << "i32"; break;
    case blob_type::ui32:
        os << "ui32"; break;
    case blob_type::i64:
        os << "i64"; break;
    case blob_type::ui64:
        os << "ui64"; break;
    case blob_type::flt16:
        os << "f16"; break;
    case blob_type::flt32:
        os << "f32"; break;
    case blob_type::flt64:
        os << "f64"; break;
    case blob_type::function:
        os << "function"; break;
    case blob_type::object:
        os << "object"; break;
    default:
        os << "unknown";
    }
    if (is_array(b)) {
        os << "]";
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
        if (val.type == blob_type::nil) {
            return nullopt;
        }
        return from_blob<T>{}(val);
    }
};

template <std::derived_from<sonia::invokation::invokable> T>
struct from_blob<T>
{
    T& operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::object) {
            if (T* result = dynamic_cast<T*>(val.bp.object); result) {
                return *result;
            }
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to object %2%"_fmt % val % typeid(T).name());
    }
};

template <>
struct from_blob<bool>
{
    bool operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::boolean) {
            return !!val.bp.i8value;
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to bool"_fmt % val);
    }
};

template <>
struct from_blob<blob_result>
{
    blob_result operator()(blob_result val) const { return val; }
};

template <typename T>
requires (std::is_integral_v<T> || std::is_floating_point_v<T>)
struct from_blob<T>
{
    T operator()(blob_result val) const
    {
        using namespace sonia;
        if (!is_array(val)) {
            switch (val.type)
            {
            case blob_type::boolean:
                return (T)val.bp.i8value;
            case blob_type::i8:
                return (T)val.bp.i8value;
            case blob_type::ui8:
                return (T)val.bp.ui8value;
            case blob_type::i16:
                return (T)val.bp.i16value;
            case blob_type::ui16:
                return (T)val.bp.ui16value;
            case blob_type::i32:
                return (T)val.bp.i32value;
            case blob_type::ui32:
                return (T)val.bp.ui32value;
            case blob_type::i64:
                return (T)val.bp.i64value;
            case blob_type::ui64:
                return (T)val.bp.ui64value;
            case blob_type::flt16:
                return (T)(float)val.bp.f16value;
            case blob_type::flt32:
                return (T)val.bp.f32value;
            case blob_type::flt64:
                return (T)val.bp.f64value;
            default:
                break;
            }
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to %2%"_fmt % val % typeid(T).name());
    }
};

template <typename CharT>
requires (sizeof(CharT) == 1)
struct from_blob<std::span<const CharT>>
{
    using span_t = std::span<const CharT>;
    span_t operator()(blob_result const& val) const
    {
        using namespace sonia;
        
        blob_type decayed_type = (blob_type)(((uint8_t)val.type) & 0x7f);
        if (val.type == blob_type::string || (is_array(val) && (decayed_type == blob_type::i8 || decayed_type == blob_type::ui8)))
        {
            CharT const* begin_ptr = data_of<CharT>(val);
            size_t sz = array_size_of<CharT>(val);
            return span_t{ begin_ptr, sz };
        } else if (!is_array(val) && (val.type == blob_type::i8 || val.type == blob_type::ui8 || decayed_type == blob_type::c8)) {
            return span_t{ reinterpret_cast<CharT const*>(&val.bp.ui8value), (size_t)1 };
        }
#ifdef BOOST_WINDOWS
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::span<%2%>"_fmt % val % typeid(CharT).name());
#else
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::span<CharT>"_fmt % val);
#endif   
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
        blob_type decayed_type = (blob_type)(((uint8_t)val.type) & 0x7f);
        if (decayed_type == blob_type_for<T>()) {
            if (is_array(val)) {
                T const* begin_ptr = data_of<T>(val);
                size_t sz = array_size_of<T>(val);
                return span_t{ begin_ptr, sz };
            } else {
                return span_t{ reinterpret_cast<T*>(&val.bp.data), 1 };
            }
        }
#ifdef BOOST_WINDOWS
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::span<%2%>"_fmt % val % typeid(T).name());
#else
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::span<T>"_fmt % val);
#endif
    }
};

template <sonia::string_type T>
requires (sizeof(typename T::value_type) == 1)
struct from_blob<T>
{
    using char_t = typename T::value_type;
    T operator()(blob_result const& val) const
    {
        using namespace sonia;
        if (val.type == blob_type::string) {
            char_t const* begin_ptr = val.inplace_size ? reinterpret_cast<char_t const*>(val.ui8array) : reinterpret_cast<char_t const*>(val.bp.data);
            size_t sz = val.inplace_size ? (size_t)val.inplace_size : (size_t)val.bp.size;
            return T{ begin_ptr, sz };
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to %2%"_fmt % val % typeid(T).name());
    }
};
/*
template <typename CharT, typename TraitsT>
struct from_blob<std::basic_string_view<CharT, TraitsT>>
{
    using view_t = std::basic_string_view<CharT, TraitsT>;
    view_t operator()(blob_result const& val) const
    {
        using namespace sonia;
        if (!val.is_array && val.type == blob_type::string) {
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
        if (!val.is_array && val.type == blob_type::string) {
            return str_t{ reinterpret_cast<const CharT*>(val.data), (size_t)val.size };
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::basic_string"_fmt % val);
    }
};
*/

template <typename ... Ts>
struct from_blob<std::tuple<Ts...>>
{
    using tuple_t = std::tuple<Ts...>;
    tuple_t operator()(blob_result const& val) const
    {
        using namespace sonia;
        if (val.type == blob_type::tuple) {
            return this->operator()(std::make_index_sequence<sizeof ...(Ts)>{}, reinterpret_cast<blob_result const*>(val.bp.data));
        } // else { to do: handle arrays }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::tuple"_fmt % val);
    }

    template <size_t ... Is>
    tuple_t operator()(std::index_sequence<Is...>, blob_result const* data) const
    {
        return { from_blob<std::tuple_element_t<Is, tuple_t>>()(data[Is]) ... };
    }
};

template <typename T, size_t SzV>
requires(SzV > 1)
struct from_blob<std::array<T, SzV>>
{
    std::array<T, SzV> operator()(blob_result const& val) const
    {
        using namespace sonia;
        if (is_array(val)) {
            return blob_type_selector(val, [this](auto idt, blob_result const& val)->std::array<T, SzV> {
                return this->selector(idt, val);
            });
        }
        THROW_INTERNAL_ERROR("can't convert blob %1% to std::array"_fmt % val);
    }

private:
    template <typename BT>
    inline std::array<T, SzV> selector(sonia::identity<BT>, blob_result const& val) const
    {
        using namespace sonia;
        T const* begin_ptr = val.inplace_size ? reinterpret_cast<T const*>(val.ui8array) : reinterpret_cast<T const*>(val.bp.data);
        size_t sz = val.inplace_size ? (size_t)val.inplace_size : (size_t)val.bp.size;
        if constexpr (is_same_v<T, BT> && (is_integral_v<T> || is_floating_point_v<T>)) {
            BOOST_ASSERT(sz / sizeof(T) >= SzV);
            return direct_decode(std::make_index_sequence<SzV>{}, reinterpret_cast<T const*>(begin_ptr));
        } else if constexpr (is_same_v<blob_result, BT>) {
            BOOST_ASSERT(sz / sizeof(blob_result) >= SzV);
            return decode(std::make_index_sequence<SzV>{}, reinterpret_cast<blob_result const*>(begin_ptr));
        } else {
            THROW_INTERNAL_ERROR("can't convert blob %1% to std::array<%2%>"_fmt % val % typeid(T).name());
        }
    }

    template <size_t ... Is>
    inline std::array<T, SzV> direct_decode(std::index_sequence<Is...>, T const* data) const
    {
        return { data[Is] ... };
    }

    template <size_t ... Is>
    inline std::array<T, SzV> decode(std::index_sequence<Is...>, blob_result const* data) const
    {
        return { from_blob<T>{}(data[Is]) ... };
    }
};

template <typename T>
inline auto as(blob_result const& val) -> decltype(from_blob<T>{}(std::declval<blob_result>()))
{
    return from_blob<T>{}(val);
}



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
    return blob_type_selector(lhs, [&rhs](auto id, blob_result const& lhs) { return blob_result_equal(id, lhs, rhs); });
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
    blob_result * operator->() { return this; }
    blob_result const* operator->() const { return this; }

    inline bool is_nil() const noexcept { return type == blob_type::nil; }
    inline bool is_array() const noexcept { return (((uint8_t)type) & 0x80) != 0; }

    inline const void* data() const noexcept
    {
        return inplace_size ? reinterpret_cast<const void*>(ui8array) : bp.data;
    }

    inline size_t size() const noexcept
    {
        return inplace_size ? (size_t)inplace_size : (size_t)bp.size;
    }

    inline void set_size(size_t sz)
    {
        if (inplace_size) {
            BOOST_ASSERT(sz <= inplace_size);
            inplace_size = (uint8_t)sz;
        } else {
            BOOST_ASSERT(bp.size <= inplace_size);
            bp.size = (uint32_t)sz;
        }
    }

    template <typename T>
    auto as() const { return ::as<T>(get()); }

    smart_blob& allocate()
    {
        blob_result_allocate(this);
        return *this;
    }

    blob_result detach()
    {
        blob_result tmp = nil_blob_result();
        std::swap(static_cast<blob_result&>(*this), tmp);
        return tmp;
    }
};

}
