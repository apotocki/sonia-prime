//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <cstdlib>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/container/small_vector.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/optional.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/variant.hpp"
#include "sonia/utility/functional/variant_compare_three_way.hpp"
#include "sonia/mp/decimal.hpp"

#include "terms.hpp"

namespace sonia::lang::beng {

class unit;
class fn_compiler_context;

struct annotated_string_view
{
    string_view value;
    lex::resource_location location;
};

struct annotated_identifier
{
    identifier id;
    lex::resource_location location;
    inline bool operator==(annotated_identifier const& r) const { return id == r.id; }
};

//struct annotated_number
//{
//    uint64_t value;
//    lex::resource_location location;
//    inline bool operator==(annotated_number const& r) const { return id == r.id; }
//};

struct annotated_qname
{
    qname name;
    lex::resource_location location;
};

// ======================================================================== preliminary types

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

template <typename T>
struct parameter
{
    optional<annotated_identifier> name;
    T type;
    inline bool operator==(parameter const&) const = default;
};

template <typename T>
using parameter_list = std::vector<parameter<T>>;

template <typename T> struct beng_preliminary_tuple
{
    parameter_list<T> fields;
    inline bool operator==(beng_preliminary_tuple const&) const = default;
};

template <class TupleT, typename T> struct beng_fn_base
{
    TupleT arg;
    T result;

    beng_fn_base() : result{ TupleT{} } {}

    template <typename RT>
    beng_fn_base(TupleT && a, RT && r) : arg(std::move(a)), result{ std::forward<RT>(r) } {}

    template <typename ArgT, typename RT>
    beng_fn_base(ArgT a, RT && r) : result{ std::forward<RT>(r) }
    {
        if (auto * ptuple = sonia::get<TupleT>(&a); ptuple) {
            arg = std::move(*ptuple);
        } else {
            arg.fields.emplace_back(nullopt, std::move(a));
        }
    }

    inline bool operator==(beng_fn_base const&) const = default;
    inline auto operator<=>(beng_fn_base const& r) const
    {
        //if (auto res = variant_compare_three_way{}(arg, r.arg); res != std::strong_ordering::equivalent) return res;
        if (auto res = arg <=> r.arg; res != std::strong_ordering::equivalent) return res;
        return variant_compare_three_way{}(result, r.result);
    }
};

template <typename T> using beng_preliminary_fn = beng_fn_base<beng_preliminary_tuple<T>, T>;

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
    beng_preliminary_fn<recursive_variant_>,
    beng_vector<recursive_variant_>,
    beng_array<recursive_variant_>,
    beng_preliminary_tuple<recursive_variant_>,
    beng_union<recursive_variant_>
>::type;

using beng_preliminary_vector_t = beng_vector<beng_preliminary_type>;
using beng_preliminary_array_t = beng_array<beng_preliminary_type>;
using beng_preliminary_tuple_t = beng_preliminary_tuple<beng_preliminary_type>;
using beng_preliminary_union_t = beng_union<beng_preliminary_type>;
using beng_preliminary_fn_t = beng_preliminary_fn<beng_preliminary_type>;

// ========================================================================

// ========================================================================

#define BENG_PRINT_SIMPLE_ENUM(r, data, i, elem) elem,

/*
#define OPERATOR_ENUM (POSTFIX_INCREMENT)(POSTFIX_DECREMENT)(PERIOD)(ARROW)(DBLCOLON)(POSTFIX_CLOSE_BROKET)(POSTFIX_RIGHTSHIFT) \
    (PREFIX_INCREMENT)(PREFIX_DECREMENT)(PREFIX_PLUS)(PREFIX_MINUS)(PREFIX_INV)(PREFIX_BITINV)(TYPE_CAST)(PREFIX_DEREF)(PREFIX_ADDRESS)(SIZEOF) \
    (MBPTR)(PTRMBPTR)\
    (MUL)(DIV)(MOD)\
    (PLUS)(MINUS)\
    (LEFTSHIFT)(RIGHTSHIFT)\
    (LESS)(LE)(GREATER)(GE)\
    (EQ)(NE)\
    (BITAND)(BITXOR)(BITOR)\
    (AND)(OR)\
    (IFF)(ASSIGN)(ADDASSIGN)(SUBASSIGN)(MULASSIGN)(DIVASSIGN)(MODASSIGN)(LSHIFTSSIGN)(RSHIFTSSIGN)(BITANDASSIGN)(BITORASSIGN)(BITXORASSIGN)(THROW)\
    (COMMA)
*/

#define BINARY_OPERATOR_ENUM (ASSIGN)
enum class operator_type
{
    BOOST_PP_SEQ_FOR_EACH_I(BENG_PRINT_SIMPLE_ENUM, _, BINARY_OPERATOR_ENUM)
};

#undef BINARY_OPERATOR_ENUM
#undef BENG_PRINT_SIMPLE_ENUM

template <operator_type Op, typename ExprT>
struct binary_expression
{
    ExprT left;
    ExprT right;
};

//'coz recursive variant problem when template has not type parameters
template <typename ExprT = recursive_variant_>
struct assign_expression : binary_expression<operator_type::ASSIGN, ExprT> {};

template <typename ExprT>
using named_expression_term = variant<std::tuple<annotated_identifier, ExprT>, ExprT>;

template <typename ExprT>
using named_expression_term_list = std::vector<named_expression_term<ExprT>>;

template <typename ExprT>
struct expression_vector
{
    boost::container::small_vector<ExprT, 4> elements;
};

template <typename ExprT>
struct function_call
{
    ExprT fn_object;
    boost::container::small_vector<ExprT, 4> positioned_args;
    boost::container::small_vector<std::tuple<annotated_identifier, ExprT>, 8> named_args;

