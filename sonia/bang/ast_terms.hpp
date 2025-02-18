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

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/optional.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/variant.hpp"
#include "sonia/function.hpp"
#include "sonia/small_vector.hpp"
#include "sonia/utility/functional/variant_compare_three_way.hpp"
#include "sonia/mp/basic_decimal.hpp"

#include "sonia/bang/utility/linked_list.hpp"
#include "terms.hpp"

namespace sonia::lang::bang {

class unit;

template <typename T>
struct annotated
{
    T value;
    lex::resource_location location;

    inline bool operator==(annotated const& r) const noexcept { return value == r.value; }
    inline auto operator<=>(annotated const& r) const noexcept { return value <=> r.value; }

    inline explicit operator bool() const noexcept { return (bool)value; }
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

struct statement_entry;
using statement_span = linked_list_node_span<statement_entry>;

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

// e.g. fn (externalName: string); fn (externalName $internalName: string);
struct named_parameter_name
{
    annotated_identifier external_name;
    optional<annotated_identifier> internal_name;
};

// e.g. fn ($internalName: string); fn (string);
struct unnamed_parameter_name
{
    annotated_identifier internal_name;
};

// e.g. fn ($varName ... : string);
struct varnamed_parameter_name
{
    annotated_identifier varname;
};

using parameter_name = variant<named_parameter_name, unnamed_parameter_name, varnamed_parameter_name>;

struct param_name_retriever : static_visitor<std::tuple<annotated_identifier const*, annotated_identifier const*, bool>>
{
    param_name_retriever() = default;

    inline result_type operator()(named_parameter_name const& np) const
    {
        return { &np.external_name, get_pointer(np.internal_name), false };
    }

    inline result_type operator()(unnamed_parameter_name const& np) const
    {
        return { nullptr, &np.internal_name, false };
    }

    inline result_type operator()(varnamed_parameter_name const& np) const
    {
        return { nullptr, &np.varname, true };
    }
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

template <typename TermT>
struct opt_named_term
{
    using named_pair_t = std::tuple<annotated_identifier, TermT>;
    variant<named_pair_t, TermT, nullptr_t> term;

    inline opt_named_term() noexcept : term{nullptr} {}

    inline explicit opt_named_term(TermT && t) noexcept : term{ std::move(t) } {}
    
    template <typename NameT>
    inline explicit opt_named_term(NameT&& narg, TermT&& t) noexcept
        : term{ named_pair_t{std::forward<NameT>(narg), std::move(t)} }
    {}

    inline bool has_name() const noexcept { return !!get<named_pair_t>(&term); }
    inline const annotated_identifier * name() const noexcept
    {
        if (named_pair_t const* p = get<named_pair_t>(&term); p) return &get<0>(*p);
        return nullptr;
    }

    inline TermT const& value() const noexcept
    {
        if (named_pair_t const* p = get<named_pair_t>(&term); p) return get<1>(*p);
        return get<TermT>(term);
    }

    inline TermT & value()  noexcept
    {
        if (named_pair_t * p = get<named_pair_t>(&term); p) return get<1>(*p);
        return get<TermT>(term);
    }
};

template <typename TermT>
using opt_named_term_list = small_vector<opt_named_term<TermT>, 2>;


template <typename T> struct bang_tuple
{
    opt_named_term_list<T> fields;
    inline bool operator==(bang_tuple const&) const = default;
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
            arg.fields.emplace_back(std::move(a));
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

template <typename T> using bang_fn_type = bang_fn_base<bang_tuple<T>, T>;

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
    inline bool operator==(bang_vector const&) const noexcept = default;
    inline auto operator<=>(bang_vector const& r) const noexcept { return variant_compare_three_way{}(type, r.type); }
};

template <typename T> struct bang_parameter_pack : bang_vector<T> {};

template <typename T> struct bang_union
{
    small_vector<T, 8> members;
    inline bool operator==(bang_union const&) const = default;
    inline auto operator<=>(bang_union const& r) const
    {
        return std::lexicographical_compare_three_way(members.begin(), members.end(), r.members.begin(), r.members.end(), variant_compare_three_way{});
    }
};

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
#define BANG_BINARY_OPERATOR_ENUM (ASSIGN)(LOGIC_AND)(LOGIC_OR)(BIT_OR)(CONCAT)(PLUS)(EQ)(NE)
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
struct expression_vector
{
    small_vector<ExprT, 4> elements;
    lex::resource_location start;
};

template <typename ExprT>
struct pure_call
{
    opt_named_term_list<ExprT> args_;
    //small_vector<ExprT, 4> positioned_args;
    //small_vector<std::tuple<annotated_identifier, ExprT>, 8> named_args;
    lex::resource_location location_; // operator or OPEN_PARENTHESIS location

