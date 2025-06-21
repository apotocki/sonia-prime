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

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.copy< constraint_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.copy< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.copy< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.move< constraint_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.YY_MOVE_OR_COPY< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.YY_MOVE_OR_COPY< constraint_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.YY_MOVE_OR_COPY< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.YY_MOVE_OR_COPY< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.YY_MOVE_OR_COPY< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.YY_MOVE_OR_COPY< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.move< constraint_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.move< pattern_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.move< pattern_t::field > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.copy< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.copy< constraint_expression_t > (that.value);
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.copy< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.copy< pattern_t > (that.value);
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.copy< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (that.value);
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.move< constraint_expression_t > (that.value);
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.move< pattern_list_t > (that.value);
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (that.value);
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.move< pattern_t::field > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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
#line 1614 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 325 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 325 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 325 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 325 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 325 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 325 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 325 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 325 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 325 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 325 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 325 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 325 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 325 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 325 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 325 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 325 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 325 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 325 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 325 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 325 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 325 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 325 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 325 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 325 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 325 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 325 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 325 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 325 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 325 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_MUT: // "mut modifier"
#line 325 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 325 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 325 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 325 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 325 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
#line 325 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 325 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_let_decl: // let-decl
#line 325 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl_start: // let-decl-start
#line 325 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 325 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_braced_statements: // braced-statements
#line 325 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
#line 325 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
#line 325 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
#line 325 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_infunction_statement: // infunction-statement
#line 325 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 325 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_internal_identifier: // internal-identifier
#line 325 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 325 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
#line 325 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_name: // fn-name
#line 325 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
#line 325 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
#line 325 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
#line 325 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list: // case-list
#line 325 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_decl: // case-decl
#line 325 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
#line 325 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_using_decl: // using-decl
#line 325 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_expression_list: // expression-list
#line 325 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
#line 325 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list: // argument-list
#line 325 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 325 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_argument_name: // argument-name
#line 325 "bang.y"
                 { }
#line 2010 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
#line 325 "bang.y"
                 { }
#line 2016 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list: // field-list
#line 325 "bang.y"
                 { }
#line 2022 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
#line 325 "bang.y"
                 { }
#line 2028 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 325 "bang.y"
                 { }
#line 2034 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
#line 325 "bang.y"
                 { }
#line 2040 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list: // parameter-list
#line 325 "bang.y"
                 { }
#line 2046 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
#line 325 "bang.y"
                 { }
#line 2052 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
#line 325 "bang.y"
                 { }
#line 2058 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
#line 325 "bang.y"
                 { }
#line 2064 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
#line 325 "bang.y"
                 { }
#line 2070 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
#line 325 "bang.y"
                 { }
#line 2076 "bang.tab.cpp"
        break;

      case symbol_kind::S_subpatterns: // subpatterns
#line 325 "bang.y"
                 { }
#line 2082 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_pattern_list: // pattern-list
#line 325 "bang.y"
                 { }
#line 2088 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
#line 325 "bang.y"
                 { }
#line 2094 "bang.tab.cpp"
        break;

      case symbol_kind::S_pattern: // pattern
#line 325 "bang.y"
                 { }
#line 2100 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
#line 325 "bang.y"
                 { }
#line 2106 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
#line 325 "bang.y"
                 { }
#line 2112 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
#line 325 "bang.y"
                 { }
#line 2118 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_syntax_expression: // syntax-expression
#line 325 "bang.y"
                 { }
#line 2124 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 325 "bang.y"
                 { }
#line 2130 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
#line 325 "bang.y"
                 { }
#line 2136 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_new_expression: // new-expression
#line 325 "bang.y"
                 { }
#line 2142 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_call_expression: // call-expression
#line 325 "bang.y"
                 { }
#line 2148 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_lambda_expression: // lambda-expression
#line 325 "bang.y"
                 { }
#line 2154 "bang.tab.cpp"
        break;

      case symbol_kind::S_159_pack_expression: // pack-expression
#line 325 "bang.y"
                 { }
#line 2160 "bang.tab.cpp"
        break;

      case symbol_kind::S_160_compound_expression: // compound-expression
#line 325 "bang.y"
                 { }
#line 2166 "bang.tab.cpp"
        break;

      case symbol_kind::S_161_type_expr: // type-expr
#line 325 "bang.y"
                 { }
#line 2172 "bang.tab.cpp"
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

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        yylhs.value.emplace< concept_expression_list_t > ();
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        yylhs.value.emplace< constraint_expression_t > ();
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< managed_statement_list > ();
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        yylhs.value.emplace< pattern_list_t > ();
        break;

      case symbol_kind::S_pattern: // pattern
        yylhs.value.emplace< pattern_t > ();
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        yylhs.value.emplace< pattern_t::field > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        yylhs.value.emplace< sonia::optional<syntax_expression_t> > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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
#line 2623 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 331 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2629 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 336 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2635 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 338 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2641 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 340 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2647 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 357 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2653 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 358 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2659 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 360 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2665 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 362 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2671 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 367 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), &ctx.push(std::move(yystack_[0].value.as < syntax_expression_t > ())) }; }
#line 2677 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 369 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2683 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 371 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2689 "bang.tab.cpp"
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
#line 2702 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 382 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2708 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 384 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2714 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 389 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2720 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 391 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2726 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: identifier
#line 396 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2732 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: "weak modifier" identifier
#line 398 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2738 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 402 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2744 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 404 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2750 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 409 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2756 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 411 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2762 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 413 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2768 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 418 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2774 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 419 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2780 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 421 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2786 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 423 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2792 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 428 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2798 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 433 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2804 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 435 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2810 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 437 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2816 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 439 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2822 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 441 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2828 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 443 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2834 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 445 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2840 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 447 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2846 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 449 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2852 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 454 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2858 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 456 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2864 "bang.tab.cpp"
    break;

  case 42: // generic-statement: LET let-decl
#line 461 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2870 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 463 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2876 "bang.tab.cpp"
    break;

  case 44: // generic-statement: compound-expression
#line 465 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2882 "bang.tab.cpp"
    break;

  case 45: // generic-statement: syntax-expression "`=`" syntax-expression
#line 467 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2888 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: generic-statement
#line 472 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2894 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: "`break`"
#line 474 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2900 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`continue`"
#line 476 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2906 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`return`"
#line 478 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .location = std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2912 "bang.tab.cpp"
    break;

  case 50: // infunction-statement: "`return`" syntax-expression
