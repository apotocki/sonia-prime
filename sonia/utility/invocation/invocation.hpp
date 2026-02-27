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
#include "sonia/span.hpp"
#include "sonia/utility/variadic.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/utility/functional/hash.hpp"
#include "numetron/float16.hpp"
#include "numetron/integer_view.hpp"
#include "numetron/basic_integer.hpp"
#include "numetron/basic_decimal.hpp"
#include "sonia/prime_config.hpp"

// X XX XXXXX-type
// | |- subtype
// |- is array

//common:
// X XX XXXXX
//    |-------- plane: default =1(0x20), aux0 =0(0)
// |----------- is array
// 
// default plane:
// X XX XXX XX
//        |  |- size exp: size = 2 ^ xx: 1, 2, 4, 8 bytes
//        |---- type: character=0, unsigned integral=1, signed integral =2, floating point=3, object=4
//   |--------- plane: default =1(0x20)
// |----------- is array

// aux plane:
// X XX XXXXX
//          |-- type: nil=0, bool=1, bigint=2, decimal=3, blob/tuple=4, function=5, blob_reference=6, error=7
//    |-------- plane: aux0 =0(0x0)
// |----------- is array



#define DEFAULT_BTVAL(isarr, type, sze) 0x20 + isarr * 0x80 + type * 0x4 + sze
#define AUX0_BTVAL(isarr, type) isarr * 0x80 + type

enum class blob_type : uint8_t {
    nil = AUX0_BTVAL(0, 0),
    boolean = AUX0_BTVAL(0, 1),
    //boolvec = AUX0_BTVAL(1, 1),
    c8 = DEFAULT_BTVAL(0, 0, 0),
    ui8 = DEFAULT_BTVAL(0, 1, 0),
    i8 = DEFAULT_BTVAL(0, 2, 0),
    ui16 = DEFAULT_BTVAL(0, 1, 1),
    i16 = DEFAULT_BTVAL(0, 2, 1),
    ui32 = DEFAULT_BTVAL(0, 1, 2),
    i32 = DEFAULT_BTVAL(0, 2, 2),
    ui64 = DEFAULT_BTVAL(0, 1, 3),
    i64 = DEFAULT_BTVAL(0, 2, 3),
    flt16 = DEFAULT_BTVAL(0, 3, 1),
    flt32 = DEFAULT_BTVAL(0, 3, 2),
    flt64 = DEFAULT_BTVAL(0, 3, 3),
    
    bigint = AUX0_BTVAL(0, 2),
    decimal = AUX0_BTVAL(0, 3),
    object = DEFAULT_BTVAL(1, 4, 0),
    //objvec = DEFAULT_BTVAL(1, 4, 3),
    
    string = DEFAULT_BTVAL(1, 0, 0),
    //string16 = DEFAULT_BTVAL(1, 0, 1),
    //string32 = DEFAULT_BTVAL(1, 0, 2),
    
    blob = AUX0_BTVAL(0, 4),
    tuple = AUX0_BTVAL(1, 4),
    function = AUX0_BTVAL(1, 5),
    blob_reference = AUX0_BTVAL(1, 6),
    error = AUX0_BTVAL(1, 7)
};

inline bool is_ref(blob_type val) noexcept
{
    return val == blob_type::blob_reference; // || val == blob_type::object;
}

template <typename T> constexpr bool is_integral_not_bool_v = std::is_integral_v<T> && !std::is_same_v<T, bool>;
//#pragma pack(push)
//#pragma pack(1)

#pragma pack(push, 1)
struct alignas(8) blob_result
{
    union {
        struct {
            union {
                void const* payload_ptr;
                uint64_t ui64value;
                int64_t i64value;
                numetron::float16 f16value;
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
            union {
                uint16_t u16;
                struct {
                    uint8_t n0;
                    uint8_t n1;
                } u8;
            } reserved;
        } bp; // base_plane
        uint8_t ui8array[14];
    } data;
    uint8_t inplace_size : 4;
    uint8_t need_unpin : 1;
    uint8_t bp_reserved_used : 1;
    uint8_t reserved0 : 1;
    uint8_t reserved1 : 1;
    
    blob_type type;
};
#pragma pack(pop)

static_assert(sizeof(blob_result) == 16);

extern "C" {

SONIA_PRIME_API void blob_result_allocate(blob_result *, bool no_inplace = false);
SONIA_PRIME_API void blob_result_pin(blob_result *);
SONIA_PRIME_API void blob_result_unpin(blob_result *);

}

namespace sonia { class smart_blob; }

namespace sonia::invocation {

class object
{
public:
    virtual ~object() = default;
};

template <typename T>
class wrapper_object : public object
{
public:
    T value;

    template <typename ... ArgsT>
    explicit wrapper_object(ArgsT&& ... args)
        : value { std::forward<ArgsT>(args) ... }
    {}
};

template <class T> struct is_wrapper_object : false_type {};
template <class T> struct is_wrapper_object<wrapper_object<T>> : true_type {};
template <class T> constexpr int is_wrapper_object_v = is_wrapper_object<T>::value;

class invocable
{
public:
    virtual ~invocable() = default;

    virtual std::type_index get_type_index() const { return typeid(*this); }

    virtual bool has_method(string_view methodname) const;
    virtual bool try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept;
    virtual bool try_get_property(string_view propname, smart_blob& result) const;
    virtual bool try_set_property(string_view propname, blob_result const& val);

    virtual void on_property_change(string_view) {}

    virtual shared_ptr<invocable> self_as_invocable_shared() { return {}; }

    // method routine
    inline bool try_invoke(string_view methodname, span<const blob_result> args) noexcept;
    
    smart_blob invoke(string_view methodname, span<const blob_result> args) noexcept;
    inline smart_blob invoke(string_view name, std::initializer_list<const blob_result> args) noexcept;

    // properties routine
    smart_blob get_property(string_view propname) const;
    void set_property(string_view propname, blob_result const& val);
    void set_property(string_view propname, blob_result&& val);
};

class callable
{
public:
    virtual ~callable() = default;

