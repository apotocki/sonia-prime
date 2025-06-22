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

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
        value.copy< constraint_expression_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_160_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_148_pattern_list: // pattern-list
        value.copy< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
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
      case symbol_kind::S_MUT: // "mut modifier"
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
      case symbol_kind::S_154_syntax_expression: // syntax-expression
      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_157_new_expression: // new-expression
      case symbol_kind::S_158_call_expression: // call-expression
      case symbol_kind::S_159_lambda_expression: // lambda-expression
      case symbol_kind::S_161_compound_expression: // compound-expression
      case symbol_kind::S_162_type_expr: // type-expr
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

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
        value.move< constraint_expression_t > (YY_MOVE (s.value));
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
      case symbol_kind::S_160_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_148_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
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
      case symbol_kind::S_MUT: // "mut modifier"
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
      case symbol_kind::S_154_syntax_expression: // syntax-expression
      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_157_new_expression: // new-expression
      case symbol_kind::S_158_call_expression: // call-expression
      case symbol_kind::S_159_lambda_expression: // lambda-expression
      case symbol_kind::S_161_compound_expression: // compound-expression
      case symbol_kind::S_162_type_expr: // type-expr
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

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
        value.YY_MOVE_OR_COPY< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
        value.YY_MOVE_OR_COPY< constraint_expression_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_160_pack_expression: // pack-expression
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_148_pattern_list: // pattern-list
        value.YY_MOVE_OR_COPY< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.YY_MOVE_OR_COPY< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
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
      case symbol_kind::S_MUT: // "mut modifier"
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
      case symbol_kind::S_154_syntax_expression: // syntax-expression
      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_157_new_expression: // new-expression
      case symbol_kind::S_158_call_expression: // call-expression
      case symbol_kind::S_159_lambda_expression: // lambda-expression
      case symbol_kind::S_161_compound_expression: // compound-expression
      case symbol_kind::S_162_type_expr: // type-expr
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

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
        value.move< constraint_expression_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_160_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_148_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
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
      case symbol_kind::S_MUT: // "mut modifier"
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
      case symbol_kind::S_154_syntax_expression: // syntax-expression
      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_157_new_expression: // new-expression
      case symbol_kind::S_158_call_expression: // call-expression
      case symbol_kind::S_159_lambda_expression: // lambda-expression
      case symbol_kind::S_161_compound_expression: // compound-expression
      case symbol_kind::S_162_type_expr: // type-expr
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

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
        value.copy< constraint_expression_t > (that.value);
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
      case symbol_kind::S_160_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_148_pattern_list: // pattern-list
        value.copy< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (that.value);
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
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
      case symbol_kind::S_MUT: // "mut modifier"
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
      case symbol_kind::S_154_syntax_expression: // syntax-expression
      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_157_new_expression: // new-expression
      case symbol_kind::S_158_call_expression: // call-expression
      case symbol_kind::S_159_lambda_expression: // lambda-expression
      case symbol_kind::S_161_compound_expression: // compound-expression
      case symbol_kind::S_162_type_expr: // type-expr
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

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
        value.move< constraint_expression_t > (that.value);
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
      case symbol_kind::S_160_pack_expression: // pack-expression
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_148_pattern_list: // pattern-list
        value.move< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (that.value);
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
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
      case symbol_kind::S_MUT: // "mut modifier"
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
      case symbol_kind::S_154_syntax_expression: // syntax-expression
      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_157_new_expression: // new-expression
      case symbol_kind::S_158_call_expression: // call-expression
      case symbol_kind::S_159_lambda_expression: // lambda-expression
      case symbol_kind::S_161_compound_expression: // compound-expression
      case symbol_kind::S_162_type_expr: // type-expr
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
#line 325 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 325 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 325 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 325 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 325 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 325 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 325 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 325 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 325 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 325 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 325 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 325 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 325 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 325 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 325 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 325 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 325 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 325 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 325 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 325 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 325 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 325 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 325 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 325 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 325 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 325 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 325 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 325 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 325 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 325 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_MUT: // "mut modifier"
#line 325 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 325 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 325 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 325 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 325 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
#line 325 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 325 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl: // let-decl
#line 325 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start: // let-decl-start
#line 325 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 325 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_braced_statements: // braced-statements
#line 325 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
#line 325 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
#line 325 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
#line 325 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_infunction_statement: // infunction-statement
#line 325 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 325 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_internal_identifier: // internal-identifier
#line 325 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 325 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
#line 325 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_name: // fn-name
#line 325 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
#line 325 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
#line 325 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
#line 325 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_list: // case-list
#line 325 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_case_decl: // case-decl
#line 325 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
#line 325 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_using_decl: // using-decl
#line 325 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_expression_list: // expression-list
#line 325 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
#line 325 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_argument_list: // argument-list
#line 325 "bang.y"
                 { }
#line 2010 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 325 "bang.y"
                 { }
#line 2016 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_argument_name: // argument-name
#line 325 "bang.y"
                 { }
#line 2022 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
#line 325 "bang.y"
                 { }
#line 2028 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_list: // field-list
#line 325 "bang.y"
                 { }
#line 2034 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
#line 325 "bang.y"
                 { }
#line 2040 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 325 "bang.y"
                 { }
#line 2046 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
#line 325 "bang.y"
                 { }
#line 2052 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_list: // parameter-list
#line 325 "bang.y"
                 { }
#line 2058 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
#line 325 "bang.y"
                 { }
#line 2064 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
#line 325 "bang.y"
                 { }
#line 2070 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
#line 325 "bang.y"
                 { }
#line 2076 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
#line 325 "bang.y"
                 { }
#line 2082 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
#line 325 "bang.y"
                 { }
#line 2088 "bang.tab.cpp"
        break;

      case symbol_kind::S_subpatterns: // subpatterns
#line 325 "bang.y"
                 { }
#line 2094 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_pattern_list: // pattern-list
#line 325 "bang.y"
                 { }
#line 2100 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
#line 325 "bang.y"
                 { }
#line 2106 "bang.tab.cpp"
        break;

      case symbol_kind::S_pattern: // pattern
#line 325 "bang.y"
                 { }
#line 2112 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
#line 325 "bang.y"
                 { }
#line 2118 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
#line 325 "bang.y"
                 { }
#line 2124 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
#line 325 "bang.y"
                 { }
#line 2130 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_syntax_expression: // syntax-expression
#line 325 "bang.y"
                 { }
#line 2136 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 325 "bang.y"
                 { }
#line 2142 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
#line 325 "bang.y"
                 { }
#line 2148 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_new_expression: // new-expression
#line 325 "bang.y"
                 { }
#line 2154 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_call_expression: // call-expression
#line 325 "bang.y"
                 { }
