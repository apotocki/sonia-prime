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

      case symbol_kind::S_123_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
      case symbol_kind::S_111_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_field_decl: // field-decl
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
      case symbol_kind::S_98_let_decl_start: // let-decl-start
      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_122_parameter_list: // parameter-list
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

      case symbol_kind::S_109_case_decl: // case-decl
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
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
      case symbol_kind::S_108_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_119_syntax_expression: // syntax-expression
      case symbol_kind::S_124_compound_expression: // compound-expression
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

      case symbol_kind::S_123_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
      case symbol_kind::S_111_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
      case symbol_kind::S_98_let_decl_start: // let-decl-start
      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_122_parameter_list: // parameter-list
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

      case symbol_kind::S_109_case_decl: // case-decl
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
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
      case symbol_kind::S_108_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_119_syntax_expression: // syntax-expression
      case symbol_kind::S_124_compound_expression: // compound-expression
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

      case symbol_kind::S_123_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
      case symbol_kind::S_111_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_field_decl: // field-decl
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.YY_MOVE_OR_COPY< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
        value.YY_MOVE_OR_COPY< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
      case symbol_kind::S_98_let_decl_start: // let-decl-start
      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_122_parameter_list: // parameter-list
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

      case symbol_kind::S_109_case_decl: // case-decl
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
      case symbol_kind::S_FN: // "`fn`"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.YY_MOVE_OR_COPY< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
      case symbol_kind::S_108_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_119_syntax_expression: // syntax-expression
      case symbol_kind::S_124_compound_expression: // compound-expression
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

      case symbol_kind::S_123_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
      case symbol_kind::S_111_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
      case symbol_kind::S_98_let_decl_start: // let-decl-start
      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_122_parameter_list: // parameter-list
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

      case symbol_kind::S_109_case_decl: // case-decl
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
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
      case symbol_kind::S_108_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_119_syntax_expression: // syntax-expression
      case symbol_kind::S_124_compound_expression: // compound-expression
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

      case symbol_kind::S_123_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
      case symbol_kind::S_111_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_112_field_decl: // field-decl
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_97_let_decl: // let-decl
      case symbol_kind::S_98_let_decl_start: // let-decl-start
      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_122_parameter_list: // parameter-list
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

      case symbol_kind::S_109_case_decl: // case-decl
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
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
      case symbol_kind::S_108_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_119_syntax_expression: // syntax-expression
      case symbol_kind::S_124_compound_expression: // compound-expression
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

      case symbol_kind::S_123_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
      case symbol_kind::S_111_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_112_field_decl: // field-decl
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.move< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_97_let_decl: // let-decl
      case symbol_kind::S_98_let_decl_start: // let-decl-start
      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_122_parameter_list: // parameter-list
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

      case symbol_kind::S_109_case_decl: // case-decl
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
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
      case symbol_kind::S_108_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_119_syntax_expression: // syntax-expression
      case symbol_kind::S_124_compound_expression: // compound-expression
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
#line 316 "bang.y"
                 { }
#line 1273 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 316 "bang.y"
                 { }
#line 1279 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 316 "bang.y"
                 { }
#line 1285 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 316 "bang.y"
                 { }
#line 1291 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 316 "bang.y"
                 { }
#line 1297 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 316 "bang.y"
                 { }
#line 1303 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 316 "bang.y"
                 { }
#line 1309 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 316 "bang.y"
                 { }
#line 1315 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 316 "bang.y"
                 { }
#line 1321 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 316 "bang.y"
                 { }
#line 1327 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 316 "bang.y"
                 { }
#line 1333 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 316 "bang.y"
                 { }
#line 1339 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 316 "bang.y"
                 { }
#line 1345 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 316 "bang.y"
                 { }
#line 1351 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 316 "bang.y"
                 { }
#line 1357 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 316 "bang.y"
                 { }
#line 1363 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 316 "bang.y"
                 { }
#line 1369 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 316 "bang.y"
                 { }
#line 1375 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 316 "bang.y"
                 { }
#line 1381 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 316 "bang.y"
                 { }
#line 1387 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 316 "bang.y"
                 { }
