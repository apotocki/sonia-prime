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

      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.copy< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
      case symbol_kind::S_151_pattern_field: // pattern-field
        value.copy< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_136_argument_name: // argument-name
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

      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::context_identifier > (YY_MOVE (that.value));
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
      case symbol_kind::S_QMARK: // "`?`"
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

      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
      case symbol_kind::S_151_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_136_argument_name: // argument-name
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

      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::context_identifier > (YY_MOVE (s.value));
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
      case symbol_kind::S_QMARK: // "`?`"
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

      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.YY_MOVE_OR_COPY< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.YY_MOVE_OR_COPY< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
      case symbol_kind::S_151_pattern_field: // pattern-field
        value.YY_MOVE_OR_COPY< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_136_argument_name: // argument-name
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

      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.YY_MOVE_OR_COPY< sonia::lang::bang::context_identifier > (YY_MOVE (that.value));
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
      case symbol_kind::S_QMARK: // "`?`"
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

      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
      case symbol_kind::S_151_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_136_argument_name: // argument-name
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

      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::context_identifier > (YY_MOVE (that.value));
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
      case symbol_kind::S_QMARK: // "`?`"
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

      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.copy< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (that.value);
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
      case symbol_kind::S_151_pattern_field: // pattern-field
        value.copy< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_136_argument_name: // argument-name
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

      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::context_identifier > (that.value);
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
      case symbol_kind::S_QMARK: // "`?`"
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

      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        value.move< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (that.value);
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
      case symbol_kind::S_151_pattern_field: // pattern-field
        value.move< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_136_argument_name: // argument-name
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

      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::context_identifier > (that.value);
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
      case symbol_kind::S_QMARK: // "`?`"
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
#line 328 "bang.y"
                 { }
#line 1697 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 328 "bang.y"
                 { }
#line 1703 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
#line 328 "bang.y"
                 { }
#line 1709 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 328 "bang.y"
                 { }
#line 1715 "bang.tab.cpp"
        break;

      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
#line 328 "bang.y"
                 { }
#line 1721 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 328 "bang.y"
                 { }
#line 1727 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 328 "bang.y"
                 { }
#line 1733 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 328 "bang.y"
                 { }
#line 1739 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 328 "bang.y"
                 { }
#line 1745 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 328 "bang.y"
                 { }
#line 1751 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 328 "bang.y"
                 { }
#line 1757 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 328 "bang.y"
                 { }
#line 1763 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 328 "bang.y"
                 { }
#line 1769 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 328 "bang.y"
                 { }
#line 1775 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 328 "bang.y"
                 { }
#line 1781 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 328 "bang.y"
                 { }
#line 1787 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 328 "bang.y"
                 { }
#line 1793 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 328 "bang.y"
                 { }
#line 1799 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 328 "bang.y"
                 { }
#line 1805 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 328 "bang.y"
                 { }
#line 1811 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 328 "bang.y"
                 { }
#line 1817 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 328 "bang.y"
                 { }
#line 1823 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 328 "bang.y"
                 { }
#line 1829 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 328 "bang.y"
                 { }
#line 1835 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 328 "bang.y"
                 { }
#line 1841 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 328 "bang.y"
                 { }
#line 1847 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 328 "bang.y"
                 { }
#line 1853 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 328 "bang.y"
                 { }
#line 1859 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 328 "bang.y"
                 { }
#line 1865 "bang.tab.cpp"
        break;

      case symbol_kind::S_QMARK: // "`?`"
#line 328 "bang.y"
                 { }
#line 1871 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 328 "bang.y"
                 { }
#line 1877 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 328 "bang.y"
                 { }
#line 1883 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 328 "bang.y"
                 { }
#line 1889 "bang.tab.cpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 328 "bang.y"
                 { }
#line 1895 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 328 "bang.y"
                 { }
#line 1901 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 328 "bang.y"
                 { }
#line 1907 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 328 "bang.y"
                 { }
#line 1913 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 328 "bang.y"
                 { }
#line 1919 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 328 "bang.y"
                 { }
#line 1925 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_finished_statement_any: // finished-statement-any
#line 328 "bang.y"
                 { }
#line 1931 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 328 "bang.y"
                 { }
#line 1937 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl: // let-decl
#line 328 "bang.y"
                 { }
#line 1943 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_let_decl_start: // let-decl-start
#line 328 "bang.y"
                 { }
#line 1949 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 328 "bang.y"
                 { }
#line 1955 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_infunction_statement_any: // infunction-statement-any
#line 328 "bang.y"
                 { }
#line 1961 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_finished_infunction_statement_any: // finished-infunction-statement-any
#line 328 "bang.y"
                 { }
#line 1967 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_braced_statements: // braced-statements
#line 328 "bang.y"
                 { }
#line 1973 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_finished_statement: // finished-statement
#line 328 "bang.y"
                 { }
#line 1979 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_infunction_statement_set: // infunction-statement-set
#line 328 "bang.y"
                 { }
#line 1985 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_generic_statement: // generic-statement
#line 328 "bang.y"
                 { }
#line 1991 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_infunction_statement: // infunction-statement
#line 328 "bang.y"
                 { }
#line 1997 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 328 "bang.y"
                 { }
#line 2003 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_internal_identifier: // internal-identifier
#line 328 "bang.y"
                 { }
#line 2009 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 328 "bang.y"
                 { }
#line 2015 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_start_decl: // fn-start-decl
#line 328 "bang.y"
                 { }
#line 2021 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_fn_name: // fn-name
#line 328 "bang.y"
                 { }
#line 2027 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_fn_decl: // fn-decl
#line 328 "bang.y"
                 { }
#line 2033 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_enum_decl: // enum-decl
#line 328 "bang.y"
                 { }
#line 2039 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_list_opt: // case-list-opt
#line 328 "bang.y"
                 { }
#line 2045 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_case_list: // case-list
#line 328 "bang.y"
                 { }
#line 2051 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_case_decl: // case-decl
#line 328 "bang.y"
                 { }
#line 2057 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_struct_decl: // struct-decl
#line 328 "bang.y"
                 { }
#line 2063 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_using_decl: // using-decl
#line 328 "bang.y"
                 { }
#line 2069 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_expression_list: // expression-list
#line 328 "bang.y"
                 { }
#line 2075 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_argument_list_opt: // argument-list-opt
#line 328 "bang.y"
                 { }
#line 2081 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_argument_list: // argument-list
#line 328 "bang.y"
                 { }
#line 2087 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 328 "bang.y"
                 { }
#line 2093 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_argument_name: // argument-name
#line 328 "bang.y"
                 { }
#line 2099 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_list_opt: // field-list-opt
#line 328 "bang.y"
                 { }
#line 2105 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_field_list: // field-list
#line 328 "bang.y"
                 { }
#line 2111 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_field_default_value_opt: // field-default-value-opt
#line 328 "bang.y"
                 { }
#line 2117 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 328 "bang.y"
                 { }
#line 2123 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_list_opt: // parameter-list-opt
#line 328 "bang.y"
                 { }
#line 2129 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_parameter_list: // parameter-list
#line 328 "bang.y"
                 { }
#line 2135 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
#line 328 "bang.y"
                 { }
#line 2141 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_default_value_opt: // parameter-default-value-opt
#line 328 "bang.y"
                 { }
#line 2147 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_parameter_decl: // parameter-decl
#line 328 "bang.y"
                 { }
#line 2153 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_constraint_expression_mod: // constraint-expression-mod
#line 328 "bang.y"
                 { }
#line 2159 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_constraint_expression: // constraint-expression
#line 328 "bang.y"
                 { }
#line 2165 "bang.tab.cpp"
        break;

      case symbol_kind::S_subpatterns: // subpatterns
#line 328 "bang.y"
                 { }
#line 2171 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_pattern_list: // pattern-list
#line 328 "bang.y"
                 { }
#line 2177 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
#line 328 "bang.y"
                 { }
