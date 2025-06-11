/*=============================================================================
    Bang(c) by Alexander A Pototskiy
==============================================================================*/
%require "3.2"
%language "c++"
//%debug
%define api.value.type variant
//%define api.value.automove
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

#include "unit.hpp"
#include "sonia/bang/utility/linked_list.ipp"

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

#define IGNORE_TERM(...)



%}

%token END 0 "end of file"

%token COMMENT_BEGIN
%token COMMENT_END

%token <sonia::lang::bang::annotated_string_view> STRING IDENTIFIER INTERNAL_IDENTIFIER RESERVED_IDENTIFIER INTEGER_INDEX
%token <sonia::lang::bang::annotated_integer> INTEGER
%token <sonia::lang::bang::annotated_decimal> DECIMAL DECIMAL_S
%token <sonia::string_view> OPERATOR_TERM

%token <sonia::lang::lex::resource_location> ASSIGN               "`=`"

%token APOSTROPHE           "APOSTROPHE"
%token AT_SYMBOL            "`@`"
%token <sonia::lang::lex::resource_location> UNDERSCORE           "`_`"
%token ARROWAST             "`->*`"
%token ARROW                "`->`"
%token ARROWEXPR            "`=>`"
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
%token <sonia::lang::lex::resource_location> OPEN_PARENTHESIS       "`(`"
%token CLOSE_PARENTHESIS    "`)`"
%token <sonia::lang::lex::resource_location> OPEN_BRACE             "`{`"
%token CLOSE_BRACE			"`}`"
%token <sonia::lang::lex::resource_location> OPEN_SQUARE_BRACKET    "`[`"
%token CLOSE_SQUARE_BRACKET "`]`"
%token <sonia::lang::lex::resource_location> OPEN_SQUARE_DBL_BRACKET "`[[`"
%token CLOSE_SQUARE_DBL_BRACKET "`]]`"
%token OPEN_BROKET          "`<`"
%token CLOSE_BROKET         "`>`"
%token END_STATEMENT		"`;`"
%token <sonia::lang::lex::resource_location> POINT      "`.`"
%token <sonia::lang::lex::resource_location> PLUS       "`+`"
%token <sonia::lang::lex::resource_location> MINUS      "`-`"
%token <sonia::lang::lex::resource_location> ASTERISK   "`*`"
%token SLASH                "`/`"
%token PERCENT              "`%`"
%token <sonia::lang::lex::resource_location> AMPERSAND  "`&`"
%token <sonia::lang::lex::resource_location> BITOR      "`|`"
%token <sonia::lang::lex::resource_location> EXCLPT     "`!`" 
%token TILDA                "`~`"
%token EXCL                 "`^`"
%token COMMA                ","
%token DOLLAR               "`$`"
%token QMARK                "`?`"
%token HASHTAG              "`#`"

%token LET
%token VAR
%token EXTERN

%token <sonia::lang::lex::resource_location> NEW        "`new`"
%token WHILE                "`while`"
%token FOR                  "`for`"
%token IN                   "`in`"
%token IF                   "`if`"
%token ELSE                 "`else`"
%token <sonia::lang::lex::resource_location> CONTINUE   "`continue`"
%token <sonia::lang::lex::resource_location> BREAK      "`break`"
%token <sonia::lang::lex::resource_location> RETURN     "`return`"

%token AUTO
%token USING
%token THROW
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



// 9 priority
//%left EQ NE

// 8 priority
//%left OPEN_BROKET LE CLOSE_BROKET GE
//%left OPEN_BROKET LE GE

// 7 priority
//%left RIGHTSHIFT LEFTSHIFT

// 4 priority
//%left PTAST ARROWAST

// 3 priority
//%right PREFIXDBLPLUS PREFIXDBLMINUS PREFIXPLUS PREFIXMINUS EXCLPT TILDA CAST ADDRESS SIZEOF

// 2 priority
//%left DBLPLUS DBLMINUS OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET POINT

// 1 priority
//%left IDENTIFIER
////////////////////////////////////////////////////////////////////////

%left LOWEST

%left ELLIPSIS
%left COLON

// 15 priority
%right ASSIGN

// 14 priority
%left LOGIC_OR

// 13 priority
%left LOGIC_AND

// 12 priority
%left BITOR

// 10 priority
%left AMPERSAND

// 9 priority
%left EQ NE

// 6 priority
%left CONCAT
%left PLUS MINUS

// 5 priority
%left ASTERISK SLASH PERCENT

// 3 priority
%right DEREF EXCLPT

// 2 priority
%left OPEN_BRACE OPEN_PARENTHESIS OPEN_SQUARE_BRACKET ARROW ARROWEXPR POINT INTEGER_INDEX
%right QMARK

%left DBLCOLON

// 1 priority
%right IDENTIFIER INTERNAL_IDENTIFIER RESERVED_IDENTIFIER

// STATEMENTS
%token INCLUDE
%type <statement> statement infunction-statement
%type <finished_statement_type> finished-statement
%type <generic_statement_type> generic-statement

%type <managed_statement_list> statement_any finished-statement-any
%type <managed_statement_list> infunction-statement-any finished-infunction-statement-any infunction-statement-set braced-statements

%type <let_statement> let-decl-start let-decl-start-with-opt-type let-decl

//%type <identifier_chain_t> identifier-chain

