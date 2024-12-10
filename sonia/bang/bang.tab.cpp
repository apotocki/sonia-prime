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
#line 24 "bang.y"

#include "sonia/config.hpp"
#include <iostream>
#include "parser.hpp"

using namespace sonia;
using namespace sonia::lang::bang;

#include "bang.tab.hpp"

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

#define IGNORE(...)


#line 70 "bang.tab.cpp"


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
#line 167 "bang.tab.cpp"

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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.copy< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
        value.copy< external_struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
      case symbol_kind::S_121_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_field_decl: // field-decl
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
        value.copy< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
        value.copy< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_131_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.copy< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
      case symbol_kind::S_117_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_132_concept_expression: // concept-expression
      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_137_syntax_expression: // syntax-expression
      case symbol_kind::S_139_compound_expression: // compound-expression
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< annotated_bool > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_138_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
        value.move< external_struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
      case symbol_kind::S_121_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_122_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
        value.move< infunction_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_131_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_118_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.move< statement_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
      case symbol_kind::S_117_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_132_concept_expression: // concept-expression
      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_137_syntax_expression: // syntax-expression
      case symbol_kind::S_139_compound_expression: // compound-expression
        value.move< syntax_expression_t > (YY_MOVE (s.value));
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.YY_MOVE_OR_COPY< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< external_struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
      case symbol_kind::S_121_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_field_decl: // field-decl
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_131_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.YY_MOVE_OR_COPY< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
      case symbol_kind::S_117_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_132_concept_expression: // concept-expression
      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_137_syntax_expression: // syntax-expression
      case symbol_kind::S_139_compound_expression: // compound-expression
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
        value.move< external_struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
      case symbol_kind::S_121_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
        value.move< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_131_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.move< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
      case symbol_kind::S_117_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_132_concept_expression: // concept-expression
      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_137_syntax_expression: // syntax-expression
      case symbol_kind::S_139_compound_expression: // compound-expression
        value.move< syntax_expression_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.copy< annotated_bool > (that.value);
        break;

      case symbol_kind::S_138_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
        value.copy< external_struct_decl > (that.value);
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
      case symbol_kind::S_121_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_122_field_decl: // field-decl
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
        value.copy< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
        value.copy< infunction_statement > (that.value);
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_131_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_118_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.copy< statement_set_t > (that.value);
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
      case symbol_kind::S_117_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_132_concept_expression: // concept-expression
      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_137_syntax_expression: // syntax-expression
      case symbol_kind::S_139_compound_expression: // compound-expression
        value.copy< syntax_expression_t > (that.value);
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< annotated_bool > (that.value);
        break;

      case symbol_kind::S_138_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
        value.move< external_struct_decl > (that.value);
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
      case symbol_kind::S_121_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_122_field_decl: // field-decl
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
        value.move< infunction_statement > (that.value);
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_131_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_118_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.move< statement_set_t > (that.value);
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
      case symbol_kind::S_117_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_132_concept_expression: // concept-expression
      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_137_syntax_expression: // syntax-expression
      case symbol_kind::S_139_compound_expression: // compound-expression
        value.move< syntax_expression_t > (that.value);
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
#line 318 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 318 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 318 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 318 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 318 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 318 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 318 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 318 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 318 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 318 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 318 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 318 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 318 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 318 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 318 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 318 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 318 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 318 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 318 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 318 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 318 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 318 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 318 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 318 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 318 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 318 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 318 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement_any: // finished_statement_any
#line 318 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 318 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_100_let_decl: // let-decl
#line 318 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_101_let_decl_start: // let-decl-start
#line 318 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 318 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
#line 318 "bang.y"
                 { }
#line 1597 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
#line 318 "bang.y"
                 { }
#line 1603 "bang.tab.cpp"
        break;

      case symbol_kind::S_braced_statements: // braced_statements
#line 318 "bang.y"
                 { }
#line 1609 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement: // finished_statement
#line 318 "bang.y"
                 { }
#line 1615 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
#line 318 "bang.y"
                 { }
#line 1621 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
#line 318 "bang.y"
                 { }
#line 1627 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
#line 318 "bang.y"
                 { }
#line 1633 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 318 "bang.y"
                 { }
#line 1639 "bang.tab.cpp"
        break;

      case symbol_kind::S_internal_identifier: // internal_identifier
#line 318 "bang.y"
                 { }
#line 1645 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 318 "bang.y"
                 { }
#line 1651 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
#line 318 "bang.y"
                 { }
#line 1657 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
#line 318 "bang.y"
                 { }
#line 1663 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
#line 318 "bang.y"
                 { }
#line 1669 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
#line 318 "bang.y"
                 { }
#line 1675 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_case_list: // case-list
#line 318 "bang.y"
                 { }
#line 1681 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_case_decl: // case-decl
#line 318 "bang.y"
                 { }
#line 1687 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
#line 318 "bang.y"
                 { }
