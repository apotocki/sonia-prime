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
    ctx.append_error(
        lang::lex::resource_location{ loc.begin.line, loc.begin.column, ctx.get_resource() },
        lang::lex::resource_location{ loc.end.line, loc.end.column, ctx.get_resource() },
        msg);
}

#define IGNORE_TERM(...)


#line 74 "bang.tab.cpp"


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
#line 171 "bang.tab.cpp"

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

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_expression_list: // expression-list
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
      case symbol_kind::S_138_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_let_decl: // let-decl
      case symbol_kind::S_111_let_decl_start: // let-decl-start
      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_115_braced_statements: // braced-statements
      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
      case symbol_kind::S_134_argument_list: // argument-list
      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_165_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_142_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_150_pattern_list: // pattern-list
        value.copy< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
        value.copy< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_136_argument_name: // argument-name
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_124_fn_name: // fn-name
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

      case symbol_kind::S_129_case_decl: // case-decl
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

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_119_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
        value.copy< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
        value.copy< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
      case symbol_kind::S_128_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
      case symbol_kind::S_158_syntax_expression: // syntax-expression
      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_161_new_expression: // new-expression
      case symbol_kind::S_162_call_expression: // call-expression
      case symbol_kind::S_163_lambda_expression: // lambda-expression
      case symbol_kind::S_166_compound_expression: // compound-expression
      case symbol_kind::S_167_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_using_decl: // using-decl
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

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_132_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
      case symbol_kind::S_138_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_let_decl: // let-decl
      case symbol_kind::S_111_let_decl_start: // let-decl-start
      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_115_braced_statements: // braced-statements
      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
      case symbol_kind::S_134_argument_list: // argument-list
      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_165_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_142_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_150_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_136_argument_name: // argument-name
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_124_fn_name: // fn-name
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

      case symbol_kind::S_129_case_decl: // case-decl
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

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_119_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
        value.move< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
        value.move< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
      case symbol_kind::S_128_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
      case symbol_kind::S_158_syntax_expression: // syntax-expression
      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_161_new_expression: // new-expression
      case symbol_kind::S_162_call_expression: // call-expression
      case symbol_kind::S_163_lambda_expression: // lambda-expression
      case symbol_kind::S_166_compound_expression: // compound-expression
      case symbol_kind::S_167_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_131_using_decl: // using-decl
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

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
        value.YY_MOVE_OR_COPY< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_expression_list: // expression-list
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
      case symbol_kind::S_138_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_let_decl: // let-decl
      case symbol_kind::S_111_let_decl_start: // let-decl-start
      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_115_braced_statements: // braced-statements
      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
      case symbol_kind::S_134_argument_list: // argument-list
      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_165_pack_expression: // pack-expression
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_142_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_150_pattern_list: // pattern-list
        value.YY_MOVE_OR_COPY< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.YY_MOVE_OR_COPY< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
        value.YY_MOVE_OR_COPY< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_136_argument_name: // argument-name
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_124_fn_name: // fn-name
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

      case symbol_kind::S_129_case_decl: // case-decl
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

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
        value.YY_MOVE_OR_COPY< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_119_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
        value.YY_MOVE_OR_COPY< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
        value.YY_MOVE_OR_COPY< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
      case symbol_kind::S_128_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
      case symbol_kind::S_158_syntax_expression: // syntax-expression
      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_161_new_expression: // new-expression
      case symbol_kind::S_162_call_expression: // call-expression
      case symbol_kind::S_163_lambda_expression: // lambda-expression
      case symbol_kind::S_166_compound_expression: // compound-expression
      case symbol_kind::S_167_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_using_decl: // using-decl
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

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
      case symbol_kind::S_138_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_let_decl: // let-decl
      case symbol_kind::S_111_let_decl_start: // let-decl-start
      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_115_braced_statements: // braced-statements
      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
      case symbol_kind::S_134_argument_list: // argument-list
      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_165_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_142_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_150_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_136_argument_name: // argument-name
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_124_fn_name: // fn-name
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

      case symbol_kind::S_129_case_decl: // case-decl
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

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_119_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
        value.move< std::pair<pattern_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
        value.move< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
      case symbol_kind::S_128_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
      case symbol_kind::S_158_syntax_expression: // syntax-expression
      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_161_new_expression: // new-expression
      case symbol_kind::S_162_call_expression: // call-expression
      case symbol_kind::S_163_lambda_expression: // lambda-expression
      case symbol_kind::S_166_compound_expression: // compound-expression
      case symbol_kind::S_167_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_using_decl: // using-decl
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

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_132_expression_list: // expression-list
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
      case symbol_kind::S_138_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_110_let_decl: // let-decl
      case symbol_kind::S_111_let_decl_start: // let-decl-start
      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_115_braced_statements: // braced-statements
      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
      case symbol_kind::S_134_argument_list: // argument-list
      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_165_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_142_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_150_pattern_list: // pattern-list
        value.copy< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (that.value);
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
        value.copy< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_136_argument_name: // argument-name
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_124_fn_name: // fn-name
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

      case symbol_kind::S_129_case_decl: // case-decl
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

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_119_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
        value.copy< std::pair<pattern_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
        value.copy< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
      case symbol_kind::S_128_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
      case symbol_kind::S_158_syntax_expression: // syntax-expression
      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_161_new_expression: // new-expression
      case symbol_kind::S_162_call_expression: // call-expression
      case symbol_kind::S_163_lambda_expression: // lambda-expression
      case symbol_kind::S_166_compound_expression: // compound-expression
      case symbol_kind::S_167_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_131_using_decl: // using-decl
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

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_132_expression_list: // expression-list
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
      case symbol_kind::S_138_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_110_let_decl: // let-decl
      case symbol_kind::S_111_let_decl_start: // let-decl-start
      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_115_braced_statements: // braced-statements
      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
      case symbol_kind::S_134_argument_list: // argument-list
      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_165_pack_expression: // pack-expression
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_142_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_150_pattern_list: // pattern-list
        value.move< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (that.value);
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
        value.move< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_136_argument_name: // argument-name
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_124_fn_name: // fn-name
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

      case symbol_kind::S_129_case_decl: // case-decl
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

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_119_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
        value.move< std::pair<pattern_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
        value.move< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > (that.value);
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
      case symbol_kind::S_128_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
      case symbol_kind::S_158_syntax_expression: // syntax-expression
      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_161_new_expression: // new-expression
      case symbol_kind::S_162_call_expression: // call-expression
      case symbol_kind::S_163_lambda_expression: // lambda-expression
      case symbol_kind::S_166_compound_expression: // compound-expression
      case symbol_kind::S_167_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_131_using_decl: // using-decl
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
#line 1673 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1679 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1685 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1691 "bang.tab.cpp"
        break;

      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
#line 327 "bang.y"
                 { }
#line 1697 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 327 "bang.y"
                 { }
#line 1703 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 327 "bang.y"
                 { }
#line 1709 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 327 "bang.y"
                 { }
#line 1715 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 327 "bang.y"
                 { }
#line 1721 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 327 "bang.y"
                 { }
#line 1727 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 327 "bang.y"
                 { }
#line 1733 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 327 "bang.y"
                 { }
#line 1739 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 327 "bang.y"
                 { }
#line 1745 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 327 "bang.y"
                 { }
#line 1751 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 327 "bang.y"
                 { }
#line 1757 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 327 "bang.y"
                 { }
#line 1763 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 327 "bang.y"
                 { }
#line 1769 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 327 "bang.y"
                 { }
#line 1775 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 327 "bang.y"
                 { }
#line 1781 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 327 "bang.y"
                 { }
#line 1787 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 327 "bang.y"
                 { }
#line 1793 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 327 "bang.y"
                 { }
#line 1799 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 327 "bang.y"
                 { }
