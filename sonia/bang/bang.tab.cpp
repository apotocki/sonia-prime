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

      case symbol_kind::S_122_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
      case symbol_kind::S_133_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_let_decl: // let-decl
      case symbol_kind::S_107_let_decl_start: // let-decl-start
      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_111_braced_statements: // braced-statements
      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
      case symbol_kind::S_129_argument_list: // argument-list
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_138_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_117_internal_identifier: // internal-identifier
      case symbol_kind::S_131_argument_name: // argument-name
      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_120_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_decl: // case-decl
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

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_115_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
      case symbol_kind::S_124_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_147_concept_expression: // concept-expression
      case symbol_kind::S_148_syntax_expression: // syntax-expression
      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_151_new_expression: // new-expression
      case symbol_kind::S_152_call_expression: // call-expression
      case symbol_kind::S_153_lambda_expression: // lambda-expression
      case symbol_kind::S_154_compound_expression: // compound-expression
      case symbol_kind::S_155_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_using_decl: // using-decl
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

      case symbol_kind::S_122_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
      case symbol_kind::S_133_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_106_let_decl: // let-decl
      case symbol_kind::S_107_let_decl_start: // let-decl-start
      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_111_braced_statements: // braced-statements
      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
      case symbol_kind::S_129_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_138_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_117_internal_identifier: // internal-identifier
      case symbol_kind::S_131_argument_name: // argument-name
      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_120_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_case_decl: // case-decl
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

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_115_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
      case symbol_kind::S_124_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_147_concept_expression: // concept-expression
      case symbol_kind::S_148_syntax_expression: // syntax-expression
      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_151_new_expression: // new-expression
      case symbol_kind::S_152_call_expression: // call-expression
      case symbol_kind::S_153_lambda_expression: // lambda-expression
      case symbol_kind::S_154_compound_expression: // compound-expression
      case symbol_kind::S_155_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_using_decl: // using-decl
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

      case symbol_kind::S_122_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
      case symbol_kind::S_133_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_let_decl: // let-decl
      case symbol_kind::S_107_let_decl_start: // let-decl-start
      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_111_braced_statements: // braced-statements
      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
      case symbol_kind::S_129_argument_list: // argument-list
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_138_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_117_internal_identifier: // internal-identifier
      case symbol_kind::S_131_argument_name: // argument-name
      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_120_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_decl: // case-decl
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

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
        value.YY_MOVE_OR_COPY< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_115_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
      case symbol_kind::S_124_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_147_concept_expression: // concept-expression
      case symbol_kind::S_148_syntax_expression: // syntax-expression
      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_151_new_expression: // new-expression
      case symbol_kind::S_152_call_expression: // call-expression
      case symbol_kind::S_153_lambda_expression: // lambda-expression
      case symbol_kind::S_154_compound_expression: // compound-expression
      case symbol_kind::S_155_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_using_decl: // using-decl
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

      case symbol_kind::S_122_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
      case symbol_kind::S_133_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_let_decl: // let-decl
      case symbol_kind::S_107_let_decl_start: // let-decl-start
      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_111_braced_statements: // braced-statements
      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
      case symbol_kind::S_129_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_138_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_117_internal_identifier: // internal-identifier
      case symbol_kind::S_131_argument_name: // argument-name
      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_120_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_decl: // case-decl
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

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_115_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
      case symbol_kind::S_124_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_147_concept_expression: // concept-expression
      case symbol_kind::S_148_syntax_expression: // syntax-expression
      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_151_new_expression: // new-expression
      case symbol_kind::S_152_call_expression: // call-expression
      case symbol_kind::S_153_lambda_expression: // lambda-expression
      case symbol_kind::S_154_compound_expression: // compound-expression
      case symbol_kind::S_155_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_using_decl: // using-decl
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

      case symbol_kind::S_122_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
      case symbol_kind::S_133_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_106_let_decl: // let-decl
      case symbol_kind::S_107_let_decl_start: // let-decl-start
      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_111_braced_statements: // braced-statements
      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
      case symbol_kind::S_129_argument_list: // argument-list
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_138_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_117_internal_identifier: // internal-identifier
      case symbol_kind::S_131_argument_name: // argument-name
      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_120_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_125_case_decl: // case-decl
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

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_115_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
      case symbol_kind::S_124_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_147_concept_expression: // concept-expression
      case symbol_kind::S_148_syntax_expression: // syntax-expression
      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_151_new_expression: // new-expression
      case symbol_kind::S_152_call_expression: // call-expression
      case symbol_kind::S_153_lambda_expression: // lambda-expression
      case symbol_kind::S_154_compound_expression: // compound-expression
      case symbol_kind::S_155_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_127_using_decl: // using-decl
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

      case symbol_kind::S_122_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
      case symbol_kind::S_133_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_106_let_decl: // let-decl
      case symbol_kind::S_107_let_decl_start: // let-decl-start
      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_111_braced_statements: // braced-statements
      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
      case symbol_kind::S_129_argument_list: // argument-list
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_138_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_117_internal_identifier: // internal-identifier
      case symbol_kind::S_131_argument_name: // argument-name
      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_120_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_125_case_decl: // case-decl
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

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_115_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
      case symbol_kind::S_124_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_147_concept_expression: // concept-expression
      case symbol_kind::S_148_syntax_expression: // syntax-expression
      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_151_new_expression: // new-expression
      case symbol_kind::S_152_call_expression: // call-expression
      case symbol_kind::S_153_lambda_expression: // lambda-expression
      case symbol_kind::S_154_compound_expression: // compound-expression
      case symbol_kind::S_155_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_127_using_decl: // using-decl
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
#line 330 "bang.y"
                 { }
#line 1464 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 330 "bang.y"
                 { }
#line 1470 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 330 "bang.y"
                 { }
#line 1476 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 330 "bang.y"
                 { }
#line 1482 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 330 "bang.y"
                 { }
#line 1488 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 330 "bang.y"
                 { }