#line 1693 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
#line 318 "bang.y"
                 { }
#line 1699 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_field_list: // field-list
#line 318 "bang.y"
                 { }
#line 1705 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_field_decl: // field-decl
#line 318 "bang.y"
                 { }
#line 1711 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
#line 318 "bang.y"
                 { }
#line 1717 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
#line 318 "bang.y"
                 { }
#line 1723 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 318 "bang.y"
                 { }
#line 1729 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
#line 318 "bang.y"
                 { }
#line 1735 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
#line 318 "bang.y"
                 { }
#line 1741 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
#line 318 "bang.y"
                 { }
#line 1747 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
#line 318 "bang.y"
                 { }
#line 1753 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
#line 318 "bang.y"
                 { }
#line 1759 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_parameter_list: // parameter-list
#line 318 "bang.y"
                 { }
#line 1765 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_concept_expression: // concept-expression
#line 318 "bang.y"
                 { }
#line 1771 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 318 "bang.y"
                 { }
#line 1777 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
#line 318 "bang.y"
                 { }
#line 1783 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
#line 318 "bang.y"
                 { }
#line 1789 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
#line 318 "bang.y"
                 { }
#line 1795 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_syntax_expression: // syntax-expression
#line 318 "bang.y"
                 { }
#line 1801 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_type_expr: // type-expr
#line 318 "bang.y"
                 { }
#line 1807 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_compound_expression: // compound-expression
#line 318 "bang.y"
                 { }
#line 1813 "bang.tab.cpp"
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        yylhs.value.emplace< annotated_bool > ();
        break;

      case symbol_kind::S_138_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_115_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_119_struct_decl: // struct-decl
        yylhs.value.emplace< external_struct_decl > ();
        break;

      case symbol_kind::S_120_field_list_opt: // field-list-opt
      case symbol_kind::S_121_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_122_field_decl: // field-decl
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_113_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_114_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_108_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_104_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_107_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< infunction_declaration_set_t > ();
        break;

      case symbol_kind::S_109_infunction_statement: // infunction-statement
        yylhs.value.emplace< infunction_statement > ();
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement_decl_t > ();
        break;

      case symbol_kind::S_134_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_135_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_136_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_124_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_125_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_128_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_123_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_129_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_130_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_131_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_118_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::bang::identifier > ();
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        yylhs.value.emplace< statement_set_t > ();
        break;

      case symbol_kind::S_116_case_list_opt: // case-list-opt
      case symbol_kind::S_117_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_126_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_127_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_132_concept_expression: // concept-expression
      case symbol_kind::S_133_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_137_syntax_expression: // syntax-expression
      case symbol_kind::S_139_compound_expression: // compound-expression
        yylhs.value.emplace< syntax_expression_t > ();
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
#line 323 "bang.y"
                            { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2230 "bang.tab.cpp"
    break;

  case 3: // begin: finished_statement_any "end of file"
#line 324 "bang.y"
                                 { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2236 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 329 "bang.y"
        { yylhs.value.as < statement_set_t > () = {}; }
#line 2242 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 331 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < statement > ()) }; }
#line 2248 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished_statement_any statement
#line 333 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2254 "bang.tab.cpp"
    break;

  case 7: // finished_statement_any: finished_statement
#line 350 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2260 "bang.tab.cpp"
    break;

  case 8: // finished_statement_any: finished_statement_any "`;`"
#line 351 "bang.y"
                                           { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2266 "bang.tab.cpp"
    break;

  case 9: // finished_statement_any: finished_statement_any finished_statement
#line 353 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2272 "bang.tab.cpp"
    break;

  case 10: // finished_statement_any: statement_any "`;`"
#line 355 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2278 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 360 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2284 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 362 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2290 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 364 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2296 "bang.tab.cpp"
    break;

  case 14: // statement: enum-decl
#line 366 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2302 "bang.tab.cpp"
    break;

  case 15: // statement: EXTERN STRUCT struct-decl
#line 368 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < external_struct_decl > ()); }
#line 2308 "bang.tab.cpp"
    break;

  case 16: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 373 "bang.y"
        { yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > (); yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), { return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } } }; }
#line 2314 "bang.tab.cpp"
    break;

  case 17: // statement: generic-statement
#line 375 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2320 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type
#line 388 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2326 "bang.tab.cpp"
    break;

  case 19: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 390 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2332 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: LET identifier
#line 395 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2338 "bang.tab.cpp"
    break;

  case 21: // let-decl-start: LET "weak modifier" identifier
#line 397 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2344 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start
#line 401 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 2350 "bang.tab.cpp"
    break;

  case 23: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 403 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 2356 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: %empty
#line 408 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = {}; }
#line 2362 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: infunction-statement
#line 410 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = infunction_declaration_set_t{std::move(yystack_[0].value.as < infunction_statement > ())}; }
#line 2368 "bang.tab.cpp"
    break;

  case 26: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 412 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); yylhs.value.as < infunction_declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < infunction_statement > ())); }
