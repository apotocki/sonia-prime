/*=============================================================================
    Bang(c) by Alexander A Pototskiy
==============================================================================*/
%require "3.2"
%language "c++"
//%debug
%define api.value.type variant
%define api.prefix {bang_lang}
//%define lr.type ielr
//%define parse.lac full

%defines
%locations
%token-table
%define parse.error verbose

%parse-param { void * scanner }
//%parse-param { parser_context & ctx }
%param { parser_context & ctx }
%lex-param { void * scanner }

%start begin

%{
#include "sonia/config.hpp"
#include <iostream>
#include "parser.hpp"

using namespace sonia;
using namespace sonia::lang::bang;

#include "bang.tab.hpp"

using YYSTYPE = bang_lang::parser::semantic_type;
using YYLTYPE = bang_lang::parser::location_type;

int bang_langlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param, parser_context & ctx, void* yyscanner);
//#include "bang.yy.hpp"

void bang_lang::parser::error(const location_type& loc, const std::string& msg)
{
    ctx.append_error(("%1%(%2%,%3%-%4%,%5%): error: %6%"_fmt
		% ctx.get_resource()
		% loc.begin.line % loc.begin.column % loc.end.line % loc.end.column % msg).str());
}

#define IGNORE(...)

%}

%token END 0 "end of file"

%token COMMENT_BEGIN
%token COMMENT_END

%token <sonia::lang::bang::annotated_string_view> STRING IDENTIFIER INTERNAL_IDENTIFIER RESERVED_IDENTIFIER
%token <sonia::lang::bang::annotated_decimal> INTEGER
%token <sonia::lang::bang::annotated_decimal> DECIMAL
%token <sonia::string_view> OPERATOR_TERM

%token <sonia::lang::lex::resource_location> ASSIGN               "`=`"

%token AT_SYMBOL            "`@`"
%token UNDERSCORE           "`_`"
%token ARROWAST             "`->*`"
%token ARROW                "`->`"
%token FARROW               "`~>`"
%token PTAST                "`.*`"

%token DBLPLUS              "`++`"
%token DBLMINUS             "`--`"
%token LEFTSHIFT            "`<<`"
%token RIGHTSHIFT           "`>>`"
%token LE                   "`<=`"
%token GE                   "`>=`"
%token <sonia::lang::lex::resource_location> EQ                   "`==`"
%token <sonia::lang::lex::resource_location> NE                   "`!=`"
%token <sonia::lang::lex::resource_location> LOGIC_AND            "`&&`"
%token <sonia::lang::lex::resource_location> LOGIC_OR             "`||`"
%token <sonia::lang::lex::resource_location> CONCAT               "`..`"
%token <sonia::lang::lex::resource_location> ELLIPSIS "`...`"
%token ADDASSIGN            "`+=`"
%token SUBASSIGN            "`-=`"
%token MULASSIGN            "`*=`"
%token DIVASSIGN            "`/=`"
%token MODASSIGN            "`%=`"
%token LSHIFTSSIGN          "`<<=`"
%token RSHIFTSSIGN          "`>>=`"
%token BITANDASSIGN         "`&=`"
%token BITORASSIGN          "`|=`"
%token BITXORASSIGN         "`^=`"

%token COLON                "`:`"
%token DBLCOLON             "`::`"
%token <sonia::lang::lex::resource_location> OPEN_PARENTHESIS     "`(`"
%token CLOSE_PARENTHESIS    "`)`"
%token <sonia::lang::lex::resource_location> OPEN_BRACE           "`{`"
%token CLOSE_BRACE			"`}`"
%token <sonia::lang::lex::resource_location> OPEN_SQUARE_BRACKET  "`[`"
%token CLOSE_SQUARE_BRACKET "`]`"
%token OPEN_BROKET          "`<`"
%token CLOSE_BROKET         "`>`"
%token END_STATEMENT		"`;`"
%token <sonia::lang::lex::resource_location> POINT                "`.`"
%token <sonia::lang::lex::resource_location> PLUS                 "`+`"
%token MINUS                "`-`"
%token ASTERISK             "`*`"
%token SLASH                "`/`"
%token PERCENT              "`%`"
%token AMPERSAND            "`&`"
%token BITOR                "`|`"
%token <sonia::lang::lex::resource_location> EXCLPT     "`!`" 
%token TILDA                "`~`"
%token EXCL                 "`^`"
%token COMMA                ","
%token DOLLAR               "`$`"
%token QMARK                "`?`"
%token HASHTAG              "`#`"

%token LET
%token VAR
%token RETURN
%token EXTERN

%token FOR
%token VOID_
%token AUTO
%token CONST_
%token USING
%token THROW
%token DELETE_
%token SIZEOF


// 16 priority
//%left COMMA

// 15 priority
//%right ASSIGN               "`=`"
//%right COLON ADDASSIGN SUBASSIGN MULASSIGN MODASSIGN DIVASSIGN LSHIFTSSIGN RSHIFTSSIGN BITANDASSIGN BITORASSIGN BITXORASSIGN THROW