#line 1393 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 316 "bang.y"
                 { }
#line 1399 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 316 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 316 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
#line 316 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_97_let_decl: // let-decl
#line 316 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_98_let_decl_start: // let-decl-start
#line 316 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 316 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 316 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
#line 316 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 316 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 316 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
#line 316 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
#line 316 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
#line 316 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
#line 316 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_case_list: // case-list
#line 316 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_case_decl: // case-decl
#line 316 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
#line 316 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_field_list: // field-list
#line 316 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_field_decl: // field-decl
#line 316 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
#line 316 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
#line 316 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 316 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
#line 316 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
#line 316 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 316 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_syntax_expression: // syntax-expression
#line 316 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
#line 316 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
#line 316 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_parameter_list: // parameter-list
#line 316 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_type_expr: // type-expr
#line 316 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_compound_expression: // compound-expression
#line 316 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
#line 316 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
#line 316 "bang.y"
                 { }
#line 1597 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
#line 316 "bang.y"
                 { }
#line 1603 "bang.tab.cpp"
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

      case symbol_kind::S_123_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_106_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_110_field_list_opt: // field-list-opt
      case symbol_kind::S_111_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_112_field_decl: // field-decl
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_104_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_105_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        yylhs.value.emplace< generic_declaration_t > ();
        break;

      case symbol_kind::S_101_opt_infunction_decl: // opt-infunction-decl
        yylhs.value.emplace< infunction_declaration_t > ();
        break;

      case symbol_kind::S_97_let_decl: // let-decl
      case symbol_kind::S_98_let_decl_start: // let-decl-start
      case symbol_kind::S_99_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement_decl_t > ();
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_114_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_115_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_116_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_113_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_120_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_121_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_122_parameter_list: // parameter-list
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

      case symbol_kind::S_109_case_decl: // case-decl
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
      case symbol_kind::S_FN: // "`fn`"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        yylhs.value.emplace< std::vector<infunction_declaration_t> > ();
        break;

      case symbol_kind::S_107_case_list_opt: // case-list-opt
      case symbol_kind::S_108_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_117_concept_expression: // concept-expression
      case symbol_kind::S_118_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_119_syntax_expression: // syntax-expression
      case symbol_kind::S_124_compound_expression: // compound-expression
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
  case 2: // begin: declaration_any "end of file"
#line 321 "bang.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 1998 "bang.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 326 "bang.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 2004 "bang.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any "`;`"
#line 328 "bang.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 2010 "bang.tab.cpp"
    break;

  case 5: // declaration_any: declaration_any generic-decl
#line 330 "bang.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < generic_declaration_t > ()));
        }
#line 2019 "bang.tab.cpp"
    break;

  case 6: // generic-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 350 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = extern_var{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; }
#line 2025 "bang.tab.cpp"
    break;

  case 7: // generic-decl: EXTERN "`fn`" fn-decl "`;`"
#line 352 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[1].value.as < fn_pure_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2031 "bang.tab.cpp"
    break;

  case 8: // generic-decl: INCLUDE STRING
#line 354 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2037 "bang.tab.cpp"
    break;

  case 9: // generic-decl: enum-decl
#line 356 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2043 "bang.tab.cpp"
    break;

  case 10: // generic-decl: fn-start-decl fn-decl "`{`" infunction_declaration_any "`}`"