#line 480 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2918 "bang.tab.cpp"
    break;

  case 51: // identifier: IDENTIFIER
#line 485 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2924 "bang.tab.cpp"
    break;

  case 52: // internal-identifier: CONTEXT_IDENTIFIER
#line 493 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2930 "bang.tab.cpp"
    break;

  case 53: // qname: "`::`" identifier
#line 508 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2936 "bang.tab.cpp"
    break;

  case 54: // qname: identifier
#line 510 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2942 "bang.tab.cpp"
    break;

  case 55: // qname: qname "`::`" identifier
#line 512 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2948 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: "`fn`"
#line 518 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2954 "bang.tab.cpp"
    break;

  case 57: // fn-start-decl: INLINE "`fn`"
#line 520 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2960 "bang.tab.cpp"
    break;

  case 58: // fn-name: qname
#line 524 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2966 "bang.tab.cpp"
    break;

  case 59: // fn-name: "`new`"
#line 525 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2972 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 530 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2978 "bang.tab.cpp"
    break;

  case 61: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`~>`" type-expr
#line 532 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2984 "bang.tab.cpp"
    break;

  case 62: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" pattern
#line 534 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2990 "bang.tab.cpp"
    break;

  case 63: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 540 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2996 "bang.tab.cpp"
    break;

  case 64: // case-list-opt: %empty
#line 544 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 3002 "bang.tab.cpp"
    break;

  case 65: // case-list-opt: case-list
#line 545 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 3008 "bang.tab.cpp"
    break;

  case 66: // case-list: case-decl
#line 550 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 3014 "bang.tab.cpp"
    break;

  case 67: // case-list: case-list "," case-decl
#line 552 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 3020 "bang.tab.cpp"
    break;

  case 68: // case-decl: identifier
#line 557 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 3026 "bang.tab.cpp"
    break;

  case 69: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 563 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3032 "bang.tab.cpp"
    break;

  case 70: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 565 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3038 "bang.tab.cpp"
    break;

  case 71: // using-decl: qname "`=>`" syntax-expression
#line 588 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 3046 "bang.tab.cpp"
    break;

  case 72: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 592 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3054 "bang.tab.cpp"
    break;

  case 73: // expression-list: syntax-expression
#line 606 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3060 "bang.tab.cpp"
    break;

  case 74: // expression-list: expression-list "," syntax-expression
#line 608 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3066 "bang.tab.cpp"
    break;

  case 75: // argument-list-opt: %empty
#line 613 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 3072 "bang.tab.cpp"
    break;

  case 76: // argument-list-opt: argument-list
#line 614 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 3078 "bang.tab.cpp"
    break;

  case 77: // argument-list: argument
#line 619 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 3084 "bang.tab.cpp"
    break;

  case 78: // argument-list: argument-list "," argument
#line 621 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 3090 "bang.tab.cpp"
    break;

  case 79: // argument: argument-name syntax-expression
#line 626 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3096 "bang.tab.cpp"
    break;

  case 80: // argument: syntax-expression
#line 628 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3102 "bang.tab.cpp"
    break;

  case 81: // argument-name: identifier "`=`"
#line 633 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3108 "bang.tab.cpp"
    break;

  case 82: // argument-name: identifier "`:`"
#line 635 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 3114 "bang.tab.cpp"
    break;

  case 83: // field-list-opt: %empty
#line 640 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 3120 "bang.tab.cpp"
    break;

  case 84: // field-list-opt: field-list
#line 641 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 3126 "bang.tab.cpp"
    break;

  case 85: // field-list: field
#line 646 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 3132 "bang.tab.cpp"
    break;

  case 86: // field-list: field-list "," field
#line 648 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 3138 "bang.tab.cpp"
    break;

  case 87: // field-default-value-opt: %empty
#line 652 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3144 "bang.tab.cpp"
    break;

  case 88: // field-default-value-opt: "`=`" syntax-expression
#line 653 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3150 "bang.tab.cpp"
    break;

  case 89: // field: identifier "`:`" type-expr field-default-value-opt
#line 658 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3156 "bang.tab.cpp"
    break;

  case 90: // field: identifier "`=>`" type-expr field-default-value-opt
#line 660 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3162 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 670 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3168 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 671 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3174 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 676 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3180 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 678 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3186 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 682 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3192 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 683 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3198 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 687 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3204 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 688 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3210 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" pattern parameter-default-value-opt
#line 693 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3216 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: internal-identifier "`:`" pattern parameter-default-value-opt
#line 695 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3222 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: pattern parameter-default-value-opt
#line 697 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{}, .constraint = std::move(yystack_[1].value.as < pattern_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3228 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`:`" constraint-expression parameter-default-value-opt
#line 699 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3234 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: internal-identifier "`:`" constraint-expression parameter-default-value-opt
#line 701 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3240 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: constraint-expression parameter-default-value-opt
#line 703 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ .name = unnamed_parameter_name{ }, .constraint = std::move(yystack_[1].value.as < constraint_expression_t > ()), .value = std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3246 "bang.tab.cpp"
    break;

  case 105: // constraint-expression: "`~`" type-expr