#line 2374 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished_statement
#line 416 "bang.y"
                         { yylhs.value.as < infunction_declaration_set_t > () = infunction_declaration_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2380 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 417 "bang.y"
                                                      { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); }
#line 2386 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: finished-infunction-statement-any finished_statement
#line 419 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); yylhs.value.as < infunction_declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2392 "bang.tab.cpp"
    break;

  case 30: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 420 "bang.y"
                                             { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); }
#line 2398 "bang.tab.cpp"
    break;

  case 31: // braced_statements: "`{`" infunction-statement-set "`}`"
#line 425 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2404 "bang.tab.cpp"
    break;

  case 32: // finished_statement: "`while`" syntax-expression braced_statements
#line 430 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2410 "bang.tab.cpp"
    break;

  case 33: // finished_statement: "`while`" syntax-expression "`;`" syntax-expression braced_statements
#line 432 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2416 "bang.tab.cpp"
    break;

  case 34: // finished_statement: "`if`" syntax-expression braced_statements
#line 434 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2422 "bang.tab.cpp"
    break;

  case 35: // finished_statement: "`if`" syntax-expression braced_statements "`else`" braced_statements
#line 436 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < infunction_declaration_set_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2428 "bang.tab.cpp"
    break;

  case 36: // finished_statement: fn-start-decl fn-decl braced_statements
#line 438 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) };  }
#line 2434 "bang.tab.cpp"
    break;

  case 37: // infunction-statement-set: infunction-statement-any
#line 442 "bang.y"
    { yylhs.value.as < infunction_declaration_set_t > () = yystack_[0].value.as < infunction_declaration_set_t > (); }
#line 2440 "bang.tab.cpp"
    break;

  case 38: // infunction-statement-set: finished-infunction-statement-any
#line 442 "bang.y"
                               { yylhs.value.as < infunction_declaration_set_t > () = yystack_[0].value.as < infunction_declaration_set_t > (); }
#line 2446 "bang.tab.cpp"
    break;

  case 39: // generic-statement: let-decl
#line 446 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2452 "bang.tab.cpp"
    break;

  case 40: // generic-statement: "`return`" syntax-expression
#line 448 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2458 "bang.tab.cpp"
    break;

  case 41: // generic-statement: compound-expression
#line 450 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2464 "bang.tab.cpp"
    break;

  case 42: // infunction-statement: generic-statement
