//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <cstdlib>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/container/small_vector.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/optional.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/variant.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/functional/variant_compare_three_way.hpp"
#include "sonia/mp/basic_decimal.hpp"

#include "terms.hpp"

namespace sonia::lang::bang {

class unit;
class fn_compiler_context;

template <typename T>
struct annotated
{
    T value;
    lex::resource_location location;

    inline bool operator==(annotated const& r) const noexcept { return value == r.value; }
    inline auto operator<=>(annotated const& r) const noexcept { return value <=> r.value; }
};

using annotated_string_view = annotated<string_view>;
using annotated_identifier = annotated<identifier>;
using annotated_qname = annotated<qname>;
using annotated_qname_identifier = annotated<qname_identifier>;
using annotated_entity_identifier = annotated<entity_identifier>;
using annotated_bool = annotated<bool>;
using annotated_integer = annotated<mp::integer>;
using annotated_decimal = annotated<mp::decimal>;
using annotated_string = annotated<small_string>;

struct declaration;
struct infunction_declaration;

//using elementary_expression = variant<
//    null_t, bool, decimal, small_u32string, 
//struct annotated_number
//{
//    uint64_t value;
//    lex::resource_location location;
//    inline bool operator==(annotated_number const& r) const { return id == r.id; }
//};


// ======================================================================== preliminary types

#define BANG_TRIVIAL_CMP(name) \
inline bool operator==(name const&) const { return true; } \
inline auto operator<=>(name const&) const { return std::strong_ordering::equivalent; }

struct bang_any_t { BANG_TRIVIAL_CMP(bang_any_t) };
//struct bang_bool_t { BANG_TRIVIAL_CMP(bang_bool_t) };
//struct bang_int_t { BANG_TRIVIAL_CMP(bang_int_t) };
//struct bang_float_t { BANG_TRIVIAL_CMP(bang_float_t) };
//struct bang_decimal_t { BANG_TRIVIAL_CMP(bang_decimal_t) };
//struct bang_string_t { BANG_TRIVIAL_CMP(bang_string_t) };

enum class parameter_constraint_modifier_t : uint8_t
{
    value_type_constraint = 0,
    typename_constraint = 2,
    value_constraint = 3
};

struct parameter_name
{
    optional<annotated_identifier> external_name;
    optional<annotated_identifier> internal_name;
};

//template <typename T>
//struct parameter_type
//{
//    T value;
//    bool is_const = false;
//
//    parameter_type() = default;
//
//    template <typename TArgT>
//    inline explicit parameter_type(TArgT&& t, bool cval = false)
//        : value{ std::forward<TArgT>(t) }, is_const{ cval }
//    {}
//};


template <typename T>
struct field
{
    optional<annotated_identifier> name;
    T type;

    field() = default;

    template <typename NameT>
    inline field(NameT&& n, T&& t)
        : name{ std::forward<NameT>(n) }
        , type{ std::move(t) }
    {}