#line 2183 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_pattern_field: // pattern-field
#line 328 "bang.y"
                 { }
#line 2189 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_pattern_mod: // pattern-mod
#line 328 "bang.y"
                 { }
#line 2195 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_pattern_sfx: // pattern-sfx
#line 328 "bang.y"
                 { }
#line 2201 "bang.tab.cpp"
        break;

      case symbol_kind::S_pattern: // pattern
#line 328 "bang.y"
                 { }
#line 2207 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_concept_expression: // concept-expression
#line 328 "bang.y"
                 { }
#line 2213 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_concept_expression_list_opt: // concept-expression-list-opt
#line 328 "bang.y"
                 { }
#line 2219 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_concept_expression_list: // concept-expression-list
#line 328 "bang.y"
                 { }
#line 2225 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_syntax_expression: // syntax-expression
#line 328 "bang.y"
                 { }
#line 2231 "bang.tab.cpp"
        break;

      case symbol_kind::S_159_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 328 "bang.y"
                 { }
#line 2237 "bang.tab.cpp"
        break;

      case symbol_kind::S_160_apostrophe_expression: // apostrophe-expression
#line 328 "bang.y"
                 { }
#line 2243 "bang.tab.cpp"
        break;

      case symbol_kind::S_161_new_expression: // new-expression
#line 328 "bang.y"
                 { }
#line 2249 "bang.tab.cpp"
        break;

      case symbol_kind::S_162_call_expression: // call-expression
#line 328 "bang.y"
                 { }
#line 2255 "bang.tab.cpp"
        break;

      case symbol_kind::S_163_lambda_expression: // lambda-expression
#line 328 "bang.y"
                 { }
#line 2261 "bang.tab.cpp"
        break;

      case symbol_kind::S_164_pack_expression_opt: // pack-expression-opt
#line 328 "bang.y"
                 { }
#line 2267 "bang.tab.cpp"
        break;

      case symbol_kind::S_165_pack_expression: // pack-expression
#line 328 "bang.y"
                 { }
#line 2273 "bang.tab.cpp"
        break;

      case symbol_kind::S_166_compound_expression: // compound-expression
#line 328 "bang.y"
                 { }
#line 2279 "bang.tab.cpp"
        break;

      case symbol_kind::S_167_type_expr: // type-expr
#line 328 "bang.y"
                 { }
#line 2285 "bang.tab.cpp"
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

      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_149_pattern_list: // pattern-list
        yylhs.value.emplace< pattern_list_t > ();
        break;

      case symbol_kind::S_pattern: // pattern
        yylhs.value.emplace< pattern_t > ();
        break;

      case symbol_kind::S_150_pattern_field_sfx: // pattern-field-sfx
      case symbol_kind::S_151_pattern_field: // pattern-field
        yylhs.value.emplace< pattern_t::field > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_136_argument_name: // argument-name
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

      case symbol_kind::S_121_internal_identifier: // internal-identifier
      case symbol_kind::S_143_internal_identifier_opt: // internal-identifier-opt
        yylhs.value.emplace< sonia::lang::bang::context_identifier > ();
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
      case symbol_kind::S_QMARK: // "`?`"
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
#line 333 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2750 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 334 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2756 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 339 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2762 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 341 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2768 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 343 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2774 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 360 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2780 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 361 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2786 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 363 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2792 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 365 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2798 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 370 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2804 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 372 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2810 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 374 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2816 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-name "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 376 "bang.y"
        { 
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) });
            yylhs.value.as < statement > () = fn_decl_t{ fn_pure_t{ .nameval = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().value), .location = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().location), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = nullptr, .kind = yystack_[6].value.as < fn_kind > () }, ctx.push(std::move(sts)) };
            IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
            //     $$ = fn_decl_t{ fn_pure_t{ .nameval = std::move($name.value), .location = std::move($name.location), .parameters = std::move($parameters), .result = std::move($value), .is_type_expression_result = false, .kind = $fnkind } }; IGNORE_TERM($OPEN_PARENTHESIS); }
        }
#line 2829 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 385 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2835 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 387 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2841 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 392 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2847 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 394 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2853 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: identifier
#line 399 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2859 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: "weak modifier" identifier
#line 401 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2865 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 405 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2871 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 407 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2877 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 412 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2883 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 414 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2889 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 416 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2895 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 421 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2901 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 422 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2907 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 424 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2913 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 426 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2919 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 431 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2925 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 436 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2931 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 438 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2937 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 440 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2943 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 442 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2949 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 444 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2955 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 446 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2961 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 448 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ .name = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), .body = ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2967 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 450 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ .name = std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[2].value.as < parameter_list_t > ()), .body = ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2973 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 452 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2979 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 457 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2985 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 459 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2991 "bang.tab.cpp"
    break;

  case 42: // generic-statement: LET let-decl
#line 464 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2997 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 466 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 3003 "bang.tab.cpp"
    break;

  case 44: // generic-statement: compound-expression
#line 468 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3009 "bang.tab.cpp"
    break;

  case 45: // generic-statement: syntax-expression "`=`" syntax-expression
#line 470 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 3015 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: generic-statement
#line 475 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 3021 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: "`break`"
#line 477 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3027 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`continue`"
#line 479 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3033 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`return`"
#line 481 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .location = std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3039 "bang.tab.cpp"
    break;

  case 50: // infunction-statement: "`return`" syntax-expression
#line 483 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3045 "bang.tab.cpp"
    break;

  case 51: // identifier: IDENTIFIER
#line 488 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3051 "bang.tab.cpp"
    break;

  case 52: // internal-identifier: CONTEXT_IDENTIFIER
#line 496 "bang.y"
        { yylhs.value.as < sonia::lang::bang::context_identifier > () = context_identifier { ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()) }; }
#line 3057 "bang.tab.cpp"
    break;

  case 53: // qname: "`::`" identifier
#line 511 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 3063 "bang.tab.cpp"
    break;

  case 54: // qname: identifier
#line 513 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 3069 "bang.tab.cpp"
    break;

  case 55: // qname: qname "`::`" identifier
#line 515 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 3075 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: "`fn`"
#line 521 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3081 "bang.tab.cpp"
    break;

  case 57: // fn-start-decl: INLINE "`fn`"
#line 523 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3087 "bang.tab.cpp"
    break;

  case 58: // fn-name: qname
#line 527 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 3093 "bang.tab.cpp"
    break;

  case 59: // fn-name: "`new`"
#line 528 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 3099 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 533 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .nameval = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), .location = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location), .parameters = std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3105 "bang.tab.cpp"
    break;

  case 61: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`~>`" type-expr
#line 535 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .nameval = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().value), .location = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().location), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3111 "bang.tab.cpp"
    break;

  case 62: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" pattern
#line 537 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .nameval = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().value), .location = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().location), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3117 "bang.tab.cpp"
    break;

  case 63: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 543 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3123 "bang.tab.cpp"
    break;

  case 64: // case-list-opt: %empty
#line 547 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 3129 "bang.tab.cpp"
    break;

  case 65: // case-list-opt: case-list
#line 548 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 3135 "bang.tab.cpp"
    break;

  case 66: // case-list: case-decl
#line 553 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 3141 "bang.tab.cpp"
    break;

  case 67: // case-list: case-list "," case-decl
#line 555 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 3147 "bang.tab.cpp"
    break;

  case 68: // case-decl: identifier
#line 560 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 3153 "bang.tab.cpp"
    break;

  case 69: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 566 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ .name = std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), .body = std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3159 "bang.tab.cpp"
    break;

  case 70: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 568 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ .name = std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[5].value.as < parameter_list_t > ()), .body = std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3165 "bang.tab.cpp"
    break;

  case 71: // using-decl: qname "`=>`" syntax-expression
#line 591 "bang.y"
        { 
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(loc) });
            yylhs.value.as < using_decl > () = using_decl{ fn_decl_t{ fn_pure_t{ .nameval = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ().value), .location = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ().location), .result = nullptr }, ctx.push(std::move(sts)) } };
        }