#line 455 "bang.y"
        { yylhs.value.as < infunction_statement > () = apply_visitor(statement_adopt_visitor<infunction_statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2470 "bang.tab.cpp"
    break;

  case 43: // infunction-statement: "`break`"
#line 457 "bang.y"
        { yylhs.value.as < infunction_statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2476 "bang.tab.cpp"
    break;

  case 44: // infunction-statement: "`continue`"
#line 459 "bang.y"
        { yylhs.value.as < infunction_statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2482 "bang.tab.cpp"
    break;

  case 45: // identifier: IDENTIFIER
#line 464 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2488 "bang.tab.cpp"
    break;

  case 46: // internal_identifier: INTERNAL_IDENTIFIER
#line 472 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2494 "bang.tab.cpp"
    break;

  case 47: // qname: "`::`" identifier
#line 487 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2500 "bang.tab.cpp"
    break;

  case 48: // qname: identifier
#line 489 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2506 "bang.tab.cpp"
    break;

  case 49: // qname: qname "`::`" identifier
#line 491 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2512 "bang.tab.cpp"
    break;

  case 50: // fn-start-decl: "`fn`"
#line 497 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2518 "bang.tab.cpp"
    break;

  case 51: // fn-start-decl: INLINE "`fn`"
#line 499 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2524 "bang.tab.cpp"
    break;

  case 52: // fn-decl: qname "`(`" parameter-list-opt "`)`"
#line 504 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2530 "bang.tab.cpp"
    break;

  case 53: // fn-decl: qname "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 506 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2536 "bang.tab.cpp"
    break;

  case 54: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 512 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2544 "bang.tab.cpp"
    break;

  case 55: // case-list-opt: %empty
#line 518 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2550 "bang.tab.cpp"
    break;

  case 56: // case-list-opt: case-list
#line 520 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2556 "bang.tab.cpp"
    break;

  case 57: // case-list: case-decl
#line 525 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2562 "bang.tab.cpp"
    break;

  case 58: // case-list: case-list "," case-decl
#line 528 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2568 "bang.tab.cpp"
    break;

  case 59: // case-decl: identifier
#line 533 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2574 "bang.tab.cpp"
    break;

  case 60: // struct-decl: qname "`->`" "`(`" field-list-opt "`)`"
#line 539 "bang.y"
    {
        yylhs.value.as < external_struct_decl > () = external_struct_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < field_list_t > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2582 "bang.tab.cpp"
    break;

  case 61: // field-list-opt: %empty
#line 569 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2588 "bang.tab.cpp"
    break;

  case 62: // field-list-opt: field-list
#line 570 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2594 "bang.tab.cpp"
    break;

  case 63: // field-list: field-decl
#line 575 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2600 "bang.tab.cpp"
    break;

  case 64: // field-list: field-list "," field-decl
#line 577 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2606 "bang.tab.cpp"
    break;

  case 65: // field-decl: IDENTIFIER "`:`" type-expr
#line 582 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2612 "bang.tab.cpp"
    break;

  case 66: // field-decl: type-expr
#line 584 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2618 "bang.tab.cpp"
    break;

  case 67: // parameter-name-decl: identifier internal_identifier "`:`"
#line 589 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }; }
#line 2624 "bang.tab.cpp"
    break;

  case 68: // parameter-name-decl: identifier "`:`"
#line 591 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), nullopt }; }
#line 2630 "bang.tab.cpp"
    break;

  case 69: // parameter-name-decl: internal_identifier "`:`"
#line 593 "bang.y"
        { yylhs.value.as < parameter_name > () = unnamed_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }; }
#line 2636 "bang.tab.cpp"
    break;

  case 70: // parameter-name-decl: internal_identifier "`...`" "`:`"
#line 595 "bang.y"
        { yylhs.value.as < parameter_name > () = varnamed_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ())}; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2642 "bang.tab.cpp"
    break;

  case 71: // parameter-constraint-modifier: TYPENAME
#line 600 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2648 "bang.tab.cpp"
    break;

  case 72: // parameter-constraint-modifier: "const modifier"
#line 602 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2654 "bang.tab.cpp"
    break;

  case 73: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 606 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2660 "bang.tab.cpp"
    break;

  case 74: // parameter-constraint-modifier-opt: %empty
#line 608 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2666 "bang.tab.cpp"
    break;

  case 75: // basic-parameter-matched-type: qname
#line 613 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2672 "bang.tab.cpp"
    break;

  case 76: // basic-parameter-matched-type: qname "`(`" opt-named-expr-list-any "`)`"
#line 615 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2678 "bang.tab.cpp"
    break;

  case 77: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 617 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2684 "bang.tab.cpp"
    break;

  case 78: // parameter-matched-type: basic-parameter-matched-type
#line 621 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2690 "bang.tab.cpp"
    break;

  case 79: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 623 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2696 "bang.tab.cpp"
    break;

  case 80: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 625 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2702 "bang.tab.cpp"
    break;

  case 81: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 627 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2708 "bang.tab.cpp"
    break;

  case 82: // parameter-constraint-set: parameter-matched-type
#line 632 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2714 "bang.tab.cpp"
    break;

  case 83: // parameter-constraint-set: concept-expression
#line 634 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2720 "bang.tab.cpp"
    break;

  case 84: // parameter-constraint-set: "`_`"
#line 636 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{}; }
#line 2726 "bang.tab.cpp"
    break;

  case 85: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 638 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2732 "bang.tab.cpp"
    break;

  case 86: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 643 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2738 "bang.tab.cpp"
    break;

  case 87: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 645 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2744 "bang.tab.cpp"
    break;

  case 88: // parameter-decl: parameter-constraint-set
#line 647 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2750 "bang.tab.cpp"
    break;

  case 89: // parameter-list-opt: %empty
#line 651 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2756 "bang.tab.cpp"
    break;

  case 90: // parameter-list-opt: parameter-list
#line 652 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2762 "bang.tab.cpp"
    break;

  case 91: // parameter-list: parameter-decl
#line 657 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2768 "bang.tab.cpp"
    break;

  case 92: // parameter-list: parameter-list "," parameter-decl
#line 659 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2774 "bang.tab.cpp"
    break;

  case 93: // concept-expression: "`@`" qname
#line 679 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 2780 "bang.tab.cpp"
    break;

  case 94: // syntax-expression-wo-ii: "true"
#line 685 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2786 "bang.tab.cpp"
    break;

  case 95: // syntax-expression-wo-ii: "false"
#line 687 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2792 "bang.tab.cpp"
    break;

  case 96: // syntax-expression-wo-ii: INTEGER
#line 689 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 2798 "bang.tab.cpp"
    break;

  case 97: // syntax-expression-wo-ii: DECIMAL
#line 691 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2804 "bang.tab.cpp"
    break;

  case 98: // syntax-expression-wo-ii: STRING
#line 693 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2810 "bang.tab.cpp"
    break;

  case 99: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 695 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2816 "bang.tab.cpp"
    break;

  case 100: // syntax-expression-wo-ii: qname
#line 697 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2822 "bang.tab.cpp"
    break;

  case 101: // syntax-expression-wo-ii: "`(`" opt-named-expr-list "`)`"
#line 699 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) };
            }
        }
#line 2834 "bang.tab.cpp"
    break;

  case 102: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 707 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2840 "bang.tab.cpp"
    break;

  case 103: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 709 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2846 "bang.tab.cpp"
    break;

  case 104: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 711 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2852 "bang.tab.cpp"
    break;

  case 105: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 713 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2858 "bang.tab.cpp"
    break;

  case 106: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 716 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2864 "bang.tab.cpp"
    break;

  case 107: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 718 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2870 "bang.tab.cpp"
    break;

  case 108: // syntax-expression-wo-ii: compound-expression
#line 721 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2876 "bang.tab.cpp"
    break;

  case 109: // opt-named-expr-list-any: %empty
#line 726 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{}; }
#line 2882 "bang.tab.cpp"
    break;

  case 110: // opt-named-expr-list-any: opt-named-expr-list
#line 727 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2888 "bang.tab.cpp"
    break;

  case 111: // opt-named-expr-list: opt-named-expr
#line 732 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2894 "bang.tab.cpp"
    break;

  case 112: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 734 "bang.y"
        {
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
            yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ()));
		}
