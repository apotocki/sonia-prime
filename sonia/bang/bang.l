/*=============================================================================
    Bang(c) by Alexander A Pototskiy
==============================================================================*/

%option header-file="bang.yy.hpp"
%option outfile="bang.yy.cpp"

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
%option prefix="bang_lang"

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
using namespace sonia::lang::bang;

#include "bang.tab.hpp"
#define YY_DECL int bang_langlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param, parser_context & ctx, void* yyscanner)
using YYSTYPE = bang_lang::parser::semantic_type;
using YYLTYPE = bang_lang::parser::location_type;

#define ECHO do { \
	ctx.append_error(("%1%:%2%.%3%-%4%.%5%: error: %6%"_fmt \
		% ctx.get_resource() \
		% yylloc->begin.line % yylloc->begin.column % yylloc->end.line % yylloc->end.column % string_view{ yytext, (size_t) yyleng }).str()); } while (0)


// bang_lang::parser::error(*yylloc, string_view{ yytext, (size_t) yyleng });  while (0)

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
ARGIDENTIFIER			$[a-zA-Z_0-9]+

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

TRUE					"true"
FALSE					"false"

WEAK					"weak"

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
BITOR                   "|"
EXCLPT                  "!"
QMARK                   "?"
DBLCOLON                "::"
LOGIC_OR                "||"
LOGIC_AND               "&&"
ARROW					"->"
CONCAT					".."

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


TILDA                   "~"
EXCL                    "^"
OPEN_BROKET             "<"
CLOSE_BROKET            ">"



DOLLAR                  "$"

HASHTAG					"#"

*/

%x COMMENT LINE_COMMENT STR USER_LITERAL

%%

<INITIAL>{WS} /* ignore whitespaces */

<INITIAL,COMMENT>"/*" {
	//GLOBAL_LOG_INFO() << "COMMENT at: line: " << yylloc->begin.line << ", col: " << yylloc->begin.column;
	yy_push_state(COMMENT, yyscanner);
	yyextra->loc_stack.emplace_back(location);
}
<COMMENT>{
	[^*/]+|("/"[^*/])|("*"[^/])|{UEASC}
	"*/" {
		//GLOBAL_LOG_INFO() << "COMMENT CLOSED at: line: " << yylloc->begin.line << ", col: " << yylloc->begin.column;
		yy_pop_state(yyscanner);
		yyextra->loc_stack.pop_back();
	}
}
<INITIAL,COMMENT>"//" { yy_push_state(LINE_COMMENT, yyscanner); }
<LINE_COMMENT>[^\xd]*\xd\xa? { yy_pop_state(yyscanner); }

