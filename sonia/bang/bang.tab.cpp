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
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
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
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
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

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_156_compound_expression: // compound-expression
      case symbol_kind::S_157_type_expr: // type-expr
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
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
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
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
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

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_156_compound_expression: // compound-expression
      case symbol_kind::S_157_type_expr: // type-expr
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
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
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
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
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

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_156_compound_expression: // compound-expression
      case symbol_kind::S_157_type_expr: // type-expr
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
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
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
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
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

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_156_compound_expression: // compound-expression
      case symbol_kind::S_157_type_expr: // type-expr
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
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
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
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
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

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_156_compound_expression: // compound-expression
      case symbol_kind::S_157_type_expr: // type-expr
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
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
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
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
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

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_156_compound_expression: // compound-expression
      case symbol_kind::S_157_type_expr: // type-expr
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
#line 324 "bang.y"
                 { }
#line 1500 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 324 "bang.y"
                 { }
#line 1506 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 324 "bang.y"
                 { }
#line 1512 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 324 "bang.y"
                 { }
#line 1518 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 324 "bang.y"
                 { }
#line 1524 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 324 "bang.y"
                 { }
#line 1530 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 324 "bang.y"
                 { }
#line 1536 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 324 "bang.y"
                 { }
#line 1542 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 324 "bang.y"
                 { }
#line 1548 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 324 "bang.y"
                 { }
#line 1554 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 324 "bang.y"
                 { }
#line 1560 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 324 "bang.y"
                 { }
#line 1566 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 324 "bang.y"
                 { }
#line 1572 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 324 "bang.y"
                 { }
#line 1578 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 324 "bang.y"
                 { }
#line 1584 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 324 "bang.y"
                 { }
#line 1590 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 324 "bang.y"
                 { }
#line 1596 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 324 "bang.y"
                 { }
#line 1602 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 324 "bang.y"
                 { }
#line 1608 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 324 "bang.y"
                 { }
#line 1614 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 324 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 324 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 324 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 324 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 324 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 324 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 324 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 324 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 324 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 324 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 324 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 324 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 324 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 324 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 324 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
#line 324 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 324 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_let_decl: // let-decl
#line 324 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl_start: // let-decl-start
#line 324 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 324 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
#line 324 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
#line 324 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_braced_statements: // braced-statements
#line 324 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
#line 324 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
#line 324 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
#line 324 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_infunction_statement: // infunction-statement
#line 324 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 324 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_internal_identifier: // internal-identifier
#line 324 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 324 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
#line 324 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_name: // fn-name
#line 324 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
#line 324 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
#line 324 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
#line 324 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list: // case-list
#line 324 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_decl: // case-decl
#line 324 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
#line 324 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_using_decl: // using-decl
#line 324 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_expression_list: // expression-list
#line 324 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
#line 324 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list: // argument-list
#line 324 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 324 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_argument_name: // argument-name
#line 324 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
#line 324 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list: // field-list
#line 324 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
#line 324 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 324 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
#line 324 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list: // parameter-list
#line 324 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
#line 324 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
#line 324 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
#line 324 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
#line 324 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
#line 324 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
#line 324 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
#line 324 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
#line 324 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_concept_expression: // concept-expression
#line 324 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_syntax_expression: // syntax-expression
#line 324 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 324 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
#line 324 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_new_expression: // new-expression
#line 324 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_call_expression: // call-expression
#line 324 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_lambda_expression: // lambda-expression
#line 324 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_compound_expression: // compound-expression
#line 324 "bang.y"
                 { }
#line 2010 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_type_expr: // type-expr
#line 324 "bang.y"
                 { }
#line 2016 "bang.tab.cpp"
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
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
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
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
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

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_156_compound_expression: // compound-expression
      case symbol_kind::S_157_type_expr: // type-expr
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
#line 329 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2448 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 330 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2454 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 335 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2460 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 337 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2466 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 339 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2472 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 356 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2478 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 357 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2484 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 359 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2490 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 361 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2496 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 366 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2502 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 368 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2508 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 370 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2514 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 376 "bang.y"
        {
            yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > ();
            auto sts = ctx.new_statement_list();
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) });
            yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), ctx.push(std::move(sts)) };
        }
#line 2525 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 383 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2531 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 385 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2537 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 390 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2543 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 392 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2549 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: LET identifier
#line 397 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2555 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: LET "weak modifier" identifier
#line 399 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2561 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 403 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2567 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 405 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2573 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 410 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2579 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 412 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2585 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 414 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2591 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 419 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2597 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 420 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2603 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 422 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2609 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 424 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2615 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 429 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2621 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 434 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2627 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 436 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2633 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 438 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2639 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 440 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2645 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 442 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2651 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 444 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2657 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 446 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2663 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 448 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2669 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 450 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2675 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 455 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2681 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 457 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2687 "bang.tab.cpp"
    break;

  case 42: // generic-statement: let-decl
#line 462 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2693 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 464 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2699 "bang.tab.cpp"
    break;

  case 44: // generic-statement: "`return`" syntax-expression
#line 466 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2705 "bang.tab.cpp"
    break;

  case 45: // generic-statement: compound-expression
#line 468 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2711 "bang.tab.cpp"
    break;

  case 46: // generic-statement: syntax-expression "`=`" syntax-expression
#line 470 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2717 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: generic-statement
#line 475 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2723 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`break`"
#line 477 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2729 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`continue`"
#line 479 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2735 "bang.tab.cpp"
    break;

  case 50: // identifier: IDENTIFIER