// 14 priority
//%left LOGIC_OR

// 13 priority
//%left LOGIC_AND

// 12 priority
//%left BITOR

// 11 priority
//%left EXCL

// 10 priority
//%left AMPERSAND

// 9 priority
//%left EQ NE

// 8 priority
//%left OPEN_BROKET LE CLOSE_BROKET GE
//%left OPEN_BROKET LE GE

// 7 priority
//%left RIGHTSHIFT LEFTSHIFT

// 5 priority
//%left ASTERISK SLASH PERCENT

// 4 priority
//%left PTAST ARROWAST

// 3 priority
//%right PREFIXDBLPLUS PREFIXDBLMINUS PREFIXPLUS PREFIXMINUS EXCLPT TILDA CAST DEREF ADDRESS SIZEOF

// 2 priority
//%left DBLPLUS DBLMINUS OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET POINT

// 1 priority
//%left IDENTIFIER
////////////////////////////////////////////////////////////////////////

%left LOWEST

// 15 priority
%right ASSIGN
//%left COLON

// 14 priority
%left LOGIC_OR

// 13 priority
%left LOGIC_AND

// 12 priority
%left BITOR

// 9 priority
%left EQ NE

// 6 priority
%left CONCAT
%left PLUS MINUS

// 3 priority
%right EXCLPT

// 2 priority
%left OPEN_BRACE OPEN_PARENTHESIS OPEN_SQUARE_BRACKET ARROW POINT
%right QMARK

%left DBLCOLON

// 1 priority
%right IDENTIFIER INTERNAL_IDENTIFIER RESERVED_IDENTIFIER






// DECLARATIONS
%token INCLUDE
%type <declaration_set_t> declaration_any
%type <generic_declaration_t> generic-decl
%type <let_statement_decl_t> let-decl-start let-decl-start-with-opt-type let-decl

%type <std::vector<infunction_declaration_t>> infunction_declaration_any
%type <infunction_declaration_t> opt-infunction-decl

//%type <identifier_chain_t> identifier-chain

// QNAME
%type <sonia::lang::bang::annotated_identifier> identifier
%type <sonia::lang::bang::annotated_qname> qname


// FUNCTIONS
%token INLINE
%token <sonia::lang::lex::resource_location> FN "`fn`"
%type <fn_kind> fn-start-decl
%type <fn_pure_t> fn-decl

// ENUMERATIONS
%token ENUM
%type <enum_decl> enum-decl
%type <std::vector<sonia::lang::bang::identifier>> case-list-opt case-list
%type <sonia::lang::bang::identifier> case-decl

// TYPES
%token TYPE
%token EXTENDS
%type <type_decl> type-decl
%type <extension_list_t> type-extension-any type-extension-list
%type <field_list_t> field-list field-list-opt
%type <field_t> field-decl
%type <parameter_t> parameter-decl
%type <parameter_name> parameter-name-decl





%type <parameter_constraint_modifier_t> parameter-constraint-modifier parameter-constraint-modifier-opt
%type <parameter_constraint_set_t> parameter-constraint-set
    %type <syntax_expression_t> syntax-expression syntax-expression-wo-ii concept-expression

%type <parameter_woa_list_t> parameter-list-opt parameter-list // for unification, empty assignment
%type <parameter_woa_list_t> parameter-woa-list-opt parameter-woa-list
%type <parameter_woa_t> parameter-woa-decl // with optional assignement

// TYPE EXPRESSIONS
%token TYPENAME
//%token BOOL
%token INT
%token FLOAT
//%token STRING_WORD
//%token DECIMAL_WORD "reserved word `decimal`"
%token WEAK "weak modifier"
%token CONST "const modifier"

%type <bang_preliminary_type> type-expr
//%type<bang_preliminary_tuple_t> opt-type-list

// STATEMENTS


// EXPRESSIONS
%token <sonia::lang::lex::resource_location> TRUE "true"
%token <sonia::lang::lex::resource_location> FALSE "false"

%type <syntax_expression_t> expression compound-expression
%type <named_expression_term_list_t> opt-named-expr-list-any opt-named-expr-list
%type <named_expression_term_t> opt-named-expr
%type <expression_list_t> expression-list-any

//%type <statement_list_t> statement-list-any statement-list
//%type <statement_t> base-statement statement

//%type <named_expression_term_t> named-arg
//%type <named_expression_term_list_t> arg-list arg-list-not-empty
//%type <function_def> function-def



////%type <type_expression_t> type-expr 
////%type <type_expression_list_t> opt-type-expr-list type-expr-list-not-empty
////%type <type_modifier> type-modifier

////%type <ct_expression_ptr_t> ct_expression ct-function-call
////%type <std::vector<ct_expression_ptr_t>> ct_expression_list ct_expression_list_not_empty
////%type <statement_ptr_t> statement
////%type <std::vector<statement_ptr_t>>