    inline explicit field(T&& t)
        : type{ std::move(t) }
    {}
};

template <typename T>
using field_list = std::vector<field<T>>;


template <typename T> struct bang_preliminary_tuple
{
    field_list<T> fields;
    inline bool operator==(bang_preliminary_tuple const&) const = default;
};

template <class TupleT, typename T> struct bang_fn_base
{
    TupleT arg;
    T result;

    bang_fn_base() : result{ TupleT{} } {}

    template <typename RT>
    bang_fn_base(TupleT && a, RT && r) : arg(std::move(a)), result{ std::forward<RT>(r) } {}

    template <typename ArgT, typename RT>
    bang_fn_base(ArgT a, RT && r) : result{ std::forward<RT>(r) }
    {
        if (auto * ptuple = sonia::get<TupleT>(&a); ptuple) {
            arg = std::move(*ptuple);
        } else {
            arg.fields.emplace_back(nullopt, std::move(a));
        }
    }

    inline bool operator==(bang_fn_base const&) const = default;
    inline auto operator<=>(bang_fn_base const& r) const
    {
        //if (auto res = variant_compare_three_way{}(arg, r.arg); res != std::strong_ordering::equivalent) return res;
        if (auto res = arg <=> r.arg; res != std::strong_ordering::equivalent) return res;
        return variant_compare_three_way{}(result, r.result);
    }
};

template <typename T> using bang_preliminary_fn = bang_fn_base<bang_preliminary_tuple<T>, T>;

template <typename T> struct bang_array
{
    T type; size_t size;
    inline bool operator==(bang_array const&) const = default;
    inline auto operator<=>(bang_array const& r) const
    {
        if (auto res = variant_compare_three_way{}(type, r.type); res != std::strong_ordering::equivalent) return res;
        return size <=> r.size;
    }
};

template <typename T> struct bang_vector
{
    T type;
    inline bool operator==(bang_vector const&) const = default;
    inline auto operator<=>(bang_vector const& r) const { return variant_compare_three_way{}(type, r.type); }
};

template <typename T> struct bang_parameter_pack : bang_vector<T> {};

template <typename T> struct bang_preliminary_union
{
    boost::container::small_vector<T, 8> members;
    inline bool operator==(bang_preliminary_union const&) const = default;
    inline auto operator<=>(bang_preliminary_union const& r) const
    {
        return std::lexicographical_compare_three_way(members.begin(), members.end(), r.members.begin(), r.members.end(), variant_compare_three_way{});
    }
};

// void type is expressedby the empty tuple

using bang_preliminary_type = make_recursive_variant<
    annotated_qname,
    annotated_identifier,
    bang_preliminary_fn<recursive_variant_>,
    bang_vector<recursive_variant_>,
    bang_array<recursive_variant_>,
    bang_parameter_pack<recursive_variant_>,
    bang_preliminary_tuple<recursive_variant_>,
    bang_preliminary_union<recursive_variant_>
>::type;

using bang_preliminary_vector_t = bang_vector<bang_preliminary_type>;
using bang_preliminary_array_t = bang_array<bang_preliminary_type>;
using bang_preliminary_tuple_t = bang_preliminary_tuple<bang_preliminary_type>;
using bang_preliminary_union_t = bang_preliminary_union<bang_preliminary_type>;
using bang_preliminary_fn_t = bang_preliminary_fn<bang_preliminary_type>;
using bang_preliminary_parameter_pack_t = bang_parameter_pack<bang_preliminary_type>;

using field_t = field<bang_preliminary_type>;
using field_list_t = field_list<bang_preliminary_type>;
// ========================================================================

// ========================================================================

#define BANG_PRINT_SIMPLE_ENUM(r, data, elem) elem,

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

#define BANG_UNARY_OPERATOR_ENUM (NEGATE)
#define BANG_BINARY_OPERATOR_ENUM (ASSIGN)(LOGIC_AND)(LOGIC_OR)(CONCAT)(PLUS)(EQ)(NE)
enum class unary_operator_type
{
    BOOST_PP_SEQ_FOR_EACH(BANG_PRINT_SIMPLE_ENUM, _, BANG_UNARY_OPERATOR_ENUM)
};

enum class binary_operator_type
{
    BOOST_PP_SEQ_FOR_EACH(BANG_PRINT_SIMPLE_ENUM, _, BANG_BINARY_OPERATOR_ENUM)
};

template <binary_operator_type Op>
using binary_operator_t = std::integral_constant<binary_operator_type, Op>;

#define BANG_PRINT_BOP_CASE(r, data, elem) case binary_operator_type::elem: return BOOST_PP_CAT(BOOST_PP_STRINGIZE(elem), sv);
inline string_view to_string(binary_operator_type op) {
    switch (op) {
    BOOST_PP_SEQ_FOR_EACH(BANG_PRINT_BOP_CASE, _, BANG_BINARY_OPERATOR_ENUM)
    default: return "unknown"sv;
    }
}

#define BANG_PRINT_BOP_CASE_VISIT(r, data, elem) case binary_operator_type::elem: return std::forward<VisitorT>(vis)(binary_operator_t<binary_operator_type::elem>{}, std::forward<BinaryExprT>(exp));
template <typename BinaryExprT, typename VisitorT>
inline auto bang_binary_switcher(BinaryExprT && exp, VisitorT && vis) {
    switch (exp.op) {
    BOOST_PP_SEQ_FOR_EACH(BANG_PRINT_BOP_CASE_VISIT, _, BANG_BINARY_OPERATOR_ENUM)
    }
    THROW_INTERNAL_ERROR("unknown binary operator '%1%'"_fmt % to_string(exp.op));
}

#undef BANG_PRINT_BOP_CASE_VISIT
#undef BANG_PRINT_BOP_CASE
#undef BANG_UNARY_OPERATOR_ENUM
#undef BANG_BINARY_OPERATOR_ENUM
#undef BANG_PRINT_SIMPLE_ENUM



//template <typename ExprT>
//struct binary_expression
//{
//    ExprT left;
//    ExprT right;
//    lex::resource_location location;
//    binary_operator_type op;
//    lex::resource_location const& start() const { return get_start_location(left); }
//
//    template <typename LET, typename RET>
//    binary_expression(binary_operator_type opval, LET && let, RET && ret, lex::resource_location loc)
//        : left{std::forward<LET>(let)}, right{ std::forward<RET>(ret) }, location{std::move(loc)}, op{ opval }
//    {}
//};

//template <typename ExprT = recursive_variant_>
//struct negate_expression : unary_expression<unary_operator_type::NEGATE, ExprT> {};

//'coz recursive variant problem when template has not type parameters
//template <typename ExprT = recursive_variant_>
//struct assign_expression : binary_expression<binary_operator_type::ASSIGN, ExprT> {};

//template <typename ExprT = recursive_variant_>
//struct logic_and_expression : binary_expression<binary_operator_type::LOGIC_AND, ExprT> {};
//
//template <typename ExprT = recursive_variant_>
//struct logic_or_expression : binary_expression<binary_operator_type::LOGIC_OR, ExprT> {};

//template <typename ExprT = recursive_variant_>
//struct concat_expression : binary_expression<binary_operator_type::CONCAT, ExprT> {};

template <typename ExprT>
struct named_expression_term
{
    variant<std::tuple<annotated_identifier, ExprT>, ExprT> term;
};

template <typename ExprT>
using named_expression_term_list = std::vector<named_expression_term<ExprT>>;

template <typename ExprT>
struct expression_vector
{
    boost::container::small_vector<ExprT, 4> elements;
    lex::resource_location start;
};

template <typename ExprT>
struct pure_call
{
    boost::container::small_vector<ExprT, 4> positioned_args;
    boost::container::small_vector<std::tuple<annotated_identifier, ExprT>, 8> named_args;
    lex::resource_location location_; // operator or OPEN_PARENTHESIS location