#line 484 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2741 "bang.tab.cpp"
    break;

  case 51: // internal-identifier: INTERNAL_IDENTIFIER
#line 492 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2747 "bang.tab.cpp"
    break;

  case 52: // qname: "`::`" identifier
#line 507 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2753 "bang.tab.cpp"
    break;

  case 53: // qname: identifier
#line 509 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2759 "bang.tab.cpp"
    break;

  case 54: // qname: qname "`::`" identifier
#line 511 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2765 "bang.tab.cpp"
    break;

  case 55: // fn-start-decl: "`fn`"
#line 517 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2771 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: INLINE "`fn`"
#line 519 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2777 "bang.tab.cpp"
    break;

  case 57: // fn-name: qname
#line 523 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2783 "bang.tab.cpp"
    break;

  case 58: // fn-name: "`new`"
#line 524 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2789 "bang.tab.cpp"
    break;

  case 59: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 529 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_list_t > ()), nullopt }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2795 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" type-expr
#line 531 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2801 "bang.tab.cpp"
    break;

  case 61: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 537 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2807 "bang.tab.cpp"
    break;

  case 62: // case-list-opt: %empty
#line 541 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2813 "bang.tab.cpp"
    break;

  case 63: // case-list-opt: case-list
#line 542 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2819 "bang.tab.cpp"
    break;

  case 64: // case-list: case-decl
#line 547 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2825 "bang.tab.cpp"
    break;

  case 65: // case-list: case-list "," case-decl
#line 549 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2831 "bang.tab.cpp"
    break;

  case 66: // case-decl: identifier
#line 554 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2837 "bang.tab.cpp"
    break;

  case 67: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 560 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2843 "bang.tab.cpp"
    break;

  case 68: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 562 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2849 "bang.tab.cpp"
    break;

  case 69: // using-decl: qname "`=>`" syntax-expression
#line 585 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 2857 "bang.tab.cpp"
    break;

  case 70: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 589 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 2865 "bang.tab.cpp"
    break;

  case 71: // expression-list: syntax-expression
#line 603 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2871 "bang.tab.cpp"
    break;

  case 72: // expression-list: expression-list "," syntax-expression
#line 605 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2877 "bang.tab.cpp"
    break;

  case 73: // argument-list-opt: %empty
#line 610 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2883 "bang.tab.cpp"
    break;

  case 74: // argument-list-opt: argument-list
#line 611 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2889 "bang.tab.cpp"
    break;

  case 75: // argument-list: argument
#line 616 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2895 "bang.tab.cpp"
    break;

  case 76: // argument-list: argument-list "," argument
#line 618 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2901 "bang.tab.cpp"
    break;

  case 77: // argument: argument-name syntax-expression
#line 623 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2907 "bang.tab.cpp"
    break;

  case 78: // argument: syntax-expression
#line 625 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2913 "bang.tab.cpp"
    break;

  case 79: // argument-name: identifier "`=`"
#line 630 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2919 "bang.tab.cpp"
    break;

  case 80: // argument-name: identifier "`:`"
#line 632 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2925 "bang.tab.cpp"
    break;

  case 81: // field-list-opt: %empty
#line 637 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2931 "bang.tab.cpp"
    break;

  case 82: // field-list-opt: field-list
#line 638 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2937 "bang.tab.cpp"
    break;

  case 83: // field-list: field
#line 643 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2943 "bang.tab.cpp"
    break;

  case 84: // field-list: field-list "," field
#line 645 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2949 "bang.tab.cpp"
    break;

  case 85: // field-default-value-opt: %empty
#line 649 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 2955 "bang.tab.cpp"
    break;

  case 86: // field-default-value-opt: "`=`" syntax-expression
#line 650 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2961 "bang.tab.cpp"
    break;

  case 87: // field: identifier "`:`" type-expr field-default-value-opt
#line 655 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2967 "bang.tab.cpp"
    break;

  case 88: // field: identifier "`=>`" type-expr field-default-value-opt
#line 657 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2973 "bang.tab.cpp"
    break;

  case 89: // parameter-list-opt: %empty
#line 667 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 2979 "bang.tab.cpp"
    break;

  case 90: // parameter-list-opt: parameter-list
#line 668 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 2985 "bang.tab.cpp"
    break;

  case 91: // parameter-list: parameter-decl
#line 673 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2991 "bang.tab.cpp"
    break;

  case 92: // parameter-list: parameter-list "," parameter-decl
#line 675 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2997 "bang.tab.cpp"
    break;

  case 93: // internal-identifier-opt: %empty
#line 679 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3003 "bang.tab.cpp"
    break;

  case 94: // internal-identifier-opt: internal-identifier
#line 680 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3009 "bang.tab.cpp"
    break;

  case 95: // parameter-default-value-opt: %empty
#line 684 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3015 "bang.tab.cpp"
    break;

  case 96: // parameter-default-value-opt: "`=`" syntax-expression
#line 685 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3021 "bang.tab.cpp"
    break;

  case 97: // parameter-decl: identifier internal-identifier-opt "`:`" parameter-constraint-set parameter-default-value-opt
#line 690 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3027 "bang.tab.cpp"
    break;

  case 98: // parameter-decl: identifier internal-identifier-opt "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 692 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3033 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 694 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3039 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: identifier internal-identifier-opt "`=>`" parameter-value-constraint-set
#line 696 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3045 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: INTERNAL_IDENTIFIER "`:`" parameter-constraint-set parameter-default-value-opt
#line 699 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3051 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 701 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3057 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 703 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3063 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: INTERNAL_IDENTIFIER "`=>`" parameter-value-constraint-set
#line 705 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3069 "bang.tab.cpp"
    break;

  case 105: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" parameter-constraint-set