// QNAME
%type <sonia::lang::bang::annotated_identifier> identifier internal-identifier internal-identifier-opt
//%type <sonia::lang::bang::annotated_identifier> any_identifier
%type <sonia::lang::bang::annotated_qname> qname


// FUNCTIONS
%token INLINE
%token <sonia::lang::lex::resource_location> FN "`fn`"
%type <fn_kind> fn-start-decl
%type <fn_pure_t> fn-decl
%type <sonia::lang::bang::annotated_qname> fn-name

// ENUMERATIONS
%token ENUM
%type <enum_decl> enum-decl
%type <std::vector<sonia::lang::bang::identifier>> case-list-opt case-list
%type <sonia::lang::bang::identifier> case-decl

// TYPES
%token STRUCT
%token EXTENDS

%type <struct_decl> struct-decl
%type <using_decl> using-decl

//%type <extension_list_t> type-extension-list
//%type <extension_list_t> type-extension-any

%type <named_expression_list_t> argument-list argument-list-opt
%type <named_expression_t> argument
%type <sonia::lang::bang::annotated_identifier> argument-name

%type <field_list_t> field-list field-list-opt
%type <field_t> field
%type <sonia::optional<syntax_expression_t>> field-default-value-opt

%type <parameter_list_t>  parameter-list parameter-list-opt // for unification, empty assignment
%type <parameter_t> parameter-decl
%type <parameter_constraint_set_t> parameter-constraint-set parameter-value-constraint-set // parameter-concept-set parameter-concept-set-opt
%type <syntax_expression_t> parameter-matched-type parameter-matched-value basic-parameter-matched-type 
%type <sonia::optional<syntax_expression_t>> parameter-default-value-opt

%token WEAK "weak modifier"
%token CONST "const modifier"
%token <sonia::lang::lex::resource_location> MUT "mut modifier"

// EXPRESSIONS
%token <annotated_nil> NIL_WORD "nil"

%token <annotated_bool> TRUE_WORD "true"
%token <annotated_bool> FALSE_WORD "false"

%type <syntax_expression_t> type-expr
%type <syntax_expression_t> syntax-expression syntax-expression-wo-ii concept-expression
%type <syntax_expression_t> apostrophe-expression new-expression call-expression lambda-expression compound-expression
%type <named_expression_list_t> pack-expression
//%type <named_expression_list_t> opt-named-expr-list-any opt-named-expr-list
//%type <named_expression_t> opt-named-expr
//%type <expression_list_t> expression-list-any
%type <expression_list_t> expression-list

//%type <named_expression_term_t> named-arg
//%type <named_expression_term_list_t> arg-list arg-list-not-empty
//%type <function_def> function-def


////%type <ct_expression_ptr_t> ct_expression ct-function-call
////%type <std::vector<ct_expression_ptr_t>> ct_expression_list ct_expression_list_not_empty
////%type <std::vector<statement_ptr_t>>

%destructor { } <*>
%printer { } <*>

%%

begin:
	  statement_any END { ctx.set_root_statements(std::move($1)); }
    | finished-statement-any END { ctx.set_root_statements(std::move($1)); }
	;

statement_any:
      %empty
        { $$ = ctx.new_statement_list(); }
    | statement
        { $$ = ctx.new_statement_list(); $$.emplace_back(std::move($statement)); }
    | finished-statement-any[sts] statement
        { $sts.emplace_back(std::move($statement)); $$ = std::move($sts); }
/*
    |
    NAMESPACE nsname
        {
            ctx.push_ns(std::move($2));
        }
        OPEN_BRACE statement_any CLOSE_BRACE
        {
            $$ = std::move($5);
            ctx.pop_ns();
        }
*/
    ;

finished-statement-any:
      finished-statement[st]
        { $$ = ctx.new_statement_list(); $$.emplace_back(std::move($st)); }
    | finished-statement-any[sts] END_STATEMENT { $$ = std::move($sts); }
    | finished-statement-any[sts] finished-statement[st]
        { $sts.emplace_back(std::move($st)); $$ = std::move($sts); }
    | statement_any[sts] END_STATEMENT
        { $$ = std::move($sts); }
    ;

statement:
      EXTERN VAR identifier COLON type-expr[type]
        { $$ = extern_var{ std::move($identifier), &ctx.push(std::move($type)) }; }
    | EXTERN FN fn-decl[fn]
        { $$ = std::move($fn); IGNORE_TERM($FN); }
    | INCLUDE STRING
        { $$ = include_decl{ctx.make_string(std::move($STRING)) }; }
    | fn-start-decl[fnkind] fn-name[name] OPEN_PARENTHESIS parameter-list-opt[parameters] CLOSE_PARENTHESIS ARROWEXPR syntax-expression[value]
        { 
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location($value);
            sts.emplace_back(return_decl_t{ std::move($value), std::move(loc) });
            $$ = fn_decl_t{ fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .kind = $fnkind }, ctx.push(std::move(sts)) };
            IGNORE_TERM($OPEN_PARENTHESIS);
            //     $$ = fn_decl_t{ fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .result = std::move($value), .is_type_expression_result = false, .kind = $fnkind } }; IGNORE_TERM($OPEN_PARENTHESIS); }
        }
    | generic-statement
        { $$ = apply_visitor(statement_adopt_visitor<statement>{}, $1); }
    | STRUCT struct-decl[struct]
        { $$ = std::move($struct); }
	;
 