#line 3176 "bang.tab.cpp"
    break;

  case 72: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 598 "bang.y"
        {
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) });
            yylhs.value.as < using_decl > () = using_decl{ fn_decl_t{ fn_pure_t{ .nameval = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().value), .location = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ().location), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = nullptr }, ctx.push(std::move(sts)) } };
            IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3188 "bang.tab.cpp"
    break;

  case 73: // expression-list: syntax-expression
#line 616 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3194 "bang.tab.cpp"
    break;

  case 74: // expression-list: expression-list "," syntax-expression
#line 618 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3200 "bang.tab.cpp"
    break;

  case 75: // argument-list-opt: %empty
#line 623 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 3206 "bang.tab.cpp"
    break;

  case 76: // argument-list-opt: argument-list
#line 624 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 3212 "bang.tab.cpp"
    break;

  case 77: // argument-list: argument
#line 629 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 3218 "bang.tab.cpp"
    break;

  case 78: // argument-list: argument-list "," argument
#line 631 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 3224 "bang.tab.cpp"
    break;

  case 79: // argument: argument-name syntax-expression
#line 636 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3230 "bang.tab.cpp"
    break;

  case 80: // argument: syntax-expression
#line 638 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3236 "bang.tab.cpp"
    break;

  case 81: // argument-name: identifier "`=`"
#line 643 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3242 "bang.tab.cpp"
    break;

  case 82: // argument-name: identifier "`:`"
#line 645 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 3248 "bang.tab.cpp"
    break;

  case 83: // field-list-opt: %empty
#line 650 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 3254 "bang.tab.cpp"
    break;

  case 84: // field-list-opt: field-list
#line 651 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 3260 "bang.tab.cpp"
    break;

  case 85: // field-list: field
#line 656 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 3266 "bang.tab.cpp"
    break;

  case 86: // field-list: field-list "," field
#line 658 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 3272 "bang.tab.cpp"
    break;

  case 87: // field-default-value-opt: %empty
#line 662 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3278 "bang.tab.cpp"
    break;

  case 88: // field-default-value-opt: "`=`" syntax-expression
#line 663 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3284 "bang.tab.cpp"
    break;

  case 89: // field: identifier "`:`" type-expr field-default-value-opt
#line 668 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), parameter_constraint_modifier_t::runtime_type, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3290 "bang.tab.cpp"
    break;

  case 90: // field: identifier "`=>`" syntax-expression
#line 670 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), parameter_constraint_modifier_t::any_constexpr_type, std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3296 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 676 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3302 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 677 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3308 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 682 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3314 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 684 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3320 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 688 "bang.y"
             { yylhs.value.as < sonia::lang::bang::context_identifier > () = context_identifier{}; }
#line 3326 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 689 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::context_identifier > () = std::move(yystack_[0].value.as < sonia::lang::bang::context_identifier > ()); }
#line 3332 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 693 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3338 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 694 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3344 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" pattern-mod parameter-default-value-opt
#line 699 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::context_identifier > ().name) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3350 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: internal-identifier "`:`" pattern-mod parameter-default-value-opt
#line 701 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::context_identifier > ().name) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3356 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: "`:`" pattern-mod parameter-default-value-opt
#line 703 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) }; }
#line 3362 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: internal-identifier parameter-default-value-opt
#line 706 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::context_identifier > ().name) }, .constraint = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::bang::context_identifier > ().name.location) }}, .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  parameter_constraint_modifier_t::const_or_runtime_type }; }
#line 3368 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: "`_`" parameter-default-value-opt
#line 708 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }}, .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  parameter_constraint_modifier_t::const_or_runtime_type }; }
#line 3374 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: internal-identifier "`...`" parameter-default-value-opt
#line 710 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::context_identifier > ().name) }, .constraint = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }}, .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  parameter_constraint_modifier_t::const_or_runtime_type | parameter_constraint_modifier_t::ellipsis }; }
#line 3380 "bang.tab.cpp"
    break;

  case 105: // parameter-decl: "`...`" parameter-default-value-opt
#line 712 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }}, .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier =  parameter_constraint_modifier_t::const_or_runtime_type | parameter_constraint_modifier_t::ellipsis }; }
#line 3386 "bang.tab.cpp"
    break;

  case 106: // parameter-decl: identifier internal-identifier-opt "`:`" constraint-expression parameter-default-value-opt
#line 717 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::context_identifier > ().name) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3392 "bang.tab.cpp"
    break;

  case 107: // parameter-decl: internal-identifier "`:`" constraint-expression parameter-default-value-opt
#line 719 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::context_identifier > ().name) }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3398 "bang.tab.cpp"
    break;

  case 108: // parameter-decl: "`:`" constraint-expression parameter-default-value-opt
#line 721 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3404 "bang.tab.cpp"
    break;

  case 109: // parameter-decl: constraint-expression parameter-default-value-opt
#line 723 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(get<0>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ())), .default_value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()), .modifier = get<1>(yystack_[1].value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > ()) }; }
#line 3410 "bang.tab.cpp"
    break;

  case 110: // constraint-expression-mod: "`~`"
#line 727 "bang.y"
            { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::const_or_runtime_type; }
#line 3416 "bang.tab.cpp"
    break;

  case 111: // constraint-expression-mod: "`~`" "constexpr modifier"
#line 728 "bang.y"
                      { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::any_constexpr_type; }
#line 3422 "bang.tab.cpp"
    break;

  case 112: // constraint-expression-mod: "`~`" "runctime modifier"
#line 729 "bang.y"
                    { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::runtime_type; }
#line 3428 "bang.tab.cpp"
    break;

  case 113: // constraint-expression: constraint-expression-mod type-expr
#line 734 "bang.y"
        { yylhs.value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[0].value.as < syntax_expression_t > ()), yystack_[1].value.as < parameter_constraint_modifier_t > () }; }
#line 3434 "bang.tab.cpp"
    break;

  case 114: // constraint-expression: constraint-expression-mod type-expr "`...`"
#line 736 "bang.y"
        { yylhs.value.as < std::pair<syntax_expression_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[1].value.as < syntax_expression_t > ()), yystack_[2].value.as < parameter_constraint_modifier_t > () | parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3440 "bang.tab.cpp"
    break;

  case 115: // subpatterns: "`(`" pattern-list "`)`"
#line 742 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[1].value.as < pattern_list_t > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3446 "bang.tab.cpp"
    break;

  case 116: // pattern-list: pattern-field
#line 747 "bang.y"
        { yylhs.value.as < pattern_list_t > () = pattern_list_t{ std::move(yystack_[0].value.as < pattern_t::field > ()) }; }
#line 3452 "bang.tab.cpp"
    break;

  case 117: // pattern-list: pattern-list "," pattern-field
#line 749 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[2].value.as < pattern_list_t > ()); yylhs.value.as < pattern_list_t > ().emplace_back(std::move(yystack_[0].value.as < pattern_t::field > ())); }
#line 3458 "bang.tab.cpp"
    break;

  case 118: // pattern-field-sfx: concept-expression-list-opt
#line 755 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = pattern_t{ .descriptor = placeholder{ }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) } }; }
#line 3464 "bang.tab.cpp"
    break;

  case 119: // pattern-field-sfx: concept-expression-list-opt "`...`"
#line 757 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = pattern_t{ .descriptor = placeholder{ }, .concepts = std::move(yystack_[1].value.as < concept_expression_list_t > ()) }, .ellipsis = true }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3470 "bang.tab.cpp"
    break;

  case 120: // pattern-field-sfx: internal-identifier concept-expression-list-opt
#line 760 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .bound_variable = std::move(yystack_[1].value.as < sonia::lang::bang::context_identifier > ().name), .value = pattern_t{ .descriptor = placeholder{ }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) } }; }
#line 3476 "bang.tab.cpp"
    break;

  case 121: // pattern-field-sfx: internal-identifier concept-expression-list-opt "`...`"
