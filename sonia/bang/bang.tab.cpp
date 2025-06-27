// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.


// Take the name prefix into account.
#define yylex   bang_langlex

// First part of user prologue.
#line 25 "bang.y"

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




#line 75 "bang.tab.cpp"


#include "bang.tab.hpp"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if BANG_LANGDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !BANG_LANGDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !BANG_LANGDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace bang_lang {
#line 172 "bang.tab.cpp"

  /// Build a parser object.
  parser::parser (void * scanner_yyarg, parser_context & ctx_yyarg)
#if BANG_LANGDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      ctx (ctx_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.copy< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.copy< annotated_nil > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_164_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.copy< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
        value.copy< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
      case symbol_kind::S_EQ: // "`==`"
      case symbol_kind::S_NE: // "`!=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_ELLIPSIS: // "`...`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
        value.copy< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
        value.copy< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
      case symbol_kind::S_157_syntax_expression: // syntax-expression
      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_160_new_expression: // new-expression
      case symbol_kind::S_161_call_expression: // call-expression
      case symbol_kind::S_162_lambda_expression: // lambda-expression
      case symbol_kind::S_165_compound_expression: // compound-expression
      case symbol_kind::S_166_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
        value.copy< using_decl > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.move< annotated_bool > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.move< annotated_nil > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_164_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
      case symbol_kind::S_EQ: // "`==`"
      case symbol_kind::S_NE: // "`!=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_ELLIPSIS: // "`...`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
        value.move< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
        value.move< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
      case symbol_kind::S_157_syntax_expression: // syntax-expression
      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_160_new_expression: // new-expression
      case symbol_kind::S_161_call_expression: // call-expression
      case symbol_kind::S_162_lambda_expression: // lambda-expression
      case symbol_kind::S_165_compound_expression: // compound-expression
      case symbol_kind::S_166_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
        value.move< using_decl > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.YY_MOVE_OR_COPY< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.YY_MOVE_OR_COPY< annotated_nil > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
        value.YY_MOVE_OR_COPY< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_164_pack_expression: // pack-expression
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.YY_MOVE_OR_COPY< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.YY_MOVE_OR_COPY< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
        value.YY_MOVE_OR_COPY< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
      case symbol_kind::S_EQ: // "`==`"
      case symbol_kind::S_NE: // "`!=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_ELLIPSIS: // "`...`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.YY_MOVE_OR_COPY< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
        value.YY_MOVE_OR_COPY< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
        value.YY_MOVE_OR_COPY< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
      case symbol_kind::S_157_syntax_expression: // syntax-expression
      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_160_new_expression: // new-expression
      case symbol_kind::S_161_call_expression: // call-expression
      case symbol_kind::S_162_lambda_expression: // lambda-expression
      case symbol_kind::S_165_compound_expression: // compound-expression
      case symbol_kind::S_166_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
        value.YY_MOVE_OR_COPY< using_decl > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.move< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.move< annotated_nil > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_164_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
      case symbol_kind::S_EQ: // "`==`"
      case symbol_kind::S_NE: // "`!=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_ELLIPSIS: // "`...`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
        value.move< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
        value.move< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
      case symbol_kind::S_157_syntax_expression: // syntax-expression
      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_160_new_expression: // new-expression
      case symbol_kind::S_161_call_expression: // call-expression
      case symbol_kind::S_162_lambda_expression: // lambda-expression
      case symbol_kind::S_165_compound_expression: // compound-expression
      case symbol_kind::S_166_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
        value.move< using_decl > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.copy< annotated_bool > (that.value);
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.copy< annotated_nil > (that.value);
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_164_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.copy< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (that.value);
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
        value.copy< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_128_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
      case symbol_kind::S_EQ: // "`==`"
      case symbol_kind::S_NE: // "`!=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_ELLIPSIS: // "`...`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
        value.copy< std::pair<pattern_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
        value.copy< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
      case symbol_kind::S_157_syntax_expression: // syntax-expression
      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_160_new_expression: // new-expression
      case symbol_kind::S_161_call_expression: // call-expression
      case symbol_kind::S_162_lambda_expression: // lambda-expression
      case symbol_kind::S_165_compound_expression: // compound-expression
      case symbol_kind::S_166_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_130_using_decl: // using-decl
        value.copy< using_decl > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.move< annotated_bool > (that.value);
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.move< annotated_nil > (that.value);
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_164_pack_expression: // pack-expression
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.move< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (that.value);
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
        value.move< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_128_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
      case symbol_kind::S_EQ: // "`==`"
      case symbol_kind::S_NE: // "`!=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_ELLIPSIS: // "`...`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
        value.move< std::pair<pattern_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
        value.move< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
      case symbol_kind::S_157_syntax_expression: // syntax-expression
      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_160_new_expression: // new-expression
      case symbol_kind::S_161_call_expression: // call-expression
      case symbol_kind::S_162_lambda_expression: // lambda-expression
      case symbol_kind::S_165_compound_expression: // compound-expression
      case symbol_kind::S_166_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_130_using_decl: // using-decl
        value.move< using_decl > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if BANG_LANGDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_STRING: // STRING
#line 327 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 327 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 327 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 327 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 327 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 327 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 327 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 327 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 327 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 327 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 327 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 327 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 327 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 327 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 327 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 327 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 327 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 327 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 327 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 327 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 327 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 327 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 327 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 327 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 327 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 327 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 327 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 327 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 327 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 327 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 327 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 327 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 327 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 327 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
#line 327 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 327 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl: // let-decl
#line 327 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start: // let-decl-start
#line 327 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 327 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
#line 327 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
#line 327 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_braced_statements: // braced-statements
#line 327 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
#line 327 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
#line 327 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
#line 327 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_infunction_statement: // infunction-statement
#line 327 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 327 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_internal_identifier: // internal-identifier
#line 327 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 327 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
#line 327 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_name: // fn-name
#line 327 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
#line 327 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
#line 327 "bang.y"
                 { }
#line 2010 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
#line 327 "bang.y"
                 { }
#line 2016 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_list: // case-list
#line 327 "bang.y"
                 { }
#line 2022 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_case_decl: // case-decl
#line 327 "bang.y"
                 { }
#line 2028 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
#line 327 "bang.y"
                 { }
#line 2034 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_using_decl: // using-decl
#line 327 "bang.y"
                 { }
#line 2040 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_expression_list: // expression-list
#line 327 "bang.y"
                 { }
#line 2046 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
#line 327 "bang.y"
                 { }
#line 2052 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_argument_list: // argument-list
#line 327 "bang.y"
                 { }
#line 2058 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 327 "bang.y"
                 { }
#line 2064 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_argument_name: // argument-name
#line 327 "bang.y"
                 { }
#line 2070 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
#line 327 "bang.y"
                 { }
#line 2076 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_list: // field-list
#line 327 "bang.y"
                 { }
#line 2082 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
#line 327 "bang.y"
                 { }
#line 2088 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 327 "bang.y"
                 { }
#line 2094 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
#line 327 "bang.y"
                 { }
#line 2100 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_list: // parameter-list
#line 327 "bang.y"
                 { }
#line 2106 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
#line 327 "bang.y"
                 { }
#line 2112 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
#line 327 "bang.y"
                 { }
#line 2118 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
#line 327 "bang.y"
                 { }
#line 2124 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
#line 327 "bang.y"
                 { }
#line 2130 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
#line 327 "bang.y"
                 { }
#line 2136 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
#line 327 "bang.y"
                 { }
#line 2142 "bang.tab.cpp"
        break;

      case symbol_kind::S_subpatterns: // subpatterns
#line 327 "bang.y"
                 { }
#line 2148 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_pattern_list: // pattern-list
#line 327 "bang.y"
                 { }