let-decl:
      let-decl-start-with-opt-type
        { $$ = std::move($1); }
    | let-decl-start-with-opt-type ASSIGN pack-expression[value]
        { $$ = std::move($1); $$.expressions = std::move($value); IGNORE_TERM($ASSIGN); }
    ;

let-decl-start:
      identifier
        { $$ = let_statement{ std::move($identifier), {}, {}, false }; }
    | WEAK identifier
        { $$ = let_statement{ std::move($identifier), {}, {}, true }; }
    ;

let-decl-start-with-opt-type :
      let-decl-start
    | let-decl-start COLON type-expr[type]
        { $$ = std::move($1); $$.type = std::move($type); }
    ;

infunction-statement-any:
      %empty
        { $$ = ctx.new_statement_list(); }
    | infunction-statement[st]
        { $$ = ctx.new_statement_list(); $$.emplace_back(std::move($st)); }
    | finished-infunction-statement-any[sts] infunction-statement[st]
        { $sts.emplace_back(std::move($st)); $$ = std::move($sts); }
    ;

finished-infunction-statement-any:
      finished-statement[st]
        { $$ = ctx.new_statement_list(); $$.emplace_back(std::move($st)); }
    | finished-infunction-statement-any[sts] END_STATEMENT { $$ = std::move($sts); }
    | finished-infunction-statement-any[sts] finished-statement[st]
        { $sts.emplace_back(std::move($st)); $$ = std::move($sts); }
    | infunction-statement-any[sts] END_STATEMENT
        { $$ = std::move($sts); }
    ;

braced-statements:
    OPEN_BRACE infunction-statement-set[sts] CLOSE_BRACE
        { $$ = std::move($sts); IGNORE_TERM($1); }
    ;

finished-statement:
      WHILE syntax-expression[condition] braced-statements[body]
        { $$ = while_decl{ std::move($condition), ctx.push(std::move($body)) }; }
    | WHILE syntax-expression[condition] END_STATEMENT syntax-expression[continue] braced-statements[body]
        { $$ = while_decl{ std::move($condition), ctx.push(std::move($body)), std::move($continue) }; }
    | FOR syntax-expression[iter] IN syntax-expression[coll] braced-statements[body]
        { $$ = for_decl{ std::move($iter), std::move($coll), ctx.push(std::move($body)) }; }
    | IF syntax-expression braced-statements[body]
        { $$ = if_decl{ std::move($2), ctx.push(std::move($body)) }; }
    | IF syntax-expression braced-statements[trueBody] ELSE braced-statements[falseBody]
        { $$ = if_decl{ std::move($2), ctx.push(std::move($trueBody)), ctx.push(std::move($falseBody)) }; }
    | fn-start-decl[fnkind] fn-decl[fn] braced-statements[body]
        { $fn.kind = $fnkind; $$ = fn_decl_t{ std::move($fn), ctx.push(std::move($body)) }; }
    | STRUCT qname braced-statements[body]
        { $$ = struct_decl{ std::move($qname), ctx.push(std::move($body)) }; }
    | STRUCT qname OPEN_PARENTHESIS[beginParams] parameter-list-opt[parameters] CLOSE_PARENTHESIS braced-statements[body]
        { $$ = struct_decl{ fn_pure_t{ std::move($qname), std::move($parameters) }, ctx.push(std::move($body)) }; IGNORE_TERM($beginParams); }
    | ENUM enum-decl[enum]
        { $$ = std::move($enum); }
    ;

infunction-statement-set:
      infunction-statement-any[sts]
        { $$ = std::move($sts); }
    | finished-infunction-statement-any[sts]
        { $$ = std::move($sts); }
    ;

generic-statement:
      LET let-decl[let]
        { $$ = std::move($let); }
    | USING using-decl[alias]
        { $$ = std::move($alias); }
    | compound-expression
        { $$ = expression_statement_t{ std::move($1) }; }
    | syntax-expression ASSIGN syntax-expression
        { $$ = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move($1), std::move($3), std::move($2) } }; }
    ;

infunction-statement:
      generic-statement
        { $$ = apply_visitor(statement_adopt_visitor<statement>{}, $1); }
    | BREAK
        { $$ = break_statement_t{ std::move($1) }; }
    | CONTINUE
        { $$ = continue_statement_t{ std::move($1) }; }
    | RETURN
        { $$ = return_decl_t{ .location = std::move($RETURN) }; }
    | RETURN syntax-expression[expr]
        { $$ = return_decl_t{ .expression = std::move($expr), .location = std::move($RETURN) }; }
    ;

identifier:
    IDENTIFIER
      { $$ = ctx.make_identifier($IDENTIFIER); }
/*  
	| DOLLAR OPEN_PARENTHESIS ct-expression CLOSE_PARENTHESIS
*/  
	;

internal-identifier:
    INTERNAL_IDENTIFIER
        { $$ = ctx.make_identifier($INTERNAL_IDENTIFIER); }
    ;

/*
any_identifier:
      identifier
    | INTERNAL_IDENTIFIER
        { $$ = ctx.make_identifier(std::move($1)); }
    | RESERVED_IDENTIFIER
        { $$ = ctx.make_identifier(std::move($1)); }
    ;
*/
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
        { $$ = fn_kind::DEFAULT; IGNORE_TERM($FN); }
    | INLINE FN
        { $$ = fn_kind::INLINE; IGNORE_TERM($FN); }
    ;

fn-name:
      qname
    | NEW { $$ = ctx.make_qname(annotated_string_view{ "new"sv, std::move($NEW) }); }
    ;