    explicit pure_call(lex::resource_location loc) : location_{ std::move(loc) } {}

    pure_call(lex::resource_location loc, named_expression_term_list<ExprT> && args)
        : location_{ std::move(loc) }
    {
        for (named_expression_term<ExprT> & narg : args) {
            if (auto const* pure_expr = get<ExprT>(&narg.term); pure_expr) {
                positioned_args.emplace_back(std::move(*pure_expr));
            } else if (auto const* named_expr = get<std::tuple<annotated_identifier, ExprT>>(&narg.term); named_expr) {
                named_args.emplace_back(std::move(std::get<0>(*named_expr)), std::move(std::get<1>(*named_expr)));
            } else {
                THROW_INTERNAL_ERROR();
            }
        }
        std::ranges::sort(named_args, {}, [](auto const& pair) { return std::get<0>(pair).value; });
        // to do: check for repeated named args
    }

    lex::resource_location const& location() const { return location_; }
};

template <typename ExprT>
struct function_call : pure_call<ExprT>
{
    using base_t = pure_call<ExprT>;
    ExprT fn_object;
    function_call(lex::resource_location && callloc, ExprT && n, named_expression_term_list<ExprT> && args)
        : base_t{ std::move(callloc), std::move(args) }
        , fn_object{ std::move(n) }
    {}

    lex::resource_location const& start() const { return get_start_location(fn_object); }
};

template <typename ExprT>
struct unary_expression : pure_call<ExprT>
{
    using base_t = pure_call<ExprT>;
    unary_operator_type op;

    template <typename ET>
    unary_expression(unary_operator_type opval, bool is_prefix, ET&& e, lex::resource_location loc)
        : base_t{ std::move(loc) }
        , op{ opval }
    {
        base_t::positioned_args.emplace_back(std::forward<ET>(e));
    }

    lex::resource_location const& start() const { return get_start_location(base_t::positioned_args.front()); }
};

template <typename ExprT>
struct binary_expression : pure_call<ExprT>
{
    using base_t = pure_call<ExprT>;
    binary_operator_type op;

    template <typename LeftET, typename RightET>
    binary_expression(binary_operator_type opval, LeftET&& le, RightET&& re, lex::resource_location loc)
        : base_t{ std::move(loc) }
        , op{ opval }
    {
        base_t::positioned_args.emplace_back(std::forward<LeftET>(le));
        base_t::positioned_args.emplace_back(std::forward<RightET>(re));
    }