#line 1805 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 327 "bang.y"
                 { }
#line 1811 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 327 "bang.y"
                 { }
#line 1817 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 327 "bang.y"
                 { }
#line 1823 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 327 "bang.y"
                 { }
#line 1829 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 327 "bang.y"
                 { }
#line 1835 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 327 "bang.y"
                 { }
#line 1841 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 327 "bang.y"
                 { }
#line 1847 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 327 "bang.y"
                 { }
#line 1853 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 327 "bang.y"
                 { }
#line 1859 "bang.tab.cpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 327 "bang.y"
                 { }
#line 1865 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 327 "bang.y"
                 { }
#line 1871 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 327 "bang.y"
                 { }
#line 1877 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 327 "bang.y"
                 { }
#line 1883 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 327 "bang.y"
                 { }
#line 1889 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 327 "bang.y"
                 { }
#line 1895 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
#line 327 "bang.y"
                 { }
#line 1901 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 327 "bang.y"
                 { }
#line 1907 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl: // let-decl
#line 327 "bang.y"
                 { }
#line 1913 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_let_decl_start: // let-decl-start
#line 327 "bang.y"
                 { }
#line 1919 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 327 "bang.y"
                 { }
#line 1925 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
#line 327 "bang.y"
                 { }
#line 1931 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
#line 327 "bang.y"
                 { }
#line 1937 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_braced_statements: // braced-statements
#line 327 "bang.y"
                 { }
#line 1943 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
#line 327 "bang.y"
                 { }
#line 1949 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
#line 327 "bang.y"
                 { }
#line 1955 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
#line 327 "bang.y"
                 { }
#line 1961 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_infunction_statement: // infunction-statement
#line 327 "bang.y"
                 { }
#line 1967 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 327 "bang.y"
                 { }
#line 1973 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_internal_identifier: // internal-identifier
#line 327 "bang.y"
                 { }
#line 1979 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 327 "bang.y"
                 { }
#line 1985 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
#line 327 "bang.y"
                 { }
#line 1991 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_fn_name: // fn-name
#line 327 "bang.y"
                 { }
#line 1997 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
#line 327 "bang.y"
                 { }
#line 2003 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
#line 327 "bang.y"
                 { }
#line 2009 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
#line 327 "bang.y"
                 { }
#line 2015 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_case_list: // case-list
#line 327 "bang.y"
                 { }
#line 2021 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_case_decl: // case-decl
#line 327 "bang.y"
                 { }
#line 2027 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
#line 327 "bang.y"
                 { }
#line 2033 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_using_decl: // using-decl
#line 327 "bang.y"
                 { }
#line 2039 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_expression_list: // expression-list
#line 327 "bang.y"
                 { }
#line 2045 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
#line 327 "bang.y"
                 { }
#line 2051 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_argument_list: // argument-list
#line 327 "bang.y"
                 { }
#line 2057 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 327 "bang.y"
                 { }
#line 2063 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_argument_name: // argument-name
#line 327 "bang.y"
                 { }
#line 2069 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
#line 327 "bang.y"
                 { }
#line 2075 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_field_list: // field-list
#line 327 "bang.y"
                 { }
#line 2081 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
#line 327 "bang.y"
                 { }
#line 2087 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 327 "bang.y"
                 { }
#line 2093 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
#line 327 "bang.y"
                 { }
#line 2099 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_parameter_list: // parameter-list
#line 327 "bang.y"
                 { }
#line 2105 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
#line 327 "bang.y"
                 { }
#line 2111 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
#line 327 "bang.y"
                 { }
#line 2117 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
#line 327 "bang.y"
                 { }
#line 2123 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
#line 327 "bang.y"
                 { }
#line 2129 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
#line 327 "bang.y"
                 { }
#line 2135 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
#line 327 "bang.y"
                 { }
#line 2141 "bang.tab.cpp"
        break;

      case symbol_kind::S_subpatterns: // subpatterns
#line 327 "bang.y"
                 { }
#line 2147 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_pattern_list: // pattern-list
#line 327 "bang.y"
                 { }
#line 2153 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
#line 327 "bang.y"
                 { }
#line 2159 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
#line 327 "bang.y"
                 { }
#line 2165 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
#line 327 "bang.y"
                 { }
#line 2171 "bang.tab.cpp"
        break;

      case symbol_kind::S_pattern: // pattern
#line 327 "bang.y"
                 { }
#line 2177 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
#line 327 "bang.y"
                 { }
#line 2183 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
#line 327 "bang.y"
                 { }
#line 2189 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
#line 327 "bang.y"
                 { }
#line 2195 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_syntax_expression: // syntax-expression
#line 327 "bang.y"
                 { }
#line 2201 "bang.tab.cpp"
        break;

      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 327 "bang.y"
                 { }
#line 2207 "bang.tab.cpp"
        break;

      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
#line 327 "bang.y"
                 { }
#line 2213 "bang.tab.cpp"
        break;

      case symbol_kind::S_161_new_expression: // new-expression
#line 327 "bang.y"
                 { }
#line 2219 "bang.tab.cpp"
        break;

      case symbol_kind::S_162_call_expression: // call-expression
#line 327 "bang.y"
                 { }
#line 2225 "bang.tab.cpp"
        break;

      case symbol_kind::S_163_lambda_expression: // lambda-expression
#line 327 "bang.y"
                 { }
#line 2231 "bang.tab.cpp"
        break;

      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
#line 327 "bang.y"
                 { }
#line 2237 "bang.tab.cpp"
        break;

      case symbol_kind::S_165_pack_expression: // pack-expression
#line 327 "bang.y"
                 { }
#line 2243 "bang.tab.cpp"
        break;

      case symbol_kind::S_166_compound_expression: // compound-expression
#line 327 "bang.y"
                 { }
#line 2249 "bang.tab.cpp"
        break;

      case symbol_kind::S_167_type_expr: // type-expr
#line 327 "bang.y"
                 { }
#line 2255 "bang.tab.cpp"
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

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
        yylhs.value.emplace< concept_expression_list_t > ();
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_132_expression_list: // expression-list
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
      case symbol_kind::S_138_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_110_let_decl: // let-decl
      case symbol_kind::S_111_let_decl_start: // let-decl-start
      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_115_braced_statements: // braced-statements
      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< managed_statement_list > ();
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
      case symbol_kind::S_134_argument_list: // argument-list
      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
      case symbol_kind::S_165_pack_expression: // pack-expression
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_142_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_148_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_150_pattern_list: // pattern-list
        yylhs.value.emplace< pattern_list_t > ();
        break;

      case symbol_kind::S_pattern: // pattern
        yylhs.value.emplace< pattern_t > ();
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
        yylhs.value.emplace< pattern_t::field > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_136_argument_name: // argument-name
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_124_fn_name: // fn-name
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

      case symbol_kind::S_129_case_decl: // case-decl
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

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
        yylhs.value.emplace< sonia::optional<syntax_expression_t> > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_119_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
        yylhs.value.emplace< std::pair<pattern_t, parameter_constraint_modifier_t> > ();
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
        yylhs.value.emplace< std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ();
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
      case symbol_kind::S_128_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
      case symbol_kind::S_158_syntax_expression: // syntax-expression
      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_161_new_expression: // new-expression
      case symbol_kind::S_162_call_expression: // call-expression
      case symbol_kind::S_163_lambda_expression: // lambda-expression
      case symbol_kind::S_166_compound_expression: // compound-expression
      case symbol_kind::S_167_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_131_using_decl: // using-decl
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
#line 2716 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 333 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2722 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 338 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2728 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 340 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2734 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 342 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2740 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 359 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2746 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 360 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2752 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 362 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2758 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 364 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2764 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 369 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2770 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 371 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2776 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 373 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2782 "bang.tab.cpp"
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
#line 2795 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 384 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2801 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 386 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2807 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 391 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2813 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 393 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2819 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: identifier
#line 398 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2825 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: "weak modifier" identifier
#line 400 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2831 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 404 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2837 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 406 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2843 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 411 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2849 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 413 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2855 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 415 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2861 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 420 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2867 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 421 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2873 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 423 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2879 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 425 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2885 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 430 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2891 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 435 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2897 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 437 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2903 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 439 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2909 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 441 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2915 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 443 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2921 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 445 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2927 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 447 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2933 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 449 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2939 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 451 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2945 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 456 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2951 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 458 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2957 "bang.tab.cpp"
    break;

  case 42: // generic-statement: LET let-decl
