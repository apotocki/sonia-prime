//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <algorithm>
#include <boost/container/small_vector.hpp>

#include "sonia/utility/lang/utility.hpp"
#include "sonia/utility/lang/qname.hpp"
#include "sonia/utility/lang/entity.hpp"

#include "sonia/variant.hpp"
#include "sonia/mp/decimal.hpp"

#include "sonia/utility/functional/variant_compare_three_way.hpp"

namespace sonia::lang::beng {

using qname = sonia::lang::qname<identifier>;
using qname_view = sonia::lang::qname_view<identifier>;
using entity = sonia::lang::entity<identifier, lex::resource_location>;

struct annotated_string_view
{
    string_view value;
    lex::resource_location location;
};

struct annotated_identifier
{
    identifier id;
    lex::resource_location location;
};

struct annotated_qname
{
    qname name;
    lex::resource_location location;
};

#define BENG_TRIVIAL_CMP(name) \
inline bool operator==(name const&) const { return true; } \
inline auto operator<=>(name const&) const { return std::strong_ordering::equivalent; }

struct beng_bool_t { BENG_TRIVIAL_CMP(beng_bool_t) };
struct beng_int_t { BENG_TRIVIAL_CMP(beng_int_t) };
struct beng_float_t { BENG_TRIVIAL_CMP(beng_float_t) };
struct beng_decimal_t { BENG_TRIVIAL_CMP(beng_decimal_t) };
struct beng_string_t { BENG_TRIVIAL_CMP(beng_string_t) };
struct beng_preliminary_object_t
{
    annotated_qname name_;
    inline qname_view name() const { return name_.name; }
    lex::resource_location const& location() const { return name_.location; }
    inline bool operator==(beng_preliminary_object_t const& rhs) const { return name() == rhs.name(); };
};

template <typename T> struct beng_fn
{
    T arg;
    T result;
    inline bool operator==(beng_fn const&) const = default;
    inline auto operator<=>(beng_fn const& r) const
    {
        if (auto res = variant_compare_three_way{}(arg, r.arg); res != std::strong_ordering::equivalent) return res;
        return variant_compare_three_way{}(result, r.result);
    }
};

template <typename T> struct beng_array
{
    T type; size_t size;
    inline bool operator==(beng_array const&) const = default;
    inline auto operator<=>(beng_array const& r) const
    {
        if (auto res = variant_compare_three_way{}(type, r.type); res != std::strong_ordering::equivalent) return res;
        return size <=> r.size;
    }
};

template <typename T> struct beng_vector
{
    T type;
    inline bool operator==(beng_vector const&) const = default;
    inline auto operator<=>(beng_vector const& r) const { return variant_compare_three_way{}(type, r.type); }
};

template <typename T> struct beng_tuple
{
    boost::container::small_vector<T, 8> fields;
    inline bool operator==(beng_tuple const&) const = default;
    inline auto operator<=>(beng_tuple const& r) const
    {
        return std::lexicographical_compare_three_way(fields.begin(), fields.end(), r.fields.begin(), r.fields.end(), variant_compare_three_way{});
    }
};

template <typename T> struct beng_union
{
    boost::container::small_vector<T, 8> members;
    inline bool operator==(beng_union const&) const = default;
    inline auto operator<=>(beng_union const& r) const
    {
        return std::lexicographical_compare_three_way(members.begin(), members.end(), r.members.begin(), r.members.end(), variant_compare_three_way{});
    }
};

// void type is expressedby the empty tuple

using beng_preliminary_type = make_recursive_variant<
    beng_bool_t, beng_int_t, beng_float_t, beng_decimal_t, beng_string_t, beng_preliminary_object_t,
    beng_fn<recursive_variant_>,
    beng_vector<recursive_variant_>,
    beng_array<recursive_variant_>,
    beng_tuple<recursive_variant_>,
    beng_union<recursive_variant_>
>::type;

using beng_preliminary_vector_t = beng_vector<beng_preliminary_type>;
using beng_preliminary_array_t = beng_array<beng_preliminary_type>;
using beng_preliminary_tuple_t = beng_tuple<beng_preliminary_type>;
using beng_preliminary_union_t = beng_union<beng_preliminary_type>;
using beng_preliminary_fn_t = beng_fn<beng_preliminary_type>;

struct beng_object_t
{
    entity const* value;
    inline qname_view name() const { return value->name(); }
    inline bool operator==(beng_object_t const& rhs) const { return value == rhs.value; };
    inline auto operator<=>(beng_object_t const& rhs) const { return value->name() <=> rhs.value->name(); };
};

using beng_type = make_recursive_variant<
    beng_bool_t, beng_int_t, beng_float_t, beng_decimal_t, beng_string_t, beng_object_t,
    beng_fn<recursive_variant_>,
    beng_vector<recursive_variant_>,
    beng_array<recursive_variant_>,
    beng_tuple<recursive_variant_>,
    beng_union<recursive_variant_>
>::type;

using beng_vector_t = beng_vector<beng_type>;
using beng_array_t = beng_array<beng_type>;
using beng_tuple_t = beng_tuple<beng_type>;
using beng_union_t = beng_union<beng_type>;
using beng_fn_t = beng_fn<beng_type>;

struct function_signature
{
    beng_type result_type;
    std::vector<std::tuple<annotated_identifier, beng_type>> named_parameters;
    std::vector<beng_type> position_parameters;
    qname mangled_name;

    size_t parameters_count() const { return position_parameters.size() + named_parameters.size(); }
};

/*
class beng_object
{
public:
    virtual ~beng_object() = default;
};
*/

using value_t = make_recursive_variant<
    null_t, bool, decimal, small_u32string, // shared_ptr<beng_object>,
    std::vector<recursive_variant_>
>::type; // to do: tuples

class compiler_context;

}
