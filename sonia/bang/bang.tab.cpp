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

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
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

      case symbol_kind::S_139_field_type_expr: // field-type-expr
      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
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

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
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

      case symbol_kind::S_139_field_type_expr: // field-type-expr
      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
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

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
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

      case symbol_kind::S_139_field_type_expr: // field-type-expr
      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
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

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
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

      case symbol_kind::S_139_field_type_expr: // field-type-expr
      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
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

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
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

      case symbol_kind::S_139_field_type_expr: // field-type-expr
      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
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

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
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

      case symbol_kind::S_139_field_type_expr: // field-type-expr
      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
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
#line 333 "bang.y"
                 { }
#line 1494 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 333 "bang.y"
                 { }
#line 1500 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 333 "bang.y"
                 { }
#line 1506 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 333 "bang.y"
                 { }
#line 1512 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 333 "bang.y"
                 { }
#line 1518 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 333 "bang.y"
                 { }
#line 1524 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 333 "bang.y"
                 { }
#line 1530 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 333 "bang.y"
                 { }
#line 1536 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 333 "bang.y"
                 { }
#line 1542 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 333 "bang.y"
                 { }
#line 1548 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 333 "bang.y"
                 { }
#line 1554 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 333 "bang.y"
                 { }
#line 1560 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 333 "bang.y"
                 { }
#line 1566 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 333 "bang.y"
                 { }
#line 1572 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 333 "bang.y"
                 { }
#line 1578 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 333 "bang.y"
                 { }
#line 1584 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 333 "bang.y"
                 { }
#line 1590 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 333 "bang.y"
                 { }
#line 1596 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 333 "bang.y"
                 { }
#line 1602 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 333 "bang.y"
                 { }
#line 1608 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 333 "bang.y"
                 { }
#line 1614 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 333 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 333 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 333 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 333 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 333 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 333 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 333 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 333 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 333 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 333 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 333 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 333 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
#line 333 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 333 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_let_decl: // let-decl
#line 333 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl_start: // let-decl-start
#line 333 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 333 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
#line 333 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
#line 333 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_braced_statements: // braced-statements
#line 333 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
#line 333 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
#line 333 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
#line 333 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_infunction_statement: // infunction-statement
#line 333 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 333 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_internal_identifier: // internal-identifier
#line 333 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 333 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
#line 333 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_name: // fn-name
#line 333 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
#line 333 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
#line 333 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
#line 333 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list: // case-list
#line 333 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_decl: // case-decl
#line 333 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
#line 333 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_using_decl: // using-decl
#line 333 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_expression_list: // expression-list
#line 333 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
#line 333 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list: // argument-list
#line 333 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 333 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_argument_name: // argument-name
#line 333 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
#line 333 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list: // field-list
#line 333 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
#line 333 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 333 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_field_type_expr: // field-type-expr
#line 333 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
#line 333 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_list: // parameter-list
#line 333 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
#line 333 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
#line 333 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
#line 333 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
#line 333 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
#line 333 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
#line 333 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
#line 333 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
#line 333 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
#line 333 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_syntax_expression: // syntax-expression
#line 333 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 333 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
#line 333 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_new_expression: // new-expression
#line 333 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_call_expression: // call-expression
#line 333 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_lambda_expression: // lambda-expression
#line 333 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_compound_expression: // compound-expression
#line 333 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_type_expr: // type-expr
#line 333 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
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

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
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

      case symbol_kind::S_139_field_type_expr: // field-type-expr
      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
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
#line 338 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2435 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 339 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2441 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 344 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2447 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 346 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2453 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 348 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2459 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 365 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2465 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 366 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2471 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 368 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2477 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 370 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2483 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 375 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2489 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 377 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2495 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 379 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2501 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 385 "bang.y"
        {
            yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > ();
            auto sts = ctx.new_statement_list();
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) });
            yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), ctx.push(std::move(sts)) };
        }
#line 2512 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 392 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2518 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 394 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2524 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 399 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2530 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 401 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2536 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: LET identifier
#line 406 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2542 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: LET "weak modifier" identifier
#line 408 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2548 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 412 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2554 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 414 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2560 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 419 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2566 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 421 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2572 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 423 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2578 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 428 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2584 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 429 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2590 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 431 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2596 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 433 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2602 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 438 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2608 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 443 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2614 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 445 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2620 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 447 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2626 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 449 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2632 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 451 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2638 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 453 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2644 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 455 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2650 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 457 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2656 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 459 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2662 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 464 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2668 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 466 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2674 "bang.tab.cpp"
    break;

  case 42: // generic-statement: let-decl
#line 471 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2680 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 473 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2686 "bang.tab.cpp"
    break;

  case 44: // generic-statement: "`return`" syntax-expression
#line 475 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2692 "bang.tab.cpp"
    break;

  case 45: // generic-statement: compound-expression
#line 477 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2698 "bang.tab.cpp"
    break;

  case 46: // generic-statement: syntax-expression "`=`" syntax-expression