#line 708 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3075 "bang.tab.cpp"
    break;

  case 106: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "const modifier" parameter-constraint-set
#line 710 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3081 "bang.tab.cpp"
    break;

  case 107: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "mut modifier" parameter-constraint-set
#line 712 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3087 "bang.tab.cpp"
    break;

  case 108: // parameter-decl: parameter-constraint-set parameter-default-value-opt
#line 715 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3093 "bang.tab.cpp"
    break;

  case 109: // parameter-decl: "const modifier" parameter-constraint-set parameter-default-value-opt
#line 717 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3099 "bang.tab.cpp"
    break;

  case 110: // parameter-decl: "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 719 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3105 "bang.tab.cpp"
    break;

  case 111: // parameter-decl: "`=>`" parameter-value-constraint-set
#line 721 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3111 "bang.tab.cpp"
    break;

  case 112: // parameter-constraint-set: parameter-matched-type
#line 726 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3117 "bang.tab.cpp"
    break;

  case 113: // parameter-constraint-set: concept-expression
#line 728 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3123 "bang.tab.cpp"
    break;

  case 114: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 730 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3129 "bang.tab.cpp"
    break;

  case 115: // parameter-value-constraint-set: parameter-matched-value
#line 735 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3135 "bang.tab.cpp"
    break;

  case 116: // parameter-value-constraint-set: concept-expression
#line 737 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3141 "bang.tab.cpp"
    break;

  case 117: // parameter-value-constraint-set: parameter-value-constraint-set concept-expression
#line 739 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3147 "bang.tab.cpp"
    break;

  case 118: // parameter-matched-value: syntax-expression
#line 758 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3153 "bang.tab.cpp"
    break;

  case 119: // parameter-matched-type: basic-parameter-matched-type
#line 762 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3159 "bang.tab.cpp"
    break;

  case 120: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 764 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3165 "bang.tab.cpp"
    break;

  case 121: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 766 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3171 "bang.tab.cpp"
    break;

  case 122: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 768 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3177 "bang.tab.cpp"
    break;

  case 123: // basic-parameter-matched-type: "`_`"
#line 774 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3183 "bang.tab.cpp"
    break;

  case 124: // basic-parameter-matched-type: qname
#line 780 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3189 "bang.tab.cpp"
    break;

  case 125: // basic-parameter-matched-type: qname "`(`" argument-list-opt "`)`"
#line 783 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3195 "bang.tab.cpp"
    break;

  case 126: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 785 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3201 "bang.tab.cpp"
    break;

  case 127: // concept-expression: "`@`" qname
#line 808 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3207 "bang.tab.cpp"
    break;

  case 128: // syntax-expression: INTERNAL_IDENTIFIER
#line 813 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3213 "bang.tab.cpp"
    break;

  case 129: // syntax-expression: syntax-expression-wo-ii
#line 814 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3219 "bang.tab.cpp"
    break;

  case 130: // syntax-expression-wo-ii: "nil"
#line 820 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3225 "bang.tab.cpp"
    break;

  case 131: // syntax-expression-wo-ii: "true"
#line 822 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3231 "bang.tab.cpp"
    break;

  case 132: // syntax-expression-wo-ii: "false"
#line 824 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3237 "bang.tab.cpp"
    break;

  case 133: // syntax-expression-wo-ii: INTEGER
#line 826 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3243 "bang.tab.cpp"
    break;

  case 134: // syntax-expression-wo-ii: DECIMAL
#line 828 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3249 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: DECIMAL_S
#line 830 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3255 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: INTEGER_INDEX
#line 832 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3261 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: STRING
#line 834 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3267 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 836 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3273 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: qname
#line 838 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3279 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: "`(`" argument-list "`)`"
#line 840 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3291 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 848 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3303 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 856 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3309 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 858 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3315 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: "`.`" identifier
#line 860 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3321 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 864 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3327 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 866 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3333 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 868 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3339 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 871 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3345 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 875 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3351 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 877 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3357 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 881 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3363 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 883 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3369 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 887 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3375 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 891 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3381 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: apostrophe-expression
#line 893 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3387 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: new-expression
#line 894 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3393 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: compound-expression
#line 895 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3399 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: lambda-expression
#line 896 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3405 "bang.tab.cpp"
    break;

  case 159: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 901 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3411 "bang.tab.cpp"
    break;

  case 160: // new-expression: "`new`" qname
#line 906 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3417 "bang.tab.cpp"
    break;

  case 161: // new-expression: "`new`" apostrophe-expression
#line 908 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3423 "bang.tab.cpp"
    break;

  case 162: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 910 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3429 "bang.tab.cpp"
    break;

  case 163: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 912 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3435 "bang.tab.cpp"
    break;

  case 164: // call-expression: qname "`(`" argument-list-opt "`)`"
#line 917 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3441 "bang.tab.cpp"
    break;

  case 165: // call-expression: call-expression "`(`" argument-list-opt "`)`"
#line 919 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3447 "bang.tab.cpp"
    break;

  case 166: // call-expression: apostrophe-expression "`(`" argument-list-opt "`)`"
#line 921 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3453 "bang.tab.cpp"
    break;

  case 167: // call-expression: lambda-expression "`(`" argument-list-opt "`)`"
