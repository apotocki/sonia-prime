/*=============================================================================
    Beng(c) by Alexander A Pototskiy
==============================================================================*/

%option header-file="beng.yy.hpp"
%option outfile="beng.yy.cpp"

/* suppresses inclusion of the non-ANSI header file unistd.h */
%option nounistd

/* %option debug */
%option 8bit

/* fast scanner table representation */
%option fast

/* multithreaded, appends the scanner param to yy functions */
%option reentrant

%option stack

/* input not interactive */
%option batch
%option never-interactive

/* use flex built-in support for line numbers */
%option yylineno

/* adds an argument yylval to yylex() */
%option bison-bridge

/* adds an argument yylloc to yylex() */
%option bison-locations

/* do not use yywrap() */
%option noyywrap

/* adds prefix to scanner functions */
%option prefix="beng_lang"

%option extra-type="sonia::lang::lex::scanner_data*"

/* to avoid C4005 warning */
%top{
#include "sonia/config.hpp"
#include <stdint.h>
}

%{

#include "parser.hpp"

using namespace sonia;
using namespace sonia::lang;
using namespace sonia::lang::beng;

#include "beng.tab.hpp"
#define YY_DECL int beng_langlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param, parser_context & ctx, void* yyscanner)
using YYSTYPE = beng_lang::parser::semantic_type;
using YYLTYPE = beng_lang::parser::location_type;

#define ECHO do { \
	ctx.append_error(("%1%:%2%.%3%-%4%.%5%: error: %6%"_fmt \
		% ctx.get_resource() \
		% yylloc->begin.line % yylloc->begin.column % yylloc->end.line % yylloc->end.column % string_view{ yytext, (size_t) yyleng }).str()); } while (0)


// beng_lang::parser::error(*yylloc, string_view{ yytext, (size_t) yyleng });  while (0)

//sonia::lang::lex::undefined_lexem(yytext, (size_t) yyleng ); } while (0)

#define YY_USER_ACTION update_location(*yylloc, yytext);

#ifdef BOOST_WINDOWS
#	include <io.h>	// read();
#	define read _read
#	define fileno _fileno
#endif

#define text_view string_view{yytext, (size_t)yyleng}
#define location lex::resource_location{yylloc->begin.line, yylloc->begin.column, ctx.get_resource()}
#define variant_emplace(...) \
	try { \
		auto value = (__VA_ARGS__); \
		yylval->template emplace<decltype(value)>(std::move(value)); \
	} catch (std::exception const& ex) { \
		ctx.append_error(("%1%(%2%,%3%-%4%,%5%): error: %6%"_fmt \
			% ctx.get_resource() \
			% yylloc->begin.line % yylloc->begin.column % yylloc->end.line % yylloc->end.column % ex.what()).str()); \
	}

%}

WS						[ \t\r\n]+

ASC						[\x00-\x7f]
ASCCHAR					[\x20-\x7e]
ASCN					[\x00-\t\v-\x7f]
U						[\x80-\xbf]
U2						[\xc2-\xdf]
U3						[\xe0-\xef]
U4						[\xf0-\xf4]

UANY				    {ASC}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}
UEASC				    [\x80-\xff]|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}

ASCALPHA				[a-zA-Z]
ASCALPHANUM				[a-zA-Z0-9]
DIGIT					[0-9]
IDENTIFIER				[a-zA-Z_]+[a-zA-Z_0-9]*

INTEGER                 [0-9]+
EXPONENT                [eE][+-]?[0-9]+
DECIMAL                 ([0-9]+"."?|[0-9]*"."[0-9]+){EXPONENT}?

FN						"fn"
LET						"let"
VAR						"var"
ENUM					"enum"
TYPE					"type"
EXTENDS					"extends"
EXTERN                  "extern"
RETURN                  "return"

BOOL					"bool"
INT						"int"
FLOAT					"float"
STRING_WORD				"string"
DECIMAL_WORD			"decimal"