#line 479 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2704 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: generic-statement
#line 484 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2710 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`break`"
#line 486 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2716 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`continue`"
#line 488 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2722 "bang.tab.cpp"
    break;

  case 50: // identifier: IDENTIFIER
#line 493 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2728 "bang.tab.cpp"
    break;

  case 51: // internal-identifier: INTERNAL_IDENTIFIER
#line 501 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2734 "bang.tab.cpp"
    break;

  case 52: // qname: "`::`" identifier
#line 516 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2740 "bang.tab.cpp"
    break;

  case 53: // qname: identifier
#line 518 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2746 "bang.tab.cpp"
    break;

  case 54: // qname: qname "`::`" identifier
#line 520 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2752 "bang.tab.cpp"
    break;

  case 55: // fn-start-decl: "`fn`"
#line 526 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2758 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: INLINE "`fn`"
#line 528 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2764 "bang.tab.cpp"
    break;

  case 57: // fn-name: qname
#line 532 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2770 "bang.tab.cpp"
    break;

  case 58: // fn-name: "`new`"
#line 533 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2776 "bang.tab.cpp"
    break;

  case 59: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 538 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_list_t > ()), nullopt }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2782 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" type-expr
#line 540 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2788 "bang.tab.cpp"
    break;

  case 61: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 546 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2794 "bang.tab.cpp"
    break;

  case 62: // case-list-opt: %empty
#line 550 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2800 "bang.tab.cpp"
    break;

  case 63: // case-list-opt: case-list
#line 551 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2806 "bang.tab.cpp"
    break;

  case 64: // case-list: case-decl
#line 556 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2812 "bang.tab.cpp"
    break;

  case 65: // case-list: case-list "," case-decl
#line 558 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2818 "bang.tab.cpp"
    break;

  case 66: // case-decl: identifier
#line 563 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2824 "bang.tab.cpp"
    break;

  case 67: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 569 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2830 "bang.tab.cpp"
    break;

  case 68: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 571 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2836 "bang.tab.cpp"
    break;

  case 69: // using-decl: qname "`=>`" syntax-expression
#line 594 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 2844 "bang.tab.cpp"
    break;

  case 70: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 598 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 2852 "bang.tab.cpp"
    break;

  case 71: // expression-list: syntax-expression
#line 612 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2858 "bang.tab.cpp"
    break;

  case 72: // expression-list: expression-list "," syntax-expression
#line 614 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2864 "bang.tab.cpp"
    break;

  case 73: // argument-list-opt: %empty
#line 619 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2870 "bang.tab.cpp"
    break;

  case 74: // argument-list-opt: argument-list
#line 620 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2876 "bang.tab.cpp"
    break;

  case 75: // argument-list: argument
#line 625 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2882 "bang.tab.cpp"
    break;

  case 76: // argument-list: argument-list "," argument
#line 627 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2888 "bang.tab.cpp"
    break;

  case 77: // argument: argument-name syntax-expression
#line 632 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2894 "bang.tab.cpp"
    break;

  case 78: // argument: syntax-expression
#line 634 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2900 "bang.tab.cpp"
    break;

  case 79: // argument-name: identifier "`=`"
#line 639 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2906 "bang.tab.cpp"
    break;

  case 80: // argument-name: identifier "`:`"
#line 641 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2912 "bang.tab.cpp"
    break;

  case 81: // field-list-opt: %empty
#line 646 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2918 "bang.tab.cpp"
    break;

  case 82: // field-list-opt: field-list
#line 647 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2924 "bang.tab.cpp"
    break;

  case 83: // field-list: field
#line 652 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2930 "bang.tab.cpp"
    break;

  case 84: // field-list: field-list "," field
#line 654 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2936 "bang.tab.cpp"
    break;

  case 85: // field-default-value-opt: %empty
#line 658 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 2942 "bang.tab.cpp"
    break;

  case 86: // field-default-value-opt: "`=`" syntax-expression
#line 659 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2948 "bang.tab.cpp"
    break;

  case 87: // field: identifier "`:`" field-type-expr field-default-value-opt
#line 664 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2954 "bang.tab.cpp"
    break;

  case 88: // field: identifier "`=>`" field-type-expr field-default-value-opt
#line 666 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2960 "bang.tab.cpp"
    break;

  case 89: // field-type-expr: qname
#line 676 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2966 "bang.tab.cpp"
    break;

  case 90: // field-type-expr: qname "`(`" argument-list-opt "`)`"
#line 678 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2972 "bang.tab.cpp"
    break;

  case 91: // field-type-expr: INTERNAL_IDENTIFIER
#line 680 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2978 "bang.tab.cpp"
    break;

  case 92: // field-type-expr: field-type-expr "`|`" field-type-expr
#line 682 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2984 "bang.tab.cpp"
    break;

  case 93: // parameter-list-opt: %empty
#line 688 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 2990 "bang.tab.cpp"
    break;

  case 94: // parameter-list-opt: parameter-list
#line 689 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 2996 "bang.tab.cpp"
    break;

  case 95: // parameter-list: parameter-decl