#line 923 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3459 "bang.tab.cpp"
    break;

  case 168: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 928 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3465 "bang.tab.cpp"
    break;

  case 169: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 930 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3471 "bang.tab.cpp"
    break;

  case 170: // compound-expression: call-expression
#line 934 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3477 "bang.tab.cpp"
    break;

  case 171: // compound-expression: syntax-expression "`...`"
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3483 "bang.tab.cpp"
    break;

  case 172: // type-expr: qname
#line 1023 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3489 "bang.tab.cpp"
    break;

  case 173: // type-expr: call-expression
#line 1024 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3495 "bang.tab.cpp"
    break;

  case 174: // type-expr: INTERNAL_IDENTIFIER
#line 1026 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3501 "bang.tab.cpp"
    break;

  case 175: // type-expr: "`[`" type-expr "`]`"
#line 1030 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3507 "bang.tab.cpp"
    break;

  case 176: // type-expr: "`(`" argument-list-opt "`)`"
#line 1032 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3513 "bang.tab.cpp"
    break;

  case 177: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1034 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3519 "bang.tab.cpp"
    break;

  case 178: // type-expr: type-expr "`|`" type-expr
#line 1036 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3531 "bang.tab.cpp"
    break;

  case 179: // type-expr: type-expr "`->`" type-expr
#line 1044 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3537 "bang.tab.cpp"
    break;