    explicit pure_call(lex::resource_location loc) : location_{ std::move(loc) } {}

    pure_call(lex::resource_location loc, opt_named_term_list<ExprT>&& args)
        : location_{ std::move(loc) }
        , args_{ std::move(args) }
    {}

    //pure_call(lex::resource_location loc, opt_named_term_list<ExprT> && args)
    //    : location_{ std::move(loc) }
    //{
    //    for (opt_named_term<ExprT> & narg : args) {
    //        if (auto const* pure_expr = get<ExprT>(&narg.term); pure_expr) {
    //            positioned_args.emplace_back(std::move(*pure_expr));
    //        } else if (auto const* named_expr = get<std::tuple<annotated_identifier, ExprT>>(&narg.term); named_expr) {
    //            named_args.emplace_back(std::move(std::get<0>(*named_expr)), std::move(std::get<1>(*named_expr)));
    //        } else {
    //            THROW_INTERNAL_ERROR();
    //        }
    //    }
    //    std::ranges::sort(named_args, {}, [](auto const& pair) { return std::get<0>(pair).value; });
    //    // to do: check for repeated named args
    //}


    inline void emplace_back(annotated_identifier name, ExprT expr) { args_.emplace_back(std::move(name), std::move(expr)); }
    inline void emplace_back(ExprT expr) { args_.emplace_back(std::move(expr)); }

    inline opt_named_term<ExprT> const& operator[](size_t index) const noexcept { return args_[index]; }
    inline std::span<const opt_named_term<ExprT>> args() const noexcept { return args_; }
    inline lex::resource_location const& location() const noexcept { return location_; }
};

template <typename ExprT>
struct function_call : pure_call<ExprT>
{
    using base_t = pure_call<ExprT>;
    ExprT fn_object;
    function_call(lex::resource_location && callloc, ExprT && n, opt_named_term_list<ExprT> && args)
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
        base_t::args_.emplace_back(opt_named_term<ExprT>(std::forward<ET>(e)));
        //base_t::positioned_args.emplace_back(std::forward<ET>(e));
    }

    lex::resource_location const& start() const { return get_start_location(base_t::args_.front().value()); }
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
        base_t::args_.emplace_back(opt_named_term<ExprT>(std::forward<LeftET>(le)));
        base_t::args_.emplace_back(opt_named_term<ExprT>(std::forward<RightET>(re)));
        //base_t::positioned_args.emplace_back(std::forward<LeftET>(le));
        //base_t::positioned_args.emplace_back(std::forward<RightET>(re));
    }

    lex::resource_location const& start() const { return get_start_location(base_t::args_.front().value()); }
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
struct context_identifier
{
    annotated_identifier name;
    lex::resource_location start;
};

template <typename ExprT>
struct member_expression
{
    ExprT object;
    ExprT property;
    //annotated_identifier property;
    //bool is_object_optional = false;

    lex::resource_location const& start() const { return get_start_location(object); }
};

struct variable_identifier
{
    annotated_qname name;
    bool implicit; // true for identifiers started with $, e.g.: $0, $$
};

struct placeholder
{
    lex::resource_location location;
};

struct context_value
{
    entity_identifier type;
    lex::resource_location location;
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

struct break_statement_t
{
    lex::resource_location location;
};

struct continue_statement_t
{
    lex::resource_location location;
};



template <typename ExprT>
struct not_empty_expression
{
    ExprT value;
};

template <typename ExprT>
struct new_expression
{
    lex::resource_location location;
    ExprT name;
    opt_named_term_list<ExprT> arguments;
};

//struct entity_expression
//{
//    entity_identifier id;
//    lex::resource_location location;
//};

template <typename T> struct opt_named_syntax_expression_list : opt_named_term_list<T>
{
    lex::resource_location location;
};

template <typename ExprT>
struct parameter_constraint_set
{
    optional<ExprT> type_expression;
    std::vector<ExprT> concepts;
    std::vector<annotated_identifier> bindings;
};



template <typename ExprT>
struct parameter
{
    parameter_name name;
    parameter_constraint_modifier_t modifier;
    parameter_constraint_set<ExprT> constraints;

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

template <typename ExprT>
struct parameter_woa : parameter<ExprT>
{
    using parameter_t = parameter<ExprT>;
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

    inline qname_view name() const noexcept { return aname.value; }
    inline lex::resource_location const& location() const noexcept { return aname.location; }
};

template <typename ExprT>
struct lambda : fn_pure<ExprT>
{
    statement_span body;

