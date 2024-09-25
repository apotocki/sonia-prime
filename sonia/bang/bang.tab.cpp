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

      case symbol_kind::S_130_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
      case symbol_kind::S_118_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_field_decl: // field-decl
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
        value.copy< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
        value.copy< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_let_decl: // let-decl
      case symbol_kind::S_100_let_decl_start: // let-decl-start
      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_129_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
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

      case symbol_kind::S_116_case_decl: // case-decl
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

      case symbol_kind::S_114_case_list_opt: // case-list-opt
      case symbol_kind::S_115_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_126_syntax_expression: // syntax-expression
      case symbol_kind::S_131_compound_expression: // compound-expression
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

      case symbol_kind::S_130_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
      case symbol_kind::S_118_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
        value.move< infunction_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_99_let_decl: // let-decl
      case symbol_kind::S_100_let_decl_start: // let-decl-start
      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_129_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
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

      case symbol_kind::S_116_case_decl: // case-decl
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

      case symbol_kind::S_114_case_list_opt: // case-list-opt
      case symbol_kind::S_115_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_126_syntax_expression: // syntax-expression
      case symbol_kind::S_131_compound_expression: // compound-expression
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

      case symbol_kind::S_130_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
      case symbol_kind::S_118_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_field_decl: // field-decl
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_let_decl: // let-decl
      case symbol_kind::S_100_let_decl_start: // let-decl-start
      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_129_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
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

      case symbol_kind::S_116_case_decl: // case-decl
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

      case symbol_kind::S_114_case_list_opt: // case-list-opt
      case symbol_kind::S_115_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_126_syntax_expression: // syntax-expression
      case symbol_kind::S_131_compound_expression: // compound-expression
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

      case symbol_kind::S_130_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
      case symbol_kind::S_118_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
        value.move< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_let_decl: // let-decl
      case symbol_kind::S_100_let_decl_start: // let-decl-start
      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_129_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
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

      case symbol_kind::S_116_case_decl: // case-decl
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

      case symbol_kind::S_114_case_list_opt: // case-list-opt
      case symbol_kind::S_115_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_126_syntax_expression: // syntax-expression
      case symbol_kind::S_131_compound_expression: // compound-expression
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

      case symbol_kind::S_130_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
      case symbol_kind::S_118_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_119_field_decl: // field-decl
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
        value.copy< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
        value.copy< infunction_statement > (that.value);
        break;

      case symbol_kind::S_99_let_decl: // let-decl
      case symbol_kind::S_100_let_decl_start: // let-decl-start
      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_129_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
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

      case symbol_kind::S_116_case_decl: // case-decl
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

      case symbol_kind::S_114_case_list_opt: // case-list-opt
      case symbol_kind::S_115_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_126_syntax_expression: // syntax-expression
      case symbol_kind::S_131_compound_expression: // compound-expression
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

      case symbol_kind::S_130_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
      case symbol_kind::S_118_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_119_field_decl: // field-decl
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
        value.move< infunction_statement > (that.value);
        break;

      case symbol_kind::S_99_let_decl: // let-decl
      case symbol_kind::S_100_let_decl_start: // let-decl-start
      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_129_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
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

      case symbol_kind::S_116_case_decl: // case-decl
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

      case symbol_kind::S_114_case_list_opt: // case-list-opt
      case symbol_kind::S_115_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_126_syntax_expression: // syntax-expression
      case symbol_kind::S_131_compound_expression: // compound-expression
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
#line 313 "bang.y"
                 { }
#line 1357 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 313 "bang.y"
                 { }
#line 1363 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 313 "bang.y"
                 { }
#line 1369 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 313 "bang.y"
                 { }
#line 1375 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 313 "bang.y"
                 { }
#line 1381 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 313 "bang.y"
                 { }
#line 1387 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 313 "bang.y"
                 { }
#line 1393 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 313 "bang.y"
                 { }
#line 1399 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 313 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 313 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 313 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 313 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 313 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 313 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 313 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 313 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 313 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 313 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 313 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 313 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 313 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 313 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 313 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 313 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 313 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 313 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement_any: // finished_statement_any
#line 313 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 313 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_99_let_decl: // let-decl
#line 313 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_100_let_decl_start: // let-decl-start
#line 313 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 313 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
#line 313 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
#line 313 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_braced_statements: // braced_statements
#line 313 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement: // finished_statement
#line 313 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
#line 313 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
#line 313 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
#line 313 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 313 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 313 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
#line 313 "bang.y"
                 { }