#line 762 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .bound_variable = std::move(yystack_[2].value.as < sonia::lang::bang::context_identifier > ().name), .value = pattern_t{ .descriptor = placeholder{ }, .concepts = std::move(yystack_[1].value.as < concept_expression_list_t > ()) }, .ellipsis = true }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3482 "bang.tab.cpp"
    break;

  case 122: // pattern-field-sfx: "`=`" pattern-sfx
#line 766 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = has(get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()), parameter_constraint_modifier_t::ellipsis) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3488 "bang.tab.cpp"
    break;

  case 123: // pattern-field-sfx: internal-identifier "`=`" pattern-sfx
#line 769 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .bound_variable = std::move(yystack_[2].value.as < sonia::lang::bang::context_identifier > ().name), .value = std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), .ellipsis = has(get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()), parameter_constraint_modifier_t::ellipsis) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3494 "bang.tab.cpp"
    break;

  case 124: // pattern-field: identifier pattern-field-sfx
#line 775 "bang.y"
        { yylhs.value.as < pattern_t::field > () = std::move(yystack_[0].value.as < pattern_t::field > ()); yylhs.value.as < pattern_t::field > ().name = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 3500 "bang.tab.cpp"
    break;

  case 125: // pattern-field: "`?`" internal-identifier pattern-field-sfx
#line 779 "bang.y"
        { yylhs.value.as < pattern_t::field > () = std::move(yystack_[0].value.as < pattern_t::field > ()); yylhs.value.as < pattern_t::field > ().name = std::move(yystack_[1].value.as < sonia::lang::bang::context_identifier > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3506 "bang.tab.cpp"
    break;

  case 126: // pattern-field: pattern-field-sfx
#line 792 "bang.y"
        { yylhs.value.as < pattern_t::field > () = std::move(yystack_[0].value.as < pattern_t::field > ()); }
#line 3512 "bang.tab.cpp"
    break;

  case 127: // pattern-field: "`_`" pattern-field-sfx
#line 795 "bang.y"
        { yylhs.value.as < pattern_t::field > () = std::move(yystack_[0].value.as < pattern_t::field > ());  yylhs.value.as < pattern_t::field > ().name = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3518 "bang.tab.cpp"
    break;

  case 128: // pattern-mod: pattern-sfx
#line 799 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::const_or_runtime_type }; }
#line 3524 "bang.tab.cpp"
    break;

  case 129: // pattern-mod: "constexpr modifier" pattern-sfx
#line 800 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::constexpr_value_type }; }
#line 3530 "bang.tab.cpp"
    break;

  case 130: // pattern-mod: "runctime modifier" pattern-sfx
#line 801 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::runtime_type }; }
#line 3536 "bang.tab.cpp"
    break;

  case 131: // pattern-mod: "typename modifier" pattern-sfx
#line 802 "bang.y"
                                        { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(get<0>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ())), get<1>(yystack_[0].value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > ()) | parameter_constraint_modifier_t::typename_type }; }
#line 3542 "bang.tab.cpp"
    break;

  case 132: // pattern-sfx: pattern
#line 806 "bang.y"
              { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[0].value.as < pattern_t > ()), parameter_constraint_modifier_t::none }; }
#line 3548 "bang.tab.cpp"
    break;

  case 133: // pattern-sfx: pattern "`...`"