#line 2160 "bang.tab.cpp"
        break;

      case symbol_kind::S_159_lambda_expression: // lambda-expression
#line 325 "bang.y"
                 { }
#line 2166 "bang.tab.cpp"
        break;

      case symbol_kind::S_160_pack_expression: // pack-expression
#line 325 "bang.y"
                 { }
#line 2172 "bang.tab.cpp"
        break;

      case symbol_kind::S_161_compound_expression: // compound-expression
#line 325 "bang.y"
                 { }
#line 2178 "bang.tab.cpp"
        break;

      case symbol_kind::S_162_type_expr: // type-expr
#line 325 "bang.y"
                 { }
#line 2184 "bang.tab.cpp"
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

      case symbol_kind::S_152_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_153_concept_expression_list: // concept-expression-list
        yylhs.value.emplace< concept_expression_list_t > ();
        break;

      case symbol_kind::S_145_constraint_expression: // constraint-expression
        yylhs.value.emplace< constraint_expression_t > ();
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
      case symbol_kind::S_160_pack_expression: // pack-expression
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_146_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_148_pattern_list: // pattern-list
        yylhs.value.emplace< pattern_list_t > ();
        break;

      case symbol_kind::S_pattern: // pattern
        yylhs.value.emplace< pattern_t > ();
        break;

      case symbol_kind::S_149_pattern_field: // pattern-field
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
      case symbol_kind::S_MUT: // "mut modifier"
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_151_concept_expression: // concept-expression
      case symbol_kind::S_154_syntax_expression: // syntax-expression
      case symbol_kind::S_155_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_156_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_157_new_expression: // new-expression
      case symbol_kind::S_158_call_expression: // call-expression
      case symbol_kind::S_159_lambda_expression: // lambda-expression
      case symbol_kind::S_161_compound_expression: // compound-expression
      case symbol_kind::S_162_type_expr: // type-expr
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
#line 330 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2636 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 331 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2642 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 336 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2648 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 338 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2654 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 340 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2660 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 357 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2666 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 358 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2672 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 360 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2678 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 362 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2684 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 367 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), &ctx.push(std::move(yystack_[0].value.as < syntax_expression_t > ())) }; }
#line 2690 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 369 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2696 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 371 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2702 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-name "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 373 "bang.y"
        { 
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) });
            yylhs.value.as < statement > () = fn_decl_t{ fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = nullptr, .kind = yystack_[6].value.as < fn_kind > () }, ctx.push(std::move(sts)) };
            IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
            //     $$ = fn_decl_t{ fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .result = std::move($value), .is_type_expression_result = false, .kind = $fnkind } }; IGNORE_TERM($OPEN_PARENTHESIS); }
        }
#line 2715 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 382 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2721 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 384 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2727 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 389 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2733 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 391 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2739 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: identifier
#line 396 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2745 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: "weak modifier" identifier
#line 398 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2751 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 402 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2757 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 404 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2763 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 409 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2769 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 411 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2775 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 413 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2781 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 418 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2787 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 419 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2793 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 421 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2799 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 423 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2805 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 428 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2811 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 433 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2817 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 435 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2823 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 437 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2829 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 439 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2835 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 441 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2841 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 443 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2847 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 445 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2853 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 447 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2859 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 449 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2865 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 454 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2871 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 456 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2877 "bang.tab.cpp"
    break;

  case 42: // generic-statement: LET let-decl
#line 461 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2883 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 463 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2889 "bang.tab.cpp"
    break;

  case 44: // generic-statement: compound-expression
#line 465 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2895 "bang.tab.cpp"
    break;

  case 45: // generic-statement: syntax-expression "`=`" syntax-expression
#line 467 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2901 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: generic-statement
#line 472 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2907 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: "`break`"
#line 474 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2913 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`continue`"
#line 476 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2919 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`return`"
#line 478 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .location = std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2925 "bang.tab.cpp"
    break;

  case 50: // infunction-statement: "`return`" syntax-expression
#line 480 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2931 "bang.tab.cpp"
    break;

  case 51: // identifier: IDENTIFIER
#line 485 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2937 "bang.tab.cpp"
    break;

  case 52: // internal-identifier: CONTEXT_IDENTIFIER
#line 493 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2943 "bang.tab.cpp"
    break;

  case 53: // qname: "`::`" identifier
#line 508 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2949 "bang.tab.cpp"
    break;

  case 54: // qname: identifier
#line 510 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2955 "bang.tab.cpp"
    break;

  case 55: // qname: qname "`::`" identifier
#line 512 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2961 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: "`fn`"
#line 518 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2967 "bang.tab.cpp"
    break;

  case 57: // fn-start-decl: INLINE "`fn`"
#line 520 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2973 "bang.tab.cpp"
    break;

  case 58: // fn-name: qname
#line 524 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2979 "bang.tab.cpp"
    break;

  case 59: // fn-name: "`new`"
#line 525 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2985 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 530 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2991 "bang.tab.cpp"
    break;

  case 61: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`~>`" type-expr
#line 532 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2997 "bang.tab.cpp"
    break;

  case 62: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" pattern
#line 534 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3003 "bang.tab.cpp"
    break;

  case 63: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 540 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3009 "bang.tab.cpp"
    break;

  case 64: // case-list-opt: %empty
#line 544 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 3015 "bang.tab.cpp"
    break;

  case 65: // case-list-opt: case-list
#line 545 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 3021 "bang.tab.cpp"
    break;

  case 66: // case-list: case-decl
#line 550 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 3027 "bang.tab.cpp"
    break;

  case 67: // case-list: case-list "," case-decl
#line 552 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 3033 "bang.tab.cpp"
    break;

  case 68: // case-decl: identifier
#line 557 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 3039 "bang.tab.cpp"
    break;

  case 69: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 563 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3045 "bang.tab.cpp"
    break;

  case 70: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 565 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3051 "bang.tab.cpp"
    break;

  case 71: // using-decl: qname "`=>`" syntax-expression
#line 588 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 3059 "bang.tab.cpp"
    break;

  case 72: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 592 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3067 "bang.tab.cpp"
    break;

  case 73: // expression-list: syntax-expression
#line 606 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3073 "bang.tab.cpp"
    break;

  case 74: // expression-list: expression-list "," syntax-expression
#line 608 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3079 "bang.tab.cpp"
    break;

  case 75: // argument-list-opt: %empty
#line 613 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 3085 "bang.tab.cpp"
    break;

  case 76: // argument-list-opt: argument-list
#line 614 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 3091 "bang.tab.cpp"
    break;

  case 77: // argument-list: argument
#line 619 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 3097 "bang.tab.cpp"
    break;

  case 78: // argument-list: argument-list "," argument