#line 1597 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
#line 313 "bang.y"
                 { }
#line 1603 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
#line 313 "bang.y"
                 { }
#line 1609 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_case_list_opt: // case-list-opt
#line 313 "bang.y"
                 { }
#line 1615 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_case_list: // case-list
#line 313 "bang.y"
                 { }
#line 1621 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_case_decl: // case-decl
#line 313 "bang.y"
                 { }
#line 1627 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
#line 313 "bang.y"
                 { }
#line 1633 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_field_list: // field-list
#line 313 "bang.y"
                 { }
#line 1639 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_field_decl: // field-decl
#line 313 "bang.y"
                 { }
#line 1645 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
#line 313 "bang.y"
                 { }
#line 1651 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
#line 313 "bang.y"
                 { }
#line 1657 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 313 "bang.y"
                 { }
#line 1663 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
#line 313 "bang.y"
                 { }
#line 1669 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
#line 313 "bang.y"
                 { }
#line 1675 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 313 "bang.y"
                 { }
#line 1681 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_syntax_expression: // syntax-expression
#line 313 "bang.y"
                 { }
#line 1687 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
#line 313 "bang.y"
                 { }
#line 1693 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
#line 313 "bang.y"
                 { }
#line 1699 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_parameter_list: // parameter-list
#line 313 "bang.y"
                 { }
#line 1705 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_type_expr: // type-expr
#line 313 "bang.y"
                 { }
#line 1711 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_compound_expression: // compound-expression
#line 313 "bang.y"
                 { }
#line 1717 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
#line 313 "bang.y"
                 { }
#line 1723 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
#line 313 "bang.y"
                 { }
#line 1729 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
#line 313 "bang.y"
                 { }
#line 1735 "bang.tab.cpp"
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

      case symbol_kind::S_130_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_113_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_117_field_list_opt: // field-list-opt
      case symbol_kind::S_118_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_119_field_decl: // field-decl
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_111_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_112_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_107_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_102_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_103_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_106_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< infunction_declaration_set_t > ();
        break;

      case symbol_kind::S_108_infunction_statement: // infunction-statement
        yylhs.value.emplace< infunction_statement > ();
        break;

      case symbol_kind::S_99_let_decl: // let-decl
      case symbol_kind::S_100_let_decl_start: // let-decl-start
      case symbol_kind::S_101_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement_decl_t > ();
        break;

      case symbol_kind::S_132_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_133_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_134_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_121_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_122_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_123_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_120_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_127_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_128_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_129_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
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

      case symbol_kind::S_116_case_decl: // case-decl
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

      case symbol_kind::S_114_case_list_opt: // case-list-opt
      case symbol_kind::S_115_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_124_concept_expression: // concept-expression
      case symbol_kind::S_125_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_126_syntax_expression: // syntax-expression
      case symbol_kind::S_131_compound_expression: // compound-expression
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
#line 318 "bang.y"
                            { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2144 "bang.tab.cpp"
    break;

  case 3: // begin: finished_statement_any "end of file"
#line 319 "bang.y"
                                 { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2150 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 324 "bang.y"
        { yylhs.value.as < statement_set_t > () = {}; }
#line 2156 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 326 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < statement > ()) }; }
#line 2162 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished_statement_any statement
#line 328 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2168 "bang.tab.cpp"
    break;

  case 7: // finished_statement_any: finished_statement
#line 345 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2174 "bang.tab.cpp"
    break;

  case 8: // finished_statement_any: finished_statement_any "`;`"
#line 346 "bang.y"
                                           { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2180 "bang.tab.cpp"
    break;

  case 9: // finished_statement_any: finished_statement_any finished_statement
#line 348 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2186 "bang.tab.cpp"
    break;

  case 10: // finished_statement_any: statement_any "`;`"
#line 350 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2192 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 355 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2198 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 357 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2204 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 359 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2210 "bang.tab.cpp"
    break;

  case 14: // statement: enum-decl
#line 361 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2216 "bang.tab.cpp"
    break;

  case 15: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 366 "bang.y"
        { yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > (); yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), { return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } } }; }
#line 2222 "bang.tab.cpp"
    break;

  case 16: // statement: generic-statement