fn-decl:
      fn-name[name] OPEN_PARENTHESIS parameter-list-opt[parameters] CLOSE_PARENTHESIS
        { $$ = fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters) }; IGNORE_TERM($OPEN_PARENTHESIS); }
    | fn-name[name] OPEN_PARENTHESIS parameter-list-opt[parameters] CLOSE_PARENTHESIS FARROW type-expr[type]
        { $$ = fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .result = std::move($type), .is_type_expression_result = true }; IGNORE_TERM($OPEN_PARENTHESIS); }
    | fn-name[name] OPEN_PARENTHESIS parameter-list-opt[parameters] CLOSE_PARENTHESIS ARROW type-expr[type]
        { $$ = fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .result = std::move($type), .is_type_expression_result = true, .is_not_a_pattern_result = true }; IGNORE_TERM($OPEN_PARENTHESIS); }
    ;

///////////////////////////////////////////////// ENUMERATIONS
enum-decl:
    qname OPEN_BRACE case-list-opt[cases] CLOSE_BRACE
        { $$ = enum_decl{ std::move($qname), std::move($cases) }; IGNORE_TERM($OPEN_BRACE); }
    ;

case-list-opt:
      %empty { $$ = {}; }
    | case-list
    ;

case-list:
      case-decl
        { $$ = std::vector<sonia::lang::bang::identifier>{std::move($1)}; }
    | case-list COMMA case-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

case-decl:
    identifier
        { $$ = $1.value; }
    ;
///////////////////////////////////////////////// TYPES

struct-decl:
      qname ARROWEXPR OPEN_PARENTHESIS[begin] field-list-opt[fields] CLOSE_PARENTHESIS
        { $$ = struct_decl{ std::move($qname), std::move($fields) }; IGNORE_TERM($begin); }
    | qname OPEN_PARENTHESIS[beginParams] parameter-list-opt[parameters] CLOSE_PARENTHESIS ARROWEXPR OPEN_PARENTHESIS[begin] field-list-opt[fields] CLOSE_PARENTHESIS
        { $$ = struct_decl{ fn_pure_t{ std::move($qname), std::move($parameters) }, std::move($fields) }; IGNORE_TERM($beginParams); IGNORE_TERM($begin); }
    ;

/*
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
*/

///////////////////////////////////////////////// ALIASES

using-decl:
      qname ARROWEXPR syntax-expression[expr]
        { 
            $$ = using_decl{ std::move($qname), nullopt, std::move($expr) };
        }
    | qname OPEN_PARENTHESIS[beginParams] parameter-list-opt[parameters] CLOSE_PARENTHESIS ARROWEXPR syntax-expression[expr]
        {
            $$ = using_decl{ std::move($qname), std::move($parameters), std::move($expr) }; IGNORE_TERM($beginParams);
        }
    ;

////////////////////// LIST (aka vector/list elements)
/*
expression-list-any:
      %empty { $$ = {}; }
    | expression-list
    ;
*/
expression-list:
      syntax-expression[value]
        { $$ = expression_list_t{ std::move($value) }; }
    | expression-list[list] COMMA syntax-expression[value]
        { $$ = std::move($list); $$.emplace_back(std::move($value)); }
    ;

////////////////////// ARGUMENTS (aka function call arguments)
argument-list-opt:
       %empty { $$ = {}; }
    | argument-list
    ;

argument-list:
      argument[arg]
        { $$ = named_expression_list_t{std::move($arg)}; }
    | argument-list[list] COMMA argument[arg]
        { $$ = std::move($list); $$.emplace_back(std::move($arg)); }
    ;

argument:
      argument-name[name] syntax-expression[value]
        { $$ = named_expression_t{ std::move($name), std::move($value) }; }
    | syntax-expression[value]
        { $$ = named_expression_t{ std::move($value) }; }
    ;

argument-name:
      identifier[id] ASSIGN
        { $$ = std::move($id); IGNORE_TERM($ASSIGN); }
    | identifier[id] COLON
        { $$ = std::move($id); }
    ;

////////////////////// FIELDS (structure fields declaration)
field-list-opt:
       %empty { $$ = {}; }
    | field-list
    ;

field-list:
      field
        { $$ = field_list_t{std::move($field)}; }
    | field-list[list] COMMA field
        { $$ = std::move($list); $$.emplace_back(std::move($field)); }
    ;

field-default-value-opt:
      %empty { $$ = nullopt; }
    | ASSIGN syntax-expression[value] { $$ = std::move($value); IGNORE_TERM($ASSIGN); }
    ;

field:
      identifier COLON type-expr[type] field-default-value-opt[default]
        { $$ = field_t{ std::move($identifier), field_modifier_t::value, std::move($type), std::move($default) }; }
    | identifier ARROWEXPR type-expr[type] field-default-value-opt[default]
        { $$ = field_t{ std::move($identifier), field_modifier_t::const_value, std::move($type), std::move($default) }; }
    /*
    | identifier COLON parameter-constraint-modifier-opt[mod] field-type-expr[type] ASSIGN syntax-expression[value]
        { $$ = field_t{ std::move($identifier), std::move($mod), std::move($type), std::move($value) }; IGNORE_TERM($ASSIGN); }
    */
    ;

////////////////////// PARAMETERS (function parameters declaration)

parameter-list-opt:
       %empty { $$ = {}; }
    |  parameter-list
    ;