#line 621 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 3103 "bang.tab.cpp"
    break;

  case 79: // argument: argument-name syntax-expression
#line 626 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3109 "bang.tab.cpp"
    break;

  case 80: // argument: syntax-expression
#line 628 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3115 "bang.tab.cpp"
    break;

  case 81: // argument-name: identifier "`=`"
#line 633 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3121 "bang.tab.cpp"
    break;

  case 82: // argument-name: identifier "`:`"
#line 635 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 3127 "bang.tab.cpp"
    break;

  case 83: // field-list-opt: %empty
#line 640 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 3133 "bang.tab.cpp"
    break;

  case 84: // field-list-opt: field-list
#line 641 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 3139 "bang.tab.cpp"
    break;

  case 85: // field-list: field
#line 646 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 3145 "bang.tab.cpp"
    break;

  case 86: // field-list: field-list "," field
#line 648 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 3151 "bang.tab.cpp"
    break;

  case 87: // field-default-value-opt: %empty
#line 652 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3157 "bang.tab.cpp"
    break;

  case 88: // field-default-value-opt: "`=`" syntax-expression
#line 653 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3163 "bang.tab.cpp"
    break;

  case 89: // field: identifier "`:`" type-expr field-default-value-opt
#line 658 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3169 "bang.tab.cpp"
    break;

  case 90: // field: identifier "`=>`" type-expr field-default-value-opt
#line 660 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3175 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 670 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3181 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 671 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3187 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 676 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3193 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 678 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3199 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 682 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3205 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 683 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3211 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 687 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3217 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 688 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3223 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" pattern parameter-default-value-opt
#line 693 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3229 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: internal-identifier "`:`" pattern parameter-default-value-opt
#line 695 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3235 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: pattern parameter-default-value-opt
#line 697 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{}, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3241 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`:`" constraint-expression parameter-default-value-opt
#line 699 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3247 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: internal-identifier "`:`" constraint-expression parameter-default-value-opt
#line 701 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3253 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: constraint-expression parameter-default-value-opt
#line 703 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3259 "bang.tab.cpp"
    break;

  case 105: // constraint-expression: "`~`" type-expr