#line 2903 "bang.tab.cpp"
    break;

  case 113: // opt-named-expr: identifier "`:`" syntax-expression
#line 742 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2909 "bang.tab.cpp"
    break;

  case 114: // opt-named-expr: syntax-expression
#line 744 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2915 "bang.tab.cpp"
    break;

  case 115: // syntax-expression: INTERNAL_IDENTIFIER
#line 749 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2921 "bang.tab.cpp"
    break;

  case 116: // syntax-expression: syntax-expression-wo-ii
#line 750 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2927 "bang.tab.cpp"
    break;

  case 117: // type-expr: qname
#line 800 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()); }
#line 2933 "bang.tab.cpp"
    break;

  case 118: // type-expr: internal_identifier
#line 804 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2939 "bang.tab.cpp"
    break;

  case 119: // type-expr: "`[`" type-expr "`]`"
#line 808 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2945 "bang.tab.cpp"
    break;

  case 120: // type-expr: "`(`" field-list-opt "`)`"
#line 810 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2951 "bang.tab.cpp"
    break;

  case 121: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 812 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2957 "bang.tab.cpp"
    break;

  case 122: // type-expr: type-expr "`|`" type-expr
#line 814 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
            IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 2969 "bang.tab.cpp"
    break;

  case 123: // type-expr: type-expr "`->`" type-expr
#line 822 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2975 "bang.tab.cpp"
    break;

  case 124: // compound-expression: syntax-expression "`(`" opt-named-expr-list-any "`)`"
#line 839 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2981 "bang.tab.cpp"
    break;

  case 125: // compound-expression: syntax-expression "`=`" syntax-expression
#line 848 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2987 "bang.tab.cpp"
    break;