#line 2154 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
#line 327 "bang.y"
                 { }
#line 2160 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
#line 327 "bang.y"
                 { }
#line 2166 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
#line 327 "bang.y"
                 { }
#line 2172 "bang.tab.cpp"
        break;

      case symbol_kind::S_pattern: // pattern
#line 327 "bang.y"
                 { }
#line 2178 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
#line 327 "bang.y"
                 { }
#line 2184 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
#line 327 "bang.y"
                 { }
#line 2190 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
#line 327 "bang.y"
                 { }
#line 2196 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_syntax_expression: // syntax-expression
#line 327 "bang.y"
                 { }
#line 2202 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 327 "bang.y"
                 { }
#line 2208 "bang.tab.cpp"
        break;

      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
#line 327 "bang.y"
                 { }
#line 2214 "bang.tab.cpp"
        break;

      case symbol_kind::S_160_new_expression: // new-expression
#line 327 "bang.y"
                 { }
#line 2220 "bang.tab.cpp"
        break;

      case symbol_kind::S_161_call_expression: // call-expression
#line 327 "bang.y"
                 { }
#line 2226 "bang.tab.cpp"
        break;

      case symbol_kind::S_162_lambda_expression: // lambda-expression
#line 327 "bang.y"
                 { }
#line 2232 "bang.tab.cpp"
        break;

      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
#line 327 "bang.y"
                 { }
#line 2238 "bang.tab.cpp"
        break;

      case symbol_kind::S_164_pack_expression: // pack-expression
#line 327 "bang.y"
                 { }
#line 2244 "bang.tab.cpp"
        break;

      case symbol_kind::S_165_compound_expression: // compound-expression
#line 327 "bang.y"
                 { }
#line 2250 "bang.tab.cpp"
        break;

      case symbol_kind::S_166_type_expr: // type-expr
#line 327 "bang.y"
                 { }
#line 2256 "bang.tab.cpp"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if BANG_LANGDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // BANG_LANGDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location, ctx, scanner));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        yylhs.value.emplace< annotated_bool > ();
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        yylhs.value.emplace< annotated_nil > ();
        break;

      case symbol_kind::S_155_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_156_concept_expression_list: // concept-expression-list
        yylhs.value.emplace< concept_expression_list_t > ();
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< managed_statement_list > ();
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_163_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_164_pack_expression: // pack-expression
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        yylhs.value.emplace< pattern_list_t > ();
        break;

      case symbol_kind::S_pattern: // pattern
        yylhs.value.emplace< pattern_t > ();
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
        yylhs.value.emplace< pattern_t::field > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_128_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::bang::identifier > ();
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
      case symbol_kind::S_EQ: // "`==`"
      case symbol_kind::S_NE: // "`!=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_ELLIPSIS: // "`...`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        yylhs.value.emplace< sonia::optional<syntax_expression_t> > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_151_pattern_mod: // pattern-mod
      case symbol_kind::S_152_pattern_sfx: // pattern-sfx
        yylhs.value.emplace< std::pair<pattern_t, parameter_constraint_modifier_t> > ();
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
        yylhs.value.emplace< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ();
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_154_concept_expression: // concept-expression
      case symbol_kind::S_157_syntax_expression: // syntax-expression
      case symbol_kind::S_158_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_159_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_160_new_expression: // new-expression
      case symbol_kind::S_161_call_expression: // call-expression
      case symbol_kind::S_162_lambda_expression: // lambda-expression
      case symbol_kind::S_165_compound_expression: // compound-expression
      case symbol_kind::S_166_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_130_using_decl: // using-decl
        yylhs.value.emplace< using_decl > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // begin: statement_any "end of file"
#line 332 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2717 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 333 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2723 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 338 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2729 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 340 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2735 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 342 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2741 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 359 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2747 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 360 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2753 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 362 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2759 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 364 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2765 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 369 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), &ctx.push(std::move(yystack_[0].value.as < syntax_expression_t > ())) }; }
#line 2771 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 371 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2777 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 373 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2783 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-name "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 375 "bang.y"
        { 
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) });
            yylhs.value.as < statement > () = fn_decl_t{ fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = nullptr, .kind = yystack_[6].value.as < fn_kind > () }, ctx.push(std::move(sts)) };
            IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
            //     $$ = fn_decl_t{ fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .result = std::move($value), .is_type_expression_result = false, .kind = $fnkind } }; IGNORE_TERM($OPEN_PARENTHESIS); }
        }
#line 2796 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 384 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2802 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 386 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2808 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 391 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2814 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 393 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2820 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: identifier
#line 398 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2826 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: "weak modifier" identifier
#line 400 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2832 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 404 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2838 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 406 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2844 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 411 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2850 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 413 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2856 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 415 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2862 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 420 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2868 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 421 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2874 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 423 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2880 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 425 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2886 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 430 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2892 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 435 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2898 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 437 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2904 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 439 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2910 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 441 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2916 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 443 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2922 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 445 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2928 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 447 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2934 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 449 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2940 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 451 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2946 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 456 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2952 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 458 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2958 "bang.tab.cpp"
    break;

  case 42: // generic-statement: LET let-decl
#line 463 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2964 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 465 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2970 "bang.tab.cpp"
    break;

  case 44: // generic-statement: compound-expression
#line 467 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2976 "bang.tab.cpp"
    break;

  case 45: // generic-statement: syntax-expression "`=`" syntax-expression
#line 469 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2982 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: generic-statement
#line 474 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2988 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: "`break`"
#line 476 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2994 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`continue`"
#line 478 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3000 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`return`"
#line 480 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .location = std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3006 "bang.tab.cpp"
    break;

  case 50: // infunction-statement: "`return`" syntax-expression
#line 482 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3012 "bang.tab.cpp"
    break;

  case 51: // identifier: IDENTIFIER
#line 487 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3018 "bang.tab.cpp"
    break;

  case 52: // internal-identifier: CONTEXT_IDENTIFIER
#line 495 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3024 "bang.tab.cpp"
    break;

  case 53: // qname: "`::`" identifier
#line 510 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 3030 "bang.tab.cpp"
    break;

  case 54: // qname: identifier
#line 512 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 3036 "bang.tab.cpp"
    break;

  case 55: // qname: qname "`::`" identifier
#line 514 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 3042 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: "`fn`"
#line 520 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3048 "bang.tab.cpp"
    break;

  case 57: // fn-start-decl: INLINE "`fn`"
#line 522 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3054 "bang.tab.cpp"
    break;

  case 58: // fn-name: qname
#line 526 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 3060 "bang.tab.cpp"
    break;

  case 59: // fn-name: "`new`"
#line 527 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 3066 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 532 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3072 "bang.tab.cpp"
    break;

  case 61: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`~>`" type-expr
#line 534 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3078 "bang.tab.cpp"
    break;

  case 62: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" pattern
#line 536 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3084 "bang.tab.cpp"
    break;

  case 63: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 542 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3090 "bang.tab.cpp"
    break;

  case 64: // case-list-opt: %empty
#line 546 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 3096 "bang.tab.cpp"
    break;

  case 65: // case-list-opt: case-list
#line 547 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 3102 "bang.tab.cpp"
    break;

  case 66: // case-list: case-decl
#line 552 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 3108 "bang.tab.cpp"
    break;

  case 67: // case-list: case-list "," case-decl
#line 554 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 3114 "bang.tab.cpp"
    break;

  case 68: // case-decl: identifier
#line 559 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 3120 "bang.tab.cpp"
    break;

  case 69: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 565 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3126 "bang.tab.cpp"
    break;

  case 70: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 567 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3132 "bang.tab.cpp"
    break;

  case 71: // using-decl: qname "`=>`" syntax-expression
