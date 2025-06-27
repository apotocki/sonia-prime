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

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
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
      case symbol_kind::S_161_pack_expression: // pack-expression
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
      case symbol_kind::S_155_syntax_expression: // syntax-expression
      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_158_new_expression: // new-expression
      case symbol_kind::S_159_call_expression: // call-expression
      case symbol_kind::S_160_lambda_expression: // lambda-expression
      case symbol_kind::S_162_compound_expression: // compound-expression
      case symbol_kind::S_163_type_expr: // type-expr
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

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
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
      case symbol_kind::S_161_pack_expression: // pack-expression
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
      case symbol_kind::S_155_syntax_expression: // syntax-expression
      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_158_new_expression: // new-expression
      case symbol_kind::S_159_call_expression: // call-expression
      case symbol_kind::S_160_lambda_expression: // lambda-expression
      case symbol_kind::S_162_compound_expression: // compound-expression
      case symbol_kind::S_163_type_expr: // type-expr
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

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
        value.YY_MOVE_OR_COPY< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
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
      case symbol_kind::S_161_pack_expression: // pack-expression
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
      case symbol_kind::S_155_syntax_expression: // syntax-expression
      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_158_new_expression: // new-expression
      case symbol_kind::S_159_call_expression: // call-expression
      case symbol_kind::S_160_lambda_expression: // lambda-expression
      case symbol_kind::S_162_compound_expression: // compound-expression
      case symbol_kind::S_163_type_expr: // type-expr
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

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
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
      case symbol_kind::S_161_pack_expression: // pack-expression
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
      case symbol_kind::S_155_syntax_expression: // syntax-expression
      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_158_new_expression: // new-expression
      case symbol_kind::S_159_call_expression: // call-expression
      case symbol_kind::S_160_lambda_expression: // lambda-expression
      case symbol_kind::S_162_compound_expression: // compound-expression
      case symbol_kind::S_163_type_expr: // type-expr
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

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
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
      case symbol_kind::S_161_pack_expression: // pack-expression
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
      case symbol_kind::S_155_syntax_expression: // syntax-expression
      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_158_new_expression: // new-expression
      case symbol_kind::S_159_call_expression: // call-expression
      case symbol_kind::S_160_lambda_expression: // lambda-expression
      case symbol_kind::S_162_compound_expression: // compound-expression
      case symbol_kind::S_163_type_expr: // type-expr
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

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
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
      case symbol_kind::S_161_pack_expression: // pack-expression
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
      case symbol_kind::S_155_syntax_expression: // syntax-expression
      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_158_new_expression: // new-expression
      case symbol_kind::S_159_call_expression: // call-expression
      case symbol_kind::S_160_lambda_expression: // lambda-expression
      case symbol_kind::S_162_compound_expression: // compound-expression
      case symbol_kind::S_163_type_expr: // type-expr
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
#line 326 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_CT_IDENTIFIER: // CT_IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 326 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 326 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 326 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 326 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 326 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 326 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 326 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 326 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 326 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 326 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 326 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 326 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 326 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 326 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 326 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 326 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 326 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 326 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 326 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 326 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 326 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 326 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 326 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 326 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 326 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 326 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 326 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 326 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 326 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 326 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 326 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 326 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 326 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
#line 326 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 326 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl: // let-decl
#line 326 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start: // let-decl-start
#line 326 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 326 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
#line 326 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
#line 326 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_braced_statements: // braced-statements
#line 326 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
#line 326 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
#line 326 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
#line 326 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_infunction_statement: // infunction-statement
#line 326 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 326 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_internal_identifier: // internal-identifier
#line 326 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 326 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
#line 326 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_name: // fn-name
#line 326 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
#line 326 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
#line 326 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
#line 326 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_list: // case-list
#line 326 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_case_decl: // case-decl
#line 326 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
#line 326 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_using_decl: // using-decl
#line 326 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_expression_list: // expression-list
#line 326 "bang.y"
                 { }
#line 2010 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
#line 326 "bang.y"
                 { }
#line 2016 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_argument_list: // argument-list
#line 326 "bang.y"
                 { }
#line 2022 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 326 "bang.y"
                 { }
#line 2028 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_argument_name: // argument-name
#line 326 "bang.y"
                 { }
#line 2034 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
#line 326 "bang.y"
                 { }
#line 2040 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_list: // field-list
#line 326 "bang.y"
                 { }
#line 2046 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
#line 326 "bang.y"
                 { }
#line 2052 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 326 "bang.y"
                 { }
#line 2058 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
#line 326 "bang.y"
                 { }
#line 2064 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_list: // parameter-list
#line 326 "bang.y"
                 { }
#line 2070 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
#line 326 "bang.y"
                 { }
#line 2076 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
#line 326 "bang.y"
                 { }
#line 2082 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
#line 326 "bang.y"
                 { }
#line 2088 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_constraint_expression_mod: // constraint-expression-mod
#line 326 "bang.y"
                 { }
#line 2094 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
#line 326 "bang.y"
                 { }
#line 2100 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_subpatterns_opt: // subpatterns-opt
#line 326 "bang.y"
                 { }
#line 2106 "bang.tab.cpp"
        break;

      case symbol_kind::S_subpatterns: // subpatterns
#line 326 "bang.y"
                 { }
#line 2112 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_pattern_list: // pattern-list
#line 326 "bang.y"
                 { }
#line 2118 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_pattern_field: // pattern-field
#line 326 "bang.y"
                 { }
#line 2124 "bang.tab.cpp"
        break;

      case symbol_kind::S_pattern: // pattern
#line 326 "bang.y"
                 { }
#line 2130 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
#line 326 "bang.y"
                 { }
#line 2136 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
#line 326 "bang.y"
                 { }
#line 2142 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
#line 326 "bang.y"
                 { }
#line 2148 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_syntax_expression: // syntax-expression
#line 326 "bang.y"
                 { }
#line 2154 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 326 "bang.y"
                 { }
#line 2160 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
#line 326 "bang.y"
                 { }
#line 2166 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_new_expression: // new-expression
#line 326 "bang.y"
                 { }
#line 2172 "bang.tab.cpp"
        break;

      case symbol_kind::S_159_call_expression: // call-expression
#line 326 "bang.y"
                 { }
#line 2178 "bang.tab.cpp"
        break;

      case symbol_kind::S_160_lambda_expression: // lambda-expression
#line 326 "bang.y"
                 { }
#line 2184 "bang.tab.cpp"
        break;

      case symbol_kind::S_161_pack_expression: // pack-expression