#line 1494 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 330 "bang.y"
                 { }
#line 1500 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 330 "bang.y"
                 { }
#line 1506 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 330 "bang.y"
                 { }
#line 1512 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 330 "bang.y"
                 { }
#line 1518 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 330 "bang.y"
                 { }
#line 1524 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 330 "bang.y"
                 { }
#line 1530 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 330 "bang.y"
                 { }
#line 1536 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 330 "bang.y"
                 { }
#line 1542 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 330 "bang.y"
                 { }
#line 1548 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 330 "bang.y"
                 { }
#line 1554 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 330 "bang.y"
                 { }
#line 1560 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 330 "bang.y"
                 { }
#line 1566 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 330 "bang.y"
                 { }
#line 1572 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 330 "bang.y"
                 { }
#line 1578 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 330 "bang.y"
                 { }
#line 1584 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 330 "bang.y"
                 { }
#line 1590 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 330 "bang.y"
                 { }
#line 1596 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 330 "bang.y"
                 { }
#line 1602 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 330 "bang.y"
                 { }
#line 1608 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 330 "bang.y"
                 { }
#line 1614 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 330 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 330 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 330 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 330 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 330 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 330 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
#line 330 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 330 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_let_decl: // let-decl
#line 330 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_let_decl_start: // let-decl-start
#line 330 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 330 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
#line 330 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
#line 330 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_braced_statements: // braced-statements
#line 330 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
#line 330 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
#line 330 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
#line 330 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_infunction_statement: // infunction-statement
#line 330 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 330 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_internal_identifier: // internal-identifier
#line 330 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 330 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
#line 330 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_fn_name: // fn-name
#line 330 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
#line 330 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_enum_decl: // enum-decl
#line 330 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
#line 330 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_case_list: // case-list
#line 330 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_case_decl: // case-decl
#line 330 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
#line 330 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_using_decl: // using-decl
#line 330 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
#line 330 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_argument_list: // argument-list
#line 330 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 330 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_argument_name: // argument-name
#line 330 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
#line 330 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_field_list: // field-list
#line 330 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
#line 330 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 330 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
#line 330 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
#line 330 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_parameter_list: // parameter-list
#line 330 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
#line 330 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
#line 330 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
#line 330 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
#line 330 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
#line 330 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
#line 330 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
#line 330 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
#line 330 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_concept_expression: // concept-expression
#line 330 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_syntax_expression: // syntax-expression
#line 330 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 330 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
#line 330 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_new_expression: // new-expression
#line 330 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_call_expression: // call-expression
#line 330 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_lambda_expression: // lambda-expression
#line 330 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_compound_expression: // compound-expression
#line 330 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_type_expr: // type-expr
#line 330 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
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

      case symbol_kind::S_122_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_132_field_list_opt: // field-list-opt
      case symbol_kind::S_133_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_112_finished_statement: // finished-statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_119_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_121_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_114_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_106_let_decl: // let-decl
      case symbol_kind::S_107_let_decl_start: // let-decl-start
      case symbol_kind::S_108_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_104_finished_statement_any: // finished-statement-any
      case symbol_kind::S_109_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_110_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_111_braced_statements: // braced-statements
      case symbol_kind::S_113_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< managed_statement_list > ();
        break;

      case symbol_kind::S_128_argument_list_opt: // argument-list-opt
      case symbol_kind::S_129_argument_list: // argument-list
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_142_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_143_parameter_value_constraint_set: // parameter-value-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_137_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_138_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_141_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_117_internal_identifier: // internal-identifier
      case symbol_kind::S_131_argument_name: // argument-name
      case symbol_kind::S_139_internal_identifier_opt: // internal-identifier-opt
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_120_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_125_case_decl: // case-decl
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

      case symbol_kind::S_134_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_140_parameter_default_value_opt: // parameter-default-value-opt
        yylhs.value.emplace< sonia::optional<syntax_expression_t> > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_115_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_123_case_list_opt: // case-list-opt
      case symbol_kind::S_124_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_126_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_136_field_type_expr: // field-type-expr
      case symbol_kind::S_144_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_145_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_146_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_147_concept_expression: // concept-expression
      case symbol_kind::S_148_syntax_expression: // syntax-expression
      case symbol_kind::S_149_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_150_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_151_new_expression: // new-expression
      case symbol_kind::S_152_call_expression: // call-expression
      case symbol_kind::S_153_lambda_expression: // lambda-expression
      case symbol_kind::S_154_compound_expression: // compound-expression
      case symbol_kind::S_155_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_127_using_decl: // using-decl
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
#line 335 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2388 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 336 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2394 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 341 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2400 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 343 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2406 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 345 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2412 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 362 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2418 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 363 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2424 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 365 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2430 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 367 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2436 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 372 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2442 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 374 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2448 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 376 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2454 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 382 "bang.y"
        {
            yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > ();
            auto sts = ctx.new_statement_list();
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) });
            yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), ctx.push(std::move(sts)) };
        }
#line 2465 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 389 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2471 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 391 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2477 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 396 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2483 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 398 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2489 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: LET identifier
#line 403 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2495 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: LET "weak modifier" identifier
#line 405 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2501 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 409 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2507 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 411 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2513 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 416 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2519 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 418 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2525 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 420 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2531 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 425 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2537 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 426 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2543 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 428 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2549 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 430 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2555 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 435 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2561 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 440 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2567 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 442 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2573 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 444 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2579 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 446 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2585 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 448 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2591 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 450 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2597 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 452 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2603 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 454 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2609 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 456 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2615 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 461 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2621 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 463 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2627 "bang.tab.cpp"
    break;

  case 42: // generic-statement: let-decl
#line 468 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2633 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 470 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2639 "bang.tab.cpp"
    break;

  case 44: // generic-statement: "`return`" syntax-expression
#line 472 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2645 "bang.tab.cpp"
    break;

  case 45: // generic-statement: compound-expression
#line 474 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2651 "bang.tab.cpp"
    break;

  case 46: // generic-statement: syntax-expression "`=`" syntax-expression