#line 590 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 3140 "bang.tab.cpp"
    break;

  case 72: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 594 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3148 "bang.tab.cpp"
    break;

  case 73: // expression-list: syntax-expression
#line 608 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3154 "bang.tab.cpp"
    break;

  case 74: // expression-list: expression-list "," syntax-expression
#line 610 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3160 "bang.tab.cpp"
    break;

  case 75: // argument-list-opt: %empty
#line 615 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 3166 "bang.tab.cpp"
    break;

  case 76: // argument-list-opt: argument-list
#line 616 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 3172 "bang.tab.cpp"
    break;

  case 77: // argument-list: argument
#line 621 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 3178 "bang.tab.cpp"
    break;

  case 78: // argument-list: argument-list "," argument
#line 623 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 3184 "bang.tab.cpp"
    break;

  case 79: // argument: argument-name syntax-expression
#line 628 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3190 "bang.tab.cpp"
    break;

  case 80: // argument: syntax-expression
#line 630 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3196 "bang.tab.cpp"
    break;

  case 81: // argument-name: identifier "`=`"
#line 635 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3202 "bang.tab.cpp"
    break;

  case 82: // argument-name: identifier "`:`"
#line 637 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 3208 "bang.tab.cpp"
    break;

  case 83: // field-list-opt: %empty
#line 642 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 3214 "bang.tab.cpp"
    break;

  case 84: // field-list-opt: field-list
#line 643 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 3220 "bang.tab.cpp"
    break;

  case 85: // field-list: field
#line 648 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 3226 "bang.tab.cpp"
    break;

  case 86: // field-list: field-list "," field
#line 650 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 3232 "bang.tab.cpp"
    break;

  case 87: // field-default-value-opt: %empty
#line 654 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3238 "bang.tab.cpp"
    break;

  case 88: // field-default-value-opt: "`=`" syntax-expression
#line 655 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3244 "bang.tab.cpp"
    break;

  case 89: // field: identifier "`:`" type-expr field-default-value-opt
#line 660 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3250 "bang.tab.cpp"
    break;

  case 90: // field: identifier "`=>`" type-expr field-default-value-opt
#line 662 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3256 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 672 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3262 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 673 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3268 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 678 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3274 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 680 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3280 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 684 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3286 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 685 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3292 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 689 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3298 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 690 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3304 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" pattern-mod parameter-default-value-opt
#line 695 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3310 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: internal-identifier "`:`" pattern-mod parameter-default-value-opt
#line 697 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3316 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: pattern-mod parameter-default-value-opt
#line 699 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{}, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3322 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`:`" constraint-expression parameter-default-value-opt
#line 701 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3328 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: internal-identifier "`:`" constraint-expression parameter-default-value-opt
#line 703 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3334 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: constraint-expression parameter-default-value-opt
#line 705 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3340 "bang.tab.cpp"
    break;

  case 105: // constraint-expression-mod: "`~`"
#line 709 "bang.y"
            { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::const_or_runtime_type; }
#line 3346 "bang.tab.cpp"
    break;

  case 106: // constraint-expression-mod: "`~`" "constexpr modifier"
#line 710 "bang.y"
                      { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::const_type; }
#line 3352 "bang.tab.cpp"
    break;

  case 107: // constraint-expression-mod: "`~`" "runctime modifier"
#line 711 "bang.y"
                    { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::runtime_type; }
#line 3358 "bang.tab.cpp"
    break;

  case 108: // constraint-expression: constraint-expression-mod type-expr
#line 716 "bang.y"
        { yylhs.value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[0].value.as < syntax_expression_t > ()), yystack_[1].value.as < parameter_constraint_modifier_t > () }; }
#line 3364 "bang.tab.cpp"
    break;

  case 109: // constraint-expression: constraint-expression-mod type-expr "`...`"
#line 718 "bang.y"
        { yylhs.value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[1].value.as < syntax_expression_t > ()), yystack_[2].value.as < parameter_constraint_modifier_t > () | parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3370 "bang.tab.cpp"
    break;

  case 110: // subpatterns-opt: %empty
#line 724 "bang.y"
      { yylhs.value.as < pattern_list_t > () = pattern_list_t{}; }
#line 3376 "bang.tab.cpp"
    break;

  case 111: // subpatterns-opt: subpatterns
#line 725 "bang.y"
      { yylhs.value.as < pattern_list_t > () = yystack_[0].value.as < pattern_list_t > (); }
#line 3382 "bang.tab.cpp"
    break;

  case 112: // subpatterns: "`(`" pattern-list "`)`"
#line 730 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[1].value.as < pattern_list_t > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3388 "bang.tab.cpp"
    break;

  case 113: // pattern-list: pattern-field
#line 735 "bang.y"
        { yylhs.value.as < pattern_list_t > () = pattern_list_t{ std::move(yystack_[0].value.as < pattern_t::field > ()) }; }
#line 3394 "bang.tab.cpp"
    break;

  case 114: // pattern-list: pattern-list "," pattern-field
#line 737 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[2].value.as < pattern_list_t > ()); yylhs.value.as < pattern_list_t > ().emplace_back(std::move(yystack_[0].value.as < pattern_t::field > ())); }
#line 3400 "bang.tab.cpp"
    break;

  case 115: // pattern-field: pattern-sfx
#line 742 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3406 "bang.tab.cpp"
    break;

  case 116: // pattern-field: "`_`" "`:`" pattern-sfx
#line 744 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3412 "bang.tab.cpp"
    break;

  case 117: // pattern-field: identifier "`:`" pattern-sfx
#line 746 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3418 "bang.tab.cpp"
    break;

  case 118: // pattern-field: internal-identifier "`:`" pattern-sfx
#line 748 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3424 "bang.tab.cpp"
    break;

  case 119: // pattern-field: "`(`" syntax-expression "`)`" "`:`" pattern-sfx
#line 750 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[3].value.as < syntax_expression_t > ()), .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3430 "bang.tab.cpp"
    break;

  case 120: // pattern-mod: pattern-sfx
#line 754 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::const_or_runtime_type }; }
#line 3436 "bang.tab.cpp"
    break;

  case 121: // pattern-mod: "constexpr modifier" pattern-sfx
#line 755 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::const_type }; }
#line 3442 "bang.tab.cpp"
    break;

  case 122: // pattern-mod: "runctime modifier" pattern-sfx
#line 756 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::runtime_type }; }
#line 3448 "bang.tab.cpp"
    break;

  case 123: // pattern-sfx: pattern
#line 760 "bang.y"
              { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[0].value.as < pattern_t > ()), parameter_constraint_modifier_t::none }; }
#line 3454 "bang.tab.cpp"
    break;

  case 124: // pattern-sfx: pattern "`...`"
#line 761 "bang.y"
                       { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[1].value.as < pattern_t > ()), parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3460 "bang.tab.cpp"
    break;

  case 125: // pattern-sfx: "`...`"
