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

      case symbol_kind::S_127_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.copy< declaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.copy< generic_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.copy< infunction_declaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.copy< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
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

      case symbol_kind::S_113_case_decl: // case-decl
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
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
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

      case symbol_kind::S_127_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< declaration > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.move< generic_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.move< infunction_declaration > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.move< infunction_declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
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

      case symbol_kind::S_113_case_decl: // case-decl
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
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
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

      case symbol_kind::S_127_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.YY_MOVE_OR_COPY< declaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< infunction_declaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.YY_MOVE_OR_COPY< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
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

      case symbol_kind::S_113_case_decl: // case-decl
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
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
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

      case symbol_kind::S_127_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< declaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.move< generic_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.move< infunction_declaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.move< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
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

      case symbol_kind::S_113_case_decl: // case-decl
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
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
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

      case symbol_kind::S_127_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.copy< declaration > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.copy< generic_statement > (that.value);
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.copy< infunction_declaration > (that.value);
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.copy< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
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

      case symbol_kind::S_113_case_decl: // case-decl
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
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
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

      case symbol_kind::S_127_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.move< declaration > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.move< generic_statement > (that.value);
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.move< infunction_declaration > (that.value);
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.move< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
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

      case symbol_kind::S_113_case_decl: // case-decl
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
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
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
#line 317 "bang.y"
                 { }
#line 1309 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 317 "bang.y"
                 { }
#line 1315 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 317 "bang.y"
                 { }
#line 1321 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 317 "bang.y"
                 { }
#line 1327 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 317 "bang.y"
                 { }
#line 1333 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 317 "bang.y"
                 { }
#line 1339 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 317 "bang.y"
                 { }
#line 1345 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 317 "bang.y"
                 { }
#line 1351 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 317 "bang.y"
                 { }
#line 1357 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 317 "bang.y"
                 { }
#line 1363 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 317 "bang.y"
                 { }
#line 1369 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 317 "bang.y"
                 { }
#line 1375 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 317 "bang.y"
                 { }
#line 1381 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 317 "bang.y"
                 { }
#line 1387 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 317 "bang.y"
                 { }
#line 1393 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 317 "bang.y"
                 { }
#line 1399 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 317 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 317 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 317 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 317 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "continue"
#line 317 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "break"
#line 317 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 317 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 317 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 317 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 317 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 317 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_100_let_decl: // let-decl
#line 317 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_101_let_decl_start: // let-decl-start
#line 317 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 317 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
#line 317 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
#line 317 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
#line 317 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 317 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 317 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
#line 317 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
#line 317 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
#line 317 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
#line 317 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_case_list: // case-list
#line 317 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_case_decl: // case-decl
#line 317 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
#line 317 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_field_list: // field-list
#line 317 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_field_decl: // field-decl
#line 317 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
#line 317 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
#line 317 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 317 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
#line 317 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
#line 317 "bang.y"
                 { }
#line 1597 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 317 "bang.y"
                 { }
#line 1603 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_syntax_expression: // syntax-expression
#line 317 "bang.y"
                 { }
#line 1609 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
#line 317 "bang.y"
                 { }
#line 1615 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
#line 317 "bang.y"
                 { }
#line 1621 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_parameter_list: // parameter-list
#line 317 "bang.y"
                 { }
#line 1627 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_type_expr: // type-expr
#line 317 "bang.y"
                 { }
#line 1633 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_compound_expression: // compound-expression
#line 317 "bang.y"
                 { }
#line 1639 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
#line 317 "bang.y"
                 { }
#line 1645 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
#line 317 "bang.y"
                 { }
#line 1651 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
#line 317 "bang.y"
                 { }
#line 1657 "bang.tab.cpp"
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

      case symbol_kind::S_127_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_statement: // statement
        yylhs.value.emplace< declaration > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement > ();
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        yylhs.value.emplace< infunction_declaration > ();
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        yylhs.value.emplace< infunction_declaration_set_t > ();
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement_decl_t > ();
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
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

      case symbol_kind::S_113_case_decl: // case-decl
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
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
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
#line 322 "bang.y"
                          { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 2058 "bang.tab.cpp"
    break;

  case 3: // statement_any: %empty
#line 327 "bang.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 2064 "bang.tab.cpp"
    break;

  case 4: // statement_any: statement_any "`;`"
#line 329 "bang.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 2070 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement_any statement
#line 331 "bang.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < declaration > ()));
        }