%destructor { } <*>
%printer { } <*>

%%

begin:
	declaration_any END { ctx.set_declarations(std::move($1)); }
	;

declaration_any:
      %empty
        { $$ = {}; }
    | declaration_any END_STATEMENT
        { $$ = std::move($1); }
	| declaration_any generic-decl
        {
	        $$ = std::move($1);
            $$.emplace_back(std::move($2));
        }
/*
    |
    NAMESPACE nsname
        {
            ctx.push_ns(std::move($2));
        }
        OPEN_BRACE declaration_any CLOSE_BRACE
        {
            $$ = std::move($5);
            ctx.pop_ns();
        }
*/
    ;

generic-decl:
      EXTERN VAR identifier COLON type-expr END_STATEMENT
        { $$ = extern_var{ std::move($3), std::move($5) }; }
    | EXTERN FN fn-decl END_STATEMENT
        { $$ = std::move($3); IGNORE($2); }
    | INCLUDE STRING
        { $$ = include_decl{ctx.make_string(std::move($2)) }; }
    | enum-decl
        { $$ = std::move($1); }
    //| type-decl
    //    { $$ = std::move($1); }
    | fn-start-decl fn-decl OPEN_BRACE infunction_declaration_any CLOSE_BRACE
        { $2.kind = $1; $$ = fn_decl_t{std::move($2), std::move($4)}; IGNORE($3); }
    | let-decl
        { $$ = std::move($1); }
    | compound-expression END_STATEMENT
        { $$ = expression_decl_t{ std::move($1) }; }

/*
    | EXTERN FN IDENTIFIER OPEN_PARENTHESIS arg-list CLOSE_PARENTHESIS ARROW expr END_STATEMENT
        { $$ = extern_function_decl{ctx.make_identifier($3), function_def{std::move($5), std::move($8)}}; }
	| function-decl
        { $$ = $1; }
    | using-decl
        { $$ = $1; }
*/
	;
 
let-decl :
      let-decl-start-with-opt-type END_STATEMENT
        { $$ = std::move($1); }
    | let-decl-start-with-opt-type ASSIGN expression END_STATEMENT
        { $$ = std::move($1); $$.expression = std::move($3); IGNORE($2); }
    ;

let-decl-start :
      LET identifier
        { $$ = let_statement_decl_t{ std::move($2), {}, {}, false }; }
    | LET WEAK identifier
        { $$ = let_statement_decl_t{ std::move($3), {}, {}, true }; }
    ;

let-decl-start-with-opt-type :
      let-decl-start
    | let-decl-start COLON type-expr
        { $$ = std::move($1); $$.type = std::move($3); }
    ;

infunction_declaration_any:
      %empty
        { $$ = {}; }
    | infunction_declaration_any END_STATEMENT
        { $$ = std::move($1); }
	| infunction_declaration_any opt-infunction-decl
        {
	        $$ = std::move($1);
            $$.emplace_back(std::move($2));
        }
    ;

opt-infunction-decl:
      let-decl
        { $$ = std::move($1); }
    | RETURN expression END_STATEMENT
        { $$ = return_decl_t{ std::move($2) }; }
    | compound-expression END_STATEMENT
        { $$ = expression_decl_t{ std::move($1) }; }
    ;

identifier:
    IDENTIFIER
      { $$ = ctx.make_identifier($1); }
/*  
	| DOLLAR OPEN_PARENTHESIS ct-expression CLOSE_PARENTHESIS
*/  
	;

///////////////////////////////////////////////// QNAME
qname:
    DBLCOLON identifier
        { $$ = annotated_qname{ qname{$2.value}, std::move($2.location) }; }
    | identifier
        { $$ = annotated_qname{ qname{$1.value, false}, std::move($1.location) }; }
    | qname DBLCOLON identifier
        { $$ = std::move($1); $$.value.append(std::move($3.value)); }
    ;

///////////////////////////////////////////////// FUNCTIONS
fn-start-decl:
      FN
        { $$ = fn_kind::DEFAULT; IGNORE($1); }
    | INLINE FN
        { $$ = fn_kind::INLINE; IGNORE($2); }
    ;

fn-decl:
      qname OPEN_PARENTHESIS parameter-list-opt CLOSE_PARENTHESIS
        { $$ = fn_pure_t{ std::move($1), std::move($3), nullopt }; IGNORE($2); }
    | qname OPEN_PARENTHESIS parameter-list-opt CLOSE_PARENTHESIS ARROW syntax-expression
        { $$ = fn_pure_t{ std::move($1), std::move($3), std::move($6) }; IGNORE($2); }
    ;

///////////////////////////////////////////////// ENUMERATIONS
enum-decl:
    ENUM qname OPEN_BRACE case-list-opt CLOSE_BRACE
    {
        $$ = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move($2.value), $2.location}), std::move($4)}; IGNORE($3);
    }
    ;