#line 3541 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -207;

  const signed char parser::yytable_ninf_ = -94;

  const short
  parser::yypact_[] =
  {
     628,  -207,  -207,  -207,  -207,  -207,  -207,  -207,  -207,   825,
      12,   825,   825,   825,    12,   825,    -5,   -68,    35,   825,
     825,   825,   825,    43,   105,   -64,  -207,    43,    43,  -207,
    -207,  -207,    72,     4,   432,  -207,  -207,    30,   115,  -207,
    -207,  -207,    70,    21,   632,  -207,    96,  -207,   122,   138,
      14,   144,   731,  -207,  -207,    45,    67,  -207,   825,   878,
     112,   878,   175,  -207,    26,    12,  -207,    12,    23,   207,
     148,   535,   436,   835,   878,   169,  -207,  -207,  -207,   177,
    -207,   150,  -207,  -207,  -207,  -207,  -207,  -207,  -207,  -207,
     325,   825,    12,   825,   204,  -207,   125,   178,    33,  -207,
     825,   825,   825,  -207,   825,    59,   825,   825,   825,   825,
     825,   825,   825,  -207,  -207,  -207,  -207,   825,   878,  -207,
     825,  -207,  -207,   193,  -207,   825,   825,   726,   825,  -207,
     825,   182,   825,   204,    12,   212,   204,  -207,  -207,   825,
     325,    70,    96,   122,   138,   -14,   878,  -207,   217,   201,
     104,    43,  -207,   747,   343,   343,    73,   222,   223,   208,
    -207,   235,  -207,    80,  -207,   204,   825,  -207,   878,   237,
     237,   845,  -207,  -207,    26,    26,    27,   596,   236,   242,
     243,  -207,   878,   325,   244,   245,  -207,  -207,    43,   226,
     530,  -207,   251,  -207,  -207,    21,   835,   835,   262,   878,
     246,  -207,   267,   252,  -207,    12,   277,   278,   154,   325,
     317,   325,  -207,   747,   283,   292,   125,   313,  -207,  -207,
     878,   296,   235,   235,  -207,  -207,   183,   825,   131,   204,
     825,  -207,  -207,  -207,   155,   286,   878,  -207,  -207,  -207,
    -207,   -14,  -207,  -207,   186,  -207,  -207,  -207,  -207,  -207,
     262,  -207,  -207,  -207,   316,  -207,    12,   202,   289,   274,
    -207,   163,  -207,  -207,  -207,   294,    -3,   313,   299,   343,
     343,   235,  -207,  -207,  -207,  -207,   747,   350,   300,   325,
    -207,  -207,   878,  -207,   328,   204,   825,  -207,   325,   325,
    -207,    12,   297,  -207,  -207,   343,   343,   313,   235,   235,
    -207,   313,   343,   343,   235,  -207,    54,   325,   303,   878,
      11,    11,  -207,    12,   313,   313,  -207,  -207,   235,   235,
    -207,  -207,   -14,   262,   825,  -207,  -207,   304,  -207,  -207,
     878,  -207
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   137,    50,   128,   138,   136,   133,   134,   135,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,   130,
     131,   132,     0,     0,     0,     5,    42,    21,    17,     7,
      15,    53,   139,     0,     0,   129,   155,   156,   170,   158,
     157,     0,     0,   157,    52,    53,     0,    75,     0,    78,
       0,    71,     0,   144,   148,     0,    19,     0,     0,   160,
     161,     0,     0,     0,    44,     0,    43,    13,    56,     0,
      39,     0,    16,     1,     2,    10,     3,     8,     6,     9,
       0,     0,     0,    73,    89,    58,    57,     0,     0,   147,
       0,     0,     0,   171,     0,     0,     0,     0,     0,     0,
      73,    73,    73,   159,    79,    80,   140,     0,    77,   141,
       0,   142,    20,     0,    12,    73,    73,    23,     0,    31,
       0,    34,     0,    89,    62,     0,    89,    37,   174,    73,
       0,   172,     0,   173,     0,    22,    18,    54,     0,    74,
     121,     0,   123,     0,     0,     0,    53,   124,     0,    90,
      91,    95,   112,   119,   113,    89,     0,    36,    46,   151,
     152,     0,   145,   146,   149,   150,   153,   154,     0,     0,
       0,    76,    72,     0,     0,     0,    49,    48,     0,    40,
      41,    26,     0,    47,    24,     0,     0,     0,     0,    69,
       0,    66,     0,    63,    64,    81,     0,     0,     0,     0,
       0,     0,   164,     0,   122,     0,   127,   111,   115,   116,
     118,   121,    95,    95,    51,    94,     0,    73,     0,     0,
       0,   108,   114,   120,     0,     0,    14,   143,   166,   165,
     167,    11,   162,   163,     0,    29,    27,    28,    25,    30,
       0,    32,    33,    35,     0,    61,     0,     0,     0,    82,
      83,     0,   176,   175,   179,     0,   178,   104,     0,     0,
       0,    95,   117,   122,   109,   110,     0,     0,     0,     0,
     168,    92,    96,   126,    59,    89,     0,    65,     0,     0,
      67,     0,     0,    38,   177,     0,     0,   105,    95,    95,
     101,   100,     0,     0,    95,   125,     0,     0,     0,    70,
      85,    85,    84,    81,   106,   107,   102,   103,    95,    95,
      97,   169,    60,     0,     0,    88,    87,     0,    98,    99,
      86,    68
  };

  const short
  parser::yypgoto_[] =
  {
    -207,  -207,  -207,  -207,   320,  -207,  -207,  -207,  -207,  -207,
     -19,   -32,  -207,  -124,   165,    29,  -207,   124,     8,  -207,
     -51,  -207,  -207,  -207,   106,  -207,  -207,   348,   -79,   352,
     247,  -207,    52,  -207,    57,    78,  -110,  -207,  -207,  -111,
     146,   -29,  -206,  -207,  -207,   139,   157,     0,  -207,    -7,
    -207,   -52,   -27,    10,    34
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    32,    33,    34,    35,    36,    37,    38,   189,   190,
     137,    39,   192,    40,   194,    41,   225,    42,    51,    97,
      98,    80,   202,   203,   204,    82,    76,    60,   148,   149,
      57,    58,   258,   259,   325,   260,   158,   159,   226,   231,
     160,   161,   217,   218,   162,   163,   164,    59,    45,    46,
      47,    48,    49,    53,   145
  };

  const short
  parser::yytable_[] =
  {
      44,     2,    89,   193,    84,   209,    67,   267,    43,    52,
      50,    70,    61,    61,   -45,    64,   209,   124,     2,    71,
      72,    73,    74,   200,    68,   324,   206,     2,    78,     2,
     209,   178,   179,   180,    44,    99,    99,   210,   143,    54,
      55,     2,    43,    63,    50,    66,   184,   185,   210,     2,
       9,   211,   129,   166,   131,   235,   101,   102,   118,   114,
     207,    85,   210,   144,   -45,     2,   193,    10,    94,    10,
     301,   -45,    83,   209,     9,    90,   211,   104,   104,   167,
     224,    10,   127,   142,   105,   105,   106,   107,   143,    10,
     115,   146,    65,   -93,   122,   191,   123,    95,   173,    95,
     168,   169,   170,   127,   171,   210,   174,   175,   176,   177,
      77,   274,   275,   144,   233,   116,    92,    93,   -93,   211,
     182,   147,    55,   156,   213,   222,   223,    44,   196,    91,
     197,   143,   199,   142,   172,   195,   117,    50,   214,    55,
      55,    55,    69,   110,   250,   234,    55,    75,   278,   215,
     279,    79,    81,   220,    55,    55,   144,   143,   247,   143,
     300,     2,   156,   201,   119,   156,   236,    96,    55,   111,
     135,    92,   152,   209,   208,   308,   142,   251,   252,   253,
     127,   120,   144,   292,   144,   112,   271,   316,   317,   132,
      44,    94,    96,   320,   156,   126,    92,   136,   195,   127,
      50,    10,   142,   276,   142,   210,   263,   328,   329,   280,
       2,   150,   127,   220,   141,    92,   133,   241,   157,   211,
     151,   152,   288,    92,   153,   165,   134,   143,   277,   121,
     282,   167,    92,   285,   257,   127,   143,   143,   183,   297,
     298,   299,   293,   264,   120,   266,    99,   289,   304,   230,
      10,   151,   144,    92,   125,   143,    55,   157,   156,   205,
     157,   144,   144,   198,   141,   212,   314,   315,    92,   227,
     117,   228,   142,   318,   319,   216,   220,   229,   157,   157,
     144,   142,   142,   245,   238,   201,   309,   321,   104,   157,
     239,   240,   242,   243,   254,   105,   106,   107,     2,   221,
     142,   249,   154,   155,   293,     2,   221,   141,   151,   152,
     219,   127,   244,   306,   156,   151,   152,   255,   232,    96,
     257,   256,   310,   311,   330,   261,   262,   265,   268,   151,
     273,     2,   138,   141,   284,   141,   286,   290,    10,   157,
       9,   322,   257,   291,   313,    10,   294,   307,   305,     2,
     221,   323,   331,   157,    88,   248,     2,   221,   157,   151,
     152,    62,   287,    56,   181,   327,   151,   152,   326,   312,
     219,    10,   139,   283,   272,   281,   140,     0,     0,   232,
     232,     0,     0,     0,     0,     0,     0,     0,     0,    10,
     269,   270,   157,   157,   157,     0,    10,   295,   296,     0,
       0,   157,     0,   141,     0,     0,     0,     0,     0,   157,
       0,     0,   141,   141,     0,     0,    25,    26,     0,   157,
     157,     0,     0,     0,   272,     0,   157,   157,   232,     0,
       0,   141,    86,   219,     0,     0,     0,     1,     2,     3,
       4,     5,     6,     7,     8,    99,     0,     9,   302,   303,
       0,     0,     0,     0,   232,   232,   232,     0,   272,     0,
       0,   232,     0,     0,     0,   101,   102,     0,     0,     0,
     103,   232,   232,     0,     0,   232,   232,     0,    10,    11,
       0,     0,     0,    12,     0,    13,     0,   104,     0,    87,
      14,     0,     0,     0,   105,   106,   107,     0,    15,     0,
     108,   109,     0,     0,     0,    16,     0,    17,    18,    19,
      20,     0,    21,     0,     0,   130,    22,     0,    23,     0,
       0,     0,    24,    25,    26,    27,    28,     0,     0,     0,
       0,     0,    29,    30,    31,     1,     2,     3,     4,     5,
       6,     7,     8,     0,    99,     9,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   101,   102,     0,     0,     0,   103,
       0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
       0,    12,     0,    13,   127,     0,   104,   246,    14,     0,
       0,     0,   128,   105,   106,   107,    15,     0,     0,   108,
     109,     0,     0,    16,     0,    99,    18,    19,    20,     0,
      21,     0,   186,   187,    22,     0,    23,     0,     0,     0,
       0,    25,    26,    27,   188,   101,   102,     0,     0,     0,
      29,    30,    31,     1,     2,     3,     4,     5,     6,     7,
       8,    99,     0,     9,     0,     0,   100,   104,     0,     0,
       0,     0,     0,     0,   105,   106,   107,     0,     0,     0,
     108,   101,   102,     0,     0,     0,   103,     0,     0,     0,
       0,     0,     0,     0,    10,    11,     0,     0,     0,    12,
       0,    13,     0,   104,     0,     0,    14,     0,     0,     0,
     105,   106,   107,     0,    15,     0,   108,   109,     0,     0,
       0,    16,     0,    17,    18,    19,    20,     0,    21,     0,
       0,     0,    22,     0,    23,     0,     0,     0,    24,    25,
      26,    27,    28,     0,     0,     0,     0,     0,    29,    30,
      31,     1,     2,     3,     4,     5,     6,     7,     8,     0,
      99,     9,     0,     0,     0,     0,   113,     0,     0,     0,
       0,     0,     1,     2,     3,     4,     5,     6,     7,     8,
     101,   102,     9,   151,     0,   103,     0,     0,     0,     0,
       0,     0,    10,    11,     0,     0,     0,    12,     0,    13,
       0,     0,   104,     0,    14,     0,     0,     0,     0,   105,
     106,   107,    15,    10,    11,   108,   109,     0,    12,    16,
      13,     0,    18,    19,    20,    14,    21,     0,   186,   187,
      22,     0,    23,    15,     0,     0,     0,    25,    26,    27,
     188,     0,     0,    18,     0,     0,    29,    30,    31,     0,
       1,     2,     3,     4,     5,     6,     7,     8,    25,    26,
       9,     0,     0,     0,    99,     0,     0,    29,    30,    31,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   101,   102,     0,     0,     0,   103,
       0,    10,    11,     0,   101,   102,    12,     0,    13,   103,
       0,     0,     0,    14,   127,     0,   104,    99,     0,     0,
       0,    15,     0,   105,   106,   107,   104,   237,     0,   108,
     109,    18,     0,   105,   106,   107,     0,   101,   102,   108,
     109,     0,   103,     0,     0,     0,    25,    26,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     0,   104,
       0,     0,     0,     0,     0,     0,   105,   106,   107,     0,
       0,     0,   108,   109
  };

  const short
  parser::yycheck_[] =
  {
       0,     6,    34,   127,     0,    19,    74,   213,     0,     9,
       0,    18,    12,    13,     0,    15,    19,    68,     6,    19,
      20,    21,    22,   133,    92,    14,   136,     6,    92,     6,
      19,   110,   111,   112,    34,     9,     9,    51,    90,    10,
      11,     6,    34,    14,    34,    16,   125,   126,    51,     6,
      15,    65,    71,    20,    73,   165,    29,    30,    58,    14,
     139,    57,    51,    90,    50,     6,   190,    46,    47,    46,
     276,    57,     0,    19,    15,    45,    65,    51,    51,    98,
       7,    46,    49,    90,    58,    58,    59,    60,   140,    46,
      45,    91,    97,    20,    65,   127,    67,    76,   105,    76,
     100,   101,   102,    49,   104,    51,   106,   107,   108,   109,
       5,   222,   223,   140,    34,    48,    46,    47,    45,    65,
     120,    92,    93,    94,    20,   154,   155,   127,   128,    14,
     130,   183,   132,   140,   105,   127,    69,   127,    34,   110,
     111,   112,    18,    47,   195,    65,   117,    23,   227,    45,
      19,    27,    28,   153,   125,   126,   183,   209,   190,   211,
     271,     6,   133,   134,    52,   136,   166,    43,   139,    47,
      20,    46,    17,    19,   140,   285,   183,   196,   197,   198,
      49,    69,   209,    20,   211,    47,   215,   298,   299,    20,
     190,    47,    68,   304,   165,    47,    46,    47,   190,    49,
     190,    46,   209,    20,   211,    51,    52,   318,   319,   228,
       6,     7,    49,   213,    90,    46,    47,   183,    94,    65,
      16,    17,    20,    46,    20,    47,    49,   279,    45,    54,
     230,   250,    46,    47,   205,    49,   288,   289,    45,   268,
     269,   270,   261,   209,    69,   211,     9,    45,   277,    14,
      46,    16,   279,    46,    47,   307,   227,   133,   229,    47,
     136,   288,   289,    81,   140,    48,   295,   296,    46,    47,
      69,    48,   279,   302,   303,   151,   276,    69,   154,   155,
     307,   288,   289,    57,    48,   256,   286,   306,    51,   165,
      48,    48,    48,    48,    48,    58,    59,    60,     6,     7,
     307,    50,    98,    99,   323,     6,     7,   183,    16,    17,
     153,    49,   188,   279,   285,    16,    17,    50,   161,   195,
     291,    69,   288,   289,   324,    48,    48,    10,    45,    16,
      34,     6,     7,   209,    48,   211,    20,    48,    46,   215,
      15,   307,   313,    69,    47,    46,    52,    19,    48,     6,
       7,    48,    48,   229,    34,   190,     6,     7,   234,    16,
      17,    13,   256,    11,   117,   313,    16,    17,   311,   291,
     213,    46,    47,   234,   217,   229,    51,    -1,    -1,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      98,    99,   268,   269,   270,    -1,    46,    98,    99,    -1,
      -1,   277,    -1,   279,    -1,    -1,    -1,    -1,    -1,   285,
      -1,    -1,   288,   289,    -1,    -1,    91,    92,    -1,   295,
     296,    -1,    -1,    -1,   267,    -1,   302,   303,   271,    -1,
      -1,   307,     0,   276,    -1,    -1,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,     9,    -1,    15,    98,    99,
      -1,    -1,    -1,    -1,   297,   298,   299,    -1,   301,    -1,
      -1,   304,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,
      34,   314,   315,    -1,    -1,   318,   319,    -1,    46,    47,
      -1,    -1,    -1,    51,    -1,    53,    -1,    51,    -1,    57,
      58,    -1,    -1,    -1,    58,    59,    60,    -1,    66,    -1,
      64,    65,    -1,    -1,    -1,    73,    -1,    75,    76,    77,
      78,    -1,    80,    -1,    -1,    79,    84,    -1,    86,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,     9,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    51,    -1,    53,    49,    -1,    51,    57,    58,    -1,
      -1,    -1,    57,    58,    59,    60,    66,    -1,    -1,    64,
      65,    -1,    -1,    73,    -1,     9,    76,    77,    78,    -1,
      80,    -1,    82,    83,    84,    -1,    86,    -1,    -1,    -1,
      -1,    91,    92,    93,    94,    29,    30,    -1,    -1,    -1,
     100,   101,   102,     5,     6,     7,     8,     9,    10,    11,
      12,     9,    -1,    15,    -1,    -1,    14,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    60,    -1,    -1,    -1,
      64,    29,    30,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    -1,    51,
      -1,    53,    -1,    51,    -1,    -1,    58,    -1,    -1,    -1,
      58,    59,    60,    -1,    66,    -1,    64,    65,    -1,    -1,
      -1,    73,    -1,    75,    76,    77,    78,    -1,    80,    -1,
      -1,    -1,    84,    -1,    86,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,     5,     6,     7,     8,     9,    10,    11,    12,    -1,
       9,    15,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,     5,     6,     7,     8,     9,    10,    11,    12,
      29,    30,    15,    16,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    -1,    51,    -1,    53,
      -1,    -1,    51,    -1,    58,    -1,    -1,    -1,    -1,    58,
      59,    60,    66,    46,    47,    64,    65,    -1,    51,    73,
      53,    -1,    76,    77,    78,    58,    80,    -1,    82,    83,
      84,    -1,    86,    66,    -1,    -1,    -1,    91,    92,    93,
      94,    -1,    -1,    76,    -1,    -1,   100,   101,   102,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    91,    92,
      15,    -1,    -1,    -1,     9,    -1,    -1,   100,   101,   102,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    34,
      -1,    46,    47,    -1,    29,    30,    51,    -1,    53,    34,
      -1,    -1,    -1,    58,    49,    -1,    51,     9,    -1,    -1,
      -1,    66,    -1,    58,    59,    60,    51,    52,    -1,    64,
      65,    76,    -1,    58,    59,    60,    -1,    29,    30,    64,
      65,    -1,    34,    -1,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      -1,    -1,    64,    65
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    51,    53,    58,    66,    73,    75,    76,    77,
      78,    80,    84,    86,    90,    91,    92,    93,    94,   100,
     101,   102,   104,   105,   106,   107,   108,   109,   110,   114,
     116,   118,   120,   121,   150,   151,   152,   153,   154,   155,
     156,   121,   150,   156,   118,   118,   132,   133,   134,   150,
     130,   150,   130,   118,   150,    97,   118,    74,    92,   120,
     152,   150,   150,   150,   150,   120,   129,     5,    92,   120,
     124,   120,   128,     0,     0,    57,     0,    57,   107,   114,
      45,    14,    46,    47,    47,    76,   120,   122,   123,     9,
      14,    29,    30,    34,    51,    58,    59,    60,    64,    65,
      47,    47,    47,    15,    14,    45,    48,    69,   150,    52,
      69,    54,   118,   118,   123,    47,    47,    49,    57,   113,
      79,   113,    20,    47,    49,    20,    47,   113,     7,    47,
      51,   120,   152,   154,   155,   157,   150,   118,   131,   132,
       7,    16,    17,    20,    98,    99,   118,   120,   139,   140,
     143,   144,   147,   148,   149,    47,    20,   113,   150,   150,
     150,   150,   118,   152,   150,   150,   150,   150,   131,   131,
     131,   133,   150,    45,   131,   131,    82,    83,    94,   111,
     112,   114,   115,   116,   117,   121,   150,   150,    81,   150,
     139,   118,   125,   126,   127,    47,   139,   131,   157,    19,
      51,    65,    48,    20,    34,    45,   120,   145,   146,   149,
     150,     7,   144,   144,     7,   119,   141,    47,    48,    69,
      14,   142,   149,    34,    65,   139,   150,    52,    48,    48,
      48,   157,    48,    48,   120,    57,    57,   114,   117,    50,
     123,   113,   113,   113,    48,    50,    69,   118,   135,   136,
     138,    48,    48,    52,   157,    10,   157,   145,    45,    98,
      99,   144,   149,    34,   142,   142,    20,    45,   131,    19,
     113,   143,   150,   148,    48,    47,    20,   127,    20,    45,
      48,    69,    20,   113,    52,    98,    99,   144,   144,   144,
     142,   145,    98,    99,   144,    48,   157,    19,   139,   150,
     157,   157,   138,    47,   144,   144,   142,   142,   144,   144,
     142,   113,   157,    48,    14,   137,   137,   135,   142,   142,
     150,    48
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   103,   104,   104,   105,   105,   105,   106,   106,   106,
     106,   107,   107,   107,   107,   107,   107,   108,   108,   109,
     109,   110,   110,   111,   111,   111,   112,   112,   112,   112,
     113,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     115,   115,   116,   116,   116,   116,   116,   117,   117,   117,
     118,   119,   120,   120,   120,   121,   121,   122,   122,   123,
     123,   124,   125,   125,   126,   126,   127,   128,   128,   129,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   134,
     134,   135,   135,   136,   136,   137,   137,   138,   138,   139,
     139,   140,   140,   141,   141,   142,   142,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   144,   144,   144,   145,   145,   145,   146,   147,
     147,   147,   147,   148,   148,   148,   148,   149,   150,   150,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   152,
     153,   153,   153,   153,   154,   154,   154,   154,   155,   155,
     156,   156,   157,   157,   157,   157,   157,   157,   157,   157
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     3,     2,     4,     1,     2,     1,     3,     2,
       3,     1,     3,     0,     1,     2,     1,     2,     2,     2,
       3,     3,     5,     5,     3,     5,     3,     3,     6,     2,
       1,     1,     1,     2,     2,     1,     3,     1,     1,     1,
       1,     1,     2,     1,     3,     1,     2,     1,     1,     4,
       6,     4,     0,     1,     1,     3,     1,     5,     8,     3,
       6,     1,     3,     0,     1,     1,     3,     2,     1,     2,
       2,     0,     1,     1,     3,     0,     2,     4,     4,     0,
       1,     1,     3,     0,     1,     0,     2,     5,     6,     6,
       4,     4,     5,     5,     3,     4,     5,     5,     2,     3,
       3,     2,     1,     1,     2,     1,     1,     2,     1,     1,
       2,     1,     2,     1,     1,     4,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     4,     2,     3,     3,     2,     2,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       2,     2,     5,     5,     4,     4,     4,     4,     5,     7,
       1,     2,     1,     1,     1,     3,     3,     4,     3,     3
  };