#line 326 "bang.y"
                 { }
#line 2190 "bang.tab.cpp"
        break;

      case symbol_kind::S_162_compound_expression: // compound-expression
#line 326 "bang.y"
                 { }
#line 2196 "bang.tab.cpp"
        break;

      case symbol_kind::S_163_type_expr: // type-expr
#line 326 "bang.y"
                 { }
#line 2202 "bang.tab.cpp"
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

      case symbol_kind::S_153_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_154_concept_expression_list: // concept-expression-list
        yylhs.value.emplace< concept_expression_list_t > ();
        break;

      case symbol_kind::S_146_constraint_expression: // constraint-expression
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
      case symbol_kind::S_161_pack_expression: // pack-expression
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

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_152_concept_expression: // concept-expression
      case symbol_kind::S_155_syntax_expression: // syntax-expression
      case symbol_kind::S_156_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_157_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_158_new_expression: // new-expression
      case symbol_kind::S_159_call_expression: // call-expression
      case symbol_kind::S_160_lambda_expression: // lambda-expression
      case symbol_kind::S_162_compound_expression: // compound-expression
      case symbol_kind::S_163_type_expr: // type-expr
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
#line 331 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2657 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 332 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2663 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 337 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2669 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 339 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2675 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 341 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2681 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 358 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2687 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 359 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2693 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 361 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2699 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 363 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2705 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 368 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), &ctx.push(std::move(yystack_[0].value.as < syntax_expression_t > ())) }; }
#line 2711 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 370 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2717 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 372 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2723 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-name "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 374 "bang.y"
        { 
            auto sts = ctx.new_statement_list();
            auto loc = get_start_location(yystack_[0].value.as < syntax_expression_t > ());
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) });
            yylhs.value.as < statement > () = fn_decl_t{ fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = nullptr, .kind = yystack_[6].value.as < fn_kind > () }, ctx.push(std::move(sts)) };
            IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
            //     $$ = fn_decl_t{ fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .result = std::move($value), .is_type_expression_result = false, .kind = $fnkind } }; IGNORE_TERM($OPEN_PARENTHESIS); }
        }
#line 2736 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 383 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2742 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 385 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2748 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 390 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2754 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 392 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2760 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: identifier
#line 397 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2766 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: "weak modifier" identifier
#line 399 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2772 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 403 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2778 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 405 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2784 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 410 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2790 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 412 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2796 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 414 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2802 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 419 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2808 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 420 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2814 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 422 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2820 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 424 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2826 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 429 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2832 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 434 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2838 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 436 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2844 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 438 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2850 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 440 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2856 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 442 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2862 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 444 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2868 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 446 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2874 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 448 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2880 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 450 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2886 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 455 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2892 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 457 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2898 "bang.tab.cpp"
    break;

  case 42: // generic-statement: LET let-decl
#line 462 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2904 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 464 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2910 "bang.tab.cpp"
    break;

  case 44: // generic-statement: compound-expression
#line 466 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2916 "bang.tab.cpp"
    break;

  case 45: // generic-statement: syntax-expression "`=`" syntax-expression
#line 468 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2922 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: generic-statement
#line 473 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2928 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: "`break`"
#line 475 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2934 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`continue`"
#line 477 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2940 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`return`"
#line 479 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .location = std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2946 "bang.tab.cpp"
    break;

  case 50: // infunction-statement: "`return`" syntax-expression
#line 481 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2952 "bang.tab.cpp"
    break;

  case 51: // identifier: IDENTIFIER
#line 486 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2958 "bang.tab.cpp"
    break;

  case 52: // internal-identifier: CONTEXT_IDENTIFIER
#line 494 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2964 "bang.tab.cpp"
    break;

  case 53: // qname: "`::`" identifier
#line 509 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2970 "bang.tab.cpp"
    break;

  case 54: // qname: identifier
#line 511 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2976 "bang.tab.cpp"
    break;

  case 55: // qname: qname "`::`" identifier
#line 513 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2982 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: "`fn`"
#line 519 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2988 "bang.tab.cpp"
    break;

  case 57: // fn-start-decl: INLINE "`fn`"
#line 521 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2994 "bang.tab.cpp"
    break;

  case 58: // fn-name: qname
#line 525 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 3000 "bang.tab.cpp"
    break;

  case 59: // fn-name: "`new`"
#line 526 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 3006 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 531 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3012 "bang.tab.cpp"
    break;

  case 61: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`~>`" type-expr
#line 533 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3018 "bang.tab.cpp"
    break;

  case 62: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" pattern
#line 535 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3024 "bang.tab.cpp"
    break;

  case 63: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 541 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3030 "bang.tab.cpp"
    break;

  case 64: // case-list-opt: %empty
#line 545 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 3036 "bang.tab.cpp"
    break;

  case 65: // case-list-opt: case-list
#line 546 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 3042 "bang.tab.cpp"
    break;

  case 66: // case-list: case-decl
#line 551 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 3048 "bang.tab.cpp"
    break;

  case 67: // case-list: case-list "," case-decl
#line 553 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 3054 "bang.tab.cpp"
    break;

  case 68: // case-decl: identifier
#line 558 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 3060 "bang.tab.cpp"
    break;

  case 69: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 564 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3066 "bang.tab.cpp"
    break;

  case 70: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 566 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3072 "bang.tab.cpp"
    break;

  case 71: // using-decl: qname "`=>`" syntax-expression
#line 589 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 3080 "bang.tab.cpp"
    break;

  case 72: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 593 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3088 "bang.tab.cpp"
    break;

  case 73: // expression-list: syntax-expression
#line 607 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3094 "bang.tab.cpp"
    break;

  case 74: // expression-list: expression-list "," syntax-expression
#line 609 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3100 "bang.tab.cpp"
    break;

  case 75: // argument-list-opt: %empty
#line 614 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 3106 "bang.tab.cpp"
    break;

  case 76: // argument-list-opt: argument-list
#line 615 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 3112 "bang.tab.cpp"
    break;

  case 77: // argument-list: argument
#line 620 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 3118 "bang.tab.cpp"
    break;

  case 78: // argument-list: argument-list "," argument
#line 622 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 3124 "bang.tab.cpp"
    break;

  case 79: // argument: argument-name syntax-expression
#line 627 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3130 "bang.tab.cpp"
    break;

  case 80: // argument: syntax-expression
#line 629 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3136 "bang.tab.cpp"
    break;

  case 81: // argument-name: identifier "`=`"
