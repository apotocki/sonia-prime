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
CONTEXT_IDENTIFIER		${IDENTIFIER}
RESERVED_IDENTIFIER		$([$0]|([1-9][0-9]*))
CT_IDENTIFIER			#{IDENTIFIER}

INTEGER                 [0-9]+
EXPONENT                [eE][+-]?[0-9]+
/*DECIMAL                 ([0-9]+"."?|[0-9]*"."[0-9]+){EXPONENT}?*/
DECIMAL                 [0-9]+"."?[0-9]*{EXPONENT}?
DECIMAL_S               "."[0-9]*{EXPONENT}?
INTEGER_INDEX			"."([0]|([1-9][0-9]*))

FN						 "fn"
LET						 "let"
VAR						 "var"
ENUM					 "enum"
STRUCT					 "struct"
USING					 "using"
EXTENDS					 "extends"
EXTERN                   "extern"
INCLUDE					 "include"
INLINE					 "inline"
NEW						 "new"

CONTINUE                 "continue"
BREAK					 "break"
RETURN                   "return"

WHILE					 "while"
FOR						 "for"
IN						 "in"
IF						 "if"
ELSE					 "else"

NIL_WORD                 "nil"
TRUE_WORD				 "true"
FALSE_WORD				 "false"

WEAK					 "weak"

TYPENAME				 "typename"
CONSTEXPR				 "constexpr"
RUNTIME					 "runtime"

APOSTROPHE				 "`"
AT_SYMBOL				 "@"
END_STATEMENT			 ";"
COMMA					 ","
OPEN_BRACE				 "{"
CLOSE_BRACE				 "}"
OPEN_PARENTHESIS		 "("
CLOSE_PARENTHESIS		 ")"
COLON                    ":"
OPEN_SQUARE_BRACKET      "["
CLOSE_SQUARE_BRACKET     "]"
ASSIGN                   "="
POINT                    "."
UNDERSCORE               "_"
BITOR                    "|"
AMPERSAND                "&"
EXCLPT                   "!"
QMARK                    "?"
PLUS                     "+"
MINUS                    "-"
ASTERISK                 "*"
TILDA                    "~"
OPEN_SQUARE_DBL_BRACKET  "[["
CLOSE_SQUARE_DBL_BRACKET "]]"
DBLCOLON                 "::"
LOGIC_OR                 "||"
LOGIC_AND                "&&"
ARROW                    "->"
ARROWEXPR                "=>"
FARROW					 "~>"
CONCAT                   ".."
EQ                       "=="
NE                       "!="
ELLIPSIS                 "..."

/*
VOID					"void"
AUTO                    "auto"
CONST_                  "const"
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


PTAST                   ".*"

DBLPLUS                 "++"
DBLMINUS                "--"
LEFTSHIFT               "<<"
RIGHTSHIFT              ">>"
LE                      "<="
GE                      ">="




ADDASSIGN               "+="
SUBASSIGN               "-="
MULASSIGN               "*="
DIVASSIGN               "/="
MODASSIGN               "%="
BITANDASSIGN            "&="
BITORASSIGN             "|="
BITXORASSIGN            "^="



SLASH                   "/"
PERCENT                 "%"

TILDA                   "~"
EXCL                    "^"
OPEN_BROKET             "<"
CLOSE_BROKET            ">"



DOLLAR                  "$"

HASHTAG					"#"

*/

%x COMMENT LINE_COMMENT STR STR2 USER_LITERAL

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
<LINE_COMMENT>[^\xd\xa]*[\xd\xa]* { yy_pop_state(yyscanner); }

<INITIAL>
	\" { 
		yyextra->str_buff_begin = yytext + 1;
		yyextra->loc_stack.emplace_back(location);
		yy_push_state(STR, yyscanner);
	}
	\' { 
		yyextra->str_buff_begin = yytext + 1;
		yyextra->loc_stack.emplace_back(location);
		yy_push_state(STR2, yyscanner);
	}