#line 694 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3002 "bang.tab.cpp"
    break;

  case 96: // parameter-list: parameter-list "," parameter-decl
#line 696 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3008 "bang.tab.cpp"
    break;

  case 97: // internal-identifier-opt: %empty
#line 700 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3014 "bang.tab.cpp"
    break;

  case 98: // internal-identifier-opt: internal-identifier
#line 701 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3020 "bang.tab.cpp"
    break;

  case 99: // parameter-default-value-opt: %empty
#line 705 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3026 "bang.tab.cpp"
    break;

  case 100: // parameter-default-value-opt: "`=`" syntax-expression
#line 706 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3032 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: identifier internal-identifier-opt "`:`" parameter-constraint-set parameter-default-value-opt
#line 711 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3038 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 713 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3044 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: identifier internal-identifier-opt "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 715 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3050 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: identifier internal-identifier-opt "`=>`" parameter-value-constraint-set
#line 717 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3056 "bang.tab.cpp"
    break;

  case 105: // parameter-decl: INTERNAL_IDENTIFIER "`:`" parameter-constraint-set parameter-default-value-opt
#line 720 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3062 "bang.tab.cpp"
    break;

  case 106: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 722 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3068 "bang.tab.cpp"
    break;

  case 107: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 724 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3074 "bang.tab.cpp"
    break;

  case 108: // parameter-decl: INTERNAL_IDENTIFIER "`=>`" parameter-value-constraint-set
#line 726 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3080 "bang.tab.cpp"
    break;

  case 109: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" parameter-constraint-set
#line 729 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3086 "bang.tab.cpp"
    break;

  case 110: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "const modifier" parameter-constraint-set
#line 731 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3092 "bang.tab.cpp"
    break;

  case 111: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "mut modifier" parameter-constraint-set
#line 733 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3098 "bang.tab.cpp"
    break;

  case 112: // parameter-decl: parameter-constraint-set parameter-default-value-opt
#line 736 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3104 "bang.tab.cpp"
    break;

  case 113: // parameter-decl: "const modifier" parameter-constraint-set parameter-default-value-opt
#line 738 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3110 "bang.tab.cpp"
    break;

  case 114: // parameter-decl: "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 740 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3116 "bang.tab.cpp"
    break;

  case 115: // parameter-decl: "`=>`" parameter-value-constraint-set
#line 742 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3122 "bang.tab.cpp"
    break;

  case 116: // parameter-constraint-set: parameter-matched-type
#line 747 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3128 "bang.tab.cpp"
    break;

  case 117: // parameter-constraint-set: concept-expression
#line 749 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3134 "bang.tab.cpp"
    break;

  case 118: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 751 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3140 "bang.tab.cpp"
    break;

  case 119: // parameter-value-constraint-set: parameter-matched-value
#line 756 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3146 "bang.tab.cpp"
    break;

  case 120: // parameter-value-constraint-set: concept-expression
#line 758 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3152 "bang.tab.cpp"
    break;

  case 121: // parameter-value-constraint-set: parameter-value-constraint-set concept-expression
#line 760 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3158 "bang.tab.cpp"
    break;

  case 122: // parameter-matched-value: syntax-expression
#line 779 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3164 "bang.tab.cpp"
    break;

  case 123: // parameter-matched-type: basic-parameter-matched-type
#line 783 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3170 "bang.tab.cpp"
    break;

  case 124: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 785 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3176 "bang.tab.cpp"
    break;

  case 125: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 787 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3182 "bang.tab.cpp"
    break;

  case 126: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 789 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3188 "bang.tab.cpp"
    break;

  case 127: // basic-parameter-matched-type: "`_`"
#line 795 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3194 "bang.tab.cpp"
    break;

  case 128: // basic-parameter-matched-type: qname
#line 801 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3200 "bang.tab.cpp"
    break;

  case 129: // basic-parameter-matched-type: qname "`(`" argument-list-opt "`)`"
#line 804 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3206 "bang.tab.cpp"
    break;

  case 130: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 806 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3212 "bang.tab.cpp"
    break;

  case 131: // concept-expression: "`@`" qname
#line 829 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3218 "bang.tab.cpp"
    break;

  case 132: // syntax-expression: INTERNAL_IDENTIFIER
#line 834 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3224 "bang.tab.cpp"
    break;

  case 133: // syntax-expression: syntax-expression-wo-ii
#line 835 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3230 "bang.tab.cpp"
    break;

  case 134: // syntax-expression-wo-ii: "nil"