parameter-list:
      parameter-decl
        { $$ = parameter_list_t{ parameter_t{std::move($1)} }; }
    | parameter-list COMMA parameter-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

internal-identifier-opt:
      %empty { $$ = annotated_identifier{}; }
    | internal-identifier[intid] { $$ = $intid; }
    ;

parameter-default-value-opt:
      %empty { $$ = nullopt; }
    | ASSIGN syntax-expression[value] { $$ = std::move($value); IGNORE_TERM($ASSIGN); }
    ;

parameter-decl:
      identifier[id] internal-identifier-opt[intid] COLON parameter-default-value-opt[default]
        { $$ = parameter_t{ named_parameter_name{ std::move($id), std::move($intid) }, parameter_constraint_modifier_t::value_type, parameter_constraint_set_t{ .expression = placeholder{} }, std::move($default) }; }
    | identifier[id] internal-identifier-opt[intid] COLON parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ named_parameter_name{ std::move($id), std::move($intid) }, parameter_constraint_modifier_t::value_type, std::move($constraint), std::move($default) }; }
    | identifier[id] internal-identifier-opt[intid] COLON CONST parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ named_parameter_name{ std::move($id), std::move($intid) }, parameter_constraint_modifier_t::const_value_type, std::move($constraint), std::move($default) }; }
    | identifier[id] internal-identifier-opt[intid] COLON MUT parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ named_parameter_name{ std::move($id), std::move($intid) }, parameter_constraint_modifier_t::mutable_value_type, std::move($constraint), std::move($default) }; IGNORE_TERM($MUT); }
    | identifier[id] internal-identifier-opt[intid] ARROWEXPR parameter-value-constraint-set[constraint]
        { $$ = parameter_t{ named_parameter_name{ std::move($id), std::move($intid) }, parameter_constraint_modifier_t::const_value, std::move($constraint) }; }
    
    | INTERNAL_IDENTIFIER[intid] COLON parameter-default-value-opt[default]
        { $$ = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::value_type, parameter_constraint_set_t{ .expression = placeholder{} }, std::move($default) }; }
    | INTERNAL_IDENTIFIER[intid] COLON parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::value_type, std::move($constraint), std::move($default) }; }
    | INTERNAL_IDENTIFIER[intid] COLON CONST parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::const_value_type, std::move($constraint), std::move($default) }; }
    | INTERNAL_IDENTIFIER[intid] COLON MUT parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::mutable_value_type, std::move($constraint), std::move($default) }; IGNORE_TERM($MUT); }
    | INTERNAL_IDENTIFIER[intid] ARROWEXPR parameter-value-constraint-set[constraint]
        { $$ = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::const_value, std::move($constraint) }; }
    
    | INTERNAL_IDENTIFIER[intid] ELLIPSIS COLON parameter-constraint-set[constraint]
        { $$ = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::value_type, std::move($constraint) }; IGNORE_TERM($ELLIPSIS); }
    | INTERNAL_IDENTIFIER[intid] ELLIPSIS COLON CONST parameter-constraint-set[constraint]
        { $$ = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::const_value_type, std::move($constraint) }; IGNORE_TERM($ELLIPSIS); }
    | INTERNAL_IDENTIFIER[intid] ELLIPSIS COLON MUT parameter-constraint-set[constraint]
        { $$ = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move($intid)) }, parameter_constraint_modifier_t::mutable_value_type, std::move($constraint) }; IGNORE_TERM($ELLIPSIS); IGNORE_TERM($MUT); }

    | parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type, std::move($constraint), std::move($default) }; }
    | CONST parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value_type, std::move($constraint), std::move($default) }; }
    | MUT parameter-constraint-set[constraint] parameter-default-value-opt[default]
        { $$ = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::mutable_value_type, std::move($constraint), std::move($default) }; IGNORE_TERM($MUT); }
    | ARROWEXPR parameter-value-constraint-set[constraint]
        { $$ = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value, std::move($constraint) }; }
    ;

parameter-constraint-set:
      parameter-matched-type[match]
        { $$ = parameter_constraint_set_t{ .expression = std::move($match) }; }
    | concept-expression[concept]
        { $$ = parameter_constraint_set_t{ .concepts = { std::move($concept) } }; }
    | parameter-constraint-set concept-expression[concept]
        { $$ = std::move($1); $$.concepts.emplace_back(std::move($concept)); }
    ;

parameter-value-constraint-set:
      parameter-matched-value[match]
        { $$ = parameter_constraint_set_t{ .expression = std::move($match) }; }
    | concept-expression[concept]
        { $$ = parameter_constraint_set_t{ .concepts = { std::move($concept) } }; }
    | parameter-value-constraint-set concept-expression[concept]
        { $$ = std::move($1); $$.concepts.emplace_back(std::move($concept)); }
    ;

parameter-matched-value:
      syntax-expression
    ;

parameter-matched-type:
      basic-parameter-matched-type
    | ELLIPSIS
        { $$ = unary_expression_t{ unary_operator_type::ELLIPSIS, false, placeholder{ $ELLIPSIS }, $ELLIPSIS }; }
    | basic-parameter-matched-type[expr] ELLIPSIS
        { $$ = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move($expr), std::move($ELLIPSIS) }; }
    | INTERNAL_IDENTIFIER[id]
        { $$ = variable_reference{ ctx.make_qname(std::move($id)), true }; }
    | INTERNAL_IDENTIFIER[id] ELLIPSIS
        { $$ = unary_expression_t{ unary_operator_type::ELLIPSIS, false, variable_reference{ ctx.make_qname(std::move($id)), true }, std::move($ELLIPSIS) }; }
    ;