#line 463 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2963 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 465 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2969 "bang.tab.cpp"
    break;

  case 44: // generic-statement: compound-expression
#line 467 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2975 "bang.tab.cpp"
    break;

  case 45: // generic-statement: syntax-expression "`=`" syntax-expression
#line 469 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2981 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: generic-statement
#line 474 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2987 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: "`break`"
#line 476 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2993 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`continue`"
#line 478 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2999 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`return`"
#line 480 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .location = std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3005 "bang.tab.cpp"
    break;

  case 50: // infunction-statement: "`return`" syntax-expression
#line 482 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3011 "bang.tab.cpp"
    break;

  case 51: // identifier: IDENTIFIER
#line 487 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3017 "bang.tab.cpp"
    break;

  case 52: // internal-identifier: CONTEXT_IDENTIFIER
#line 495 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3023 "bang.tab.cpp"
    break;

  case 53: // qname: "`::`" identifier
#line 510 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 3029 "bang.tab.cpp"
    break;

  case 54: // qname: identifier
#line 512 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 3035 "bang.tab.cpp"
    break;

  case 55: // qname: qname "`::`" identifier
#line 514 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 3041 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: "`fn`"
#line 520 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3047 "bang.tab.cpp"
    break;

  case 57: // fn-start-decl: INLINE "`fn`"
#line 522 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3053 "bang.tab.cpp"
    break;

  case 58: // fn-name: qname
#line 526 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 3059 "bang.tab.cpp"
    break;

  case 59: // fn-name: "`new`"
#line 527 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 3065 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 532 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3071 "bang.tab.cpp"
    break;

  case 61: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`~>`" type-expr
#line 534 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3077 "bang.tab.cpp"
    break;

  case 62: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" pattern
#line 536 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3083 "bang.tab.cpp"
    break;

  case 63: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 542 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3089 "bang.tab.cpp"
    break;

  case 64: // case-list-opt: %empty
#line 546 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 3095 "bang.tab.cpp"
    break;

  case 65: // case-list-opt: case-list
#line 547 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 3101 "bang.tab.cpp"
    break;

  case 66: // case-list: case-decl
#line 552 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 3107 "bang.tab.cpp"
    break;

  case 67: // case-list: case-list "," case-decl
#line 554 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 3113 "bang.tab.cpp"
    break;

  case 68: // case-decl: identifier
#line 559 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 3119 "bang.tab.cpp"
    break;

  case 69: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 565 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3125 "bang.tab.cpp"
    break;

  case 70: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 567 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3131 "bang.tab.cpp"
    break;

  case 71: // using-decl: qname "`=>`" syntax-expression
#line 590 "bang.y"
        { 
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(loc) });
            yylhs.value.as < using_decl > () = using_decl{ fn_decl_t{ fn_pure_t{ .aname = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), .result = nullptr }, ctx.push(std::move(sts)) } };
        }
#line 3142 "bang.tab.cpp"
    break;

  case 72: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 597 "bang.y"
        {
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) });
            yylhs.value.as < using_decl > () = using_decl{ fn_decl_t{ fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = nullptr }, ctx.push(std::move(sts)) } };
            IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3154 "bang.tab.cpp"
    break;

  case 73: // expression-list: syntax-expression
#line 615 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3160 "bang.tab.cpp"
    break;

  case 74: // expression-list: expression-list "," syntax-expression
#line 617 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3166 "bang.tab.cpp"
    break;

  case 75: // argument-list-opt: %empty
#line 622 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 3172 "bang.tab.cpp"
    break;

  case 76: // argument-list-opt: argument-list
#line 623 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 3178 "bang.tab.cpp"
    break;

  case 77: // argument-list: argument
#line 628 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 3184 "bang.tab.cpp"
    break;

  case 78: // argument-list: argument-list "," argument
#line 630 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 3190 "bang.tab.cpp"
    break;

  case 79: // argument: argument-name syntax-expression
#line 635 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3196 "bang.tab.cpp"
    break;

  case 80: // argument: syntax-expression
#line 637 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3202 "bang.tab.cpp"
    break;

  case 81: // argument-name: identifier "`=`"
#line 642 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3208 "bang.tab.cpp"
    break;

  case 82: // argument-name: identifier "`:`"
#line 644 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 3214 "bang.tab.cpp"
    break;

  case 83: // field-list-opt: %empty
#line 649 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 3220 "bang.tab.cpp"
    break;

  case 84: // field-list-opt: field-list
#line 650 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 3226 "bang.tab.cpp"
    break;

  case 85: // field-list: field
#line 655 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 3232 "bang.tab.cpp"
    break;

  case 86: // field-list: field-list "," field
#line 657 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 3238 "bang.tab.cpp"
    break;

  case 87: // field-default-value-opt: %empty
#line 661 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3244 "bang.tab.cpp"
    break;

  case 88: // field-default-value-opt: "`=`" syntax-expression
#line 662 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3250 "bang.tab.cpp"
    break;

  case 89: // field: identifier "`:`" type-expr field-default-value-opt
#line 667 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), parameter_constraint_modifier_t::runtime_type, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3256 "bang.tab.cpp"
    break;

  case 90: // field: identifier "`=>`" syntax-expression
#line 669 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), parameter_constraint_modifier_t::const_type, std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3262 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 675 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3268 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 676 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3274 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 681 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3280 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 683 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3286 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 687 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3292 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 688 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3298 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 692 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3304 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 693 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3310 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" pattern-mod parameter-default-value-opt
#line 698 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3316 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: internal-identifier "`:`" pattern-mod parameter-default-value-opt
#line 700 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3322 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: pattern-mod parameter-default-value-opt
#line 702 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{}, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3328 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`:`" constraint-expression parameter-default-value-opt
#line 704 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3334 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: internal-identifier "`:`" constraint-expression parameter-default-value-opt
#line 706 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3340 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: constraint-expression parameter-default-value-opt
#line 708 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3346 "bang.tab.cpp"
    break;

  case 105: // constraint-expression-mod: "`~`"
#line 712 "bang.y"
            { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::const_or_runtime_type; }
#line 3352 "bang.tab.cpp"
    break;

  case 106: // constraint-expression-mod: "`~`" "constexpr modifier"
#line 713 "bang.y"
                      { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::const_type; }
#line 3358 "bang.tab.cpp"
    break;

  case 107: // constraint-expression-mod: "`~`" "runctime modifier"
#line 714 "bang.y"
                    { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::runtime_type; }
#line 3364 "bang.tab.cpp"
    break;

  case 108: // constraint-expression: constraint-expression-mod type-expr
#line 719 "bang.y"
        { yylhs.value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[0].value.as < syntax_expression_t > ()), yystack_[1].value.as < parameter_constraint_modifier_t > () }; }
#line 3370 "bang.tab.cpp"
    break;

  case 109: // constraint-expression: constraint-expression-mod type-expr "`...`"
#line 721 "bang.y"
        { yylhs.value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[1].value.as < syntax_expression_t > ()), yystack_[2].value.as < parameter_constraint_modifier_t > () | parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3376 "bang.tab.cpp"
    break;

  case 110: // subpatterns-opt: %empty