#line 841 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3236 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: "true"
#line 843 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3242 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: "false"
#line 845 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3248 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: INTEGER
#line 847 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3254 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: DECIMAL
#line 849 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3260 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: DECIMAL_S
#line 851 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3266 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: INTEGER_INDEX
#line 853 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3272 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: STRING
#line 855 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3278 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 857 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3284 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: qname
#line 859 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3290 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: "`(`" argument-list "`)`"
#line 861 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3302 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 869 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3314 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 877 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3320 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: "`.`" identifier
#line 879 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3326 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 883 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3332 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 885 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3338 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 887 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3344 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 889 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3350 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 891 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3356 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 897 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3362 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 899 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3368 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: apostrophe-expression
#line 901 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3374 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: new-expression
#line 902 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3380 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: compound-expression
#line 903 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3386 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: lambda-expression
#line 904 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3392 "bang.tab.cpp"
    break;

  case 159: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 909 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3398 "bang.tab.cpp"
    break;

  case 160: // new-expression: "`new`" qname
#line 914 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3404 "bang.tab.cpp"
    break;

  case 161: // new-expression: "`new`" apostrophe-expression
#line 916 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3410 "bang.tab.cpp"
    break;

  case 162: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 918 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3416 "bang.tab.cpp"
    break;

  case 163: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 920 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3422 "bang.tab.cpp"
    break;

  case 164: // call-expression: qname "`(`" argument-list-opt "`)`"
#line 925 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3428 "bang.tab.cpp"
    break;

  case 165: // call-expression: call-expression "`(`" argument-list-opt "`)`"
#line 927 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3434 "bang.tab.cpp"
    break;

  case 166: // call-expression: apostrophe-expression "`(`" argument-list-opt "`)`"
#line 929 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3440 "bang.tab.cpp"
    break;

  case 167: // call-expression: lambda-expression "`(`" argument-list-opt "`)`"
#line 931 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3446 "bang.tab.cpp"
    break;

  case 168: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3452 "bang.tab.cpp"
    break;

  case 169: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 938 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3458 "bang.tab.cpp"
    break;

  case 170: // compound-expression: call-expression
#line 942 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3464 "bang.tab.cpp"
    break;

  case 171: // compound-expression: syntax-expression "`...`"
#line 944 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3470 "bang.tab.cpp"
    break;

  case 172: // type-expr: qname
#line 1031 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3476 "bang.tab.cpp"
    break;

  case 173: // type-expr: call-expression
#line 1032 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3482 "bang.tab.cpp"
    break;

  case 174: // type-expr: INTERNAL_IDENTIFIER
#line 1036 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3488 "bang.tab.cpp"
    break;

  case 175: // type-expr: "`[`" type-expr "`]`"
#line 1040 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3494 "bang.tab.cpp"
    break;

  case 176: // type-expr: "`(`" argument-list-opt "`)`"