#line 634 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3142 "bang.tab.cpp"
    break;

  case 82: // argument-name: identifier "`:`"
#line 636 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 3148 "bang.tab.cpp"
    break;

  case 83: // field-list-opt: %empty
#line 641 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 3154 "bang.tab.cpp"
    break;

  case 84: // field-list-opt: field-list
#line 642 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 3160 "bang.tab.cpp"
    break;

  case 85: // field-list: field
#line 647 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 3166 "bang.tab.cpp"
    break;

  case 86: // field-list: field-list "," field
#line 649 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 3172 "bang.tab.cpp"
    break;

  case 87: // field-default-value-opt: %empty
#line 653 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3178 "bang.tab.cpp"
    break;

  case 88: // field-default-value-opt: "`=`" syntax-expression
#line 654 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3184 "bang.tab.cpp"
    break;

  case 89: // field: identifier "`:`" type-expr field-default-value-opt
#line 659 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3190 "bang.tab.cpp"
    break;

  case 90: // field: identifier "`=>`" type-expr field-default-value-opt
#line 661 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3196 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 671 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3202 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 672 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3208 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 677 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3214 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 679 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3220 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 683 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3226 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 684 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3232 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 688 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3238 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 689 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3244 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" pattern parameter-default-value-opt
#line 694 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3250 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: internal-identifier "`:`" pattern parameter-default-value-opt
#line 696 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3256 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: pattern parameter-default-value-opt
#line 698 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{}, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3262 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`:`" constraint-expression parameter-default-value-opt
#line 700 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3268 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: internal-identifier "`:`" constraint-expression parameter-default-value-opt
#line 702 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3274 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: constraint-expression parameter-default-value-opt
#line 704 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3280 "bang.tab.cpp"
    break;

  case 105: // constraint-expression-mod: "`~`"
#line 708 "bang.y"
            { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::const_or_runtime_type; }
#line 3286 "bang.tab.cpp"
    break;

  case 106: // constraint-expression-mod: "`~`" "constexpr modifier"
#line 709 "bang.y"
                      { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::const_type; }
#line 3292 "bang.tab.cpp"
    break;

  case 107: // constraint-expression-mod: "`~`" "runctime modifier"
#line 710 "bang.y"
                    { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::runtime_type; }
#line 3298 "bang.tab.cpp"
    break;

  case 108: // constraint-expression: constraint-expression-mod type-expr
#line 715 "bang.y"
        { yylhs.value.as < constraint_expression_t > () = constraint_expression_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .modifier = yystack_[1].value.as < parameter_constraint_modifier_t > () }; }
#line 3304 "bang.tab.cpp"
    break;

  case 109: // constraint-expression: constraint-expression-mod type-expr "`...`"