#line 708 "bang.y"
        { yylhs.value.as < constraint_expression_t > () = constraint_expression_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3265 "bang.tab.cpp"
    break;

  case 106: // constraint-expression: "`~`" type-expr "`...`"
#line 710 "bang.y"
        { yylhs.value.as < constraint_expression_t > () = constraint_expression_t{ .expression = std::move(yystack_[1].value.as < syntax_expression_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3271 "bang.tab.cpp"
    break;

  case 107: // subpatterns-opt: %empty
#line 834 "bang.y"
      { yylhs.value.as < pattern_list_t > () = pattern_list_t{}; }
#line 3277 "bang.tab.cpp"
    break;

  case 108: // subpatterns-opt: subpatterns
#line 835 "bang.y"
      { yylhs.value.as < pattern_list_t > () = yystack_[0].value.as < pattern_list_t > (); }
#line 3283 "bang.tab.cpp"
    break;

  case 109: // subpatterns: "`(`" pattern-list "`)`"
#line 840 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[1].value.as < pattern_list_t > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3289 "bang.tab.cpp"
    break;

  case 110: // pattern-list: pattern-field
#line 845 "bang.y"
        { yylhs.value.as < pattern_list_t > () = pattern_list_t{ std::move(yystack_[0].value.as < pattern_t::field > ()) }; }
#line 3295 "bang.tab.cpp"
    break;

  case 111: // pattern-list: pattern-list "," pattern-field
#line 847 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[2].value.as < pattern_list_t > ()); yylhs.value.as < pattern_list_t > ().emplace_back(std::move(yystack_[0].value.as < pattern_t::field > ())); }
#line 3301 "bang.tab.cpp"
    break;

  case 112: // pattern-field: pattern
#line 852 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3307 "bang.tab.cpp"
    break;

  case 113: // pattern-field: "`_`" "`:`" pattern
#line 854 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3313 "bang.tab.cpp"
    break;

  case 114: // pattern-field: identifier "`:`" pattern
#line 856 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3319 "bang.tab.cpp"
    break;

  case 115: // pattern-field: internal-identifier "`:`" pattern
#line 858 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3325 "bang.tab.cpp"
    break;

  case 116: // pattern-field: "`(`" syntax-expression "`)`" "`:`" pattern
#line 860 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[3].value.as < syntax_expression_t > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3331 "bang.tab.cpp"
    break;

  case 117: // pattern: qname subpatterns-opt concept-expression-list-opt
#line 865 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3337 "bang.tab.cpp"
    break;

  case 118: // pattern: qname subpatterns-opt "`...`" concept-expression-list-opt
#line 867 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[2].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3343 "bang.tab.cpp"
    break;

  case 119: // pattern: internal-identifier concept-expression-list-opt
#line 869 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3349 "bang.tab.cpp"
    break;

  case 120: // pattern: internal-identifier "`...`" concept-expression-list-opt
#line 871 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3355 "bang.tab.cpp"
    break;

  case 121: // pattern: "`_`" subpatterns concept-expression-list-opt
#line 873 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3361 "bang.tab.cpp"
    break;

  case 122: // pattern: "`(`" syntax-expression "`)`" concept-expression-list-opt
#line 875 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[2].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3367 "bang.tab.cpp"
    break;

  case 123: // pattern: "`(`" syntax-expression "`)`" "`...`" concept-expression-list-opt
#line 877 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[3].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3373 "bang.tab.cpp"
    break;

  case 124: // pattern: "`_`" concept-expression-list-opt
#line 879 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3379 "bang.tab.cpp"
    break;

  case 125: // pattern: concept-expression-list
#line 881 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3385 "bang.tab.cpp"
    break;

  case 126: // pattern: "`_`" "`...`" concept-expression-list-opt
#line 883 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3391 "bang.tab.cpp"
    break;

  case 127: // pattern: "`...`" concept-expression-list-opt
#line 885 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3397 "bang.tab.cpp"
    break;

  case 128: // concept-expression: "`@`" qname
#line 890 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_reference{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3403 "bang.tab.cpp"
    break;

  case 129: // concept-expression-list-opt: %empty
#line 895 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = {}; }
#line 3409 "bang.tab.cpp"
    break;

  case 130: // concept-expression-list-opt: concept-expression-list
#line 896 "bang.y"
      { yylhs.value.as < concept_expression_list_t > () = yystack_[0].value.as < concept_expression_list_t > (); }
#line 3415 "bang.tab.cpp"
    break;

  case 131: // concept-expression-list: concept-expression
#line 901 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = concept_expression_list_t{std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3421 "bang.tab.cpp"
    break;

  case 132: // concept-expression-list: concept-expression-list concept-expression
#line 903 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = std::move(yystack_[1].value.as < concept_expression_list_t > ()); yylhs.value.as < concept_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3427 "bang.tab.cpp"
    break;

  case 133: // syntax-expression: CONTEXT_IDENTIFIER
#line 911 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3433 "bang.tab.cpp"
    break;

  case 134: // syntax-expression: CT_IDENTIFIER
#line 913 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3439 "bang.tab.cpp"
    break;

  case 135: // syntax-expression: syntax-expression-wo-ii
#line 914 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3445 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: "nil"
#line 920 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3451 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: "true"
#line 922 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3457 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: "false"
#line 924 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3463 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: INTEGER
#line 926 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3469 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: DECIMAL
#line 928 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3475 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: DECIMAL_S
#line 930 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3481 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: INTEGER_INDEX
#line 932 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3487 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: STRING
#line 934 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3493 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3499 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: qname
#line 938 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3505 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 940 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                BOOST_ASSERT(!yystack_[1].value.as < named_expression_list_t > ().empty());
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3518 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 949 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3530 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 957 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3536 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 959 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3542 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: "`.`" identifier
#line 961 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3548 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 965 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3554 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 967 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3560 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 969 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3566 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 972 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3572 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 974 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3578 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 979 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3584 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 981 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3590 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 985 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3596 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 987 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3602 "bang.tab.cpp"
    break;

  case 160: // syntax-expression-wo-ii: syntax-expression "`..`" syntax-expression
#line 989 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3608 "bang.tab.cpp"
    break;

  case 161: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 992 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3614 "bang.tab.cpp"
    break;

  case 162: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 996 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3620 "bang.tab.cpp"
    break;

  case 163: // syntax-expression-wo-ii: syntax-expression "`&&`" syntax-expression
#line 999 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3626 "bang.tab.cpp"
    break;

  case 164: // syntax-expression-wo-ii: syntax-expression "`||`" syntax-expression
#line 1002 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3632 "bang.tab.cpp"
    break;

  case 165: // syntax-expression-wo-ii: apostrophe-expression
#line 1004 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3638 "bang.tab.cpp"
    break;

  case 166: // syntax-expression-wo-ii: new-expression
#line 1005 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3644 "bang.tab.cpp"
    break;

  case 167: // syntax-expression-wo-ii: compound-expression
#line 1006 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3650 "bang.tab.cpp"
    break;

  case 168: // syntax-expression-wo-ii: lambda-expression
#line 1007 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3656 "bang.tab.cpp"
    break;

  case 169: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 1012 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3662 "bang.tab.cpp"
    break;

  case 170: // new-expression: "`new`" qname
#line 1017 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3668 "bang.tab.cpp"
    break;

  case 171: // new-expression: "`new`" apostrophe-expression
#line 1019 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3674 "bang.tab.cpp"
    break;

  case 172: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 1021 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3680 "bang.tab.cpp"
    break;

  case 173: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 1023 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3686 "bang.tab.cpp"
    break;

  case 174: // call-expression: qname "`(`" pack-expression "`)`"
#line 1028 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3692 "bang.tab.cpp"
    break;

  case 175: // call-expression: "mut modifier" "`(`" pack-expression "`)`"
#line 1030 "bang.y"
        { 
            auto aid = ctx.make_identifier(annotated_string_view{ "mut"sv, std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()) });
            yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{ qname{ aid.value, true }, std::move(aid.location) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
        }
#line 3701 "bang.tab.cpp"
    break;

  case 176: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 1035 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3707 "bang.tab.cpp"
    break;

  case 177: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 1037 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3713 "bang.tab.cpp"
    break;

  case 178: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 1039 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3719 "bang.tab.cpp"
    break;

  case 179: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 1044 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3725 "bang.tab.cpp"
    break;

  case 180: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 1046 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3731 "bang.tab.cpp"
    break;

  case 181: // pack-expression: syntax-expression
#line 1051 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{ named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3737 "bang.tab.cpp"
    break;

  case 182: // pack-expression: identifier "`:`" syntax-expression
#line 1060 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3747 "bang.tab.cpp"
    break;

  case 183: // pack-expression: pack-expression "," syntax-expression
#line 1066 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3756 "bang.tab.cpp"
    break;

  case 184: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 1071 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3765 "bang.tab.cpp"
    break;

  case 185: // compound-expression: syntax-expression "`...`"
#line 1079 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3771 "bang.tab.cpp"
    break;

  case 186: // compound-expression: call-expression
#line 1080 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3777 "bang.tab.cpp"
    break;

  case 187: // type-expr: qname
#line 1167 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3783 "bang.tab.cpp"
    break;

  case 188: // type-expr: call-expression
#line 1168 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3789 "bang.tab.cpp"
    break;

  case 189: // type-expr: CONTEXT_IDENTIFIER
#line 1170 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3795 "bang.tab.cpp"
    break;

  case 190: // type-expr: "`[`" type-expr "`]`"
#line 1174 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3801 "bang.tab.cpp"
    break;

  case 191: // type-expr: "`(`" "`)`"
#line 1176 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3807 "bang.tab.cpp"
    break;

  case 192: // type-expr: "`(`" pack-expression "`)`"
#line 1179 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3813 "bang.tab.cpp"
    break;

  case 193: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1181 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3819 "bang.tab.cpp"
    break;

  case 194: // type-expr: type-expr "`|`" type-expr
#line 1183 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3831 "bang.tab.cpp"
    break;

  case 195: // type-expr: type-expr "`->`" type-expr
#line 1191 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 3840 "bang.tab.cpp"
    break;


#line 3844 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -228;

  const signed char parser::yytable_ninf_ = -96;

  const short
  parser::yypact_[] =
  {
     785,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
     885,    25,   885,   885,   885,    25,   885,   885,    -5,    21,
      35,   885,   885,   885,    13,    37,   -45,  -228,    13,    13,
      41,  -228,  -228,  -228,   147,    26,   488,  -228,  -228,  -228,
    -228,   127,    -3,   690,  -228,    98,  -228,   156,   160,    20,
     167,   894,  -228,  -228,   174,  1026,   -22,   133,  1026,    43,
    -228,     5,     5,    25,  -228,   179,   213,  -228,    25,     6,
     270,   202,   592,   493,   775,   223,  -228,  -228,  -228,   151,
    -228,   205,  -228,   885,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,    25,   885,   384,  -228,   211,   224,   250,  -228,   885,
     885,   885,   885,   885,   885,  -228,   885,   116,   885,   885,
     885,   885,   885,   885,   885,  -228,   885,  -228,   885,  -228,
     885,  -228,  -228,   378,   885,   232,   257,  -228,   885,   885,
     686,   885,  -228,   885,   225,   885,   384,    25,   262,   384,
    -228,   101,  -228,   120,  -228,    13,   107,   295,   885,   378,
      45,    88,   301,   265,   255,  -228,   305,   305,  -228,   295,
     384,  -228,  1026,   175,   175,  1059,   455,    27,   938,  -228,
    -228,     5,     5,   577,   676,   143,   161,   190,  1026,   281,
    1026,  1026,  -228,   806,   378,   127,    98,   156,   160,    15,
     259,   378,   384,    44,   283,   267,  -228,   885,  1026,   292,
    -228,  -228,   885,    13,   285,   587,  -228,   294,  -228,  -228,
      -3,   775,   775,   250,  1026,   304,  -228,   299,   284,  -228,
      25,   306,  -228,  -228,   211,   295,   403,   295,  -228,   295,
    -228,   979,    60,  -228,   312,   295,   384,  -228,   137,  -228,
      19,   384,   885,  -228,  -228,  -228,   310,  -228,  -228,  -228,
    -228,   885,  -228,   227,    99,   378,   353,   378,    15,   318,
    -228,  -228,  -228,   885,  1026,  -228,  1026,   254,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,   349,  -228,    25,    71,
     323,   322,  -228,    64,  -228,   239,   885,   330,   122,   228,
    -228,  -228,  -228,   141,  -228,   384,  -228,   146,   305,   305,
     295,  -228,   378,  -228,  -228,  1026,   260,  1026,  -228,  -228,
    -228,   327,    42,   244,  -228,   384,   885,  -228,   378,   378,
    -228,    25,   334,  -228,   958,  1016,   958,   958,  -228,   403,
     295,  -228,   305,   305,  -228,  -228,  -228,   191,   958,   885,
     378,  -228,   339,  1026,    55,    55,  -228,    25,  -228,   145,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  1026,    15,
     250,   885,  -228,  -228,   344,   958,  1026,  -228,  -228
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   143,    51,   133,   144,   134,   142,   139,   140,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,   136,   137,   138,     0,     0,     0,     5,     7,    15,
      54,   145,     0,     0,   135,   165,   166,   186,   168,   167,
       0,     0,   167,    53,    54,   181,     0,     0,    73,     0,
     150,   155,   154,     0,    42,    21,    17,    19,     0,     0,
     170,   171,     0,     0,     0,     0,    43,    13,    57,     0,
      39,     0,    16,     0,     1,     2,    10,     3,     8,     6,
       9,     0,     0,    91,    59,    58,     0,     0,   153,     0,
       0,     0,     0,     0,     0,   185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   169,     0,   146,     0,   147,
       0,   148,    20,     0,     0,     0,     0,    12,    75,    75,
      23,     0,    31,     0,    34,     0,    91,    64,     0,    91,
      37,     0,    55,     0,    52,     0,   129,   129,     0,     0,
      54,   129,   107,     0,    92,    93,    97,    97,   131,   125,
      91,    36,    45,   158,   159,   163,   164,   160,     0,   151,
     152,   156,   157,   161,   162,     0,     0,     0,   182,    54,
     183,    74,   189,     0,     0,   187,     0,   188,     0,    22,
      18,     0,    91,    54,     0,    76,    77,     0,    80,     0,
      48,    47,    49,     0,    40,    41,    26,     0,    46,    24,
       0,     0,     0,     0,    71,     0,    68,     0,    65,    66,
      83,     0,   175,   174,   128,   129,     0,   129,   124,   130,
     127,     0,   105,    96,     0,   129,     0,   119,   129,   108,
       0,     0,     0,   104,   101,   132,     0,   149,   177,   176,
     178,     0,   191,     0,     0,     0,     0,     0,    11,     0,
      81,    82,   172,     0,    79,   173,    50,     0,    29,    27,
      28,    25,    30,    32,    33,    35,     0,    63,     0,     0,
       0,    84,    85,     0,   126,   129,     0,    54,   129,     0,
     110,   112,   121,   129,   106,     0,   120,   129,    97,    97,
     129,   117,     0,   179,    94,    98,    60,   184,   192,   190,
     195,     0,   194,    60,    78,    91,     0,    67,     0,     0,
      69,     0,     0,    38,     0,     0,     0,     0,   109,     0,
     129,   122,    97,    97,   103,   100,   118,     0,     0,     0,
       0,   193,     0,    72,    87,    87,    86,    83,   113,   129,
     114,   115,   111,   123,   102,    99,   180,    62,    14,    61,
       0,     0,    90,    89,     0,     0,    88,    70,   116
  };

  const short
  parser::yypgoto_[] =
  {
    -228,  -228,  -228,  -228,   359,  -228,  -228,  -228,  -228,  -228,
     -34,    14,  -228,  -128,   193,   155,    39,     4,     7,   357,
     337,  -228,  -228,  -228,   125,  -228,  -228,   394,   282,  -228,
     149,  -228,    66,  -228,    69,    94,  -131,  -228,  -228,  -139,
     176,  -227,  -228,   275,  -228,    93,   110,  -153,   162,    86,
       0,  -228,    38,  -228,  -119,    33,   -58,    11,   105
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    34,    35,    36,    37,    64,    65,    66,   204,   205,
     140,    38,   207,    39,   209,    40,   151,    41,    50,   126,
      97,    80,   217,   218,   219,    82,    76,    57,   194,   195,
     196,   197,   280,   281,   362,   282,   153,   154,   234,   243,
     155,   156,   238,   227,   289,   290,   157,   158,   237,   159,
      55,    44,    45,    46,    47,    48,    56,    52,   189
  };

  const short
  parser::yytable_[] =
  {
      43,     2,   208,     2,   187,   215,   245,    42,   221,   298,
      51,    49,     2,    58,    58,    98,    61,    62,   244,     2,
     -44,    72,    73,    74,    70,   141,    85,   117,    75,   246,
     187,     2,    79,    81,   143,   255,    43,    98,   132,   302,
     134,     2,    77,    42,    11,    93,    95,    49,   118,    78,
      90,    10,   144,    11,   175,   176,   177,   106,    71,   260,
      11,   259,   255,   161,   107,   187,   190,   256,   332,   130,
     361,   -44,   187,    95,    94,   255,   245,   208,   -44,   106,
     255,   257,    11,    94,    86,   322,   107,   108,   109,    83,
     261,   -95,   318,    63,   256,   294,    68,   152,   121,   162,
     163,   164,   165,   166,   167,   145,   168,   256,   171,   172,
     173,   174,   256,   120,   130,    69,   178,   319,   180,   255,
     181,   257,     2,   235,   145,   253,   257,   185,   198,   198,
      43,   211,    10,   212,   236,   214,   187,   210,   187,   145,
     152,    49,   225,   152,   206,   170,   112,    84,   231,   224,
     222,   256,   309,   185,   145,   226,   188,   235,   145,   334,
     335,   186,   145,   145,   152,   257,    53,    54,   327,   223,
      60,   118,   300,    67,    91,    92,   330,   273,   274,   275,
     330,   235,   188,   187,   342,    98,   119,   186,   185,   233,
     118,   365,   248,   354,   355,   185,   152,   264,    91,   187,
     187,   137,   266,   120,   113,    43,   303,   267,   114,   104,
     249,   255,   210,   118,    95,    93,    49,   188,   122,   270,
     116,   187,   186,   125,   188,   123,   138,   106,   124,   186,
     152,   118,   229,   229,   107,   108,   109,   229,    54,   250,
     152,   130,   305,   256,   135,   152,   142,    54,   150,   323,
     129,   307,    91,   139,   232,   130,   145,   257,    91,   185,
     118,   185,   169,   198,   338,   288,   340,    54,    54,    54,
      91,   136,   160,   179,   225,   297,   308,   328,   191,    54,
     338,   339,   340,   193,   193,   324,   325,   226,   188,   254,
     188,   150,   216,   186,   150,   186,   258,   118,   329,   152,
     130,    91,   315,   356,   130,   192,   185,   213,   228,   230,
     220,   229,   145,   229,   240,   150,   343,    91,   128,   152,
     242,   229,   185,   185,   229,   241,   323,   251,   152,   118,
     152,   152,   262,   152,   297,   188,   291,   263,    54,   358,
     186,   265,   152,   268,   185,   272,   299,   150,    91,   226,
     277,   188,   188,   276,   278,   283,   186,   186,   295,   306,
     310,   366,   312,   297,   311,   297,   297,   313,   288,   152,
     316,   229,   320,   188,   229,   279,   326,   297,   186,   229,
     341,   287,   347,   229,     2,   182,   229,   284,   360,   292,
       2,   144,   321,   367,    10,    89,   150,   296,   271,    96,
     301,   145,   146,   317,   297,   333,   127,   337,    59,     2,
     144,   199,   314,   364,   363,   346,   229,   304,   193,   147,
     145,   285,   352,   344,   345,    11,   183,   239,     0,     0,
     184,    11,   148,   216,   348,   229,   350,   351,   147,   291,
       0,     0,     0,     0,     0,   359,     0,   228,   357,     0,
      11,   286,   149,     0,     0,   331,     0,     0,     0,     0,
       0,     0,   336,     0,     0,    98,     0,     0,     0,     0,
     150,    26,    27,     0,     0,   368,   279,     0,    30,     0,
       0,     0,     0,     0,   287,   100,   101,   102,    87,   104,
       0,     0,   353,     1,     2,     3,     4,     5,     6,     7,
       8,     9,   279,    98,    10,     0,     0,   106,     0,     0,
       0,   331,     0,     0,   107,   108,   109,     0,     0,     0,
     110,   111,     0,   100,   101,   102,   103,   104,   105,     0,
       0,     0,     0,     0,     0,    11,    12,     0,     0,     0,
      13,     0,    14,     0,     0,   106,    88,    15,     0,     0,
      16,     0,   107,   108,   109,    17,     0,     0,   110,   111,
       0,     0,    18,     0,    19,    20,    21,    22,     0,    23,
       0,     0,     0,   133,     0,    24,     0,     0,     0,     0,
      25,    26,    27,    28,    29,     0,     0,    98,    30,    31,
      32,    33,     1,     2,     3,     4,     5,     6,     7,     8,
       9,     0,    98,    10,     0,     0,     0,   100,   101,     0,
       0,   104,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   100,   101,   102,   103,   104,   105,     0,   106,
       0,     0,     0,     0,    11,    12,   107,   108,   109,    13,
       0,    14,   130,     0,   106,   269,    15,     0,     0,    16,
     131,   107,   108,   109,    17,     0,     0,   110,   111,     0,
       0,    18,     0,     0,    20,    21,    22,     0,    23,     0,
     200,   201,   202,     0,    24,     0,     0,     0,     0,     0,
      26,    27,    28,   203,     0,     0,    98,    30,    31,    32,
      33,     1,     2,     3,     4,     5,     6,     7,     8,     9,
      98,     0,    10,     0,     0,    99,   100,   101,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     100,   101,   102,   103,   104,   105,     0,     0,   106,     0,
       0,     0,     0,    11,    12,   107,   108,   109,    13,     0,
      14,   110,   106,     0,     0,    15,     0,     0,    16,   107,
     108,   109,     0,    17,     0,   110,   111,     0,     0,     0,
      18,     0,     0,    20,    21,    22,     0,    23,     0,   200,
     201,   202,     0,    24,     0,     0,     0,     0,     0,    26,
      27,    28,   203,     0,     0,    98,    30,    31,    32,    33,
       1,     2,     3,     4,     5,     6,     7,     8,     9,     0,
       0,    10,     0,     0,     0,   100,   101,   102,   103,   104,
     105,     1,     2,     3,     4,     5,     6,     7,     8,     9,
       0,     0,    10,     0,     0,   130,     0,   106,     0,     0,
       0,     0,    11,    12,   107,   108,   109,    13,     0,    14,
     110,   111,     0,     0,    15,     0,     0,    16,     0,     0,
       0,     0,    17,    11,    12,   252,     0,     0,    13,    18,
      14,    19,    20,    21,    22,    15,    23,     0,    16,     0,
       0,     0,    24,    17,     0,     0,     0,    25,    26,    27,
      28,    29,     0,    20,     0,    30,    31,    32,    33,     0,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    26,
      27,    10,     0,     0,    98,     0,    30,    31,    32,    33,
     115,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   100,   101,   102,   103,   104,   105,
       0,     0,    11,    12,     0,     0,     0,    13,     0,    14,
       0,     0,     0,     0,    15,     0,   106,    16,    98,     0,
       0,     0,    17,   107,   108,   109,     0,     0,     0,   110,
     111,     0,    20,     0,     2,   144,     0,     0,   100,   101,
     102,   103,   104,   105,     0,   145,   146,     0,    26,    27,
       0,     0,     0,     0,     0,    30,    31,    32,    33,    98,
     106,   247,     0,   147,     0,     0,     0,   107,   108,   109,
       0,     0,     0,   110,   111,    11,   148,     0,     0,   100,
     101,   102,   103,   104,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,   293,     0,
       0,   106,     0,     0,     0,     0,    98,     0,   107,   108,
     109,     0,     0,     0,   110,   111,   100,   101,   102,   103,
     104,   105,     0,     0,     0,     0,   100,   101,   102,   103,
     104,   105,     0,     0,     0,   349,     0,     0,   106,    98,
       0,     0,     0,     0,     0,   107,   108,   109,   106,     0,
       0,   110,   111,     0,     0,   107,   108,   109,     0,   100,
     101,   110,   111,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,   107,   108,
     109,     0,     0,     0,   110,   111
  };

  const short
  parser::yycheck_[] =
  {
       0,     6,   130,     6,   123,   136,   159,     0,   139,   236,
      10,     0,     6,    13,    14,    10,    16,    17,   157,     6,
       0,    21,    22,    23,    20,    83,     0,    49,    24,   160,
     149,     6,    28,    29,    92,    20,    36,    10,    72,    20,
      74,     6,     5,    36,    47,    48,    42,    36,    70,    94,
      36,    16,     7,    47,   112,   113,   114,    52,    20,    15,
      47,   192,    20,    97,    59,   184,   124,    52,   295,    50,
      15,    51,   191,    69,    77,    20,   229,   205,    58,    52,
      20,    66,    47,    77,    58,    21,    59,    60,    61,    48,
      46,    46,    21,    98,    52,    35,    75,    93,    55,    99,
     100,   101,   102,   103,   104,    17,   106,    52,   108,   109,
     110,   111,    52,    70,    50,    94,   116,    46,   118,    20,
     120,    66,     6,    35,    17,   183,    66,   123,   128,   129,
     130,   131,    16,   133,    46,   135,   255,   130,   257,    17,
     136,   130,    35,   139,   130,   107,    48,     0,   148,   145,
      49,    52,    53,   149,    17,    48,   123,    35,    17,   298,
     299,   123,    17,    17,   160,    66,    11,    12,    46,    49,
      15,    70,    35,    18,    47,    48,    35,   211,   212,   213,
      35,    35,   149,   302,   315,    10,    53,   149,   184,   150,
      70,    46,    49,   332,   333,   191,   192,   197,    47,   318,
     319,    50,   202,    70,    48,   205,   240,   203,    48,    34,
      49,    20,   205,    70,   210,    48,   205,   184,    63,   205,
      46,   340,   184,    68,   191,    46,    21,    52,    15,   191,
     226,    70,   146,   147,    59,    60,    61,   151,    83,    49,
     236,    50,   242,    52,    21,   241,    91,    92,    93,   283,
      48,   251,    47,    48,   149,    50,    17,    66,    47,   255,
      70,   257,   107,   263,    20,   226,    22,   112,   113,   114,
      47,    48,    48,   118,    35,   236,    49,    49,    46,   124,
      20,    21,    22,   128,   129,    46,   286,    48,   255,   184,
     257,   136,   137,   255,   139,   257,   191,    70,    70,   295,
      50,    47,    48,   337,    50,    48,   302,    82,   146,   147,
      48,   225,    17,   227,    49,   160,   316,    47,    48,   315,
      15,   235,   318,   319,   238,    70,   360,    46,   324,    70,
     326,   327,    49,   329,   295,   302,   226,    70,   183,   339,
     302,    49,   338,    58,   340,    51,   236,   192,    47,    48,
      51,   318,   319,    49,    70,    49,   318,   319,    46,    49,
     255,   361,   257,   324,    11,   326,   327,    49,   329,   365,
      21,   285,    49,   340,   288,   220,    46,   338,   340,   293,
      53,   226,    48,   297,     6,     7,   300,   225,    49,   227,
       6,     7,    70,    49,    16,    36,   241,   235,   205,    42,
     238,    17,    18,   278,   365,   295,    69,   302,    14,     6,
       7,   129,   263,   347,   345,   321,   330,   241,   263,    35,
      17,    18,   329,   318,   319,    47,    48,   152,    -1,    -1,
      52,    47,    48,   278,   324,   349,   326,   327,    35,   329,
      -1,    -1,    -1,    -1,    -1,   340,    -1,   285,   338,    -1,
      47,    48,    68,    -1,    -1,   293,    -1,    -1,    -1,    -1,
      -1,    -1,   300,    -1,    -1,    10,    -1,    -1,    -1,    -1,
     315,    93,    94,    -1,    -1,   365,   321,    -1,   100,    -1,
      -1,    -1,    -1,    -1,   329,    30,    31,    32,     0,    34,
      -1,    -1,   330,     5,     6,     7,     8,     9,    10,    11,
      12,    13,   347,    10,    16,    -1,    -1,    52,    -1,    -1,
      -1,   349,    -1,    -1,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    48,    -1,    -1,    -1,
      52,    -1,    54,    -1,    -1,    52,    58,    59,    -1,    -1,
      62,    -1,    59,    60,    61,    67,    -1,    -1,    65,    66,
      -1,    -1,    74,    -1,    76,    77,    78,    79,    -1,    81,
      -1,    -1,    -1,    80,    -1,    87,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    -1,    -1,    10,   100,   101,
     102,   103,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    -1,    10,    16,    -1,    -1,    -1,    30,    31,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    -1,    52,
      -1,    -1,    -1,    -1,    47,    48,    59,    60,    61,    52,
      -1,    54,    50,    -1,    52,    58,    59,    -1,    -1,    62,
      58,    59,    60,    61,    67,    -1,    -1,    65,    66,    -1,
      -1,    74,    -1,    -1,    77,    78,    79,    -1,    81,    -1,
      83,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    10,   100,   101,   102,
     103,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      10,    -1,    16,    -1,    -1,    15,    30,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    47,    48,    59,    60,    61,    52,    -1,
      54,    65,    52,    -1,    -1,    59,    -1,    -1,    62,    59,
      60,    61,    -1,    67,    -1,    65,    66,    -1,    -1,    -1,
      74,    -1,    -1,    77,    78,    79,    -1,    81,    -1,    83,
      84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    10,   100,   101,   102,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    -1,
      -1,    16,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    16,    -1,    -1,    50,    -1,    52,    -1,    -1,
      -1,    -1,    47,    48,    59,    60,    61,    52,    -1,    54,
      65,    66,    -1,    -1,    59,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    67,    47,    48,    49,    -1,    -1,    52,    74,
      54,    76,    77,    78,    79,    59,    81,    -1,    62,    -1,
      -1,    -1,    87,    67,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    -1,    77,    -1,   100,   101,   102,   103,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    93,
      94,    16,    -1,    -1,    10,    -1,   100,   101,   102,   103,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      -1,    -1,    47,    48,    -1,    -1,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,    59,    -1,    52,    62,    10,    -1,
      -1,    -1,    67,    59,    60,    61,    -1,    -1,    -1,    65,
      66,    -1,    77,    -1,     6,     7,    -1,    -1,    30,    31,
      32,    33,    34,    35,    -1,    17,    18,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,    10,
      52,    53,    -1,    35,    -1,    -1,    -1,    59,    60,    61,
      -1,    -1,    -1,    65,    66,    47,    48,    -1,    -1,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    49,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    10,    -1,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    49,    -1,    -1,    52,    10,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,    52,    -1,
      -1,    65,    66,    -1,    -1,    59,    60,    61,    -1,    30,
      31,    65,    66,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,    -1,    -1,    -1,    65,    66
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      16,    47,    48,    52,    54,    59,    62,    67,    74,    76,
      77,    78,    79,    81,    87,    92,    93,    94,    95,    96,
     100,   101,   102,   103,   105,   106,   107,   108,   115,   117,
     119,   121,   122,   154,   155,   156,   157,   158,   159,   161,
     122,   154,   161,   119,   119,   154,   160,   131,   154,   131,
     119,   154,   154,    98,   109,   110,   111,   119,    75,    94,
     121,   156,   154,   154,   154,   121,   130,     5,    94,   121,
     125,   121,   129,    48,     0,     0,    58,     0,    58,   108,
     115,    47,    48,    48,    77,   121,   123,   124,    10,    15,
      30,    31,    32,    33,    34,    35,    52,    59,    60,    61,
      65,    66,    48,    48,    48,    16,    46,    49,    70,    53,
      70,    55,   119,    46,    15,   119,   123,   124,    48,    48,
      50,    58,   114,    80,   114,    21,    48,    50,    21,    48,
     114,   160,   119,   160,     7,    17,    18,    35,    48,    68,
     119,   120,   121,   140,   141,   144,   145,   150,   151,   153,
      48,   114,   154,   154,   154,   154,   154,   154,   154,   119,
     156,   154,   154,   154,   154,   160,   160,   160,   154,   119,
     154,   154,     7,    48,    52,   121,   156,   158,   159,   162,
     160,    46,    48,   119,   132,   133,   134,   135,   154,   132,
      83,    84,    85,    96,   112,   113,   115,   116,   117,   118,
     122,   154,   154,    82,   154,   140,   119,   126,   127,   128,
      48,   140,    49,    49,   121,    35,    48,   147,   152,   153,
     152,   154,   162,   120,   142,    35,    46,   152,   146,   147,
      49,    70,    15,   143,   143,   151,   140,    53,    49,    49,
      49,    46,    49,   160,   162,    20,    52,    66,   162,   140,
      15,    46,    49,    70,   154,    49,   154,   121,    58,    58,
     115,   118,    51,   114,   114,   114,    49,    51,    70,   119,
     136,   137,   139,    49,   152,    18,    48,   119,   120,   148,
     149,   150,   152,    49,    35,    46,   152,   120,   145,   150,
      35,   152,    20,   114,   144,   154,    49,   154,    49,    53,
     162,    11,   162,    49,   134,    48,    21,   128,    21,    46,
      49,    70,    21,   114,    46,   154,    46,    46,    49,    70,
      35,   152,   145,   150,   143,   143,   152,   162,    20,    21,
      22,    53,   140,   154,   162,   162,   139,    48,   150,    49,
     150,   150,   149,   152,   143,   143,   114,   150,   154,   162,
      49,    15,   138,   138,   136,    46,   154,    49,   150
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
     144,   144,   144,   144,   144,   145,   145,   146,   146,   147,
     148,   148,   149,   149,   149,   149,   149,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   151,   152,
     152,   153,   153,   154,   154,   154,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     157,   157,   157,   157,   158,   158,   158,   158,   158,   159,
     159,   160,   160,   160,   160,   161,   161,   162,   162,   162,
     162,   162,   162,   162,   162,   162
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
       4,     2,     5,     4,     2,     2,     3,     0,     1,     3,
       1,     3,     1,     3,     3,     3,     5,     3,     4,     2,
       3,     3,     4,     5,     2,     1,     3,     2,     2,     0,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     4,
       2,     3,     3,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       2,     2,     5,     5,     4,     4,     4,     4,     4,     5,
       7,     1,     3,     3,     5,     2,     1,     1,     1,     1,
       3,     2,     3,     4,     3,     3
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
  "ENUM", "STRUCT", "EXTENDS", "\"weak modifier\"", "\"const modifier\"",
  "\"mut modifier\"", "\"nil\"", "\"true\"", "\"false\"", "$accept",
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
  "parameter-default-value-opt", "parameter-decl", "constraint-expression",
  "subpatterns-opt", "subpatterns", "pattern-list", "pattern-field",
  "pattern", "concept-expression", "concept-expression-list-opt",
  "concept-expression-list", "syntax-expression",
  "syntax-expression-wo-ii", "apostrophe-expression", "new-expression",
  "call-expression", "lambda-expression", "pack-expression",
  "compound-expression", "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   330,   330,   331,   335,   337,   339,   356,   358,   359,
     361,   366,   368,   370,   372,   381,   383,   388,   390,   395,
     397,   402,   403,   408,   410,   412,   417,   419,   420,   422,
     427,   432,   434,   436,   438,   440,   442,   444,   446,   448,
     453,   455,   460,   462,   464,   466,   471,   473,   475,   477,
     479,   484,   492,   507,   509,   511,   517,   519,   524,   525,
     529,   531,   533,   539,   544,   545,   549,   551,   556,   562,
     564,   587,   591,   605,   607,   613,   614,   618,   620,   625,
     627,   632,   634,   640,   641,   645,   647,   652,   653,   657,
     659,   670,   671,   675,   677,   682,   683,   687,   688,   692,
     694,   696,   698,   700,   702,   707,   709,   833,   835,   839,
     844,   846,   851,   853,   855,   857,   859,   864,   866,   868,
     870,   872,   874,   876,   878,   880,   882,   884,   889,   894,
     896,   900,   902,   910,   912,   914,   919,   921,   923,   925,
     927,   929,   931,   933,   935,   937,   939,   948,   956,   958,
     960,   964,   966,   968,   971,   973,   978,   980,   984,   986,
     988,   991,   995,   998,  1001,  1004,  1005,  1006,  1007,  1011,
    1016,  1018,  1020,  1022,  1027,  1029,  1034,  1036,  1038,  1043,
    1045,  1050,  1059,  1065,  1070,  1078,  1080,  1166,  1168,  1169,
    1173,  1175,  1178,  1180,  1182,  1190
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
#line 4793 "bang.tab.cpp"

#line 1522 "bang.y"