#line 727 "bang.y"
        { yylhs.value.as < pattern_list_t > () = pattern_list_t{}; }
#line 3382 "bang.tab.cpp"
    break;

  case 111: // subpatterns-opt: subpatterns
#line 728 "bang.y"
      { yylhs.value.as < pattern_list_t > () = yystack_[0].value.as < pattern_list_t > (); }
#line 3388 "bang.tab.cpp"
    break;

  case 112: // subpatterns: "`(`" pattern-list "`)`"
#line 733 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[1].value.as < pattern_list_t > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3394 "bang.tab.cpp"
    break;

  case 113: // pattern-list: pattern-field
#line 738 "bang.y"
        { yylhs.value.as < pattern_list_t > () = pattern_list_t{ std::move(yystack_[0].value.as < pattern_t::field > ()) }; }
#line 3400 "bang.tab.cpp"
    break;

  case 114: // pattern-list: pattern-list "," pattern-field
#line 740 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[2].value.as < pattern_list_t > ()); yylhs.value.as < pattern_list_t > ().emplace_back(std::move(yystack_[0].value.as < pattern_t::field > ())); }
#line 3406 "bang.tab.cpp"
    break;

  case 115: // pattern-field: pattern-sfx
#line 745 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3412 "bang.tab.cpp"
    break;

  case 116: // pattern-field: "`_`" "`:`" pattern-sfx
#line 747 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3418 "bang.tab.cpp"
    break;

  case 117: // pattern-field: identifier "`:`" pattern-sfx
#line 749 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3424 "bang.tab.cpp"
    break;

  case 118: // pattern-field: internal-identifier "`:`" pattern-sfx
#line 751 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; }
#line 3430 "bang.tab.cpp"
    break;

  case 119: // pattern-field: "`(`" syntax-expression "`)`" "`:`" pattern-sfx
#line 753 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[3].value.as < syntax_expression_t > ()), .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) == parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3436 "bang.tab.cpp"
    break;

  case 120: // pattern-mod: pattern-sfx
#line 757 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::const_or_runtime_type }; }
#line 3442 "bang.tab.cpp"
    break;

  case 121: // pattern-mod: "constexpr modifier" pattern-sfx
#line 758 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::const_type }; }
#line 3448 "bang.tab.cpp"
    break;

  case 122: // pattern-mod: "runctime modifier" pattern-sfx
#line 759 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::runtime_type }; }
#line 3454 "bang.tab.cpp"
    break;

  case 123: // pattern-mod: "typename modifier" pattern-sfx
#line 760 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::typename_type }; }
#line 3460 "bang.tab.cpp"
    break;

  case 124: // pattern-sfx: pattern
#line 764 "bang.y"
              { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[0].value.as < pattern_t > ()), parameter_constraint_modifier_t::none }; }
#line 3466 "bang.tab.cpp"
    break;

  case 125: // pattern-sfx: pattern "`...`"
#line 765 "bang.y"
                       { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[1].value.as < pattern_t > ()), parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3472 "bang.tab.cpp"
    break;

  case 126: // pattern-sfx: "`...`"