    function_call(ExprT n, named_expression_term_list<ExprT> args)
        : fn_object{std::move(n)}
    {
        for (auto & narg : args) {
            if (auto const* pure_expr = get<ExprT>(&narg); pure_expr) {
                positioned_args.emplace_back(std::move(*pure_expr));
            } else if (auto const* named_expr = get<std::tuple<annotated_identifier, ExprT>>(&narg); named_expr) {
                named_args.emplace_back(std::move(*named_expr));
            } else {
                THROW_INTERNAL_ERROR();
            }
        }
        std::ranges::sort(named_args, {}, [](auto const& pair) { return std::get<0>(pair).id; });
        // to do: check for repeated named args
    }
};

// e.g. backgroundColor: .red
struct case_expression
{
    identifier name;
};

struct variable_identifier
{
    qname name;
    lex::resource_location location;
    bool scope_local; // true e.g. for $0, $$
};

using expression_t = make_recursive_variant<
    variable_identifier, case_expression, decimal, small_u32string,
    assign_expression<>,
    expression_vector<recursive_variant_>,
    function_call<recursive_variant_>
    //, ctprocedure
>::type;

using expression_list_t = boost::container::small_vector<expression_t, 4>;
using named_expression_term_t = named_expression_term<expression_t>;
using named_expression_term_list_t = named_expression_term_list<expression_t>;
using assign_expression_t = assign_expression<expression_t>;
using function_call_t = function_call<expression_t>;
using expression_vector_t = expression_vector<expression_t>;
template <operator_type Op> using binary_expression_t = binary_expression<Op, expression_t>;

/*
enum class call_type
{
    FUNCTION_CALL,
    SUBSCRIPT
};
*/

/*

struct ctprocedure;
struct expression_group;
//struct comma {};
//using expression_t = boost::variant<operator_type, identifier, decimal, string_literal, integer_literal, function_t>;
//using expression_t = boost::variant<qname, numeric_literal, expression_group>;
//using expression_list = std::vector<expression_t>;

//using expression_term_t = boost::variant<qname, u32string, string_literal, numeric, numeric_literal, procedure, ctprocedure>;

using expression_terms_t = std::vector<expression_term_t>;
//using signature_named_expression_terms_t = std::tuple<identifier, expression_terms_t>;
//using signature_named_expression_terms_list_t = std::vector<signature_named_expression_terms_t>;


//struct procedure
//{
//    call_type type;
//    qname name;
//    expression_list args;
//};



struct ctprocedure
{
    qname name;
    expression_named_terms_list_t args;
};

struct expression_group
{
    expression_terms_t terms;
};

struct noop_statement{};

struct return_statement_t
{
    expression_terms_t value;
};

struct decl_var_statement_t
{
    identifier name;
    expression_terms_t type;
    expression_terms_t value;
};

struct for_statement_t;

using statement_t = boost::variant<
    noop_statement,
    expression_terms_t,
    decl_var_statement_t,
    shared_ptr<for_statement_t>,
    return_statement_t
>;
using statement_list_t = std::vector<statement_t>;

struct for_statement_t
{
    statement_t init;
    expression_terms_t condition;
    expression_terms_t update;
    statement_list_t body;
};

struct function_def
{
    expression_named_terms_list_t args;
    expression_terms_t result;
    boost::variant<char_array_t, statement_list_t> body; // external or body
};
*/

/*
inline size_t hash_value(function_def const& v)
{
    size_t hv = hash_init_value();
    hash_combine(hv, hash_value(v.result));

    return hasher()(v.args, v.result);
}

inline bool operator== (function_def const& l, function_def const& r)
{
    if (l.result != r.result || l.args.size() != r.args.size()) return false;
    for (auto lit = l.args.begin(), leit = l.args.end(), rit = r.args.begin(); lit != leit; ++lit, ++rit) {
        if (std::get<0>(*lit) != std::get<0>(*rit)) return false;
    }
    return true;
}
*/

/*
struct using_decl
{
    qname ns;
    identifier name;
    expression_terms_t value;
};

struct function_decl
{
    qname name;
    function_def def;
};

struct extern_function_decl
{
    identifier name;
    function_def def;
};

//inline size_t hash_value(function_decl const& v)
//{
//    return hasher()(v.name, v.def);
//}
//
//inline bool operator== (function_decl const& l, function_decl const& r)
//{
//    return l.name == r.name && l.def == r.def;
//}
*/

using parameter_t = parameter<beng_preliminary_type>;
using parameter_list_t = parameter_list<beng_preliminary_type>;

struct parameter_woa_t : parameter_t
{
    optional<expression_t> value;
};

using parameter_woa_list_t = std::vector<parameter_woa_t>;

using extension_list_t = std::vector<annotated_qname>;

// for extern functions
struct fn_pure_decl
{
    annotated_qname aname;
    parameter_woa_list_t parameters;
    optional<beng_preliminary_type> result;