#line 717 "bang.y"
        { yylhs.value.as < constraint_expression_t > () = constraint_expression_t{ .expression = std::move(yystack_[1].value.as < syntax_expression_t > ()), .modifier = yystack_[2].value.as < parameter_constraint_modifier_t > () | parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3310 "bang.tab.cpp"
    break;

  case 110: // subpatterns-opt: %empty
#line 831 "bang.y"
      { yylhs.value.as < pattern_list_t > () = pattern_list_t{}; }
#line 3316 "bang.tab.cpp"
    break;

  case 111: // subpatterns-opt: subpatterns
#line 832 "bang.y"
      { yylhs.value.as < pattern_list_t > () = yystack_[0].value.as < pattern_list_t > (); }
#line 3322 "bang.tab.cpp"
    break;

  case 112: // subpatterns: "`(`" pattern-list "`)`"
#line 837 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[1].value.as < pattern_list_t > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3328 "bang.tab.cpp"
    break;

  case 113: // pattern-list: pattern-field
#line 842 "bang.y"
        { yylhs.value.as < pattern_list_t > () = pattern_list_t{ std::move(yystack_[0].value.as < pattern_t::field > ()) }; }
#line 3334 "bang.tab.cpp"
    break;

  case 114: // pattern-list: pattern-list "," pattern-field
#line 844 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[2].value.as < pattern_list_t > ()); yylhs.value.as < pattern_list_t > ().emplace_back(std::move(yystack_[0].value.as < pattern_t::field > ())); }
#line 3340 "bang.tab.cpp"
    break;

  case 115: // pattern-field: pattern
#line 849 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3346 "bang.tab.cpp"
    break;

  case 116: // pattern-field: "`_`" "`:`" pattern
#line 851 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3352 "bang.tab.cpp"
    break;

  case 117: // pattern-field: identifier "`:`" pattern
#line 853 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3358 "bang.tab.cpp"
    break;

  case 118: // pattern-field: internal-identifier "`:`" pattern
#line 855 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3364 "bang.tab.cpp"
    break;

  case 119: // pattern-field: "`(`" syntax-expression "`)`" "`:`" pattern
#line 857 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[3].value.as < syntax_expression_t > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3370 "bang.tab.cpp"
    break;

  case 120: // pattern: qname subpatterns-opt concept-expression-list-opt
#line 862 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3376 "bang.tab.cpp"
    break;

  case 121: // pattern: qname subpatterns-opt "`...`" concept-expression-list-opt
#line 864 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[2].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .modifier = parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3382 "bang.tab.cpp"
    break;

  case 122: // pattern: internal-identifier concept-expression-list-opt
#line 866 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3388 "bang.tab.cpp"
    break;

  case 123: // pattern: internal-identifier "`...`" concept-expression-list-opt
#line 868 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .modifier = parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3394 "bang.tab.cpp"
    break;

  case 124: // pattern: "`_`" subpatterns concept-expression-list-opt
#line 870 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3400 "bang.tab.cpp"
    break;

  case 125: // pattern: "`(`" syntax-expression "`)`" concept-expression-list-opt
#line 872 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[2].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3406 "bang.tab.cpp"
    break;

  case 126: // pattern: "`(`" syntax-expression "`)`" "`...`" concept-expression-list-opt
#line 874 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[3].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .modifier = parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3412 "bang.tab.cpp"
    break;

  case 127: // pattern: "`_`" concept-expression-list-opt
#line 876 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3418 "bang.tab.cpp"
    break;

  case 128: // pattern: concept-expression-list
#line 878 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3424 "bang.tab.cpp"
    break;

  case 129: // pattern: "`_`" "`...`" concept-expression-list-opt
#line 880 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .modifier = parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3430 "bang.tab.cpp"
    break;

  case 130: // pattern: "`...`" concept-expression-list-opt
#line 882 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .modifier = parameter_constraint_modifier_t::ellipsis }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3436 "bang.tab.cpp"
    break;

  case 131: // concept-expression: "`@`" qname
#line 887 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_reference{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3442 "bang.tab.cpp"
    break;

  case 132: // concept-expression-list-opt: %empty
#line 892 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = {}; }
#line 3448 "bang.tab.cpp"
    break;

  case 133: // concept-expression-list-opt: concept-expression-list
#line 893 "bang.y"
      { yylhs.value.as < concept_expression_list_t > () = yystack_[0].value.as < concept_expression_list_t > (); }
#line 3454 "bang.tab.cpp"
    break;

  case 134: // concept-expression-list: concept-expression
#line 898 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = concept_expression_list_t{std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3460 "bang.tab.cpp"
    break;

  case 135: // concept-expression-list: concept-expression-list concept-expression
#line 900 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = std::move(yystack_[1].value.as < concept_expression_list_t > ()); yylhs.value.as < concept_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3466 "bang.tab.cpp"
    break;

  case 136: // syntax-expression: CONTEXT_IDENTIFIER
#line 908 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3472 "bang.tab.cpp"
    break;

  case 137: // syntax-expression: CT_IDENTIFIER
#line 910 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3478 "bang.tab.cpp"
    break;

  case 138: // syntax-expression: syntax-expression-wo-ii
#line 911 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3484 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: "nil"
#line 917 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3490 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: "true"
#line 919 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3496 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: "false"
#line 921 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3502 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: INTEGER
#line 923 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3508 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: DECIMAL
#line 925 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3514 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: DECIMAL_S
#line 927 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3520 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: INTEGER_INDEX
#line 929 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3526 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: STRING
#line 931 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3532 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 933 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3538 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: qname
#line 935 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3544 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 937 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                BOOST_ASSERT(!yystack_[1].value.as < named_expression_list_t > ().empty());
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3557 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 946 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3569 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 954 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3575 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 956 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3581 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: "`.`" identifier
#line 958 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3587 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 962 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3593 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 964 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3599 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 966 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3605 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 969 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3611 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 971 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3617 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 976 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3623 "bang.tab.cpp"
    break;

  case 160: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 978 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3629 "bang.tab.cpp"
    break;

  case 161: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 982 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3635 "bang.tab.cpp"
    break;

  case 162: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 984 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3641 "bang.tab.cpp"
    break;

  case 163: // syntax-expression-wo-ii: syntax-expression "`..`" syntax-expression
#line 986 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3647 "bang.tab.cpp"
    break;

  case 164: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 989 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3653 "bang.tab.cpp"
    break;

  case 165: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 993 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3659 "bang.tab.cpp"
    break;

  case 166: // syntax-expression-wo-ii: syntax-expression "`&&`" syntax-expression
#line 996 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3665 "bang.tab.cpp"
    break;

  case 167: // syntax-expression-wo-ii: syntax-expression "`||`" syntax-expression
#line 999 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3671 "bang.tab.cpp"
    break;

  case 168: // syntax-expression-wo-ii: apostrophe-expression
#line 1001 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3677 "bang.tab.cpp"
    break;

  case 169: // syntax-expression-wo-ii: new-expression
#line 1002 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3683 "bang.tab.cpp"
    break;

  case 170: // syntax-expression-wo-ii: compound-expression
#line 1003 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3689 "bang.tab.cpp"
    break;

  case 171: // syntax-expression-wo-ii: lambda-expression
#line 1004 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3695 "bang.tab.cpp"
    break;

  case 172: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 1009 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3701 "bang.tab.cpp"
    break;

  case 173: // new-expression: "`new`" qname
#line 1014 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3707 "bang.tab.cpp"
    break;

  case 174: // new-expression: "`new`" apostrophe-expression
#line 1016 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3713 "bang.tab.cpp"
    break;

  case 175: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 1018 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3719 "bang.tab.cpp"
    break;

  case 176: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 1020 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3725 "bang.tab.cpp"
    break;

  case 177: // call-expression: qname "`(`" pack-expression "`)`"
#line 1025 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3731 "bang.tab.cpp"
    break;

  case 178: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 1033 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3737 "bang.tab.cpp"
    break;

  case 179: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 1035 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3743 "bang.tab.cpp"
    break;

  case 180: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 1037 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3749 "bang.tab.cpp"
    break;

  case 181: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 1042 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3755 "bang.tab.cpp"
    break;

  case 182: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 1044 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3761 "bang.tab.cpp"
    break;

  case 183: // pack-expression: syntax-expression
#line 1049 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{ named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3767 "bang.tab.cpp"
    break;

  case 184: // pack-expression: identifier "`:`" syntax-expression
#line 1058 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3777 "bang.tab.cpp"
    break;

  case 185: // pack-expression: pack-expression "," syntax-expression
#line 1064 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3786 "bang.tab.cpp"
    break;

  case 186: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 1069 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3795 "bang.tab.cpp"
    break;

  case 187: // compound-expression: syntax-expression "`...`"
#line 1077 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3801 "bang.tab.cpp"
    break;

  case 188: // compound-expression: call-expression
#line 1078 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3807 "bang.tab.cpp"
    break;

  case 189: // type-expr: qname
#line 1165 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3813 "bang.tab.cpp"
    break;

  case 190: // type-expr: call-expression
#line 1166 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3819 "bang.tab.cpp"
    break;

  case 191: // type-expr: CONTEXT_IDENTIFIER
#line 1168 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3825 "bang.tab.cpp"
    break;

  case 192: // type-expr: "`[`" type-expr "`]`"
#line 1172 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3831 "bang.tab.cpp"
    break;

  case 193: // type-expr: "`(`" "`)`"
#line 1174 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3837 "bang.tab.cpp"
    break;

  case 194: // type-expr: "`(`" pack-expression "`)`"
#line 1177 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3843 "bang.tab.cpp"
    break;

  case 195: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1179 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3849 "bang.tab.cpp"
    break;

  case 196: // type-expr: type-expr "`|`" type-expr
#line 1181 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3861 "bang.tab.cpp"
    break;

  case 197: // type-expr: type-expr "`->`" type-expr
#line 1189 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 3870 "bang.tab.cpp"
    break;


#line 3874 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -215;

  const signed char parser::yytable_ninf_ = -96;

  const short
  parser::yypact_[] =
  {
     778,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,
     878,    23,   878,   878,   878,    23,   878,   878,    -5,    85,
      38,   878,   878,   878,    36,    26,   -47,  -215,    36,    36,
    -215,  -215,  -215,    84,     9,   481,  -215,  -215,  -215,  -215,
      72,    12,   683,  -215,    63,  -215,   130,   132,     7,   184,
     887,  -215,  -215,   227,  1015,   -44,    21,  1015,   199,  -215,
      17,    17,    23,  -215,   242,   128,  -215,    23,    19,   125,
     211,   585,   485,   768,   216,  -215,  -215,  -215,   239,  -215,
     338,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,    23,
     878,   359,  -215,   234,   244,   246,  -215,   878,   878,   878,
     878,   878,   878,  -215,   878,    97,   878,   878,   878,   878,
     878,   878,   878,  -215,   878,  -215,   878,  -215,   878,  -215,
    -215,   223,   878,   255,   262,  -215,   878,   878,   679,   878,
    -215,   878,   221,   878,   359,    23,   271,   359,  -215,  -215,
     158,  -215,    36,    75,   306,   878,   213,    31,    40,   177,
     275,   256,  -215,   223,   313,   313,  -215,   306,   359,  -215,
    1015,   297,   297,   444,   398,    95,   931,  -215,  -215,    17,
      17,   570,   669,   164,   172,   187,  1015,   287,  1015,  1015,
    -215,   799,   223,    72,    63,   130,   132,   150,   265,   223,
     359,    25,   290,   270,  -215,   878,  1015,   301,  -215,  -215,
     878,    36,   293,   580,  -215,   304,  -215,  -215,    12,   768,
     768,   246,  1015,   314,  -215,   316,   294,  -215,    23,   323,
    -215,   234,   306,   374,   306,  -215,   306,  -215,   968,  -215,
    -215,  -215,   332,   306,   359,  -215,   131,  -215,    -1,   359,
     149,   878,  -215,  -215,  -215,   333,  -215,  -215,  -215,  -215,
     878,  -215,   195,    28,   223,   378,   223,   150,   341,  -215,
    -215,  -215,   878,  1015,  -215,  1015,   135,  -215,  -215,  -215,
    -215,  -215,  -215,  -215,  -215,   372,  -215,    23,    69,   348,
     329,  -215,    32,  -215,   115,   878,   354,   105,   235,  -215,
    -215,  -215,   205,   359,  -215,   214,   313,   313,   306,  -215,
     223,  -215,  -215,  -215,  1015,   349,  1015,  -215,  -215,  -215,
     350,    35,   257,  -215,   359,   878,  -215,   223,   223,  -215,
      23,   353,  -215,   506,  1005,   506,   506,  -215,   374,   306,
    -215,   313,   313,  -215,  -215,  -215,    92,   506,   878,   223,
    -215,   356,  1015,   144,   144,  -215,    23,  -215,   173,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,  1015,   150,   246,
     878,  -215,  -215,   362,   506,  1015,  -215,  -215
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   146,    51,   136,   147,   137,   145,   142,   143,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
     139,   140,   141,     0,     0,     0,     5,     7,    15,    54,
     148,     0,     0,   138,   168,   169,   188,   171,   170,     0,
       0,   170,    53,    54,   183,     0,     0,    73,     0,   153,
     158,   157,     0,    42,    21,    17,    19,     0,     0,   173,
     174,     0,     0,     0,     0,    43,    13,    57,     0,    39,
       0,    16,     1,     2,    10,     3,     8,     6,     9,     0,
       0,    91,    59,    58,     0,     0,   156,     0,     0,     0,
       0,     0,     0,   187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   172,     0,   149,     0,   150,     0,   151,
      20,     0,     0,     0,     0,    12,    75,    75,    23,     0,
      31,     0,    34,     0,    91,    64,     0,    91,    37,    55,
       0,    52,     0,   132,   132,     0,   105,    54,   132,   110,
       0,    92,    93,     0,    97,    97,   134,   128,    91,    36,
      45,   161,   162,   166,   167,   163,     0,   154,   155,   159,
     160,   164,   165,     0,     0,     0,   184,    54,   185,    74,
     191,     0,     0,   189,     0,   190,     0,    22,    18,     0,
      91,    54,     0,    76,    77,     0,    80,     0,    48,    47,
      49,     0,    40,    41,    26,     0,    46,    24,     0,     0,
       0,     0,    71,     0,    68,     0,    65,    66,    83,     0,
     177,   131,   132,     0,   132,   127,   133,   130,     0,   106,
     107,    96,     0,   132,     0,   122,   132,   111,     0,     0,
     108,     0,   104,   101,   135,     0,   152,   179,   178,   180,
       0,   193,     0,     0,     0,     0,     0,    11,     0,    81,
      82,   175,     0,    79,   176,    50,     0,    29,    27,    28,
      25,    30,    32,    33,    35,     0,    63,     0,     0,     0,
      84,    85,     0,   129,   132,     0,    54,   132,     0,   113,
     115,   124,   132,     0,   123,   132,    97,    97,   132,   120,
       0,   181,    94,   109,    98,    60,   186,   194,   192,   197,
       0,   196,    60,    78,    91,     0,    67,     0,     0,    69,
       0,     0,    38,     0,     0,     0,     0,   112,     0,   132,
     125,    97,    97,   103,   100,   121,     0,     0,     0,     0,
     195,     0,    72,    87,    87,    86,    83,   116,   132,   117,
     118,   114,   126,   102,    99,   182,    62,    14,    61,     0,
       0,    90,    89,     0,     0,    88,    70,   119
  };

  const short
  parser::yypgoto_[] =
  {
    -215,  -215,  -215,  -215,   367,  -215,  -215,  -215,  -215,  -215,
     -34,    11,  -215,  -125,   210,   156,   -17,     4,     6,   373,
     351,  -215,  -215,  -215,   140,  -215,  -215,   406,   296,  -215,
     162,  -215,    79,  -215,    82,   111,  -122,  -215,  -215,  -144,
     197,  -215,  -214,  -215,   285,  -215,   120,   119,  -153,   112,
     151,     0,  -215,    44,  -215,  -119,   -65,   -60,     8,    98
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    33,    34,    35,    36,    63,    64,    65,   202,   203,
     138,    37,   205,    38,   207,    39,   148,    40,    49,   124,
      95,    79,   215,   216,   217,    81,    75,    56,   192,   193,
     194,   195,   279,   280,   361,   281,   150,   151,   232,   242,
     152,   153,   154,   236,   224,   288,   289,   155,   156,   235,
     157,    54,    43,    44,    45,    46,    47,    55,    51,   187
  };

  const short
  parser::yytable_[] =
  {
      42,     2,   185,   206,   244,   115,    41,   -44,    48,    83,
      50,   243,   213,    57,    57,   219,    60,    61,     2,   300,
     296,    71,    72,    73,    69,     2,   116,    96,    74,     2,
     140,    76,    78,    80,   185,    42,   245,   130,   141,   132,
     259,    41,     2,    48,     2,    93,    88,    77,   254,   128,
     173,   174,   175,   321,    10,   254,   186,   142,   -44,    11,
      91,   159,   188,   185,    70,   -44,    11,    84,   258,   104,
     185,   260,    93,   244,   117,   233,   105,   -95,   206,   331,
     255,   308,   128,    11,    82,    11,   234,   255,   186,    92,
     317,   118,   142,    62,   256,   149,    92,   160,   161,   162,
     163,   164,   165,     2,   166,    96,   169,   170,   171,   172,
     222,   110,   254,    10,   176,   318,   178,   186,   179,    89,
      90,   252,   142,   223,   186,   183,   196,   196,    42,   209,
     231,   210,   142,   212,   208,   185,    48,   185,   149,   204,
     233,   149,   128,   122,   255,   228,   221,   104,   142,   168,
     222,   326,   333,   334,   105,   106,   107,   183,   256,   360,
      67,   323,   149,   223,   254,   184,   298,    52,    53,   254,
     254,    59,    89,   126,    66,   272,   273,   274,   111,    68,
     112,   185,    89,   314,   303,   128,   183,   353,   354,   186,
     142,   186,   341,   183,   149,   263,   255,   184,   185,   185,
     265,   255,   255,    42,   301,   266,   287,   220,   329,   208,
     256,    48,    93,   247,   269,   256,   256,   295,   120,   364,
     185,   248,   142,   123,    89,   223,   184,   149,   116,     2,
     180,   142,    91,   184,   116,   186,   249,   133,   149,    10,
     329,   304,   116,   149,   307,   139,    53,   147,   322,   233,
     306,   240,   186,   186,   119,   225,   227,   116,   183,   127,
     183,   167,   196,    89,   134,   116,    53,    53,    53,   118,
      11,   181,   177,   114,   186,   182,   295,   337,    53,   339,
     253,    89,   191,   191,   327,   324,    89,   257,   121,   135,
     147,   214,   158,   147,   226,   226,   128,   149,   184,   226,
     184,   189,   355,   211,   183,   328,   295,    96,   295,   295,
     190,   287,   229,   230,   147,   342,    26,    27,   149,   218,
     295,   183,   183,   142,   238,   322,   239,   149,   241,   149,
     149,   102,   149,   250,   283,   116,   291,    53,   357,   261,
     262,   149,   290,   183,   184,   294,   147,   295,   299,   104,
     264,   267,   309,   297,   311,   271,   105,   106,   107,   136,
     365,   184,   184,   275,   277,     2,   141,   276,   149,   337,
     338,   339,   282,   226,   278,   226,   142,   143,   293,   286,
       2,   141,   305,   184,   226,    89,   137,   226,   128,   310,
     312,   142,   284,   315,   144,   147,   225,   319,   336,   320,
     325,   346,    87,   340,   330,   359,    11,   145,    96,   144,
     335,   366,   332,   270,    94,   343,   344,   316,   191,   125,
      58,    11,   285,   197,   313,   363,   362,   146,    98,    99,
     100,   345,   102,   214,   237,   226,   302,   358,   226,     0,
       0,   352,   347,   226,   349,   350,   226,   290,   351,   226,
     104,     0,     0,     0,    96,     0,   356,   105,   106,   107,
     330,     0,     0,   108,   109,     0,     0,     0,     0,     0,
     147,     0,     0,     0,    98,    99,   278,     0,   102,     0,
     226,    85,     0,   367,   286,     0,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    96,   104,    10,     0,   226,
       0,     0,   278,   105,   106,   107,     0,     0,     0,   108,
     109,     0,     2,   141,     0,    98,    99,   100,   101,   102,
     103,     0,     0,   142,   143,     0,     0,     0,    11,    12,
       0,     0,     0,    13,     0,    14,     0,   104,     0,    86,
      15,   144,     0,    16,   105,   106,   107,     0,    17,     0,
     108,   109,     0,    11,   145,    18,     0,    19,    20,    21,
      22,     0,    23,     0,     0,   131,     0,     0,    24,     0,
       0,     0,     0,    25,    26,    27,    28,    29,     0,     0,
      96,     0,    30,    31,    32,     1,     2,     3,     4,     5,
       6,     7,     8,     9,     0,    96,    10,     0,     0,     0,
      98,    99,     0,     0,   102,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,    99,   100,   101,   102,
     103,     0,   104,     0,     0,     0,     0,    11,    12,   105,
     106,   107,    13,     0,    14,   128,     0,   104,   268,    15,
       0,     0,    16,   129,   105,   106,   107,    17,     0,     0,
     108,   109,     0,     0,    18,     0,     0,    20,    21,    22,
       0,    23,     0,   198,   199,   200,     0,    24,     0,     0,
       0,     0,     0,    26,    27,    28,   201,     0,     0,    96,
       0,    30,    31,    32,     1,     2,     3,     4,     5,     6,
       7,     8,     9,    96,     0,    10,     0,     0,    97,    98,
      99,     0,     0,   102,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,    99,   100,   101,   102,   103,     0,
       0,   104,     0,     0,     0,     0,    11,    12,   105,   106,
     107,    13,     0,    14,   108,   104,     0,     0,    15,     0,
       0,    16,   105,   106,   107,     0,    17,     0,   108,   109,
       0,     0,     0,    18,     0,     0,    20,    21,    22,     0,
      23,     0,   198,   199,   200,     0,    24,     0,     0,     0,
       0,     0,    26,    27,    28,   201,     0,     0,    96,     0,
      30,    31,    32,     1,     2,     3,     4,     5,     6,     7,
       8,     9,     0,     0,    10,     0,     0,     0,    98,    99,
     100,   101,   102,   103,     1,     2,     3,     4,     5,     6,
       7,     8,     9,     0,     0,    10,     0,     0,   128,     0,
     104,     0,     0,     0,     0,    11,    12,   105,   106,   107,
      13,     0,    14,   108,   109,     0,     0,    15,     0,     0,
      16,     0,     0,     0,     0,    17,    11,    12,   251,     0,
       0,    13,    18,    14,    19,    20,    21,    22,    15,    23,
       0,    16,     0,     0,     0,    24,    17,     0,     0,     0,
      25,    26,    27,    28,    29,     0,    20,     0,     0,    30,
      31,    32,     0,     1,     2,     3,     4,     5,     6,     7,
       8,     9,    26,    27,    10,     0,     0,    96,     0,     0,
      30,    31,    32,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,    99,   100,
     101,   102,   103,     0,     0,    11,    12,     0,     0,     0,
      13,     0,    14,     0,     0,     0,     0,    15,     0,   104,
      16,    96,     0,     0,     0,    17,   105,   106,   107,     0,
       0,     0,   108,   109,     0,    20,     0,     0,     0,     0,
       0,    98,    99,   100,   101,   102,   103,     0,     0,     0,
       0,    26,    27,     0,     0,     0,     0,     0,    96,    30,
      31,    32,     0,   104,   246,     0,     0,     0,     0,     0,
     105,   106,   107,     0,     0,     0,   108,   109,    98,    99,
     100,   101,   102,   103,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,   292,     0,     0,
     104,     0,     0,     0,     0,    96,     0,   105,   106,   107,
       0,     0,     0,   108,   109,    98,    99,   100,   101,   102,
     103,     0,     0,     0,     0,    98,    99,   100,   101,   102,
     103,     0,     0,     0,   348,     0,     0,   104,     0,     0,
       0,     0,     0,     0,   105,   106,   107,   104,     0,     0,
     108,   109,     0,     0,   105,   106,   107,     0,     0,     0,
     108,   109
  };

  const short
  parser::yycheck_[] =
  {
       0,     6,   121,   128,   157,    49,     0,     0,     0,     0,
      10,   155,   134,    13,    14,   137,    16,    17,     6,    20,
     234,    21,    22,    23,    20,     6,    70,    10,    24,     6,
      90,     5,    28,    29,   153,    35,   158,    71,     7,    73,
      15,    35,     6,    35,     6,    41,    35,    94,    20,    50,
     110,   111,   112,    21,    16,    20,   121,    17,    51,    47,
      48,    95,   122,   182,    20,    58,    47,    58,   190,    52,
     189,    46,    68,   226,    53,    35,    59,    46,   203,   293,
      52,    53,    50,    47,     0,    47,    46,    52,   153,    77,
      21,    70,    17,    98,    66,    91,    77,    97,    98,    99,
     100,   101,   102,     6,   104,    10,   106,   107,   108,   109,
      35,    48,    20,    16,   114,    46,   116,   182,   118,    47,
      48,   181,    17,    48,   189,   121,   126,   127,   128,   129,
     147,   131,    17,   133,   128,   254,   128,   256,   134,   128,
      35,   137,    50,    15,    52,   145,   142,    52,    17,   105,
      35,    46,   296,   297,    59,    60,    61,   153,    66,    15,
      75,    46,   158,    48,    20,   121,    35,    11,    12,    20,
      20,    15,    47,    48,    18,   209,   210,   211,    48,    94,
      48,   300,    47,    48,    35,    50,   182,   331,   332,   254,
      17,   256,   314,   189,   190,   195,    52,   153,   317,   318,
     200,    52,    52,   203,   238,   201,   223,    49,    35,   203,
      66,   203,   208,    49,   203,    66,    66,   234,    62,    46,
     339,    49,    17,    67,    47,    48,   182,   223,    70,     6,
       7,    17,    48,   189,    70,   300,    49,    21,   234,    16,
      35,   241,    70,   239,    49,    89,    90,    91,   282,    35,
     250,   153,   317,   318,    55,   143,   144,    70,   254,    48,
     256,   105,   262,    47,    48,    70,   110,   111,   112,    70,
      47,    48,   116,    46,   339,    52,   293,    20,   122,    22,
     182,    47,   126,   127,    49,   285,    47,   189,    46,    50,
     134,   135,    48,   137,   143,   144,    50,   293,   254,   148,
     256,    46,   336,    82,   300,    70,   323,    10,   325,   326,
      48,   328,    99,   100,   158,   315,    93,    94,   314,    48,
     337,   317,   318,    17,    49,   359,    70,   323,    15,   325,
     326,    34,   328,    46,   222,    70,   224,   181,   338,    49,
      70,   337,   223,   339,   300,   233,   190,   364,   236,    52,
      49,    58,   254,   234,   256,    51,    59,    60,    61,    21,
     360,   317,   318,    49,    70,     6,     7,    51,   364,    20,
      21,    22,    49,   222,   218,   224,    17,    18,    46,   223,
       6,     7,    49,   339,   233,    47,    48,   236,    50,    11,
      49,    17,    18,    21,    35,   239,   284,    49,   300,    70,
      46,    48,    35,    53,   292,    49,    47,    48,    10,    35,
     298,    49,   293,   203,    41,   317,   318,   277,   262,    68,
      14,    47,    48,   127,   262,   346,   344,    68,    30,    31,
      32,   320,    34,   277,   149,   284,   239,   339,   287,    -1,
      -1,   329,   323,   292,   325,   326,   295,   328,   328,   298,
      52,    -1,    -1,    -1,    10,    -1,   337,    59,    60,    61,
     348,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
     314,    -1,    -1,    -1,    30,    31,   320,    -1,    34,    -1,
     329,     0,    -1,   364,   328,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    10,    52,    16,    -1,   348,
      -1,    -1,   346,    59,    60,    61,    -1,    -1,    -1,    65,
      66,    -1,     6,     7,    -1,    30,    31,    32,    33,    34,
      35,    -1,    -1,    17,    18,    -1,    -1,    -1,    47,    48,
      -1,    -1,    -1,    52,    -1,    54,    -1,    52,    -1,    58,
      59,    35,    -1,    62,    59,    60,    61,    -1,    67,    -1,
      65,    66,    -1,    47,    48,    74,    -1,    76,    77,    78,
      79,    -1,    81,    -1,    -1,    80,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    -1,    -1,
      10,    -1,   101,   102,   103,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    -1,    10,    16,    -1,    -1,    -1,
      30,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    -1,    52,    -1,    -1,    -1,    -1,    47,    48,    59,
      60,    61,    52,    -1,    54,    50,    -1,    52,    58,    59,
      -1,    -1,    62,    58,    59,    60,    61,    67,    -1,    -1,
      65,    66,    -1,    -1,    74,    -1,    -1,    77,    78,    79,
      -1,    81,    -1,    83,    84,    85,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    10,
      -1,   101,   102,   103,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    10,    -1,    16,    -1,    -1,    15,    30,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    47,    48,    59,    60,
      61,    52,    -1,    54,    65,    52,    -1,    -1,    59,    -1,
      -1,    62,    59,    60,    61,    -1,    67,    -1,    65,    66,
      -1,    -1,    -1,    74,    -1,    -1,    77,    78,    79,    -1,
      81,    -1,    83,    84,    85,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    10,    -1,
     101,   102,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    16,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    16,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    47,    48,    59,    60,    61,
      52,    -1,    54,    65,    66,    -1,    -1,    59,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    67,    47,    48,    49,    -1,
      -1,    52,    74,    54,    76,    77,    78,    79,    59,    81,
      -1,    62,    -1,    -1,    -1,    87,    67,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    -1,    77,    -1,    -1,   101,
     102,   103,    -1,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    93,    94,    16,    -1,    -1,    10,    -1,    -1,
     101,   102,   103,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    -1,    -1,    47,    48,    -1,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    59,    -1,    52,
      62,    10,    -1,    -1,    -1,    67,    59,    60,    61,    -1,
      -1,    -1,    65,    66,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    10,   101,
     102,   103,    -1,    52,    53,    -1,    -1,    -1,    -1,    -1,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    10,    -1,    59,    60,    61,
      -1,    -1,    -1,    65,    66,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
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
     101,   102,   103,   105,   106,   107,   108,   115,   117,   119,
     121,   122,   155,   156,   157,   158,   159,   160,   162,   122,
     155,   162,   119,   119,   155,   161,   131,   155,   131,   119,
     155,   155,    98,   109,   110,   111,   119,    75,    94,   121,
     157,   155,   155,   155,   121,   130,     5,    94,   121,   125,
     121,   129,     0,     0,    58,     0,    58,   108,   115,    47,
      48,    48,    77,   121,   123,   124,    10,    15,    30,    31,
      32,    33,    34,    35,    52,    59,    60,    61,    65,    66,
      48,    48,    48,    16,    46,    49,    70,    53,    70,    55,
     119,    46,    15,   119,   123,   124,    48,    48,    50,    58,
     114,    80,   114,    21,    48,    50,    21,    48,   114,   119,
     161,     7,    17,    18,    35,    48,    68,   119,   120,   121,
     140,   141,   144,   145,   146,   151,   152,   154,    48,   114,
     155,   155,   155,   155,   155,   155,   155,   119,   157,   155,
     155,   155,   155,   161,   161,   161,   155,   119,   155,   155,
       7,    48,    52,   121,   157,   159,   160,   163,   161,    46,
      48,   119,   132,   133,   134,   135,   155,   132,    83,    84,
      85,    96,   112,   113,   115,   116,   117,   118,   122,   155,
     155,    82,   155,   140,   119,   126,   127,   128,    48,   140,
      49,   121,    35,    48,   148,   153,   154,   153,   155,    99,
     100,   120,   142,    35,    46,   153,   147,   148,    49,    70,
     163,    15,   143,   143,   152,   140,    53,    49,    49,    49,
      46,    49,   161,   163,    20,    52,    66,   163,   140,    15,
      46,    49,    70,   155,    49,   155,   121,    58,    58,   115,
     118,    51,   114,   114,   114,    49,    51,    70,   119,   136,
     137,   139,    49,   153,    18,    48,   119,   120,   149,   150,
     151,   153,    49,    46,   153,   120,   146,   151,    35,   153,
      20,   114,   144,    35,   155,    49,   155,    49,    53,   163,
      11,   163,    49,   134,    48,    21,   128,    21,    46,    49,
      70,    21,   114,    46,   155,    46,    46,    49,    70,    35,
     153,   146,   151,   143,   143,   153,   163,    20,    21,    22,
      53,   140,   155,   163,   163,   139,    48,   151,    49,   151,
     151,   150,   153,   143,   143,   114,   151,   155,   163,    49,
      15,   138,   138,   136,    46,   155,    49,   151
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
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   152,   153,   153,   154,   154,   155,   155,   155,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   157,   158,   158,   158,   158,   159,   159,   159,
     159,   160,   160,   161,   161,   161,   161,   162,   162,   163,
     163,   163,   163,   163,   163,   163,   163,   163
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
       3,     4,     2,     3,     3,     4,     5,     2,     1,     3,
       2,     2,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     4,     2,     3,     3,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     2,     2,     5,     5,     4,     4,     4,
       4,     5,     7,     1,     3,     3,     5,     2,     1,     1,
       1,     1,     3,     2,     3,     4,     3,     3
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
  "subpatterns", "pattern-list", "pattern-field", "pattern",
  "concept-expression", "concept-expression-list-opt",
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
       0,   331,   331,   332,   336,   338,   340,   357,   359,   360,
     362,   367,   369,   371,   373,   382,   384,   389,   391,   396,
     398,   403,   404,   409,   411,   413,   418,   420,   421,   423,
     428,   433,   435,   437,   439,   441,   443,   445,   447,   449,
     454,   456,   461,   463,   465,   467,   472,   474,   476,   478,
     480,   485,   493,   508,   510,   512,   518,   520,   525,   526,
     530,   532,   534,   540,   545,   546,   550,   552,   557,   563,
     565,   588,   592,   606,   608,   614,   615,   619,   621,   626,
     628,   633,   635,   641,   642,   646,   648,   653,   654,   658,
     660,   671,   672,   676,   678,   683,   684,   688,   689,   693,
     695,   697,   699,   701,   703,   708,   709,   710,   714,   716,
     830,   832,   836,   841,   843,   848,   850,   852,   854,   856,
     861,   863,   865,   867,   869,   871,   873,   875,   877,   879,
     881,   886,   891,   893,   897,   899,   907,   909,   911,   916,
     918,   920,   922,   924,   926,   928,   930,   932,   934,   936,
     945,   953,   955,   957,   961,   963,   965,   968,   970,   975,
     977,   981,   983,   985,   988,   992,   995,   998,  1001,  1002,
    1003,  1004,  1008,  1013,  1015,  1017,  1019,  1024,  1032,  1034,
    1036,  1041,  1043,  1048,  1057,  1063,  1068,  1076,  1078,  1164,
    1166,  1167,  1171,  1173,  1176,  1178,  1180,  1188
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
#line 4816 "bang.tab.cpp"

#line 1520 "bang.y"