    lambda(lex::resource_location loc, parameter_woa_list<ExprT>&& params, statement_span&& b, optional<ExprT> rtype = nullopt)
        : fn_pure<ExprT>{ annotated_qname{ {}, std::move(loc) }, std::move(params), std::move(rtype) }
        , body{ std::move(b) }
    {}
};

using syntax_expression_t = make_recursive_variant<
    placeholder, variable_identifier,
    annotated_bool, annotated_integer, annotated_decimal, annotated_string, annotated_identifier, annotated_qname,
    bang_fn_type<recursive_variant_>,
    bang_array<recursive_variant_>, bang_vector<recursive_variant_>, bang_tuple<recursive_variant_>,
    bang_union<recursive_variant_>, bang_parameter_pack<recursive_variant_>,
    context_value, context_identifier, not_empty_expression<recursive_variant_>, member_expression<recursive_variant_>,
    lambda<recursive_variant_>,
    unary_expression<recursive_variant_>,
    binary_expression<recursive_variant_>,
    //assign_expression<>, logic_and_expression<>, logic_or_expression<>, concat_expression<>,
    expression_vector<recursive_variant_>,
    function_call<recursive_variant_>,
    new_expression<recursive_variant_>,
    annotated_entity_identifier,
    opt_named_syntax_expression_list<recursive_variant_>
    
    //, chained_expression<recursive_variant_>
    //, ctprocedure
>::type;

using parameter_constraint_set_t = parameter_constraint_set<syntax_expression_t>;
using parameter_t = parameter<syntax_expression_t>;
using fn_pure_t = fn_pure<syntax_expression_t>;
using lambda_t = lambda<syntax_expression_t>;
using opt_named_syntax_expression_t = opt_named_term<syntax_expression_t>;
using opt_named_syntax_expression_list_t = opt_named_syntax_expression_list<syntax_expression_t>;

using bang_parameter_pack_t = bang_parameter_pack<syntax_expression_t>;

struct field_t
{
    annotated_identifier name;
    parameter_constraint_modifier_t modifier;
    syntax_expression_t type;
    optional<syntax_expression_t> value;
};
using field_list_t = std::vector<field_t>;

using parameter_list_t = std::vector<parameter_t>;

using parameter_woa_t = parameter_woa<syntax_expression_t>;
using parameter_woa_list_t = parameter_woa_list<syntax_expression_t>;


using expression_list_t = small_vector<syntax_expression_t, 4>;
using opt_chain_t = opt_chain<syntax_expression_t>;
using opt_chain_link_t = opt_chain_link<syntax_expression_t>;
using chained_expression_t = chained_expression<syntax_expression_t>;
using named_expression_t = opt_named_term<syntax_expression_t>;
using named_expression_list_t = opt_named_term_list<syntax_expression_t>;
using not_empty_expression_t = not_empty_expression<syntax_expression_t>;
using member_expression_t = member_expression<syntax_expression_t>;
using unary_expression_t = unary_expression<syntax_expression_t>;
using binary_expression_t = binary_expression<syntax_expression_t>;
using pure_call_t = pure_call<syntax_expression_t>;
using new_expression_t = new_expression<syntax_expression_t>;
using function_call_t = function_call<syntax_expression_t>;
using expression_vector_t = expression_vector<syntax_expression_t>;

using bang_fn_type_t = bang_fn_type<syntax_expression_t>;
using bang_tuple_t = bang_tuple<syntax_expression_t>;
using bang_vector_t = bang_vector<syntax_expression_t>;
using bang_array_t = bang_array<syntax_expression_t>;
using bang_union_t = bang_union<syntax_expression_t>;
//template <unary_operator_type Op> using unary_expression_t = unary_expression<Op, syntax_expression_t>;

template <typename T> struct is_unary_expression : false_type {};
template <typename T> requires(std::is_same_v<decltype(T::op), const unary_operator_type>) struct is_unary_expression<T> : true_type {};

lex::resource_location const& get_start_location(syntax_expression_t const&);

// {particular location or expression, optional reference location}
struct error_context
{
    variant<lex::resource_location, syntax_expression_t> loc_or_expr;
    lex::resource_location refloc;

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

//using expression_term_t = boost::variant<qname, u32string, string_literal, numeric, numeric_literal, procedure, ctprocedure>;

using expression_terms_t = std::vector<expression_term_t>;
//using signature_named_expression_terms_t = std::tuple<identifier, expression_terms_t>;
//using signature_named_expression_terms_list_t = std::vector<signature_named_expression_terms_t>;


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

using extension_list_t = std::vector<annotated_qname_identifier>;

struct fn_decl_t : fn_pure_t
{
    statement_span body;
};

struct using_decl
{
    annotated_qname aname;
    optional<parameter_woa_list_t> parameters;
    syntax_expression_t expression;