    qname_view name() const { return aname.name; }
    lex::resource_location const& location() const { return aname.location; }
};

template <typename DeclT>
struct fn_decl : fn_pure_decl
{
    std::vector<DeclT> body;
};

struct enum_decl
{
    annotated_qname aname;
    std::vector<identifier> cases;

    qname_view name() const { return aname.name; }
    lex::resource_location const& location() const { return aname.location; }
};

// e.g: type View(disabled: bool, enabled: bool, hidden:bool, empty: bool, backgroundColor: Color);
struct type_decl
{
    annotated_qname aname;
    extension_list_t bases;
    parameter_woa_list_t parameters;

    qname const& name() const { return aname.name; }
    lex::resource_location const& location() const { return aname.location; }
};

struct let_statement_decl
{
    identifier name;
    expression_t expression;
};

struct expression_decl
{
    expression_t expression;
};

struct return_decl
{
    expression_t expression;
};

struct exten_var
{
    identifier name;
    beng_preliminary_type type;
};

using infunction_declaration_t = make_recursive_variant<
    let_statement_decl, expression_decl, return_decl
>::type;

using declaration_t = make_recursive_variant<
    exten_var, let_statement_decl, expression_decl,
    fn_pure_decl, fn_decl<infunction_declaration_t>
>::type;

using type_declaration_t = variant<type_decl, enum_decl>;

using fn_decl_t = fn_decl<infunction_declaration_t>;

struct declaration_set_t
{
    std::vector<declaration_t> generic;
    std::vector<type_declaration_t> types;
};


/*
struct declaration
{
    boost::variant<function_decl> value;
};
*/



template <typename LocationT>
void update_location(LocationT & loc, const char* text)
{
    loc.begin.line = loc.end.line;
    loc.begin.column = loc.end.column;
    for(int i = 0; text[i] != '\0'; i++) {
        if(text[i] == '\n') {
            loc.end.line++;
            loc.end.column = 1;
        } else {
            loc.end.column++;
        }
    }
}

}