END_STATEMENT			";"
COMMA					","
OPEN_BRACE				"{"
CLOSE_BRACE				"}"
OPEN_PARENTHESIS		"("
CLOSE_PARENTHESIS		")"
COLON                   ":"
OPEN_SQUARE_BRACKET     "["
CLOSE_SQUARE_BRACKET    "]"
ASSIGN                  "="
POINT                   "."
UNDERSCORE              "_"
DBLCOLON                "::"
LOGIC_OR                "||"
ARROW					"->"

/*
NEW						"new"
FOR						"for"
VOID					"void"
AUTO                    "auto"
CONST_                  "const"
USING                   "using"
THROW                   "throw"
DELETE_                 "delete"
SIZEOF                  "sizeof"


PRIVATE                 "private"
TEMPLATE                "template"
NAMESPACE               "namespace"
*/

/* exclude '->', '=', '<', '//', '/*'  */

/*


ARROWAST                "->*"
LSHIFTSSIGN             "<<="
RSHIFTSSIGN             ">>="

FARROW					"~>"
PTAST                   ".*"

DBLPLUS                 "++"
DBLMINUS                "--"
LEFTSHIFT               "<<"
RIGHTSHIFT              ">>"
LE                      "<="
GE                      ">="
EQ                      "=="
NE                      "!="

AND                     "&&"
ADDASSIGN               "+="
SUBASSIGN               "-="
MULASSIGN               "*="
DIVASSIGN               "/="
MODASSIGN               "%="
BITANDASSIGN            "&="
BITORASSIGN             "|="
BITXORASSIGN            "^="




PLUS                    "+"
MINUS                   "-"
ASTERISK                "*"
SLASH                   "/"
PERCENT                 "%"
AMPERSAND               "&"
BITOR                   "|"
EXCLPT                  "!"
TILDA                   "~"
EXCL                    "^"
OPEN_BROKET             "<"
CLOSE_BROKET            ">"



DOLLAR                  "$"
QMARK                   "?"
HASHTAG					"#"
*/

%x COMMENT LINE_COMMENT STR USER_LITERAL

%%

<INITIAL>{WS} /* ignore whitespaces */

<INITIAL,COMMENT>"/*" {
	yy_push_state(COMMENT, yyscanner);
	yyextra->loc_stack.emplace_back(yylloc->begin.line, yylloc->begin.column);
}
<COMMENT>{
	[^*]+|("*"[^/])|{UEASC}
	"*/" {
		yy_pop_state(yyscanner);
		yyextra->loc_stack.pop_back();
	}
}

<INITIAL>"//" { yy_push_state(LINE_COMMENT, yyscanner); }
<LINE_COMMENT>[^\xd]*\xd\xa? { yy_pop_state(yyscanner); }