#line 368 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2228 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 381 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2234 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 383 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2240 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: LET identifier
#line 388 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2246 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: LET "weak modifier" identifier
#line 390 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2252 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 394 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 2258 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 396 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 2264 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 401 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = {}; }
#line 2270 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 403 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = infunction_declaration_set_t{std::move(yystack_[0].value.as < infunction_statement > ())}; }
#line 2276 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 405 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); yylhs.value.as < infunction_declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < infunction_statement > ())); }
#line 2282 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished_statement
#line 409 "bang.y"
                         { yylhs.value.as < infunction_declaration_set_t > () = infunction_declaration_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2288 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 410 "bang.y"
                                                      { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); }
#line 2294 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished_statement
#line 412 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); yylhs.value.as < infunction_declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2300 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 413 "bang.y"
                                             { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); }
#line 2306 "bang.tab.cpp"
    break;

  case 30: // braced_statements: "`{`" infunction-statement-set "`}`"
#line 418 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2312 "bang.tab.cpp"
    break;

  case 31: // finished_statement: "`while`" syntax-expression braced_statements
#line 423 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2318 "bang.tab.cpp"
    break;

  case 32: // finished_statement: "`while`" syntax-expression "`;`" syntax-expression braced_statements
#line 425 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2324 "bang.tab.cpp"
    break;

  case 33: // finished_statement: "`if`" syntax-expression braced_statements
#line 427 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2330 "bang.tab.cpp"
    break;

  case 34: // finished_statement: "`if`" syntax-expression braced_statements "`else`" braced_statements
#line 429 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < infunction_declaration_set_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2336 "bang.tab.cpp"
    break;

  case 35: // finished_statement: fn-start-decl fn-decl braced_statements
#line 431 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) };  }
#line 2342 "bang.tab.cpp"
    break;

  case 36: // infunction-statement-set: infunction-statement-any
#line 435 "bang.y"
    { yylhs.value.as < infunction_declaration_set_t > () = yystack_[0].value.as < infunction_declaration_set_t > (); }
#line 2348 "bang.tab.cpp"
    break;

  case 37: // infunction-statement-set: finished-infunction-statement-any
#line 435 "bang.y"
                               { yylhs.value.as < infunction_declaration_set_t > () = yystack_[0].value.as < infunction_declaration_set_t > (); }
#line 2354 "bang.tab.cpp"
    break;

  case 38: // generic-statement: let-decl
#line 439 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2360 "bang.tab.cpp"
    break;

  case 39: // generic-statement: "`return`" syntax-expression
#line 441 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2366 "bang.tab.cpp"
    break;

  case 40: // generic-statement: compound-expression
#line 443 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2372 "bang.tab.cpp"
    break;

  case 41: // infunction-statement: generic-statement