#line 360 "bang.y"
        { yystack_[3].value.as < fn_pure_t > ().kind = yystack_[4].value.as < fn_kind > (); yylhs.value.as < generic_declaration_t > () = fn_decl_t{ std::move(yystack_[3].value.as < fn_pure_t > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2049 "bang.tab.cpp"
    break;

  case 11: // generic-decl: fn-start-decl fn-decl "`=>`" syntax-expression "`;`"
#line 362 "bang.y"
        { yystack_[3].value.as < fn_pure_t > ().kind = yystack_[4].value.as < fn_kind > (); yylhs.value.as < generic_declaration_t > () = fn_decl_t{ std::move(yystack_[3].value.as < fn_pure_t > ()), { return_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) } } }; }
#line 2055 "bang.tab.cpp"
    break;

  case 12: // generic-decl: let-decl
#line 364 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2061 "bang.tab.cpp"
    break;

  case 13: // generic-decl: WHILE syntax-expression "`{`" infunction_declaration_any "`}`"
#line 366 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = while_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2067 "bang.tab.cpp"
    break;

  case 14: // generic-decl: compound-expression "`;`"
#line 368 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2073 "bang.tab.cpp"
    break;

  case 15: // let-decl: let-decl-start-with-opt-type "`;`"
#line 382 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[1].value.as < let_statement_decl_t > ()); }
#line 2079 "bang.tab.cpp"
    break;

  case 16: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression "`;`"
#line 384 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[3].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[1].value.as < syntax_expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2085 "bang.tab.cpp"
    break;

  case 17: // let-decl-start: LET identifier
#line 389 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2091 "bang.tab.cpp"
    break;

  case 18: // let-decl-start: LET "weak modifier" identifier
#line 391 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2097 "bang.tab.cpp"
    break;

  case 19: // let-decl-start-with-opt-type: let-decl-start
#line 395 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 2103 "bang.tab.cpp"
    break;

  case 20: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 397 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 2109 "bang.tab.cpp"
    break;

  case 21: // infunction_declaration_any: %empty
#line 402 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 2115 "bang.tab.cpp"
    break;

  case 22: // infunction_declaration_any: infunction_declaration_any "`;`"
#line 404 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()); }
#line 2121 "bang.tab.cpp"
    break;

  case 23: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 406 "bang.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 2130 "bang.tab.cpp"
    break;

  case 24: // opt-infunction-decl: let-decl
#line 414 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2136 "bang.tab.cpp"
    break;

  case 25: // opt-infunction-decl: RETURN syntax-expression "`;`"
#line 416 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = return_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2142 "bang.tab.cpp"
    break;

  case 26: // opt-infunction-decl: compound-expression "`;`"
#line 418 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2148 "bang.tab.cpp"
    break;

  case 27: // identifier: IDENTIFIER
#line 423 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2154 "bang.tab.cpp"
    break;

  case 28: // qname: "`::`" identifier
#line 432 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2160 "bang.tab.cpp"
    break;

  case 29: // qname: identifier
#line 434 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2166 "bang.tab.cpp"
    break;

  case 30: // qname: qname "`::`" identifier
#line 436 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2172 "bang.tab.cpp"
    break;

  case 31: // fn-start-decl: "`fn`"
#line 442 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2178 "bang.tab.cpp"
    break;

  case 32: // fn-start-decl: INLINE "`fn`"
#line 444 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2184 "bang.tab.cpp"
    break;

  case 33: // fn-decl: qname "`(`" parameter-list-opt "`)`"
#line 449 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2190 "bang.tab.cpp"
    break;

  case 34: // fn-decl: qname "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 451 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2196 "bang.tab.cpp"
    break;

  case 35: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 457 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2204 "bang.tab.cpp"
    break;

  case 36: // case-list-opt: %empty
#line 463 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2210 "bang.tab.cpp"
    break;

  case 37: // case-list-opt: case-list
#line 465 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2216 "bang.tab.cpp"
    break;

  case 38: // case-list: case-decl
#line 470 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2222 "bang.tab.cpp"
    break;

  case 39: // case-list: case-list "," case-decl
#line 473 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2228 "bang.tab.cpp"
    break;

  case 40: // case-decl: identifier
#line 478 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2234 "bang.tab.cpp"
    break;

  case 41: // field-list-opt: %empty
#line 503 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2240 "bang.tab.cpp"
    break;

  case 42: // field-list-opt: field-list
#line 504 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2246 "bang.tab.cpp"
    break;

  case 43: // field-list: field-decl
#line 509 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2252 "bang.tab.cpp"
    break;

  case 44: // field-list: field-list "," field-decl
#line 511 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2258 "bang.tab.cpp"
    break;

  case 45: // field-decl: IDENTIFIER "`:`" type-expr
#line 516 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2264 "bang.tab.cpp"
    break;

  case 46: // field-decl: type-expr
#line 518 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2270 "bang.tab.cpp"
    break;

  case 47: // parameter-name-decl: IDENTIFIER INTERNAL_IDENTIFIER "`:`"
#line 523 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())}; }
#line 2276 "bang.tab.cpp"
    break;

  case 48: // parameter-name-decl: IDENTIFIER "`:`"
#line 525 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), nullopt}; }
#line 2282 "bang.tab.cpp"
    break;

  case 49: // parameter-name-decl: INTERNAL_IDENTIFIER "`:`"
#line 527 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{nullopt, ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ()))}; }
#line 2288 "bang.tab.cpp"
    break;

  case 50: // parameter-constraint-modifier: TYPENAME
#line 532 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2294 "bang.tab.cpp"
    break;

  case 51: // parameter-constraint-modifier: "const modifier"
#line 534 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2300 "bang.tab.cpp"
    break;

  case 52: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 540 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2306 "bang.tab.cpp"
    break;

  case 53: // parameter-constraint-modifier-opt: %empty
#line 542 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2312 "bang.tab.cpp"
    break;

  case 54: // parameter-constraint-set: syntax-expression-wo-ii
#line 548 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2318 "bang.tab.cpp"
    break;

  case 55: // parameter-constraint-set: INTERNAL_IDENTIFIER
#line 550 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {}, { ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) } }; }
#line 2324 "bang.tab.cpp"
    break;

  case 56: // parameter-constraint-set: concept-expression
#line 552 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2330 "bang.tab.cpp"
    break;

  case 57: // parameter-constraint-set: parameter-constraint-set INTERNAL_IDENTIFIER
#line 554 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().bindings.emplace_back(ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()))); }
#line 2336 "bang.tab.cpp"
    break;

  case 58: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 556 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2342 "bang.tab.cpp"
    break;

  case 59: // concept-expression: "`@`" qname
#line 561 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 2348 "bang.tab.cpp"
    break;

  case 60: // syntax-expression-wo-ii: "true"
#line 567 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2354 "bang.tab.cpp"
    break;

  case 61: // syntax-expression-wo-ii: "false"
#line 569 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2360 "bang.tab.cpp"
    break;

  case 62: // syntax-expression-wo-ii: INTEGER
#line 571 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 2366 "bang.tab.cpp"
    break;

  case 63: // syntax-expression-wo-ii: DECIMAL
#line 573 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2372 "bang.tab.cpp"
    break;

  case 64: // syntax-expression-wo-ii: STRING
#line 575 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2378 "bang.tab.cpp"
    break;

  case 65: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 577 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2384 "bang.tab.cpp"
    break;

  case 66: // syntax-expression-wo-ii: qname
#line 579 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2390 "bang.tab.cpp"
    break;

  case 67: // syntax-expression-wo-ii: "`(`" syntax-expression "`)`"
#line 581 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2396 "bang.tab.cpp"
    break;

  case 68: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 583 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2402 "bang.tab.cpp"
    break;

  case 69: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 585 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2408 "bang.tab.cpp"
    break;

  case 70: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 587 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2414 "bang.tab.cpp"
    break;

  case 71: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 590 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2420 "bang.tab.cpp"
    break;

  case 72: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 592 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2426 "bang.tab.cpp"
    break;

  case 73: // syntax-expression-wo-ii: compound-expression
#line 595 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2432 "bang.tab.cpp"
    break;

  case 74: // syntax-expression: INTERNAL_IDENTIFIER
#line 600 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2438 "bang.tab.cpp"
    break;

  case 75: // syntax-expression: syntax-expression-wo-ii
#line 601 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2444 "bang.tab.cpp"
    break;

  case 76: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 626 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2450 "bang.tab.cpp"
    break;

  case 77: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 628 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2456 "bang.tab.cpp"
    break;

  case 78: // parameter-decl: parameter-constraint-set
#line 630 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2462 "bang.tab.cpp"
    break;

  case 79: // parameter-decl: "`_`"
#line 632 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, {} }; }
#line 2468 "bang.tab.cpp"
    break;

  case 80: // parameter-list-opt: %empty
#line 636 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2474 "bang.tab.cpp"
    break;

  case 81: // parameter-list-opt: parameter-list
#line 637 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2480 "bang.tab.cpp"
    break;

  case 82: // parameter-list: parameter-decl
#line 642 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2486 "bang.tab.cpp"
    break;

  case 83: // parameter-list: parameter-list "," parameter-decl
#line 644 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2492 "bang.tab.cpp"
    break;

  case 84: // type-expr: qname
#line 669 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()); }
#line 2498 "bang.tab.cpp"
    break;

  case 85: // type-expr: qname "`(`" opt-named-expr-list-any "`)`"
#line 671 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2504 "bang.tab.cpp"
    break;

  case 86: // type-expr: INTERNAL_IDENTIFIER
#line 673 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2510 "bang.tab.cpp"
    break;

  case 87: // type-expr: INTERNAL_IDENTIFIER "`(`" opt-named-expr-list-any "`)`"
#line 675 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2516 "bang.tab.cpp"
    break;

  case 88: // type-expr: "`[`" type-expr "`]`"
#line 677 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2522 "bang.tab.cpp"
    break;

  case 89: // type-expr: "`(`" field-list-opt "`)`"
#line 679 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2528 "bang.tab.cpp"
    break;

  case 90: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 681 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2534 "bang.tab.cpp"
    break;

  case 91: // type-expr: type-expr "`|`" type-expr
#line 683 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
        }
