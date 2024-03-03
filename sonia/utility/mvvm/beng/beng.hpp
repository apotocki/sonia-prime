//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/container/small_vector.hpp>

#include "sonia/utility/lang/utility.hpp"
#include "sonia/utility/lang/qname.hpp"
#include "sonia/utility/lang/entity.hpp"

#include "sonia/variant.hpp"
#include "sonia/utility/number/decimal.hpp"

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

struct beng_bool_t {};
struct beng_int_t {};
struct beng_float_t {};
struct beng_string_t {};
struct beng_object_t
{
    variant<qname, qname_view> name_;
    inline qname_view name() const {
        if (qname const* qn = get<qname>(&name_); qn) { return *qn; }
        return get<qname_view>(name_);
    }
};
template <typename T> struct beng_fn_t { T arg; T result; };
template <typename T> struct beng_array_t { T type; size_t size; };
template <typename T> struct beng_tuple_t { boost::container::small_vector<T, 8> fields; };
template <typename T> struct beng_union_t { boost::container::small_vector<T, 8> members; };

// void type is expressedby the empty tuple

using beng_generic_type = make_recursive_variant<
    beng_bool_t, beng_int_t, beng_float_t, beng_string_t, beng_object_t,
    beng_fn_t<recursive_variant_>,
    beng_array_t<recursive_variant_>,
    beng_tuple_t<recursive_variant_>,
    beng_union_t<recursive_variant_>
>::type;

class beng_object
{
public:
    virtual ~beng_object() = default;
};

using value_t = variant<null_t, bool, decimal, small_u32string, shared_ptr<beng_object>>; // to do: + arrays, tuples

class compiler_context;

}