    virtual smart_blob invoke(span<const blob_result> args) = 0;
    inline smart_blob invoke(std::initializer_list<const blob_result> args);
};

struct error
{
    string_view what;
};

inline size_t hash_value(error const& val) noexcept { return hasher{}(val.what); }

}

inline blob_type arrayify(blob_type l) noexcept
{
    return (blob_type)((uint8_t)l | (uint8_t)0x80);
}

inline auto operator <=> (blob_type l, blob_type r) noexcept
{
    return ((uint8_t)l) <=> ((uint8_t)r);
}


inline bool is_basic_integral(blob_type val) noexcept
{
    switch(val) {
    case blob_type::i8:
    case blob_type::ui8:
    case blob_type::i16:
    case blob_type::ui16:
    case blob_type::i32:
    case blob_type::ui32:
    case blob_type::i64:
    case blob_type::ui64:
        return true;
    default:
        return false;
    }
}

inline bool is_integral(blob_type val) noexcept
{
    switch (val) {
    case blob_type::i8:
    case blob_type::ui8:
    case blob_type::i16:
    case blob_type::ui16:
    case blob_type::i32:
    case blob_type::ui32:
    case blob_type::i64:
    case blob_type::ui64:
    case blob_type::bigint:
        return true;
    default:
        return false;
    }
}

inline bool is_floating_point(blob_type val) noexcept
{
    switch (val) {
    case blob_type::flt16:
    case blob_type::flt32:
    case blob_type::flt64:
    case blob_type::decimal:
        return true;
    default:
        return false;
    }
}

inline bool is_numeric(blob_type val) noexcept
{
    switch (val) {
    case blob_type::i8:
    case blob_type::ui8:
    case blob_type::i16:
    case blob_type::ui16:
    case blob_type::i32:
    case blob_type::ui32:
    case blob_type::i64:
    case blob_type::ui64:
    case blob_type::flt16:
    case blob_type::flt32:
    case blob_type::flt64:
    case blob_type::bigint:
    case blob_type::decimal:
        return true;
    default:
        return false;
    }
}

inline void reset(blob_result& br) { br.type = blob_type::nil; br.need_unpin = 0; }

template <typename T>
inline const T* data_of(blob_result const& val) noexcept
{
    return val.inplace_size ? reinterpret_cast<T const*>(val.data.ui8array) : reinterpret_cast<T const*>(val.data.bp.payload_ptr);
}

template <typename T>
inline T* mutable_data_of(blob_result const& val) noexcept
{
    return const_cast<T*>(data_of<T>(val));
}

inline blob_result const& unref(blob_result const& val)
{
    blob_result const* pval = &val;
    while (is_ref(pval->type)) {
        pval = data_of<blob_result>(*pval);
    }
    return *pval;
}

inline bool is_nil(blob_result const& val) noexcept
{
    return val.type == blob_type::nil;
}

inline bool is_array(blob_result const& val) noexcept
{
    return (((uint8_t)val.type) & 0x80) != 0;
}

inline bool contains_string(blob_result const& val) noexcept
{
    blob_type vtype = val.type;
    return vtype == blob_type::string || vtype == blob_type::error || vtype == blob_type::function;
}

template <typename T>
inline size_t array_size_of(blob_result const& val) noexcept(!sonia_is_debug)
{
    size_t raw_sz = (val.inplace_size ? (size_t)val.inplace_size : (size_t)val.data.bp.size);
    BOOST_ASSERT(0 == raw_sz % sizeof(T));
    return raw_sz / sizeof(T);
}

using invocation_bigint_limb_type = uint64_t;
//constexpr size_t invocation_bigint_sign_mask = uint16_t{ 0x8000 };
//constexpr size_t invocation_bigint_skip_bits_mask = uint16_t{ 0x3f };

template <std::unsigned_integral LimbT>
auto make_basic_integer_view(blob_result const& val)
{
    // val.reserved0 - sign
    // val.data.bp.reserved8.n0 - skip bits
    return numetron::basic_integer_view<LimbT>{ std::span{ data_of<const LimbT>(val), array_size_of<LimbT>(val) }, val.reserved0 ? -1 : 1, val.data.bp.reserved.u8.n0 };
}

#include "sonia/type_traits.hpp"
#include "sonia/string.hpp"

template <typename T>
inline consteval blob_type blob_type_for()
{
    using type = std::remove_cv_t<T>;
    if constexpr (std::is_same_v<std::nullptr_t, type>) {
        return blob_type::nil;
    } else if constexpr (std::is_same_v<bool, type>) {
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
    } else if constexpr (std::is_same_v<type, numetron::float16> || (std::is_floating_point_v<type> && sizeof(type) == 2)) {
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
    return blob_result {
            .data = { .bp = { .payload_ptr = data, .size = size, .reserved = { 0 } } },
        .inplace_size = 0,
        .need_unpin = 0,
        .bp_reserved_used = 0,
        .reserved0 = 0,
        .reserved1 = 0,
        .type = bt };
}

inline blob_result nil_blob_result()
{
    return make_blob_result(blob_type::nil);
}

inline blob_result function_blob_result(sonia::string_view value)
{
    blob_result result = make_blob_result(blob_type::function, value.data());
    result.data.bp.size = static_cast<uint32_t>(value.size());
    return result;
}

template <std::derived_from<sonia::invocation::object> ObjectT, typename ... ArgsT>
inline blob_result object_blob_result(ArgsT&& ... args)
{
    blob_result result = make_blob_result(blob_type::object, nullptr, sizeof(ObjectT));
    blob_result_allocate(&result, true);
    ObjectT* place = mutable_data_of<ObjectT>(result);
    try {
        new (place) ObjectT(std::forward<ArgsT>(args)...);
    } catch (...) {
        result.type = blob_type::string;
        blob_result_unpin(&result);
        throw;
    }
    return result;
}

template <typename T>
inline blob_result object_blob_result(sonia::shared_ptr<T> object)
{
    return object_blob_result<sonia::invocation::wrapper_object<sonia::shared_ptr<T>>>(std::move(object));
}

template <typename T>
inline blob_result object_blob_result(sonia::weak_ptr<T> object)
{
    return object_blob_result<sonia::invocation::wrapper_object<sonia::weak_ptr<T>>>(std::move(object));
}

inline blob_result bool_blob_result(bool value)
{
    blob_result result = make_blob_result(blob_type::boolean);
    result.data.bp.ui8value = value ? 1 : 0;
    return result;
}

inline blob_result ui8_blob_result(uint8_t value)
{
    blob_result result = make_blob_result(blob_type::ui8);
    result.data.bp.ui8value = value;
    return result;
}

inline blob_result i8_blob_result(int8_t value)
{
    blob_result result = make_blob_result(blob_type::i8);
    result.data.bp.i8value = value;
    return result;
}

inline blob_result char_blob_result(char value)
{
    blob_result result = make_blob_result(blob_type::c8);
    result.data.bp.i8value = value;
    return result;
}

inline blob_result ui16_blob_result(uint16_t value)
{
    blob_result result = make_blob_result(blob_type::ui16);
    result.data.bp.ui16value = value;
    return result;
}

inline blob_result i16_blob_result(int16_t value)
{
    blob_result result = make_blob_result(blob_type::i16);
    result.data.bp.i16value = value;
    return result;
}

inline blob_result ui32_blob_result(uint32_t value)
{
    blob_result result = make_blob_result(blob_type::ui32);
    result.data.bp.ui32value = value;
    return result;
}

inline blob_result i32_blob_result(int32_t value)
{
    blob_result result = make_blob_result(blob_type::i32);
    result.data.bp.i32value = value;
    return result;
}

inline blob_result ui64_blob_result(uint64_t value)
{
    blob_result result = make_blob_result(blob_type::ui64);
    result.data.bp.ui64value = value;
    return result;
}

inline blob_result i64_blob_result(int64_t value)
{
    blob_result result = make_blob_result(blob_type::i64);
    result.data.bp.i64value = value;
    return result;
}

inline blob_result f16_blob_result(numetron::float16 value)
{
    blob_result result = make_blob_result(blob_type::flt16);
    result.data.bp.f16value = value;
    return result;
}

inline blob_result f32_blob_result(float_t value)
{
    blob_result result = make_blob_result(blob_type::flt32);
    result.data.bp.f32value = value;
    return result;
}

inline blob_result f64_blob_result(double_t value)
{
    blob_result result = make_blob_result(blob_type::flt64);
    result.data.bp.f64value = value;
    return result;
}

template <typename CharT, size_t N>
requires (sizeof(CharT) == 1)
inline blob_result string_blob_result(const CharT(&value)[N], blob_type t = blob_type::string)
{
    assert(!value[N - 1]);
    blob_result result = make_blob_result(t, value);
    result.data.bp.size = static_cast<uint32_t>(N - 1);
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
    return make_blob_result(t, value.get_pointer(), static_cast<uint32_t>(value.size()));
}

inline blob_result string_blob_result(const char* value, blob_type t = blob_type::string)
{
    return string_blob_result(std::string_view{value}, t);
}

template <typename ArgT>
inline blob_result error_blob_result(ArgT && arg)
{
    return string_blob_result(std::forward<ArgT>(arg), blob_type::error);
}

template <std::unsigned_integral LimbT>
inline blob_result bigint_blob_result(numetron::basic_integer_view<LimbT> bival) noexcept
{
    using namespace sonia;
    blob_result result;
    result.type = blob_type::bigint;
    result.reserved0 = bival.sgn() < 0 ? 1 : 0;
    result.need_unpin = 0;
    if (bival = bival.abs(); bival.template is_fit<uint64_t>()) {
        result.data.bp.ui64value = (uint64_t)bival;
        result.data.bp.size = 0;
        result.inplace_size = 8;
    } else {
        result.data.bp.payload_ptr = bival.data();
        result.data.bp.size = static_cast<uint32_t>(bival.size() * sizeof(LimbT));
        result.data.bp.reserved.u8.n0 = static_cast<uint8_t>(bival.most_significant_skipping_bits());
        result.data.bp.reserved.u8.n1 = static_cast<uint8_t>(sizeof(LimbT));
        result.bp_reserved_used = 1;
        result.inplace_size = 0;
    }
    return result;
}

template <std::unsigned_integral LimbT, size_t N, typename AllocatorT>
inline blob_result bigint_blob_result(numetron::basic_integer<LimbT, N, AllocatorT> const& bival) noexcept
{
    return bigint_blob_result((numetron::basic_integer_view<LimbT>)bival);
}

//inline blob_result decimal_blob_result(numetron::basic_integer_view<invocation_bigint_limb_type> mval, int16_t eval)
//{
//    using namespace sonia;
//    auto sp = (std::span<const invocation_bigint_limb_type>)mval;
//    blob_result result = make_blob_result(blob_type::decimal, sp.data(), static_cast<uint32_t>(sp.size() * sizeof(invocation_bigint_limb_type)));
//    result.data.bp.reserved = static_cast<uint16_t>(eval);
//    result.reserved = mval.sgn() < 0 ? 1 : 0;
//    return result;
//}

template <std::unsigned_integral LimbT>
inline blob_result decimal_blob_result(numetron::basic_decimal_view<LimbT> const& dv)
{
    blob_result result;
    result.type = blob_type::decimal;
    result.reserved0 = dv.sgn() < 0 ? 1 : 0;
    if (dv.exponent().template is_fit<int16_t>()) {
        numetron::basic_integer_view<LimbT> sval = dv.significand().abs();
        if (sval.template is_fit<uint64_t>()) {
            int64_t eval = (int16_t)dv.exponent();
            result.data.bp.reserved.u16 = static_cast<uint16_t>(eval);
            result.data.bp.ui64value = (uint64_t)sval;
            result.data.bp.size = 0;
            result.need_unpin = 0;
            result.inplace_size = 8;
            return result;
        }
    }
    THROW_NOT_IMPLEMENTED_ERROR("decimal_blob_result");
}

template <std::unsigned_integral LimbT, size_t N, size_t E, typename AllocatorT>
inline blob_result decimal_blob_result(numetron::basic_decimal<LimbT, N, E, AllocatorT> const& dv)
{
    return decimal_blob_result((numetron::basic_decimal_view<LimbT>)dv);
}

template <typename T>
inline blob_result optional_blob_result(sonia::optional<T> const& value)
{
    if (!value) return nil_blob_result();
    return particular_blob_result(*value);
}

template <typename T, size_t EV>
requires std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<numetron::float16, std::remove_cv_t<T>> || std::is_same_v<blob_result, std::remove_cv_t<T>>
inline blob_result array_blob_result(std::span<T, EV> arr)
{
    return make_blob_result(arrayify(blob_type_for<T>()), arr.data(), static_cast<uint32_t>(arr.size() * sizeof(T)));
}

template <typename T, size_t N>
requires std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<numetron::float16, std::remove_cv_t<T>> || std::is_same_v<blob_result, std::remove_cv_t<T>>
inline blob_result array_blob_result(T(&arr)[N])
{
    return make_blob_result(arrayify(blob_type_for<T>()), arr, static_cast<uint32_t>(N * sizeof(T)));
}

[[nodiscard]]
inline blob_result reference_blob_result(blob_result const& br)
{
    blob_result res = make_blob_result(blob_type::blob_reference, &br, static_cast<uint32_t>(sizeof(blob_result)));
    blob_result_allocate(&res, true);
    return res;
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
    if constexpr (std::is_same_v<T, std::nullptr_t>) return nil_blob_result();
    else if constexpr (std::is_same_v<T, bool>) return bool_blob_result(value);
    else if constexpr (std::is_same_v<T, uint8_t>) return ui8_blob_result(value);
    else if constexpr (std::is_same_v<T, int8_t>) return i8_blob_result(value);
    else if constexpr (std::is_same_v<T, char>) return char_blob_result(value);
    else if constexpr (std::is_same_v<T, uint16_t>) return ui16_blob_result(value);
    else if constexpr (std::is_same_v<T, int16_t>) return i16_blob_result(value);
    else if constexpr (std::is_same_v<T, uint32_t> || (std::is_integral_v<T> && !std::is_signed_v<T> && sizeof(T) == 4)) return ui32_blob_result(value);
    else if constexpr (std::is_same_v<T, int32_t> || (std::is_integral_v<T> && std::is_signed_v<T> && sizeof(T) == 4)) return i32_blob_result(value);
    else if constexpr (std::is_same_v<T, uint64_t> || (std::is_integral_v<T> && !std::is_signed_v<T> && sizeof(T) == 8)) return ui64_blob_result(value);
    else if constexpr (std::is_same_v<T, int64_t> || (std::is_integral_v<T> && std::is_signed_v<T> && sizeof(T) == 8)) return i64_blob_result(value);
    else if constexpr (std::is_same_v<T, numetron::float16>) return f16_blob_result(value);
    else if constexpr (numetron::is_basic_integer_view_v<T>) return bigint_blob_result(value);
    else if constexpr (numetron::is_basic_decimal_view_v<T>) return decimal_blob_result(value);
    else if constexpr (std::is_same_v<T, float_t>) return f32_blob_result(value);
    else if constexpr (std::is_same_v<T, double_t>) return f64_blob_result(value);
    else if constexpr (sonia::is_string_v<T>) return string_blob_result(std::forward<ArgT>(value));
    else if constexpr (std::is_same_v<T, blob_result>) return value;
    else if constexpr (sonia::is_template_instance_v<sonia::optional, T>) return optional_blob_result(std::forward<ArgT>(value));
    else if constexpr (sonia::is_template_instance_v<std::pair, T>) return pair_blob_result(std::forward<ArgT>(value));
    else if constexpr (sonia::is_template_instance_v<std::tuple, T>) return tuple_blob_result(std::forward<ArgT>(value));
    else if constexpr (sonia::is_template_instance_v<sonia::shared_ptr, T>) return object_blob_result(std::forward<ArgT>(value));
    else {
        static_assert(sonia::dependent_false<T>);
        return nil_blob_result();
    }
}



template <typename FT>
inline auto blob_bigint_dispatch(blob_result const& val, FT&& ftor)
{
    using namespace std::string_view_literals;
    using numetron::basic_integer_view;
    if (val.inplace_size) {
        return ftor(basic_integer_view<invocation_bigint_limb_type>{ val.data.bp.ui64value, val.reserved0 ? -1 : 1 });
    } else {
        //int sign = val.reserved0 ? -1 : 1;
        switch(val.data.bp.reserved.u8.n1) {
        case 8:
            return ftor(make_basic_integer_view<uint8_t>(val));
        case 16:
            return ftor(make_basic_integer_view<uint16_t>(val));
        case 32:
            return ftor(make_basic_integer_view<uint32_t>(val));
        case 64:
            return ftor(make_basic_integer_view<uint64_t>(val));
        default:
            throw std::runtime_error((std::ostringstream() << "unsupported limb bit size: "sv << val.data.bp.reserved.u8.n1).str());
        }
    }
}

template <typename FT>
inline auto blob_decimal_dispatch(blob_result const& val, FT&& ftor)
{
    using namespace std::string_view_literals;
    using numetron::basic_integer_view;
    using numetron::basic_decimal_view;
    if (val.inplace_size) {
        return ftor(basic_decimal_view<invocation_bigint_limb_type>{
              basic_integer_view<invocation_bigint_limb_type>{ val.data.bp.ui64value, val.reserved0 ? -1 : 1 }
            , static_cast<int16_t>(val.data.bp.reserved.u16)
        });
    } else {
        THROW_NOT_IMPLEMENTED_ERROR("basic_decimal_view");
    }
}

template <typename FT>
inline auto blob_type_dispatch(blob_result const& b, FT&& ftor)
{
    using namespace std::string_view_literals;

    switch (b.type) {
    case blob_type::nil:
        return ftor(nullptr);
    case blob_type::blob_reference:
        return blob_type_dispatch(*data_of<blob_result>(b), std::forward<FT>(ftor));
    case blob_type::boolean:
        return ftor(!!b.data.bp.ui8value);
    case blob_type::i8:
        return ftor(b.data.bp.i8value);
    case blob_type::ui8:
        return ftor(b.data.bp.ui8value);
    case blob_type::c8:
        return ftor((char)b.data.bp.i8value);
    case blob_type::i16:
        return ftor(b.data.bp.i16value);
    case blob_type::ui16:
        return ftor(b.data.bp.ui16value);
    case blob_type::i32:
        return ftor(b.data.bp.i32value);
    case blob_type::ui32:
        return ftor(b.data.bp.ui32value);
    case blob_type::i64:
        return ftor(b.data.bp.i64value);
    case blob_type::ui64:
        return ftor(b.data.bp.ui64value);
    case blob_type::flt16:
        return ftor(b.data.bp.f16value);
    case blob_type::flt32:
        return ftor(b.data.bp.f32value);
    case blob_type::flt64:
        return ftor(b.data.bp.f64value);
    case blob_type::string:
        return ftor(b.inplace_size ?
              std::string_view{ reinterpret_cast<char const*>(b.data.ui8array), static_cast<size_t>(b.inplace_size) }
            : std::string_view{ reinterpret_cast<char const*>(b.data.bp.payload_ptr), static_cast<size_t>(b.data.bp.size) }
        );
    case blob_type::error:
        return ftor(sonia::invocation::error{
            b.inplace_size ?
                  std::string_view{ reinterpret_cast<char const*>(b.data.ui8array), static_cast<size_t>(b.inplace_size) }
                : std::string_view{ reinterpret_cast<char const*>(b.data.bp.payload_ptr), static_cast<size_t>(b.data.bp.size) }
        });
    case blob_type::tuple:
        return ftor(std::span<const blob_result>{ data_of<blob_result>(b), array_size_of<blob_result>(b) });
    case blob_type::bigint:
        return blob_bigint_dispatch(b, std::forward<FT>(ftor));
    case blob_type::decimal:
        return blob_decimal_dispatch(b, std::forward<FT>(ftor));
    default:
        break;
    }
    throw std::runtime_error((std::ostringstream() << "unexpected blob type: "sv << std::hex << (int)b.type).str());
}



template <typename FT>
auto blob_type_selector(blob_result const& b, FT&& ftor)
{
    switch (b.type) {
    case blob_type::nil:
        return ftor(std::type_identity<std::nullptr_t>{}, b);
    case blob_type::blob_reference:
        return ftor(std::type_identity<blob_result>{}, b);
    case blob_type::tuple:
        return ftor(std::type_identity<blob_result>{}, b);
    case blob_type::bigint:
        return ftor(std::type_identity<numetron::basic_integer_view<invocation_bigint_limb_type>>{}, b);
    case blob_type::decimal:
        return ftor(std::type_identity<numetron::basic_decimal_view<invocation_bigint_limb_type>>{}, b);
    case blob_type::string:
    case blob_type::error:
        return ftor(std::type_identity<std::string_view>{}, b);
        default:
            break;
    }
    blob_type decayed_type = (blob_type)(((uint8_t)b.type) & 0x7f);
    switch (decayed_type)
    {
    case blob_type::boolean:
        return ftor(std::type_identity<bool>{}, b);
    case blob_type::i8:
        return ftor(std::type_identity<int8_t>{}, b);
    case blob_type::ui8:
        return ftor(std::type_identity<uint8_t>{}, b);
    case blob_type::c8:
        return ftor(std::type_identity<char>{}, b);
    case blob_type::i16:
        return ftor(std::type_identity<int16_t>{}, b);
    case blob_type::ui16:
        return ftor(std::type_identity<uint16_t>{}, b);
    case blob_type::i32:
        return ftor(std::type_identity<int32_t>{}, b);
    case blob_type::ui32:
        return ftor(std::type_identity<uint32_t>{}, b);
    case blob_type::i64:
        return ftor(std::type_identity<int64_t>{}, b);
    case blob_type::ui64:
        return ftor(std::type_identity<uint64_t>{}, b);
    case blob_type::flt16:
        return ftor(std::type_identity<numetron::float16>{}, b);
    case blob_type::flt32:
        return ftor(std::type_identity<float_t>{}, b);
    case blob_type::flt64:
        return ftor(std::type_identity<double_t>{}, b);
    case blob_type::tuple:
        return ftor(std::type_identity<blob_result>{}, b);
    default:
        break;
    }
    return ftor(std::type_identity<void>{}, b);
}

template <typename Elem, typename Traits>
inline std::basic_ostream<Elem, Traits>& print_type(std::basic_ostream<Elem, Traits>& os, blob_result const& b)
{
    using namespace std::string_view_literals;
    if (b.type == blob_type::nil) {
        return os << "nil"sv;
    } else if (b.type == blob_type::string) {
        return os << "string"sv;
    } else if (b.type == blob_type::bigint) {
        return os << "bigint"sv;
    } else if (b.type == blob_type::decimal) {
        return os << "decimal"sv;
    } else if (b.type == blob_type::error) {
        return os << "error"sv;
    } else if (b.type == blob_type::object) {
        return os << "object"sv;
    } else if (b.type == blob_type::tuple) {
        return os << "tuple"sv;
    } else if (b.type == blob_type::blob_reference) {
        return print_type(os << '&', *data_of<blob_result>(b));
    }
    if (is_array(b)) {
        os << "[";
    }
    blob_type decayed_type = (blob_type)(((uint8_t)b.type) & 0x7f);
    switch (decayed_type)
    {
    case blob_type::boolean:
        os << "bool"sv; break;
    case blob_type::i8:
        os << "i8"sv; break;
    case blob_type::ui8:
        os << "ui8"sv; break;
    case blob_type::i16:
        os << "i16"sv; break;
    case blob_type::ui16:
        os << "ui16"sv; break;
    case blob_type::i32:
        os << "i32"sv; break;
    case blob_type::ui32:
        os << "ui32"sv; break;
    case blob_type::i64:
        os << "i64"sv; break;
    case blob_type::ui64:
        os << "ui64"sv; break;
    case blob_type::flt16:
        os << "f16"sv; break;
    case blob_type::flt32:
        os << "f32"sv; break;
    case blob_type::flt64:
        os << "f64"sv; break;
    case blob_type::blob:
        os << "blob"sv; break;
    case blob_type::function:
        os << "function"sv; break;
    case blob_type::object:
        os << "object"sv; break;
    default:
        os << "unknown"sv;
    }
    if (is_array(b)) {
        os << ']';
    }
    return os;
}

inline size_t hash_value(blob_result const& val) noexcept
{
    return blob_type_dispatch(val, []<typename DT>(DT v)->size_t { return sonia::hash<DT>{}(v); });
}

template <typename T>
struct from_blob
{
    T operator()(blob_result val) const {
        using namespace sonia;
        static_assert(dependent_false<T>);
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to type `%2%`"_fmt % val % typeid(T).name());
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

template <std::derived_from<sonia::invocation::object> T>
struct from_blob<T>
{
    T& operator()(blob_result const& val) const
    {
        using namespace sonia;
        if (val.type == blob_type::object) {
            if (T* result = dynamic_cast<T*>(mutable_data_of<sonia::invocation::object>(val)); result) {
                return *result;
            }
        }
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to object `%2%`"_fmt % val % typeid(T).name());
    }
};

template <>
struct from_blob<bool>
{
    bool operator()(blob_result val) const
    {
        using namespace sonia;
        if (val.type == blob_type::boolean) {
            return !!val.data.bp.i8value;
        }
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to bool"_fmt % val);
    }
};

template <>
struct from_blob<blob_result>
{
    inline blob_result operator()(blob_result val) const noexcept { return val; }
};

template <>
struct from_blob<sonia::smart_blob>
{
    sonia::smart_blob operator()(blob_result const& val) const noexcept;
};

template <>
struct from_blob<numetron::float16>
{
    numetron::float16 operator()(blob_result val) const
    {
        return val.data.bp.f16value;
    }
};

template <std::integral T>
struct from_blob<T>
{
    inline T operator()(blob_result const& val) const
    {
        using namespace sonia;

        return blob_type_dispatch(val, [&val]<typename DT>(DT ival)->T {
            if constexpr (is_integral_not_bool_v<DT>) { return (T)ival; } // to do: checked cast?
            else if constexpr (numetron::is_basic_integer_view_v<DT>) {
                if (ival.template is_fit<T>()) return (T)ival;
            }

            THROW_INTERNAL_ERROR("can't convert blob `%1%` to `%2%`"_fmt % val % typeid(T).name());
        });
    }
};

template <std::floating_point T>
struct from_blob<T>
{
    inline T operator()(blob_result const& val) const
    {
        using namespace sonia;
        
        return blob_type_dispatch(val, [&val]<typename DT>(DT ival) ->T {
            if constexpr (is_integral_not_bool_v<DT> || std::is_floating_point_v<DT> ||
                numetron::is_basic_integer_view_v<DT> || numetron::is_basic_decimal_view_v<DT>)
            {
                return (T)ival;
            } else if constexpr (std::is_same_v<DT, numetron::float16>) {
                return (T)(float)ival;
            } else {
                THROW_INTERNAL_ERROR("can't convert blob `%1%` to `%2%`"_fmt % val % typeid(T).name());
            }
        });
    }
};

template <std::unsigned_integral LimbT, size_t N, typename AllocatorT>
struct from_blob<numetron::basic_integer<LimbT, N, AllocatorT>>
{
    using bigint_t = numetron::basic_integer<LimbT, N, AllocatorT>;
    inline bigint_t operator()(blob_result const& val, AllocatorT const& alloc = AllocatorT{}) const
    {
        using namespace sonia;
        return blob_type_dispatch(val, [&val, &alloc]<typename DT>(DT ival)->bigint_t {
            if constexpr (is_integral_not_bool_v<DT> || numetron::is_basic_integer_view_v<DT>) {
                return bigint_t{ ival, alloc };
            }
            THROW_INTERNAL_ERROR("can't convert blob `%1%` to basic_integer<%2%>"_fmt % val % typeid(LimbT).name());
        });
    }
};

template <std::unsigned_integral LimbT>
struct from_blob<numetron::basic_integer_view<LimbT>>
{
    using bigint_view_t = numetron::basic_integer_view<LimbT>;
    inline bigint_view_t operator()(blob_result const& val) const
    {
        using namespace sonia;
        return blob_type_dispatch(val, [&val]<typename DT>(DT ival)->bigint_view_t {
            if constexpr (std::is_same_v<bool, DT>) {
                return bigint_view_t{ ival ? 1 : 0 };
            } else if constexpr (is_integral_not_bool_v<DT>) {
                return bigint_view_t{ ival };
            } else if constexpr (std::is_same_v<bigint_view_t, DT>) {
                return ival;
            }
            THROW_INTERNAL_ERROR("can't convert blob `%1%` to basic_integer_view<%2%>"_fmt % val % typeid(LimbT).name());
        });
    }
};

template <std::unsigned_integral LimbT, size_t N, size_t E, typename AllocatorT>
struct from_blob<numetron::basic_decimal<LimbT, N, E, AllocatorT>>
{
    using decimal_t = numetron::basic_decimal<LimbT, N, E, AllocatorT>;
    using decimal_view_t = numetron::basic_decimal_view<LimbT>;
    using bigint_view_t = numetron::basic_integer_view<LimbT>;
    inline decimal_t operator()(blob_result const& val, AllocatorT const& alloc = AllocatorT{}) const
    {
        using namespace sonia;
        return blob_type_dispatch(val, [&val, &alloc]<typename DT>(DT dval)->decimal_t {
            if constexpr (is_integral_not_bool_v<DT>) { // || std::is_floating_point_v<DT>  flt16
                return decimal_t{ dval, alloc };
            }
            else if constexpr (std::is_same_v<bigint_view_t, DT>) {
                return decimal_t{ numetron::basic_integer_view<LimbT>{ dval }, numetron::basic_integer_view<LimbT>{}, alloc };
            } else if constexpr (numetron::is_basic_integer_view_v<DT>) {
                return decimal_t{ numetron::basic_integer<LimbT, 1, AllocatorT>{ dval, alloc }, numetron::basic_integer_view<LimbT>{}, alloc };
            } else if constexpr (std::is_same_v<decimal_view_t, DT>) {
                return decimal_t{ dval, alloc };
            } else if constexpr (numetron::is_basic_decimal_view_v<DT>) {
                return decimal_t{ numetron::basic_integer<LimbT, 1, AllocatorT>{ dval.significand(), alloc }, numetron::basic_integer<LimbT, 1, AllocatorT>{ dval.exponent(), alloc }, alloc };
            }
            THROW_INTERNAL_ERROR("can't convert blob `%1%` to basic_decimal<%2%>"_fmt % val % typeid(LimbT).name());
        });
    }
};

template <std::unsigned_integral LimbT>
struct from_blob<numetron::basic_decimal_view<LimbT>>
{
    using decimal_view_t = numetron::basic_decimal_view<LimbT>;
    using bigint_view_t = numetron::basic_integer_view<LimbT>;
    inline decimal_view_t operator()(blob_result const& val) const
    {
        using namespace sonia;
        return blob_type_dispatch(val, [&val]<typename DT>(DT dval)->decimal_view_t {
            if constexpr (is_integral_not_bool_v<DT>) {
                return decimal_view_t{ dval };
            } else if constexpr (std::is_same_v<bigint_view_t, DT>) {
                return decimal_view_t{ numetron::basic_integer_view<LimbT>{dval}, numetron::basic_integer_view<LimbT>{} };
            } else if constexpr (std::is_same_v<decimal_view_t, DT>) {
                return dval;
            } else if constexpr (std::is_floating_point_v<DT> || std::is_same_v<numetron::float16, DT>) {
                return decimal_view_t{ dval };
            }
            THROW_INTERNAL_ERROR("can't convert blob `%1%` to basic_decimal_view"_fmt % val);
        });
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
            return span_t{ reinterpret_cast<CharT const*>(&val.data.bp.ui8value), (size_t)1 };
        }
#ifdef BOOST_WINDOWS
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to std::span<%2%>"_fmt % val % typeid(CharT).name());
#else
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to std::span<CharT>"_fmt % val);
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
                return span_t{ reinterpret_cast<T*>(&val.data.bp.payload_ptr), 1 };
            }
        }
#ifdef BOOST_WINDOWS
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to std::span<%2%>"_fmt % val % typeid(T).name());
#else
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to std::span<T>"_fmt % val);
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
        if (contains_string(val)) {
            char_t const* begin_ptr = val.inplace_size ? reinterpret_cast<char_t const*>(val.data.ui8array) : reinterpret_cast<char_t const*>(val.data.bp.payload_ptr);
            size_t sz = val.inplace_size ? (size_t)val.inplace_size : (size_t)val.data.bp.size;
            return T{ begin_ptr, sz };
        }
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to `%2%`"_fmt % val % typeid(T).name());
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
            return this->operator()(std::make_index_sequence<sizeof ...(Ts)>{}, reinterpret_cast<blob_result const*>(val.data.bp.payload_ptr));
        } // else { to do: handle arrays }
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to std::tuple"_fmt % val);
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
        THROW_INTERNAL_ERROR("can't convert blob `%1%` to std::array"_fmt % val);
    }

private:
    template <typename BT>
    inline std::array<T, SzV> selector(std::type_identity<BT>, blob_result const& val) const
    {
        using namespace sonia;
        void const* begin_ptr = val.inplace_size ? val.data.ui8array : val.data.bp.payload_ptr;
        size_t sz = val.inplace_size ? (size_t)val.inplace_size : (size_t)val.data.bp.size;
        if constexpr (is_same_v<T, BT> && (is_integral_v<T> || is_floating_point_v<T>)) {
            BOOST_ASSERT(sz / sizeof(T) >= SzV);
            return direct_decode(std::make_index_sequence<SzV>{}, reinterpret_cast<T const*>(begin_ptr));
        } else if constexpr (is_same_v<blob_result, BT>) {
            BOOST_ASSERT(sz / sizeof(blob_result) >= SzV);
            return decode(std::make_index_sequence<SzV>{}, reinterpret_cast<blob_result const*>(begin_ptr));
        } else {
            THROW_INTERNAL_ERROR("can't convert blob `%1%` to std::array<%2%>"_fmt % val % typeid(T).name());
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
    return from_blob<T>{}(unref(val));
}

template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& print_to_stream(std::basic_ostream<Elem, Traits>& os, blob_result const& b, bool with_types)
{
    using namespace std::string_view_literals;

    if (b.type == blob_type::nil) {
        return os << "nil"sv;
    }
    else if (b.type == blob_type::object) {
        sonia::invocation::object const* pobj = data_of<sonia::invocation::object>(b);
        if (auto *p = dynamic_cast<sonia::invocation::wrapper_object<sonia::shared_ptr<sonia::invocation::invocable>> const*>(pobj)) {
            if (sonia::shared_ptr<sonia::invocation::invocable> sptr = p->value) {
                sonia::invocation::invocable& object_ref = *sptr;
                return os << "object at 0x"sv << std::hex << std::uppercase << reinterpret_cast<std::uintptr_t>(sptr.get()) << ": "sv << typeid(object_ref).name();
            } else {
                return os << "object : null invocable pointer"sv;
            }
        } else {
            return os << "object at 0x"sv << std::hex << reinterpret_cast<std::uintptr_t>(pobj) << " : "sv << typeid(*pobj).name();
        }
    }
    else if (b.type == blob_type::blob_reference) {
        return os << '&' << *data_of<blob_result>(b);
    }
    if (is_array(b) && !contains_string(b)) {
        os << '[';
        blob_type_selector(b, [&os](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            if constexpr (std::is_same_v<type, std::nullptr_t>) { os << "nil"sv; }
            else if constexpr (std::is_void_v<type>) { os << "unknown"sv; }
            else if constexpr (std::is_same_v<type, numetron::basic_integer_view<invocation_bigint_limb_type>>) { os << "bigint"sv; }
            else {
                using fstype = std::conditional_t<std::is_same_v<type, bool>, uint8_t, type>;
                fstype const* begin_ptr = data_of<fstype>(b);
                for (auto* p = begin_ptr, *e = begin_ptr + array_size_of<fstype>(b); p != e; ++p) {
                    if (p != begin_ptr) os << ", "sv;
                    os << particular_blob_result((type)*p);
                }
            }
            });
        return os << ']';
    }
    switch (b.type)
    {
    case blob_type::nil:
        return os << "nil"sv;
    case blob_type::boolean:
        return os << (b.data.bp.i8value ? "true"sv : "false"sv);
    case blob_type::c8:
        return with_types ? (os << '\'' << (char)b.data.bp.i8value << '\'') : (os << (char)b.data.bp.i8value);
    case blob_type::i8:
        return os << (int)b.data.bp.i8value << (with_types ? ":i8"sv : ""sv);
    case blob_type::ui8:
        return os << (int)b.data.bp.ui8value << (with_types ? ":ui8"sv : ""sv);
    case blob_type::i16:
        return os << (int)b.data.bp.i16value << (with_types ? ":i16"sv : ""sv);
    case blob_type::ui16:
        return os << (int)b.data.bp.ui16value << (with_types ? ":ui16"sv : ""sv);
    case blob_type::i32:
        return os << b.data.bp.i32value << (with_types ? ":i32"sv : ""sv);
    case blob_type::ui32:
        return os << b.data.bp.ui32value << (with_types ? ":ui32"sv : ""sv);
    case blob_type::i64:
        return os << b.data.bp.i64value << (with_types ? ":i64"sv : ""sv);
    case blob_type::ui64:
        return os << b.data.bp.ui64value << (with_types ? ":ui64"sv : ""sv);
    case blob_type::flt16:
        return os << b.data.bp.f16value << (with_types ? ":f16"sv : ""sv);
    case blob_type::flt32:
        return os << b.data.bp.f32value << (with_types ? ":f32"sv : ""sv);
    case blob_type::flt64:
        return os << b.data.bp.f64value << (with_types ? ":f64"sv : ""sv);
    case blob_type::bigint:
        return os << as<numetron::basic_integer_view<invocation_bigint_limb_type>>(b) << (with_types ? ":bigint"sv : ""sv);
    case blob_type::decimal:
        return os << as<numetron::basic_decimal<invocation_bigint_limb_type, 1, 8>>(b) << (with_types ? ":decimal"sv : ""sv);
    case blob_type::string:
        if (with_types)
            return os << '"' << sonia::string_view{ data_of<char>(b), array_size_of<char>(b) } << '"';
        else return os << sonia::string_view{ data_of<char>(b), array_size_of<char>(b) };
    case blob_type::function:
        return os << "function"sv;
    case blob_type::object: {
        auto& obj = *data_of<sonia::invocation::object>(b);
        return os << "object : "sv << typeid(obj).name();
    }
    case blob_type::error:
        return os << "error: "sv << sonia::string_view{ data_of<char>(b), array_size_of<char>(b) };
    default:
        return os << "unknown"sv;
    }
}

template <typename Elem, typename Traits>
inline std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, blob_result const& b)
{
    return print_to_stream(os, b, true);
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

inline bool operator== (blob_result const& lhs, blob_result const& rhs) noexcept
{
    return blob_type_dispatch(unref(lhs), [&rhs = unref(rhs)]<typename DT>(DT v)->bool {
        if constexpr (std::is_same_v<nullptr_t, DT>) { return is_nil(rhs); }
        else if constexpr (std::is_same_v<bool, DT>) { return rhs.type == blob_type::boolean && v == !!rhs.data.bp.i8value; }
        else if constexpr (is_integral_not_bool_v<DT>) { return ::is_integral(rhs.type) && from_blob<numetron::basic_integer_view<invocation_bigint_limb_type>>{}(rhs) == v; }
        else if constexpr (std::is_floating_point_v<DT>) { return ::is_floating_point(rhs.type) && (double_t)v == from_blob<double_t>{}(rhs); } // to do: improve
        else if constexpr (std::is_same_v<numetron::float16, DT>) { return ::is_floating_point(rhs.type) && (float)v == from_blob<float>{}(rhs); } // to do: improve
        else if constexpr (numetron::is_basic_integer_view_v<DT>) { return ::is_integral(rhs.type) && from_blob<numetron::basic_integer_view<typename DT::limb_type>>{}(rhs) == v; }
        else if constexpr (numetron::is_basic_decimal_view_v<DT>) { return ::is_floating_point(rhs.type) && from_blob<numetron::basic_decimal_view<typename DT::limb_type>>{}(rhs) == v; }
        else if constexpr (std::is_same_v<std::string_view, DT>) { return rhs.type == blob_type::string && v == from_blob<std::string_view>{}(rhs); }
        else if constexpr (std::is_same_v<sonia::invocation::error, DT>) { return rhs.type == blob_type::error && v.what == from_blob<std::string_view>{}(rhs); }
        else if constexpr (std::is_same_v<std::span<const blob_result>, DT>) { return rhs.type == blob_type::tuple && sonia::range_equal{}(v, from_blob<std::span<const blob_result>>{}(rhs)); }
        else { return false; }
    });
}

struct blob_result_strict_equal_to
{
    bool operator()(blob_result const& lhs, blob_result const& rhs) const noexcept
    {
        return lhs.type == rhs.type && blob_type_dispatch(unref(lhs), [&rhs = unref(rhs)]<typename LDT>(LDT const& lv)->bool {
            return blob_type_dispatch(rhs, [lv]<typename RDT>(RDT const& rv)->bool {
                if constexpr (std::is_same_v<LDT, RDT>) {
                    if constexpr (std::is_same_v<nullptr_t, LDT>) { return true; }
                    else if constexpr (std::is_same_v<bool, LDT>) { return lv == !!rv; }
                    else if constexpr (is_integral_not_bool_v<LDT> || std::is_floating_point_v<LDT> || std::is_same_v<LDT, numetron::float16>) { return lv == rv; }
                    else if constexpr (numetron::is_basic_integer_view_v<LDT> || numetron::is_basic_decimal_view_v<LDT>) { return lv == rv; }
                    else if constexpr (std::is_same_v<std::string_view, LDT>) { return lv == rv; }
                    else if constexpr (std::is_same_v<std::span<const blob_result>, LDT>) {
                        if (rv.size() != lv.size()) return false;
                        for (size_t i = 0; i < lv.size(); ++i) {
                            if (!blob_result_strict_equal_to{}(lv[i], rv[i])) return false;
                        }
                        return true;
                    } else if constexpr (sonia::is_span_v<LDT>) { // other span types can be compared directly
                        return sonia::range_equal{}(lv, rv);
                    }
                    else { return false; }
                } else {
                    return false;
                }
            });
        });
    }
};

namespace sonia {

class smart_blob : blob_result
{
public:
    inline smart_blob() noexcept : blob_result{ nil_blob_result() } {}
    inline smart_blob(blob_result const& b) noexcept : blob_result{ b } { blob_result_pin(this); }
    inline smart_blob(blob_result&& b) noexcept : blob_result{ b } { reset(b); }

    inline smart_blob(smart_blob const& rhs) noexcept
        : blob_result { rhs }
    {
        blob_result_pin(this);
    }

    smart_blob(smart_blob && rhs) noexcept
        : blob_result{ rhs }
    {
        reset(rhs);
    }

    smart_blob& operator= (smart_blob const& rhs)
    {
        if (is_ref(type) && this != &rhs) {
            blob_result * actual_value = mutable_data_of<blob_result>(*this);
            blob_result_unpin(actual_value);
            if (is_ref(rhs->type)) {
                *actual_value = *rhs.data_of<blob_result>();
            } else {
                *actual_value = *rhs;
            }
            blob_result_pin(actual_value);
        } else {
            smart_blob tmp{ rhs };
            swap(tmp);
        }
        return *this;
    }

    smart_blob& operator= (smart_blob && rhs)
    {
        if (is_ref(type) && this != &rhs) {
            blob_result* actual_value = mutable_data_of<blob_result>(*this);
            blob_result_unpin(actual_value);
            if (is_ref(rhs->type)) {
                *actual_value = *rhs.data_of<blob_result>();
                blob_result_pin(actual_value);
                blob_result_unpin(&rhs.get());
            } else {
                *actual_value = *rhs;
            }
            reset(*rhs);
        } else {
            smart_blob tmp{ std::move(rhs) };
            swap(tmp);
        }
        return *this;
    }

    void replace(smart_blob&& with) noexcept
    {
        blob_result_unpin(this);
        static_cast<blob_result&>(*this) = static_cast<blob_result&>(with);
        reset(with);
    }

    inline void swap(smart_blob & rhs) noexcept
    {
        std::swap(static_cast<blob_result&>(*this), static_cast<blob_result&>(rhs));
    }

    ~smart_blob()
    {
        blob_result_unpin(this);
    }

    friend bool operator ==(smart_blob const& lhs, smart_blob const& rhs)
    {
        return *lhs == *rhs;
    }

    friend bool operator ==(smart_blob const& lhs, blob_result const& rhs)
    {
        return *lhs == rhs;
    }

    friend bool operator ==(blob_result const& lhs, smart_blob const& rhs)
    {
        return lhs == *rhs;
    }

    blob_result & operator*() { return *this; }
    blob_result const& operator*() const { return *this; }
    blob_result & get() { return *this; }
    blob_result const& get() const { return *this; }
    blob_result * operator->() { return this; }
    blob_result const* operator->() const { return this; }

    inline bool is_nil() const noexcept { return ::is_nil(**this); }
    inline bool is_array() const noexcept { return ::is_array(**this); }
    inline bool is_inplace() const noexcept { return !!inplace_size; }
    inline bool is_error() const noexcept { return type == blob_type::error; }

    inline const void* get_pointer() const noexcept
    {
        return inplace_size ? reinterpret_cast<const void*>(data.ui8array) : data.bp.payload_ptr;
    }

    inline size_t size() const noexcept
    {
        return inplace_size ? (size_t)inplace_size : (size_t)data.bp.size;
    }

    template <typename T>
    inline const T* data_of() const noexcept
    {
        return ::data_of<T>(*this);
    }

    template <typename T>
    inline T* data_of() noexcept
    {
        return ::mutable_data_of<T>(*this);
    }

    template <typename T>
    inline size_t size_of() const noexcept
    {
        return array_size_of<T>(*this);
    }

    inline void set_size(size_t sz)
    {
        if (inplace_size) {
            BOOST_ASSERT(sz <= inplace_size);
            inplace_size = (uint8_t)sz;
        } else {
            BOOST_ASSERT(sz <= data.bp.size);
            data.bp.size = (uint32_t)sz;
        }
    }

    template <typename T>
    inline decltype(auto) as() const { return ::as<T>(get()); }

    smart_blob& allocate(bool no_inplace = false)
    {
        blob_result_allocate(this, no_inplace);
        return *this;
    }

    [[nodiscard]]
    blob_result detach()
    {
        blob_result tmp = nil_blob_result();
        std::swap(static_cast<blob_result&>(*this), tmp);
        return tmp;
    }

    void referify()
    {
        static_cast<blob_result&>(*this) = reference_blob_result(*this);
    };
};

inline size_t hash_value(smart_blob const& v)
{
    return hash_value(*v);
}

template <typename Elem, typename Traits>
inline std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, smart_blob const& b)
{
    return os << *b;
}

// method routine
inline bool invocation::invocable::try_invoke(string_view methodname, span<const blob_result> args) noexcept
{
    smart_blob result;
    return try_invoke(methodname, args, result);
}

inline smart_blob invocation::invocable::invoke(string_view name, std::initializer_list<const blob_result> args) noexcept
{
    return invoke(name, span{ args });
}

inline smart_blob invocation::callable::invoke(std::initializer_list<const blob_result> args)
{
    return invoke(span{ args });
}

}

inline sonia::smart_blob from_blob<sonia::smart_blob>::operator()(blob_result const& val) const noexcept
{
    return sonia::smart_blob{ val };
}
