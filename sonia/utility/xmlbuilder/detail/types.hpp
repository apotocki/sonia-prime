//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <boost/container/small_vector.hpp>

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::xmlbuilder::detail {

#define X_BASIC_TYPE_DECLARATION(name)                            \
struct name {                                                     \
    bool is_optional;                                             \
    explicit name(bool is_opt = false) : is_optional{ is_opt } {} \
    bool operator== (name const&) const { return true; }          \
}

X_BASIC_TYPE_DECLARATION(x_bool);
X_BASIC_TYPE_DECLARATION(x_int);
X_BASIC_TYPE_DECLARATION(x_float);
X_BASIC_TYPE_DECLARATION(x_string);
X_BASIC_TYPE_DECLARATION(x_action);

struct x_type;

template <typename T>
struct x_array
{
    T type;
    size_t size;
    bool is_optional;
    explicit x_array(T t, size_t sz, bool is_opt = false) : type{ t }, size{ sz }, is_optional{ is_opt } {}
    bool operator== (x_array const& rhs) const { return size == rhs.size && type == rhs.type; }
};

template <typename T>
struct x_tuple
{
    boost::container::small_vector<T, 8> fields;
    bool is_optional;
    explicit x_tuple(std::span<T> sp, bool is_opt = false) : fields{ sp.begin(), sp.end() }, is_optional{ is_opt } {}
    bool operator== (x_tuple const& rhs) const { return fields == rhs.fields; }
};

struct x_object
{
    x_type const* type;
    explicit x_object(x_type const& t) : type{&t} {}
    bool operator== (x_object const& rhs) const { return type == rhs.type; }
};

using x_generic_type = make_recursive_variant<
    x_bool, x_int, x_float, x_string, x_object, x_action,
    x_array<recursive_variant_>, x_tuple<recursive_variant_>
>::type;

struct array_value_parser_visitor : static_visitor<std::pair<optional<blob_result>, string_view>>
{
    string_view value;
    size_t size;
    explicit array_value_parser_visitor(string_view v, size_t sz) : value{ v }, size{ sz } {}

    template <typename BlobT, typename XT>
    result_type do_apply(XT const&) const;

    result_type operator()(x_float const&) const;

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

struct type_value_parser_visitor : static_visitor<std::pair<optional<blob_result>, string_view>>
{
    string_view value;
    bool is_greedy; // impacts on strings
    explicit type_value_parser_visitor(string_view v, bool greedy) noexcept;

    result_type operator()(x_bool const&) const noexcept;
    result_type operator()(x_float const&) const noexcept;
    result_type operator()(x_string const&) const noexcept;
    result_type operator()(x_action const&) const noexcept;
    result_type operator()(x_array<x_generic_type> const& at) const noexcept;
    
    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