case-list-opt:
    %empty { $$ = {}; }
    |
    case-list
    ;

case-list:
    case-decl
        { $$ = std::vector<sonia::lang::bang::identifier>{std::move($1)}; }
    |
    case-list COMMA case-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

case-decl:
    identifier
        { $$ = $1.value; }
    ;
///////////////////////////////////////////////// TYPES
type-decl:
    TYPE qname type-extension-any OPEN_BRACE parameter-woa-list-opt CLOSE_BRACE
    {
        $$ = type_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move($2.value), $2.location}), std::move($3), std::move($5)}; IGNORE($4);
    }
    ;

type-extension-any:
    %empty { $$ = {}; }
    |
    EXTENDS type-extension-list { $$ = std::move($2); }
    ;

type-extension-list:
    qname
        { $$ = extension_list_t{ctx.make_qname_identifier(std::move($1))}; }
    |
    type-extension-list COMMA qname
        { $$ = std::move($1); $$.emplace_back(ctx.make_qname_identifier(std::move($3))); }
    ;

field-list-opt:
       %empty { $$ = {}; }
    | field-list
    ;

field-list:
      field-decl
        { $$ = field_list_t{std::move($1)}; }
    | field-list COMMA field-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

field-decl:
    IDENTIFIER COLON type-expr
        { $$ = field_t{ ctx.make_identifier($1), std::move($3) }; }
    | type-expr
        { $$ = field_t{ std::move($1) }; }
    ;

parameter-name-decl:
      IDENTIFIER INTERNAL_IDENTIFIER COLON
        { $$ = parameter_name{ctx.make_identifier($1), ctx.make_identifier($2)}; }
    | IDENTIFIER COLON
        { $$ = parameter_name{ctx.make_identifier($1), nullopt}; }
    | INTERNAL_IDENTIFIER COLON
        { $$ = parameter_name{nullopt, ctx.make_identifier($1)}; }
    ;

parameter-constraint-modifier:
      TYPENAME
        { $$ = parameter_constraint_modifier_t::typename_constraint; }
    | CONST
        { $$ = parameter_constraint_modifier_t::value_constraint; }
    //| %empty
    //    { $$ = parameter_constraint_modifier_t::value_type_constraint; }
    ;

parameter-constraint-modifier-opt:
      parameter-constraint-modifier
    | %empty
        { $$ = parameter_constraint_modifier_t::value_type_constraint; }
    ;

// one optional syntax-expression, and arbitrary count of concept-expression or internal idetifiers
parameter-constraint-set:
      syntax-expression-wo-ii
        { $$ = parameter_constraint_set_t{ std::move($1), {}, {} }; }
    | INTERNAL_IDENTIFIER
        { $$ = parameter_constraint_set_t{ nullopt, {}, { ctx.make_identifier(std::move($1)) } }; }
    | concept-expression
        { $$ = parameter_constraint_set_t{ nullopt, {std::move($1)}, {} }; }
    | parameter-constraint-set INTERNAL_IDENTIFIER
        { $$ = std::move($1); $$.bindings.emplace_back(ctx.make_identifier(std::move($2))); }
    | parameter-constraint-set concept-expression
        { $$ = std::move($1); $$.concepts.emplace_back(std::move($2)); }
    ;

concept-expression:
    AT_SYMBOL qname
        { $$ = syntax_expression_t{ std::move($2) }; }
    ;

// without internal identifiers
syntax-expression-wo-ii:
     RESERVED_IDENTIFIER
        { $$ = variable_identifier{ctx.make_qname(std::move($1)), true}; }
    | qname
        { $$ = variable_identifier{ std::move($1) }; }
    | syntax-expression ELLIPSIS
        { $$ = bang_parameter_pack_t{std::move($1)}; IGNORE($2); }
    ;

syntax-expression:
      INTERNAL_IDENTIFIER
        { $$ = variable_identifier{ctx.make_qname(std::move($1)), true}; }
    | syntax-expression-wo-ii
    ;

    /*
syntax-expression-wii:
      INTERNAL_IDENTIFIER
        { $$ = variable_identifier{ctx.make_qname_identifier(std::move($1), false), true}; }
    | syntax-expression
    ;
    */
/*
parameter-constraint-decl:
      CONST type-expr ELLIPSIS
        { $$ = parameter_type_t{bang_preliminary_parameter_pack_t{std::move($2)}, true}; IGNORE($3); }
    | CONST type-expr
        { $$ = parameter_type_t{std::move($2), true}; }
    | type-expr ELLIPSIS
        { $$ = parameter_type_t{bang_preliminary_parameter_pack_t{std::move($1)}, false}; IGNORE($2); }
    | type-expr
        { $$ = parameter_type_t{std::move($1), false}; }
    ;
*/