    lex::resource_location const& start() const { return get_start_location(base_t::positioned_args.front()); }
};

template <typename ExprT>
using opt_chain_link = variant<annotated_identifier, pure_call<ExprT>>;
template <typename ExprT>
using opt_chain = std::vector<opt_chain_link<ExprT>>;

template <typename ExprT>
struct chained_expression
{
    ExprT expression;
    opt_chain<ExprT> chaining;
};

// e.g. backgroundColor: .red
struct case_expression
{
    annotated_identifier name;
    lex::resource_location start;
};

template <typename ExprT>
struct member_expression
{
    ExprT object;
    annotated_identifier name;
    //bool is_object_optional = false;

    lex::resource_location const& start() const { return get_start_location(object); }
};

struct property_expression
{
    annotated_identifier name;
};

struct variable_identifier
{
    annotated_qname name;
    bool implicit; // true for identifiers started with $, e.g.: $0, $$
};

struct context_value
{
    entity_identifier type;
    lex::resource_location location;
};

template <typename ExprT>
struct let_statement_decl
{
    annotated_identifier aname;
    optional<ExprT> expression;
    optional<bang_preliminary_type> type;
    bool weakness;

    identifier const& name() const { return aname.value; }
    lex::resource_location const& location() const { return aname.location; }
};

//template <typename ExprT>
//struct assign_decl
//{
//    ExprT lvalue;
//    ExprT rvalue;
//    lex::resource_location location;
//};

template <typename ExprT>
struct expression_decl
{
    ExprT expression;
};

template <typename ExprT>
struct return_decl
{
    ExprT expression;
    lex::resource_location location;
};

template <typename ExprT>
struct while_decl
{
    ExprT condition;
    std::vector<infunction_declaration> body;
};

struct break_statement_t
{
    lex::resource_location location;
};

struct continue_statement_t
{
    lex::resource_location location;
};

template <typename ExprT>
struct if_decl
{
    ExprT condition;
    std::vector<infunction_declaration> body;
};

//template <typename ExprT>
//struct infunction_declaration
//{
//    template <typename DT> using if_decl_t = if_decl<ExprT, DT>;
//    template <typename DT> using while_decl_t = while_decl<ExprT, DT>;
//    using type = typename make_recursive_variant<
//        let_statement_decl<ExprT>, expression_decl<ExprT>, return_decl<ExprT>,
//        if_decl_t<recursive_variant_>, while_decl_t<recursive_variant_>
//    >::type;
//};

template <typename ExprT>
struct not_empty_expression
{
    ExprT value;
};

struct entity_expression
{
    entity_identifier id;
    lex::resource_location location;
};

using syntax_expression_t = make_recursive_variant<
    variable_identifier,
    annotated_bool, annotated_integer, annotated_decimal, annotated_string,
    bang_parameter_pack<recursive_variant_>,
    context_value, case_expression, property_expression, not_empty_expression<recursive_variant_>, member_expression<recursive_variant_>,
    //lambda<recursive_variant_>,
    unary_expression<recursive_variant_>,
    binary_expression<recursive_variant_>,
    //assign_expression<>, logic_and_expression<>, logic_or_expression<>, concat_expression<>,
    expression_vector<recursive_variant_>,
    function_call<recursive_variant_>,
    entity_expression
    //, chained_expression<recursive_variant_>
    //, ctprocedure
>::type;

using bang_parameter_pack_t = bang_parameter_pack<syntax_expression_t>;

struct parameter_constraint_set_t
{
    optional<syntax_expression_t> type_expression;
    std::vector<syntax_expression_t> concepts;
    std::vector<annotated_identifier> bindings;
};

template <typename T>
struct parameter
{
    parameter_name name;
    parameter_constraint_modifier_t modifier;
    parameter_constraint_set_t constraints;

    //parameter() = default;

    //explicit inline parameter(parameter_constraint_set_t cs)
    //    : modifier{ cs.modifier }
    //    , constraints{ std::move(cs.constraints) }
    //{}

    //inline parameter(parameter_name n, parameter_constraint_set_t cs)
    //    : name{ std::move(n) }
    //    , modifier{ cs.modifier }
    //    , constraints{ std::move(cs.constraints) }
    //{}

    //inline bool operator==(parameter const&) const = default;
};

template <typename T>
using parameter_list = std::vector<parameter<T>>;

using parameter_t = parameter<bang_preliminary_type>;
//using parameter_type_t = parameter_type<bang_preliminary_type>;
using parameter_list_t = parameter_list<bang_preliminary_type>;



template <typename ExprT>
struct parameter_woa : parameter_t
{
    optional<ExprT> value;

    parameter_woa() = default;