#line 807 "bang.y"
                       { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ std::move(yystack_[1].value.as < pattern_t > ()), parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3554 "bang.tab.cpp"
    break;

  case 134: // pattern-sfx: "`...`"
#line 808 "bang.y"
               { yylhs.value.as < std::pair<pattern_t, parameter_constraint_modifier_t> > () = std::pair{ pattern_t{ .descriptor = placeholder{} }, parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3560 "bang.tab.cpp"
    break;

  case 135: // pattern: qname
#line 813 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3566 "bang.tab.cpp"
    break;

  case 136: // pattern: qname subpatterns concept-expression-list-opt
#line 815 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3572 "bang.tab.cpp"
    break;

  case 137: // pattern: "`_`" concept-expression-list-opt
#line 817 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3578 "bang.tab.cpp"
    break;

  case 138: // pattern: "`_`" subpatterns concept-expression-list-opt
#line 819 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3584 "bang.tab.cpp"
    break;

  case 139: // pattern: "`(`" syntax-expression "`)`" concept-expression-list-opt
#line 821 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[2].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3590 "bang.tab.cpp"
    break;

  case 140: // pattern: "`(`" syntax-expression "`)`" subpatterns concept-expression-list-opt
#line 823 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[3].value.as < syntax_expression_t > ()), .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3596 "bang.tab.cpp"
    break;

  case 141: // pattern: concept-expression-list
#line 825 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3602 "bang.tab.cpp"
    break;

  case 142: // concept-expression: "`@`" qname
#line 830 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_reference{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3608 "bang.tab.cpp"
    break;

  case 143: // concept-expression-list-opt: %empty
#line 835 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = {}; }
#line 3614 "bang.tab.cpp"
    break;

  case 144: // concept-expression-list-opt: concept-expression-list
#line 836 "bang.y"
      { yylhs.value.as < concept_expression_list_t > () = yystack_[0].value.as < concept_expression_list_t > (); }
#line 3620 "bang.tab.cpp"
    break;

  case 145: // concept-expression-list: concept-expression
#line 841 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = concept_expression_list_t{std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3626 "bang.tab.cpp"
    break;

  case 146: // concept-expression-list: concept-expression-list concept-expression
#line 843 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = std::move(yystack_[1].value.as < concept_expression_list_t > ()); yylhs.value.as < concept_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3632 "bang.tab.cpp"
    break;

  case 147: // syntax-expression: CONTEXT_IDENTIFIER
#line 851 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3638 "bang.tab.cpp"
    break;

  case 148: // syntax-expression: CT_IDENTIFIER
#line 853 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3644 "bang.tab.cpp"
    break;

  case 149: // syntax-expression: syntax-expression-wo-ii
#line 854 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3650 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: "nil"
#line 860 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3656 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: "true"
#line 862 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3662 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: "false"
#line 864 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3668 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: INTEGER
#line 866 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3674 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: DECIMAL
#line 868 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3680 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: DECIMAL_S
#line 870 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3686 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: INTEGER_INDEX
#line 872 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3692 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: STRING
#line 874 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3698 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 876 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3704 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: qname
#line 878 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3710 "bang.tab.cpp"
    break;

  case 160: // syntax-expression-wo-ii: "`(`" "`)`"
#line 880 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3716 "bang.tab.cpp"
    break;

  case 161: // syntax-expression-wo-ii: "`(`" "`:`" syntax-expression "`)`"
#line 882 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, named_expression_list_t{ named_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) } } }; }
#line 3722 "bang.tab.cpp"
    break;

  case 162: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 884 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                BOOST_ASSERT(!yystack_[1].value.as < named_expression_list_t > ().empty());
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3735 "bang.tab.cpp"
    break;

  case 163: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 893 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3747 "bang.tab.cpp"
    break;

  case 164: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 901 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3753 "bang.tab.cpp"
    break;

  case 165: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 903 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3759 "bang.tab.cpp"
    break;

  case 166: // syntax-expression-wo-ii: "`.`" identifier
#line 905 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3765 "bang.tab.cpp"
    break;

  case 167: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 909 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3771 "bang.tab.cpp"
    break;

  case 168: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 911 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3777 "bang.tab.cpp"
    break;

  case 169: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 913 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3783 "bang.tab.cpp"
    break;

  case 170: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 916 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3789 "bang.tab.cpp"
    break;

  case 171: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 918 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3795 "bang.tab.cpp"
    break;

  case 172: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 923 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3801 "bang.tab.cpp"
    break;

  case 173: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 925 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3807 "bang.tab.cpp"
    break;

  case 174: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 929 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3813 "bang.tab.cpp"
    break;

  case 175: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 931 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3819 "bang.tab.cpp"
    break;

  case 176: // syntax-expression-wo-ii: syntax-expression "`..`" syntax-expression
#line 933 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3825 "bang.tab.cpp"
    break;

  case 177: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3831 "bang.tab.cpp"
    break;

  case 178: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 940 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3837 "bang.tab.cpp"
    break;

  case 179: // syntax-expression-wo-ii: syntax-expression "`&&`" syntax-expression
#line 943 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3843 "bang.tab.cpp"
    break;

  case 180: // syntax-expression-wo-ii: syntax-expression "`||`" syntax-expression
#line 946 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3849 "bang.tab.cpp"
    break;

  case 181: // syntax-expression-wo-ii: apostrophe-expression
#line 948 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3855 "bang.tab.cpp"
    break;

  case 182: // syntax-expression-wo-ii: new-expression
#line 949 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3861 "bang.tab.cpp"
    break;

  case 183: // syntax-expression-wo-ii: compound-expression
#line 950 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3867 "bang.tab.cpp"
    break;

  case 184: // syntax-expression-wo-ii: lambda-expression
#line 951 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3873 "bang.tab.cpp"
    break;

  case 185: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 956 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3879 "bang.tab.cpp"
    break;

  case 186: // new-expression: "`new`" qname
#line 961 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3885 "bang.tab.cpp"
    break;

  case 187: // new-expression: "`new`" apostrophe-expression
#line 963 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3891 "bang.tab.cpp"
    break;

  case 188: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 965 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3897 "bang.tab.cpp"
    break;

  case 189: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 967 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3903 "bang.tab.cpp"
    break;

  case 190: // call-expression: qname "`(`" pack-expression-opt "`)`"
#line 972 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3909 "bang.tab.cpp"
    break;

  case 191: // call-expression: CONTEXT_IDENTIFIER "`(`" pack-expression-opt "`)`"
#line 974 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ ctx.make_qname(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())), true }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3915 "bang.tab.cpp"
    break;

  case 192: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 982 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3921 "bang.tab.cpp"
    break;

  case 193: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 984 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3927 "bang.tab.cpp"
    break;

  case 194: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 986 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3933 "bang.tab.cpp"
    break;

  case 195: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 991 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3939 "bang.tab.cpp"
    break;

  case 196: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 993 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3945 "bang.tab.cpp"
    break;

  case 197: // pack-expression-opt: %empty
#line 998 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{}; }
#line 3951 "bang.tab.cpp"
    break;

  case 198: // pack-expression-opt: pack-expression
#line 1000 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[0].value.as < named_expression_list_t > ()); }
#line 3957 "bang.tab.cpp"
    break;

  case 199: // pack-expression: syntax-expression
#line 1005 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{ named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3963 "bang.tab.cpp"
    break;

  case 200: // pack-expression: identifier "`:`" syntax-expression
#line 1014 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3973 "bang.tab.cpp"
    break;

  case 201: // pack-expression: pack-expression "," syntax-expression
#line 1020 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3982 "bang.tab.cpp"
    break;

  case 202: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 1025 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3991 "bang.tab.cpp"
    break;

  case 203: // compound-expression: syntax-expression "`...`"
#line 1033 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3997 "bang.tab.cpp"
    break;

  case 204: // compound-expression: call-expression
#line 1034 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 4003 "bang.tab.cpp"
    break;

  case 205: // type-expr: qname
#line 1076 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 4009 "bang.tab.cpp"
    break;

  case 206: // type-expr: call-expression
#line 1077 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 4015 "bang.tab.cpp"
    break;

  case 207: // type-expr: CONTEXT_IDENTIFIER
#line 1079 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 4021 "bang.tab.cpp"
    break;

  case 208: // type-expr: "`[`" type-expr "`]`"
#line 1083 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 4027 "bang.tab.cpp"
    break;

  case 209: // type-expr: "`(`" "`)`"
#line 1085 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 4033 "bang.tab.cpp"
    break;

  case 210: // type-expr: "`(`" pack-expression "`)`"
#line 1088 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 4039 "bang.tab.cpp"
    break;

  case 211: // type-expr: type-expr "`[`" syntax-expression "`]`"
#line 1090 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 4045 "bang.tab.cpp"
    break;

  case 212: // type-expr: type-expr "`|`" type-expr
#line 1092 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 4057 "bang.tab.cpp"
    break;

  case 213: // type-expr: type-expr "`->`" type-expr
#line 1100 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 4066 "bang.tab.cpp"
    break;


#line 4070 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -309;

  const signed char parser::yytable_ninf_ = -45;

  const short
  parser::yypact_[] =
  {
     680,  -309,  -309,    -1,  -309,  -309,  -309,  -309,  -309,  -309,
     801,    73,   780,   801,   801,    73,   801,   801,    -4,   -43,
     144,   801,   801,   801,    25,    82,    35,  -309,    25,    25,
    -309,  -309,  -309,    93,    11,   380,  -309,  -309,  -309,  -309,
     105,   213,   855,  -309,   117,  -309,   127,   131,     5,   801,
     135,   892,  -309,  -309,   801,  -309,   183,  1006,   173,    36,
    1006,    96,  -309,    33,    33,    73,  -309,   188,   223,  -309,
      73,    19,   220,   197,   485,   570,   670,   211,  -309,  -309,
    -309,   189,  -309,    78,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,    73,   801,   329,  -309,   224,   231,   236,  -309,
     801,   801,   801,   801,   801,   801,  -309,   801,   143,   801,
     801,   801,   801,   801,   801,   801,   271,   242,  -309,   907,
     801,  -309,   801,  -309,   801,  -309,  -309,   259,   801,   275,
     279,  -309,   801,   801,   580,   801,  -309,   801,   246,   801,
     329,    73,   284,   329,  -309,  -309,   296,  -309,   331,   331,
     401,   216,   344,   169,   306,   286,  -309,   259,   331,   329,
    -309,  1006,   350,   350,  1053,  1043,   309,   944,  -309,  -309,
      33,    33,   391,   791,   203,   204,   232,  -309,  -309,  1006,
     313,  1006,  1006,    -1,   701,   259,   105,   117,   127,   131,
      24,   242,   259,   329,    38,   314,   292,  -309,   801,  1006,
     317,  -309,  -309,   801,    25,   316,   480,  -309,   321,  -309,
    -309,   213,   670,   670,   236,  1006,   333,  -309,   332,   324,
    -309,    73,   346,  -309,   801,  -309,  -309,    25,    23,  -309,
     801,   462,   462,   462,   257,   331,   331,  -309,   363,  -309,
     400,  -309,  -309,  -309,   354,   331,   401,  -309,    18,   329,
     141,  -309,   359,  -309,  -309,  -309,  -309,   801,  -309,   254,
     120,   259,   801,   259,    24,   371,  -309,  -309,  -309,   801,
    1006,  -309,  1006,   365,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,   402,  -309,    73,   124,   375,   356,  -309,   196,
    1006,   224,   278,   400,  -309,   400,   959,  -309,  -309,  -309,
     400,  -309,  -309,  -309,  -309,   401,  -309,   331,   331,   259,
    -309,  -309,  -309,   383,  1006,  -309,  -309,  -309,   996,    -2,
      14,  -309,   329,   801,  -309,   801,   259,  -309,    73,   381,
    -309,   462,   327,   344,   327,   210,   261,  -309,  -309,   396,
    -309,    23,  -309,   331,   331,  -309,  -309,   263,   291,   801,
     259,  -309,   384,  1006,  1006,   185,  -309,    73,  -309,  -309,
     327,  -309,   462,   405,  -309,   278,  -309,   400,  -309,  -309,
    -309,  -309,  -309,  1006,    24,   236,   801,  -309,   386,  -309,
    -309,  -309,  -309,  -309,  1006,  -309
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   157,    51,   147,   158,   148,   156,   153,   154,   155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
     150,   151,   152,     0,     0,     0,     5,     7,    15,    54,
     159,     0,     0,   149,   181,   182,   204,   184,   183,   197,
       0,     0,   183,    53,     0,   160,    54,   199,     0,     0,
      73,     0,   166,   171,   170,     0,    42,    21,    17,    19,
       0,     0,   186,   187,     0,     0,     0,     0,    43,    13,
      57,     0,    39,     0,    16,     1,     2,    10,     3,     8,
       6,     9,     0,   197,    91,    59,    58,     0,     0,   169,
       0,     0,     0,     0,     0,     0,   203,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   198,   185,     0,
       0,   162,     0,   163,     0,   164,    20,     0,     0,     0,
       0,    12,    75,    75,    23,     0,    31,     0,    34,     0,
      91,    64,     0,    91,    37,    55,     0,    52,    97,    97,
       0,   110,    95,    97,     0,    92,    93,     0,    97,    91,
      36,    45,   174,   175,   179,   180,   176,     0,   167,   168,
     172,   173,   177,   178,     0,     0,     0,   191,   161,   200,
      54,   201,    74,   207,     0,     0,   205,     0,   206,     0,
      22,    18,     0,    91,    54,     0,    76,    77,     0,    80,
       0,    48,    47,    49,     0,    40,    41,    26,     0,    46,
      24,     0,     0,     0,     0,    71,     0,    68,     0,    65,
      66,    83,     0,   190,     0,   103,   105,     0,   143,   134,
       0,     0,     0,     0,   135,    97,    97,   128,   132,   145,
     141,   111,   112,    96,     0,    97,     0,   102,     0,     0,
     113,   109,     0,   165,   193,   192,   194,     0,   209,     0,
       0,     0,     0,     0,    11,     0,    81,    82,   188,     0,
      79,   189,    50,     0,    29,    27,    28,    25,    30,    32,
      33,    35,     0,    63,     0,     0,     0,    84,    85,     0,
      98,   142,   143,   143,   137,   144,     0,   131,   129,   130,
     143,   108,   101,   133,   146,     0,   104,    97,    97,     0,
     195,    94,   114,    60,   202,   210,   208,   213,     0,   212,
      60,    78,    91,     0,    67,     0,     0,    69,     0,     0,
      38,     0,   143,     0,   143,   143,     0,   126,   116,   118,
     138,   143,   136,    97,    97,   107,   100,     0,     0,     0,
       0,   211,     0,    72,    90,    87,    86,    83,   122,   127,
     143,   124,     0,   120,   115,   143,   119,   143,   139,   106,
      99,   196,    62,    14,    61,     0,     0,    89,     0,   125,
     123,   121,   117,   140,    88,    70
  };

  const short
  parser::yypgoto_[] =
  {
    -309,  -309,  -309,  -309,   406,  -309,  -309,  -309,  -309,  -309,
     -17,   -26,  -309,  -131,   238,    49,   -78,    17,     4,   404,
     366,  -309,  -309,  -309,   162,  -309,  -309,   439,   322,  -309,
     191,  -309,   106,  -309,  -309,   134,  -110,  -309,  -309,   -67,
     215,  -309,  -149,  -228,  -309,  -308,   100,  -227,  -204,   118,
    -233,  -220,    68,     0,  -309,    28,  -309,  -115,   -62,   377,
       3,    20,    31
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    33,    34,    35,    36,    66,    67,    68,   205,   206,
     144,    37,   208,    38,   210,    39,   153,    40,    50,   130,
      98,    82,   218,   219,   220,    84,    78,    59,   195,   196,
     197,   198,   286,   287,   377,   288,   154,   155,   244,   225,
     156,   157,   158,   293,   336,   337,   338,   236,   237,   238,
     239,   339,   295,    57,    43,    44,    45,    46,    47,   116,
     117,    52,   190
  };

  const short
  parser::yytable_[] =
  {
      42,   235,     2,   209,    41,   -44,   300,   304,   294,    91,
      51,    86,   188,    60,    60,    58,    63,    64,   261,   308,
      48,    74,    75,    76,   359,     2,   361,   297,   298,   299,
     216,     2,    70,   222,   348,    42,   350,    72,   309,    41,
     227,    77,   188,    99,   261,    81,    83,    49,    73,   252,
     262,    71,   379,   266,   119,    48,   -44,   136,    96,   138,
      53,    56,   304,   -44,    62,   189,    11,    69,   134,    87,
     188,   292,    11,   340,   243,   209,   262,   188,   344,     2,
     342,   160,   226,   265,   267,   107,   247,    79,    96,   123,
     263,   251,   108,    85,    65,   189,    95,   307,    56,   142,
     161,   162,   163,   164,   165,   166,   124,   167,   207,   170,
     171,   172,   173,   367,   126,   363,   174,   175,   176,   129,
     179,   368,   181,   189,   182,    92,   143,   358,   134,    80,
     189,   191,   199,   199,    42,   212,   169,   213,   211,   215,
     261,   145,    56,   152,   186,   325,   188,   383,   188,     2,
       2,   125,    92,    93,    48,   187,   343,   168,   380,    10,
      10,   261,    56,    56,    56,   113,   124,   234,   301,   302,
     326,   180,   262,   316,   186,   114,   312,    56,   306,   115,
     276,   194,   194,    94,   224,   187,   263,   259,   250,   152,
     217,    11,   152,   262,   188,   279,   280,   281,   270,   189,
     376,   189,   186,   272,   245,   261,    42,   263,   152,   186,
     211,   188,   352,   187,   335,   246,   260,   329,   240,     2,
     187,   273,   121,   264,   290,   362,    48,   227,    96,   120,
     296,   310,   139,    56,   127,   188,    92,   262,   128,   141,
     345,   346,   152,   122,   291,   133,   134,   189,   234,   234,
     234,   263,   254,   255,   335,   360,   335,   314,    92,   140,
      11,    94,   318,   234,   189,     2,   183,    92,   132,   199,
     285,    92,   330,   122,   122,    10,   369,   370,   186,   159,
     186,   256,   335,   261,     2,   147,   134,   335,   189,   187,
      95,   187,   317,   331,   319,   227,   332,     2,   152,   240,
     240,   240,   122,   315,    92,   292,    11,   184,   227,   228,
     364,   185,   122,   134,   240,   262,   241,   242,   194,    99,
     177,   192,   234,   353,   122,   354,   186,   193,   214,   263,
     371,   365,   221,   217,   147,     2,   147,   187,    11,   230,
     347,   334,   331,   186,   227,   223,   224,   148,   234,   373,
     333,   147,    26,    27,   187,   248,   249,   355,   330,   257,
      99,   107,   269,   268,   149,   234,   271,   186,   108,   109,
     110,   152,   278,   240,   274,   150,   384,   285,   187,   234,
      88,   374,   282,   283,   105,     1,     2,     3,     4,     5,
       6,     7,     8,     9,   284,   289,    10,   151,   303,   240,
     305,    99,   107,   348,   349,   350,   285,     2,   313,   108,
     109,   110,    92,   322,   334,   134,   240,   227,   227,   228,
     320,   101,   102,   323,   327,   105,   328,    11,    12,   357,
     240,   366,    13,   375,    14,   385,   229,   131,    89,    15,
     381,    90,    16,   107,   277,    97,   324,    17,    11,   230,
     108,   109,   110,    61,    18,   200,    19,    20,    21,    22,
     321,    23,   356,   378,   311,   382,   372,    24,     2,   151,
     146,     0,    25,    26,    27,    28,    29,     0,     0,   227,
     228,     0,    30,    31,    32,     1,     2,     3,     4,     5,
       6,     7,     8,     9,     0,    99,    10,   229,     0,     0,
     231,   232,   233,     0,     0,     0,     0,     0,     0,    11,
     230,     0,     0,     0,     0,   101,   102,   103,   104,   105,
     106,     0,     0,     0,     0,     0,     0,    11,    12,     0,
       0,     0,    13,     0,    14,   134,     0,   107,   275,    15,
       0,     0,    16,   135,   108,   109,   110,    17,     0,     0,
     111,   112,     0,     0,    18,     0,     0,    20,    21,    22,
       0,    23,     0,   201,   202,   203,     0,    24,     0,     0,
       0,     0,     0,    26,    27,    28,   204,     0,     0,     0,
      99,     0,    30,    31,    32,     1,     2,     3,     4,     5,
       6,     7,     8,     9,     0,     0,    10,     0,     0,     0,
     101,   102,   103,   104,   105,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,    11,    12,   108,
     109,   110,    13,     0,    14,   111,   112,     0,     0,    15,
       0,     0,    16,     0,     0,     0,     0,    17,     0,     0,
     137,     0,     0,     0,    18,     0,     0,    20,    21,    22,
       0,    23,     0,   201,   202,   203,     0,    24,     0,     0,
       0,     0,     0,    26,    27,    28,   204,     0,     0,     0,
      99,     0,    30,    31,    32,     1,     2,     3,     4,     5,
       6,     7,     8,     9,     0,     0,    10,     0,     0,     0,
     101,   102,   103,   104,   105,   106,     1,     2,     3,     4,
       5,     6,     7,     8,     9,     0,     0,    10,     0,     0,
     134,     0,   107,     0,     0,     0,     0,    11,    12,   108,
     109,   110,    13,     0,    14,   111,   112,     0,     0,    15,
       0,     0,    16,     0,     0,     0,     0,    17,    11,    12,
     258,     0,     0,    13,    18,    14,    19,    20,    21,    22,
      15,    23,     0,    16,     0,     0,     0,    24,    17,     0,
       0,     0,    25,    26,    27,    28,    29,     0,    20,     0,
       0,     0,    30,    31,    32,     1,     2,     3,     4,     5,
       6,     7,     8,     9,    26,    27,    10,     0,     0,     0,
       0,    99,     0,    30,    31,    32,     1,     2,     3,     4,
       5,     6,     7,     8,     9,     0,     0,    10,     0,     0,
       0,   101,   102,     0,     0,   105,    54,    11,    12,    55,
       0,     0,    13,     0,    14,     0,     0,     0,     0,    15,
       0,     0,    16,   107,     0,     0,     0,    17,    11,    12,
     108,   109,   110,    13,     0,    14,   111,    20,     0,     0,
      15,     0,     0,    16,     0,    99,     0,     0,    17,     0,
     100,     0,     0,    26,    27,     0,     0,     0,    20,     0,
       0,     0,    30,    31,    32,   101,   102,   103,   104,   105,
     106,     0,     0,     0,    26,    27,     0,     0,     0,     0,
       0,     0,    99,    30,    31,    32,     0,   107,   118,     0,
       0,     0,     0,     0,   108,   109,   110,    99,     0,     0,
     111,   112,   101,   102,   103,   104,   105,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   101,   102,   103,
     104,   105,   106,     0,   107,     0,     0,     0,     0,     0,
       0,   108,   109,   110,    99,     0,   178,   111,   112,   107,
       0,     0,     0,     0,     0,     0,   108,   109,   110,    99,
       0,     0,   111,   112,   101,   102,   103,   104,   105,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   101,
     102,   103,   104,   105,   106,     0,   107,   253,     0,     0,
       0,     0,     0,   108,   109,   110,    99,     0,   341,   111,
     112,   107,     0,     0,     0,     0,    99,     0,   108,   109,
     110,     0,     0,     0,   111,   112,   101,   102,   103,   104,
     105,   106,     0,     0,     0,     0,   101,   102,   103,   104,
     105,   106,     0,     0,     0,     0,     0,     0,   107,   351,
       0,     0,     0,    99,     0,   108,   109,   110,   107,     0,
       0,   111,   112,    99,     0,   108,   109,   110,     0,     0,
       0,   111,   112,   101,   102,   103,     0,   105,     0,     0,
       0,     0,     0,   101,   102,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,   107,     0,     0,     0,     0,
       0,     0,   108,   109,   110,   107,     0,     0,   111,   112,
       0,     0,   108,   109,   110,     0,     0,     0,   111,   112
  };

  const short
  parser::yycheck_[] =
  {
       0,   150,     6,   134,     0,     0,   234,   240,   228,    35,
      10,     0,   127,    13,    14,    12,    16,    17,    20,   246,
       0,    21,    22,    23,   332,     6,   334,   231,   232,   233,
     140,     6,    75,   143,    20,    35,    22,    20,    20,    35,
      17,    24,   157,    10,    20,    28,    29,    48,    20,   159,
      52,    94,   360,    15,    54,    35,    51,    74,    41,    76,
      11,    12,   295,    58,    15,   127,    47,    18,    50,    58,
     185,    48,    47,   293,   152,   206,    52,   192,   305,     6,
     300,    98,   149,   193,    46,    52,   153,     5,    71,    53,
      66,   158,    59,     0,    98,   157,    77,   246,    49,    21,
     100,   101,   102,   103,   104,   105,    70,   107,   134,   109,
     110,   111,   112,   341,    65,   335,   113,   114,   115,    70,
     120,   341,   122,   185,   124,    47,    48,   331,    50,    94,
     192,   128,   132,   133,   134,   135,   108,   137,   134,   139,
      20,    92,    93,    94,   127,    21,   261,   367,   263,     6,
       6,    55,    47,    48,   134,   127,   305,   108,   362,    16,
      16,    20,   113,   114,   115,    48,    70,   150,   235,   236,
      46,   122,    52,    53,   157,    48,    35,   128,   245,    48,
     206,   132,   133,    48,    15,   157,    66,   184,   157,   140,
     141,    47,   143,    52,   309,   212,   213,   214,   198,   261,
      15,   263,   185,   203,    35,    20,   206,    66,   159,   192,
     206,   326,   322,   185,   292,    46,   185,    21,   150,     6,
     192,   204,    49,   192,   224,    15,   206,    17,   211,    46,
     230,   248,    21,   184,    46,   350,    47,    52,    15,    50,
     307,   308,   193,    70,   227,    48,    50,   309,   231,   232,
     233,    66,    49,    49,   332,   333,   334,   257,    47,    48,
      47,    48,   262,   246,   326,     6,     7,    47,    48,   269,
     221,    47,   289,    70,    70,    16,   343,   344,   261,    48,
     263,    49,   360,    20,     6,     7,    50,   365,   350,   261,
      77,   263,   261,    15,   263,    17,    18,     6,   249,   231,
     232,   233,    70,    49,    47,    48,    47,    48,    17,    18,
      49,    52,    70,    50,   246,    52,   100,   101,   269,    10,
      49,    46,   305,   323,    70,   325,   309,    48,    82,    66,
     347,    70,    48,   284,     7,     6,     7,   309,    47,    48,
     309,   292,    15,   326,    17,    49,    15,    18,   331,   349,
      72,     7,    93,    94,   326,    49,    70,   326,   375,    46,
      10,    52,    70,    49,    35,   348,    49,   350,    59,    60,
      61,   322,    51,   305,    58,    46,   376,   328,   350,   362,
       0,   350,    49,    51,    34,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    70,    49,    16,    68,    35,   331,
      46,    10,    52,    20,    21,    22,   357,     6,    49,    59,
      60,    61,    47,    48,   365,    50,   348,    17,    17,    18,
      49,    30,    31,    21,    49,    34,    70,    47,    48,    48,
     362,    35,    52,    49,    54,    49,    35,    71,    58,    59,
      35,    35,    62,    52,   206,    41,   284,    67,    47,    48,
      59,    60,    61,    14,    74,   133,    76,    77,    78,    79,
     269,    81,   328,   357,   249,   365,   348,    87,     6,    68,
      93,    -1,    92,    93,    94,    95,    96,    -1,    -1,    17,
      18,    -1,   102,   103,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    -1,    10,    16,    35,    -1,    -1,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      48,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    -1,
      -1,    -1,    52,    -1,    54,    50,    -1,    52,    58,    59,
      -1,    -1,    62,    58,    59,    60,    61,    67,    -1,    -1,
      65,    66,    -1,    -1,    74,    -1,    -1,    77,    78,    79,
      -1,    81,    -1,    83,    84,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      10,    -1,   102,   103,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    47,    48,    59,
      60,    61,    52,    -1,    54,    65,    66,    -1,    -1,    59,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      80,    -1,    -1,    -1,    74,    -1,    -1,    77,    78,    79,
      -1,    81,    -1,    83,    84,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      10,    -1,   102,   103,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    47,    48,    59,
      60,    61,    52,    -1,    54,    65,    66,    -1,    -1,    59,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    67,    47,    48,
      49,    -1,    -1,    52,    74,    54,    76,    77,    78,    79,
      59,    81,    -1,    62,    -1,    -1,    -1,    87,    67,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    -1,    77,    -1,
      -1,    -1,   102,   103,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    93,    94,    16,    -1,    -1,    -1,
      -1,    10,    -1,   102,   103,   104,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,
      -1,    30,    31,    -1,    -1,    34,    46,    47,    48,    49,
      -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    62,    52,    -1,    -1,    -1,    67,    47,    48,
      59,    60,    61,    52,    -1,    54,    65,    77,    -1,    -1,
      59,    -1,    -1,    62,    -1,    10,    -1,    -1,    67,    -1,
      15,    -1,    -1,    93,    94,    -1,    -1,    -1,    77,    -1,
      -1,    -1,   102,   103,   104,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    10,   102,   103,   104,    -1,    52,    16,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    10,    -1,    -1,
      65,    66,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    10,    -1,    49,    65,    66,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,    10,
      -1,    -1,    65,    66,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    -1,    52,    53,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    10,    -1,    49,    65,
      66,    52,    -1,    -1,    -1,    -1,    10,    -1,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    52,    53,
      -1,    -1,    -1,    10,    -1,    59,    60,    61,    52,    -1,
      -1,    65,    66,    10,    -1,    59,    60,    61,    -1,    -1,
      -1,    65,    66,    30,    31,    32,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    30,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,    52,    -1,    -1,    65,    66,
      -1,    -1,    59,    60,    61,    -1,    -1,    -1,    65,    66
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      16,    47,    48,    52,    54,    59,    62,    67,    74,    76,
      77,    78,    79,    81,    87,    92,    93,    94,    95,    96,
     102,   103,   104,   106,   107,   108,   109,   116,   118,   120,
     122,   123,   158,   159,   160,   161,   162,   163,   166,    48,
     123,   158,   166,   120,    46,    49,   120,   158,   165,   132,
     158,   132,   120,   158,   158,    98,   110,   111,   112,   120,
      75,    94,   122,   160,   158,   158,   158,   122,   131,     5,
      94,   122,   126,   122,   130,     0,     0,    58,     0,    58,
     109,   116,    47,    48,    48,    77,   122,   124,   125,    10,
      15,    30,    31,    32,    33,    34,    35,    52,    59,    60,
      61,    65,    66,    48,    48,    48,   164,   165,    16,   158,
      46,    49,    70,    53,    70,    55,   120,    46,    15,   120,
     124,   125,    48,    48,    50,    58,   115,    80,   115,    21,
      48,    50,    21,    48,   115,   120,   164,     7,    18,    35,
      46,    68,   120,   121,   141,   142,   145,   146,   147,    48,
     115,   158,   158,   158,   158,   158,   158,   158,   120,   160,
     158,   158,   158,   158,   165,   165,   165,    49,    49,   158,
     120,   158,   158,     7,    48,    52,   122,   160,   162,   163,
     167,   165,    46,    48,   120,   133,   134,   135,   136,   158,
     133,    83,    84,    85,    96,   113,   114,   116,   117,   118,
     119,   123,   158,   158,    82,   158,   141,   120,   127,   128,
     129,    48,   141,    49,    15,   144,   144,    17,    18,    35,
      48,    99,   100,   101,   122,   147,   152,   153,   154,   155,
     157,   100,   101,   121,   143,    35,    46,   144,    49,    70,
     167,   144,   141,    53,    49,    49,    49,    46,    49,   165,
     167,    20,    52,    66,   167,   141,    15,    46,    49,    70,
     158,    49,   158,   122,    58,    58,   116,   119,    51,   115,
     115,   115,    49,    51,    70,   120,   137,   138,   140,    49,
     158,   122,    48,   148,   156,   157,   158,   153,   153,   153,
     148,   144,   144,    35,   155,    46,   144,   147,   152,    20,
     115,   145,    35,    49,   158,    49,    53,   167,   158,   167,
      49,   135,    48,    21,   129,    21,    46,    49,    70,    21,
     115,    15,    18,    72,   120,   121,   149,   150,   151,   156,
     156,    49,   156,   147,   152,   144,   144,   167,    20,    21,
      22,    53,   141,   158,   158,   167,   140,    48,   153,   150,
     121,   150,    15,   156,    49,    70,    35,   148,   156,   144,
     144,   115,   154,   158,   167,    49,    15,   139,   137,   150,
     153,    35,   151,   156,   158,    49
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
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     146,   146,   146,   147,   147,   148,   149,   149,   150,   150,
     150,   150,   150,   150,   151,   151,   151,   151,   152,   152,
     152,   152,   153,   153,   153,   154,   154,   154,   154,   154,
     154,   154,   155,   156,   156,   157,   157,   158,   158,   158,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   160,   161,   161,   161,   161,
     162,   162,   162,   162,   162,   163,   163,   164,   164,   165,
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
       4,     3,     2,     2,     3,     2,     5,     4,     3,     2,
       1,     2,     2,     2,     3,     3,     1,     3,     1,     2,
       2,     3,     2,     3,     2,     3,     1,     2,     1,     2,
       2,     2,     1,     2,     1,     1,     3,     2,     3,     4,
       5,     1,     2,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     3,     3,     3,     4,     2,     3,     3,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     2,     2,     5,     5,
       4,     4,     4,     4,     4,     5,     7,     0,     1,     1,
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
  "constraint-expression-mod", "constraint-expression", "subpatterns",
  "pattern-list", "pattern-field-sfx", "pattern-field", "pattern-mod",
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
       0,   333,   333,   334,   338,   340,   342,   359,   361,   362,
     364,   369,   371,   373,   375,   384,   386,   391,   393,   398,
     400,   405,   406,   411,   413,   415,   420,   422,   423,   425,
     430,   435,   437,   439,   441,   443,   445,   447,   449,   451,
     456,   458,   463,   465,   467,   469,   474,   476,   478,   480,
     482,   487,   495,   510,   512,   514,   520,   522,   527,   528,
     532,   534,   536,   542,   547,   548,   552,   554,   559,   565,
     567,   590,   597,   615,   617,   623,   624,   628,   630,   635,
     637,   642,   644,   650,   651,   655,   657,   662,   663,   667,
     669,   676,   677,   681,   683,   688,   689,   693,   694,   698,
     700,   702,   705,   707,   709,   711,   716,   718,   720,   722,
     727,   728,   729,   733,   735,   741,   746,   748,   754,   756,
     759,   761,   765,   768,   774,   778,   791,   794,   799,   800,
     801,   802,   806,   807,   808,   812,   814,   816,   818,   820,
     822,   824,   829,   834,   836,   840,   842,   850,   852,   854,
     859,   861,   863,   865,   867,   869,   871,   873,   875,   877,
     879,   881,   883,   892,   900,   902,   904,   908,   910,   912,
     915,   917,   922,   924,   928,   930,   932,   935,   939,   942,
     945,   948,   949,   950,   951,   955,   960,   962,   964,   966,
     971,   973,   981,   983,   985,   990,   992,   997,   999,  1004,
    1013,  1019,  1024,  1032,  1034,  1075,  1077,  1078,  1082,  1084,
    1087,  1089,  1091,  1099
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
#line 5034 "bang.tab.cpp"

#line 1426 "bang.y"

