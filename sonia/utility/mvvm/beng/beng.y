/*=============================================================================
    Beng(c) by Alexander A Pototskiy
==============================================================================*/
%require "3.2"
%language "c++"
//%debug
%define api.value.type variant
%define api.prefix {beng_lang}
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
using namespace sonia::lang::beng;

#include "beng.tab.hpp"

using YYSTYPE = beng_lang::parser::semantic_type;
using YYLTYPE = beng_lang::parser::location_type;

int beng_langlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param, parser_context & ctx, void* yyscanner);
//#include "beng.yy.hpp"

void beng_lang::parser::error(const location_type& loc, const std::string& msg)
{
    ctx.append_error(("%1%(%2%,%3%-%4%,%5%): error: %6%"_fmt
		% ctx.get_resource()
		% loc.begin.line % loc.begin.column % loc.end.line % loc.end.column % msg).str());
}

%}

%token END 0 "end of file"

%token COMMENT_BEGIN
%token COMMENT_END

%token <sonia::string_view> STRING
%token <sonia::lang::beng::annotated_string_view> IDENTIFIER
%token <sonia::decimal> DECIMAL
%token <int> INTEGER
%token <sonia::string_view> OPERATOR_TERM

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
%token EQ                   "`==`"
%token NE                   "`!=`"
%token AND                  "`&&`"
%token LOGIC_OR             "`||`"
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
%token OPEN_PARENTHESIS     "`(`"
%token CLOSE_PARENTHESIS    "`)`"
%token OPEN_BRACE           "`{`"
%token CLOSE_BRACE			"`}`"
%token OPEN_SQUARE_BRACKET  "`[`"
%token CLOSE_SQUARE_BRACKET "`]`"
%token OPEN_BROKET          "`<`"
%token CLOSE_BROKET         "`>`"
%token END_STATEMENT		"`;`"
%token POINT                "`.`"
%token PLUS                 "`+`"
%token MINUS                "`-`"
%token ASTERISK             "`*`"
%token SLASH                "`/`"
%token PERCENT              "`%`"
%token AMPERSAND            "`&`"
%token BITOR                "`|`"
%token EXCLPT               "`!`" 
%token TILDA                "`~`"
%token EXCL                 "`^`"
%token COMMA                ","
%token DOLLAR               "`$`"
%token QMARK                "`?`"
%token HASHTAG              "`#`"

%token LET
%token VAR


%token FN
%token NEW
%token FOR
%token VOID_
%token AUTO
%token CONST_
%token USING
%token THROW
%token DELETE_
%token SIZEOF
%token RETURN
%token EXTERN
%token PRIVATE
%token TEMPLATE
%token NAMESPACE


// 16 priority
//%left COMMA

// 15 priority
//%right ASSIGN               "`=`"
//%right QMARK COLON ADDASSIGN SUBASSIGN MULASSIGN MODASSIGN DIVASSIGN LSHIFTSSIGN RSHIFTSSIGN BITANDASSIGN BITORASSIGN BITXORASSIGN THROW

// 14 priority
//%left LOGIC_OR

// 13 priority
//%left AND

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

// 6 priority
//%left PLUS MINUS

// 5 priority
//%left ASTERISK SLASH PERCENT

// 4 priority
//%left PTAST ARROWAST

// 3 priority
//%right PREFIXDBLPLUS PREFIXDBLMINUS PREFIXPLUS PREFIXMINUS EXCLPT TILDA CAST DEREF ADDRESS SIZEOF

// 2 priority
//%left DBLPLUS DBLMINUS OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET POINT ARROW

// 1 priority
//%left IDENTIFIER
////////////////////////////////////////////////////////////////////////

%left LOWEST

// 15 priority
%right ASSIGN               "`=`"

// 14 priority
%left LOGIC_OR

// 2 priority
%left OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET

//%token DBLCOLON             "`::`"
%left DBLCOLON             "`::`"

%type <std::vector<declaration_t>> declaration_any
%type <declaration_t> opt-decl  /* type_declaration */