<STR>{
	(([\x20-\x7e\xa\xd]{-}[\"\\])|{UEASC})+
	\\({DIGIT}+|[abfnrtv\\\'\"])
	\\[xX][0-9a-fA-F]+
	\" {
		variant_emplace(annotated_string_view{string_view{yyextra->str_buff_begin, yytext}, yyextra->loc_stack.back()});
		yyextra->loc_stack.pop_back();
		yy_pop_state(yyscanner);
		return (bang_lang::parser::token::STRING);
	}
}

<STR2>{
	(([\x20-\x7e\xa\xd]{-}[\'\\])|{UEASC})+
	\\({DIGIT}+|[abfnrtv\\\'\"])
	\\[xX][0-9a-fA-F]+
	\' {
		variant_emplace(annotated_string_view{string_view{yyextra->str_buff_begin, yytext}, yyextra->loc_stack.back()});
		yyextra->loc_stack.pop_back();
		yy_pop_state(yyscanner);
		return (bang_lang::parser::token::STRING);
	}
}

<INITIAL>{
	{FN} { variant_emplace(location); return (bang_lang::parser::token::FN); }
	{LET} { return (bang_lang::parser::token::LET); }
	{VAR} { return (bang_lang::parser::token::VAR); }
	{ENUM} { return (bang_lang::parser::token::ENUM); }
	{STRUCT} { return (bang_lang::parser::token::STRUCT); }
	{USING} { return (bang_lang::parser::token::USING); }
	{EXTENDS} { return (bang_lang::parser::token::EXTENDS); }
	{EXTERN} { return (bang_lang::parser::token::EXTERN); }
	{INCLUDE} { return (bang_lang::parser::token::INCLUDE); }
	{INLINE} { return (bang_lang::parser::token::INLINE); }
	{NEW} { variant_emplace(location); return (bang_lang::parser::token::NEW); }

	{WHILE} { return (bang_lang::parser::token::WHILE); }
	{FOR} { return (bang_lang::parser::token::FOR); }
	{IN} { return (bang_lang::parser::token::IN); }
	{IF} { return (bang_lang::parser::token::IF); }
	{ELSE} { return (bang_lang::parser::token::ELSE); }

	{CONTINUE} { variant_emplace(location); return (bang_lang::parser::token::CONTINUE); }
	{BREAK} { variant_emplace(location); return (bang_lang::parser::token::BREAK); }
	{RETURN} { variant_emplace(location); return (bang_lang::parser::token::RETURN); }

	{WEAK} { return (bang_lang::parser::token::WEAK); }

	{TYPENAME} { return (bang_lang::parser::token::TYPENAME); }
	{CONSTEXPR} { return (bang_lang::parser::token::CONSTEXPR); }
	{RUNTIME} { return (bang_lang::parser::token::RUNTIME); }

	{NIL_WORD} {
		variant_emplace(annotated_nil{location});
		return (bang_lang::parser::token::NIL_WORD);
	}
	{TRUE_WORD} {
		variant_emplace(annotated_bool{true, location});
		return (bang_lang::parser::token::TRUE_WORD);
	}
	{FALSE_WORD} {
		variant_emplace(annotated_bool{false, location});
		return (bang_lang::parser::token::FALSE_WORD);
	}
	
	{APOSTROPHE}			   { return (bang_lang::parser::token::APOSTROPHE); }
	{AT_SYMBOL}				   { return (bang_lang::parser::token::AT_SYMBOL); }
	{OPEN_BRACE}			   { variant_emplace(location); return (bang_lang::parser::token::OPEN_BRACE); }
	{CLOSE_BRACE}			   { return (bang_lang::parser::token::CLOSE_BRACE); }
	{OPEN_PARENTHESIS}		   { variant_emplace(location); return (bang_lang::parser::token::OPEN_PARENTHESIS); }
	{CLOSE_PARENTHESIS}		   { return (bang_lang::parser::token::CLOSE_PARENTHESIS);	}
	{END_STATEMENT}			   { return (bang_lang::parser::token::END_STATEMENT); }
	{COMMA}					   { return (bang_lang::parser::token::COMMA);	}
	{COLON}                    { return (bang_lang::parser::token::COLON); }
	{OPEN_SQUARE_BRACKET}	   { variant_emplace(location); return (bang_lang::parser::token::OPEN_SQUARE_BRACKET); }
	{CLOSE_SQUARE_BRACKET}	   { return (bang_lang::parser::token::CLOSE_SQUARE_BRACKET); }
	{OPEN_SQUARE_DBL_BRACKET}  { variant_emplace(location); return (bang_lang::parser::token::OPEN_SQUARE_DBL_BRACKET); }
	{CLOSE_SQUARE_DBL_BRACKET} { return (bang_lang::parser::token::CLOSE_SQUARE_DBL_BRACKET); }
	{ASSIGN}                   { variant_emplace(location); return (bang_lang::parser::token::ASSIGN); }
	{POINT}					   { variant_emplace(location); return (bang_lang::parser::token::POINT); }
	{UNDERSCORE}		       { variant_emplace(location); return (bang_lang::parser::token::UNDERSCORE); }
	{BITOR}				       { variant_emplace(location); return (bang_lang::parser::token::BITOR); }
	{AMPERSAND}                { variant_emplace(location); return (bang_lang::parser::token::AMPERSAND); }
	{EXCLPT}                   { variant_emplace(location); return (bang_lang::parser::token::EXCLPT); }
	{QMARK}                    { variant_emplace(location); return (bang_lang::parser::token::QMARK); }
	{DBLCOLON}                 { return (bang_lang::parser::token::DBLCOLON); }
	{PLUS}					   { variant_emplace(location); return (bang_lang::parser::token::PLUS); }
	{MINUS}					   { variant_emplace(location); return (bang_lang::parser::token::MINUS); }
	{ASTERISK}				   { variant_emplace(location); return (bang_lang::parser::token::ASTERISK); }
	{TILDA}                    { return (bang_lang::parser::token::TILDA); }
	{LOGIC_OR}                 { variant_emplace(location); return (bang_lang::parser::token::LOGIC_OR); }
    {LOGIC_AND}                { variant_emplace(location); return (bang_lang::parser::token::LOGIC_AND); }
	{ARROW}                    { return (bang_lang::parser::token::ARROW); }
	{ARROWEXPR}				   { return (bang_lang::parser::token::ARROWEXPR); }
	{FARROW}				   { return (bang_lang::parser::token::FARROW); }
	{CONCAT}                   { variant_emplace(location); return (bang_lang::parser::token::CONCAT); }
	{EQ}					   { variant_emplace(location); return (bang_lang::parser::token::EQ); }
	{NE}					   { variant_emplace(location); return (bang_lang::parser::token::NE); }
	{ELLIPSIS}                 { variant_emplace(location); return (bang_lang::parser::token::ELLIPSIS); }

	{IDENTIFIER} {
		variant_emplace(annotated_string_view{ text_view, location });
		return (bang_lang::parser::token::IDENTIFIER);
	}
	{CONTEXT_IDENTIFIER} {
		variant_emplace(annotated_string_view{ text_view, location });
		return (bang_lang::parser::token::CONTEXT_IDENTIFIER);
	}
	{RESERVED_IDENTIFIER}  {
		variant_emplace(annotated_string_view{ text_view, location });
		return (bang_lang::parser::token::RESERVED_IDENTIFIER);
	}
	{CT_IDENTIFIER} {
		variant_emplace(annotated_string_view{ text_view, location });
		return (bang_lang::parser::token::CT_IDENTIFIER);
	}

	{INTEGER} {
		variant_emplace(annotated_integer{ ctx.make_integer(text_view), location });
		return (bang_lang::parser::token::INTEGER);
	}
	{INTEGER_INDEX} {
		variant_emplace(annotated_string_view{ text_view, location });
		return (bang_lang::parser::token::INTEGER_INDEX);
	}
	{DECIMAL} {
		variant_emplace(annotated_decimal{ ctx.make_decimal(text_view), location });
		return (bang_lang::parser::token::DECIMAL);
	}
	{DECIMAL_S} {
		variant_emplace(annotated_decimal{ctx.make_decimal(text_view), location});
		return (bang_lang::parser::token::DECIMAL);
	}
	/*
	
	{VOID} { return (bang_lang::parser::token::VOID_); }
	{AUTO} { return (bang_lang::parser::token::AUTO); }
	{CONST_} { return (bang_lang::parser::token::CONST_); }
	{DELETE_} { return (bang_lang::parser::token::DELETE_); }
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


	// /
	{SLASH}				    { return (bang_lang::parser::token::SLASH);	}

	// %
    {PERCENT}               { return (bang_lang::parser::token::PERCENT);	}



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