parameter-decl:
      parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
        { $$ = parameter_t{ std::move($1), std::move($2), std::move($3) }; }
    | parameter-constraint-modifier parameter-constraint-set
        { $$ = parameter_t{ {}, std::move($1), std::move($2) }; }
    | parameter-constraint-set
        { $$ = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, std::move($1) }; }
    ;

parameter-list-opt:
       %empty { $$ = {}; }
    |  parameter-list
    ;

parameter-list:
      parameter-decl
        { $$ = parameter_woa_list_t{ parameter_woa_t{std::move($1)} }; }
    | parameter-list COMMA parameter-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

parameter-woa-list-opt:
       %empty { $$ = {}; }
    |  parameter-woa-list
    ;

parameter-woa-list:
      parameter-woa-decl
        { $$ = parameter_woa_list_t{std::move($1)}; }
    | parameter-woa-list COMMA parameter-woa-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

parameter-woa-decl:
      parameter-decl
        { $$ = parameter_woa_t { std::move($1) }; }
    | parameter-decl ASSIGN expression
        { $$ = parameter_woa_t{ std::move($1), std::move($3) }; IGNORE($2); }
    ;

// TYPE EXPRESSIONS
type-expr:
      qname
        { $$ = std::move($1); }
    | qname OPEN_PARENTHESIS opt-named-expr-list-any CLOSE_PARENTHESIS
        { $$ = std::move($1); IGNORE($2); IGNORE($3); }
    | INTERNAL_IDENTIFIER
        { $$ = ctx.make_identifier(std::move($1)); }
    | INTERNAL_IDENTIFIER OPEN_PARENTHESIS opt-named-expr-list-any CLOSE_PARENTHESIS
        { $$ = ctx.make_identifier(std::move($1)); IGNORE($2); IGNORE($3); }
    | OPEN_SQUARE_BRACKET type-expr CLOSE_SQUARE_BRACKET
        { $$ = bang_preliminary_vector_t{std::move($2)}; IGNORE($1); }
    | OPEN_PARENTHESIS field-list-opt CLOSE_PARENTHESIS
        { $$ = bang_preliminary_tuple_t { std::move($2) }; IGNORE($1); }
    | type-expr OPEN_SQUARE_BRACKET INTEGER CLOSE_SQUARE_BRACKET
        { $$ = bang_preliminary_array_t{std::move($1), (size_t)$3.value}; IGNORE($2); }
    | type-expr BITOR type-expr
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move($1));
            uni.members.emplace_back(std::move($3));
            $$ = std::move(uni);
        }
    | type-expr ARROW type-expr
        { $$ = bang_preliminary_fn_t{std::move($1), std::move($3)}; }
    ;

    /*
opt-type-list:
    %empty
        { $$ = bang_preliminary_tuple_t{}; }
    | type-expr
        { $$ = bang_preliminary_tuple_t{}; $$.fields.emplace_back(std::move($1)); }
    | opt-type-list COMMA type-expr
        { $$ = std::move($1); $$.fields.emplace_back(std::move($3)); }
    ;
    */

/////////////////////////// EXPRESSIONS
compound-expression:
      expression OPEN_PARENTHESIS opt-named-expr-list-any CLOSE_PARENTHESIS
        { $$ = function_call_t{ std::move($2), std::move($1), std::move($3) }; }
    | expression OPEN_BRACE opt-named-expr-list-any CLOSE_BRACE
        { 
           $$ = function_call_t{ std::move($2), std::move($1), std::move($3) };
            //$$ = function_call_t{}; IGNORE($1, $2, $3);
        }
    //| expression ASSIGN expression
    //    { $$ = binary_expression_t{ binary_operator_type::ASSIGN, std::move($1), std::move($3), std::move($2) }; }
    ;