// QNAME
%type <sonia::lang::beng::qname> qname
%type <sonia::lang::beng::annotated_identifier> identifier

// ENUMERATIONS
%token ENUM
%type <declaration_t> enum-decl
%type <std::vector<sonia::lang::identifier>> case-list-opt case-list
%type <sonia::lang::identifier> case-decl

// TYPES
%token TYPE
%token EXTENDS
%type <declaration_t> type-decl
%type <extension_list_t> type-extension-any type-extension-list
%type <parameter_list_t> parameter-list-opt parameter-list
%type <parameter_t> parameter-decl

// TYPE EXPRESSIONS
%token BOOL
%token INT
%token FLOAT
%type<beng_generic_type> type-expr

// STATEMENTS
//%type <declaration_t> 

// EXPRESSIONS
%type <expression_t> expression compound-expression
%type <named_expression_term_list_t> opt-named-expr-list-any opt-named-expr-list
%type <named_expression_term_t> opt-named-expr


//%type <statement_list_t> statement-list-any statement-list
//%type <statement_t> base-statement statement

//%type <named_expression_term_t> named-arg
//%type <named_expression_term_list_t> arg-list arg-list-not-empty
//%type <function_def> function-def



//%type <declaration_t> function-decl using-decl

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
	|
    declaration_any opt-decl
        {
	        $$ = std::move($1);
            if ($2.which()) $$.emplace_back(std::move($2));
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

opt-decl:
	END_STATEMENT
        { $$ = empty_t{}; }
    | EXTERN VAR identifier COLON type-expr END_STATEMENT
        { $$ = exten_var{ std::move($3.id), std::move($5) }; }
    | enum-decl
        { $$ = $1; }
    | type-decl
        { $$ = $1; }
    | LET identifier /* optional type here*/ ASSIGN expression END_STATEMENT
        { $$ = let_statement_decl{ std::move($2.id), std::move($4) }; }
    | compound-expression END_STATEMENT
        { $$ = expression_decl{ std::move($1) }; }

/*
    | EXTERN FN IDENTIFIER OPEN_PARENTHESIS arg-list CLOSE_PARENTHESIS ARROW expr END_STATEMENT
        { $$ = extern_function_decl{ctx.make_identifier($3), function_def{std::move($5), std::move($8)}}; }
	| function-decl
        { $$ = $1; }
    | using-decl
        { $$ = $1; }
*/
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
        { $$ = sonia::lang::qname($2.id); }
    | identifier
        { $$ = sonia::lang::qname($1.id); $$.set_relative(); }
    | qname DBLCOLON identifier
        { $$ = std::move($1); $$.append(std::move($3.id)); }
    ;

///////////////////////////////////////////////// ENUMERATIONS
enum-decl:
    ENUM qname OPEN_BRACE case-list-opt CLOSE_BRACE
    {
        $$ = enum_decl{ctx.ns() + std::move($2), std::move($4)};
    }
    ;

case-list-opt:
    %empty { $$ = {}; }
    |
    case-list
    ;

case-list:
    case-decl
        { $$ = std::vector<sonia::lang::identifier>{std::move($1)}; }
    |
    case-list COMMA case-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

case-decl:
    identifier
        { $$ = $1.id; }
    ;
///////////////////////////////////////////////// TYPES
type-decl:
    TYPE qname type-extension-any OPEN_BRACE parameter-list-opt CLOSE_BRACE
    {
        $$ = type_decl{ctx.ns() + std::move($2), std::move($3), std::move($5)};
    }
    ;

type-extension-any:
    %empty { $$ = {}; }
    |
    EXTENDS type-extension-list { $$ = std::move($2); }
    ;

type-extension-list:
    qname
        { $$ = extension_list_t{std::move($1)}; }
    |
    type-extension-list COMMA qname
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

parameter-list-opt:
    %empty { $$ = {}; }
    |
    parameter-list
    ;

parameter-list:
    parameter-decl
        { $$ = parameter_list_t{std::move($1)}; }
    |
    parameter-list COMMA parameter-decl
        { $$ = std::move($1); $$.emplace_back(std::move($3)); }
    ;

parameter-decl:
    identifier COLON type-expr
        { $$ = parameter_t{ std::move($1.id), std::move($3) }; }
    | UNDERSCORE COLON type-expr
        { $$ = parameter_t{ sonia::lang::identifier{}, std::move($3) }; }
    ;


// TYPE EXPRESSIONS
type-expr:
      BOOL { $$ = beng_bool_t{}; }
    | INT { $$ = beng_int_t{}; }
    | FLOAT { $$ = beng_float_t{}; }
    | qname { $$ = beng_object_t{std::move($1)}; }
    | type-expr OPEN_SQUARE_BRACKET INTEGER CLOSE_SQUARE_BRACKET
        {
            $$ = beng_array_t{std::move($1), (size_t)std::move($3)};
        }
    | type-expr LOGIC_OR type-expr
        {
            beng_union_t<beng_generic_type> uni{};
            uni.members.emplace_back(std::move($1));
            uni.members.emplace_back(std::move($3));
            $$ = std::move(uni);
        }
    ;



/////////////////////////// EXPRESSIONS
compound-expression:
    //expression OPEN_PARENTHESIS opt-named-expr-list-any CLOSE_PARENTHESIS
    //    { $$ = expression_t { procedure_t{ std::move($1), std::move($3) } }; }
    qname OPEN_PARENTHESIS opt-named-expr-list-any CLOSE_PARENTHESIS
        { $$ = expression_t { procedure_t{ std::move($1), std::move($3) } }; }
    | expression ASSIGN expression
        { $$ = assign_expression_t{ std::move($1), std::move($3) }; }
    ;

expression:
    INTEGER
        { $$ = expression_t { decimal($1) }; }
    | DECIMAL
        { $$ = expression_t { std::move($1) }; }
    | STRING
        { $$ = expression_t { ctx.make_string($1) }; }
    | POINT identifier
        { $$ = expression_t { case_expression { std::move($2.id) } }; }
    | qname
        { $$ = expression_t { std::move($1) }; }
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
        { $$ = expression_t { std::move($2) }; }
    | compound-expression
        { $$ = std::move($1); }
    /*
    | qname OPEN_BROKET opt-named-expr-list CLOSE_BROKET
        { $$ = expression_t { ctprocedure{ std::move($1), std::move($3) } }; }
    
    */
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
    identifier COLON expression
        { $$ = std::tuple{std::move($1), std::move($3)}; }
    | expression
        { $$ = std::move($1); }
    ;

/*
using-decl:
    USING identifier ASSIGN expr END_STATEMENT
        { $$ = using_decl{ctx.ns(), $2, std::move($4) }; }
    ;

function-decl:
    FN qname function-def
        { $$ = function_decl{$2.is_absolute() ? std::move($2) : ctx.ns() + $2, std::move($3)}; }
    ;
    
function-def:
    OPEN_PARENTHESIS arg-list CLOSE_PARENTHESIS ARROW expr OPEN_BRACE statement-list-any CLOSE_BRACE
    {
        $$ = function_def{std::move($2), std::move($5), std::move($7)};
    }
    ;

statement-list-any:
    %empty { $$ = statement_list_t{}; }
    |
    statement-list
    ;

statement-list:
    statement
        { $$ = statement_list_t{std::move($1)}; }
    |
    statement-list statement
        { $$ = std::move($1); $$.emplace_back(std::move($2)); }
    ;

base-statement:
      END_STATEMENT
        { $$ = noop_statement(); }
    | expr END_STATEMENT
        { $$ = std::move($1); }
    | VAR identifier COLON expr ASSIGN expr END_STATEMENT
        { $$ = decl_var_statement_t{std::move($2), std::move($4), std::move($6)}; }
    | VAR identifier ASSIGN expr END_STATEMENT
        { $$ = decl_var_statement_t{std::move($2), expression_terms_t{}, std::move($4)}; }
    | VAR identifier COLON expr END_STATEMENT
        { $$ = decl_var_statement_t{std::move($2), std::move($4), expression_terms_t{}}; }

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