#line 476 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2657 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: generic-statement
#line 481 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2663 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`break`"
#line 483 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2669 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`continue`"
#line 485 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2675 "bang.tab.cpp"
    break;

  case 50: // identifier: IDENTIFIER
#line 490 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2681 "bang.tab.cpp"
    break;

  case 51: // internal-identifier: INTERNAL_IDENTIFIER
#line 498 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2687 "bang.tab.cpp"
    break;

  case 52: // qname: "`::`" identifier
#line 513 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2693 "bang.tab.cpp"
    break;

  case 53: // qname: identifier
#line 515 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2699 "bang.tab.cpp"
    break;

  case 54: // qname: qname "`::`" identifier
#line 517 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2705 "bang.tab.cpp"
    break;

  case 55: // fn-start-decl: "`fn`"
#line 523 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2711 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: INLINE "`fn`"
#line 525 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2717 "bang.tab.cpp"
    break;

  case 57: // fn-name: qname
#line 529 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2723 "bang.tab.cpp"
    break;

  case 58: // fn-name: "`new`"
#line 530 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2729 "bang.tab.cpp"
    break;

  case 59: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 535 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_list_t > ()), nullopt }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2735 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" type-expr
#line 537 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2741 "bang.tab.cpp"
    break;

  case 61: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 543 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2747 "bang.tab.cpp"
    break;

  case 62: // case-list-opt: %empty
#line 547 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2753 "bang.tab.cpp"
    break;

  case 63: // case-list-opt: case-list
#line 548 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2759 "bang.tab.cpp"
    break;

  case 64: // case-list: case-decl
#line 553 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2765 "bang.tab.cpp"
    break;

  case 65: // case-list: case-list "," case-decl
#line 555 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2771 "bang.tab.cpp"
    break;

  case 66: // case-decl: identifier
#line 560 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2777 "bang.tab.cpp"
    break;

  case 67: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 566 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2783 "bang.tab.cpp"
    break;

  case 68: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 568 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2789 "bang.tab.cpp"
    break;

  case 69: // using-decl: qname "`=>`" syntax-expression
#line 591 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 2797 "bang.tab.cpp"
    break;

  case 70: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 595 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 2805 "bang.tab.cpp"
    break;

  case 71: // argument-list-opt: %empty
#line 603 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2811 "bang.tab.cpp"
    break;

  case 72: // argument-list-opt: argument-list
#line 604 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2817 "bang.tab.cpp"
    break;

  case 73: // argument-list: argument
#line 609 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2823 "bang.tab.cpp"
    break;

  case 74: // argument-list: argument-list "," argument
#line 611 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2829 "bang.tab.cpp"
    break;

  case 75: // argument: argument-name syntax-expression
#line 616 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2835 "bang.tab.cpp"
    break;

  case 76: // argument: syntax-expression
#line 618 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2841 "bang.tab.cpp"
    break;

  case 77: // argument-name: identifier "`=`"
#line 623 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2847 "bang.tab.cpp"
    break;

  case 78: // argument-name: identifier "`:`"
#line 625 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2853 "bang.tab.cpp"
    break;

  case 79: // field-list-opt: %empty
#line 630 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2859 "bang.tab.cpp"
    break;

  case 80: // field-list-opt: field-list
#line 631 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2865 "bang.tab.cpp"
    break;

  case 81: // field-list: field
#line 636 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2871 "bang.tab.cpp"
    break;

  case 82: // field-list: field-list "," field
#line 638 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2877 "bang.tab.cpp"
    break;

  case 83: // field-default-value-opt: %empty
#line 642 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 2883 "bang.tab.cpp"
    break;

  case 84: // field-default-value-opt: "`=`" syntax-expression
#line 643 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2889 "bang.tab.cpp"
    break;

  case 85: // field: identifier "`:`" field-type-expr field-default-value-opt
#line 648 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2895 "bang.tab.cpp"
    break;

  case 86: // field: identifier "`=>`" field-type-expr field-default-value-opt
#line 650 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2901 "bang.tab.cpp"
    break;

  case 87: // field-type-expr: qname
#line 660 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2907 "bang.tab.cpp"
    break;

  case 88: // field-type-expr: qname "`(`" argument-list-opt "`)`"
#line 662 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2913 "bang.tab.cpp"
    break;

  case 89: // field-type-expr: INTERNAL_IDENTIFIER
#line 664 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2919 "bang.tab.cpp"
    break;

  case 90: // field-type-expr: field-type-expr "`|`" field-type-expr
#line 666 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2925 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 672 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 2931 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 673 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 2937 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 678 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2943 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 680 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2949 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 684 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 2955 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 685 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 2961 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 689 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 2967 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 690 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2973 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" parameter-constraint-set parameter-default-value-opt
#line 695 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2979 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: identifier internal-identifier-opt "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 697 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2985 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: identifier internal-identifier-opt "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 699 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2991 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`=>`" parameter-value-constraint-set
#line 701 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2997 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: INTERNAL_IDENTIFIER "`:`" parameter-constraint-set parameter-default-value-opt
#line 704 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3003 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 706 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3009 "bang.tab.cpp"
    break;

  case 105: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 708 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3015 "bang.tab.cpp"
    break;

  case 106: // parameter-decl: INTERNAL_IDENTIFIER "`=>`" parameter-value-constraint-set
#line 710 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3021 "bang.tab.cpp"
    break;

  case 107: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" parameter-constraint-set
#line 713 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3027 "bang.tab.cpp"
    break;

  case 108: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "const modifier" parameter-constraint-set
#line 715 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3033 "bang.tab.cpp"
    break;

  case 109: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "mut modifier" parameter-constraint-set
#line 717 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3039 "bang.tab.cpp"
    break;

  case 110: // parameter-decl: parameter-constraint-set parameter-default-value-opt
#line 720 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3045 "bang.tab.cpp"
    break;

  case 111: // parameter-decl: "const modifier" parameter-constraint-set parameter-default-value-opt