expression:
      TRUE
        { $$ = annotated_bool{true, $1}; }
    | FALSE
        { $$ = annotated_bool{false, $1}; }
    | INTEGER
        { $$ = std::move($1); }
    | DECIMAL
        { $$ = std::move($1); }
    | STRING
        { $$ = ctx.make_string(std::move($1)); }
    | POINT identifier
        { $$ = case_expression { std::move($2), std::move($1) }; }
    | qname
        { $$ = variable_identifier{ std::move($1), false}; }
    | INTERNAL_IDENTIFIER
        { $$ = variable_identifier{ ctx.make_qname(std::move($1)), true }; }
    | RESERVED_IDENTIFIER
        { $$ = variable_identifier{ ctx.make_qname(std::move($1)), true }; }
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
        { $$ = std::move($2); IGNORE($1); }
    //| FN OPEN_PARENTHESIS parameter-woa-list-opt CLOSE_PARENTHESIS OPEN_BRACE infunction_declaration_any CLOSE_BRACE
    //    { $$ = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move($2)}, std::move($3), nullopt, std::move($6), std::move($1)}; IGNORE($5); }
    //| OPEN_PARENTHESIS parameter-woa-list-opt CLOSE_PARENTHESIS OPEN_BRACE infunction_declaration_any CLOSE_BRACE
    //    { $$ = lambda_t{annotated_qname{qname{ctx.new_identifier()}, $1}, std::move($2), nullopt, std::move($5), $1}; IGNORE($4); }
    //| FN OPEN_PARENTHESIS parameter-woa-list-opt CLOSE_PARENTHESIS ARROW type-expr OPEN_BRACE infunction_declaration_any CLOSE_BRACE
    //    { $$ = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move($2)}, std::move($3), std::move($6), std::move($8), std::move($1)}; IGNORE($7); }
    | OPEN_SQUARE_BRACKET expression-list-any CLOSE_SQUARE_BRACKET
        { $$ = expression_vector_t{ {std::move($2)}, std::move($1) }; }
    | EXCLPT expression
		{ $$ = unary_expression_t{ unary_operator_type::NEGATE, true /* prefixed */, std::move($2), std::move($1) }; }
    | expression QMARK
        { $$ = not_empty_expression_t{ std::move($1) }; }
    | expression POINT identifier
        { $$ = member_expression_t { std::move($1), std::move($3) }; IGNORE($2); }
    //| expression QMARK POINT identifier
    //    { $$ = member_expression_t { std::move($1), std::move($4), true }; IGNORE($3); }
    | expression LOGIC_AND expression
        { $$ = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move($1), std::move($3), std::move($2) }; }
	| expression LOGIC_OR expression
        { $$ = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move($1), std::move($3), std::move($2) }; }
    | expression PLUS expression
        { $$ = binary_expression_t{ binary_operator_type::PLUS, std::move($1), std::move($3), std::move($2) }; }
    | expression CONCAT expression
        { $$ = binary_expression_t{ binary_operator_type::CONCAT, std::move($1), std::move($3), std::move($2) }; }

//////////////////////////// 9 priority
    | expression EQ expression
        { $$ = binary_expression_t{ binary_operator_type::EQ, std::move($1), std::move($3), std::move($2) }; }
    | expression NE expression
        { $$ = binary_expression_t{ binary_operator_type::NE, std::move($1), std::move($3), std::move($2) }; }

    | compound-expression
        { $$ = std::move($1); }

    /*
    | qname OPEN_BROKET opt-named-expr-list CLOSE_BROKET
        { $$ = syntax_expression_t { ctprocedure{ std::move($1), std::move($3) } }; }
    
    */
    ;

expression-list-any:
    %empty
        { $$ = expression_list_t{}; }
    | expression
        { $$ = expression_list_t{ std::move($1) }; }
    | expression-list-any COMMA expression
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

opt-named-expr-list-any:
    %empty
        { $$ = named_expression_term_list_t{}; }
    | opt-named-expr-list
    ;

opt-named-expr-list:
      opt-named-expr
        { $$ = named_expression_term_list_t{std::move($1)}; }
    | opt-named-expr-list COMMA opt-named-expr
        {
            $$ = std::move($1);
            $$.push_back(std::move($3));
		}
	;

opt-named-expr:
      qname COLON expression
        { $1.value.set_absolute(); $$ = named_expression_term_t{ std::tuple{std::move($1), std::move($3)} }; }
    | expression
        { $$ = named_expression_term_t{ std::move($1) }; }
    ;

//identifier-chain:
//      identifier
//        { $$ = identifier_chain_t{ std::move($1) }; }
//    | identifier-chain POINT identifier
//        { $$ = std::move($1); $1.emplace_back(std::move($3)); }
//    ;