#line 2079 "bang.tab.cpp"
    break;

  case 6: // statement: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 351 "bang.y"
        { yylhs.value.as < declaration > () = extern_var{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; }
#line 2085 "bang.tab.cpp"
    break;

  case 7: // statement: EXTERN "`fn`" fn-decl "`;`"
#line 353 "bang.y"
        { yylhs.value.as < declaration > () = std::move(yystack_[1].value.as < fn_pure_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2091 "bang.tab.cpp"
    break;

  case 8: // statement: INCLUDE STRING
#line 355 "bang.y"
        { yylhs.value.as < declaration > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2097 "bang.tab.cpp"
    break;

  case 9: // statement: enum-decl
#line 357 "bang.y"
        { yylhs.value.as < declaration > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2103 "bang.tab.cpp"
    break;

  case 10: // statement: fn-start-decl fn-decl "`{`" infunction-statement-any "`}`"
#line 361 "bang.y"
        { yystack_[3].value.as < fn_pure_t > ().kind = yystack_[4].value.as < fn_kind > (); yylhs.value.as < declaration > () = fn_decl_t{ std::move(yystack_[3].value.as < fn_pure_t > ()), std::move(yystack_[1].value.as < infunction_declaration_set_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2109 "bang.tab.cpp"
    break;

  case 11: // statement: fn-start-decl fn-decl "`=>`" syntax-expression "`;`"
#line 363 "bang.y"
        { yystack_[3].value.as < fn_pure_t > ().kind = yystack_[4].value.as < fn_kind > (); yylhs.value.as < declaration > () = fn_decl_t{ std::move(yystack_[3].value.as < fn_pure_t > ()), { return_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) } } }; }
#line 2115 "bang.tab.cpp"
    break;

  case 12: // statement: generic-statement
#line 365 "bang.y"
        { yylhs.value.as < declaration > () = apply_visitor(statement_adopt_visitor<declaration>{}, yystack_[0].value.as < generic_statement > ()); }
#line 2121 "bang.tab.cpp"
    break;

  case 13: // let-decl: let-decl-start-with-opt-type "`;`"
#line 384 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[1].value.as < let_statement_decl_t > ()); }
#line 2127 "bang.tab.cpp"
    break;

  case 14: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression "`;`"
#line 386 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[3].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[1].value.as < syntax_expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2133 "bang.tab.cpp"
    break;

  case 15: // let-decl-start: LET identifier
#line 391 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2139 "bang.tab.cpp"
    break;

  case 16: // let-decl-start: LET "weak modifier" identifier
#line 393 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2145 "bang.tab.cpp"
    break;

  case 17: // let-decl-start-with-opt-type: let-decl-start
#line 397 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 2151 "bang.tab.cpp"
    break;

  case 18: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 399 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 2157 "bang.tab.cpp"
    break;

  case 19: // infunction-statement-any: %empty
#line 404 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = {}; }
#line 2163 "bang.tab.cpp"
    break;

  case 20: // infunction-statement-any: infunction-statement-any "`;`"
#line 406 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); }
#line 2169 "bang.tab.cpp"
    break;

  case 21: // infunction-statement-any: infunction-statement-any infunction-statement
#line 408 "bang.y"
        {
	        yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ());
            yylhs.value.as < infunction_declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration > ()));
        }
#line 2178 "bang.tab.cpp"
    break;

  case 22: // generic-statement: let-decl
#line 416 "bang.y"
        { yylhs.value.as < generic_statement > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2184 "bang.tab.cpp"
    break;

  case 23: // generic-statement: RETURN syntax-expression "`;`"
#line 418 "bang.y"
        { yylhs.value.as < generic_statement > () = return_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2190 "bang.tab.cpp"
    break;

  case 24: // generic-statement: WHILE syntax-expression "`{`" infunction-statement-any "`}`"
#line 420 "bang.y"
        { yylhs.value.as < generic_statement > () = while_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < infunction_declaration_set_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2196 "bang.tab.cpp"
    break;

  case 25: // generic-statement: IF syntax-expression "`{`" infunction-statement-any "`}`"
#line 422 "bang.y"
        { yylhs.value.as < generic_statement > () = if_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < infunction_declaration_set_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2202 "bang.tab.cpp"
    break;

  case 26: // generic-statement: compound-expression "`;`"
#line 424 "bang.y"
        { yylhs.value.as < generic_statement > () = expression_statement_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2208 "bang.tab.cpp"
    break;

  case 27: // infunction-statement: generic-statement
#line 429 "bang.y"
        { yylhs.value.as < infunction_declaration > () = apply_visitor(statement_adopt_visitor<infunction_declaration>{}, yystack_[0].value.as < generic_statement > ()); }
#line 2214 "bang.tab.cpp"
    break;

  case 28: // infunction-statement: "break"
#line 431 "bang.y"
        { yylhs.value.as < infunction_declaration > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2220 "bang.tab.cpp"
    break;

  case 29: // infunction-statement: "continue"
#line 433 "bang.y"
        { yylhs.value.as < infunction_declaration > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2226 "bang.tab.cpp"
    break;

  case 30: // identifier: IDENTIFIER
#line 438 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2232 "bang.tab.cpp"
    break;

  case 31: // qname: "`::`" identifier
#line 447 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2238 "bang.tab.cpp"
    break;

  case 32: // qname: identifier
#line 449 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2244 "bang.tab.cpp"
    break;

  case 33: // qname: qname "`::`" identifier
#line 451 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2250 "bang.tab.cpp"
    break;

  case 34: // fn-start-decl: "`fn`"
#line 457 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2256 "bang.tab.cpp"
    break;

  case 35: // fn-start-decl: INLINE "`fn`"
#line 459 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2262 "bang.tab.cpp"
    break;

  case 36: // fn-decl: qname "`(`" parameter-list-opt "`)`"
#line 464 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2268 "bang.tab.cpp"
    break;

  case 37: // fn-decl: qname "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 466 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2274 "bang.tab.cpp"
    break;

  case 38: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 472 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2282 "bang.tab.cpp"
    break;

  case 39: // case-list-opt: %empty
#line 478 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2288 "bang.tab.cpp"
    break;

  case 40: // case-list-opt: case-list
#line 480 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2294 "bang.tab.cpp"
    break;

  case 41: // case-list: case-decl
#line 485 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2300 "bang.tab.cpp"
    break;

  case 42: // case-list: case-list "," case-decl
#line 488 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2306 "bang.tab.cpp"
    break;

  case 43: // case-decl: identifier
#line 493 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2312 "bang.tab.cpp"
    break;

  case 44: // field-list-opt: %empty
#line 518 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2318 "bang.tab.cpp"
    break;

  case 45: // field-list-opt: field-list
#line 519 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2324 "bang.tab.cpp"
    break;

  case 46: // field-list: field-decl
#line 524 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2330 "bang.tab.cpp"
    break;

  case 47: // field-list: field-list "," field-decl
#line 526 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2336 "bang.tab.cpp"
    break;

  case 48: // field-decl: IDENTIFIER "`:`" type-expr
#line 531 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2342 "bang.tab.cpp"
    break;

  case 49: // field-decl: type-expr
#line 533 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2348 "bang.tab.cpp"
    break;

  case 50: // parameter-name-decl: IDENTIFIER INTERNAL_IDENTIFIER "`:`"
#line 538 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())}; }
#line 2354 "bang.tab.cpp"
    break;

  case 51: // parameter-name-decl: IDENTIFIER "`:`"
#line 540 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), nullopt}; }
#line 2360 "bang.tab.cpp"
    break;

  case 52: // parameter-name-decl: INTERNAL_IDENTIFIER "`:`"
#line 542 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{nullopt, ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ()))}; }
#line 2366 "bang.tab.cpp"
    break;

  case 53: // parameter-constraint-modifier: TYPENAME
#line 547 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2372 "bang.tab.cpp"
    break;

  case 54: // parameter-constraint-modifier: "const modifier"
#line 549 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2378 "bang.tab.cpp"
    break;

  case 55: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 555 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2384 "bang.tab.cpp"
    break;

  case 56: // parameter-constraint-modifier-opt: %empty
#line 557 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2390 "bang.tab.cpp"
    break;

  case 57: // parameter-constraint-set: syntax-expression-wo-ii
#line 563 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2396 "bang.tab.cpp"
    break;

  case 58: // parameter-constraint-set: INTERNAL_IDENTIFIER
#line 565 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {}, { ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) } }; }
#line 2402 "bang.tab.cpp"
    break;

  case 59: // parameter-constraint-set: concept-expression
#line 567 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2408 "bang.tab.cpp"
    break;

  case 60: // parameter-constraint-set: parameter-constraint-set INTERNAL_IDENTIFIER
#line 569 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().bindings.emplace_back(ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()))); }
#line 2414 "bang.tab.cpp"
    break;

  case 61: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 571 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2420 "bang.tab.cpp"
    break;

  case 62: // concept-expression: "`@`" qname
#line 576 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 2426 "bang.tab.cpp"
    break;

  case 63: // syntax-expression-wo-ii: "true"
#line 582 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2432 "bang.tab.cpp"
    break;

  case 64: // syntax-expression-wo-ii: "false"
#line 584 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2438 "bang.tab.cpp"
    break;

  case 65: // syntax-expression-wo-ii: INTEGER
#line 586 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 2444 "bang.tab.cpp"
    break;

  case 66: // syntax-expression-wo-ii: DECIMAL
#line 588 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2450 "bang.tab.cpp"
    break;

  case 67: // syntax-expression-wo-ii: STRING
#line 590 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2456 "bang.tab.cpp"
    break;

  case 68: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 592 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2462 "bang.tab.cpp"
    break;

  case 69: // syntax-expression-wo-ii: qname
#line 594 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2468 "bang.tab.cpp"
    break;

  case 70: // syntax-expression-wo-ii: "`(`" syntax-expression "`)`"
#line 596 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2474 "bang.tab.cpp"
    break;

  case 71: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 598 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2480 "bang.tab.cpp"
    break;

  case 72: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 600 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2486 "bang.tab.cpp"
    break;

  case 73: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 602 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2492 "bang.tab.cpp"
    break;

  case 74: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 605 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2498 "bang.tab.cpp"
    break;

  case 75: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 607 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2504 "bang.tab.cpp"
    break;

  case 76: // syntax-expression-wo-ii: compound-expression
#line 610 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2510 "bang.tab.cpp"
    break;

  case 77: // syntax-expression: INTERNAL_IDENTIFIER
#line 615 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2516 "bang.tab.cpp"
    break;

  case 78: // syntax-expression: syntax-expression-wo-ii
#line 616 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2522 "bang.tab.cpp"
    break;

  case 79: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 641 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2528 "bang.tab.cpp"
    break;

  case 80: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 643 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2534 "bang.tab.cpp"
    break;

  case 81: // parameter-decl: parameter-constraint-set
#line 645 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2540 "bang.tab.cpp"
    break;

  case 82: // parameter-decl: "`_`"
#line 647 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, {} }; }
#line 2546 "bang.tab.cpp"
    break;

  case 83: // parameter-list-opt: %empty
#line 651 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2552 "bang.tab.cpp"
    break;

  case 84: // parameter-list-opt: parameter-list
#line 652 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2558 "bang.tab.cpp"
    break;

  case 85: // parameter-list: parameter-decl
#line 657 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2564 "bang.tab.cpp"
    break;

  case 86: // parameter-list: parameter-list "," parameter-decl
#line 659 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2570 "bang.tab.cpp"
    break;

  case 87: // type-expr: qname
#line 684 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()); }
#line 2576 "bang.tab.cpp"
    break;

  case 88: // type-expr: qname "`(`" opt-named-expr-list-any "`)`"
#line 686 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2582 "bang.tab.cpp"
    break;

  case 89: // type-expr: INTERNAL_IDENTIFIER
#line 688 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2588 "bang.tab.cpp"
    break;

  case 90: // type-expr: INTERNAL_IDENTIFIER "`(`" opt-named-expr-list-any "`)`"
#line 690 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2594 "bang.tab.cpp"
    break;

  case 91: // type-expr: "`[`" type-expr "`]`"
#line 692 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2600 "bang.tab.cpp"
    break;

  case 92: // type-expr: "`(`" field-list-opt "`)`"
#line 694 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2606 "bang.tab.cpp"
    break;

  case 93: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 696 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2612 "bang.tab.cpp"
    break;

  case 94: // type-expr: type-expr "`|`" type-expr
#line 698 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
        }
#line 2623 "bang.tab.cpp"
    break;

  case 95: // type-expr: type-expr "`->`" type-expr
#line 705 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2629 "bang.tab.cpp"
    break;

  case 96: // compound-expression: syntax-expression "`(`" opt-named-expr-list-any "`)`"
#line 722 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2635 "bang.tab.cpp"
    break;

  case 97: // compound-expression: syntax-expression "`=`" syntax-expression
#line 731 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2641 "bang.tab.cpp"
    break;

  case 98: // opt-named-expr-list-any: %empty
#line 782 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2647 "bang.tab.cpp"
    break;

  case 99: // opt-named-expr-list-any: opt-named-expr-list
#line 783 "bang.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2653 "bang.tab.cpp"
    break;

  case 100: // opt-named-expr-list: opt-named-expr
#line 788 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2659 "bang.tab.cpp"
    break;

  case 101: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 790 "bang.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2668 "bang.tab.cpp"
    break;

  case 102: // opt-named-expr: IDENTIFIER "`:`" syntax-expression
#line 798 "bang.y"
        { yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::tuple{ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), std::move(yystack_[0].value.as < syntax_expression_t > ())} }; }
#line 2674 "bang.tab.cpp"
    break;

  case 103: // opt-named-expr: syntax-expression
#line 800 "bang.y"
        { yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2680 "bang.tab.cpp"
    break;


#line 2684 "bang.tab.cpp"

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

  const signed char parser::yytable_ninf_ = -59;

  const short
  parser::yypact_[] =
  {
    -113,    17,    49,  -113,  -113,  -113,  -113,  -113,  -113,  -113,
    -113,    16,   225,  -113,   225,     0,   -49,   225,    72,    -5,
    -113,     9,   225,   225,  -113,  -113,  -113,  -113,    47,    -9,
    -113,  -113,    56,     9,  -113,  -113,   315,    39,  -113,   245,
    -113,    58,    16,  -113,    16,     9,   248,  -113,  -113,   -25,
     270,   282,   305,   225,  -113,    16,   123,    57,   225,   225,
     225,  -113,   244,   225,  -113,  -113,  -113,    83,    84,  -113,
      16,  -113,  -113,    96,   344,   305,   139,    19,   291,  -113,
      20,   225,  -113,   340,   -30,   -30,   104,   315,   119,   120,
    -113,    58,   305,  -113,  -113,   121,   122,  -113,    77,   146,
     244,   143,   142,   124,  -113,    19,   131,   244,   305,   183,
     305,  -113,    -3,    63,     9,  -113,  -113,  -113,    48,   168,
      99,  -113,    59,  -113,   155,   130,   303,   152,   225,  -113,
     244,   149,  -113,    16,  -113,  -113,  -113,  -113,  -113,  -113,
    -113,   157,   305,  -113,   344,  -113,   160,  -113,   159,    -4,
     164,  -113,  -113,    56,  -113,   168,   203,    99,  -113,  -113,
     199,    20,  -113,  -113,   315,  -113,  -113,  -113,  -113,    19,
    -113,  -113,  -113,  -113,    99,   225,  -113,   315
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    67,    30,    77,    68,    65,
      66,     0,     0,     4,     0,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    63,    64,     5,    22,    17,     0,
      12,    32,    69,     0,     9,    78,     0,    76,    31,     0,
      76,    71,     0,    15,     0,     0,     0,     8,    35,     0,
       0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
       0,    72,    98,     0,    26,    70,    16,     0,     0,    23,
      39,    19,    19,    89,    44,     0,    87,    18,     0,    33,
      83,     0,    19,    97,    74,    75,    30,   103,     0,    99,
     100,    73,     0,     7,    43,     0,    40,    41,     0,     0,
      98,    30,     0,    45,    46,    49,     0,    98,     0,     0,
       0,    14,    30,    77,     0,    82,    53,    54,    56,     0,
      81,    59,    78,    85,     0,    84,     0,     0,     0,    96,
       0,     0,    38,     0,    24,    20,    29,    28,    27,    21,
      25,     0,     0,    92,     0,    91,     0,    95,     0,    94,
       0,    51,    52,    62,    55,     0,    77,    80,    60,    61,
      36,     0,    11,    10,   102,   101,     6,    42,    90,    48,
      47,    88,    93,    50,    79,     0,    86,    37
  };

  const short
  parser::yypgoto_[] =
  {
    -113,  -113,  -113,  -113,  -113,  -113,  -113,    -7,   219,  -113,
      -2,   -14,  -113,   177,  -113,  -113,  -113,    94,  -113,  -113,
      92,  -113,   110,  -113,   -82,  -112,   -48,   -12,    76,  -113,
    -113,   151,    -1,   -84,  -113,   112
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    26,    27,    28,    29,    98,   138,   139,
      31,    32,    33,    57,    34,    95,    96,    97,   102,   103,
     104,   118,   119,   155,   120,   121,    35,    36,   123,   124,
     125,   105,    40,    88,    89,    90
  };

  const short
  parser::yytable_[] =
  {
      39,    37,    41,    53,   150,    46,     6,    49,   159,    38,
      50,    51,   108,    43,    62,     6,   141,     3,    55,    56,
      44,    70,     6,   146,    63,     5,   112,   113,     8,     9,
      10,    56,   122,   114,   115,   108,    45,   157,    76,   151,
      66,    78,    67,    54,   109,   159,    83,    84,    85,     4,
      87,    91,    11,    79,     5,     6,     7,     8,     9,    10,
      76,    76,   159,    11,    12,    99,   -57,   109,    94,   126,
     -58,   122,   -57,   174,    81,   127,   -58,    47,    76,   110,
      48,    14,     5,     6,     7,     8,     9,    10,    87,    52,
      42,    64,    11,    12,    76,    87,    76,    37,    37,    55,
     153,    13,    62,    82,   -57,   152,   158,   122,   -58,   116,
      14,   117,   114,   122,    24,    25,   164,    15,    87,    16,
      11,    12,    17,   -57,   134,    92,    37,   -58,    76,   135,
      76,    94,    18,    19,    20,    21,    93,   116,    14,   117,
     100,    22,    23,    24,    25,    15,   128,   108,   136,   137,
      17,     5,     6,     7,     8,     9,    10,     5,     6,     7,
       8,     9,    10,   177,   129,   108,    55,    80,   132,    22,
      23,    24,    25,     5,     6,   156,     8,     9,    10,   109,
     145,   114,    55,   107,   130,   142,   133,   143,   144,    11,
      12,   110,   148,   140,   161,    11,    12,   109,   135,   163,
     160,   166,   168,    77,   135,   171,   173,    14,   172,   110,
     -58,    11,    12,    14,    15,   175,   -58,   136,   137,    17,
      15,    30,    68,   136,   137,    17,   106,   167,   154,    14,
       5,     6,     7,     8,     9,    10,   170,   176,    22,    23,
      24,    25,   165,   131,    22,    23,    24,    25,   -58,     5,
      86,     7,     8,     9,    10,     0,     0,    58,     0,   147,
      58,   149,    24,    25,     0,     0,     0,   -58,    11,    12,
       0,    59,    60,     0,    59,    60,    61,     0,     0,    61,
       0,     0,    58,     0,     0,     0,    14,    11,    12,    62,
      65,     0,    62,   169,    58,     0,    59,    60,     0,    63,
      69,    61,    63,    58,     0,    14,     0,     0,    59,    60,
       0,     6,    73,    61,    62,    58,    71,    59,    60,    24,
      25,     0,    61,     0,    63,     0,    62,    58,    72,    59,
      60,     0,     0,     0,    61,    62,    63,     0,    24,    25,
       0,    59,    60,   111,     0,    63,    61,    62,    11,    74,
     101,    73,    58,    75,     0,   162,     0,    63,     0,    62,
       0,     0,     0,     0,     0,     0,    59,    60,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,    11,    74,     0,
       0,     0,    75,     0,    63
  };

  const short
  parser::yycheck_[] =
  {
      12,     2,    14,    12,     7,    17,     6,    21,   120,    11,
      22,    23,    16,    15,    44,     6,   100,     0,    43,    33,
      69,    46,     6,   107,    54,     5,     6,     7,     8,     9,
      10,    45,    80,    13,    14,    16,    85,   119,    52,    42,
      42,    53,    44,    52,    48,   157,    58,    59,    60,     0,
      62,    63,    43,    55,     5,     6,     7,     8,     9,    10,
      74,    75,   174,    43,    44,    72,     7,    48,    70,    81,
       7,   119,    13,   155,    17,    82,    13,     5,    92,    60,
      85,    61,     5,     6,     7,     8,     9,    10,   100,    42,
      90,    52,    43,    44,   108,   107,   110,    98,    99,    43,
     114,    52,    44,    46,    45,    42,     7,   155,    45,    89,
      61,    91,    13,   161,    94,    95,   128,    68,   130,    70,
      43,    44,    73,    64,    47,    42,   127,    64,   142,    52,
     144,   133,    83,    84,    85,    86,    52,    89,    61,    91,
      44,    92,    93,    94,    95,    68,    42,    16,    71,    72,
      73,     5,     6,     7,     8,     9,    10,     5,     6,     7,
       8,     9,    10,   175,    45,    16,    43,    44,    47,    92,
      93,    94,    95,     5,     6,     7,     8,     9,    10,    48,
      49,    13,    43,    44,    64,    42,    64,    45,    64,    43,
      44,    60,     9,    47,    64,    43,    44,    48,    52,    47,
      45,    52,    45,    52,    52,    45,    42,    61,    49,    60,
       7,    43,    44,    61,    68,    16,    13,    71,    72,    73,
      68,     2,    45,    71,    72,    73,    75,   133,   118,    61,
       5,     6,     7,     8,     9,    10,   144,   161,    92,    93,
      94,    95,   130,    92,    92,    93,    94,    95,    45,     5,
       6,     7,     8,     9,    10,    -1,    -1,    12,    -1,   108,
      12,   110,    94,    95,    -1,    -1,    -1,    64,    43,    44,
      -1,    26,    27,    -1,    26,    27,    31,    -1,    -1,    31,
      -1,    -1,    12,    -1,    -1,    -1,    61,    43,    44,    44,
      45,    -1,    44,   142,    12,    -1,    26,    27,    -1,    54,
      52,    31,    54,    12,    -1,    61,    -1,    -1,    26,    27,
      -1,     6,     7,    31,    44,    12,    46,    26,    27,    94,
      95,    -1,    31,    -1,    54,    -1,    44,    12,    46,    26,
      27,    -1,    -1,    -1,    31,    44,    54,    -1,    94,    95,
      -1,    26,    27,    52,    -1,    54,    31,    44,    43,    44,
       6,     7,    12,    48,    -1,    52,    -1,    54,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    54
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    97,    98,     0,     0,     5,     6,     7,     8,     9,
      10,    43,    44,    52,    61,    68,    70,    73,    83,    84,
      85,    86,    92,    93,    94,    95,    99,   100,   101,   102,
     104,   106,   107,   108,   110,   122,   123,   128,   106,   123,
     128,   123,    90,   106,    69,    85,   123,     5,    85,   107,
     123,   123,    42,    12,    52,    43,   107,   109,    12,    26,
      27,    31,    44,    54,    52,    45,   106,   106,   109,    52,
      46,    46,    46,     7,    44,    48,   107,   127,   123,   106,
      44,    17,    46,   123,   123,   123,     6,   123,   129,   130,
     131,   123,    42,    52,   106,   111,   112,   113,   103,   103,
      44,     6,   114,   115,   116,   127,   127,    44,    16,    48,
      60,    52,     6,     7,    13,    14,    89,    91,   117,   118,
     120,   121,   122,   124,   125,   126,   123,   103,    42,    45,
      64,   127,    47,    64,    47,    52,    71,    72,   104,   105,
      47,   129,    42,    45,    64,    49,   129,   127,     9,   127,
       7,    42,    42,   107,   118,   119,     7,   120,     7,   121,
      45,    64,    52,    47,   123,   131,    52,   113,    45,   127,
     116,    45,    49,    42,   120,    16,   124,   123
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    96,    97,    98,    98,    98,    99,    99,    99,    99,
      99,    99,    99,   100,   100,   101,   101,   102,   102,   103,
     103,   103,   104,   104,   104,   104,   104,   105,   105,   105,
     106,   107,   107,   107,   108,   108,   109,   109,   110,   111,
     111,   112,   112,   113,   114,   114,   115,   115,   116,   116,
     117,   117,   117,   118,   118,   119,   119,   120,   120,   120,
     120,   120,   121,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   124,
     124,   124,   124,   125,   125,   126,   126,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   128,   128,   129,   129,
     130,   130,   131,   131
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     6,     4,     2,     1,
       5,     5,     1,     2,     4,     2,     3,     1,     3,     0,
       2,     2,     1,     3,     5,     5,     2,     1,     1,     1,
       1,     2,     1,     3,     1,     2,     4,     6,     5,     0,
       1,     1,     3,     1,     0,     1,     1,     3,     3,     1,
       3,     2,     2,     1,     1,     1,     0,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     2,     3,     3,     3,     1,     1,     1,     3,
       2,     1,     1,     0,     1,     1,     3,     1,     4,     1,
       4,     3,     3,     4,     3,     3,     4,     3,     0,     1,
       1,     3,     3,     1
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
  "\"`#`\"", "LET", "VAR", "EXTERN", "\"continue\"", "\"break\"", "RETURN",
  "FOR", "VOID_", "AUTO", "CONST_", "USING", "THROW", "DELETE_", "SIZEOF",
  "LOWEST", "INCLUDE", "INLINE", "\"`fn`\"", "ENUM", "TYPE", "EXTENDS",
  "TYPENAME", "\"weak modifier\"", "\"const modifier\"", "WHILE", "IF",
  "\"true\"", "\"false\"", "$accept", "begin", "statement_any",
  "statement", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction-statement-any",
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
       0,   322,   322,   326,   328,   330,   350,   352,   354,   356,
     360,   362,   364,   383,   385,   390,   392,   397,   398,   403,
     405,   407,   415,   417,   419,   421,   423,   428,   430,   432,
     437,   446,   448,   450,   456,   458,   463,   465,   471,   478,
     480,   484,   487,   492,   518,   519,   523,   525,   530,   532,
     537,   539,   541,   546,   548,   555,   556,   562,   564,   566,
     568,   570,   575,   581,   583,   585,   587,   589,   591,   593,
     595,   597,   599,   601,   604,   606,   609,   614,   616,   640,
     642,   644,   646,   651,   652,   656,   658,   683,   685,   687,
     689,   691,   693,   695,   697,   704,   721,   730,   781,   783,
     787,   789,   797,   799
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
      95
    };
    // Last valid token kind.
    const int code_max = 350;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3390 "bang.tab.cpp"

#line 1128 "bang.y"