    qname_view name() const { return aname.value; }
    lex::resource_location const& location() const { return aname.location; }
};

struct struct_decl
{
    variant<annotated_qname, fn_pure_t> decl;
    variant<field_list_t, statement_span> body;

    inline bool is_function() const noexcept
    {
        return get<fn_pure_t>(&decl) != nullptr;
    }

    fn_pure_t const* as_fn() const noexcept
    {
        return get<fn_pure_t>(&decl);
    }

    annotated_qname const* as_name() const noexcept
    {
        return get<annotated_qname>(&decl);
    }

    inline qname_view name() const noexcept
    {
        if (auto const* pfn = get<fn_pure_t>(&decl); pfn) {
            return pfn->name();
        }
        return get<annotated_qname>(decl).value;
    }

    lex::resource_location const& location() const noexcept
    {
        if (auto const* pfn = get<fn_pure_t>(&decl); pfn) {
            return pfn->location();
        }
        return get<annotated_qname>(decl).location;
    }
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
    syntax_expression_t type;
};

struct include_decl
{
    annotated_string path;
};

struct let_statement
{
    annotated_identifier aname;
    optional<syntax_expression_t> expression;
    optional<syntax_expression_t> type;
    bool weakness;

    identifier const& name() const { return aname.value; }
    lex::resource_location const& location() const { return aname.location; }
};

using return_decl_t = return_decl<syntax_expression_t>;
using expression_statement_t = expression_decl<syntax_expression_t>;
//using assign_decl_t = assign_decl<syntax_expression_t>;

//using fn_decl_t = fn_decl<infunction_declaration_t>;

struct if_decl
{
    syntax_expression_t condition;
    statement_span true_body;
    statement_span false_body;
};

struct while_decl
{
    syntax_expression_t condition;
    statement_span body;
    optional<syntax_expression_t> continue_expression; // called before condition strating eith second condition check (like c/c++ for expression)
};

struct for_decl
{
    syntax_expression_t iter;
    syntax_expression_t coll;
    statement_span body;
};

// statements that don't need ';' separator at the end
using finished_statement_type = variant<
    while_decl, for_decl, if_decl, fn_decl_t, struct_decl
>;

using generic_statement_type = variant<
    let_statement, expression_statement_t, return_decl_t, fn_decl_t, struct_decl, using_decl
>;

using statement = variant<
    extern_var, let_statement, expression_statement_t, fn_pure_t,
    include_decl, struct_decl, using_decl, enum_decl, return_decl_t,
    fn_decl_t, if_decl, while_decl, for_decl, break_statement_t, continue_statement_t
>;

//using infunction_statement_type = variant<
//    extern_var, let_statement, expression_statement_t, fn_pure, struct_decl, using_decl,
//    fn_decl_t, if_decl_t, while_decl_t, continue_statement_t, break_statement_t, return_decl_t
//>;

using statement_entry_type = linked_list_node<statement>;
using statement_list = linked_list<statement>;
//struct statement : statement_type { using statement_type::statement_type; };

struct statement_entry : statement_entry_type { using statement_entry_type::statement_entry_type; };
using managed_statement_list = managed_linked_list<statement, unit>;

static_assert(sizeof(statement_entry) == sizeof(statement_entry_type));

//using declaration_t = variant<
//    extern_var, let_statement_decl_t, expression_statement_t, fn_pure,
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
//    extern_var, let_statement_decl_t, expression_statement_t, fn_pure,
//    include_decl, type_decl, enum_decl, return_decl_t,
//    fn_decl<infunction_declaration_t>, if_decl<expression_statement_t, infunction_declaration_t>, while_decl<expression_statement_t, infunction_declaration_t>
//>::type;
    

//using declaration_t = variant<
//    extern_var, let_statement_decl_t, expression_statement_t,
//    fn_pure, fn_decl<infunction_declaration_t>,
//    if_decl<infunction_declaration_t>,
//    while_decl<infunction_declaration_t>
//>;
//
//using generic_declaration_t = variant<
//    extern_var, let_statement_decl_t, expression_statement_t,
//    fn_pure, fn_decl<infunction_declaration_t>,
//    if_decl<infunction_declaration_t>,
//    while_decl<infunction_declaration_t>,
//    include_decl, type_decl, enum_decl
//>;


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