/*
using-decl:
    USING identifier ASSIGN expr END_STATEMENT
        { $$ = using_decl{ctx.ns(), $2, std::move($4) }; }
    ;

statement:
      base-statement
    | FOR OPEN_PARENTHESIS base-statement expr END_STATEMENT expr CLOSE_PARENTHESIS OPEN_BRACE statement-list-any CLOSE_BRACE
        { $$ =  sonia::make_shared<for_statement_t>(std::move($3), std::move($4), std::move($6), std::move($9)); }
    | RETURN expr END_STATEMENT
        { $$ =  return_statement_t { std::move($2) }; }
    ;
*/
/*
   expr-with-comma END_STATEMENT
        { $$ = std::move($1); }
   |
   RETURN expr-with-comma END_STATEMENT
        { $$ = return_statement_t{std::move($2)}; }
   ;
*/
/*

arg-list:
	  %empty
        { $$ = named_expression_term_list_t{}; }
	| arg-list-not-empty
	;

arg-list-not-empty:
	  named-arg
        { $$ = named_expression_term_list_t{std::move($1)}; }
	| arg-list-not-empty COMMA named-arg
        { $$ = std::move($1); $$.push_back(std::move($3)); }
	;

named-arg:
	identifier COLON expr
        { $$ = std::pair{std::move($1), $3}; }
    | HASHTAG IDENTIFIER COLON expr
        { $$ = std::pair{ctx.make_required_identifier($2), $4}; }
	;
*/
/*
type-expr:
      qname
        { $$ = std::move($1); }
    | qname OPEN_BROKET opt-type-expr-list CLOSE_BROKET
        { $$ = ctprocedure2{ std::move($1), std::move($3) }; }
    
    ;


opt-type-expr-list:
    %empty
        { $$ = type_expression_list_t{}; }
	| type-expr-list-not-empty
	;

type-expr-list-not-empty:
	  type-expr
        { $$ = type_expression_list_t{std::move($1)}; }
	| type-expr-list-not-empty COMMA type-expr
        { $$ = std::move($1); $$.push_back(std::move($3)); }
	;
*/
/*
    CONST_ IDENTIFIER opt-type-modifiers
        {
            $$ = type_expression_list_t{type_modifier::CONST_MODIFIER, ctx.make_identifier($2)};
            $$.insert($$.end(), std::make_move_iterator($3.begin()), std::make_move_iterator($3.end()));
        }
    | IDENTIFIER opt-type-modifiers
        {
            $$ = type_expression_list_t{ctx.make_identifier($1)};
            $$.insert($$.end(), std::make_move_iterator($2.begin()), std::make_move_iterator($2.end()));
        }
    | OPEN_PARENTHESIS type-expr CLOSE_PARENTHESIS
        {
            $$ = $2;
        }
    | type-expr ARROW type-expr
        {
            $$ = $1;
            $$.push_back(std::move($3));
        }
    ;

opt-type-modifiers:
    %empty
        { $$ = type_expression_list_t{}; }
    | type-modifiers
        { $$ = std::move($1); }
    ;
  
type-modifiers:
      type-modifier
        { $$ = type_expression_list_t{$1}; }
    | type-modifiers type-modifier
        { $$ = std::move($1); $$.push_back($2); }
    ;
    

type-modifier:
      CONST { $$ = type_modifier::CONST_MODIFIER; }
    | ASTERISK { $$ = type_modifier::POINTER_MODIFIER; }
    | AMPERSAND { $$ = type_modifier::REFERENCE_MODIFIER; }
    | AND { $$ = type_modifier::RVALUE_REFERENCE_MODIFIER; }
    ;
*/

/*
nsname:
    DBLCOLON
        {
            $$ = qname();
        }
    |
    qname
    ;


*/