#if BANG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "INTERNAL_IDENTIFIER",
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
  "LOWEST", "INCLUDE", "INLINE", "\"`fn`\"", "ENUM", "STRUCT", "EXTENDS",
  "TYPENAME", "\"weak modifier\"", "\"const modifier\"",
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
  "parameter-default-value-opt", "parameter-decl",
  "parameter-constraint-set", "parameter-value-constraint-set",
  "parameter-matched-value", "parameter-matched-type",
  "basic-parameter-matched-type", "concept-expression",
  "syntax-expression", "syntax-expression-wo-ii", "apostrophe-expression",
  "new-expression", "call-expression", "lambda-expression",
  "compound-expression", "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   329,   329,   330,   334,   336,   338,   355,   357,   358,
     360,   365,   367,   369,   375,   382,   384,   389,   391,   396,
     398,   403,   404,   409,   411,   413,   418,   420,   421,   423,
     428,   433,   435,   437,   439,   441,   443,   445,   447,   449,
     454,   456,   461,   463,   465,   467,   469,   474,   476,   478,
     483,   491,   506,   508,   510,   516,   518,   523,   524,   528,
     530,   536,   541,   542,   546,   548,   553,   559,   561,   584,
     588,   602,   604,   610,   611,   615,   617,   622,   624,   629,
     631,   637,   638,   642,   644,   649,   650,   654,   656,   667,
     668,   672,   674,   679,   680,   684,   685,   689,   691,   693,
     695,   698,   700,   702,   704,   707,   709,   711,   714,   716,
     718,   720,   725,   727,   729,   734,   736,   738,   758,   762,
     763,   765,   767,   773,   779,   782,   784,   807,   812,   814,
     819,   821,   823,   825,   827,   829,   831,   833,   835,   837,
     839,   847,   855,   857,   859,   863,   865,   867,   870,   874,
     876,   880,   882,   886,   890,   893,   894,   895,   896,   900,
     905,   907,   909,   911,   916,   918,   920,   922,   927,   929,
     934,   935,  1022,  1024,  1025,  1029,  1031,  1033,  1035,  1043
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
#line 4439 "bang.tab.cpp"

#line 1385 "bang.y"