<INITIAL>\" { yyextra->str_buff_begin = yytext + 1; yy_push_state(STR, yyscanner); }
<STR>{
	(([\x20-\x7e\xa\xd]{-}[\"\\])|{UEASC})+
	\\({DIGIT}+|[abfnrtv\\\'\"])
	\\[xX][0-9a-fA-F]+
	\" {
		variant_emplace(string_view{yyextra->str_buff_begin, yytext});
		yy_pop_state(yyscanner);
		return (beng_lang::parser::token::STRING);
	}
}

<INITIAL>{
	{FN} { return (beng_lang::parser::token::FN); }
	{LET} { return (beng_lang::parser::token::LET); }
	{VAR} { return (beng_lang::parser::token::VAR); }
	{ENUM} { return (beng_lang::parser::token::ENUM); }
	{TYPE} { return (beng_lang::parser::token::TYPE); }
	{EXTENDS} { return (beng_lang::parser::token::EXTENDS); }
	{EXTERN} { return (beng_lang::parser::token::EXTERN); }
	{RETURN} { return (beng_lang::parser::token::RETURN); }

	{BOOL} { return (beng_lang::parser::token::BOOL); }
	{INT} { return (beng_lang::parser::token::INT); }
	{FLOAT} { return (beng_lang::parser::token::FLOAT); }
	{STRING_WORD} { return (beng_lang::parser::token::STRING_WORD); }
	{DECIMAL_WORD} { return (beng_lang::parser::token::DECIMAL_WORD); }

	{OPEN_BRACE}			{ return (beng_lang::parser::token::OPEN_BRACE); }
	{CLOSE_BRACE}			{ return (beng_lang::parser::token::CLOSE_BRACE); }
	{OPEN_PARENTHESIS}		{ return (beng_lang::parser::token::OPEN_PARENTHESIS); }
	{CLOSE_PARENTHESIS}		{ return (beng_lang::parser::token::CLOSE_PARENTHESIS);	}
	{END_STATEMENT}			{ return (beng_lang::parser::token::END_STATEMENT); }
	{COMMA}					{ return (beng_lang::parser::token::COMMA);	}
	{COLON}                 { return (beng_lang::parser::token::COLON); }
	{OPEN_SQUARE_BRACKET}	{ return (beng_lang::parser::token::OPEN_SQUARE_BRACKET); }
	{CLOSE_SQUARE_BRACKET}	{ return (beng_lang::parser::token::CLOSE_SQUARE_BRACKET); }
	{ASSIGN}                { return (beng_lang::parser::token::ASSIGN); }
	{POINT}					{ return (beng_lang::parser::token::POINT);	}
	{UNDERSCORE}			{ return (beng_lang::parser::token::UNDERSCORE); }
	{DBLCOLON}              { return (beng_lang::parser::token::DBLCOLON); }
	{LOGIC_OR}              { return (beng_lang::parser::token::LOGIC_OR); }
	{ARROW}                 { return (beng_lang::parser::token::ARROW); }

	{IDENTIFIER} {
		variant_emplace(annotated_string_view{text_view, location});
		return (beng_lang::parser::token::IDENTIFIER);
	}
	{INTEGER} {
		variant_emplace(ctx.make_int(text_view));
		return (beng_lang::parser::token::INTEGER);
	}
	{DECIMAL} {
		variant_emplace(ctx.make_numeric(text_view));
		return (beng_lang::parser::token::DECIMAL);
	}
	/*
	{FOR} { return (beng_lang::parser::token::FOR); }
	{VOID} { return (beng_lang::parser::token::VOID_); }
	{AUTO} { return (beng_lang::parser::token::AUTO); }
	{CONST_} { return (beng_lang::parser::token::CONST_); }
	{DELETE_} { return (beng_lang::parser::token::DELETE_); }
	{USING} { return (beng_lang::parser::token::USING); }
	{SIZEOF} { return (beng_lang::parser::token::SIZEOF); }
    
	
	{PRIVATE} { return (beng_lang::parser::token::PRIVATE); }
	{TEMPLATE} { return (beng_lang::parser::token::TEMPLATE); }
	{NAMESPACE} { return (beng_lang::parser::token::NAMESPACE); }


	
	{OPERATOR_TERM0}|{OPERATOR_TERM1}|{OPERATOR_TERM2}|{OPERATOR_TERM3} {
		variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
		return (beng_lang::parser::token::OPERATOR_TERM);
	}

	
	
    
	
    
	



	{OPEN_BROKET}			{ return (beng_lang::parser::token::OPEN_BROKET); }
	{CLOSE_BROKET}			{ return (beng_lang::parser::token::CLOSE_BROKET); }
	
	
	
	{QMARK}                 { return (beng_lang::parser::token::QMARK); }
	{HASHTAG}               { return (beng_lang::parser::token::HASHTAG); }
	*/
}

<*><<EOF>>     return (beng_lang::parser::token::END);

%%

/*
	<USER_LITERAL>{
	{UNDERSCORE} { yy_pop_state(yyscanner); return (beng_lang::parser::token::UNDERSCORE); }
	{DBLCOLON} { yy_pop_state(yyscanner); return (beng_lang::parser::token::DBLCOLON); }
	.|[\xd\xa] { yy_pop_state(yyscanner); yyless(0); }
}

DECIMAL_LITERAL			{DECIMAL}"_"



<INITIAL>{DECIMAL} {
	variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
	yy_push_state(NUMERIC, yyscanner);
	return (beng_lang::parser::token::DECIMAL);
}

{DECIMAL} {
    variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
	return (beng_lang::parser::token::DECIMAL);
}

{DECIMAL_LITERAL} {
	variant_emplace(yylval, string_view{yytext, (size_t) yyleng - 1});
	return (beng_lang::parser::token::DECIMAL_LITERAL);
}

<NUMERIC>{
	{UNDERSCORE} { return (beng_lang::parser::token::UNDERSCORE); }
	{DBLCOLON} { return (beng_lang::parser::token::DBLCOLON); }
	{IDENTIFIER} {
		variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
		return (beng_lang::parser::token::IDENTIFIER);
	}

	.|[\xd\xa] { yy_pop_state(yyscanner); --yytext; }
}
------------------------
	//->*
	{ARROWAST}              { return (beng_lang::parser::token::ARROWAST); }

	// ~>
	{FARROW}                { return (beng_lang::parser::token::FARROW); }

	// .*
	{PTAST}                 { return (beng_lang::parser::token::PTAST); }

	// ++
	{DBLPLUS}               { return (beng_lang::parser::token::DBLPLUS); }

	// --
	{DBLMINUS}              { return (beng_lang::parser::token::DBLMINUS); }

	// <<
	{LEFTSHIFT}             { return (beng_lang::parser::token::LEFTSHIFT); }

	// >>
    {RIGHTSHIFT}            { return (beng_lang::parser::token::RIGHTSHIFT); }

	// <=
	{LE}                    { return (beng_lang::parser::token::LE); }

	// >=
	{GE}                    { return (beng_lang::parser::token::GE); }

	// ==
	{EQ}                    { return (beng_lang::parser::token::EQ); }

	// !=
    {NE}                    { return (beng_lang::parser::token::NE); }

	

	// &&
    {AND}                   { return (beng_lang::parser::token::AND); }

	// +=
	{ADDASSIGN}             { return (beng_lang::parser::token::ADDASSIGN); }

	// -=
    {SUBASSIGN}             { return (beng_lang::parser::token::SUBASSIGN); }

	// *=
    {MULASSIGN}             { return (beng_lang::parser::token::MULASSIGN); }

	// /=
    {DIVASSIGN}             { return (beng_lang::parser::token::DIVASSIGN); }

	// %=
    {MODASSIGN}             { return (beng_lang::parser::token::MODASSIGN); }

	// <<=
	{LSHIFTSSIGN}           { return (beng_lang::parser::token::LSHIFTSSIGN); }

	// >>=
    {RSHIFTSSIGN}           { return (beng_lang::parser::token::RSHIFTSSIGN); }

	// &=
	{BITANDASSIGN}          { return (beng_lang::parser::token::BITANDASSIGN); }

	// |=
    {BITORASSIGN}           { return (beng_lang::parser::token::BITORASSIGN); }

	// ^=
    {BITXORASSIGN}          { return (beng_lang::parser::token::BITXORASSIGN); }

	// +
	{PLUS}					{ return (beng_lang::parser::token::PLUS); }

	// -
	{MINUS}					{ return (beng_lang::parser::token::MINUS);	}

	// *
	{ASTERISK}				{ return (beng_lang::parser::token::ASTERISK);	}

	// /
	{SLASH}				    { return (beng_lang::parser::token::SLASH);	}

	// %
    {PERCENT}               { return (beng_lang::parser::token::PERCENT);	}

	// &
	{AMPERSAND}				{ return (beng_lang::parser::token::AMPERSAND);	}

	// |
    {BITOR}				    { return (beng_lang::parser::token::BITOR);	}

	// ^
	{EXCL}					{ return (beng_lang::parser::token::EXCL);	}

	// !
	{EXCLPT}                { return (beng_lang::parser::token::EXCLPT); }
	
	// ~
	{TILDA}					{ return (beng_lang::parser::token::TILDA);	}

	// .
	

	// $
	{DOLLAR}                { return (beng_lang::parser::token::DOLLAR); }

	// >
	{CLOSE_BROKET}			{ return (beng_lang::parser::token::CLOSE_BROKET); }
*/