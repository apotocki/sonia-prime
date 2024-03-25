//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <vector>
#include "sonia/variant.hpp"
#include "sonia/string.hpp"

#include "ast_terms.hpp"

#include "sonia/utility/lang/entity.hpp"

namespace sonia::lang::beng {

using entity = sonia::lang::entity<identifier, lex::resource_location>;

class variable_entity;
class functional_entity;

// ======================================================================== types
struct beng_object_t
{
    entity const* value;
    inline qname_view name() const { return value->name(); }
    inline bool operator==(beng_object_t const& rhs) const { return value == rhs.value; };
    inline auto operator<=>(beng_object_t const& rhs) const { return value->name() <=> rhs.value->name(); };
};

template <typename T> struct beng_tuple
{
    std::vector<T> fields;
    std::vector<std::tuple<annotated_identifier, T>> named_fields;

    inline bool operator==(beng_tuple const&) const = default;
    inline auto operator<=>(beng_tuple const& r) const
    {
        if (auto res = std::lexicographical_compare_three_way(
            fields.begin(), fields.end(), r.fields.begin(), r.fields.end(), variant_compare_three_way{});
            res != std::strong_ordering::equivalent) return res;
        return std::lexicographical_compare_three_way(
            named_fields.begin(), named_fields.end(), r.named_fields.begin(), r.named_fields.end(),
            [](auto const& ltpl, auto const& rtpl) {
                if (auto res = std::get<0>(ltpl).id <=> std::get<0>(rtpl).id; res != std::strong_ordering::equivalent) return res;
                return variant_compare_three_way{}(std::get<1>(ltpl), std::get<1>(ltpl));
            });
    }
};

template <typename T> using beng_fn = beng_fn_base<beng_tuple<T>, T>;

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

// ======================================================================== function

struct function_signature
{
    beng_fn_t fn_type;
    //beng_type result_type;
    //beng_tuple_t parameters;
    //std::vector<std::tuple<annotated_identifier, beng_type>> named_parameters;
    //std::vector<beng_type> position_parameters;
    identifier mangled_id;

    size_t parameters_count() const { return fn_type.arg.fields.size() + fn_type.arg.named_fields.size(); }

    inline auto& position_parameters() noexcept { return fn_type.arg.fields; }
    inline auto const& position_parameters() const noexcept { return fn_type.arg.fields; }
    inline auto& named_parameters() noexcept { return fn_type.arg.named_fields; }
    inline auto const& named_parameters() const noexcept { return fn_type.arg.named_fields; }

    void setup(fn_compiler_context&, parameter_woa_list_t&);
    void normilize(fn_compiler_context&);
    void build_mangled_id(unit&);

    //beng_type to_function_type() const { return beng_fn_t{}}
    //{
    //    beng_tuple_t argtpl;
    //    argtpl.fields = position_parameters;
    //    argtpl.named_fields = named_parameters;
    //    return beng_fn_t{ std::move(argtpl), }
    //    argtpl.named_fields.reserve(named_parameters.size());
    //    std::ranges::for_each(named_parameters, std::back_inserter(argtpl.named_fields),
    //        [](auto const& tpl) { return std::tuple{ std::get<0>(tpl).id, std::get<1>(tpl) }; });
    //}
};

// ======================================================================== values
struct function_value { qname mangled_name; };
using value_t = make_recursive_variant<
    null_t, bool, decimal, small_u32string, // shared_ptr<beng_object>,
    function_value,
    std::vector<recursive_variant_>
>::type; // to do: tuples

namespace semantic {

struct push_variable { variable_entity const* entity; };
struct push_value { value_t value; };
struct set_variable { variable_entity const* entity; };
struct return_statement {};

struct invoke_function
{
    variant<qname, qname_view> varname;
    qname_view function_entity_name() const
    {
        if (qname_view const* qv = get<qname_view>(&varname); qv) return *qv;
        return get<qname>(varname);
    }
};

template <typename SemanticExpressionT>
struct conditional
{
    std::vector<SemanticExpressionT> true_branch;
    std::vector<SemanticExpressionT> false_branch;
};

}

// make_recursive_variant<
using semantic_expression_type = make_recursive_variant<
    empty_t, // no op
    semantic::push_variable, semantic::push_value,
    semantic::set_variable, semantic::invoke_function, semantic::return_statement,
    std::vector<recursive_variant_>,
    semantic::conditional<recursive_variant_>
>::type;

using semantic_expression_pair = std::pair<semantic_expression_type, beng_type>;

class function_scope_type
{

};

/*
class function_t
{
public:
    virtual ~function_t() = default;
};

class implemented_function : public function_t
{
public:
    qname name;
    std::vector<semantic_expression_type> body;
    bool is_inline = false;
};
*/

}