#line 1042 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3500 "bang.tab.cpp"
    break;

  case 177: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1045 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_array_t{std::move(yystack_[3].value.as < syntax_expression_t > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3506 "bang.tab.cpp"
    break;

  case 178: // type-expr: type-expr "`|`" type-expr
#line 1047 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3518 "bang.tab.cpp"
    break;

  case 179: // type-expr: type-expr "`->`" type-expr
#line 1055 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3524 "bang.tab.cpp"
    break;


#line 3528 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -246;

  const signed char parser::yytable_ninf_ = -98;

  const short
  parser::yypact_[] =
  {
     583,  -246,  -246,  -246,  -246,  -246,  -246,  -246,  -246,   780,
     107,   780,   780,   780,   107,   780,    10,    83,    36,   780,
     780,   780,   780,    42,    49,    35,  -246,    42,    42,  -246,
    -246,  -246,   121,    29,   387,  -246,  -246,   128,   127,  -246,
    -246,  -246,    -1,    22,   296,  -246,   116,  -246,   148,   172,
      30,   183,   489,  -246,  -246,    85,    96,  -246,   780,   167,
     139,   167,   118,  -246,    23,   107,  -246,   107,    19,   210,
     188,   392,   261,   476,   167,   120,  -246,  -246,  -246,   175,
    -246,   182,  -246,  -246,  -246,  -246,  -246,  -246,  -246,  -246,
     197,   780,   107,   780,   233,  -246,   199,   207,    13,  -246,
     780,   780,   780,  -246,   178,   780,   780,   780,   780,  -246,
    -246,  -246,  -246,   780,   167,  -246,   780,  -246,  -246,   216,
    -246,   780,   780,   681,   780,  -246,   780,   194,   780,   233,
     107,   229,   233,  -246,  -246,   780,   197,    -1,   116,   148,
     172,    52,   167,  -246,   232,   214,   217,    42,  -246,   702,
     401,   401,    77,   218,   236,   224,  -246,   201,  -246,     2,
    -246,   233,   780,  -246,   167,     1,     1,  -246,  -246,    23,
     237,   252,   253,  -246,   167,   197,   254,   255,  -246,  -246,
      42,   247,   485,  -246,   256,  -246,  -246,    22,   476,   476,
     262,   167,   274,  -246,   277,   259,  -246,   107,   295,   300,
      25,   197,   334,   197,  -246,   702,   304,   307,   199,   341,
    -246,  -246,   167,   324,   201,   201,  -246,  -246,   169,   780,
      89,   233,   780,  -246,  -246,  -246,    46,   312,   167,  -246,
    -246,  -246,    52,  -246,  -246,   250,  -246,  -246,  -246,  -246,
    -246,   262,  -246,  -246,  -246,   342,  -246,   107,   227,   317,
     297,  -246,    21,  -246,  -246,  -246,   316,    59,   341,   330,
     401,   401,   201,  -246,  -246,  -246,  -246,   702,   357,   321,
     197,  -246,  -246,   167,  -246,   353,   233,   780,  -246,    50,
      50,  -246,   107,   328,  -246,  -246,   401,   401,   341,   201,
     201,  -246,   341,   401,   401,   201,  -246,   129,   197,   331,
     167,  -246,   222,    24,    24,  -246,   107,   341,   341,  -246,
    -246,   201,   201,  -246,  -246,    52,   262,   780,   780,    50,
    -246,  -246,   332,  -246,  -246,   333,   167,  -246,  -246,  -246
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   141,    50,   132,   142,   140,   137,   138,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,   134,
     135,   136,     0,     0,     0,     5,    42,    21,    17,     7,
      15,    53,   143,     0,     0,   133,   155,   156,   170,   158,
     157,     0,     0,   157,    52,    53,     0,    75,     0,    78,
       0,    71,     0,   147,   151,     0,    19,     0,     0,   160,
     161,     0,     0,     0,    44,     0,    43,    13,    56,     0,
      39,     0,    16,     1,     2,    10,     3,     8,     6,     9,
       0,     0,     0,    73,    93,    58,    57,     0,     0,   150,
       0,     0,     0,   171,     0,     0,    73,    73,    73,   159,
      79,    80,   144,     0,    77,   145,     0,   146,    20,     0,
      12,    73,    73,    23,     0,    31,     0,    34,     0,    93,
      62,     0,    93,    37,   174,    73,     0,   172,     0,   173,
       0,    22,    18,    54,     0,    74,   125,     0,   127,     0,
       0,     0,    53,   128,     0,    94,    95,    99,   116,   123,
     117,    93,     0,    36,    46,   153,   154,   148,   149,   152,
       0,     0,     0,    76,    72,     0,     0,     0,    49,    48,
       0,    40,    41,    26,     0,    47,    24,     0,     0,     0,
       0,    69,     0,    66,     0,    63,    64,    81,     0,     0,
       0,     0,     0,     0,   164,     0,   126,     0,   131,   115,
     119,   120,   122,   125,    99,    99,    51,    98,     0,    73,
       0,     0,     0,   112,   118,   124,     0,     0,    14,   166,
     165,   167,    11,   162,   163,     0,    29,    27,    28,    25,
      30,     0,    32,    33,    35,     0,    61,     0,     0,     0,
      82,    83,     0,   176,   175,   179,     0,   178,   108,     0,
       0,     0,    99,   121,   126,   113,   114,     0,     0,     0,
       0,   168,    96,   100,   130,    59,    93,     0,    65,     0,
       0,    67,     0,     0,    38,   177,     0,     0,   109,    99,
      99,   105,   104,     0,     0,    99,   129,     0,     0,     0,
      70,    91,    89,    85,    85,    84,    81,   110,   111,   106,
     107,    99,    99,   101,   169,    60,     0,    73,     0,     0,
      88,    87,     0,   102,   103,     0,    86,    92,    68,    90
  };

  const short
  parser::yypgoto_[] =
  {
    -246,  -246,  -246,  -246,   349,  -246,  -246,  -246,  -246,  -246,
     -34,   -29,  -246,   -99,   206,    39,  -246,    91,     6,  -246,
     -67,  -246,  -246,  -246,   143,  -246,  -246,   391,  -104,   389,
     298,  -246,   103,  -246,   111,   138,  -245,  -118,  -246,  -246,
     -79,   202,    48,  -182,  -246,  -246,   198,   124,     0,  -246,
     -11,  -246,   -64,   -24,    27,  -128
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    32,    33,    34,    35,    36,    37,    38,   181,   182,
     133,    39,   184,    40,   186,    41,   217,    42,    51,    97,
      98,    80,   194,   195,   196,    82,    76,    60,   144,   145,
      57,    58,   249,   250,   320,   251,   303,   154,   155,   218,
     223,   156,   157,   209,   210,   158,   159,   160,    59,    45,
      46,    47,    48,    49,    53,   141
  };

  const short
  parser::yytable_[] =
  {
      44,   120,   170,   171,   172,    89,    43,    70,   200,    52,
      99,   192,    61,    61,   198,    64,     2,   176,   177,    71,
      72,    73,    74,   258,   185,     2,   139,    50,     2,    84,
     -45,   199,    99,   162,    44,   304,   225,   125,   318,   127,
      43,   283,     2,   227,   201,    92,    93,   232,     2,    54,
      55,     9,     2,    63,    77,    66,     2,   301,   114,   104,
     105,    50,   123,   148,   163,    10,   140,   226,    10,    94,
     123,   201,   139,   255,   327,   257,   202,   254,   201,   138,
     -45,   104,    10,   185,   216,   292,    85,   -45,    10,   319,
     203,   142,    10,   168,   183,    95,    10,   -97,    95,   110,
     164,   165,   166,   202,   118,   169,   119,    65,   270,    69,
     202,   139,   140,     2,    75,   269,   174,   203,    79,    81,
     241,    83,   -97,    44,   188,   138,   189,    78,   191,   187,
     111,   143,    55,   152,    96,   265,   266,   139,   123,   139,
     128,    91,   297,   167,   112,    55,    55,    55,   201,   212,
      50,   140,    55,   238,   242,   243,   244,    67,   299,    96,
      55,    55,   228,   106,   138,   113,    92,   129,   152,   193,
     315,   152,   117,    90,    55,    68,    99,   140,   123,   140,
     202,   137,    44,   291,     2,   153,   271,   116,   187,   267,
     138,   115,   138,     9,   203,   107,   101,   102,   214,   215,
     152,   103,   131,     2,   134,   212,   139,   163,   116,    50,
     309,   310,     9,   325,   268,   222,   313,   147,   284,   108,
     153,    92,   273,   153,   130,   104,   105,   137,    92,   132,
      94,   123,   323,   324,   139,   122,   248,   205,   208,     2,
     146,   153,   153,    10,   135,    92,   140,   279,   136,   147,
     148,   206,   153,   149,   161,   262,    92,   121,    55,   138,
     152,   175,   207,   314,    92,   219,   137,   212,    92,   317,
      99,   235,   280,   211,   140,   190,   197,   300,    96,    10,
     204,   224,   284,   113,   220,   229,   193,   138,    25,    26,
     101,   102,   137,   221,   137,   103,    92,   276,   153,   123,
     230,   231,   233,   234,   236,    99,   240,   288,   289,   290,
     100,   123,   153,     2,   213,   152,   295,   153,   326,   104,
     105,   248,   245,   147,   148,   101,   102,   246,   247,   211,
     103,   150,   151,   263,   307,   308,     2,   213,   224,   224,
     126,   311,   312,   252,   256,   248,   147,   148,   253,   259,
     153,   153,   153,    10,   104,   105,    55,   147,   264,   153,
     275,   137,   277,     2,   213,   281,   282,   153,   285,   296,
     302,   302,   298,   147,   148,   306,    10,   153,   153,   316,
     328,   329,   263,    88,   153,   153,   224,    86,   239,   137,
     278,   211,     1,     2,     3,     4,     5,     6,     7,     8,
      56,    99,     9,    10,    62,   260,   261,     2,   213,   322,
     302,   173,   224,   224,   224,   321,   263,   147,   148,   224,
     305,   101,   102,   272,   274,     0,   103,     0,   286,   287,
       0,   224,   224,    10,    11,   224,   224,     0,    12,     0,
      13,   123,     0,     0,    87,    14,     0,    10,     0,   124,
     104,   105,     0,    15,     0,   293,   294,     0,     0,     0,
      16,     0,    17,    18,    19,    20,     0,    21,     0,     0,
       0,    22,     0,    23,     0,     0,     0,    24,    25,    26,
      27,    28,     0,     0,     0,    99,     0,    29,    30,    31,
       1,     2,     3,     4,     5,     6,     7,     8,    99,     0,
       9,     0,     0,     0,   109,   101,   102,     0,     0,     0,
     103,     0,     0,     0,     0,     0,     0,     0,   101,   102,
       0,     0,     0,   103,     0,   123,     0,     0,     0,     0,
       0,    10,    11,     0,   104,   105,    12,     0,    13,     0,
       0,     0,   237,    14,     0,     0,     0,   104,   105,     0,
       0,    15,     0,     0,     0,     0,     0,     0,    16,     0,
       0,    18,    19,    20,     0,    21,     0,   178,   179,    22,
       0,    23,     0,     0,     0,     0,    25,    26,    27,   180,
       0,     0,     0,     0,     0,    29,    30,    31,     1,     2,
       3,     4,     5,     6,     7,     8,     0,     0,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,     0,     0,    12,     0,    13,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,    15,
       0,     0,     0,     0,     0,     0,    16,     0,    17,    18,
      19,    20,     0,    21,     0,     0,     0,    22,     0,    23,
       0,     0,     0,    24,    25,    26,    27,    28,     0,     0,
       0,     0,     0,    29,    30,    31,     1,     2,     3,     4,
       5,     6,     7,     8,     0,     0,     9,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     2,     3,
       4,     5,     6,     7,     8,     0,     0,     9,   147,     0,
       0,     0,     0,     0,     0,     0,     0,    10,    11,     0,
       0,     0,    12,     0,    13,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,    15,    10,    11,
       0,     0,     0,    12,    16,    13,     0,    18,    19,    20,
      14,    21,     0,   178,   179,    22,     0,    23,    15,     0,
       0,     0,    25,    26,    27,   180,     0,     0,    18,     0,
       0,    29,    30,    31,     0,     1,     2,     3,     4,     5,
       6,     7,     8,    25,    26,     9,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
       0,    12,     0,    13,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,    15,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,     0,     0,
      29,    30,    31
  };

  const short
  parser::yycheck_[] =
  {
       0,    68,   106,   107,   108,    34,     0,    18,   136,     9,
       9,   129,    12,    13,   132,    15,     6,   121,   122,    19,
      20,    21,    22,   205,   123,     6,    90,     0,     6,     0,
       0,   135,     9,    20,    34,   280,    34,    71,    14,    73,
      34,    20,     6,   161,    19,    46,    47,   175,     6,    10,
      11,    15,     6,    14,     5,    16,     6,     7,    58,    58,
      59,    34,    49,    17,    98,    46,    90,    65,    46,    47,
      49,    19,   136,   201,   319,   203,    51,    52,    19,    90,
      50,    58,    46,   182,     7,   267,    57,    57,    46,    65,
      65,    91,    46,   104,   123,    76,    46,    20,    76,    14,
     100,   101,   102,    51,    65,   105,    67,    97,    19,    18,
      51,   175,   136,     6,    23,   219,   116,    65,    27,    28,
     187,     0,    45,   123,   124,   136,   126,    92,   128,   123,
      45,    92,    93,    94,    43,   214,   215,   201,    49,   203,
      20,    14,   270,   104,    48,   106,   107,   108,    19,   149,
     123,   175,   113,   182,   188,   189,   190,    74,   276,    68,
     121,   122,   162,    47,   175,    69,    46,    47,   129,   130,
     298,   132,    54,    45,   135,    92,     9,   201,    49,   203,
      51,    90,   182,   262,     6,    94,   220,    69,   182,    20,
     201,    52,   203,    15,    65,    47,    29,    30,   150,   151,
     161,    34,    20,     6,     7,   205,   270,   241,    69,   182,
     289,   290,    15,   317,    45,    14,   295,    16,   252,    47,
     129,    46,   222,   132,    49,    58,    59,   136,    46,    47,
      47,    49,   311,   312,   298,    47,   197,    20,   147,     6,
       7,   150,   151,    46,    47,    46,   270,    20,    51,    16,
      17,    34,   161,    20,    47,   207,    46,    47,   219,   270,
     221,    45,    45,   297,    46,    47,   175,   267,    46,    47,
       9,   180,    45,   149,   298,    81,    47,   277,   187,    46,
      48,   157,   316,    69,    48,    48,   247,   298,    91,    92,
      29,    30,   201,    69,   203,    34,    46,    47,   207,    49,
      48,    48,    48,    48,    57,     9,    50,   259,   260,   261,
      14,    49,   221,     6,     7,   276,   268,   226,   318,    58,
      59,   282,    48,    16,    17,    29,    30,    50,    69,   205,
      34,    98,    99,   209,   286,   287,     6,     7,   214,   215,
      79,   293,   294,    48,    10,   306,    16,    17,    48,    45,
     259,   260,   261,    46,    58,    59,   317,    16,    34,   268,
      48,   270,    20,     6,     7,    48,    69,   276,    52,    48,
     279,   280,    19,    16,    17,    47,    46,   286,   287,    48,
      48,    48,   258,    34,   293,   294,   262,     0,   182,   298,
     247,   267,     5,     6,     7,     8,     9,    10,    11,    12,
      11,     9,    15,    46,    13,    98,    99,     6,     7,   306,
     319,   113,   288,   289,   290,   304,   292,    16,    17,   295,
     282,    29,    30,   221,   226,    -1,    34,    -1,    98,    99,
      -1,   307,   308,    46,    47,   311,   312,    -1,    51,    -1,
      53,    49,    -1,    -1,    57,    58,    -1,    46,    -1,    57,
      58,    59,    -1,    66,    -1,    98,    99,    -1,    -1,    -1,
      73,    -1,    75,    76,    77,    78,    -1,    80,    -1,    -1,
      -1,    84,    -1,    86,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    -1,    -1,    -1,     9,    -1,   100,   101,   102,
       5,     6,     7,     8,     9,    10,    11,    12,     9,    -1,
      15,    -1,    -1,    -1,    15,    29,    30,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    -1,    34,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    58,    59,    51,    -1,    53,    -1,
      -1,    -1,    57,    58,    -1,    -1,    -1,    58,    59,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    76,    77,    78,    -1,    80,    -1,    82,    83,    84,
      -1,    86,    -1,    -1,    -1,    -1,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,     5,     6,
       7,     8,     9,    10,    11,    12,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    -1,    51,    -1,    53,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    75,    76,
      77,    78,    -1,    80,    -1,    -1,    -1,    84,    -1,    86,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    46,    47,
      -1,    -1,    -1,    51,    73,    53,    -1,    76,    77,    78,
      58,    80,    -1,    82,    83,    84,    -1,    86,    66,    -1,
      -1,    -1,    91,    92,    93,    94,    -1,    -1,    76,    -1,
      -1,   100,   101,   102,    -1,     5,     6,     7,     8,     9,
      10,    11,    12,    91,    92,    15,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    51,    53,    58,    66,    73,    75,    76,    77,
      78,    80,    84,    86,    90,    91,    92,    93,    94,   100,
     101,   102,   104,   105,   106,   107,   108,   109,   110,   114,
     116,   118,   120,   121,   151,   152,   153,   154,   155,   156,
     157,   121,   151,   157,   118,   118,   132,   133,   134,   151,
     130,   151,   130,   118,   151,    97,   118,    74,    92,   120,
     153,   151,   151,   151,   151,   120,   129,     5,    92,   120,
     124,   120,   128,     0,     0,    57,     0,    57,   107,   114,
      45,    14,    46,    47,    47,    76,   120,   122,   123,     9,
      14,    29,    30,    34,    58,    59,    47,    47,    47,    15,
      14,    45,    48,    69,   151,    52,    69,    54,   118,   118,
     123,    47,    47,    49,    57,   113,    79,   113,    20,    47,
      49,    20,    47,   113,     7,    47,    51,   120,   153,   155,
     156,   158,   151,   118,   131,   132,     7,    16,    17,    20,
      98,    99,   118,   120,   140,   141,   144,   145,   148,   149,
     150,    47,    20,   113,   151,   151,   151,   118,   153,   151,
     131,   131,   131,   133,   151,    45,   131,   131,    82,    83,
      94,   111,   112,   114,   115,   116,   117,   121,   151,   151,
      81,   151,   140,   118,   125,   126,   127,    47,   140,   131,
     158,    19,    51,    65,    48,    20,    34,    45,   120,   146,
     147,   150,   151,     7,   145,   145,     7,   119,   142,    47,
      48,    69,    14,   143,   150,    34,    65,   140,   151,    48,
      48,    48,   158,    48,    48,   120,    57,    57,   114,   117,
      50,   123,   113,   113,   113,    48,    50,    69,   118,   135,
     136,   138,    48,    48,    52,   158,    10,   158,   146,    45,
      98,    99,   145,   150,    34,   143,   143,    20,    45,   131,
      19,   113,   144,   151,   149,    48,    47,    20,   127,    20,
      45,    48,    69,    20,   113,    52,    98,    99,   145,   145,
     145,   143,   146,    98,    99,   145,    48,   158,    19,   140,
     151,     7,   120,   139,   139,   138,    47,   145,   145,   143,
     143,   145,   145,   143,   113,   158,    48,    47,    14,    65,
     137,   137,   135,   143,   143,   131,   151,   139,    48,    48
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
     139,   139,   139,   140,   140,   141,   141,   142,   142,   143,
     143,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   145,   145,   145,   146,
     146,   146,   147,   148,   148,   148,   148,   149,   149,   149,
     149,   150,   151,   151,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   153,
     154,   154,   154,   154,   155,   155,   155,   155,   156,   156,
     157,   157,   158,   158,   158,   158,   158,   158,   158,   158
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
       2,     0,     1,     1,     3,     0,     2,     4,     4,     1,
       4,     1,     3,     0,     1,     1,     3,     0,     1,     0,
       2,     5,     6,     6,     4,     4,     5,     5,     3,     4,
       5,     5,     2,     3,     3,     2,     1,     1,     2,     1,
       1,     2,     1,     1,     2,     1,     2,     1,     1,     4,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     2,     3,     3,
       2,     2,     3,     3,     3,     1,     1,     1,     1,     3,
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
  "field-default-value-opt", "field", "field-type-expr",
  "parameter-list-opt", "parameter-list", "internal-identifier-opt",
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
       0,   338,   338,   339,   343,   345,   347,   364,   366,   367,
     369,   374,   376,   378,   384,   391,   393,   398,   400,   405,
     407,   412,   413,   418,   420,   422,   427,   429,   430,   432,
     437,   442,   444,   446,   448,   450,   452,   454,   456,   458,
     463,   465,   470,   472,   474,   476,   478,   483,   485,   487,
     492,   500,   515,   517,   519,   525,   527,   532,   533,   537,
     539,   545,   550,   551,   555,   557,   562,   568,   570,   593,
     597,   611,   613,   619,   620,   624,   626,   631,   633,   638,
     640,   646,   647,   651,   653,   658,   659,   663,   665,   675,
     677,   679,   681,   688,   689,   693,   695,   700,   701,   705,
     706,   710,   712,   714,   716,   719,   721,   723,   725,   728,
     730,   732,   735,   737,   739,   741,   746,   748,   750,   755,
     757,   759,   779,   783,   784,   786,   788,   794,   800,   803,
     805,   828,   833,   835,   840,   842,   844,   846,   848,   850,
     852,   854,   856,   858,   860,   868,   876,   878,   882,   884,
     886,   888,   890,   896,   898,   901,   902,   903,   904,   908,
     913,   915,   917,   919,   924,   926,   928,   930,   935,   937,
     942,   943,  1030,  1032,  1035,  1039,  1041,  1044,  1046,  1054
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
#line 4411 "bang.tab.cpp"

#line 1418 "bang.y"