#line 722 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3051 "bang.tab.cpp"
    break;

  case 112: // parameter-decl: "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 724 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3057 "bang.tab.cpp"
    break;

  case 113: // parameter-decl: "`=>`" parameter-value-constraint-set
#line 726 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3063 "bang.tab.cpp"
    break;

  case 114: // parameter-constraint-set: parameter-matched-type
#line 731 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3069 "bang.tab.cpp"
    break;

  case 115: // parameter-constraint-set: concept-expression
#line 733 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3075 "bang.tab.cpp"
    break;

  case 116: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 735 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3081 "bang.tab.cpp"
    break;

  case 117: // parameter-value-constraint-set: parameter-matched-value
#line 740 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3087 "bang.tab.cpp"
    break;

  case 118: // parameter-value-constraint-set: concept-expression
#line 742 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3093 "bang.tab.cpp"
    break;

  case 119: // parameter-value-constraint-set: parameter-value-constraint-set concept-expression
#line 744 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3099 "bang.tab.cpp"
    break;

  case 120: // parameter-matched-value: syntax-expression
#line 763 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3105 "bang.tab.cpp"
    break;

  case 121: // parameter-matched-type: basic-parameter-matched-type
#line 767 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3111 "bang.tab.cpp"
    break;

  case 122: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 769 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3117 "bang.tab.cpp"
    break;

  case 123: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 771 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3123 "bang.tab.cpp"
    break;

  case 124: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 773 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3129 "bang.tab.cpp"
    break;

  case 125: // basic-parameter-matched-type: "`_`"
#line 779 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3135 "bang.tab.cpp"
    break;

  case 126: // basic-parameter-matched-type: qname
#line 785 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3141 "bang.tab.cpp"
    break;

  case 127: // basic-parameter-matched-type: qname "`(`" argument-list-opt "`)`"
#line 788 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3147 "bang.tab.cpp"
    break;

  case 128: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 790 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3153 "bang.tab.cpp"
    break;

  case 129: // concept-expression: "`@`" qname
#line 813 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3159 "bang.tab.cpp"
    break;

  case 130: // syntax-expression: INTERNAL_IDENTIFIER
#line 818 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3165 "bang.tab.cpp"
    break;

  case 131: // syntax-expression: syntax-expression-wo-ii
#line 819 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3171 "bang.tab.cpp"
    break;

  case 132: // syntax-expression-wo-ii: "nil"
#line 825 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3177 "bang.tab.cpp"
    break;

  case 133: // syntax-expression-wo-ii: "true"
#line 827 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3183 "bang.tab.cpp"
    break;

  case 134: // syntax-expression-wo-ii: "false"
#line 829 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3189 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: INTEGER
#line 831 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3195 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: DECIMAL
#line 833 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3201 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: DECIMAL_S
#line 835 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3207 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: INTEGER_INDEX
#line 837 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3213 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: STRING
#line 839 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3219 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 841 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3225 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: qname
#line 843 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3231 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: "`(`" argument-list "`)`"
#line 845 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) };
            }
        }
#line 3243 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: "`.`" identifier
#line 853 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3249 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 857 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3255 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 859 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3261 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 861 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3267 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 863 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3273 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 865 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3279 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 871 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3285 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 873 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3291 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: apostrophe-expression
#line 875 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3297 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: new-expression
#line 876 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3303 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: compound-expression
#line 877 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3309 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: lambda-expression
#line 878 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3315 "bang.tab.cpp"
    break;

  case 155: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 883 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3321 "bang.tab.cpp"
    break;

  case 156: // new-expression: "`new`" qname
#line 888 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3327 "bang.tab.cpp"
    break;

  case 157: // new-expression: "`new`" apostrophe-expression
#line 890 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3333 "bang.tab.cpp"
    break;

  case 158: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 892 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3339 "bang.tab.cpp"
    break;

  case 159: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 894 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3345 "bang.tab.cpp"
    break;

  case 160: // call-expression: qname "`(`" argument-list-opt "`)`"
#line 899 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3351 "bang.tab.cpp"
    break;

  case 161: // call-expression: call-expression "`(`" argument-list-opt "`)`"
#line 901 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3357 "bang.tab.cpp"
    break;

  case 162: // call-expression: apostrophe-expression "`(`" argument-list-opt "`)`"
#line 903 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3363 "bang.tab.cpp"
    break;

  case 163: // call-expression: lambda-expression "`(`" argument-list-opt "`)`"
#line 905 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3369 "bang.tab.cpp"
    break;

  case 164: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 910 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3375 "bang.tab.cpp"
    break;

  case 165: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 912 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3381 "bang.tab.cpp"
    break;

  case 166: // compound-expression: call-expression
#line 916 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3387 "bang.tab.cpp"
    break;

  case 167: // compound-expression: syntax-expression "`...`"
#line 918 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3393 "bang.tab.cpp"
    break;

  case 168: // type-expr: qname
#line 1005 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3399 "bang.tab.cpp"
    break;

  case 169: // type-expr: call-expression
#line 1006 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3405 "bang.tab.cpp"
    break;

  case 170: // type-expr: INTERNAL_IDENTIFIER
#line 1010 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3411 "bang.tab.cpp"
    break;

  case 171: // type-expr: "`[`" type-expr "`]`"
#line 1014 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3417 "bang.tab.cpp"
    break;

  case 172: // type-expr: "`(`" argument-list-opt "`)`"