//////////////
/*
expr:
      VOID_
        { $$ = expression_list{ctx.make_identifier("void")}; }
    | IDENTIFIER
        { $$ = expression_list{ctx.make_identifier($1)}; }
    | expr DBLCOLON IDENTIFIER
        {
            expression_list arg{ctx.make_identifier($3)};
            $$ = handle_call_op($1, arg, operator_type::DBLCOLON);
        }
    | DECIMAL
        { $$ = expression_list{ctx.make_numeric($1)}; }
    | STRING
        { $$ = expression_list{ctx.make_string_literal($1)}; }
    | expr CLOSE_BROKET %prec IDENTIFIER
        { $$ = std::move($1); $$.push_back(operator_type::POSTFIX_CLOSE_BROKET); }
    | expr RIGHTSHIFT %prec IDENTIFIER
        { $$ = std::move($1); $$.push_back(operator_type::POSTFIX_RIGHTSHIFT); }
    | OPEN_PARENTHESIS expr-with-comma CLOSE_PARENTHESIS
        { $$ = std::move($2); }
	| expr DBLPLUS
		{ $$ = std::move($1); $$.push_back(operator_type::POSTFIX_INCREMENT); }
	| expr DBLMINUS
		{ $$ = std::move($1); $$.push_back(operator_type::POSTFIX_DECREMENT); }
	| expr OPEN_PARENTHESIS opt-expr CLOSE_PARENTHESIS
		{ $$ = expression_list{function_t{call_type::FUNCTION_CALL, std::move($1), std::move($3)}}; }
	| expr OPEN_SQUARE_BRACKET opt-expr CLOSE_SQUARE_BRACKET
        { $$ = expression_list{function_t{call_type::SUBSCRIPT, std::move($1), std::move($3)}}; }
	| expr POINT expr
		{ $$ = handle_call_op($1, $3, operator_type::PERIOD); }
	| expr ARROW expr
        { $$ = handle_call_op($1, $3, operator_type::ARROW); }

    //////////////////////////// 3 priority
    
	| DBLPLUS expr %prec PREFIXDBLPLUS
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_INCREMENT); }
	| DBLMINUS expr %prec PREFIXDBLMINUS
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_DECREMENT); }
	| PLUS expr %prec PREFIXPLUS
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_PLUS); }
	| MINUS expr %prec PREFIXMINUS
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_MINUS); }
	| EXCLPT expr
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_INV); }
	| TILDA expr
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_BITINV); }
	| OPEN_PARENTHESIS expr-with-comma CLOSE_PARENTHESIS expr %prec CAST
        { $$ = handle_call_op($2, $4, operator_type::TYPE_CAST); }
    | ASTERISK expr %prec DEREF
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_DEREF); }
    | AMPERSAND expr %prec ADDRESS
		{ $$ = std::move($2); $$.push_back(operator_type::PREFIX_ADDRESS); }
	| SIZEOF expr
		{ $$ = std::move($2); $$.push_back(operator_type::SIZEOF); }

    //////////////////////////// 4 priority
    
    | expr PTAST expr
        { $$ = handle_call_op($1, $3, operator_type::MBPTR); }
    | expr ARROWAST expr
        { $$ = handle_call_op($1, $3, operator_type::PTRMBPTR); }

    //////////////////////////// 5 priority
    
    | expr ASTERISK expr
        { $$ = handle_call_op($1, $3, operator_type::MUL); }
    | expr SLASH expr
        { $$ = handle_call_op($1, $3, operator_type::DIV); }
    | expr PERCENT expr
        { $$ = handle_call_op($1, $3, operator_type::MOD); }

    //////////////////////////// 6 priority
    
	| expr PLUS expr
        { $$ = handle_call_op($1, $3, operator_type::PLUS); }
	| expr MINUS expr
        { $$ = handle_call_op($1, $3, operator_type::MINUS); }

    //////////////////////////// 7 priority
    
	| expr LEFTSHIFT expr
        { $$ = handle_call_op($1, $3, operator_type::LEFTSHIFT); }
	| expr RIGHTSHIFT expr
        { $$ = handle_call_op($1, $3, operator_type::RIGHTSHIFT); }

    //////////////////////////// 8 priority

	| expr OPEN_BROKET expr
        { $$ = handle_call_op($1, $3, operator_type::LESS); }
    | expr LE expr
        { $$ = handle_call_op($1, $3, operator_type::LE); }
    | expr CLOSE_BROKET expr
        { $$ = handle_call_op($1, $3, operator_type::GREATER); }
    | expr GE expr
        { $$ = handle_call_op($1, $3, operator_type::GE); }

    //////////////////////////// 9 priority

	| expr EQ expr
        { $$ = handle_call_op($1, $3, operator_type::EQ); }
    | expr NE expr
        { $$ = handle_call_op($1, $3, operator_type::NE); }

    //////////////////////////// 10 priority

	| expr AMPERSAND expr
        { $$ = handle_call_op($1, $3, operator_type::BITAND); }

    //////////////////////////// 11 priority

	| expr EXCL expr
        { $$ = handle_call_op($1, $3, operator_type::BITXOR); }

    //////////////////////////// 12 priority

	| expr BITOR expr
        { $$ = handle_call_op($1, $3, operator_type::BITOR); }

    //////////////////////////// 14 priority

	| expr AND expr
        { $$ = handle_call_op($1, $3, operator_type::AND); }

    //////////////////////////// 14 priority

	| expr OR expr
        { $$ = handle_call_op($1, $3, operator_type::OR); }

    //////////////////////////// 15 priority

    | expr QMARK expr COLON expr
        {
			$$ = std::move($1);
			$$.insert($$.end(), std::make_move_iterator($3.begin()), std::make_move_iterator($3.end()));
            $$.insert($$.end(), std::make_move_iterator($5.begin()), std::make_move_iterator($5.end()));
			$$.push_back(operator_type::IFF);
		}
    | expr ASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::ASSIGN); }
    | expr ADDASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::ADDASSIGN); }
    | expr SUBASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::SUBASSIGN); }
    | expr MULASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::MULASSIGN); }
    | expr DIVASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::DIVASSIGN); }
    | expr MODASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::MODASSIGN); }
    | expr LSHIFTSSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::LSHIFTSSIGN); }
    | expr RSHIFTSSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::RSHIFTSSIGN); }
    | expr BITANDASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::BITANDASSIGN); }
    | expr BITORASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::BITORASSIGN); }
    | expr BITXORASSIGN expr
        { $$ = handle_call_op($1, $3, operator_type::BITXORASSIGN); }
    | THROW expr
        {
			$$ = std::move($2);
			$$.push_back(operator_type::THROW);
		}
        
//	| expr COMMA expr
//        {
//            $$ = std::move($1);
//            $$.insert($$.end(), std::make_move_iterator($3.begin()), std::make_move_iterator($3.end()));
//            $$.push_back(operator_type::COMMA);
//		}

	;
*/
    //////////////////////////// 16 priority
    
    /*
expr-with-comma:
      expr
    | expr-with-comma COMMA expr
        {
            $$ = std::move($1);
            $$.insert($$.end(), std::make_move_iterator($3.begin()), std::make_move_iterator($3.end()));
            $$.push_back(operator_type::COMMA);
		}
	;
    
opt-expr:
      %empty { $$ = expression_list{}; }
    | expr-with-comma
    ;
    */
%%