#line 2991 "bang.tab.cpp"

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


  const signed char parser::yypact_ninf_ = -113;

  const signed char parser::yytable_ninf_ = -47;

  const short
  parser::yypact_[] =
  {
     195,  -113,  -113,  -113,  -113,  -113,  -113,    79,   135,   135,
       6,    49,   135,   135,   135,    30,    13,  -113,     8,  -113,
    -113,   105,    29,   123,  -113,  -113,    68,    96,  -113,  -113,
    -113,    81,     8,  -113,  -113,   231,    31,  -113,    80,    42,
    -113,   231,  -113,   -16,    79,  -113,    79,     8,     8,   299,
     302,   231,  -113,  -113,    -3,  -113,  -113,  -113,  -113,  -113,
    -113,  -113,   114,   135,    79,    -2,    45,   135,   135,   135,
    -113,   135,    79,   135,   135,  -113,   135,  -113,    84,  -113,
      66,  -113,   297,   135,  -113,    77,    79,  -113,   175,   114,
    -113,    81,   100,   231,  -113,     9,   135,  -113,   171,   102,
     102,    94,    88,  -113,   -16,   231,  -113,   114,   109,  -113,
    -113,   107,   225,  -113,   118,  -113,  -113,     8,   302,   108,
    -113,   125,    97,  -113,   128,   129,   112,  -113,   100,    87,
     114,   164,   114,    -4,     8,  -113,  -113,  -113,    11,     3,
      69,   -42,   207,    65,  -113,   167,  -113,   140,   124,  -113,
     231,  -113,   100,   175,  -113,  -113,  -113,  -113,  -113,   108,
    -113,  -113,  -113,    79,   114,  -113,   175,  -113,  -113,   138,
      10,  -113,    81,  -113,   148,   150,  -113,   135,  -113,   207,
     179,   167,  -113,     8,  -113,   196,     9,   166,  -113,   100,
    -113,  -113,  -113,  -113,   177,   167,  -113,   135,  -113,  -113,
    -113,   231
  };

  const signed char
  parser::yydefact_[] =
  {
       4,    98,    45,   115,    99,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,    94,
      95,     0,     0,     0,     5,    39,    22,    18,     7,    17,
      48,   100,     0,    14,   116,     0,   108,    47,    48,     0,
     111,   114,   108,   103,     0,    20,     0,     0,     0,     0,
       0,    40,    13,    51,     0,     1,     2,    10,     3,     8,
       6,     9,     0,     0,     0,     0,     0,     0,     0,     0,
     104,   109,     0,     0,     0,   101,     0,    21,     0,    12,
       0,    15,    24,     0,    32,    34,    55,    46,    61,     0,
     118,   117,    23,    19,    49,    89,     0,    36,   125,   106,
     107,     0,   110,   102,   105,   113,   112,     0,     0,    44,
      43,    37,    38,    27,     0,    42,    25,     0,     0,     0,
      59,     0,    56,    57,    45,     0,    62,    63,    66,     0,
       0,     0,     0,    80,     0,    84,    71,    72,    48,     0,
      75,    74,     0,    78,    82,    88,    91,     0,    90,    83,
      16,   124,    11,    61,    30,    28,    29,    26,    31,     0,
      33,    35,    54,     0,     0,   120,     0,   119,   123,     0,
     122,    81,    93,    68,     0,     0,    69,   109,    73,     0,
      80,    87,    79,     0,    85,    52,     0,     0,    58,    65,
      64,   121,    67,    70,     0,    86,    77,     0,    92,    60,
      76,    53
  };

  const short
  parser::yypgoto_[] =
  {
    -113,  -113,  -113,  -113,   203,  -113,  -113,  -113,  -113,  -113,
     -29,   -12,  -113,   -63,   115,     0,   -82,   -15,   -43,   -23,
    -113,  -113,  -113,    73,  -113,    89,  -113,    71,  -113,    99,
    -113,    58,  -113,  -112,    59,  -113,  -113,   -88,  -113,    67,
     238,   172,    -8,   -53,     2
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,    21,    22,    23,    24,    25,    26,    27,   111,   112,
      97,    28,   114,    29,   116,    30,    90,    31,    32,    66,
      33,   121,   122,   123,    81,   125,   126,   127,   141,   142,
     179,   143,   144,   145,   146,   147,   148,   149,    34,   101,
     102,    40,    35,   128,    42
  };

  const short
  parser::yytable_[] =
  {
      41,    43,    36,    54,    49,    50,    51,    37,    38,    92,
      45,    61,     2,   139,     2,     2,   133,    65,    87,   115,
      84,    85,   134,   135,    79,    36,   130,   -46,    71,    56,
     181,   -41,    65,    80,   175,    52,   129,    72,   -46,   117,
      64,    64,    95,    86,    77,   176,    78,    91,   136,   115,
     137,     7,     7,   173,   152,    93,   174,   184,   131,    98,
      99,   100,    96,    41,    94,   104,   105,   195,    41,   117,
     113,    38,   103,    91,    91,   118,    38,   168,   -41,   170,
     140,    57,   108,   -41,    36,     2,   120,    75,   150,   160,
     161,    82,    91,   184,   159,   138,   182,    44,    53,   136,
     156,   137,    65,   130,   139,    55,    76,   184,    63,    64,
      62,   189,    64,   177,    36,    91,   130,    91,    46,   172,
       2,    87,    74,    58,    64,   183,   107,   140,     1,     2,
       3,     4,     5,     6,    47,   131,   167,    48,    91,   151,
       1,     2,     3,     4,     5,     6,    71,   132,   131,    91,
     119,    91,    76,   153,    82,    72,    73,     7,    88,   154,
     132,   163,    89,   120,   140,   158,     7,     8,   140,    41,
     164,   140,   162,   169,   165,    59,   166,    38,     7,     8,
     134,   124,    87,    67,     9,   185,   138,   191,   186,   201,
     192,    10,   193,    11,    12,    13,     9,    68,    69,    14,
       1,     2,     3,     4,     5,     6,    15,    16,    17,    18,
     171,   199,   197,     2,   180,    71,    19,    20,     7,    88,
     134,   135,   200,    89,    72,    73,    60,   157,    19,    20,
       1,     2,     3,     4,     5,     6,   188,   190,     7,     8,
     178,   196,   187,    67,   194,   198,    39,     0,   106,     0,
       7,     0,     0,     0,     0,     0,     9,    68,    69,     0,
       0,     0,    70,    10,     0,    11,    12,    13,     7,     8,
       0,    14,     0,     0,     0,    71,     0,   155,    15,    16,
      17,    18,     0,     0,    72,    73,     9,     0,    19,    20,
       0,     0,     0,    10,     0,     0,    12,    13,     0,   109,
     110,    14,     1,     2,     3,     4,     5,     6,     0,    16,
      17,    67,     0,     0,    67,     0,     0,     0,    19,    20,
       0,     0,     0,     0,     0,    68,    69,     0,    68,    69,
      70,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       7,     8,     0,    71,     0,    82,    71,     0,    82,     0,
       0,    83,    72,    73,     0,    72,    73,     0,     9,     0,
       0,     0,     0,     0,     0,    10,     0,     0,    12,    13,
       0,   109,   110,    14,     0,     0,     0,     0,     0,     0,
       0,    16,    17,     0,     0,     0,     0,     0,     0,     0,
      19,    20
  };

  const short
  parser::yycheck_[] =
  {
       8,     9,     0,    18,    12,    13,    14,     7,     8,    62,
      10,    23,     6,    95,     6,     6,     7,    32,     7,    82,
      49,    50,    13,    14,    47,    23,    16,    31,    44,     0,
     142,     0,    47,    48,    31,     5,    89,    53,    42,    82,
      43,    43,    44,    46,    44,    42,    46,    62,    90,   112,
      92,    43,    43,    42,   107,    63,   138,   145,    48,    67,
      68,    69,    17,    71,    64,    73,    74,   179,    76,   112,
      82,    71,    72,    88,    89,    83,    76,   130,    47,   132,
      95,    52,    16,    52,    82,     6,    86,    45,    96,   118,
     119,    46,   107,   181,   117,    95,    31,    91,    85,    90,
     112,    92,   117,    16,   186,     0,    64,   195,    12,    43,
      42,   164,    43,    44,   112,   130,    16,   132,    69,   134,
       6,     7,    42,     0,    43,    60,    42,   142,     5,     6,
       7,     8,     9,    10,    85,    48,    49,    88,   153,    45,
       5,     6,     7,     8,     9,    10,    44,    60,    48,   164,
      73,   166,    64,    44,    46,    53,    54,    43,    44,    52,
      60,    64,    48,   163,   179,    47,    43,    44,   183,   177,
      42,   186,    47,     9,    45,    52,    64,   177,    43,    44,
      13,     6,     7,    12,    61,    45,   186,    49,    64,   197,
      42,    68,    42,    70,    71,    72,    61,    26,    27,    76,
       5,     6,     7,     8,     9,    10,    83,    84,    85,    86,
      31,    45,    16,     6,     7,    44,    93,    94,    43,    44,
      13,    14,    45,    48,    53,    54,    23,   112,    93,    94,
       5,     6,     7,     8,     9,    10,   163,   166,    43,    44,
     141,   183,   153,    12,   177,   186,     8,    -1,    76,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    61,    26,    27,    -1,
      -1,    -1,    31,    68,    -1,    70,    71,    72,    43,    44,
      -1,    76,    -1,    -1,    -1,    44,    -1,    52,    83,    84,
      85,    86,    -1,    -1,    53,    54,    61,    -1,    93,    94,
      -1,    -1,    -1,    68,    -1,    -1,    71,    72,    -1,    74,
      75,    76,     5,     6,     7,     8,     9,    10,    -1,    84,
      85,    12,    -1,    -1,    12,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    26,    27,
      31,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    44,    -1,    44,    -1,    46,    44,    -1,    46,    -1,
      -1,    52,    53,    54,    -1,    53,    54,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    43,    44,    61,
      68,    70,    71,    72,    76,    83,    84,    85,    86,    93,
      94,    96,    97,    98,    99,   100,   101,   102,   106,   108,
     110,   112,   113,   115,   133,   137,   139,   110,   110,   135,
     136,   137,   139,   137,    91,   110,    69,    85,    88,   137,
     137,   137,     5,    85,   112,     0,     0,    52,     0,    52,
      99,   106,    42,    12,    43,   112,   114,    12,    26,    27,
      31,    44,    53,    54,    42,    45,    64,   110,   110,   114,
     112,   119,    46,    52,   105,   105,    46,     7,    44,    48,
     111,   112,   138,   137,   110,    44,    17,   105,   137,   137,
     137,   134,   135,   110,   137,   137,   136,    42,    16,    74,
      75,   103,   104,   106,   107,   108,   109,   113,   137,    73,
     110,   116,   117,   118,     6,   120,   121,   122,   138,   138,
      16,    48,    60,     7,    13,    14,    90,    92,   110,   111,
     112,   123,   124,   126,   127,   128,   129,   130,   131,   132,
     137,    45,   138,    44,    52,    52,   106,   109,    47,   114,
     105,   105,    47,    64,    42,    45,    64,    49,   138,     9,
     138,    31,   112,    42,   111,    31,    42,    44,   124,   125,
       7,   128,    31,    60,   132,    45,    64,   120,   118,   138,
     122,    49,    42,    42,   134,   128,   126,    16,   129,    45,
      45,   137
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    95,    96,    96,    97,    97,    97,    98,    98,    98,
      98,    99,    99,    99,    99,    99,    99,    99,   100,   100,
     101,   101,   102,   102,   103,   103,   103,   104,   104,   104,
     104,   105,   106,   106,   106,   106,   106,   107,   107,   108,
     108,   108,   109,   109,   109,   110,   111,   112,   112,   112,
     113,   113,   114,   114,   115,   116,   116,   117,   117,   118,
     119,   120,   120,   121,   121,   122,   122,   123,   123,   123,
     123,   124,   124,   125,   125,   126,   126,   126,   127,   127,
     127,   127,   128,   128,   128,   128,   129,   129,   129,   130,
     130,   131,   131,   132,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   134,
     134,   135,   135,   136,   136,   137,   137,   138,   138,   138,
     138,   138,   138,   138,   139,   139
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     3,     2,     1,     3,     4,     1,     1,     3,
       2,     3,     1,     3,     0,     1,     2,     1,     2,     2,
       2,     3,     3,     5,     3,     5,     3,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     2,     1,     3,
       1,     2,     4,     6,     5,     0,     1,     1,     3,     1,
       5,     0,     1,     1,     3,     3,     1,     3,     2,     2,
       3,     1,     1,     1,     0,     1,     4,     3,     1,     2,
       1,     2,     1,     1,     1,     2,     3,     2,     1,     0,
       1,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     2,     3,     3,     3,     1,     0,
       1,     1,     3,     3,     1,     1,     1,     1,     1,     3,
       3,     4,     3,     3,     4,     3
  };