    explicit inline parameter_woa(parameter_t&& p) : parameter_t{ std::move(p) } {}

    template <typename EArgT>
    inline parameter_woa(parameter_t&& p, EArgT&& earg)
        : parameter_t{ std::move(p) }
        , value{ std::forward<EArgT>(earg) }
    {}
};

template <typename ExprT> using parameter_woa_list = std::vector<parameter_woa<ExprT>>;

enum class fn_kind : int8_t
{
    DEFAULT,
    INLINE
};

template <typename ExprT>
struct fn_pure
{
    annotated_qname aname;
    parameter_woa_list<ExprT> parameters;
    optional<ExprT> result;
    fn_kind kind = fn_kind::DEFAULT;

    qname_view name() const { return aname.value; }
    lex::resource_location const& location() const { return aname.location; }
};

template <typename ExprT>
struct lambda : fn_pure<ExprT>
{
    std::vector<infunction_declaration> body;
    lex::resource_location start;
};

using lambda_t = lambda<syntax_expression_t>;
using expression_list_t = boost::container::small_vector<syntax_expression_t, 4>;
using opt_chain_t = opt_chain<syntax_expression_t>;
using opt_chain_link_t = opt_chain_link<syntax_expression_t>;
using chained_expression_t = chained_expression<syntax_expression_t>;
using named_expression_term_t = named_expression_term<syntax_expression_t>;
using named_expression_term_list_t = named_expression_term_list<syntax_expression_t>;
using not_empty_expression_t = not_empty_expression<syntax_expression_t>;
using member_expression_t = member_expression<syntax_expression_t>;
using unary_expression_t = unary_expression<syntax_expression_t>;
using binary_expression_t = binary_expression<syntax_expression_t>;
using pure_call_t = pure_call<syntax_expression_t>;
using function_call_t = function_call<syntax_expression_t>;
using expression_vector_t = expression_vector<syntax_expression_t>;

//template <unary_operator_type Op> using unary_expression_t = unary_expression<Op, syntax_expression_t>;

template <typename T> struct is_unary_expression : false_type {};
template <typename T> requires(std::is_same_v<decltype(T::op), const unary_operator_type>) struct is_unary_expression<T> : true_type {};

struct expression_location_visitor : static_visitor<lex::resource_location const&>
{
    expression_location_visitor() = default;

    template <typename T>
    inline result_type operator()(annotated<T> const& ae) const noexcept { return ae.location; }

    inline result_type operator()(bang_parameter_pack_t const& v) const noexcept
    { 
        return apply_visitor(*this, v.type);
    }

    inline result_type operator()(context_value const& v) const noexcept { return v.location; }
    inline result_type operator()(variable_identifier const& v) const noexcept { return v.name.location; }
    inline result_type operator()(case_expression const& ce) const noexcept { return ce.start; }
    inline result_type operator()(not_empty_expression_t const& me) const noexcept { return apply_visitor(*this, me.value); }
    inline result_type operator()(member_expression_t const& me) const noexcept { return me.start(); }
    inline result_type operator()(property_expression const& me) const noexcept { return me.name.location; }
    inline result_type operator()(lambda_t const& le) const noexcept { return le.start; }
    inline result_type operator()(entity_expression const& ee) const noexcept { return ee.location; }

    inline result_type operator()(unary_expression_t const& ue) const noexcept { return ue.start(); }
    inline result_type operator()(binary_expression_t const& be) const noexcept { return be.start(); }

    inline result_type operator()(expression_vector_t const& v) const noexcept { return v.start; }
    inline result_type operator()(function_call_t const& f) const noexcept { return f.start(); }

    //template <typename T>
    //inline lex::resource_location const& operator()(T const&) const
    //{
    //    THROW_NOT_IMPLEMENTED_ERROR();
    //}
};

inline lex::resource_location const& get_start_location(syntax_expression_t const& e)
{
    return apply_visitor(expression_location_visitor{}, e);
}

// {particular location or expression, optional reference location}
struct error_context
{
    variant<lex::resource_location, syntax_expression_t> loc_or_expr;
    optional<lex::resource_location> refloc;

    lex::resource_location const& location() const
    {
        if (lex::resource_location const* ploc = get<lex::resource_location>(&loc_or_expr); ploc) {
            return *ploc;
        } else {
            return get_start_location(get<syntax_expression_t>(loc_or_expr));
        }
    }