<INITIAL>\" { 
	yyextra->str_buff_begin = yytext + 1;
	yyextra->loc_stack.emplace_back(location);
	yy_push_state(STR, yyscanner);
}
<STR>{
	(([\x20-\x7e\xa\xd]{-}[\"\\])|{UEASC})+
	\\({DIGIT}+|[abfnrtv\\\'\"])
	\\[xX][0-9a-fA-F]+
	\" {
		variant_emplace(annotated_string_view{string_view{yyextra->str_buff_begin, yytext}, yyextra->loc_stack.back()});
		yy_pop_state(yyscanner);
		return (bang_lang::parser::token::STRING);
	}
}

<INITIAL>{
	{FN} { variant_emplace(location); return (bang_lang::parser::token::FN); }
	{LET} { return (bang_lang::parser::token::LET); }
	{VAR} { return (bang_lang::parser::token::VAR); }
	{ENUM} { return (bang_lang::parser::token::ENUM); }
	{TYPE} { return (bang_lang::parser::token::TYPE); }
	{EXTENDS} { return (bang_lang::parser::token::EXTENDS); }
	{EXTERN} { return (bang_lang::parser::token::EXTERN); }
	{RETURN} { return (bang_lang::parser::token::RETURN); }

	{BOOL} { return (bang_lang::parser::token::BOOL); }
	{INT} { return (bang_lang::parser::token::INT); }
	{FLOAT} { return (bang_lang::parser::token::FLOAT); }
	{STRING_WORD} { return (bang_lang::parser::token::STRING_WORD); }
	{DECIMAL_WORD} { return (bang_lang::parser::token::DECIMAL_WORD); }

	{WEAK} { return (bang_lang::parser::token::WEAK); }

	{TRUE} { variant_emplace(location); return (bang_lang::parser::token::TRUE); }
	{FALSE} { variant_emplace(location); return (bang_lang::parser::token::FALSE); }
	
	{OPEN_BRACE}			{ return (bang_lang::parser::token::OPEN_BRACE); }
	{CLOSE_BRACE}			{ return (bang_lang::parser::token::CLOSE_BRACE); }
	{OPEN_PARENTHESIS}		{ variant_emplace(location); return (bang_lang::parser::token::OPEN_PARENTHESIS); }
	{CLOSE_PARENTHESIS}		{ return (bang_lang::parser::token::CLOSE_PARENTHESIS);	}
	{END_STATEMENT}			{ return (bang_lang::parser::token::END_STATEMENT); }
	{COMMA}					{ return (bang_lang::parser::token::COMMA);	}
	{COLON}                 { return (bang_lang::parser::token::COLON); }
	{OPEN_SQUARE_BRACKET}	{ variant_emplace(location); return (bang_lang::parser::token::OPEN_SQUARE_BRACKET); }
	{CLOSE_SQUARE_BRACKET}	{ return (bang_lang::parser::token::CLOSE_SQUARE_BRACKET); }
	{ASSIGN}                { variant_emplace(location); return (bang_lang::parser::token::ASSIGN); }
	{POINT}					{ variant_emplace(location); return (bang_lang::parser::token::POINT);	}
	{UNDERSCORE}			{ return (bang_lang::parser::token::UNDERSCORE); }
	{BITOR}				    { return (bang_lang::parser::token::BITOR);	}
	{EXCLPT}                { variant_emplace(location); return (bang_lang::parser::token::EXCLPT); }
	{QMARK}                 { return (bang_lang::parser::token::QMARK); }
	{DBLCOLON}              { return (bang_lang::parser::token::DBLCOLON); }
	{LOGIC_OR}              { variant_emplace(location); return (bang_lang::parser::token::LOGIC_OR); }
    {LOGIC_AND}             { variant_emplace(location); return (bang_lang::parser::token::LOGIC_AND); }
	{ARROW}                 { return (bang_lang::parser::token::ARROW); }
	{CONCAT}                { variant_emplace(location); return (bang_lang::parser::token::CONCAT); }

	{IDENTIFIER} {
		variant_emplace(annotated_string_view{text_view, location});
		return (bang_lang::parser::token::IDENTIFIER);
	}
	{ARGIDENTIFIER} {
		variant_emplace(annotated_string_view{text_view, location});
		return (bang_lang::parser::token::ARGIDENTIFIER);
	}
	{INTEGER} {
		variant_emplace(annotated_decimal{decimal{ctx.make_int(text_view)}, location});
		return (bang_lang::parser::token::INTEGER);
	}
	{DECIMAL} {
		variant_emplace(annotated_decimal{ctx.make_numeric(text_view), location});
		return (bang_lang::parser::token::DECIMAL);
	}
	/*
	{FOR} { return (bang_lang::parser::token::FOR); }
	{VOID} { return (bang_lang::parser::token::VOID_); }
	{AUTO} { return (bang_lang::parser::token::AUTO); }
	{CONST_} { return (bang_lang::parser::token::CONST_); }
	{DELETE_} { return (bang_lang::parser::token::DELETE_); }
	{USING} { return (bang_lang::parser::token::USING); }
	{SIZEOF} { return (bang_lang::parser::token::SIZEOF); }
    
	
	{PRIVATE} { return (bang_lang::parser::token::PRIVATE); }
	{TEMPLATE} { return (bang_lang::parser::token::TEMPLATE); }
	{NAMESPACE} { return (bang_lang::parser::token::NAMESPACE); }


	
	{OPERATOR_TERM0}|{OPERATOR_TERM1}|{OPERATOR_TERM2}|{OPERATOR_TERM3} {
		variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
		return (bang_lang::parser::token::OPERATOR_TERM);
	}

	
	
    
	
    
	



	{OPEN_BROKET}			{ return (bang_lang::parser::token::OPEN_BROKET); }
	{CLOSE_BROKET}			{ return (bang_lang::parser::token::CLOSE_BROKET); }
	
	
	
	
	{HASHTAG}               { return (bang_lang::parser::token::HASHTAG); }
	*/
}

<*><<EOF>>     return (bang_lang::parser::token::END);

%%

/*
	<USER_LITERAL>{
	{UNDERSCORE} { yy_pop_state(yyscanner); return (bang_lang::parser::token::UNDERSCORE); }
	{DBLCOLON} { yy_pop_state(yyscanner); return (bang_lang::parser::token::DBLCOLON); }
	.|[\xd\xa] { yy_pop_state(yyscanner); yyless(0); }
}

DECIMAL_LITERAL			{DECIMAL}"_"



<INITIAL>{DECIMAL} {
	variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
	yy_push_state(NUMERIC, yyscanner);
	return (bang_lang::parser::token::DECIMAL);
}

{DECIMAL} {
    variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
	return (bang_lang::parser::token::DECIMAL);
}

{DECIMAL_LITERAL} {
	variant_emplace(yylval, string_view{yytext, (size_t) yyleng - 1});
	return (bang_lang::parser::token::DECIMAL_LITERAL);
}

<NUMERIC>{
	{UNDERSCORE} { return (bang_lang::parser::token::UNDERSCORE); }
	{DBLCOLON} { return (bang_lang::parser::token::DBLCOLON); }
	{IDENTIFIER} {
		variant_emplace(yylval, string_view{yytext, (size_t) yyleng});
		return (bang_lang::parser::token::IDENTIFIER);
	}

	.|[\xd\xa] { yy_pop_state(yyscanner); --yytext; }
}
------------------------
	//->*
	{ARROWAST}              { return (bang_lang::parser::token::ARROWAST); }

	// ~>
	{FARROW}                { return (bang_lang::parser::token::FARROW); }

	// .*
	{PTAST}                 { return (bang_lang::parser::token::PTAST); }

	// ++
	{DBLPLUS}               { return (bang_lang::parser::token::DBLPLUS); }

	// --
	{DBLMINUS}              { return (bang_lang::parser::token::DBLMINUS); }

	// <<
	{LEFTSHIFT}             { return (bang_lang::parser::token::LEFTSHIFT); }

	// >>
    {RIGHTSHIFT}            { return (bang_lang::parser::token::RIGHTSHIFT); }

	// <=
	{LE}                    { return (bang_lang::parser::token::LE); }

	// >=
	{GE}                    { return (bang_lang::parser::token::GE); }

	// ==
	{EQ}                    { return (bang_lang::parser::token::EQ); }

	// !=
    {NE}                    { return (bang_lang::parser::token::NE); }

	



	// +=
	{ADDASSIGN}             { return (bang_lang::parser::token::ADDASSIGN); }

	// -=
    {SUBASSIGN}             { return (bang_lang::parser::token::SUBASSIGN); }

	// *=
    {MULASSIGN}             { return (bang_lang::parser::token::MULASSIGN); }

	// /=
    {DIVASSIGN}             { return (bang_lang::parser::token::DIVASSIGN); }

	// %=
    {MODASSIGN}             { return (bang_lang::parser::token::MODASSIGN); }

	// <<=
	{LSHIFTSSIGN}           { return (bang_lang::parser::token::LSHIFTSSIGN); }

	// >>=
    {RSHIFTSSIGN}           { return (bang_lang::parser::token::RSHIFTSSIGN); }

	// &=
	{BITANDASSIGN}          { return (bang_lang::parser::token::BITANDASSIGN); }

	// |=
    {BITORASSIGN}           { return (bang_lang::parser::token::BITORASSIGN); }

	// ^=
    {BITXORASSIGN}          { return (bang_lang::parser::token::BITXORASSIGN); }

	// +
	{PLUS}					{ return (bang_lang::parser::token::PLUS); }

	// -
	{MINUS}					{ return (bang_lang::parser::token::MINUS);	}

	// *
	{ASTERISK}				{ return (bang_lang::parser::token::ASTERISK);	}

	// /
	{SLASH}				    { return (bang_lang::parser::token::SLASH);	}

	// %
    {PERCENT}               { return (bang_lang::parser::token::PERCENT);	}

	// &
	{AMPERSAND}				{ return (bang_lang::parser::token::AMPERSAND);	}

	// |
    {BITOR}				    { return (bang_lang::parser::token::BITOR);	}

	// ^
	{EXCL}					{ return (bang_lang::parser::token::EXCL);	}


	
	// ~
	{TILDA}					{ return (bang_lang::parser::token::TILDA);	}

	// .
	

	// $
	{DOLLAR}                { return (bang_lang::parser::token::DOLLAR); }

	// >
	{CLOSE_BROKET}			{ return (bang_lang::parser::token::CLOSE_BROKET); }
*/