basic-parameter-matched-type:
      UNDERSCORE
        { $$ = placeholder{ std::move($UNDERSCORE) }; }
        /*
    | type-expr
        { $$ = std::move($1); }
        */
    | qname
        { $$ = variable_reference{ std::move($qname) }; }

    | qname OPEN_PARENTHESIS pack-expression[arguments] CLOSE_PARENTHESIS
        { $$ = function_call_t{ std::move($OPEN_PARENTHESIS), std::move($qname), std::move($arguments) }; }
    | basic-parameter-matched-type[left] BITOR basic-parameter-matched-type[right]
        { $$ = binary_expression_t{ binary_operator_type::BIT_OR, std::move($left), std::move($right), std::move($BITOR) }; }
    ;
    
/*
// one optional syntax-expression, and arbitrary count of concept-expression or internal idetifiers
parameter-constraint-set:
      syntax-expression-wo-ii
        { $$ = parameter_constraint_set_t{ std::move($1), {}, {} }; }
    | internal-identifier[id]
        { $$ = parameter_constraint_set_t{ nullopt, {}, { std::move($id) } }; }
    | concept-expression
        { $$ = parameter_constraint_set_t{ nullopt, {std::move($1)}, {} }; }
    | parameter-constraint-set internal-identifier[id]
        { $$ = std::move($1); $$.bindings.emplace_back(std::move($id)); }
    | parameter-constraint-set concept-expression
        { $$ = std::move($1); $$.concepts.emplace_back(std::move($2)); }
    ;
*/

/////////////////////////// EXPRESSIONS

concept-expression:
    AT_SYMBOL qname
        { $$ = syntax_expression_t{ variable_reference{std::move($2), false} }; }
    ;

syntax-expression:
      INTERNAL_IDENTIFIER[id]
        { $$ = variable_reference{ ctx.make_qname(std::move($id)), true }; }
    | syntax-expression-wo-ii
    ;

// without internal identifiers
syntax-expression-wo-ii:
      NIL_WORD
        { $$ = std::move($1); }
    | TRUE_WORD
        { $$ = std::move($1); }
    | FALSE_WORD
        { $$ = std::move($1); }
    | INTEGER
        { $$ = std::move($INTEGER); }
    | DECIMAL
        { $$ = std::move($DECIMAL); }
    | DECIMAL_S
        { $$ = std::move($DECIMAL_S); }
    | INTEGER_INDEX
        { $$ = annotated_decimal{ ctx.make_decimal($INTEGER_INDEX.value), $INTEGER_INDEX.location }; }
    | STRING
        { $$ = ctx.make_string(std::move($STRING)); }
    | RESERVED_IDENTIFIER
        { $$ = variable_reference{ ctx.make_qname(std::move($RESERVED_IDENTIFIER)), true }; }
    | qname
        { $$ = variable_reference{ std::move($qname) }; }
    | OPEN_PARENTHESIS[start] pack-expression[list] CLOSE_PARENTHESIS
        {
            if ($list.size() == 1 && !$list.front().has_name()) { // single unnamed expression => extract
                $$ = std::move($list.front().value());
            } else {
                BOOST_ASSERT(!$list.empty());
                $$ = function_call_t{ std::move($start), annotated_qname{}, std::move($list) };
            }
        }
    | OPEN_SQUARE_BRACKET expression-list[list] CLOSE_SQUARE_BRACKET
        { 
            if ($list.size() == 1) {
                $$ = bang_vector_t{ std::move($OPEN_SQUARE_BRACKET), std::move($list.front()) };
            } else {
                $$ = array_expression_t{ std::move($OPEN_SQUARE_BRACKET), std::move($list )};
            }
        }
    | OPEN_SQUARE_DBL_BRACKET expression-list[list] CLOSE_SQUARE_DBL_BRACKET
        { $$ = array_expression_t{ std::move($OPEN_SQUARE_DBL_BRACKET), std::move($list )}; }
    | syntax-expression[type] OPEN_SQUARE_BRACKET syntax-expression[index] CLOSE_SQUARE_BRACKET
        { $$ = index_expression_t{ std::move($type), std::move($index), std::move($OPEN_SQUARE_BRACKET) }; }
    | POINT identifier
        { $$ = std::move($identifier); IGNORE_TERM($POINT); }
    //| syntax-expression[object] POINT syntax-expression[property]
    //    { $$ = member_expression_t{ std::move($object), std::move($property) }; IGNORE_TERM($2); }
    | syntax-expression[object] POINT identifier[property]
         { $$ = member_expression_t{ std::move($object), std::move($property) }; IGNORE_TERM($2); }
    | syntax-expression[object] POINT apostrophe-expression[property]
         { $$ = member_expression_t{ std::move($object), std::move($property) }; IGNORE_TERM($2); }
    | syntax-expression[object] INTEGER_INDEX[property]
         { $$ = member_expression_t{ std::move($object), annotated_integer{ ctx.make_integer($property.value.substr(1)), $property.location } }; IGNORE_TERM($2); }
//////////////////////////// 3 priority
    | EXCLPT syntax-expression
		{ $$ = unary_expression_t{ unary_operator_type::NEGATE, true, std::move($2), std::move($1) }; }
    | ASTERISK syntax-expression %prec DEREF
		{ $$ = unary_expression_t{ unary_operator_type::DEREF, true, std::move($2), std::move($1) }; }