    optional<syntax_expression_t> expression() const
    {
        if (syntax_expression_t const* pe = get<syntax_expression_t>(&loc_or_expr); pe) {
            return *pe;
        }
        return nullopt;
    }
};

using context_locator_t = function<error_context()>;
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
//using syntax_expression_t = boost::variant<operator_type, identifier, decimal, string_literal, integer_literal, function_t>;
//using syntax_expression_t = boost::variant<qname, numeric_literal, expression_group>;
//using expression_list = std::vector<syntax_expression_t>;

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



using parameter_woa_t = parameter_woa<syntax_expression_t>;
using parameter_woa_list_t = parameter_woa_list<syntax_expression_t>;

using extension_list_t = std::vector<annotated_qname_identifier>;

using fn_pure_t = fn_pure<syntax_expression_t>;

struct fn_decl_t : fn_pure_t
{
    std::vector<infunction_declaration> body;
};



struct enum_decl
{
    annotated_qname_identifier aname;
    std::vector<identifier> cases;

    qname_identifier name() const { return aname.value; }
    lex::resource_location const& location() const { return aname.location; }
};

// e.g: type View(disabled: bool, enabled: bool, hidden:bool, empty: bool, backgroundColor: Color);
struct type_decl
{
    annotated_qname_identifier aname;
    extension_list_t bases;
    parameter_woa_list_t parameters;

    qname_identifier name() const { return aname.value; }
    lex::resource_location const& location() const { return aname.location; }
};



struct extern_var
{
    annotated_identifier name;
    bang_preliminary_type type;
};

struct include_decl
{
    annotated_string path;
};

using return_decl_t = return_decl<syntax_expression_t>;
using let_statement_decl_t = let_statement_decl<syntax_expression_t>;
using expression_statement_t = expression_decl<syntax_expression_t>;
//using assign_decl_t = assign_decl<syntax_expression_t>;

//using infunction_declaration_t = typename infunction_declaration<syntax_expression_t>::type;
//using fn_decl_t = fn_decl<infunction_declaration_t>;
using if_decl_t = if_decl<syntax_expression_t>;
using while_decl_t = while_decl<syntax_expression_t>;

using generic_statement = variant<
    let_statement_decl_t, expression_statement_t, return_decl_t, while_decl_t, if_decl_t, fn_decl_t
>;

using declaration_var_type = variant<
    extern_var, let_statement_decl_t, expression_statement_t, fn_pure_t,
    include_decl, type_decl, enum_decl, return_decl_t,
    fn_decl_t, if_decl_t, while_decl_t
>;

using infunction_declaration_var_type = variant<
    let_statement_decl_t, expression_statement_t, fn_pure_t,
    fn_decl_t, if_decl_t, while_decl_t, continue_statement_t, break_statement_t, return_decl_t
>;

struct declaration : declaration_var_type { using declaration_var_type::declaration_var_type; };

struct infunction_declaration : infunction_declaration_var_type { using infunction_declaration_var_type::infunction_declaration_var_type; };

//using declaration_t = variant<
//    extern_var, let_statement_decl_t, expression_statement_t, fn_pure_t,
//    include_decl, type_decl, enum_decl, return_decl_t,
//    fn_decl_t, if_decl_t, while_decl_t
//>;

template <typename StatementT>
struct statement_adopt_visitor : static_visitor<StatementT>
{
    statement_adopt_visitor() = default;
    template <typename IDT>
    inline StatementT operator()(IDT& v) const { return StatementT{ std::move(v) }; }
};
//using declaration_t = make_recursive_variant<
//    extern_var, let_statement_decl_t, expression_statement_t, fn_pure_t,
//    include_decl, type_decl, enum_decl, return_decl_t,
//    fn_decl<infunction_declaration_t>, if_decl<expression_statement_t, infunction_declaration_t>, while_decl<expression_statement_t, infunction_declaration_t>
//>::type;
    

//using declaration_t = variant<
//    extern_var, let_statement_decl_t, expression_statement_t,
//    fn_pure_t, fn_decl<infunction_declaration_t>,
//    if_decl<infunction_declaration_t>,
//    while_decl<infunction_declaration_t>
//>;
//
//using generic_declaration_t = variant<
//    extern_var, let_statement_decl_t, expression_statement_t,
//    fn_pure_t, fn_decl<infunction_declaration_t>,
//    if_decl<infunction_declaration_t>,
//    while_decl<infunction_declaration_t>,
//    include_decl, type_decl, enum_decl
//>;

using declaration_set_t = std::vector<declaration>;
using infunction_declaration_set_t = std::vector<infunction_declaration>;



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