#line 766 "bang.y"
               { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ pattern_t{ .descriptor = placeholder{} }, parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3478 "bang.tab.cpp"
    break;

  case 127: // pattern: qname subpatterns-opt concept-expression-list-opt
#line 771 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3484 "bang.tab.cpp"
    break;

  case 128: // pattern: internal-identifier concept-expression-list-opt
#line 773 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3490 "bang.tab.cpp"
    break;

  case 129: // pattern: "`_`" subpatterns concept-expression-list-opt
#line 775 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3496 "bang.tab.cpp"
    break;

  case 130: // pattern: "`(`" syntax-expression "`)`" concept-expression-list-opt
#line 777 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[2].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3502 "bang.tab.cpp"
    break;

  case 131: // pattern: "`_`" concept-expression-list-opt
#line 779 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3508 "bang.tab.cpp"
    break;

  case 132: // pattern: concept-expression-list
#line 781 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3514 "bang.tab.cpp"
    break;

  case 133: // concept-expression: "`@`" qname
#line 786 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_reference{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3520 "bang.tab.cpp"
    break;

  case 134: // concept-expression-list-opt: %empty
#line 791 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = {}; }
#line 3526 "bang.tab.cpp"
    break;

  case 135: // concept-expression-list-opt: concept-expression-list
#line 792 "bang.y"
      { yylhs.value.as < concept_expression_list_t > () = yystack_[0].value.as < concept_expression_list_t > (); }
#line 3532 "bang.tab.cpp"
    break;

  case 136: // concept-expression-list: concept-expression
#line 797 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = concept_expression_list_t{std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3538 "bang.tab.cpp"
    break;

  case 137: // concept-expression-list: concept-expression-list concept-expression
#line 799 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = std::move(yystack_[1].value.as < concept_expression_list_t > ()); yylhs.value.as < concept_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3544 "bang.tab.cpp"
    break;

  case 138: // syntax-expression: CONTEXT_IDENTIFIER
#line 807 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3550 "bang.tab.cpp"
    break;

  case 139: // syntax-expression: CT_IDENTIFIER
#line 809 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3556 "bang.tab.cpp"
    break;

  case 140: // syntax-expression: syntax-expression-wo-ii
#line 810 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3562 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: "nil"
#line 816 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3568 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: "true"
#line 818 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3574 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: "false"
#line 820 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3580 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: INTEGER
#line 822 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3586 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: DECIMAL
#line 824 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3592 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: DECIMAL_S
#line 826 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3598 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: INTEGER_INDEX
#line 828 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3604 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: STRING
#line 830 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3610 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 832 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3616 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: qname
#line 834 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3622 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: "`(`" "`)`"
#line 836 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3628 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: "`(`" "`:`" syntax-expression "`)`"
#line 838 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, named_expression_list_t{ named_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) } } }; }
#line 3634 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 840 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                BOOST_ASSERT(!yystack_[1].value.as < named_expression_list_t > ().empty());
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3647 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 849 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3659 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 857 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3665 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 859 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3671 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: "`.`" identifier
#line 861 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3677 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 865 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3683 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 867 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3689 "bang.tab.cpp"
    break;

  case 160: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 869 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3695 "bang.tab.cpp"
    break;

  case 161: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 872 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3701 "bang.tab.cpp"
    break;

  case 162: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 874 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3707 "bang.tab.cpp"
    break;

  case 163: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 879 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3713 "bang.tab.cpp"
    break;

  case 164: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 881 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3719 "bang.tab.cpp"
    break;

  case 165: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 885 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3725 "bang.tab.cpp"
    break;

  case 166: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 887 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3731 "bang.tab.cpp"
    break;

  case 167: // syntax-expression-wo-ii: syntax-expression "`..`" syntax-expression
#line 889 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3737 "bang.tab.cpp"
    break;

  case 168: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 892 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3743 "bang.tab.cpp"
    break;

  case 169: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 896 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3749 "bang.tab.cpp"
    break;

  case 170: // syntax-expression-wo-ii: syntax-expression "`&&`" syntax-expression
#line 899 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3755 "bang.tab.cpp"
    break;

  case 171: // syntax-expression-wo-ii: syntax-expression "`||`" syntax-expression
#line 902 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3761 "bang.tab.cpp"
    break;

  case 172: // syntax-expression-wo-ii: apostrophe-expression
#line 904 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3767 "bang.tab.cpp"
    break;

  case 173: // syntax-expression-wo-ii: new-expression
#line 905 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3773 "bang.tab.cpp"
    break;

  case 174: // syntax-expression-wo-ii: compound-expression
#line 906 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3779 "bang.tab.cpp"
    break;

  case 175: // syntax-expression-wo-ii: lambda-expression
#line 907 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3785 "bang.tab.cpp"
    break;

  case 176: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 912 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3791 "bang.tab.cpp"
    break;

  case 177: // new-expression: "`new`" qname
#line 917 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3797 "bang.tab.cpp"
    break;

  case 178: // new-expression: "`new`" apostrophe-expression
#line 919 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3803 "bang.tab.cpp"
    break;

  case 179: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 921 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3809 "bang.tab.cpp"
    break;

  case 180: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 923 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3815 "bang.tab.cpp"
    break;

  case 181: // call-expression: qname "`(`" pack-expression-opt "`)`"
#line 928 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3821 "bang.tab.cpp"
    break;

  case 182: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3827 "bang.tab.cpp"
    break;

  case 183: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 938 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3833 "bang.tab.cpp"
    break;

  case 184: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 940 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3839 "bang.tab.cpp"
    break;

  case 185: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 945 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3845 "bang.tab.cpp"
    break;

  case 186: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 947 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3851 "bang.tab.cpp"
    break;

  case 187: // pack-expression-opt: %empty
#line 952 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{}; }
#line 3857 "bang.tab.cpp"
    break;

  case 188: // pack-expression-opt: pack-expression
#line 954 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[0].value.as < named_expression_list_t > ()); }
#line 3863 "bang.tab.cpp"
    break;

  case 189: // pack-expression: syntax-expression
#line 959 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{ named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3869 "bang.tab.cpp"
    break;

  case 190: // pack-expression: identifier "`:`" syntax-expression
#line 968 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3879 "bang.tab.cpp"
    break;

  case 191: // pack-expression: pack-expression "," syntax-expression
#line 974 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3888 "bang.tab.cpp"
    break;

  case 192: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 979 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3897 "bang.tab.cpp"
    break;

  case 193: // compound-expression: syntax-expression "`...`"
#line 987 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3903 "bang.tab.cpp"
    break;

  case 194: // compound-expression: call-expression
#line 988 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3909 "bang.tab.cpp"
    break;

  case 195: // type-expr: qname
#line 1030 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3915 "bang.tab.cpp"
    break;

  case 196: // type-expr: call-expression
#line 1031 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3921 "bang.tab.cpp"
    break;

  case 197: // type-expr: CONTEXT_IDENTIFIER
#line 1033 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3927 "bang.tab.cpp"
    break;

  case 198: // type-expr: "`[`" type-expr "`]`"
#line 1037 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3933 "bang.tab.cpp"
    break;

  case 199: // type-expr: "`(`" "`)`"
#line 1039 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3939 "bang.tab.cpp"
    break;

  case 200: // type-expr: "`(`" pack-expression "`)`"
#line 1042 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3945 "bang.tab.cpp"
    break;

  case 201: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1044 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3951 "bang.tab.cpp"
    break;

  case 202: // type-expr: type-expr "`|`" type-expr
#line 1046 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3963 "bang.tab.cpp"
    break;

  case 203: // type-expr: type-expr "`->`" type-expr
#line 1054 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 3972 "bang.tab.cpp"
    break;


#line 3976 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -241;

  const signed char parser::yytable_ninf_ = -96;

  const short
  parser::yypact_[] =
  {
     681,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
     802,    24,   781,   802,   802,    24,   802,   802,    -5,   131,
      23,   802,   802,   802,    21,    36,   -51,  -241,    21,    21,
    -241,  -241,  -241,    79,    15,   381,  -241,  -241,  -241,  -241,
     303,    13,   867,  -241,    50,  -241,    74,   112,    25,   119,
     904,  -241,  -241,   802,  -241,   126,  1018,   120,   236,  1018,
     205,  -241,    55,    55,    24,  -241,   153,    70,  -241,    24,
       5,   305,   170,   857,   571,   671,   160,  -241,  -241,  -241,
     292,  -241,    30,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,    24,   802,   499,  -241,   204,   176,   206,  -241,   802,
     802,   802,   802,   802,   802,  -241,   802,   157,   802,   802,
     802,   802,   802,   802,   802,  -241,   919,   802,  -241,   802,
    -241,   802,  -241,  -241,     2,   802,   218,   249,  -241,   802,
     802,   581,   802,  -241,   802,   220,   802,   499,    24,   262,
     499,  -241,  -241,   239,   212,  -241,    21,    46,  -241,   802,
     266,   378,   378,   378,    -4,    69,   326,   272,   263,  -241,
       2,   330,   330,  -241,   306,  -241,   331,   499,  -241,  1018,
     178,   178,  1065,  1055,   233,   956,  -241,  -241,    55,    55,
     411,   492,   162,   234,   235,  -241,  1018,   308,  1018,  1018,
    -241,   702,     2,   303,    50,    74,   112,   225,   212,     2,
     499,   108,   313,   298,  -241,   802,  1018,   327,  -241,  -241,
     802,    21,   317,   481,  -241,   328,  -241,  -241,    13,   671,
     671,   206,  1018,   334,  -241,   347,   307,  -241,    24,   354,
    -241,   204,   502,   331,  -241,   331,   971,  -241,  -241,   331,
    -241,  -241,  -241,  -241,   358,   499,  -241,   331,  -241,    39,
     499,   113,   802,  -241,  -241,  -241,  -241,   356,  -241,  -241,
    -241,  -241,   802,  -241,   241,   100,     2,   395,     2,   225,
     359,  -241,  -241,  -241,   802,  1018,  -241,  1018,   284,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,   386,  -241,    24,
     240,   360,   340,  -241,   137,   130,   802,   365,   177,   250,
    -241,  -241,  -241,   331,   499,   330,   330,  -241,     2,  -241,
    -241,  -241,  1018,   295,  1018,  -241,  -241,  -241,   361,    61,
     278,  -241,   499,   802,  -241,   802,     2,  -241,    24,   364,
    -241,   378,  1008,   378,   378,  -241,   502,  -241,   330,   330,
    -241,  -241,   125,   352,   802,     2,  -241,   366,  1018,  1018,
      72,  -241,    24,  -241,   200,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,  1018,   225,   206,   802,  -241,   367,   378,  1018,
    -241,  -241
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   148,    51,   138,   149,   139,   147,   144,   145,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
     141,   142,   143,     0,     0,     0,     5,     7,    15,    54,
     150,     0,     0,   140,   172,   173,   194,   175,   174,     0,
       0,   174,    53,     0,   151,    54,   189,     0,     0,    73,
       0,   157,   162,   161,     0,    42,    21,    17,    19,     0,
       0,   177,   178,     0,     0,     0,     0,    43,    13,    57,
       0,    39,     0,    16,     1,     2,    10,     3,     8,     6,
       9,     0,   187,    91,    59,    58,     0,     0,   160,     0,
       0,     0,     0,     0,     0,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   176,     0,     0,   153,     0,
     154,     0,   155,    20,     0,     0,     0,     0,    12,    75,
      75,    23,     0,    31,     0,    34,     0,    91,    64,     0,
      91,    37,    55,     0,   188,    52,     0,   134,   126,     0,
     105,     0,     0,     0,    54,   134,   110,     0,    92,    93,
       0,    97,    97,   120,   124,   136,   132,    91,    36,    45,
     165,   166,   170,   171,   167,     0,   158,   159,   163,   164,
     168,   169,     0,     0,     0,   152,   190,    54,   191,    74,
     197,     0,     0,   195,     0,   196,     0,    22,    18,     0,
      91,    54,     0,    76,    77,     0,    80,     0,    48,    47,
      49,     0,    40,    41,    26,     0,    46,    24,     0,     0,
       0,     0,    71,     0,    68,     0,    65,    66,    83,     0,
     181,   133,     0,   134,   131,   135,     0,   106,   107,   134,
     123,   121,   122,    96,     0,     0,   128,   134,   111,     0,
       0,   108,     0,   104,   101,   125,   137,     0,   156,   183,
     182,   184,     0,   199,     0,     0,     0,     0,     0,    11,
       0,    81,    82,   179,     0,    79,   180,    50,     0,    29,
      27,    28,    25,    30,    32,    33,    35,     0,    63,     0,
       0,     0,    84,    85,     0,   134,     0,    54,   134,     0,
     113,   115,   129,   134,     0,    97,    97,   127,     0,   185,
      94,   109,    98,    60,   192,   200,   198,   203,     0,   202,
      60,    78,    91,     0,    67,     0,     0,    69,     0,     0,
      38,     0,     0,     0,     0,   112,     0,   130,    97,    97,
     103,   100,     0,     0,     0,     0,   201,     0,    72,    90,
      87,    86,    83,   116,   134,   117,   118,   114,   102,    99,
     186,    62,    14,    61,     0,     0,    89,     0,     0,    88,
      70,   119
  };

  const short
  parser::yypgoto_[] =
  {
    -241,  -241,  -241,  -241,   383,  -241,  -241,  -241,  -241,  -241,
     -35,   -15,  -241,  -129,   207,   128,   -49,     4,    31,   382,
     362,  -241,  -241,  -241,   135,  -241,  -241,   416,   301,  -241,
     163,  -241,    82,  -241,  -241,   110,  -133,  -241,  -241,   -25,
     186,  -241,  -240,  -241,   288,  -241,   111,  -233,  -105,   103,
    -160,  -121,   124,     0,  -241,    35,  -241,     1,    56,  -241,
     -56,    37,    10
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    33,    34,    35,    36,    65,    66,    67,   212,   213,
     141,    37,   215,    38,   217,    39,   239,    40,    49,   127,
      97,    81,   225,   226,   227,    83,    77,    58,   202,   203,
     204,   205,   291,   292,   366,   293,   157,   158,   244,   253,
     159,   160,   161,   247,   233,   299,   300,   162,   163,   164,
     165,   246,   166,    56,    43,    44,    45,    46,    47,   143,
      57,    51,   197
  };

  const short
  parser::yytable_[] =
  {
      42,     2,   216,   145,   223,   305,   256,   229,     2,   190,
      50,     2,   306,    59,    59,    85,    62,    63,    10,     2,
      90,    73,    74,    75,    71,   -44,   234,     2,    76,     2,
       2,    41,    80,    82,   257,    42,   144,    48,   133,    10,
     135,    78,   -95,    79,   155,    95,   240,   241,   242,    11,
     191,   139,    11,   116,   192,    72,   182,   183,   184,   308,
      11,    93,   168,   146,   338,    98,    41,   270,    11,   198,
      11,   339,    48,    86,    95,   256,   -44,    91,   140,    84,
     131,   266,    94,   -44,   216,   125,   146,   365,   155,   131,
      94,   155,   266,    64,   232,    26,    27,   156,   112,   169,
     170,   171,   172,   173,   174,   243,   175,   106,   178,   179,
     180,   181,   302,   267,   107,   245,   214,   186,   155,   188,
     266,   189,   113,   271,   267,   195,   307,   301,   193,   206,
     206,    42,   219,   266,   220,   264,   222,   254,   268,    52,
      55,   156,   177,    61,   156,   266,    68,   146,   311,   236,
     231,   155,   267,   316,   272,   156,   156,   156,   329,   194,
     114,   195,   218,     2,   193,   267,   268,    93,    48,   118,
     251,   156,   117,    10,   234,   131,   331,   267,   232,   268,
     196,   136,   337,   298,   284,   285,   286,   131,    98,   347,
     119,   268,   123,   195,   146,   194,   193,   126,   281,   124,
     195,   155,   265,   193,   156,   275,    69,    91,   137,   269,
     277,   259,   104,    42,   309,   278,   196,   146,   130,   142,
      55,   154,    95,   334,   167,    70,   353,   194,   355,   356,
     106,   301,   119,   337,   194,   176,   156,   107,   108,   109,
      55,    55,    55,    98,   218,   266,   368,   187,   196,   156,
      48,    91,   312,    55,   156,   196,   131,   201,   201,   330,
     122,   325,   314,   371,   199,   154,   224,   195,   154,   195,
     193,   235,   193,   155,   206,   121,   317,   267,   319,   235,
     340,   341,   119,   260,   261,   106,   326,   298,   230,   120,
     315,   268,   107,   108,   109,   154,   332,   200,   343,   335,
     345,   194,   221,   194,   119,   119,   121,   360,   156,   195,
     228,   119,   193,   358,   359,   343,   344,   345,   342,    55,
     336,   249,   196,   348,   196,   349,   156,   195,   154,   330,
     193,    91,   322,   250,   131,   156,   350,   156,   156,    91,
     156,   255,   138,   194,   362,   252,   195,   156,   146,   193,
      91,    92,    91,   129,   262,   363,   290,   235,     2,   145,
     297,   194,   273,   235,   196,   369,   237,   238,   274,   146,
     147,   235,   156,    91,   232,   279,   276,   289,   154,   283,
     194,    87,   196,   287,     2,   145,     1,     2,     3,     4,
       5,     6,     7,     8,     9,   146,   147,    10,   288,    11,
     149,   196,   201,   294,   304,   313,   318,   323,   320,   327,
     328,   333,   352,   148,   346,   364,   370,   224,    89,   235,
     282,    98,   235,    96,   324,    11,   149,   235,    11,    12,
      60,   207,   128,    13,   367,    14,   310,   321,   351,    88,
      15,   100,   101,    16,   248,   104,   361,   357,    17,     0,
     154,     0,     0,     0,     0,    18,   290,    19,    20,    21,
      22,     0,    23,   106,   297,     0,     0,     0,    24,     0,
     107,   108,   109,    25,    26,    27,    28,    29,   235,     0,
     290,     0,     0,    30,    31,    32,     1,     2,     3,     4,
       5,     6,     7,     8,     9,     0,     0,    10,     0,     0,
       0,     0,    98,     0,     0,     2,   145,     0,     2,   145,
       0,     0,     0,     0,     0,     0,   146,   147,     0,   146,
     295,     0,   100,   101,     0,     0,   104,     0,    11,    12,
       0,     0,     0,    13,   148,    14,     0,   148,     0,   280,
      15,     0,     0,    16,   106,     0,    11,   149,    17,    11,
     296,   107,   108,   109,     0,    18,     0,   110,    20,    21,
      22,     0,    23,     0,   208,   209,   210,   150,    24,     0,
       0,     0,     0,     0,    26,    27,    28,   211,     0,     0,
       0,    98,     0,    30,    31,    32,     1,     2,     3,     4,
       5,     6,     7,     8,     9,     0,     0,    10,   151,   152,
     153,   100,   101,   102,   103,   104,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,     0,     0,     0,     0,    11,    12,
     107,   108,   109,    13,     0,    14,   110,   111,     0,     0,
      15,     0,     0,    16,     0,     0,     0,     0,    17,     0,
       0,   134,     0,     0,     0,    18,     0,     0,    20,    21,
      22,     0,    23,     0,   208,   209,   210,     0,    24,     0,
       0,     0,     0,     0,    26,    27,    28,   211,     0,     0,
       0,    98,     0,    30,    31,    32,     1,     2,     3,     4,
       5,     6,     7,     8,     9,     0,     0,    10,     0,     0,
       0,   100,   101,   102,   103,   104,   105,     1,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,    10,     0,
       0,   131,     0,   106,     0,     0,     0,     0,    11,    12,
     107,   108,   109,    13,     0,    14,   110,   111,     0,     0,
      15,     0,     0,    16,     0,     0,     0,     0,    17,    11,
      12,   263,     0,     0,    13,    18,    14,    19,    20,    21,
      22,    15,    23,     0,    16,     0,     0,     0,    24,    17,
       0,     0,     0,    25,    26,    27,    28,    29,     0,    20,
       0,     0,     0,    30,    31,    32,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    26,    27,    10,     0,     0,
       0,     0,     0,     0,    30,    31,    32,     1,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,    10,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    11,    12,
      54,     0,     0,    13,     0,    14,     0,     0,     0,     0,
      15,     0,     0,    16,     0,     0,     0,     0,    17,    11,
      12,     0,     0,     0,    13,     0,    14,     0,    20,     0,
       0,    15,     0,     0,    16,     0,     0,    98,     0,    17,
       0,     0,     0,     0,    26,    27,     0,    98,     0,    20,
       0,     0,    99,    30,    31,    32,     0,   100,   101,   102,
     103,   104,   105,     0,     0,    26,    27,   100,   101,   102,
     103,   104,   105,     0,    30,    31,    32,   131,     0,   106,
       0,     0,     0,     0,    98,   132,   107,   108,   109,   106,
     115,     0,   110,   111,     0,     0,   107,   108,   109,    98,
       0,     0,   110,   111,   100,   101,   102,   103,   104,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
     101,   102,   103,   104,   105,     0,   106,     0,     0,     0,
       0,     0,     0,   107,   108,   109,    98,     0,   185,   110,
     111,   106,     0,     0,     0,     0,     0,     0,   107,   108,
     109,    98,     0,     0,   110,   111,   100,   101,   102,   103,
     104,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,   101,   102,   103,   104,   105,     0,   106,   258,
       0,     0,     0,     0,     0,   107,   108,   109,    98,     0,
     303,   110,   111,   106,     0,     0,     0,     0,    98,     0,
     107,   108,   109,     0,     0,     0,   110,   111,   100,   101,
     102,   103,   104,   105,     0,     0,     0,     0,   100,   101,
     102,   103,   104,   105,     0,     0,     0,   354,     0,     0,
     106,     0,     0,     0,     0,    98,     0,   107,   108,   109,
     106,     0,     0,   110,   111,    98,     0,   107,   108,   109,
       0,     0,     0,   110,   111,   100,   101,   102,     0,   104,
       0,     0,     0,     0,     0,   100,   101,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,     0,     0,   107,   108,   109,   106,     0,     0,
     110,   111,     0,     0,   107,   108,   109,     0,     0,     0,
     110,   111
  };

  const short
  parser::yycheck_[] =
  {
       0,     6,   131,     7,   137,   245,   166,   140,     6,     7,
      10,     6,   245,    13,    14,     0,    16,    17,    16,     6,
      35,    21,    22,    23,    20,     0,   147,     6,    24,     6,
       6,     0,    28,    29,   167,    35,    92,     0,    73,    16,
      75,     5,    46,    94,    93,    41,   151,   152,   153,    47,
      48,    21,    47,    53,    52,    20,   112,   113,   114,    20,
      47,    48,    97,    17,   304,    10,    35,   200,    47,   125,
      47,   304,    35,    58,    70,   235,    51,    47,    48,     0,
      50,    20,    77,    58,   213,    15,    17,    15,   137,    50,
      77,   140,    20,    98,    48,    93,    94,    93,    48,    99,
     100,   101,   102,   103,   104,   154,   106,    52,   108,   109,
     110,   111,   233,    52,    59,    46,   131,   117,   167,   119,
      20,   121,    48,    15,    52,   124,   247,   232,   124,   129,
     130,   131,   132,    20,   134,   191,   136,   162,    66,    11,
      12,   137,   107,    15,   140,    20,    18,    17,    35,   149,
     146,   200,    52,    53,    46,   151,   152,   153,    21,   124,
      48,   160,   131,     6,   160,    52,    66,    48,   131,    49,
     160,   167,    46,    16,   295,    50,    46,    52,    48,    66,
     124,    21,   303,   232,   219,   220,   221,    50,    10,   322,
      70,    66,    64,   192,    17,   160,   192,    69,   213,    46,
     199,   250,   192,   199,   200,   205,    75,    47,    48,   199,
     210,    49,    34,   213,   249,   211,   160,    17,    48,    91,
      92,    93,   218,    46,    48,    94,   331,   192,   333,   334,
      52,   336,    70,   354,   199,   107,   232,    59,    60,    61,
     112,   113,   114,    10,   213,    20,    46,   119,   192,   245,
     213,    47,   252,   125,   250,   199,    50,   129,   130,   294,
      55,    21,   262,   368,    46,   137,   138,   266,   140,   268,
     266,   147,   268,   322,   274,    70,   266,    52,   268,   155,
     305,   306,    70,    49,    49,    52,    46,   336,    49,    53,
      49,    66,    59,    60,    61,   167,   296,    48,    20,    49,
      22,   266,    82,   268,    70,    70,    70,   342,   304,   308,
      48,    70,   308,   338,   339,    20,    21,    22,   308,   191,
      70,    49,   266,   323,   268,   325,   322,   326,   200,   364,
     326,    47,    48,    70,    50,   331,   326,   333,   334,    47,
     336,    35,    50,   308,   344,    15,   345,   343,    17,   345,
      47,    48,    47,    48,    46,   345,   228,   233,     6,     7,
     232,   326,    49,   239,   308,   365,   100,   101,    70,    17,
      18,   247,   368,    47,    48,    58,    49,    70,   250,    51,
     345,     0,   326,    49,     6,     7,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    17,    18,    16,    51,    47,
      48,   345,   274,    49,    46,    49,    11,    21,    49,    49,
      70,    46,    48,    35,    53,    49,    49,   289,    35,   295,
     213,    10,   298,    41,   289,    47,    48,   303,    47,    48,
      14,   130,    70,    52,   352,    54,   250,   274,   328,    58,
      59,    30,    31,    62,   156,    34,   343,   336,    67,    -1,
     322,    -1,    -1,    -1,    -1,    74,   328,    76,    77,    78,
      79,    -1,    81,    52,   336,    -1,    -1,    -1,    87,    -1,
      59,    60,    61,    92,    93,    94,    95,    96,   354,    -1,
     352,    -1,    -1,   102,   103,   104,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,
      -1,    -1,    10,    -1,    -1,     6,     7,    -1,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    17,
      18,    -1,    30,    31,    -1,    -1,    34,    -1,    47,    48,
      -1,    -1,    -1,    52,    35,    54,    -1,    35,    -1,    58,
      59,    -1,    -1,    62,    52,    -1,    47,    48,    67,    47,
      48,    59,    60,    61,    -1,    74,    -1,    65,    77,    78,
      79,    -1,    81,    -1,    83,    84,    85,    68,    87,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    10,    -1,   102,   103,   104,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    16,    99,   100,
     101,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    47,    48,
      59,    60,    61,    52,    -1,    54,    65,    66,    -1,    -1,
      59,    -1,    -1,    62,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    80,    -1,    -1,    -1,    74,    -1,    -1,    77,    78,
      79,    -1,    81,    -1,    83,    84,    85,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    10,    -1,   102,   103,   104,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    16,    -1,
      -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    47,    48,
      59,    60,    61,    52,    -1,    54,    65,    66,    -1,    -1,
      59,    -1,    -1,    62,    -1,    -1,    -1,    -1,    67,    47,
      48,    49,    -1,    -1,    52,    74,    54,    76,    77,    78,
      79,    59,    81,    -1,    62,    -1,    -1,    -1,    87,    67,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    -1,    77,
      -1,    -1,    -1,   102,   103,   104,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    93,    94,    16,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    62,    -1,    -1,    -1,    -1,    67,    47,
      48,    -1,    -1,    -1,    52,    -1,    54,    -1,    77,    -1,
      -1,    59,    -1,    -1,    62,    -1,    -1,    10,    -1,    67,
      -1,    -1,    -1,    -1,    93,    94,    -1,    10,    -1,    77,
      -1,    -1,    15,   102,   103,   104,    -1,    30,    31,    32,
      33,    34,    35,    -1,    -1,    93,    94,    30,    31,    32,
      33,    34,    35,    -1,   102,   103,   104,    50,    -1,    52,
      -1,    -1,    -1,    -1,    10,    58,    59,    60,    61,    52,
      16,    -1,    65,    66,    -1,    -1,    59,    60,    61,    10,
      -1,    -1,    65,    66,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    10,    -1,    49,    65,
      66,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,    10,    -1,    -1,    65,    66,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    -1,    52,    53,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,    10,    -1,
      49,    65,    66,    52,    -1,    -1,    -1,    -1,    10,    -1,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    10,    -1,    59,    60,    61,
      52,    -1,    -1,    65,    66,    10,    -1,    59,    60,    61,
      -1,    -1,    -1,    65,    66,    30,    31,    32,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    52,    -1,    -1,
      65,    66,    -1,    -1,    59,    60,    61,    -1,    -1,    -1,
      65,    66
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      16,    47,    48,    52,    54,    59,    62,    67,    74,    76,
      77,    78,    79,    81,    87,    92,    93,    94,    95,    96,
     102,   103,   104,   106,   107,   108,   109,   116,   118,   120,
     122,   123,   158,   159,   160,   161,   162,   163,   166,   123,
     158,   166,   120,    46,    49,   120,   158,   165,   132,   158,
     132,   120,   158,   158,    98,   110,   111,   112,   120,    75,
      94,   122,   160,   158,   158,   158,   122,   131,     5,    94,
     122,   126,   122,   130,     0,     0,    58,     0,    58,   109,
     116,    47,    48,    48,    77,   122,   124,   125,    10,    15,
      30,    31,    32,    33,    34,    35,    52,    59,    60,    61,
      65,    66,    48,    48,    48,    16,   158,    46,    49,    70,
      53,    70,    55,   120,    46,    15,   120,   124,   125,    48,
      48,    50,    58,   115,    80,   115,    21,    48,    50,    21,
      48,   115,   120,   164,   165,     7,    17,    18,    35,    48,
      68,    99,   100,   101,   120,   121,   122,   141,   142,   145,
     146,   147,   152,   153,   154,   155,   157,    48,   115,   158,
     158,   158,   158,   158,   158,   158,   120,   160,   158,   158,
     158,   158,   165,   165,   165,    49,   158,   120,   158,   158,
       7,    48,    52,   122,   160,   162,   163,   167,   165,    46,
      48,   120,   133,   134,   135,   136,   158,   133,    83,    84,
      85,    96,   113,   114,   116,   117,   118,   119,   123,   158,
     158,    82,   158,   141,   120,   127,   128,   129,    48,   141,
      49,   122,    48,   149,   156,   157,   158,   100,   101,   121,
     153,   153,   153,   121,   143,    46,   156,   148,   149,    49,
      70,   167,    15,   144,   144,    35,   155,   141,    53,    49,
      49,    49,    46,    49,   165,   167,    20,    52,    66,   167,
     141,    15,    46,    49,    70,   158,    49,   158,   122,    58,
      58,   116,   119,    51,   115,   115,   115,    49,    51,    70,
     120,   137,   138,   140,    49,    18,    48,   120,   121,   150,
     151,   153,   156,    49,    46,   147,   152,   156,    20,   115,
     145,    35,   158,    49,   158,    49,    53,   167,    11,   167,
      49,   135,    48,    21,   129,    21,    46,    49,    70,    21,
     115,    46,   158,    46,    46,    49,    70,   156,   147,   152,
     144,   144,   167,    20,    21,    22,    53,   141,   158,   158,
     167,   140,    48,   153,    49,   153,   153,   151,   144,   144,
     115,   154,   158,   167,    49,    15,   139,   137,    46,   158,
      49,   153
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   105,   106,   106,   107,   107,   107,   108,   108,   108,
     108,   109,   109,   109,   109,   109,   109,   110,   110,   111,
     111,   112,   112,   113,   113,   113,   114,   114,   114,   114,
     115,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     117,   117,   118,   118,   118,   118,   119,   119,   119,   119,
     119,   120,   121,   122,   122,   122,   123,   123,   124,   124,
     125,   125,   125,   126,   127,   127,   128,   128,   129,   130,
     130,   131,   131,   132,   132,   133,   133,   134,   134,   135,
     135,   136,   136,   137,   137,   138,   138,   139,   139,   140,
     140,   141,   141,   142,   142,   143,   143,   144,   144,   145,
     145,   145,   145,   145,   145,   146,   146,   146,   147,   147,
     148,   148,   149,   150,   150,   151,   151,   151,   151,   151,
     152,   152,   152,   152,   153,   153,   153,   154,   154,   154,
     154,   154,   154,   155,   156,   156,   157,   157,   158,   158,
     158,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   160,   161,   161,   161,
     161,   162,   162,   162,   162,   163,   163,   164,   164,   165,
     165,   165,   165,   166,   166,   167,   167,   167,   167,   167,
     167,   167,   167,   167
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
       3,     0,     1,     1,     3,     0,     1,     0,     2,     5,
       4,     2,     5,     4,     2,     1,     2,     2,     2,     3,
       0,     1,     3,     1,     3,     1,     3,     3,     3,     5,
       1,     2,     2,     2,     1,     2,     1,     3,     2,     3,
       4,     2,     1,     2,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     3,     3,     3,     4,     2,     3,     3,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     3,     2,     2,     5,
       5,     4,     4,     4,     4,     5,     7,     0,     1,     1,
       3,     3,     5,     2,     1,     1,     1,     1,     3,     2,
       3,     4,     3,     3
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
  "\"typename modifier\"", "\"constexpr modifier\"",
  "\"runctime modifier\"", "\"nil\"", "\"true\"", "\"false\"", "$accept",
  "begin", "statement_any", "finished-statement-any", "statement",
  "let-decl", "let-decl-start", "let-decl-start-with-opt-type",
  "infunction-statement-any", "finished-infunction-statement-any",
  "braced-statements", "finished-statement", "infunction-statement-set",
  "generic-statement", "infunction-statement", "identifier",
  "internal-identifier", "qname", "fn-start-decl", "fn-name", "fn-decl",
  "enum-decl", "case-list-opt", "case-list", "case-decl", "struct-decl",
  "using-decl", "expression-list", "argument-list-opt", "argument-list",
  "argument", "argument-name", "field-list-opt", "field-list",
  "field-default-value-opt", "field", "parameter-list-opt",
  "parameter-list", "internal-identifier-opt",
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
     566,   589,   596,   614,   616,   622,   623,   627,   629,   634,
     636,   641,   643,   649,   650,   654,   656,   661,   662,   666,
     668,   675,   676,   680,   682,   687,   688,   692,   693,   697,
     699,   701,   703,   705,   707,   712,   713,   714,   718,   720,
     726,   728,   732,   737,   739,   744,   746,   748,   750,   752,
     757,   758,   759,   760,   764,   765,   766,   770,   772,   774,
     776,   778,   780,   785,   790,   792,   796,   798,   806,   808,
     810,   815,   817,   819,   821,   823,   825,   827,   829,   831,
     833,   835,   837,   839,   848,   856,   858,   860,   864,   866,
     868,   871,   873,   878,   880,   884,   886,   888,   891,   895,
     898,   901,   904,   905,   906,   907,   911,   916,   918,   920,
     922,   927,   935,   937,   939,   944,   946,   951,   953,   958,
     967,   973,   978,   986,   988,  1029,  1031,  1032,  1036,  1038,
    1041,  1043,  1045,  1053
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
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104
    };
    // Last valid token kind.
    const int code_max = 359;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4938 "bang.tab.cpp"

#line 1380 "bang.y"

