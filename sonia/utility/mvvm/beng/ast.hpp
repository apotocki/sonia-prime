//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <cstdlib>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/optional.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/variant.hpp"

#include "beng.hpp"

namespace sonia::lang::beng {

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
    //ExprT subject;
    qname name;
    boost::container::small_vector<ExprT, 4> positioned_args;
    boost::container::small_vector<std::tuple<annotated_identifier, ExprT>, 8> named_args;

    function_call(qname n, named_expression_term_list<ExprT> args)
        : name{std::move(n)}
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
    }
};

// e.g. backgroundColor: .red
struct case_expression
{
    identifier name;
};

using expression_t = make_recursive_variant<
    qname, case_expression, decimal, small_u32string, 
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

struct parameter_t
{
    identifier name;
    beng_generic_type type;
    optional<expression_t> default_value;
};

using parameter_list_t = std::vector<parameter_t>;
using extension_list_t = std::vector<qname>;

// for extern functions
struct fn_pure_decl
{
    qname name;
    parameter_list_t parameters;
    beng_generic_type result;
};

template <typename DeclT>
struct fn_decl : fn_pure_decl
{
    std::vector<DeclT> body;
};

struct enum_decl
{
    qname name;
    std::vector<identifier> cases;
};

// e.g: type View(disabled: bool, enabled: bool, hidden:bool, empty: bool, backgroundColor: Color);
struct type_decl
{
    qname name;
    extension_list_t bases;
    parameter_list_t parameters;
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

struct exten_var
{
    identifier name;
    beng_generic_type type;
};

using infunction_declaration_t = make_recursive_variant<
    empty_t, let_statement_decl, expression_decl
>::type;

using declaration_t = make_recursive_variant<
    empty_t, exten_var, type_decl, enum_decl, let_statement_decl, expression_decl,
    fn_pure_decl, fn_decl<infunction_declaration_t>
>::type;



using fn_decl_t = fn_decl<infunction_declaration_t>;
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