#line 448 "bang.y"
        { yylhs.value.as < infunction_statement > () = apply_visitor(statement_adopt_visitor<infunction_statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2378 "bang.tab.cpp"
    break;

  case 42: // infunction-statement: "`break`"
#line 450 "bang.y"
        { yylhs.value.as < infunction_statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2384 "bang.tab.cpp"
    break;

  case 43: // infunction-statement: "`continue`"
#line 452 "bang.y"
        { yylhs.value.as < infunction_statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2390 "bang.tab.cpp"
    break;

  case 44: // identifier: IDENTIFIER
#line 457 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2396 "bang.tab.cpp"
    break;

  case 45: // qname: "`::`" identifier
#line 466 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2402 "bang.tab.cpp"
    break;

  case 46: // qname: identifier
#line 468 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2408 "bang.tab.cpp"
    break;

  case 47: // qname: qname "`::`" identifier
#line 470 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2414 "bang.tab.cpp"
    break;

  case 48: // fn-start-decl: "`fn`"
#line 476 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2420 "bang.tab.cpp"
    break;

  case 49: // fn-start-decl: INLINE "`fn`"
#line 478 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2426 "bang.tab.cpp"
    break;

  case 50: // fn-decl: qname "`(`" parameter-list-opt "`)`"
#line 483 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2432 "bang.tab.cpp"
    break;

  case 51: // fn-decl: qname "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 485 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2438 "bang.tab.cpp"
    break;

  case 52: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 491 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2446 "bang.tab.cpp"
    break;

  case 53: // case-list-opt: %empty
#line 497 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2452 "bang.tab.cpp"
    break;

  case 54: // case-list-opt: case-list
#line 499 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2458 "bang.tab.cpp"
    break;

  case 55: // case-list: case-decl
#line 504 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2464 "bang.tab.cpp"
    break;

  case 56: // case-list: case-list "," case-decl
#line 507 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2470 "bang.tab.cpp"
    break;

  case 57: // case-decl: identifier
#line 512 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2476 "bang.tab.cpp"
    break;

  case 58: // field-list-opt: %empty
#line 537 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2482 "bang.tab.cpp"
    break;

  case 59: // field-list-opt: field-list
#line 538 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2488 "bang.tab.cpp"
    break;

  case 60: // field-list: field-decl
#line 543 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2494 "bang.tab.cpp"
    break;

  case 61: // field-list: field-list "," field-decl
#line 545 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2500 "bang.tab.cpp"
    break;

  case 62: // field-decl: IDENTIFIER "`:`" type-expr
#line 550 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2506 "bang.tab.cpp"
    break;

  case 63: // field-decl: type-expr
#line 552 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2512 "bang.tab.cpp"
    break;

  case 64: // parameter-name-decl: IDENTIFIER INTERNAL_IDENTIFIER "`:`"
#line 557 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())}; }
#line 2518 "bang.tab.cpp"
    break;

  case 65: // parameter-name-decl: IDENTIFIER "`:`"
#line 559 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), nullopt}; }
#line 2524 "bang.tab.cpp"
    break;

  case 66: // parameter-name-decl: INTERNAL_IDENTIFIER "`:`"
#line 561 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{nullopt, ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ()))}; }
#line 2530 "bang.tab.cpp"
    break;

  case 67: // parameter-constraint-modifier: TYPENAME
#line 566 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2536 "bang.tab.cpp"
    break;

  case 68: // parameter-constraint-modifier: "const modifier"
#line 568 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2542 "bang.tab.cpp"
    break;

  case 69: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 574 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2548 "bang.tab.cpp"
    break;

  case 70: // parameter-constraint-modifier-opt: %empty
#line 576 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2554 "bang.tab.cpp"
    break;

  case 71: // parameter-constraint-set: syntax-expression-wo-ii
#line 582 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2560 "bang.tab.cpp"
    break;

  case 72: // parameter-constraint-set: INTERNAL_IDENTIFIER
#line 584 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {}, { ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) } }; }
#line 2566 "bang.tab.cpp"
    break;

  case 73: // parameter-constraint-set: concept-expression
#line 586 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2572 "bang.tab.cpp"
    break;

  case 74: // parameter-constraint-set: parameter-constraint-set INTERNAL_IDENTIFIER
#line 588 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().bindings.emplace_back(ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()))); }
#line 2578 "bang.tab.cpp"
    break;

  case 75: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 590 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2584 "bang.tab.cpp"
    break;

  case 76: // concept-expression: "`@`" qname
#line 595 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 2590 "bang.tab.cpp"
    break;

  case 77: // syntax-expression-wo-ii: "true"
#line 601 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2596 "bang.tab.cpp"
    break;

  case 78: // syntax-expression-wo-ii: "false"
#line 603 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2602 "bang.tab.cpp"
    break;

  case 79: // syntax-expression-wo-ii: INTEGER
#line 605 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 2608 "bang.tab.cpp"
    break;

  case 80: // syntax-expression-wo-ii: DECIMAL
#line 607 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2614 "bang.tab.cpp"
    break;

  case 81: // syntax-expression-wo-ii: STRING
#line 609 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2620 "bang.tab.cpp"
    break;

  case 82: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 611 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2626 "bang.tab.cpp"
    break;

  case 83: // syntax-expression-wo-ii: qname
#line 613 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2632 "bang.tab.cpp"
    break;

  case 84: // syntax-expression-wo-ii: "`(`" syntax-expression "`)`"
#line 615 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2638 "bang.tab.cpp"
    break;

  case 85: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 617 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2644 "bang.tab.cpp"
    break;

  case 86: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 619 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2650 "bang.tab.cpp"
    break;

  case 87: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 621 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2656 "bang.tab.cpp"
    break;

  case 88: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 624 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2662 "bang.tab.cpp"
    break;

  case 89: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 626 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2668 "bang.tab.cpp"
    break;

  case 90: // syntax-expression-wo-ii: compound-expression
#line 629 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2674 "bang.tab.cpp"
    break;

  case 91: // syntax-expression: INTERNAL_IDENTIFIER
#line 634 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2680 "bang.tab.cpp"
    break;

  case 92: // syntax-expression: syntax-expression-wo-ii
#line 635 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2686 "bang.tab.cpp"
    break;

  case 93: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 660 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2692 "bang.tab.cpp"
    break;

  case 94: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 662 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2698 "bang.tab.cpp"
    break;

  case 95: // parameter-decl: parameter-constraint-set
#line 664 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2704 "bang.tab.cpp"
    break;

  case 96: // parameter-decl: "`_`"
#line 666 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, {} }; }
#line 2710 "bang.tab.cpp"
    break;

  case 97: // parameter-list-opt: %empty
#line 670 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2716 "bang.tab.cpp"
    break;

  case 98: // parameter-list-opt: parameter-list
#line 671 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2722 "bang.tab.cpp"
    break;

  case 99: // parameter-list: parameter-decl
#line 676 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2728 "bang.tab.cpp"
    break;

  case 100: // parameter-list: parameter-list "," parameter-decl
#line 678 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2734 "bang.tab.cpp"
    break;

  case 101: // type-expr: qname
#line 703 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()); }
#line 2740 "bang.tab.cpp"
    break;

  case 102: // type-expr: qname "`(`" opt-named-expr-list-any "`)`"
#line 705 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2746 "bang.tab.cpp"
    break;

  case 103: // type-expr: INTERNAL_IDENTIFIER
#line 707 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2752 "bang.tab.cpp"
    break;

  case 104: // type-expr: INTERNAL_IDENTIFIER "`(`" opt-named-expr-list-any "`)`"
#line 709 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2758 "bang.tab.cpp"
    break;

  case 105: // type-expr: "`[`" type-expr "`]`"
#line 711 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2764 "bang.tab.cpp"
    break;

  case 106: // type-expr: "`(`" field-list-opt "`)`"
#line 713 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2770 "bang.tab.cpp"
    break;

  case 107: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 715 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2776 "bang.tab.cpp"
    break;

  case 108: // type-expr: type-expr "`|`" type-expr
#line 717 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
        }
#line 2787 "bang.tab.cpp"
    break;

  case 109: // type-expr: type-expr "`->`" type-expr
#line 724 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2793 "bang.tab.cpp"
    break;

  case 110: // compound-expression: syntax-expression "`(`" opt-named-expr-list-any "`)`"
#line 741 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2799 "bang.tab.cpp"
    break;

  case 111: // compound-expression: syntax-expression "`=`" syntax-expression
#line 750 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2805 "bang.tab.cpp"
    break;

  case 112: // opt-named-expr-list-any: %empty
#line 801 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2811 "bang.tab.cpp"
    break;

  case 113: // opt-named-expr-list-any: opt-named-expr-list
#line 802 "bang.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2817 "bang.tab.cpp"
    break;

  case 114: // opt-named-expr-list: opt-named-expr
#line 807 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2823 "bang.tab.cpp"
    break;

  case 115: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 809 "bang.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2832 "bang.tab.cpp"
    break;

  case 116: // opt-named-expr: IDENTIFIER "`:`" syntax-expression
#line 817 "bang.y"
        { yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::tuple{ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), std::move(yystack_[0].value.as < syntax_expression_t > ())} }; }
#line 2838 "bang.tab.cpp"
    break;

  case 117: // opt-named-expr: syntax-expression
#line 819 "bang.y"
        { yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2844 "bang.tab.cpp"
    break;


#line 2848 "bang.tab.cpp"

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


  const signed char parser::yypact_ninf_ = -119;

  const signed char parser::yytable_ninf_ = -73;

  const short
  parser::yypact_[] =
  {
     186,  -119,  -119,  -119,  -119,  -119,  -119,    15,   120,   120,
       1,    21,   120,   120,   120,    28,   -50,  -119,    31,  -119,
    -119,    75,    25,   114,  -119,  -119,    57,    82,  -119,  -119,
    -119,    65,    31,  -119,  -119,   222,    36,  -119,   124,  -119,
      66,    15,  -119,    15,    31,   189,   219,   222,  -119,  -119,
      96,  -119,  -119,  -119,  -119,  -119,  -119,  -119,   166,   120,
      15,    41,    35,   120,   120,   120,  -119,   307,   120,  -119,
    -119,    98,  -119,   288,   120,  -119,    44,    15,   103,   196,
     166,   118,    47,   222,  -119,   297,   120,  -119,   133,   -16,
     -16,   107,   222,   108,    92,  -119,    66,   166,  -119,  -119,
     113,   216,  -119,   127,  -119,  -119,    31,   219,   137,  -119,
     132,   140,  -119,   307,   146,   160,   144,  -119,    47,    -2,
     307,   166,   202,   166,    22,   125,    31,  -119,  -119,  -119,
     -49,    10,   128,  -119,    37,  -119,   172,   154,   222,   120,
    -119,   307,    47,  -119,  -119,  -119,  -119,  -119,   137,  -119,
    -119,  -119,    15,   174,   166,  -119,   196,  -119,   182,  -119,
     179,    52,   190,  -119,  -119,    65,  -119,    10,    73,   128,
    -119,  -119,   220,   297,   222,  -119,  -119,  -119,    47,  -119,
    -119,  -119,  -119,   128,   120,  -119,   222
  };

  const signed char
  parser::yydefact_[] =
  {
       4,    81,    44,    91,    82,    79,    80,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,     0,    77,
      78,     0,     0,     0,     5,    38,    21,    17,     7,    16,
      46,    83,     0,    14,    92,     0,    90,    45,     0,    90,
      85,     0,    19,     0,     0,     0,     0,    39,    13,    49,
       0,     1,     2,    10,     3,     8,     6,     9,     0,     0,
       0,     0,     0,     0,     0,     0,    86,   112,     0,    84,
      20,     0,    12,    23,     0,    31,    33,    53,   103,    58,
       0,   101,    22,    18,    47,    97,     0,    35,   111,    88,
      89,    44,   117,     0,   113,   114,    87,     0,    43,    42,
      36,    37,    26,     0,    41,    24,     0,     0,     0,    57,
       0,    54,    55,   112,    44,     0,    59,    60,    63,     0,
     112,     0,     0,     0,    44,    91,     0,    96,    67,    68,
      70,     0,    95,    73,    92,    99,     0,    98,    15,     0,
     110,     0,    11,    29,    27,    28,    25,    30,     0,    32,
      34,    52,     0,     0,     0,   106,     0,   105,     0,   109,
       0,   108,     0,    65,    66,    76,    69,     0,    91,    94,
      74,    75,    50,     0,   116,   115,    56,   104,    62,    61,
     102,   107,    64,    93,     0,   100,    51
  };

  const short
  parser::yypgoto_[] =
  {
    -119,  -119,  -119,  -119,   214,  -119,  -119,  -119,  -119,  -119,
     -15,   -12,  -119,    70,   141,     2,   -10,    79,   -34,  -119,
    -119,  -119,    86,  -119,  -119,    95,  -119,   122,  -119,  -118,
     -35,   -58,    -8,    88,  -119,  -119,   -56,     3,   -81,  -119,
     123
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,    21,    22,    23,    24,    25,    26,    27,   100,   101,
      87,    28,   103,    29,   105,    30,    31,    32,    62,    33,
     110,   111,   112,   115,   116,   117,   130,   131,   167,   132,
     133,    34,    35,   135,   136,   137,   118,    39,    93,    94,
      95
  };

  const short
  parser::yytable_[] =
  {
      38,    40,    82,    36,    45,    46,    47,     2,    50,    37,
      72,    57,    42,   169,   121,     1,     2,   168,     4,     5,
       6,     2,    61,   126,   119,    52,    36,   134,    67,   162,
      75,    76,   153,    48,    61,    49,   -40,     2,    68,   158,
     128,   142,   129,    70,   -71,    71,   122,   157,    81,   183,
     -71,    83,    86,     7,     8,    88,    89,    90,   123,    92,
      96,   102,    84,   121,   163,   159,   107,   161,   121,    81,
      81,     9,   148,   134,     7,    51,    36,    53,   138,   109,
     -72,    73,   -71,   -40,    60,    85,   -72,    81,   -40,   145,
      43,    41,   149,   150,    59,   122,    61,   171,   178,    58,
     122,   -71,    19,    20,    36,    92,    44,   123,    60,   134,
      67,    81,    92,    81,    54,   134,   165,   108,   -72,     1,
       2,     3,     4,     5,     6,     1,     2,     3,     4,     5,
       6,   174,   -72,    92,   171,   170,    63,   -72,   -72,    60,
      97,   126,    77,   104,    81,    63,    81,   113,   171,   139,
      64,    65,   106,   140,   109,    66,   141,     7,     8,    64,
      65,    60,   120,     7,     8,   143,    55,   164,    67,    69,
     -72,   104,     2,    78,   147,     9,   186,    67,    68,   151,
     106,     9,    10,    73,    11,    12,    13,    68,   154,   -72,
      14,     1,     2,     3,     4,     5,     6,    15,    16,    17,
      18,    63,   114,    78,   152,   155,    19,    20,   156,     7,
      79,   160,    19,    20,    80,    64,    65,   172,   173,   177,
      66,     1,     2,     3,     4,     5,     6,   180,   181,     7,
       8,    63,   182,    67,    63,    73,   184,    56,   176,     7,
      79,    74,   146,    68,    80,    64,    65,     9,    64,    65,
      66,   179,   166,    66,    10,     0,    11,    12,    13,     7,
       8,   185,    14,    67,   175,    73,    67,     0,   144,    15,
      16,    17,    18,    68,     0,     0,    68,     9,    19,    20,
       0,     0,     0,     0,    10,     0,     0,    12,    13,     0,
      98,    99,    14,     1,     2,     3,     4,     5,     6,     0,
      16,    17,     1,   124,   125,     4,     5,     6,    19,    20,
     126,   127,     1,    91,     3,     4,     5,     6,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     9,
       7,     8,     0,     0,     0,     0,    10,     0,     9,    12,
      13,     0,    98,    99,    14,     0,     0,     0,     9,     0,
       0,     0,    16,    17,     0,     0,     0,     0,     0,     0,
      19,    20,     0,     0,     0,     0,   128,     0,   129,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    19,
      20
  };

  const short
  parser::yycheck_[] =
  {
       8,     9,    58,     0,    12,    13,    14,     6,    18,     7,
      44,    23,    10,   131,    16,     5,     6,     7,     8,     9,
      10,     6,    32,    13,    80,     0,    23,    85,    44,     7,
      45,    46,   113,     5,    44,    85,     0,     6,    54,   120,
      89,    97,    91,    41,     7,    43,    48,    49,    58,   167,
      13,    59,    17,    43,    44,    63,    64,    65,    60,    67,
      68,    73,    60,    16,    42,   121,    74,   123,    16,    79,
      80,    61,   106,   131,    43,     0,    73,    52,    86,    77,
       7,    46,    45,    47,    43,    44,    13,    97,    52,   101,
      69,    90,   107,   108,    12,    48,   106,   132,   154,    42,
      48,    64,    92,    93,   101,   113,    85,    60,    43,   167,
      44,   121,   120,   123,     0,   173,   126,    73,    45,     5,
       6,     7,     8,     9,    10,     5,     6,     7,     8,     9,
      10,   139,     7,   141,   169,     7,    12,    64,    13,    43,
      42,    13,    46,    73,   154,    12,   156,    44,   183,    42,
      26,    27,    73,    45,   152,    31,    64,    43,    44,    26,
      27,    43,    44,    43,    44,    52,    52,    42,    44,    45,
      45,   101,     6,     7,    47,    61,   184,    44,    54,    47,
     101,    61,    68,    46,    70,    71,    72,    54,    42,    64,
      76,     5,     6,     7,     8,     9,    10,    83,    84,    85,
      86,    12,     6,     7,    64,    45,    92,    93,    64,    43,
      44,     9,    92,    93,    48,    26,    27,    45,    64,    45,
      31,     5,     6,     7,     8,     9,    10,    45,    49,    43,
      44,    12,    42,    44,    12,    46,    16,    23,   152,    43,
      44,    52,   101,    54,    48,    26,    27,    61,    26,    27,
      31,   156,   130,    31,    68,    -1,    70,    71,    72,    43,
      44,   173,    76,    44,   141,    46,    44,    -1,    52,    83,
      84,    85,    86,    54,    -1,    -1,    54,    61,    92,    93,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    71,    72,    -1,
      74,    75,    76,     5,     6,     7,     8,     9,    10,    -1,
      84,    85,     5,     6,     7,     8,     9,    10,    92,    93,
      13,    14,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      43,    44,    -1,    -1,    -1,    -1,    68,    -1,    61,    71,
      72,    -1,    74,    75,    76,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    -1,    -1,    -1,    89,    -1,    91,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    43,    44,    61,
      68,    70,    71,    72,    76,    83,    84,    85,    86,    92,
      93,    95,    96,    97,    98,    99,   100,   101,   105,   107,
     109,   110,   111,   113,   125,   126,   131,   109,   126,   131,
     126,    90,   109,    69,    85,   126,   126,   126,     5,    85,
     110,     0,     0,    52,     0,    52,    98,   105,    42,    12,
      43,   110,   112,    12,    26,    27,    31,    44,    54,    45,
     109,   109,   112,    46,    52,   104,   104,    46,     7,    44,
      48,   110,   130,   126,   109,    44,    17,   104,   126,   126,
     126,     6,   126,   132,   133,   134,   126,    42,    74,    75,
     102,   103,   105,   106,   107,   108,   111,   126,    73,   109,
     114,   115,   116,    44,     6,   117,   118,   119,   130,   130,
      44,    16,    48,    60,     6,     7,    13,    14,    89,    91,
     120,   121,   123,   124,   125,   127,   128,   129,   126,    42,
      45,    64,   130,    52,    52,   105,   108,    47,   112,   104,
     104,    47,    64,   132,    42,    45,    64,    49,   132,   130,
       9,   130,     7,    42,    42,   110,   121,   122,     7,   123,
       7,   124,    45,    64,   126,   134,   116,    45,   130,   119,
      45,    49,    42,   123,    16,   127,   126
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    94,    95,    95,    96,    96,    96,    97,    97,    97,
      97,    98,    98,    98,    98,    98,    98,    99,    99,   100,
     100,   101,   101,   102,   102,   102,   103,   103,   103,   103,
     104,   105,   105,   105,   105,   105,   106,   106,   107,   107,
     107,   108,   108,   108,   109,   110,   110,   110,   111,   111,
     112,   112,   113,   114,   114,   115,   115,   116,   117,   117,
     118,   118,   119,   119,   120,   120,   120,   121,   121,   122,
     122,   123,   123,   123,   123,   123,   124,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   126,   126,   127,   127,   127,   127,   128,   128,   129,
     129,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     131,   131,   132,   132,   133,   133,   134,   134
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     3,     2,     1,     4,     1,     1,     3,     2,
       3,     1,     3,     0,     1,     2,     1,     2,     2,     2,
       3,     3,     5,     3,     5,     3,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     2,     1,     3,     1,     2,
       4,     6,     5,     0,     1,     1,     3,     1,     0,     1,
       1,     3,     3,     1,     3,     2,     2,     1,     1,     1,
       0,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     2,     3,     3,     3,
       1,     1,     1,     3,     2,     1,     1,     0,     1,     1,
       3,     1,     4,     1,     4,     3,     3,     4,     3,     3,
       4,     3,     0,     1,     1,     3,     3,     1
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
  "\"`fn`\"", "ENUM", "TYPE", "EXTENDS", "TYPENAME", "\"weak modifier\"",
  "\"const modifier\"", "\"true\"", "\"false\"", "$accept", "begin",
  "statement_any", "finished_statement_any", "statement", "let-decl",
  "let-decl-start", "let-decl-start-with-opt-type",
  "infunction-statement-any", "finished-infunction-statement-any",
  "braced_statements", "finished_statement", "infunction-statement-set",
  "generic-statement", "infunction-statement", "identifier", "qname",
  "fn-start-decl", "fn-decl", "enum-decl", "case-list-opt", "case-list",
  "case-decl", "field-list-opt", "field-list", "field-decl",
  "parameter-name-decl", "parameter-constraint-modifier",
  "parameter-constraint-modifier-opt", "parameter-constraint-set",
  "concept-expression", "syntax-expression-wo-ii", "syntax-expression",
  "parameter-decl", "parameter-list-opt", "parameter-list", "type-expr",
  "compound-expression", "opt-named-expr-list-any", "opt-named-expr-list",
  "opt-named-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   318,   318,   319,   323,   325,   327,   344,   346,   347,
     349,   354,   356,   358,   360,   365,   367,   380,   382,   387,
     389,   394,   395,   400,   402,   404,   409,   410,   411,   413,
     417,   422,   424,   426,   428,   430,   435,   435,   438,   440,
     442,   447,   449,   451,   456,   465,   467,   469,   475,   477,
     482,   484,   490,   497,   499,   503,   506,   511,   537,   538,
     542,   544,   549,   551,   556,   558,   560,   565,   567,   574,
     575,   581,   583,   585,   587,   589,   594,   600,   602,   604,
     606,   608,   610,   612,   614,   616,   618,   620,   623,   625,
     628,   633,   635,   659,   661,   663,   665,   670,   671,   675,
     677,   702,   704,   706,   708,   710,   712,   714,   716,   723,
     740,   749,   800,   802,   806,   808,   816,   818
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
      85,    86,    87,    88,    89,    90,    91,    92,    93
    };
    // Last valid token kind.
    const int code_max = 348;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3565 "bang.tab.cpp"

#line 1147 "bang.y"