#line 762 "bang.y"
               { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ pattern_t{ .descriptor = placeholder{} }, parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3466 "bang.tab.cpp"
    break;

  case 126: // pattern: qname subpatterns-opt concept-expression-list-opt
#line 767 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3472 "bang.tab.cpp"
    break;

  case 127: // pattern: internal-identifier concept-expression-list-opt
#line 769 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3478 "bang.tab.cpp"
    break;

  case 128: // pattern: "`_`" subpatterns concept-expression-list-opt
#line 771 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3484 "bang.tab.cpp"
    break;

  case 129: // pattern: "`(`" syntax-expression "`)`" concept-expression-list-opt
#line 773 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[2].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3490 "bang.tab.cpp"
    break;

  case 130: // pattern: "`_`" concept-expression-list-opt
#line 775 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3496 "bang.tab.cpp"
    break;

  case 131: // pattern: concept-expression-list
#line 777 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3502 "bang.tab.cpp"
    break;

  case 132: // concept-expression: "`@`" qname
#line 782 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_reference{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3508 "bang.tab.cpp"
    break;

  case 133: // concept-expression-list-opt: %empty
#line 787 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = {}; }
#line 3514 "bang.tab.cpp"
    break;

  case 134: // concept-expression-list-opt: concept-expression-list
#line 788 "bang.y"
      { yylhs.value.as < concept_expression_list_t > () = yystack_[0].value.as < concept_expression_list_t > (); }
#line 3520 "bang.tab.cpp"
    break;

  case 135: // concept-expression-list: concept-expression
#line 793 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = concept_expression_list_t{std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3526 "bang.tab.cpp"
    break;

  case 136: // concept-expression-list: concept-expression-list concept-expression
#line 795 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = std::move(yystack_[1].value.as < concept_expression_list_t > ()); yylhs.value.as < concept_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3532 "bang.tab.cpp"
    break;

  case 137: // syntax-expression: CONTEXT_IDENTIFIER
#line 803 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3538 "bang.tab.cpp"
    break;

  case 138: // syntax-expression: CT_IDENTIFIER
#line 805 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3544 "bang.tab.cpp"
    break;

  case 139: // syntax-expression: syntax-expression-wo-ii
#line 806 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3550 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: "nil"
#line 812 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3556 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: "true"
#line 814 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3562 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: "false"
#line 816 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3568 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: INTEGER
#line 818 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3574 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: DECIMAL
#line 820 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3580 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: DECIMAL_S
#line 822 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3586 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: INTEGER_INDEX
#line 824 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3592 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: STRING
#line 826 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3598 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 828 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3604 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: qname
#line 830 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3610 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: "`(`" "`)`"
#line 832 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3616 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: "`(`" "`:`" syntax-expression "`)`"
#line 834 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, named_expression_list_t{ named_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) } } }; }
#line 3622 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 836 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                BOOST_ASSERT(!yystack_[1].value.as < named_expression_list_t > ().empty());
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3635 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 845 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3647 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 853 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3653 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 855 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3659 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: "`.`" identifier
#line 857 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3665 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 861 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3671 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 863 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3677 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 865 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3683 "bang.tab.cpp"
    break;

  case 160: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 868 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3689 "bang.tab.cpp"
    break;

  case 161: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 870 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3695 "bang.tab.cpp"
    break;

  case 162: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 875 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3701 "bang.tab.cpp"
    break;

  case 163: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 877 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3707 "bang.tab.cpp"
    break;

  case 164: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 881 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3713 "bang.tab.cpp"
    break;

  case 165: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 883 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3719 "bang.tab.cpp"
    break;

  case 166: // syntax-expression-wo-ii: syntax-expression "`..`" syntax-expression
#line 885 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3725 "bang.tab.cpp"
    break;

  case 167: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 888 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3731 "bang.tab.cpp"
    break;

  case 168: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 892 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3737 "bang.tab.cpp"
    break;

  case 169: // syntax-expression-wo-ii: syntax-expression "`&&`" syntax-expression
#line 895 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3743 "bang.tab.cpp"
    break;

  case 170: // syntax-expression-wo-ii: syntax-expression "`||`" syntax-expression
#line 898 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3749 "bang.tab.cpp"
    break;

  case 171: // syntax-expression-wo-ii: apostrophe-expression
#line 900 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3755 "bang.tab.cpp"
    break;

  case 172: // syntax-expression-wo-ii: new-expression
#line 901 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3761 "bang.tab.cpp"
    break;

  case 173: // syntax-expression-wo-ii: compound-expression
#line 902 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3767 "bang.tab.cpp"
    break;

  case 174: // syntax-expression-wo-ii: lambda-expression
#line 903 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3773 "bang.tab.cpp"
    break;

  case 175: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 908 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3779 "bang.tab.cpp"
    break;

  case 176: // new-expression: "`new`" qname
#line 913 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3785 "bang.tab.cpp"
    break;

  case 177: // new-expression: "`new`" apostrophe-expression
#line 915 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3791 "bang.tab.cpp"
    break;

  case 178: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 917 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3797 "bang.tab.cpp"
    break;

  case 179: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 919 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3803 "bang.tab.cpp"
    break;

  case 180: // call-expression: qname "`(`" pack-expression-opt "`)`"
#line 924 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3809 "bang.tab.cpp"
    break;

  case 181: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 932 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3815 "bang.tab.cpp"
    break;

  case 182: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 934 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3821 "bang.tab.cpp"
    break;

  case 183: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3827 "bang.tab.cpp"
    break;

  case 184: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 941 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3833 "bang.tab.cpp"
    break;

  case 185: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 943 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3839 "bang.tab.cpp"
    break;

  case 186: // pack-expression-opt: %empty
#line 948 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{}; }
#line 3845 "bang.tab.cpp"
    break;

  case 187: // pack-expression-opt: pack-expression
#line 950 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[0].value.as < named_expression_list_t > ()); }
#line 3851 "bang.tab.cpp"
    break;

  case 188: // pack-expression: syntax-expression
#line 955 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{ named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3857 "bang.tab.cpp"
    break;

  case 189: // pack-expression: identifier "`:`" syntax-expression
#line 964 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3867 "bang.tab.cpp"
    break;

  case 190: // pack-expression: pack-expression "," syntax-expression
#line 970 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3876 "bang.tab.cpp"
    break;

  case 191: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 975 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3885 "bang.tab.cpp"
    break;

  case 192: // compound-expression: syntax-expression "`...`"
#line 983 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3891 "bang.tab.cpp"
    break;

  case 193: // compound-expression: call-expression
#line 984 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3897 "bang.tab.cpp"
    break;

  case 194: // type-expr: qname
#line 1026 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3903 "bang.tab.cpp"
    break;

  case 195: // type-expr: call-expression
#line 1027 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3909 "bang.tab.cpp"
    break;

  case 196: // type-expr: CONTEXT_IDENTIFIER
#line 1029 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3915 "bang.tab.cpp"
    break;

  case 197: // type-expr: "`[`" type-expr "`]`"
#line 1033 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3921 "bang.tab.cpp"
    break;

  case 198: // type-expr: "`(`" "`)`"
#line 1035 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3927 "bang.tab.cpp"
    break;

  case 199: // type-expr: "`(`" pack-expression "`)`"
#line 1038 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3933 "bang.tab.cpp"
    break;

  case 200: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1040 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3939 "bang.tab.cpp"
    break;

  case 201: // type-expr: type-expr "`|`" type-expr
#line 1042 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3951 "bang.tab.cpp"
    break;

  case 202: // type-expr: type-expr "`->`" type-expr
#line 1050 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 3960 "bang.tab.cpp"
    break;


#line 3964 "bang.tab.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -242;

  const signed char parser::yytable_ninf_ = -96;

  const short
  parser::yypact_[] =
  {
     712,  -242,  -242,  -242,  -242,  -242,  -242,  -242,  -242,  -242,
     911,    41,   811,   911,   911,    41,   911,   911,    -3,   -32,
     127,   911,   911,   911,   129,   115,    65,  -242,   129,   129,
    -242,  -242,  -242,   162,    30,   415,  -242,  -242,  -242,  -242,
      79,     2,   716,  -242,   142,  -242,   160,   167,    38,   185,
     920,  -242,  -242,   911,  -242,   193,  1090,    90,   245,  1090,
     -18,  -242,   141,   141,    41,  -242,   227,   190,  -242,    41,
       9,   266,   228,   618,   504,   959,   208,  -242,  -242,  -242,
       1,  -242,   215,  -242,  -242,  -242,  -242,  -242,  -242,  -242,
    -242,    41,   911,   433,  -242,   257,   259,   216,  -242,   911,
     911,   911,   911,   911,   911,  -242,   911,   159,   911,   911,
     911,   911,   911,   911,   911,  -242,   996,   911,  -242,   911,
    -242,   911,  -242,  -242,   342,   911,   271,   272,  -242,   911,
     911,   613,   911,  -242,   911,   243,   911,   433,    41,   281,
     433,  -242,  -242,   282,   260,  -242,   129,    43,  -242,   911,
     219,   358,   358,    13,   131,   275,   283,   264,  -242,   342,
     325,   325,  -242,   308,  -242,   327,   433,  -242,  1090,    53,
      53,  1123,   837,    16,  1006,  -242,  -242,   141,   141,   424,
     702,   179,   218,   220,  -242,  1090,   300,  1090,  1090,  -242,
     832,   342,    79,   142,   160,   167,   165,   260,   342,   433,
      21,   302,   284,  -242,   911,  1090,   307,  -242,  -242,   911,
     129,   299,   514,  -242,   309,  -242,  -242,     2,   959,   959,
     216,  1090,   313,  -242,   315,   297,  -242,    41,   319,  -242,
     257,   542,   327,  -242,   327,  1043,  -242,  -242,   327,  -242,
    -242,  -242,   323,   433,  -242,   327,  -242,     5,   433,    -8,
     911,  -242,  -242,  -242,  -242,   321,  -242,  -242,  -242,  -242,
     911,  -242,   233,   239,   342,   361,   342,   165,   324,  -242,
    -242,  -242,   911,  1090,  -242,  1090,   261,  -242,  -242,  -242,
    -242,  -242,  -242,  -242,  -242,   357,  -242,    41,    69,   332,
     312,  -242,   157,   140,   911,   337,   170,   246,  -242,  -242,
    -242,   327,   433,   325,   325,  -242,   342,  -242,  -242,  -242,
    1090,   221,  1090,  -242,  -242,  -242,   331,    19,    20,  -242,
     433,   911,  -242,   342,   342,  -242,    41,   338,  -242,   358,
    1080,   358,   358,  -242,   542,  -242,   325,   325,  -242,  -242,
      72,   439,   911,   342,  -242,   336,  1090,   231,   231,  -242,
      41,  -242,   194,  -242,  -242,  -242,  -242,  -242,  -242,  -242,
    1090,   165,   216,   911,  -242,  -242,   349,   358,  1090,  -242,
    -242
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   147,    51,   137,   148,   138,   146,   143,   144,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
     140,   141,   142,     0,     0,     0,     5,     7,    15,    54,
     149,     0,     0,   139,   171,   172,   193,   174,   173,     0,
       0,   173,    53,     0,   150,    54,   188,     0,     0,    73,
       0,   156,   161,   160,     0,    42,    21,    17,    19,     0,
       0,   176,   177,     0,     0,     0,     0,    43,    13,    57,
       0,    39,     0,    16,     1,     2,    10,     3,     8,     6,
       9,     0,   186,    91,    59,    58,     0,     0,   159,     0,
       0,     0,     0,     0,     0,   192,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,     0,     0,   152,     0,
     153,     0,   154,    20,     0,     0,     0,     0,    12,    75,
      75,    23,     0,    31,     0,    34,     0,    91,    64,     0,
      91,    37,    55,     0,   187,    52,     0,   133,   125,     0,
     105,     0,     0,    54,   133,   110,     0,    92,    93,     0,
      97,    97,   120,   123,   135,   131,    91,    36,    45,   164,
     165,   169,   170,   166,     0,   157,   158,   162,   163,   167,
     168,     0,     0,     0,   151,   189,    54,   190,    74,   196,
       0,     0,   194,     0,   195,     0,    22,    18,     0,    91,
      54,     0,    76,    77,     0,    80,     0,    48,    47,    49,
       0,    40,    41,    26,     0,    46,    24,     0,     0,     0,
       0,    71,     0,    68,     0,    65,    66,    83,     0,   180,
     132,     0,   133,   130,   134,     0,   106,   107,   133,   121,
     122,    96,     0,     0,   127,   133,   111,     0,     0,   108,
       0,   104,   101,   124,   136,     0,   155,   182,   181,   183,
       0,   198,     0,     0,     0,     0,     0,    11,     0,    81,
      82,   178,     0,    79,   179,    50,     0,    29,    27,    28,
      25,    30,    32,    33,    35,     0,    63,     0,     0,     0,
      84,    85,     0,   133,     0,    54,   133,     0,   113,   115,
     128,   133,     0,    97,    97,   126,     0,   184,    94,   109,
      98,    60,   191,   199,   197,   202,     0,   201,    60,    78,
      91,     0,    67,     0,     0,    69,     0,     0,    38,     0,
       0,     0,     0,   112,     0,   129,    97,    97,   103,   100,
       0,     0,     0,     0,   200,     0,    72,    87,    87,    86,
      83,   116,   133,   117,   118,   114,   102,    99,   185,    62,
      14,    61,     0,     0,    90,    89,     0,     0,    88,    70,
     119
  };

  const short
  parser::yypgoto_[] =
  {
    -242,  -242,  -242,  -242,   352,  -242,  -242,  -242,  -242,  -242,
     -66,    -6,  -242,  -127,   188,    54,   -59,     4,     6,   350,
     329,  -242,  -242,  -242,   116,  -242,  -242,   388,   277,  -242,
     136,  -242,    61,  -242,    66,    91,   -83,  -242,  -242,   -79,
     182,  -242,  -241,  -242,   278,  -242,   103,  -238,  -133,   100,
    -164,   -63,   -74,     0,  -242,    73,  -242,    86,    89,  -242,
     187,    11,    95
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    33,    34,    35,    36,    65,    66,    67,   211,   212,
     141,    37,   214,    38,   216,    39,   238,    40,    49,   127,
      97,    81,   224,   225,   226,    83,    77,    58,   201,   202,
     203,   204,   289,   290,   364,   291,   156,   157,   242,   251,
     158,   159,   160,   245,   232,   297,   298,   161,   162,   163,
     164,   244,   165,    56,    43,    44,    45,    46,    47,   143,
      57,    51,   196
  };

  const short
  parser::yytable_[] =
  {
      42,   254,   303,     2,   215,   304,    41,   133,     2,   135,
      50,    48,   264,    59,    59,     2,    62,    63,   239,   240,
     145,    73,    74,    75,    71,   306,    98,   309,    76,    90,
      85,   167,    80,    82,   154,    42,   269,   122,   -44,   264,
     341,    41,   343,    69,   265,    95,    48,     2,    91,    11,
      93,   138,   121,   116,   222,   131,    11,   228,   266,   -95,
     146,   336,    70,    98,   337,    52,    55,   270,   106,    61,
     254,   265,    68,   234,    95,   107,   108,   109,   154,    94,
     234,   154,   252,   255,   233,   215,    94,   104,    86,   -44,
     323,   231,   264,    72,   241,    64,   -44,   155,   299,   168,
     169,   170,   171,   172,   173,   106,   174,   154,   177,   178,
     179,   180,   107,   108,   109,   324,   268,   185,   123,   187,
      78,   188,   131,   126,   265,   213,    91,    92,   192,   205,
     205,    42,   218,     2,   219,     2,   221,   217,   266,   118,
     154,   155,    48,    10,   155,   142,    55,   153,   146,   235,
     230,    98,   282,   283,   284,   155,   155,   146,   234,    79,
     119,   175,    84,   192,   234,     2,    55,    55,    55,   300,
     155,   234,   296,   186,    11,    10,    11,   243,   327,    55,
     176,   307,   305,   200,   200,   264,   329,   146,   231,   154,
     112,   153,   223,   106,   153,   192,   351,   193,   353,   354,
     107,   299,   192,   155,   273,   125,   279,   131,   113,   275,
     194,   146,    42,   195,   276,   114,   332,   265,   217,   234,
     153,    95,   234,    48,   338,   339,   328,   234,   257,   136,
     233,   266,   193,    93,   370,   155,   139,   345,   335,   117,
     367,   341,   342,   343,    55,   194,   363,   155,   195,   119,
     310,   264,   155,   153,   249,    91,   137,   356,   357,   264,
     312,   154,    91,   140,   193,   131,   131,   258,   192,   259,
     192,   193,   205,   124,   358,   296,   130,   194,   234,   144,
     195,   288,   313,   265,   194,   295,   263,   195,   119,   335,
     119,   265,   314,   267,   330,   333,   328,   266,   120,   181,
     182,   183,   153,   119,    91,   266,   155,   166,    91,   320,
     192,   131,   197,    91,   129,   121,   334,   198,   236,   237,
     199,   346,    91,   231,   155,   220,   200,   192,   192,   227,
     119,   229,   247,   155,   248,   155,   155,   193,   155,   193,
     250,   223,   360,   253,   146,   155,   260,   192,     2,   189,
     194,   271,   194,   195,   272,   195,   274,   277,    10,   315,
     281,   317,   285,   368,     2,   145,   286,   287,   292,   302,
     311,   155,   316,   318,   153,   146,   147,   262,   321,   193,
     288,   325,   326,   331,   344,   362,   350,    89,   295,    11,
     190,    96,   194,   148,   191,   195,   193,   193,   369,   128,
     280,   340,    60,   322,   288,    11,   149,   206,   319,   194,
     194,   366,   195,   195,   365,    87,   193,   349,   347,   348,
       1,     2,     3,     4,     5,     6,     7,     8,     9,   194,
     308,    10,   195,   246,    98,    26,    27,   355,   361,     2,
     145,   359,     0,     0,     0,     2,   145,     0,     0,     0,
     146,   147,     0,     0,   100,   101,   146,   147,   104,     0,
       0,     0,    11,    12,     0,     0,     0,    13,   148,    14,
       0,     0,     0,    88,    15,     0,   106,    16,     0,     0,
      11,   149,    17,   107,   108,   109,    11,   149,     0,    18,
       0,    19,    20,    21,    22,     0,    23,     0,     0,     0,
       0,   150,    24,     0,     0,     0,     0,    25,    26,    27,
      28,    29,     0,     0,    98,     0,    30,    31,    32,     1,
       2,     3,     4,     5,     6,     7,     8,     9,     0,     0,
      10,     0,   151,   152,   100,   101,   102,   103,   104,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     2,   145,
       0,     0,     0,     0,     0,     0,   106,     0,     0,   146,
     293,    11,    12,   107,   108,   109,    13,     0,    14,   110,
     111,     0,   278,    15,     0,     0,    16,   148,     0,     0,
       0,    17,     0,     0,   134,     0,     0,     0,    18,    11,
     294,    20,    21,    22,     0,    23,     0,   207,   208,   209,
       0,    24,     0,     0,     0,     0,     0,    26,    27,    28,
     210,     0,     0,     0,     0,    30,    31,    32,     1,     2,
       3,     4,     5,     6,     7,     8,     9,     0,    98,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   100,   101,
     102,   103,   104,   105,     0,     0,     0,     0,     0,     0,
      11,    12,     0,     0,     0,    13,     0,    14,   131,     0,
     106,     0,    15,     0,     0,    16,   132,   107,   108,   109,
      17,     0,     0,   110,   111,     0,     0,    18,     0,     0,
      20,    21,    22,     0,    23,     0,   207,   208,   209,     0,
      24,     0,     0,     0,     0,     0,    26,    27,    28,   210,
       0,     0,    98,     0,    30,    31,    32,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    98,     0,    10,     0,
       0,    99,   100,   101,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   100,   101,   102,   103,
     104,   105,     0,     0,   106,     0,     0,     0,     0,    11,
      12,   107,   108,   109,    13,     0,    14,   110,   106,     0,
       0,    15,     0,     0,    16,   107,   108,   109,     0,    17,
       0,   110,   111,     0,     0,     0,    18,     0,    19,    20,
      21,    22,     0,    23,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,    25,    26,    27,    28,    29,     0,
       0,     0,     0,    30,    31,    32,     1,     2,     3,     4,
       5,     6,     7,     8,     9,     0,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     2,     3,
       4,     5,     6,     7,     8,     9,     0,    98,    10,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    11,    12,
      54,     0,     0,    13,     0,    14,     0,   100,   101,   102,
      15,   104,     0,    16,     0,     0,     0,     0,    17,    11,
      12,   261,     0,     0,    13,     0,    14,     0,    20,   106,
       0,    15,     0,     0,    16,     0,   107,   108,   109,    17,
       0,     0,   110,   111,    26,    27,     0,     0,     0,    20,
       0,     0,    30,    31,    32,     0,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    26,    27,    10,     0,     0,
      98,     0,     0,    30,    31,    32,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     100,   101,   102,   103,   104,   105,     0,     0,    11,    12,
       0,     0,     0,    13,     0,    14,     0,     0,     0,    98,
      15,     0,   106,    16,     0,     0,     0,     0,    17,   107,
     108,   109,     0,     0,     0,   110,   111,     0,    20,   100,
     101,   102,   103,   104,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    27,    98,     0,     0,   131,
       0,   106,    30,    31,    32,     0,    98,     0,   107,   108,
     109,     0,     0,     0,   110,   111,   100,   101,   102,   103,
     104,   105,     0,     0,     0,     0,   100,   101,   102,   103,
     104,   105,     0,     0,     0,   184,     0,     0,   106,     0,
       0,     0,     0,    98,     0,   107,   108,   109,   106,   256,
       0,   110,   111,     0,     0,   107,   108,   109,     0,     0,
       0,   110,   111,   100,   101,   102,   103,   104,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,   301,     0,     0,   106,     0,     0,     0,     0,
      98,     0,   107,   108,   109,     0,     0,     0,   110,   111,
     100,   101,   102,   103,   104,   105,     0,     0,     0,     0,
     100,   101,   102,   103,   104,   105,     0,     0,     0,   352,
       0,     0,   106,    98,     0,     0,     0,     0,     0,   107,
     108,   109,   106,     0,     0,   110,   111,     0,     0,   107,
     108,   109,     0,   100,   101,   110,   111,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,   107,   108,   109,     0,     0,     0,   110,   111
  };

  const short
  parser::yycheck_[] =
  {
       0,   165,   243,     6,   131,   243,     0,    73,     6,    75,
      10,     0,    20,    13,    14,     6,    16,    17,   151,   152,
       7,    21,    22,    23,    20,    20,    10,    35,    24,    35,
       0,    97,    28,    29,    93,    35,    15,    55,     0,    20,
      20,    35,    22,    75,    52,    41,    35,     6,    47,    47,
      48,    50,    70,    53,   137,    50,    47,   140,    66,    46,
      17,   302,    94,    10,   302,    11,    12,    46,    52,    15,
     234,    52,    18,   147,    70,    59,    60,    61,   137,    77,
     154,   140,   161,   166,   147,   212,    77,    34,    58,    51,
      21,    48,    20,    20,   153,    98,    58,    93,   231,    99,
     100,   101,   102,   103,   104,    52,   106,   166,   108,   109,
     110,   111,    59,    60,    61,    46,   199,   117,    64,   119,
       5,   121,    50,    69,    52,   131,    47,    48,   124,   129,
     130,   131,   132,     6,   134,     6,   136,   131,    66,    49,
     199,   137,   131,    16,   140,    91,    92,    93,    17,   149,
     146,    10,   218,   219,   220,   151,   152,    17,   232,    94,
      70,   107,     0,   159,   238,     6,   112,   113,   114,   232,
     166,   245,   231,   119,    47,    16,    47,    46,    21,   125,
     107,   247,   245,   129,   130,    20,    46,    17,    48,   248,
      48,   137,   138,    52,   140,   191,   329,   124,   331,   332,
      59,   334,   198,   199,   204,    15,   212,    50,    48,   209,
     124,    17,   212,   124,   210,    48,    46,    52,   212,   293,
     166,   217,   296,   212,   303,   304,   292,   301,    49,    21,
     293,    66,   159,    48,   367,   231,    21,   320,   301,    46,
      46,    20,    21,    22,   190,   159,    15,   243,   159,    70,
     250,    20,   248,   199,   159,    47,    48,   336,   337,    20,
     260,   320,    47,    48,   191,    50,    50,    49,   264,    49,
     266,   198,   272,    46,   340,   334,    48,   191,   352,    92,
     191,   227,    49,    52,   198,   231,   191,   198,    70,   352,
      70,    52,    53,   198,   294,    49,   362,    66,    53,   112,
     113,   114,   248,    70,    47,    66,   302,    48,    47,    48,
     306,    50,   125,    47,    48,    70,    70,    46,    99,   100,
      48,   321,    47,    48,   320,    82,   272,   323,   324,    48,
      70,    49,    49,   329,    70,   331,   332,   264,   334,   266,
      15,   287,   342,    35,    17,   341,    46,   343,     6,     7,
     264,    49,   266,   264,    70,   266,    49,    58,    16,   264,
      51,   266,    49,   363,     6,     7,    51,    70,    49,    46,
      49,   367,    11,    49,   320,    17,    18,   190,    21,   306,
     326,    49,    70,    46,    53,    49,    48,    35,   334,    47,
      48,    41,   306,    35,    52,   306,   323,   324,    49,    70,
     212,   306,    14,   287,   350,    47,    48,   130,   272,   323,
     324,   350,   323,   324,   348,     0,   343,   326,   323,   324,
       5,     6,     7,     8,     9,    10,    11,    12,    13,   343,
     248,    16,   343,   155,    10,    93,    94,   334,   343,     6,
       7,   341,    -1,    -1,    -1,     6,     7,    -1,    -1,    -1,
      17,    18,    -1,    -1,    30,    31,    17,    18,    34,    -1,
      -1,    -1,    47,    48,    -1,    -1,    -1,    52,    35,    54,
      -1,    -1,    -1,    58,    59,    -1,    52,    62,    -1,    -1,
      47,    48,    67,    59,    60,    61,    47,    48,    -1,    74,
      -1,    76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    68,    87,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    -1,    -1,    10,    -1,   101,   102,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      16,    -1,    99,   100,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    17,
      18,    47,    48,    59,    60,    61,    52,    -1,    54,    65,
      66,    -1,    58,    59,    -1,    -1,    62,    35,    -1,    -1,
      -1,    67,    -1,    -1,    80,    -1,    -1,    -1,    74,    47,
      48,    77,    78,    79,    -1,    81,    -1,    83,    84,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,   102,   103,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    10,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    -1,    -1,    -1,    52,    -1,    54,    50,    -1,
      52,    -1,    59,    -1,    -1,    62,    58,    59,    60,    61,
      67,    -1,    -1,    65,    66,    -1,    -1,    74,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    83,    84,    85,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    10,    -1,   101,   102,   103,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    10,    -1,    16,    -1,
      -1,    15,    30,    31,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    52,    -1,    -1,    -1,    -1,    47,
      48,    59,    60,    61,    52,    -1,    54,    65,    52,    -1,
      -1,    59,    -1,    -1,    62,    59,    60,    61,    -1,    67,
      -1,    65,    66,    -1,    -1,    -1,    74,    -1,    76,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,   102,   103,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    -1,    10,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    -1,    -1,    52,    -1,    54,    -1,    30,    31,    32,
      59,    34,    -1,    62,    -1,    -1,    -1,    -1,    67,    47,
      48,    49,    -1,    -1,    52,    -1,    54,    -1,    77,    52,
      -1,    59,    -1,    -1,    62,    -1,    59,    60,    61,    67,
      -1,    -1,    65,    66,    93,    94,    -1,    -1,    -1,    77,
      -1,    -1,   101,   102,   103,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    93,    94,    16,    -1,    -1,
      10,    -1,    -1,   101,   102,   103,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    47,    48,
      -1,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    10,
      59,    -1,    52,    62,    -1,    -1,    -1,    -1,    67,    59,
      60,    61,    -1,    -1,    -1,    65,    66,    -1,    77,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    10,    -1,    -1,    50,
      -1,    52,   101,   102,   103,    -1,    10,    -1,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    10,    -1,    59,    60,    61,    52,    53,
      -1,    65,    66,    -1,    -1,    59,    60,    61,    -1,    -1,
      -1,    65,    66,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      10,    -1,    59,    60,    61,    -1,    -1,    -1,    65,    66,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    49,
      -1,    -1,    52,    10,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,    52,    -1,    -1,    65,    66,    -1,    -1,    59,
      60,    61,    -1,    30,    31,    65,    66,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,    -1,    -1,    -1,    65,    66
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      16,    47,    48,    52,    54,    59,    62,    67,    74,    76,
      77,    78,    79,    81,    87,    92,    93,    94,    95,    96,
     101,   102,   103,   105,   106,   107,   108,   115,   117,   119,
     121,   122,   157,   158,   159,   160,   161,   162,   165,   122,
     157,   165,   119,    46,    49,   119,   157,   164,   131,   157,
     131,   119,   157,   157,    98,   109,   110,   111,   119,    75,
      94,   121,   159,   157,   157,   157,   121,   130,     5,    94,
     121,   125,   121,   129,     0,     0,    58,     0,    58,   108,
     115,    47,    48,    48,    77,   121,   123,   124,    10,    15,
      30,    31,    32,    33,    34,    35,    52,    59,    60,    61,
      65,    66,    48,    48,    48,    16,   157,    46,    49,    70,
      53,    70,    55,   119,    46,    15,   119,   123,   124,    48,
      48,    50,    58,   114,    80,   114,    21,    48,    50,    21,
      48,   114,   119,   163,   164,     7,    17,    18,    35,    48,
      68,    99,   100,   119,   120,   121,   140,   141,   144,   145,
     146,   151,   152,   153,   154,   156,    48,   114,   157,   157,
     157,   157,   157,   157,   157,   119,   159,   157,   157,   157,
     157,   164,   164,   164,    49,   157,   119,   157,   157,     7,
      48,    52,   121,   159,   161,   162,   166,   164,    46,    48,
     119,   132,   133,   134,   135,   157,   132,    83,    84,    85,
      96,   112,   113,   115,   116,   117,   118,   122,   157,   157,
      82,   157,   140,   119,   126,   127,   128,    48,   140,    49,
     121,    48,   148,   155,   156,   157,    99,   100,   120,   152,
     152,   120,   142,    46,   155,   147,   148,    49,    70,   166,
      15,   143,   143,    35,   154,   140,    53,    49,    49,    49,
      46,    49,   164,   166,    20,    52,    66,   166,   140,    15,
      46,    49,    70,   157,    49,   157,   121,    58,    58,   115,
     118,    51,   114,   114,   114,    49,    51,    70,   119,   136,
     137,   139,    49,    18,    48,   119,   120,   149,   150,   152,
     155,    49,    46,   146,   151,   155,    20,   114,   144,    35,
     157,    49,   157,    49,    53,   166,    11,   166,    49,   134,
      48,    21,   128,    21,    46,    49,    70,    21,   114,    46,
     157,    46,    46,    49,    70,   155,   146,   151,   143,   143,
     166,    20,    21,    22,    53,   140,   157,   166,   166,   139,
      48,   152,    49,   152,   152,   150,   143,   143,   114,   153,
     157,   166,    49,    15,   138,   138,   136,    46,   157,    49,
     152
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   104,   105,   105,   106,   106,   106,   107,   107,   107,
     107,   108,   108,   108,   108,   108,   108,   109,   109,   110,
     110,   111,   111,   112,   112,   112,   113,   113,   113,   113,
     114,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     116,   116,   117,   117,   117,   117,   118,   118,   118,   118,
     118,   119,   120,   121,   121,   121,   122,   122,   123,   123,
     124,   124,   124,   125,   126,   126,   127,   127,   128,   129,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   134,
     134,   135,   135,   136,   136,   137,   137,   138,   138,   139,
     139,   140,   140,   141,   141,   142,   142,   143,   143,   144,
     144,   144,   144,   144,   144,   145,   145,   145,   146,   146,
     147,   147,   148,   149,   149,   150,   150,   150,   150,   150,
     151,   151,   151,   152,   152,   152,   153,   153,   153,   153,
     153,   153,   154,   155,   155,   156,   156,   157,   157,   157,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   159,   160,   160,   160,   160,
     161,   161,   161,   161,   162,   162,   163,   163,   164,   164,
     164,   164,   165,   165,   166,   166,   166,   166,   166,   166,
     166,   166,   166
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     3,     2,     7,     1,     2,     1,     3,     1,
       2,     1,     3,     0,     1,     2,     1,     2,     2,     2,
       3,     3,     5,     5,     3,     5,     3,     3,     6,     2,
       1,     1,     2,     2,     1,     3,     1,     1,     1,     1,
       2,     1,     1,     2,     1,     3,     1,     2,     1,     1,
       4,     6,     6,     4,     0,     1,     1,     3,     1,     5,
       8,     3,     6,     1,     3,     0,     1,     1,     3,     2,
       1,     2,     2,     0,     1,     1,     3,     0,     2,     4,
       4,     0,     1,     1,     3,     0,     1,     0,     2,     5,
       4,     2,     5,     4,     2,     1,     2,     2,     2,     3,
       0,     1,     3,     1,     3,     1,     3,     3,     3,     5,
       1,     2,     2,     1,     2,     1,     3,     2,     3,     4,
       2,     1,     2,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     3,     3,     3,     4,     2,     3,     3,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     2,     2,     5,     5,
       4,     4,     4,     4,     5,     7,     0,     1,     1,     3,
       3,     5,     2,     1,     1,     1,     1,     3,     2,     3,
       4,     3,     3
  };


#if BANG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "CONTEXT_IDENTIFIER",
  "RESERVED_IDENTIFIER", "CT_IDENTIFIER", "INTEGER_INDEX", "INTEGER",
  "DECIMAL", "DECIMAL_S", "OPERATOR_TERM", "\"`=`\"", "\"APOSTROPHE\"",
  "\"`@`\"", "\"`_`\"", "\"`->*`\"", "\"`->`\"", "\"`=>`\"", "\"`~>`\"",
  "\"`.*`\"", "\"`++`\"", "\"`--`\"", "\"`<<`\"", "\"`>>`\"", "\"`<=`\"",
  "\"`>=`\"", "\"`==`\"", "\"`!=`\"", "\"`&&`\"", "\"`||`\"", "\"`..`\"",
  "\"`...`\"", "\"`+=`\"", "\"`-=`\"", "\"`*=`\"", "\"`/=`\"", "\"`%=`\"",
  "\"`<<=`\"", "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"", "\"`^=`\"", "\"`:`\"",
  "\"`::`\"", "\"`(`\"", "\"`)`\"", "\"`{`\"", "\"`}`\"", "\"`[`\"",
  "\"`]`\"", "\"`[[`\"", "\"`]]`\"", "\"`<`\"", "\"`>`\"", "\"`;`\"",
  "\"`.`\"", "\"`+`\"", "\"`-`\"", "\"`*`\"", "\"`/`\"", "\"`%`\"",
  "\"`&`\"", "\"`|`\"", "\"`!`\"", "\"`~`\"", "\"`^`\"", "\",\"",
  "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET", "VAR", "EXTERN", "\"`new`\"",
  "\"`while`\"", "\"`for`\"", "\"`in`\"", "\"`if`\"", "\"`else`\"",
  "\"`continue`\"", "\"`break`\"", "\"`return`\"", "AUTO", "USING",
  "THROW", "SIZEOF", "LOWEST", "DEREF", "INCLUDE", "INLINE", "\"`fn`\"",
  "ENUM", "STRUCT", "EXTENDS", "\"weak modifier\"",
  "\"constexpr modifier\"", "\"runctime modifier\"", "\"nil\"", "\"true\"",
  "\"false\"", "$accept", "begin", "statement_any",
  "finished-statement-any", "statement", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction-statement-any",
  "finished-infunction-statement-any", "braced-statements",
  "finished-statement", "infunction-statement-set", "generic-statement",
  "infunction-statement", "identifier", "internal-identifier", "qname",
  "fn-start-decl", "fn-name", "fn-decl", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "struct-decl", "using-decl", "expression-list",
  "argument-list-opt", "argument-list", "argument", "argument-name",
  "field-list-opt", "field-list", "field-default-value-opt", "field",
  "parameter-list-opt", "parameter-list", "internal-identifier-opt",
  "parameter-default-value-opt", "parameter-decl",
  "constraint-expression-mod", "constraint-expression", "subpatterns-opt",
  "subpatterns", "pattern-list", "pattern-field", "pattern-mod",
  "pattern-sfx", "pattern", "concept-expression",
  "concept-expression-list-opt", "concept-expression-list",
  "syntax-expression", "syntax-expression-wo-ii", "apostrophe-expression",
  "new-expression", "call-expression", "lambda-expression",
  "pack-expression-opt", "pack-expression", "compound-expression",
  "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   332,   332,   333,   337,   339,   341,   358,   360,   361,
     363,   368,   370,   372,   374,   383,   385,   390,   392,   397,
     399,   404,   405,   410,   412,   414,   419,   421,   422,   424,
     429,   434,   436,   438,   440,   442,   444,   446,   448,   450,
     455,   457,   462,   464,   466,   468,   473,   475,   477,   479,
     481,   486,   494,   509,   511,   513,   519,   521,   526,   527,
     531,   533,   535,   541,   546,   547,   551,   553,   558,   564,
     566,   589,   593,   607,   609,   615,   616,   620,   622,   627,
     629,   634,   636,   642,   643,   647,   649,   654,   655,   659,
     661,   672,   673,   677,   679,   684,   685,   689,   690,   694,
     696,   698,   700,   702,   704,   709,   710,   711,   715,   717,
     723,   725,   729,   734,   736,   741,   743,   745,   747,   749,
     754,   755,   756,   760,   761,   762,   766,   768,   770,   772,
     774,   776,   781,   786,   788,   792,   794,   802,   804,   806,
     811,   813,   815,   817,   819,   821,   823,   825,   827,   829,
     831,   833,   835,   844,   852,   854,   856,   860,   862,   864,
     867,   869,   874,   876,   880,   882,   884,   887,   891,   894,
     897,   900,   901,   902,   903,   907,   912,   914,   916,   918,
     923,   931,   933,   935,   940,   942,   947,   949,   954,   963,
     969,   974,   982,   984,  1025,  1027,  1028,  1032,  1034,  1037,
    1039,  1041,  1049
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // BANG_LANGDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103
    };
    // Last valid token kind.
    const int code_max = 358;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4935 "bang.tab.cpp"

#line 1376 "bang.y"