#line 1016 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3423 "bang.tab.cpp"
    break;

  case 173: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1019 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_array_t{std::move(yystack_[3].value.as < syntax_expression_t > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3429 "bang.tab.cpp"
    break;

  case 174: // type-expr: type-expr "`|`" type-expr
#line 1021 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3441 "bang.tab.cpp"
    break;

  case 175: // type-expr: type-expr "`->`" type-expr
#line 1029 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3447 "bang.tab.cpp"
    break;


#line 3451 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -244;

  const signed char parser::yytable_ninf_ = -96;

  const short
  parser::yypact_[] =
  {
     565,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,   492,
       2,   492,     2,   492,    -3,    63,    93,   492,   492,   492,
     492,    30,    19,    11,  -244,    30,    30,  -244,  -244,  -244,
     109,    43,   373,  -244,  -244,    98,   160,  -244,  -244,  -244,
     220,     4,   168,  -244,   111,  -244,   137,   167,    27,   187,
     665,  -244,  -244,   119,   193,  -244,   492,   576,  -244,    -4,
       2,  -244,     2,    33,   230,   190,   570,   214,   384,   576,
     226,  -244,  -244,  -244,   102,  -244,   239,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,    15,   492,     2,   492,   294,
    -244,   201,   192,   172,  -244,   492,   492,   492,  -244,   163,
     492,   492,   492,   492,  -244,  -244,  -244,  -244,   492,   576,
    -244,   207,  -244,   492,   492,   661,   492,  -244,   492,   175,
     492,   294,     2,   210,   294,  -244,  -244,   492,    15,   220,
     111,   137,   167,   200,   576,  -244,   246,   195,   197,    30,
    -244,   396,   286,   286,   155,   235,   256,   248,  -244,   216,
    -244,   145,  -244,   294,   492,  -244,   576,   129,   129,  -244,
    -244,    -4,   268,   271,   273,  -244,    15,   275,   278,  -244,
    -244,    30,   209,   469,  -244,   279,  -244,  -244,     4,   384,
     384,   284,   576,   280,  -244,   289,   267,  -244,     2,   293,
     295,    51,    15,   332,    15,  -244,   396,   300,   370,   201,
     335,  -244,  -244,   576,   319,   216,   216,  -244,  -244,   -16,
     492,   141,   294,   492,  -244,  -244,  -244,   166,   306,   576,
    -244,  -244,  -244,   200,  -244,  -244,    41,  -244,  -244,  -244,
    -244,  -244,   284,  -244,  -244,  -244,   336,  -244,     2,    20,
     309,   291,  -244,   189,  -244,  -244,  -244,   307,   -13,   335,
     393,   286,   286,   216,  -244,  -244,  -244,  -244,   396,   506,
     312,    15,  -244,  -244,   576,  -244,   342,   294,   492,  -244,
     159,   159,  -244,     2,   317,  -244,  -244,   286,   286,   335,
     216,   216,  -244,   335,   286,   286,   216,  -244,   144,    15,
     318,   576,  -244,   261,    12,    12,  -244,     2,   335,   335,
    -244,  -244,   216,   216,  -244,  -244,   200,   284,   492,   492,
     159,  -244,  -244,   326,  -244,  -244,   327,   576,  -244,  -244,
    -244
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   139,    50,   130,   140,   138,   135,   136,   137,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,   132,   133,   134,
       0,     0,     0,     5,    42,    21,    17,     7,    15,    53,
     141,     0,     0,   131,   151,   152,   166,   154,   153,     0,
       0,   153,    52,    53,     0,    73,     0,    76,   143,   147,
       0,    19,     0,     0,   156,   157,     0,     0,     0,    44,
       0,    43,    13,    56,     0,    39,     0,    16,     1,     2,
      10,     3,     8,     6,     9,     0,     0,     0,    71,    91,
      58,    57,     0,     0,   146,     0,     0,     0,   167,     0,
       0,    71,    71,    71,   155,    77,    78,   142,     0,    75,
      20,     0,    12,    71,    71,    23,     0,    31,     0,    34,
       0,    91,    62,     0,    91,    37,   170,    71,     0,   168,
       0,   169,     0,    22,    18,    54,     0,    72,   123,     0,
     125,     0,     0,     0,    53,   126,     0,    92,    93,    97,
     114,   121,   115,    91,     0,    36,    46,   149,   150,   144,
     145,   148,     0,     0,     0,    74,     0,     0,     0,    49,
      48,     0,    40,    41,    26,     0,    47,    24,     0,     0,
       0,     0,    69,     0,    66,     0,    63,    64,    79,     0,
       0,     0,     0,     0,     0,   160,     0,   124,     0,   129,
     113,   117,   118,   120,   123,    97,    97,    51,    96,     0,
      71,     0,     0,     0,   110,   116,   122,     0,     0,    14,
     162,   161,   163,    11,   158,   159,     0,    29,    27,    28,
      25,    30,     0,    32,    33,    35,     0,    61,     0,     0,
       0,    80,    81,     0,   172,   171,   175,     0,   174,   106,
       0,     0,     0,    97,   119,   124,   111,   112,     0,     0,
       0,     0,   164,    94,    98,   128,    59,    91,     0,    65,
       0,     0,    67,     0,     0,    38,   173,     0,     0,   107,
      97,    97,   103,   102,     0,     0,    97,   127,     0,     0,
       0,    70,    89,    87,    83,    83,    82,    79,   108,   109,
     104,   105,    97,    97,    99,   165,    60,     0,    71,     0,
       0,    86,    85,     0,   100,   101,     0,    84,    90,    68,
      88
  };

  const short
  parser::yypgoto_[] =
  {
    -244,  -244,  -244,  -244,   333,  -244,  -244,  -244,  -244,  -244,
     -52,   -31,  -244,   -92,   219,    23,  -244,    28,    31,  -244,
     -48,  -244,  -244,  -244,   151,  -244,  -244,   -55,   383,   287,
    -244,    99,  -244,   103,   124,  -243,   -64,  -244,  -244,    44,
     203,    85,  -194,  -244,  -244,   204,    69,     0,  -244,    -5,
    -244,   -73,   -60,    42,  -121
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    30,    31,    32,    33,    34,    35,    36,   172,   173,
     125,    37,   175,    38,   177,    39,   208,    40,    49,    92,
      93,    75,   185,   186,   187,    77,    71,   136,   137,    55,
      56,   240,   241,   311,   242,   294,   146,   147,   209,   214,
     148,   149,   200,   201,   150,   151,   152,    57,    43,    44,
      45,    46,    47,    51,   133
  };

  const short
  parser::yytable_[] =
  {
      42,    84,   249,     2,   258,    94,   192,   191,     2,    50,
       2,    65,   131,    59,   117,   112,   119,    66,    67,    68,
      69,     2,   126,   176,    72,   132,   309,   -45,   295,   259,
       9,    41,    42,    52,    53,    58,     2,    61,   193,     2,
     270,   155,    48,    79,    64,   223,   162,   163,   164,    70,
      10,    89,    99,    74,    76,   131,   109,   183,   167,   168,
     189,    10,   127,    41,   283,   271,   128,   318,   132,    91,
     192,   246,   190,   248,    48,   310,    10,   -45,    90,    10,
     130,   176,   -45,   110,   174,   111,   134,    87,   267,   218,
     115,    91,    60,   131,   160,   156,   157,   158,    80,     2,
     161,    73,   193,   245,    23,    24,   132,    90,     9,    78,
     135,    53,   144,   129,   194,    42,   179,   145,   180,   131,
     182,   131,   159,   130,    53,    53,    53,   233,   234,   235,
     232,    53,   132,   105,   132,    62,    53,    53,    94,    10,
     288,   203,   229,    85,   144,   184,   178,   144,    87,   145,
      53,   122,   145,    63,   219,   260,   129,    48,   101,   262,
     261,   130,   207,   192,   106,     2,   292,   199,   306,     2,
     145,   145,     2,    42,    86,   -95,   144,    94,     9,   216,
     155,   145,    95,   140,   102,    99,   100,   130,   131,   130,
     115,   275,   154,   115,   129,   193,   203,    96,    97,   226,
     -95,   132,    98,   290,   178,    10,    91,   194,   217,   274,
     202,   239,    10,   264,   103,    48,   131,   196,   215,   192,
     129,   115,   129,    94,    99,   100,   145,   205,   206,   132,
     213,   197,   139,    53,    89,   144,   305,   114,   115,   153,
     145,   107,   198,    96,    97,   145,   120,    87,    98,   256,
     257,   193,   166,   316,   181,   275,   130,   188,   203,   123,
     108,   184,   108,   194,   227,   202,    87,    88,   291,   254,
      99,   100,    87,   121,   215,   215,    87,   113,   145,   145,
     145,    87,   210,   253,   130,    87,   124,   145,   115,   129,
     144,   118,     2,   204,   195,   145,   239,   282,   293,   293,
       2,   138,   139,   140,   211,   145,   145,    87,   308,   317,
     139,   140,   145,   145,   141,   212,   220,   129,   254,   221,
     239,   222,   215,   224,   300,   301,   225,   202,   236,   231,
     304,    53,    10,   115,   238,   279,   280,   281,   293,   237,
      10,   243,   247,   244,   286,   250,   314,   315,   215,   215,
     215,   139,   254,   255,   266,   215,   268,   272,   273,   276,
     287,   289,   298,   299,   297,    83,   307,   215,   215,   302,
     303,   215,   215,    81,   319,   320,     2,   204,     1,     2,
       3,     4,     5,     6,     7,     8,   139,   140,     9,   269,
     142,   143,   230,    94,    54,   165,   313,   296,   312,     2,
     204,     1,     2,     3,     4,     5,     6,     7,     8,   139,
     140,     9,   139,    96,    97,   263,    10,     0,    98,    10,
      11,   265,     0,     0,     0,     0,     0,     0,    82,    12,
       0,     0,     0,   115,     0,     0,     0,    13,     0,    10,
      99,   100,    10,    11,    14,     0,    15,    16,    17,    18,
       0,    19,    12,     0,     0,    20,     0,    21,     0,     0,
      13,    22,    23,    24,    25,    26,   251,   252,     0,     0,
      16,    27,    28,    29,     1,     2,     3,     4,     5,     6,
       7,     8,     0,     0,     9,    23,    24,     0,     0,   277,
     278,     0,     0,     0,    27,    28,    29,     1,     2,     3,
       4,     5,     6,     7,     8,     0,     0,     9,     0,     0,
       0,     0,     2,   204,     0,    10,    11,     0,     0,     0,
       0,     0,   139,   140,   228,    12,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,     0,    10,    11,
      14,     0,     0,    16,    17,    18,     0,    19,    12,   169,
     170,    20,    10,    21,     0,     0,    13,     0,    23,    24,
      25,   171,     0,     0,     0,     0,    16,    27,    28,    29,
       1,     2,     3,     4,     5,     6,     7,     8,     0,    94,
       9,    23,    24,     0,     0,    94,     0,     0,     0,     0,
      27,    28,    29,     0,     0,     0,     0,     0,     0,    96,
      97,     0,   284,   285,    98,    96,    97,     0,     0,     0,
      98,    10,    11,     0,     0,     0,     0,     0,     0,   115,
       0,    12,     0,     0,     0,   116,    99,   100,     0,    13,
       0,     0,    99,   100,     0,     0,    14,     0,    15,    16,
      17,    18,     0,    19,     0,     0,     0,    20,     0,    21,
       0,     0,     0,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,    27,    28,    29,     1,     2,     3,     4,
       5,     6,     7,     8,    94,     0,     9,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,    10,    11,     0,
       0,     0,     0,     0,     0,     0,     0,    12,     0,     0,
       0,    99,   100,     0,     0,    13,     0,     0,     0,     0,
       0,     0,    14,     0,     0,    16,    17,    18,     0,    19,
       0,   169,   170,    20,     0,    21,     0,     0,     0,     0,
      23,    24,    25,   171,     0,     0,     0,     0,     0,    27,
      28,    29
  };

  const short
  parser::yycheck_[] =
  {
       0,    32,   196,     6,    20,     9,    19,   128,     6,     9,
       6,    16,    85,    13,    66,    63,    68,    17,    18,    19,
      20,     6,     7,   115,     5,    85,    14,     0,   271,    45,
      15,     0,    32,    10,    11,    12,     6,    14,    51,     6,
      20,    93,     0,     0,    16,   166,   101,   102,   103,    21,
      46,    47,    56,    25,    26,   128,    56,   121,   113,   114,
     124,    46,    47,    32,   258,    45,    51,   310,   128,    41,
      19,   192,   127,   194,    32,    63,    46,    50,    74,    46,
      85,   173,    55,    60,   115,    62,    86,    46,    47,   153,
      49,    63,    95,   166,    99,    95,    96,    97,    55,     6,
     100,    90,    51,    52,    89,    90,   166,    74,    15,     0,
      87,    88,    89,    85,    63,   115,   116,    89,   118,   192,
     120,   194,    99,   128,   101,   102,   103,   179,   180,   181,
     178,   108,   192,    14,   194,    72,   113,   114,     9,    46,
     261,   141,   173,    45,   121,   122,   115,   124,    46,   121,
     127,    49,   124,    90,   154,   210,   128,   115,    47,   211,
      19,   166,     7,    19,    45,     6,     7,   139,   289,     6,
     142,   143,     6,   173,    14,    20,   153,     9,    15,    34,
     232,   153,    14,    17,    47,    56,    57,   192,   261,   194,
      49,   243,    20,    49,   166,    51,   196,    29,    30,   171,
      45,   261,    34,   267,   173,    46,   178,    63,    63,    20,
     141,   188,    46,   213,    47,   173,   289,    20,   149,    19,
     192,    49,   194,     9,    56,    57,   198,   142,   143,   289,
      14,    34,    16,   210,    47,   212,   288,    47,    49,    47,
     212,    48,    45,    29,    30,   217,    20,    46,    34,   205,
     206,    51,    45,   308,    79,   307,   261,    47,   258,    20,
      67,   238,    67,    63,    55,   196,    46,    47,   268,   200,
      56,    57,    46,    47,   205,   206,    46,    47,   250,   251,
     252,    46,    47,   198,   289,    46,    47,   259,    49,   261,
     267,    77,     6,     7,    48,   267,   273,   253,   270,   271,
       6,     7,    16,    17,    48,   277,   278,    46,    47,   309,
      16,    17,   284,   285,    20,    67,    48,   289,   249,    48,
     297,    48,   253,    48,   280,   281,    48,   258,    48,    50,
     286,   308,    46,    49,    67,   250,   251,   252,   310,    50,
      46,    48,    10,    48,   259,    45,   302,   303,   279,   280,
     281,    16,   283,    34,    48,   286,    20,    48,    67,    52,
      48,    19,   277,   278,    47,    32,    48,   298,   299,   284,
     285,   302,   303,     0,    48,    48,     6,     7,     5,     6,
       7,     8,     9,    10,    11,    12,    16,    17,    15,   238,
      96,    97,   173,     9,    11,   108,   297,   273,   295,     6,
       7,     5,     6,     7,     8,     9,    10,    11,    12,    16,
      17,    15,    16,    29,    30,   212,    46,    -1,    34,    46,
      47,   217,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    64,    -1,    46,
      56,    57,    46,    47,    71,    -1,    73,    74,    75,    76,
      -1,    78,    56,    -1,    -1,    82,    -1,    84,    -1,    -1,
      64,    88,    89,    90,    91,    92,    96,    97,    -1,    -1,
      74,    98,    99,   100,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    -1,    15,    89,    90,    -1,    -1,    96,
      97,    -1,    -1,    -1,    98,    99,   100,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,     6,     7,    -1,    46,    47,    -1,    -1,    -1,
      -1,    -1,    16,    17,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    46,    47,
      71,    -1,    -1,    74,    75,    76,    -1,    78,    56,    80,
      81,    82,    46,    84,    -1,    -1,    64,    -1,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    74,    98,    99,   100,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,     9,
      15,    89,    90,    -1,    -1,     9,    -1,    -1,    -1,    -1,
      98,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    -1,    96,    97,    34,    29,    30,    -1,    -1,    -1,
      34,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    56,    -1,    -1,    -1,    55,    56,    57,    -1,    64,
      -1,    -1,    56,    57,    -1,    -1,    71,    -1,    73,    74,
      75,    76,    -1,    78,    -1,    -1,    -1,    82,    -1,    84,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,     5,     6,     7,     8,
       9,    10,    11,    12,     9,    -1,    15,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    56,    57,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    74,    75,    76,    -1,    78,
      -1,    80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    56,    64,    71,    73,    74,    75,    76,    78,
      82,    84,    88,    89,    90,    91,    92,    98,    99,   100,
     102,   103,   104,   105,   106,   107,   108,   112,   114,   116,
     118,   119,   148,   149,   150,   151,   152,   153,   154,   119,
     148,   154,   116,   116,   129,   130,   131,   148,   116,   148,
      95,   116,    72,    90,   118,   150,   148,   148,   148,   148,
     118,   127,     5,    90,   118,   122,   118,   126,     0,     0,
      55,     0,    55,   105,   112,    45,    14,    46,    47,    47,
      74,   118,   120,   121,     9,    14,    29,    30,    34,    56,
      57,    47,    47,    47,    15,    14,    45,    48,    67,   148,
     116,   116,   121,    47,    47,    49,    55,   111,    77,   111,
      20,    47,    49,    20,    47,   111,     7,    47,    51,   118,
     150,   152,   153,   155,   148,   116,   128,   129,     7,    16,
      17,    20,    96,    97,   116,   118,   137,   138,   141,   142,
     145,   146,   147,    47,    20,   111,   148,   148,   148,   116,
     150,   148,   128,   128,   128,   130,    45,   128,   128,    80,
      81,    92,   109,   110,   112,   113,   114,   115,   119,   148,
     148,    79,   148,   137,   116,   123,   124,   125,    47,   137,
     128,   155,    19,    51,    63,    48,    20,    34,    45,   118,
     143,   144,   147,   148,     7,   142,   142,     7,   117,   139,
      47,    48,    67,    14,   140,   147,    34,    63,   137,   148,
      48,    48,    48,   155,    48,    48,   118,    55,    55,   112,
     115,    50,   121,   111,   111,   111,    48,    50,    67,   116,
     132,   133,   135,    48,    48,    52,   155,    10,   155,   143,
      45,    96,    97,   142,   147,    34,   140,   140,    20,    45,
     128,    19,   111,   141,   148,   146,    48,    47,    20,   125,
      20,    45,    48,    67,    20,   111,    52,    96,    97,   142,
     142,   142,   140,   143,    96,    97,   142,    48,   155,    19,
     137,   148,     7,   118,   136,   136,   135,    47,   142,   142,
     140,   140,   142,   142,   140,   111,   155,    48,    47,    14,
      63,   134,   134,   132,   140,   140,   128,   148,   136,    48,
      48
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   101,   102,   102,   103,   103,   103,   104,   104,   104,
     104,   105,   105,   105,   105,   105,   105,   106,   106,   107,
     107,   108,   108,   109,   109,   109,   110,   110,   110,   110,
     111,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     113,   113,   114,   114,   114,   114,   114,   115,   115,   115,
     116,   117,   118,   118,   118,   119,   119,   120,   120,   121,
     121,   122,   123,   123,   124,   124,   125,   126,   126,   127,
     127,   128,   128,   129,   129,   130,   130,   131,   131,   132,
     132,   133,   133,   134,   134,   135,   135,   136,   136,   136,
     136,   137,   137,   138,   138,   139,   139,   140,   140,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   142,   142,   142,   143,   143,   143,
     144,   145,   145,   145,   145,   146,   146,   146,   146,   147,
     148,   148,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   150,   151,   151,   151,   151,
     152,   152,   152,   152,   153,   153,   154,   154,   155,   155,
     155,   155,   155,   155,   155,   155
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
       6,     0,     1,     1,     3,     2,     1,     2,     2,     0,
       1,     1,     3,     0,     2,     4,     4,     1,     4,     1,
       3,     0,     1,     1,     3,     0,     1,     0,     2,     5,
       6,     6,     4,     4,     5,     5,     3,     4,     5,     5,
       2,     3,     3,     2,     1,     1,     2,     1,     1,     2,
       1,     1,     2,     1,     2,     1,     1,     4,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     3,     3,     2,     2,     3,     3,
       3,     1,     1,     1,     1,     3,     2,     2,     5,     5,
       4,     4,     4,     4,     5,     7,     1,     2,     1,     1,
       1,     3,     3,     4,     3,     3
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
  "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"", "\"`+`\"", "\"`-`\"",
  "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"", "\"`|`\"", "\"`!`\"",
  "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET",
  "VAR", "EXTERN", "\"`new`\"", "\"`while`\"", "\"`for`\"", "\"`in`\"",
  "\"`if`\"", "\"`else`\"", "\"`continue`\"", "\"`break`\"",
  "\"`return`\"", "AUTO", "USING", "THROW", "SIZEOF", "LOWEST", "INCLUDE",
  "INLINE", "\"`fn`\"", "ENUM", "STRUCT", "EXTENDS", "TYPENAME",
  "\"weak modifier\"", "\"const modifier\"", "\"mut modifier\"", "\"nil\"",
  "\"true\"", "\"false\"", "$accept", "begin", "statement_any",
  "finished-statement-any", "statement", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction-statement-any",
  "finished-infunction-statement-any", "braced-statements",
  "finished-statement", "infunction-statement-set", "generic-statement",
  "infunction-statement", "identifier", "internal-identifier", "qname",
  "fn-start-decl", "fn-name", "fn-decl", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "struct-decl", "using-decl",
  "argument-list-opt", "argument-list", "argument", "argument-name",
  "field-list-opt", "field-list", "field-default-value-opt", "field",
  "field-type-expr", "parameter-list-opt", "parameter-list",
  "internal-identifier-opt", "parameter-default-value-opt",
  "parameter-decl", "parameter-constraint-set",
  "parameter-value-constraint-set", "parameter-matched-value",
  "parameter-matched-type", "basic-parameter-matched-type",
  "concept-expression", "syntax-expression", "syntax-expression-wo-ii",
  "apostrophe-expression", "new-expression", "call-expression",
  "lambda-expression", "compound-expression", "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   335,   335,   336,   340,   342,   344,   361,   363,   364,
     366,   371,   373,   375,   381,   388,   390,   395,   397,   402,
     404,   409,   410,   415,   417,   419,   424,   426,   427,   429,
     434,   439,   441,   443,   445,   447,   449,   451,   453,   455,
     460,   462,   467,   469,   471,   473,   475,   480,   482,   484,
     489,   497,   512,   514,   516,   522,   524,   529,   530,   534,
     536,   542,   547,   548,   552,   554,   559,   565,   567,   590,
     594,   603,   604,   608,   610,   615,   617,   622,   624,   630,
     631,   635,   637,   642,   643,   647,   649,   659,   661,   663,
     665,   672,   673,   677,   679,   684,   685,   689,   690,   694,
     696,   698,   700,   703,   705,   707,   709,   712,   714,   716,
     719,   721,   723,   725,   730,   732,   734,   739,   741,   743,
     763,   767,   768,   770,   772,   778,   784,   787,   789,   812,
     817,   819,   824,   826,   828,   830,   832,   834,   836,   838,
     840,   842,   844,   852,   856,   858,   860,   862,   864,   870,
     872,   875,   876,   877,   878,   882,   887,   889,   891,   893,
     898,   900,   902,   904,   909,   911,   916,   917,  1004,  1006,
    1009,  1013,  1015,  1018,  1020,  1028
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
      95,    96,    97,    98,    99,   100
    };
    // Last valid token kind.
    const int code_max = 355;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4309 "bang.tab.cpp"

#line 1405 "bang.y"