#line 708 "bang.y"
        { yylhs.value.as < constraint_expression_t > () = constraint_expression_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3252 "bang.tab.cpp"
    break;

  case 106: // constraint-expression: "`~`" type-expr "`...`"
#line 710 "bang.y"
        { yylhs.value.as < constraint_expression_t > () = constraint_expression_t{ .expression = std::move(yystack_[1].value.as < syntax_expression_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3258 "bang.tab.cpp"
    break;

  case 107: // subpatterns-opt: %empty
#line 834 "bang.y"
      { yylhs.value.as < pattern_list_t > () = pattern_list_t{}; }
#line 3264 "bang.tab.cpp"
    break;

  case 108: // subpatterns-opt: subpatterns
#line 835 "bang.y"
      { yylhs.value.as < pattern_list_t > () = yystack_[0].value.as < pattern_list_t > (); }
#line 3270 "bang.tab.cpp"
    break;

  case 109: // subpatterns: "`(`" pattern-list "`)`"
#line 840 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[1].value.as < pattern_list_t > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3276 "bang.tab.cpp"
    break;

  case 110: // pattern-list: pattern-field
#line 845 "bang.y"
        { yylhs.value.as < pattern_list_t > () = pattern_list_t{ std::move(yystack_[0].value.as < pattern_t::field > ()) }; }
#line 3282 "bang.tab.cpp"
    break;

  case 111: // pattern-list: pattern-list "," pattern-field
#line 847 "bang.y"
        { yylhs.value.as < pattern_list_t > () = std::move(yystack_[2].value.as < pattern_list_t > ()); yylhs.value.as < pattern_list_t > ().emplace_back(std::move(yystack_[0].value.as < pattern_t::field > ())); }
#line 3288 "bang.tab.cpp"
    break;

  case 112: // pattern-field: pattern
#line 852 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = nullptr, .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3294 "bang.tab.cpp"
    break;

  case 113: // pattern-field: "`_`" "`:`" pattern
#line 854 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3300 "bang.tab.cpp"
    break;

  case 114: // pattern-field: identifier "`:`" pattern
#line 856 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3306 "bang.tab.cpp"
    break;

  case 115: // pattern-field: internal-identifier "`:`" pattern
#line 858 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; }
#line 3312 "bang.tab.cpp"
    break;

  case 116: // pattern-field: "`(`" syntax-expression "`)`" "`:`" pattern
#line 860 "bang.y"
        { yylhs.value.as < pattern_t::field > () = pattern_t::field{ .name = std::move(yystack_[3].value.as < syntax_expression_t > ()), .value = std::move(yystack_[0].value.as < pattern_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 3318 "bang.tab.cpp"
    break;

  case 117: // pattern: qname subpatterns-opt concept-expression-list-opt
#line 865 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3324 "bang.tab.cpp"
    break;

  case 118: // pattern: qname subpatterns-opt "`...`" concept-expression-list-opt
#line 867 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .fields = std::move(yystack_[2].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3330 "bang.tab.cpp"
    break;

  case 119: // pattern: internal-identifier concept-expression-list-opt
#line 869 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3336 "bang.tab.cpp"
    break;

  case 120: // pattern: internal-identifier "`...`" concept-expression-list-opt
#line 871 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = context_identifier{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3342 "bang.tab.cpp"
    break;

  case 121: // pattern: "`_`" subpatterns concept-expression-list-opt
#line 873 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = pattern_t::signature_descriptor{ .name = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .fields = std::move(yystack_[1].value.as < pattern_list_t > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3348 "bang.tab.cpp"
    break;

  case 122: // pattern: "`(`" syntax-expression "`)`" concept-expression-list-opt
#line 875 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[2].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3354 "bang.tab.cpp"
    break;

  case 123: // pattern: "`(`" syntax-expression "`)`" "`...`" concept-expression-list-opt
#line 877 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = std::move(yystack_[3].value.as < syntax_expression_t > ()), .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3360 "bang.tab.cpp"
    break;

  case 124: // pattern: "`_`" concept-expression-list-opt
#line 879 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3366 "bang.tab.cpp"
    break;

  case 125: // pattern: concept-expression-list
#line 881 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()) }; }
#line 3372 "bang.tab.cpp"
    break;

  case 126: // pattern: "`_`" "`...`" concept-expression-list-opt
#line 883 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3378 "bang.tab.cpp"
    break;

  case 127: // pattern: "`...`" concept-expression-list-opt
#line 885 "bang.y"
        { yylhs.value.as < pattern_t > () = pattern_t{ .descriptor = placeholder{}, .concepts = std::move(yystack_[0].value.as < concept_expression_list_t > ()), .ellipsis = true }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3384 "bang.tab.cpp"
    break;

  case 128: // concept-expression: "`@`" qname
#line 890 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_reference{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3390 "bang.tab.cpp"
    break;

  case 129: // concept-expression-list-opt: %empty
#line 895 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = {}; }
#line 3396 "bang.tab.cpp"
    break;

  case 130: // concept-expression-list-opt: concept-expression-list
#line 896 "bang.y"
      { yylhs.value.as < concept_expression_list_t > () = yystack_[0].value.as < concept_expression_list_t > (); }
#line 3402 "bang.tab.cpp"
    break;

  case 131: // concept-expression-list: concept-expression
#line 901 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = concept_expression_list_t{std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3408 "bang.tab.cpp"
    break;

  case 132: // concept-expression-list: concept-expression-list concept-expression
#line 903 "bang.y"
        { yylhs.value.as < concept_expression_list_t > () = std::move(yystack_[1].value.as < concept_expression_list_t > ()); yylhs.value.as < concept_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3414 "bang.tab.cpp"
    break;

  case 133: // syntax-expression: CONTEXT_IDENTIFIER
#line 911 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3420 "bang.tab.cpp"
    break;

  case 134: // syntax-expression: syntax-expression-wo-ii
#line 912 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3426 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: "nil"
#line 918 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3432 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: "true"
#line 920 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3438 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: "false"
#line 922 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3444 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: INTEGER
#line 924 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3450 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: DECIMAL
#line 926 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3456 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: DECIMAL_S
#line 928 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3462 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: INTEGER_INDEX
#line 930 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3468 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: STRING
#line 932 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3474 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 934 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3480 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: qname
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3486 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 938 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                BOOST_ASSERT(!yystack_[1].value.as < named_expression_list_t > ().empty());
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3499 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 947 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3511 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 955 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3517 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 957 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3523 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: "`.`" identifier
#line 959 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3529 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 963 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3535 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 965 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3541 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 967 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3547 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 970 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3553 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 972 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3559 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 977 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3565 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 979 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3571 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 983 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3577 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 985 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3583 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: syntax-expression "`..`" syntax-expression
#line 987 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3589 "bang.tab.cpp"
    break;

  case 160: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 990 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3595 "bang.tab.cpp"
    break;

  case 161: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 994 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3601 "bang.tab.cpp"
    break;

  case 162: // syntax-expression-wo-ii: syntax-expression "`&&`" syntax-expression
#line 997 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3607 "bang.tab.cpp"
    break;

  case 163: // syntax-expression-wo-ii: syntax-expression "`||`" syntax-expression
#line 1000 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3613 "bang.tab.cpp"
    break;

  case 164: // syntax-expression-wo-ii: apostrophe-expression
#line 1002 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3619 "bang.tab.cpp"
    break;

  case 165: // syntax-expression-wo-ii: new-expression
#line 1003 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3625 "bang.tab.cpp"
    break;

  case 166: // syntax-expression-wo-ii: compound-expression
#line 1004 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3631 "bang.tab.cpp"
    break;

  case 167: // syntax-expression-wo-ii: lambda-expression
#line 1005 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3637 "bang.tab.cpp"
    break;

  case 168: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 1010 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3643 "bang.tab.cpp"
    break;

  case 169: // new-expression: "`new`" qname
#line 1015 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3649 "bang.tab.cpp"
    break;

  case 170: // new-expression: "`new`" apostrophe-expression
#line 1017 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3655 "bang.tab.cpp"
    break;

  case 171: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 1019 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3661 "bang.tab.cpp"
    break;

  case 172: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 1021 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3667 "bang.tab.cpp"
    break;

  case 173: // call-expression: qname "`(`" pack-expression "`)`"
#line 1026 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3673 "bang.tab.cpp"
    break;

  case 174: // call-expression: "mut modifier" "`(`" pack-expression "`)`"
#line 1028 "bang.y"
        { 
            auto aid = ctx.make_identifier(annotated_string_view{ "mut"sv, std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()) });
            yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{ qname{ aid.value, true }, std::move(aid.location) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
        }
#line 3682 "bang.tab.cpp"
    break;

  case 175: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 1033 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3688 "bang.tab.cpp"
    break;

  case 176: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 1035 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3694 "bang.tab.cpp"
    break;

  case 177: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 1037 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3700 "bang.tab.cpp"
    break;

  case 178: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 1042 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3706 "bang.tab.cpp"
    break;

  case 179: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 1044 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3712 "bang.tab.cpp"
    break;

  case 180: // pack-expression: syntax-expression
#line 1049 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{ named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3718 "bang.tab.cpp"
    break;

  case 181: // pack-expression: identifier "`:`" syntax-expression
#line 1058 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3728 "bang.tab.cpp"
    break;

  case 182: // pack-expression: pack-expression "," syntax-expression
#line 1064 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3737 "bang.tab.cpp"
    break;

  case 183: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 1069 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3746 "bang.tab.cpp"
    break;

  case 184: // compound-expression: syntax-expression "`...`"
#line 1077 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3752 "bang.tab.cpp"
    break;

  case 185: // compound-expression: call-expression
#line 1078 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3758 "bang.tab.cpp"
    break;

  case 186: // type-expr: qname
#line 1165 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3764 "bang.tab.cpp"
    break;

  case 187: // type-expr: call-expression
#line 1166 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3770 "bang.tab.cpp"
    break;

  case 188: // type-expr: CONTEXT_IDENTIFIER
#line 1168 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3776 "bang.tab.cpp"
    break;

  case 189: // type-expr: "`[`" type-expr "`]`"
#line 1172 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3782 "bang.tab.cpp"
    break;

  case 190: // type-expr: "`(`" "`)`"
#line 1174 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3788 "bang.tab.cpp"
    break;

  case 191: // type-expr: "`(`" pack-expression "`)`"
#line 1177 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3794 "bang.tab.cpp"
    break;

  case 192: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1179 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3800 "bang.tab.cpp"
    break;

  case 193: // type-expr: type-expr "`|`" type-expr
#line 1181 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3812 "bang.tab.cpp"
    break;

  case 194: // type-expr: type-expr "`->`" type-expr
#line 1189 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 3821 "bang.tab.cpp"
    break;


#line 3825 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -226;

  const signed char parser::yytable_ninf_ = -96;

  const short
  parser::yypact_[] =
  {
     781,  -226,  -226,  -226,  -226,  -226,  -226,  -226,  -226,   881,
      44,   881,   881,   881,    44,   881,   881,    -5,    82,   108,
     881,   881,   881,    74,    47,    64,  -226,    74,    74,   114,
    -226,  -226,  -226,   219,    19,   487,  -226,  -226,  -226,  -226,
     137,     2,   687,  -226,   160,  -226,   177,   179,     6,   183,
     890,  -226,  -226,   198,  1022,   -22,   207,  1022,   211,  -226,
      16,    16,    44,  -226,   204,   255,  -226,    44,    12,   250,
     239,   590,   491,   771,    14,  -226,  -226,  -226,   235,  -226,
     205,  -226,   881,  -226,  -226,  -226,  -226,  -226,  -226,  -226,
      44,   881,   395,  -226,   260,   262,   264,  -226,   881,   881,
     881,   881,   881,   881,  -226,   881,    98,   881,   881,   881,
     881,   881,   881,   881,  -226,   881,  -226,   881,  -226,   881,
    -226,  -226,   412,   881,   266,   269,  -226,   881,   881,   683,
     881,  -226,   881,   236,   881,   395,    44,   272,   395,  -226,
      37,  -226,   123,  -226,    74,   115,   308,   881,   412,    36,
     125,   257,   278,   259,  -226,   317,   317,  -226,   308,   395,
    -226,  1022,    35,    35,  1069,  1059,    31,   934,  -226,  -226,
      16,    16,   455,   422,   141,   164,   186,  1022,   291,  1022,
    1022,  -226,   802,   412,   137,   160,   177,   179,    99,   271,
     412,   395,   129,   294,   275,  -226,   881,  1022,   299,  -226,
    -226,   881,    74,   292,   585,  -226,   298,  -226,  -226,     2,
     771,   771,   264,  1022,   304,  -226,   303,   285,  -226,    44,
     309,  -226,  -226,   260,   308,   512,   308,  -226,   308,  -226,
     975,    32,  -226,   313,   308,   395,  -226,   174,  -226,   119,
     395,   881,  -226,  -226,  -226,   315,  -226,  -226,  -226,  -226,
     881,  -226,   187,    60,   412,   356,   412,    99,   321,  -226,
    -226,  -226,   881,  1022,  -226,  1022,   224,  -226,  -226,  -226,
    -226,  -226,  -226,  -226,  -226,   351,  -226,    44,   113,   327,
     310,  -226,    10,  -226,   193,   881,   336,   157,   194,  -226,
    -226,  -226,   184,  -226,   395,  -226,   241,   317,   317,   308,
    -226,   412,  -226,  -226,  1022,   280,  1022,  -226,  -226,  -226,
     331,    65,   258,  -226,   395,   881,  -226,   412,   412,  -226,
      44,   337,  -226,   568,  1012,   568,   568,  -226,   512,   308,
    -226,   317,   317,  -226,  -226,  -226,   102,   568,   881,   412,
    -226,   339,  1022,    22,    22,  -226,    44,  -226,   169,  -226,
    -226,  -226,  -226,  -226,  -226,  -226,  -226,  1022,    99,   264,
     881,  -226,  -226,   341,   568,  1022,  -226,  -226
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   142,    51,   133,   143,   141,   138,   139,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
     135,   136,   137,     0,     0,     0,     5,     7,    15,    54,
     144,     0,     0,   134,   164,   165,   185,   167,   166,     0,
       0,   166,    53,    54,   180,     0,     0,    73,     0,   149,
     154,   153,     0,    42,    21,    17,    19,     0,     0,   169,
     170,     0,     0,     0,     0,    43,    13,    57,     0,    39,
       0,    16,     0,     1,     2,    10,     3,     8,     6,     9,
       0,     0,    91,    59,    58,     0,     0,   152,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   168,     0,   145,     0,   146,     0,
     147,    20,     0,     0,     0,     0,    12,    75,    75,    23,
       0,    31,     0,    34,     0,    91,    64,     0,    91,    37,
       0,    55,     0,    52,     0,   129,   129,     0,     0,    54,
     129,   107,     0,    92,    93,    97,    97,   131,   125,    91,
      36,    45,   157,   158,   162,   163,   159,     0,   150,   151,
     155,   156,   160,   161,     0,     0,     0,   181,    54,   182,
      74,   188,     0,     0,   186,     0,   187,     0,    22,    18,
       0,    91,    54,     0,    76,    77,     0,    80,     0,    48,
      47,    49,     0,    40,    41,    26,     0,    46,    24,     0,
       0,     0,     0,    71,     0,    68,     0,    65,    66,    83,
       0,   174,   173,   128,   129,     0,   129,   124,   130,   127,
       0,   105,    96,     0,   129,     0,   119,   129,   108,     0,
       0,     0,   104,   101,   132,     0,   148,   176,   175,   177,
       0,   190,     0,     0,     0,     0,     0,    11,     0,    81,
      82,   171,     0,    79,   172,    50,     0,    29,    27,    28,
      25,    30,    32,    33,    35,     0,    63,     0,     0,     0,
      84,    85,     0,   126,   129,     0,    54,   129,     0,   110,
     112,   121,   129,   106,     0,   120,   129,    97,    97,   129,
     117,     0,   178,    94,    98,    60,   183,   191,   189,   194,
       0,   193,    60,    78,    91,     0,    67,     0,     0,    69,
       0,     0,    38,     0,     0,     0,     0,   109,     0,   129,
     122,    97,    97,   103,   100,   118,     0,     0,     0,     0,
     192,     0,    72,    87,    87,    86,    83,   113,   129,   114,
     115,   111,   123,   102,    99,   179,    62,    14,    61,     0,
       0,    90,    89,     0,     0,    88,    70,   116
  };

  const short
  parser::yypgoto_[] =
  {
    -226,  -226,  -226,  -226,   355,  -226,  -226,  -226,  -226,  -226,
     -34,    -7,  -226,  -127,   188,   155,    39,     4,     7,   350,
     325,  -226,  -226,  -226,   117,  -226,  -226,   384,   270,  -226,
     138,  -226,    59,  -226,    63,    88,  -121,  -226,  -226,  -152,
     170,  -225,  -226,   263,  -226,    81,   110,  -153,   162,    86,
       0,  -226,    38,  -226,  -119,    33,   -58,    11,   105
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    33,    34,    35,    36,    63,    64,    65,   203,   204,
     139,    37,   206,    38,   208,    39,   150,    40,    49,   125,
      96,    79,   216,   217,   218,    81,    75,    56,   193,   194,
     195,   196,   279,   280,   361,   281,   152,   153,   233,   242,
     154,   155,   237,   226,   288,   289,   156,   157,   236,   158,
      54,    43,    44,    45,    46,    47,    55,    51,   188
  };

  const short
  parser::yytable_[] =
  {
      42,     2,   207,   186,   243,   244,   -44,    41,     2,    50,
     297,    48,    57,    57,   214,    60,    61,   220,     2,    84,
      71,    72,    73,    69,   140,    97,   116,    74,    89,   186,
     321,    78,    80,   142,   134,    42,   360,   131,   245,   133,
      97,   254,    41,   143,    97,    94,    48,   117,    10,    92,
       2,   254,    76,   174,   175,   176,   -44,    70,    10,   129,
      90,   135,   160,   -44,   186,   189,   293,   105,   103,   331,
     258,   186,    94,   255,   106,   244,    85,   207,    93,   254,
       2,   -95,   105,   255,   254,   221,   105,   256,    93,   106,
     107,   108,    62,   106,   107,   108,   151,   256,   161,   162,
     163,   164,   165,   166,     2,   167,   117,   170,   171,   172,
     173,   255,   308,     9,     2,   177,   255,   179,   254,   180,
      10,   254,   205,     9,   252,   256,   184,   197,   197,    42,
     210,   144,   211,   317,   213,   186,   209,   186,   301,   151,
      48,   144,   151,   259,   169,   333,   334,   230,   223,   224,
     255,   129,   184,   255,    10,   187,    67,    77,   318,   234,
     185,    82,   225,   151,   256,    52,    53,   256,   129,    59,
     235,   222,    66,   144,   260,    68,   272,   273,   274,   353,
     354,   187,   186,    90,    91,   144,   185,   184,   232,   247,
     144,   234,   117,   341,   184,   151,   263,   269,   186,   186,
     144,   265,   326,   329,    42,   302,   266,   111,   299,   144,
     117,   209,   248,    94,   364,    48,   187,   121,   329,    83,
     186,   185,   124,   187,   112,   137,   113,   224,   185,   151,
      92,   228,   228,   117,   249,   307,   228,    53,   323,   151,
     225,   304,   327,   115,   151,   141,    53,   149,   322,   122,
     306,    90,   138,   231,   129,   117,   117,   144,   184,   118,
     184,   168,   197,   328,   287,   120,    53,    53,    53,   123,
      90,   314,   178,   129,   296,   234,   119,   337,    53,   339,
     119,    90,   192,   192,   136,   324,   128,   187,   253,   187,
     149,   215,   185,   149,   185,   257,    90,   127,   151,   337,
     338,   339,   355,    90,   225,   184,    90,   227,   229,   159,
     228,   190,   228,   129,   149,   342,   191,   212,   151,   219,
     228,   184,   184,   228,   144,   322,   239,   151,   240,   151,
     151,   241,   151,   296,   187,   290,   250,    53,   357,   185,
     117,   151,   261,   184,   262,   298,   149,   264,   271,   267,
     187,   187,   275,   276,   277,   185,   185,   282,   294,   309,
     365,   311,   296,   305,   296,   296,   310,   287,   151,   312,
     228,   315,   187,   228,   278,   319,   296,   185,   228,   320,
     286,   325,   228,   340,   346,   228,   283,   359,   291,   366,
      88,    95,   270,   126,   316,   149,   295,    58,   198,   300,
     313,     2,   143,   296,   332,   363,   336,   362,   345,   351,
     303,   144,   145,     0,   238,   228,     0,   192,     2,   181,
       0,     0,   343,   344,     0,     0,     0,     9,     0,   146,
       0,    97,   215,   347,   228,   349,   350,     0,   290,     0,
       0,    10,   147,     0,   358,     0,   227,   356,     0,     0,
       0,    99,   100,     0,   330,   103,     0,     0,    10,   182,
       0,   335,   148,   183,    97,     0,     0,     0,     0,   149,
       0,     0,     0,   105,   367,   278,     0,     0,     0,     0,
     106,   107,   108,   286,    99,   100,   109,    86,   103,     0,
       0,   352,     1,     2,     3,     4,     5,     6,     7,     8,
      97,   278,     9,     0,    25,    26,   105,     0,     0,     0,
     330,    29,     0,   106,   107,   108,     0,     0,     2,   143,
      99,   100,   101,   102,   103,   104,     0,     0,   144,   284,
       0,     0,     0,    10,    11,     0,     0,     0,    12,     0,
      13,     0,   105,     0,    87,    14,   146,     0,    15,   106,
     107,   108,     0,    16,     0,   109,   110,     0,    10,   285,
      17,     0,    18,    19,    20,    21,     0,    22,     0,     0,
     132,     0,     0,    23,     2,   143,     0,     0,    24,    25,
      26,    27,    28,     0,   144,   145,    29,    30,    31,    32,
       1,     2,     3,     4,     5,     6,     7,     8,     0,    97,
       9,     0,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,   147,     0,     0,     0,    99,
     100,   101,   102,   103,   104,     0,     0,     0,     0,     0,
       0,    10,    11,     0,     0,     0,    12,     0,    13,   129,
       0,   105,   268,    14,     0,     0,    15,   130,   106,   107,
     108,    16,     0,     0,   109,   110,     0,     0,    17,     0,
       0,    19,    20,    21,     0,    22,     0,   199,   200,   201,
       0,    23,     0,     0,     0,     0,     0,    25,    26,    27,
     202,     0,     0,     0,    29,    30,    31,    32,     1,     2,
       3,     4,     5,     6,     7,     8,    97,     0,     9,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,   100,   101,   102,
     103,   104,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,     0,     0,    12,     0,    13,     0,   105,     0,
       0,    14,     0,     0,    15,   106,   107,   108,     0,    16,
       0,   109,   110,     0,     0,     0,    17,     0,     0,    19,
      20,    21,     0,    22,     0,   199,   200,   201,     0,    23,
       0,     0,     0,     0,     0,    25,    26,    27,   202,     0,
      97,     0,    29,    30,    31,    32,     1,     2,     3,     4,
       5,     6,     7,     8,     0,     0,     9,     0,     0,     0,
      99,   100,   101,   102,   103,   104,     0,     1,     2,     3,
       4,     5,     6,     7,     8,     0,     0,     9,     0,     0,
     129,     0,   105,     0,     0,     0,     0,    10,    11,   106,
     107,   108,    12,     0,    13,   109,   110,     0,     0,    14,
       0,     0,    15,     0,     0,     0,     0,    16,    10,    11,
     251,     0,     0,    12,    17,    13,    18,    19,    20,    21,
      14,    22,     0,    15,     0,     0,     0,    23,    16,     0,
       0,     0,    24,    25,    26,    27,    28,     0,    19,     0,
      29,    30,    31,    32,     0,     0,     1,     2,     3,     4,
       5,     6,     7,     8,    25,    26,     9,     0,     0,    97,
       0,    29,    30,    31,    32,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
     100,   101,   102,   103,   104,     0,     0,    10,    11,     0,
       0,     0,    12,     0,    13,     0,     0,     0,     0,    14,
       0,   105,    15,    97,     0,     0,     0,    16,   106,   107,
     108,     0,     0,     0,   109,   110,     0,    19,     0,     0,
       0,     0,     0,    99,   100,   101,   102,   103,   104,     0,
       0,     0,     0,    25,    26,     0,     0,     0,     0,     0,
      29,    30,    31,    32,    97,   105,   246,     0,     0,     0,
       0,     0,   106,   107,   108,     0,     0,     0,   109,   110,
       0,     0,     0,     0,    99,   100,   101,   102,   103,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,   292,     0,     0,   105,     0,     0,     0,
       0,    97,     0,   106,   107,   108,     0,     0,     0,   109,
     110,    99,   100,   101,   102,   103,   104,     0,     0,     0,
       0,    99,   100,   101,   102,   103,   104,     0,     0,     0,
     348,     0,     0,   105,     0,     0,     0,     0,    97,     0,
     106,   107,   108,   105,     0,     0,   109,   110,    97,     0,
     106,   107,   108,     0,     0,     0,   109,   110,    99,   100,
     101,     0,   103,     0,     0,     0,     0,     0,    99,   100,
       0,     0,   103,     0,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,   106,   107,   108,
     105,     0,     0,   109,   110,     0,     0,   106,   107,   108,
       0,     0,     0,   109,   110
  };

  const short
  parser::yycheck_[] =
  {
       0,     6,   129,   122,   156,   158,     0,     0,     6,     9,
     235,     0,    12,    13,   135,    15,    16,   138,     6,     0,
      20,    21,    22,    19,    82,     9,    48,    23,    35,   148,
      20,    27,    28,    91,    20,    35,    14,    71,   159,    73,
       9,    19,    35,     7,     9,    41,    35,    69,    46,    47,
       6,    19,     5,   111,   112,   113,    50,    19,    46,    49,
      46,    47,    96,    57,   183,   123,    34,    51,    33,   294,
     191,   190,    68,    51,    58,   228,    57,   204,    76,    19,
       6,    45,    51,    51,    19,    48,    51,    65,    76,    58,
      59,    60,    97,    58,    59,    60,    92,    65,    98,    99,
     100,   101,   102,   103,     6,   105,    69,   107,   108,   109,
     110,    51,    52,    15,     6,   115,    51,   117,    19,   119,
      46,    19,   129,    15,   182,    65,   122,   127,   128,   129,
     130,    16,   132,    20,   134,   254,   129,   256,    19,   135,
     129,    16,   138,    14,   106,   297,   298,   147,   144,    34,
      51,    49,   148,    51,    46,   122,    74,    93,    45,    34,
     122,    47,    47,   159,    65,    10,    11,    65,    49,    14,
      45,    48,    17,    16,    45,    93,   210,   211,   212,   331,
     332,   148,   301,    46,    47,    16,   148,   183,   149,    48,
      16,    34,    69,   314,   190,   191,   196,   204,   317,   318,
      16,   201,    45,    34,   204,   239,   202,    47,    34,    16,
      69,   204,    48,   209,    45,   204,   183,    62,    34,     0,
     339,   183,    67,   190,    47,    20,    47,    34,   190,   225,
      47,   145,   146,    69,    48,    48,   150,    82,    45,   235,
      47,   241,    48,    45,   240,    90,    91,    92,   282,    45,
     250,    46,    47,   148,    49,    69,    69,    16,   254,    52,
     256,   106,   262,    69,   225,    54,   111,   112,   113,    14,
      46,    47,   117,    49,   235,    34,    69,    19,   123,    21,
      69,    46,   127,   128,    49,   285,    47,   254,   183,   256,
     135,   136,   254,   138,   256,   190,    46,    47,   294,    19,
      20,    21,   336,    46,    47,   301,    46,   145,   146,    47,
     224,    45,   226,    49,   159,   315,    47,    81,   314,    47,
     234,   317,   318,   237,    16,   359,    48,   323,    69,   325,
     326,    14,   328,   294,   301,   225,    45,   182,   338,   301,
      69,   337,    48,   339,    69,   235,   191,    48,    50,    57,
     317,   318,    48,    50,    69,   317,   318,    48,    45,   254,
     360,   256,   323,    48,   325,   326,    10,   328,   364,    48,
     284,    20,   339,   287,   219,    48,   337,   339,   292,    69,
     225,    45,   296,    52,    47,   299,   224,    48,   226,    48,
      35,    41,   204,    68,   277,   240,   234,    13,   128,   237,
     262,     6,     7,   364,   294,   346,   301,   344,   320,   328,
     240,    16,    17,    -1,   151,   329,    -1,   262,     6,     7,
      -1,    -1,   317,   318,    -1,    -1,    -1,    15,    -1,    34,
      -1,     9,   277,   323,   348,   325,   326,    -1,   328,    -1,
      -1,    46,    47,    -1,   339,    -1,   284,   337,    -1,    -1,
      -1,    29,    30,    -1,   292,    33,    -1,    -1,    46,    47,
      -1,   299,    67,    51,     9,    -1,    -1,    -1,    -1,   314,
      -1,    -1,    -1,    51,   364,   320,    -1,    -1,    -1,    -1,
      58,    59,    60,   328,    29,    30,    64,     0,    33,    -1,
      -1,   329,     5,     6,     7,     8,     9,    10,    11,    12,
       9,   346,    15,    -1,    92,    93,    51,    -1,    -1,    -1,
     348,    99,    -1,    58,    59,    60,    -1,    -1,     6,     7,
      29,    30,    31,    32,    33,    34,    -1,    -1,    16,    17,
      -1,    -1,    -1,    46,    47,    -1,    -1,    -1,    51,    -1,
      53,    -1,    51,    -1,    57,    58,    34,    -1,    61,    58,
      59,    60,    -1,    66,    -1,    64,    65,    -1,    46,    47,
      73,    -1,    75,    76,    77,    78,    -1,    80,    -1,    -1,
      79,    -1,    -1,    86,     6,     7,    -1,    -1,    91,    92,
      93,    94,    95,    -1,    16,    17,    99,   100,   101,   102,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,     9,
      15,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    -1,    51,    -1,    53,    49,
      -1,    51,    57,    58,    -1,    -1,    61,    57,    58,    59,
      60,    66,    -1,    -1,    64,    65,    -1,    -1,    73,    -1,
      -1,    76,    77,    78,    -1,    80,    -1,    82,    83,    84,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    99,   100,   101,   102,     5,     6,
       7,     8,     9,    10,    11,    12,     9,    -1,    15,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    -1,    51,    -1,    53,    -1,    51,    -1,
      -1,    58,    -1,    -1,    61,    58,    59,    60,    -1,    66,
      -1,    64,    65,    -1,    -1,    -1,    73,    -1,    -1,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
       9,    -1,    99,   100,   101,   102,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    -1,    46,    47,    58,
      59,    60,    51,    -1,    53,    64,    65,    -1,    -1,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    66,    46,    47,
      48,    -1,    -1,    51,    73,    53,    75,    76,    77,    78,
      58,    80,    -1,    61,    -1,    -1,    -1,    86,    66,    -1,
      -1,    -1,    91,    92,    93,    94,    95,    -1,    76,    -1,
      99,   100,   101,   102,    -1,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    92,    93,    15,    -1,    -1,     9,
      -1,    99,   100,   101,   102,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    -1,    -1,    46,    47,    -1,
      -1,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    58,
      -1,    51,    61,     9,    -1,    -1,    -1,    66,    58,    59,
      60,    -1,    -1,    -1,    64,    65,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,     9,    51,    52,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      -1,     9,    -1,    58,    59,    60,    -1,    -1,    -1,    64,
      65,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    -1,     9,    -1,
      58,    59,    60,    51,    -1,    -1,    64,    65,     9,    -1,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      51,    -1,    -1,    64,    65,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    51,    53,    58,    61,    66,    73,    75,    76,
      77,    78,    80,    86,    91,    92,    93,    94,    95,    99,
     100,   101,   102,   104,   105,   106,   107,   114,   116,   118,
     120,   121,   153,   154,   155,   156,   157,   158,   160,   121,
     153,   160,   118,   118,   153,   159,   130,   153,   130,   118,
     153,   153,    97,   108,   109,   110,   118,    74,    93,   120,
     155,   153,   153,   153,   120,   129,     5,    93,   120,   124,
     120,   128,    47,     0,     0,    57,     0,    57,   107,   114,
      46,    47,    47,    76,   120,   122,   123,     9,    14,    29,
      30,    31,    32,    33,    34,    51,    58,    59,    60,    64,
      65,    47,    47,    47,    15,    45,    48,    69,    52,    69,
      54,   118,    45,    14,   118,   122,   123,    47,    47,    49,
      57,   113,    79,   113,    20,    47,    49,    20,    47,   113,
     159,   118,   159,     7,    16,    17,    34,    47,    67,   118,
     119,   120,   139,   140,   143,   144,   149,   150,   152,    47,
     113,   153,   153,   153,   153,   153,   153,   153,   118,   155,
     153,   153,   153,   153,   159,   159,   159,   153,   118,   153,
     153,     7,    47,    51,   120,   155,   157,   158,   161,   159,
      45,    47,   118,   131,   132,   133,   134,   153,   131,    82,
      83,    84,    95,   111,   112,   114,   115,   116,   117,   121,
     153,   153,    81,   153,   139,   118,   125,   126,   127,    47,
     139,    48,    48,   120,    34,    47,   146,   151,   152,   151,
     153,   161,   119,   141,    34,    45,   151,   145,   146,    48,
      69,    14,   142,   142,   150,   139,    52,    48,    48,    48,
      45,    48,   159,   161,    19,    51,    65,   161,   139,    14,
      45,    48,    69,   153,    48,   153,   120,    57,    57,   114,
     117,    50,   113,   113,   113,    48,    50,    69,   118,   135,
     136,   138,    48,   151,    17,    47,   118,   119,   147,   148,
     149,   151,    48,    34,    45,   151,   119,   144,   149,    34,
     151,    19,   113,   143,   153,    48,   153,    48,    52,   161,
      10,   161,    48,   133,    47,    20,   127,    20,    45,    48,
      69,    20,   113,    45,   153,    45,    45,    48,    69,    34,
     151,   144,   149,   142,   142,   151,   161,    19,    20,    21,
      52,   139,   153,   161,   161,   138,    47,   149,    48,   149,
     149,   148,   151,   142,   142,   113,   149,   153,   161,    48,
      14,   137,   137,   135,    45,   153,    48,   149
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   103,   104,   104,   105,   105,   105,   106,   106,   106,
     106,   107,   107,   107,   107,   107,   107,   108,   108,   109,
     109,   110,   110,   111,   111,   111,   112,   112,   112,   112,
     113,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     115,   115,   116,   116,   116,   116,   117,   117,   117,   117,
     117,   118,   119,   120,   120,   120,   121,   121,   122,   122,
     123,   123,   123,   124,   125,   125,   126,   126,   127,   128,
     128,   129,   129,   130,   130,   131,   131,   132,   132,   133,
     133,   134,   134,   135,   135,   136,   136,   137,   137,   138,
     138,   139,   139,   140,   140,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   143,   144,   144,   145,   145,   146,
     147,   147,   148,   148,   148,   148,   148,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   150,   151,
     151,   152,   152,   153,   153,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   155,   156,
     156,   156,   156,   157,   157,   157,   157,   157,   158,   158,
     159,   159,   159,   159,   160,   160,   161,   161,   161,   161,
     161,   161,   161,   161,   161
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
       1,     1,     1,     1,     1,     3,     3,     3,     4,     2,
       3,     3,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     3,     2,
       2,     5,     5,     4,     4,     4,     4,     4,     5,     7,
       1,     3,     3,     5,     2,     1,     1,     1,     1,     3,
       2,     3,     4,     3,     3
  };


#if BANG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "CONTEXT_IDENTIFIER",
  "RESERVED_IDENTIFIER", "INTEGER_INDEX", "INTEGER", "DECIMAL",
  "DECIMAL_S", "OPERATOR_TERM", "\"`=`\"", "\"APOSTROPHE\"", "\"`@`\"",
  "\"`_`\"", "\"`->*`\"", "\"`->`\"", "\"`=>`\"", "\"`~>`\"", "\"`.*`\"",
  "\"`++`\"", "\"`--`\"", "\"`<<`\"", "\"`>>`\"", "\"`<=`\"", "\"`>=`\"",
  "\"`==`\"", "\"`!=`\"", "\"`&&`\"", "\"`||`\"", "\"`..`\"", "\"`...`\"",
  "\"`+=`\"", "\"`-=`\"", "\"`*=`\"", "\"`/=`\"", "\"`%=`\"", "\"`<<=`\"",
  "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"", "\"`^=`\"", "\"`:`\"", "\"`::`\"",
  "\"`(`\"", "\"`)`\"", "\"`{`\"", "\"`}`\"", "\"`[`\"", "\"`]`\"",
  "\"`[[`\"", "\"`]]`\"", "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"",
  "\"`+`\"", "\"`-`\"", "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"",
  "\"`|`\"", "\"`!`\"", "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"",
  "\"`?`\"", "\"`#`\"", "LET", "VAR", "EXTERN", "\"`new`\"", "\"`while`\"",
  "\"`for`\"", "\"`in`\"", "\"`if`\"", "\"`else`\"", "\"`continue`\"",
  "\"`break`\"", "\"`return`\"", "AUTO", "USING", "THROW", "SIZEOF",
  "LOWEST", "DEREF", "INCLUDE", "INLINE", "\"`fn`\"", "ENUM", "STRUCT",
  "EXTENDS", "\"weak modifier\"", "\"const modifier\"", "\"mut modifier\"",
  "\"nil\"", "\"true\"", "\"false\"", "$accept", "begin", "statement_any",
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
     896,   900,   902,   910,   912,   917,   919,   921,   923,   925,
     927,   929,   931,   933,   935,   937,   946,   954,   956,   958,
     962,   964,   966,   969,   971,   976,   978,   982,   984,   986,
     989,   993,   996,   999,  1002,  1003,  1004,  1005,  1009,  1014,
    1016,  1018,  1020,  1025,  1027,  1032,  1034,  1036,  1041,  1043,
    1048,  1057,  1063,  1068,  1076,  1078,  1164,  1166,  1167,  1171,
    1173,  1176,  1178,  1180,  1188
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
      95,    96,    97,    98,    99,   100,   101,   102
    };
    // Last valid token kind.
    const int code_max = 357;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4775 "bang.tab.cpp"

#line 1520 "bang.y"