#if BANG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "INTERNAL_IDENTIFIER",
  "RESERVED_IDENTIFIER", "INTEGER", "DECIMAL", "OPERATOR_TERM", "\"`=`\"",
  "\"`@`\"", "\"`_`\"", "\"`->*`\"", "\"`->`\"", "\"`=>`\"", "\"`~>`\"",
  "\"`.*`\"", "\"`++`\"", "\"`--`\"", "\"`<<`\"", "\"`>>`\"", "\"`<=`\"",
  "\"`>=`\"", "\"`==`\"", "\"`!=`\"", "\"`&&`\"", "\"`||`\"", "\"`..`\"",
  "\"`...`\"", "\"`+=`\"", "\"`-=`\"", "\"`*=`\"", "\"`/=`\"", "\"`%=`\"",
  "\"`<<=`\"", "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"", "\"`^=`\"", "\"`:`\"",
  "\"`::`\"", "\"`(`\"", "\"`)`\"", "\"`{`\"", "\"`}`\"", "\"`[`\"",
  "\"`]`\"", "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"", "\"`+`\"",
  "\"`-`\"", "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"", "\"`|`\"",
  "\"`!`\"", "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"", "\"`?`\"",
  "\"`#`\"", "LET", "VAR", "EXTERN", "\"`while`\"", "\"`if`\"",
  "\"`else`\"", "\"`continue`\"", "\"`break`\"", "\"`return`\"", "FOR",
  "AUTO", "USING", "THROW", "SIZEOF", "LOWEST", "INCLUDE", "INLINE",
  "\"`fn`\"", "ENUM", "TYPE", "STRUCT", "EXTENDS", "TYPENAME",
  "\"weak modifier\"", "\"const modifier\"", "\"true\"", "\"false\"",
  "$accept", "begin", "statement_any", "finished_statement_any",
  "statement", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction-statement-any",
  "finished-infunction-statement-any", "braced_statements",
  "finished_statement", "infunction-statement-set", "generic-statement",
  "infunction-statement", "identifier", "internal_identifier", "qname",
  "fn-start-decl", "fn-decl", "enum-decl", "case-list-opt", "case-list",
  "case-decl", "struct-decl", "field-list-opt", "field-list", "field-decl",
  "parameter-name-decl", "parameter-constraint-modifier",
  "parameter-constraint-modifier-opt", "basic-parameter-matched-type",
  "parameter-matched-type", "parameter-constraint-set", "parameter-decl",
  "parameter-list-opt", "parameter-list", "concept-expression",
  "syntax-expression-wo-ii", "opt-named-expr-list-any",
  "opt-named-expr-list", "opt-named-expr", "syntax-expression",
  "type-expr", "compound-expression", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   323,   323,   324,   328,   330,   332,   349,   351,   352,
     354,   359,   361,   363,   365,   367,   372,   374,   387,   389,
     394,   396,   401,   402,   407,   409,   411,   416,   417,   418,
     420,   424,   429,   431,   433,   435,   437,   442,   442,   445,
     447,   449,   454,   456,   458,   463,   471,   486,   488,   490,
     496,   498,   503,   505,   511,   518,   520,   524,   527,   532,
     538,   569,   570,   574,   576,   581,   583,   588,   590,   592,
     594,   599,   601,   606,   607,   612,   614,   616,   621,   622,
     624,   626,   631,   633,   635,   637,   642,   644,   646,   651,
     652,   656,   658,   678,   684,   686,   688,   690,   692,   694,
     696,   698,   706,   708,   710,   712,   715,   717,   720,   725,
     727,   731,   733,   741,   743,   748,   750,   799,   803,   807,
     809,   811,   813,   821,   838,   847
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
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94
    };
    // Last valid token kind.
    const int code_max = 349;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3717 "bang.tab.cpp"

#line 1225 "bang.y"