#line 2545 "bang.tab.cpp"
    break;

  case 92: // type-expr: type-expr "`->`" type-expr
#line 690 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2551 "bang.tab.cpp"
    break;

  case 93: // compound-expression: syntax-expression "`(`" opt-named-expr-list-any "`)`"
#line 707 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2557 "bang.tab.cpp"
    break;

  case 94: // compound-expression: syntax-expression "`=`" syntax-expression
#line 716 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2563 "bang.tab.cpp"
    break;

  case 95: // opt-named-expr-list-any: %empty
#line 767 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2569 "bang.tab.cpp"
    break;

  case 96: // opt-named-expr-list-any: opt-named-expr-list
#line 768 "bang.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2575 "bang.tab.cpp"
    break;

  case 97: // opt-named-expr-list: opt-named-expr
#line 773 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2581 "bang.tab.cpp"
    break;

  case 98: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 775 "bang.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2590 "bang.tab.cpp"
    break;

  case 99: // opt-named-expr: IDENTIFIER "`:`" syntax-expression
#line 783 "bang.y"
        { yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::tuple{ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())), std::move(yystack_[0].value.as < syntax_expression_t > ())} }; }
#line 2596 "bang.tab.cpp"
    break;

  case 100: // opt-named-expr: syntax-expression
#line 785 "bang.y"
        { yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2602 "bang.tab.cpp"
    break;


#line 2606 "bang.tab.cpp"

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


  const signed char parser::yypact_ninf_ = -98;

  const signed char parser::yytable_ninf_ = -56;

  const short
  parser::yypact_[] =
  {
     -98,    35,    59,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,     5,   180,   -98,   180,    -3,   -62,    52,    -4,   -98,
       7,   180,   -98,   -98,   -98,   -98,    34,    -8,   -98,    41,
       7,   -98,   -98,   226,    39,   -98,   234,   -98,    50,     5,
     -98,     5,     7,   -98,   -98,   -17,   250,   277,   180,   -98,
       5,    -5,    60,   180,   180,   180,   -98,   224,   180,   -98,
     -98,   -98,    67,    55,     5,   -98,    68,   296,   277,    30,
     131,   274,   -98,    91,   180,   -98,   319,   -32,   -32,    73,
     226,    72,    58,   -98,    50,   277,   -98,   -98,    76,    64,
     -98,   152,   224,    82,    84,    74,   -98,   131,    65,   224,
     277,   122,   277,   -98,    10,    18,     7,   -98,   -98,   -98,
     -57,   164,     1,   -98,    11,   -98,    87,    75,   280,   158,
     180,   -98,   224,   199,   -98,     5,   -98,   -98,   180,   -98,
     -98,    85,    99,   277,   -98,   296,   -98,   100,   -98,    97,
     -11,    94,   -98,   -98,    41,   -98,   164,   190,     1,   -98,
     -98,   132,    91,   -98,   -98,   226,   -98,   -98,   -98,   283,
     -98,   -98,   131,   -98,   -98,   -98,   -98,     1,   180,   -98,
     -98,   226
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    64,    27,    74,    65,    62,
      63,     0,     0,     4,     0,     0,     0,     0,     0,    31,
       0,     0,    60,    61,     5,    12,    19,     0,    29,    66,
       0,     9,    75,     0,    73,    28,     0,    73,    68,     0,
      17,     0,     0,     8,    32,     0,     0,     0,     0,    15,
       0,     0,     0,     0,     0,     0,    69,    95,     0,    14,
      67,    18,     0,     0,    36,    21,    86,    41,     0,    84,
      20,     0,    30,    80,     0,    21,    94,    71,    72,    27,
     100,     0,    96,    97,    70,     0,     7,    40,     0,    37,
      38,     0,    95,    27,     0,    42,    43,    46,     0,    95,
       0,     0,     0,    16,    27,    74,     0,    79,    50,    51,
      53,     0,    78,    56,    75,    82,     0,    81,     0,     0,
       0,    93,     0,     0,    35,     0,    13,    22,     0,    24,
      23,    73,     0,     0,    89,     0,    88,     0,    92,     0,
      91,     0,    48,    49,    59,    52,     0,    74,    77,    57,
      58,    33,     0,    11,    10,    99,    98,     6,    39,     0,
      26,    87,    45,    44,    85,    90,    47,    76,     0,    83,
      25,    34
  };

  const short
  parser::yypgoto_[] =
  {
     -98,   -98,   -98,   -98,   151,   -98,   -98,    79,   -98,     8,
     -14,   -98,   113,   -98,   -98,   -98,    51,   -98,   -98,    40,
     -98,    71,   -98,   -91,   -97,   -63,   -12,    32,   -98,   -98,
      -7,    -1,   -65,   -98,    70
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    24,   129,    26,    27,    91,   130,    28,
      29,    30,    52,    31,    88,    89,    90,    94,    95,    96,
     110,   111,   146,   112,   113,    32,    33,   115,   116,   117,
      97,    37,    81,    82,    83
  };

  const short
  parser::yytable_[] =
  {
      36,    34,    38,     6,    48,   100,    45,    41,   149,    46,
     114,     6,    57,     6,   106,   150,    51,   141,   -54,    35,
     148,    42,    58,    40,   -54,   -55,    50,   132,    51,    64,
     108,   -55,   109,    69,   137,     3,    71,   101,    50,    73,
      70,    76,    77,    78,    49,    80,    84,    61,   114,    62,
      11,   150,   142,    69,    69,   167,   -54,    43,    72,     4,
     143,    98,   118,   -55,     5,     6,     7,     8,     9,    10,
     150,    69,    87,    50,    99,   -54,    47,    74,   123,    44,
      80,   100,   -55,   114,    50,    39,    69,    80,    69,   114,
     131,    59,   144,   138,    57,   140,     5,   104,   105,     8,
       9,    10,    11,    12,   106,   107,    75,    86,   155,    85,
      80,    13,    92,   101,   136,   120,   159,   121,   131,    69,
      14,    69,   122,   124,   133,   102,   162,    15,   125,   134,
      16,   139,   151,    87,    11,    12,   166,   160,   135,   152,
      17,    18,    19,    20,   161,   164,   165,   100,   168,    21,
      22,    23,    14,    25,   119,    63,   171,     5,     6,     7,
       8,     9,    10,     5,     6,     7,     8,     9,    10,     5,
       6,   147,     8,     9,    10,   163,   158,   106,   108,   101,
     109,   145,    22,    23,   169,     5,     6,     7,     8,     9,
      10,   102,   156,     0,     0,    11,    12,   -55,     0,   126,
       0,    11,    12,   -55,   127,   154,     0,    11,    12,     0,
     127,     0,     0,    14,     0,   100,     0,     0,     0,    14,
      15,     0,   128,    11,    12,    14,    15,     0,   128,     5,
      79,     7,     8,     9,    10,   -55,     0,     0,    53,     0,
       0,    14,     0,    22,    23,     0,    53,   101,     0,    22,
      23,   157,    54,    55,   -55,    22,    23,    56,     0,   102,
      54,    55,    53,     0,     0,    56,     0,    11,    12,     0,
      57,    22,    23,     0,     0,     0,    54,    55,    57,    60,
      58,    56,     0,     6,    66,    14,    53,     0,    58,     0,
       0,     0,    53,     0,    57,    53,    65,     0,     0,     0,
      54,    55,    93,    66,    58,    56,    54,    55,     0,    54,
      55,    56,     0,     0,    56,    22,    23,     0,    57,     0,
      11,    67,     0,     0,    57,    68,   103,    57,    58,     0,
       0,    53,   153,     0,    58,   170,     0,    58,     0,    11,
      67,     0,     0,     0,    68,    54,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58
  };

  const short
  parser::yycheck_[] =
  {
      12,     2,    14,     6,    12,    16,    20,    69,     7,    21,
      73,     6,    44,     6,    13,   112,    30,     7,     7,    11,
     111,    83,    54,    15,    13,     7,    43,    92,    42,    46,
      87,    13,    89,    47,    99,     0,    48,    48,    43,    44,
      47,    53,    54,    55,    52,    57,    58,    39,   111,    41,
      43,   148,    42,    67,    68,   146,    45,     5,    50,     0,
      42,    68,    74,    45,     5,     6,     7,     8,     9,    10,
     167,    85,    64,    43,    44,    64,    42,    17,    85,    83,
      92,    16,    64,   146,    43,    88,   100,    99,   102,   152,
      91,    52,   106,   100,    44,   102,     5,     6,     7,     8,
       9,    10,    43,    44,    13,    14,    46,    52,   120,    42,
     122,    52,    44,    48,    49,    42,   128,    45,   119,   133,
      61,   135,    64,    47,    42,    60,   133,    68,    64,    45,
      71,     9,    45,   125,    43,    44,    42,    52,    64,    64,
      81,    82,    83,    84,    45,    45,    49,    16,    16,    90,
      91,    92,    61,     2,    75,    42,   168,     5,     6,     7,
       8,     9,    10,     5,     6,     7,     8,     9,    10,     5,
       6,     7,     8,     9,    10,   135,   125,    13,    87,    48,
      89,   110,    91,    92,   152,     5,     6,     7,     8,     9,
      10,    60,   122,    -1,    -1,    43,    44,     7,    -1,    47,
      -1,    43,    44,    13,    52,    47,    -1,    43,    44,    -1,
      52,    -1,    -1,    61,    -1,    16,    -1,    -1,    -1,    61,
      68,    -1,    70,    43,    44,    61,    68,    -1,    70,     5,
       6,     7,     8,     9,    10,    45,    -1,    -1,    12,    -1,
      -1,    61,    -1,    91,    92,    -1,    12,    48,    -1,    91,
      92,    52,    26,    27,    64,    91,    92,    31,    -1,    60,
      26,    27,    12,    -1,    -1,    31,    -1,    43,    44,    -1,
      44,    91,    92,    -1,    -1,    -1,    26,    27,    44,    45,
      54,    31,    -1,     6,     7,    61,    12,    -1,    54,    -1,
      -1,    -1,    12,    -1,    44,    12,    46,    -1,    -1,    -1,
      26,    27,     6,     7,    54,    31,    26,    27,    -1,    26,
      27,    31,    -1,    -1,    31,    91,    92,    -1,    44,    -1,
      43,    44,    -1,    -1,    44,    48,    52,    44,    54,    -1,
      -1,    12,    52,    -1,    54,    52,    -1,    54,    -1,    43,
      44,    -1,    -1,    -1,    48,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54
  };

  const signed char
  parser::yystos_[] =
  {
       0,    94,    95,     0,     0,     5,     6,     7,     8,     9,
      10,    43,    44,    52,    61,    68,    71,    81,    82,    83,
      84,    90,    91,    92,    96,    97,    98,    99,   102,   103,
     104,   106,   118,   119,   124,   102,   119,   124,   119,    88,
     102,    69,    83,     5,    83,   103,   119,    42,    12,    52,
      43,   103,   105,    12,    26,    27,    31,    44,    54,    52,
      45,   102,   102,   105,    46,    46,     7,    44,    48,   103,
     123,   119,   102,    44,    17,    46,   119,   119,   119,     6,
     119,   125,   126,   127,   119,    42,    52,   102,   107,   108,
     109,   100,    44,     6,   110,   111,   112,   123,   123,    44,
      16,    48,    60,    52,     6,     7,    13,    14,    87,    89,
     113,   114,   116,   117,   118,   120,   121,   122,   119,   100,
      42,    45,    64,   123,    47,    64,    47,    52,    70,    97,
     101,   124,   125,    42,    45,    64,    49,   125,   123,     9,
     123,     7,    42,    42,   103,   114,   115,     7,   116,     7,
     117,    45,    64,    52,    47,   119,   127,    52,   109,   119,
      52,    45,   123,   112,    45,    49,    42,   116,    16,   120,
      52,   119
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    93,    94,    95,    95,    95,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    97,    97,    98,    98,    99,
      99,   100,   100,   100,   101,   101,   101,   102,   103,   103,
     103,   104,   104,   105,   105,   106,   107,   107,   108,   108,
     109,   110,   110,   111,   111,   112,   112,   113,   113,   113,
     114,   114,   115,   115,   116,   116,   116,   116,   116,   117,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   119,   119,   120,   120,   120,   120,
     121,   121,   122,   122,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   124,   124,   125,   125,   126,   126,   127,
     127
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     6,     4,     2,     1,
       5,     5,     1,     5,     2,     2,     4,     2,     3,     1,
       3,     0,     2,     2,     1,     3,     2,     1,     2,     1,
       3,     1,     2,     4,     6,     5,     0,     1,     1,     3,
       1,     0,     1,     1,     3,     3,     1,     3,     2,     2,
       1,     1,     1,     0,     1,     1,     1,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     2,
       3,     3,     3,     1,     1,     1,     3,     2,     1,     1,
       0,     1,     1,     3,     1,     4,     1,     4,     3,     3,
       4,     3,     3,     4,     3,     0,     1,     1,     3,     3,
       1
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
  "\"`#`\"", "LET", "VAR", "RETURN", "EXTERN", "FOR", "VOID_", "AUTO",
  "CONST_", "USING", "THROW", "DELETE_", "SIZEOF", "LOWEST", "INCLUDE",
  "INLINE", "\"`fn`\"", "ENUM", "TYPE", "EXTENDS", "TYPENAME",
  "\"weak modifier\"", "\"const modifier\"", "WHILE", "\"true\"",
  "\"false\"", "$accept", "begin", "declaration_any", "generic-decl",
  "let-decl", "let-decl-start", "let-decl-start-with-opt-type",
  "infunction_declaration_any", "opt-infunction-decl", "identifier",
  "qname", "fn-start-decl", "fn-decl", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "field-list-opt", "field-list", "field-decl",
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
       0,   321,   321,   325,   327,   329,   349,   351,   353,   355,
     359,   361,   363,   365,   367,   381,   383,   388,   390,   395,
     396,   401,   403,   405,   413,   415,   417,   422,   431,   433,
     435,   441,   443,   448,   450,   456,   463,   465,   469,   472,
     477,   503,   504,   508,   510,   515,   517,   522,   524,   526,
     531,   533,   540,   541,   547,   549,   551,   553,   555,   560,
     566,   568,   570,   572,   574,   576,   578,   580,   582,   584,
     586,   589,   591,   594,   599,   601,   625,   627,   629,   631,
     636,   637,   641,   643,   668,   670,   672,   674,   676,   678,
     680,   682,   689,   706,   715,   766,   768,   772,   774,   782,
     784
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
      85,    86,    87,    88,    89,    90,    91,    92
    };
    // Last valid token kind.
    const int code_max = 347;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3306 "bang.tab.cpp"

#line 1120 "bang.y"