//////////////////////////// 5 priorit

//////////////////////////// 6 priority
    | syntax-expression PLUS syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::PLUS, std::move($1), std::move($3), std::move($2) }; }
    | syntax-expression MINUS syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::MINUS, std::move($1), std::move($3), std::move($2) }; }

//////////////////////////// 9 priority
    | syntax-expression EQ syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::EQ, std::move($1), std::move($3), std::move($2) }; }
    | syntax-expression NE syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::NE, std::move($1), std::move($3), std::move($2) }; }
    | syntax-expression CONCAT syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::CONCAT, std::move($1), std::move($3), std::move($2) }; }
//////////////////////////// 10 priority
    | syntax-expression AMPERSAND syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::BIT_AND, std::move($1), std::move($3), std::move($2) }; }

//////////////////////////// 12 priority
    | syntax-expression BITOR syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::BIT_OR, std::move($1), std::move($3), std::move($2) }; }
//////////////////////////// 13 priority
    | syntax-expression LOGIC_AND syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move($1), std::move($3), std::move($2) }; }
//////////////////////////// 14 priority
    | syntax-expression LOGIC_OR syntax-expression
        { $$ = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move($1), std::move($3), std::move($2) }; }

    | apostrophe-expression
    | new-expression
    | compound-expression
    | lambda-expression
    ;

apostrophe-expression:
      APOSTROPHE syntax-expression[expr] APOSTROPHE
        { $$ = std::move($expr); }
    ;

new-expression:
      NEW qname[type]
        { $$ = new_expression_t{ std::move($NEW), variable_reference{ std::move($type) } }; }
    | NEW apostrophe-expression[typeExpr]
        { $$ = new_expression_t{ std::move($NEW), std::move($typeExpr) }; }
    | NEW qname[type] OPEN_PARENTHESIS argument-list-opt[arguments] CLOSE_PARENTHESIS
        { $$ = new_expression_t{ std::move($NEW), variable_reference{ std::move($type) }, std::move($arguments) }; IGNORE_TERM($OPEN_PARENTHESIS); }
    | NEW apostrophe-expression[typeExpr] OPEN_PARENTHESIS argument-list-opt[arguments] CLOSE_PARENTHESIS
        { $$ = new_expression_t{ std::move($NEW), std::move($typeExpr), std::move($arguments) }; IGNORE_TERM($OPEN_PARENTHESIS); }
    ;

call-expression:
      qname[name] OPEN_PARENTHESIS[start] pack-expression[arguments] CLOSE_PARENTHESIS
        { $$ = function_call_t{ std::move($start), std::move($name), std::move($arguments) }; }
    | MUT OPEN_PARENTHESIS[start] pack-expression[arguments] CLOSE_PARENTHESIS
        { 
            auto aid = ctx.make_identifier(annotated_string_view{ "mut"sv, std::move($MUT) });
            $$ = function_call_t{ std::move($start), annotated_qname{ qname{ aid.value, true }, std::move(aid.location) }, std::move($arguments) };
        }
    | call-expression[nameExpr] OPEN_PARENTHESIS[start] pack-expression[arguments] CLOSE_PARENTHESIS
        { $$ = function_call_t{ std::move($start), std::move($nameExpr), std::move($arguments) }; }
    | apostrophe-expression[nameExpr] OPEN_PARENTHESIS[start] pack-expression[arguments] CLOSE_PARENTHESIS
        { $$ = function_call_t{ std::move($start), std::move($nameExpr), std::move($arguments) }; }
    | lambda-expression[lambda] OPEN_PARENTHESIS[start] pack-expression[arguments] CLOSE_PARENTHESIS
        { $$ = function_call_t{ std::move($start), std::move($lambda), std::move($arguments) }; }
    ;

lambda-expression:
      fn-start-decl[fnkind] OPEN_PARENTHESIS[start] parameter-list-opt[parameters] CLOSE_PARENTHESIS braced-statements[body]
        { $$ = lambda_t{ $fnkind, std::move($start), std::move($parameters), std::move($body) }; }
    | fn-start-decl[fnkind] OPEN_PARENTHESIS[start] parameter-list-opt[parameters] CLOSE_PARENTHESIS ARROW type-expr[type] braced-statements[body]
        { $$ = lambda_t{ $fnkind, std::move($start), std::move($parameters), std::move($body), std::move($type) }; }
    ;

pack-expression:
      syntax-expression[expr]
        { $$ = named_expression_list_t{ named_expression_t{ std::move($expr) } }; }
        /*
    | MUT syntax-expression[expr]
    {
        auto aid = ctx.make_identifier(annotated_string_view{ "mut"sv, std::move($MUT) });
        $$ = named_expression_list_t{ named_expression_t{ function_call_t{ qname{ aid.value, true }, std::move(aid.location) }, std::move($expr) } };
    }
    */
    | identifier[id] COLON syntax-expression[expr]
        {
            named_expression_list_t list{};
            list.emplace_back(std::move($id), std::move($expr));
            $$ = std::move(list);
        }
    | pack-expression[list] COMMA syntax-expression[expr]
        {
            $list.emplace_back(std::move($expr));
            $$ = std::move($list);
        }
    | pack-expression[list] COMMA identifier[id] COLON syntax-expression[expr]
        {
            $list.emplace_back(std::move($id), std::move($expr));
            $$ = std::move($list);
        }
    ;

