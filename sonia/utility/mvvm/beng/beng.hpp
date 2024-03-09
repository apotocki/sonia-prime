//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/container/small_vector.hpp>

#include "sonia/utility/lang/utility.hpp"
#include "sonia/utility/lang/qname.hpp"
#include "sonia/utility/lang/entity.hpp"

#include "sonia/variant.hpp"
#include "sonia/mp/decimal.hpp"

namespace sonia::lang::beng {

using qname = sonia::lang::qname<identifier>;
using qname_view = sonia::lang::qname_view<identifier>;
using entity = sonia::lang::entity<identifier>;

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

struct beng_bool_t { inline bool operator==(beng_bool_t const&) const { return true; } };
struct beng_int_t { inline bool operator==(beng_int_t const&) const { return true; } };
struct beng_float_t { inline bool operator==(beng_float_t const&) const { return true; } };
struct beng_decimal_t { inline bool operator==(beng_decimal_t const&) const { return true; } };
struct beng_string_t { inline bool operator==(beng_string_t const&) const { return true; } };
struct beng_object_t
{
    variant<qname, qname_view> name_;
    inline qname_view name() const {
        if (qname const* qn = get<qname>(&name_); qn) { return *qn; }
        return get<qname_view>(name_);
    }
    inline bool operator==(beng_object_t const& rhs) const { return name() == rhs.name(); };
};
template <typename T> struct beng_fn { T arg; T result; inline bool operator==(beng_fn const&) const = default; };
template <typename T> struct beng_vector { T type; inline bool operator==(beng_vector const&) const = default; };
template <typename T> struct beng_array { T type; size_t size; inline bool operator==(beng_array const&) const = default; };
template <typename T> struct beng_tuple { boost::container::small_vector<T, 8> fields; inline bool operator==(beng_tuple const&) const = default; };
template <typename T> struct beng_union { boost::container::small_vector<T, 8> members; inline bool operator==(beng_union const&) const = default; };

// void type is expressedby the empty tuple

using beng_generic_type = make_recursive_variant<
    beng_bool_t, beng_int_t, beng_float_t, beng_decimal_t, beng_string_t, beng_object_t,
    beng_fn<recursive_variant_>,
    beng_vector<recursive_variant_>,
    beng_array<recursive_variant_>,
    beng_tuple<recursive_variant_>,
    beng_union<recursive_variant_>
>::type;

using beng_vector_t = beng_vector<beng_generic_type>;
using beng_array_t = beng_array<beng_generic_type>;
using beng_tuple_t = beng_tuple<beng_generic_type>;
using beng_union_t = beng_union<beng_generic_type>;
using beng_fn_t = beng_fn<beng_generic_type>;

struct function_signature
{
    beng_generic_type result_type;
    std::vector<std::tuple<identifier, beng_generic_type>> named_parameters;
    std::vector<beng_generic_type> position_parameters;
    qname mangled_name;
};

class beng_object
{
public:
    virtual ~beng_object() = default;
};

using value_t = make_recursive_variant<
    null_t, bool, decimal, small_u32string, shared_ptr<beng_object>,
    std::vector<recursive_variant_>
>::type; // to do: tuples

class compiler_context;

}