compound-expression:
        syntax-expression[expr] ELLIPSIS
        { $$ = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move($expr), std::move($ELLIPSIS) }; }
      | call-expression

    /*
    | syntax-expression OPEN_BRACE argument-list-opt[arguments] CLOSE_BRACE
        { 
           $$ = function_call_t{ std::move($2), std::move($1), std::move($arguments) };
            //$$ = function_call_t{}; IGNORE_TERM($1, $2, $3);
        }
    */
    //| syntax-expression ASSIGN syntax-expression
    //    { $$ = binary_expression_t{ binary_operator_type::ASSIGN, std::move($1), std::move($3), std::move($2) }; }
    ;

/*
opt-named-expr-list-any:
    %empty
        { $$ = named_expression_list_t{}; }
    | opt-named-expr-list
    ;

opt-named-expr-list:
      opt-named-expr
        { $$ = named_expression_list_t{std::move($1)}; }
    | opt-named-expr-list COMMA opt-named-expr
        {
            $$ = std::move($1);
            $$.emplace_back(std::move($3));
		}
	;

opt-named-expr:
      identifier COLON syntax-expression
        { $$ = named_expression_t{ std::move($1), std::move($3) }; }
    | syntax-expression
        { $$ = named_expression_t{ std::move($1) }; }
    ;
*/



    /*
syntax-expression-wii:
      INTERNAL_IDENTIFIER
        { $$ = variable_reference{ctx.make_qname_identifier(std::move($1), false), true}; }
    | syntax-expression
    ;
    */
/*
parameter-constraint-decl:
      CONST type-expr ELLIPSIS
        { $$ = parameter_type_t{bang_parameter_pack_t{std::move($2)}, true}; IGNORE_TERM($3); }
    | CONST type-expr
        { $$ = parameter_type_t{std::move($2), true}; }
    | type-expr ELLIPSIS
        { $$ = parameter_type_t{bang_parameter_pack_t{std::move($1)}, false}; IGNORE_TERM($2); }
    | type-expr
        { $$ = parameter_type_t{std::move($1), false}; }
    ;
*/




/*
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
    | parameter-decl ASSIGN syntax-expression
        { $$ = parameter_woa_t{ std::move($1), std::move($3) }; IGNORE_TERM($2); }
    ;
*/

// TYPE EXPRESSIONS
type-expr:
      qname
        { $$ = variable_reference{ std::move($qname) }; }
    | call-expression
    | INTERNAL_IDENTIFIER[id]
        { $$ = variable_reference{ ctx.make_qname(std::move($id)), true }; }
    //| internal-identifier[id] OPEN_PARENTHESIS opt-named-expr-list-any CLOSE_PARENTHESIS
    //    { $$ = std::move($id); IGNORE_TERM($2); IGNORE_TERM($3); }
    | OPEN_SQUARE_BRACKET type-expr[type] CLOSE_SQUARE_BRACKET
        { $$ = bang_vector_t{ std::move($OPEN_SQUARE_BRACKET), std::move($type)}; }
    | OPEN_PARENTHESIS[start] CLOSE_PARENTHESIS
        { $$ = ctx.make_void(std::move($start)); }
    //    { $$ = function_call_t{ std::move($start), annotated_qname{} }; }
    | OPEN_PARENTHESIS[start] pack-expression[elements] CLOSE_PARENTHESIS
        { $$ = function_call_t{ std::move($start), annotated_qname{}, std::move($elements) }; } 
    | type-expr[type] OPEN_SQUARE_BRACKET INTEGER CLOSE_SQUARE_BRACKET
        { $$ = index_expression_t{ std::move($type), std::move($INTEGER) }; IGNORE_TERM($OPEN_SQUARE_BRACKET); }
    | type-expr[ltype] BITOR type-expr[rtype]
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move($ltype));
            uni.members.emplace_back(std::move($rtype));
            $$ = std::move(uni);
            IGNORE_TERM($BITOR);
        }
    | type-expr[argexpr] ARROW type-expr[rexpr]
        { 
            auto loc = get_start_location($argexpr);
            $$ = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move($argexpr) } }, std::move($rexpr), std::move(loc) };
        }
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



    /*
expression:
      expression QMARK
        { $$ = not_empty_expression_t{ std::move($1) }; }

    //| expression QMARK POINT identifier
    //    { $$ = member_expression_t { std::move($1), std::move($4), true }; IGNORE_TERM($3); }
    | expression LOGIC_AND expression
        { $$ = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move($1), std::move($3), std::move($2) }; }
	| expression LOGIC_OR expression
        { $$ = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move($1), std::move($3), std::move($2) }; }
    //| expression PLUS expression
    //    { $$ = binary_expression_t{ binary_operator_type::PLUS, std::move($1), std::move($3), std::move($2) }; }


//////////////////////////// 9 priority



    //| qname OPEN_BROKET opt-named-expr-list CLOSE_BROKET
   //     { $$ = syntax_expression_t { ctprocedure{ std::move($1), std::move($3) } }; }
    ;

    */

//identifier-chain:
//      identifier
//        { $$ = identifier_chain_t{ std::move($1) }; }
//    | identifier-chain POINT identifier
//        { $$ = std::move($1); $1.emplace_back(std::move($3)); }
//    ;


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

    //////////////////////////// 11 priority

	| expr EXCL expr
        { $$ = handle_call_op($1, $3, operator_type::BITXOR); }


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
