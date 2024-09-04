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
      case symbol_kind::S_121_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
      case symbol_kind::S_109_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_field_decl: // field-decl
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_let_decl: // let-decl
      case symbol_kind::S_97_let_decl_start: // let-decl-start
      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_120_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
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

      case symbol_kind::S_107_case_decl: // case-decl
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_117_syntax_expression: // syntax-expression
      case symbol_kind::S_122_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
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
      case symbol_kind::S_121_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
      case symbol_kind::S_109_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_96_let_decl: // let-decl
      case symbol_kind::S_97_let_decl_start: // let-decl-start
      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_120_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
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

      case symbol_kind::S_107_case_decl: // case-decl
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_117_syntax_expression: // syntax-expression
      case symbol_kind::S_122_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
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
      case symbol_kind::S_121_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
      case symbol_kind::S_109_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_field_decl: // field-decl
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
        value.YY_MOVE_OR_COPY< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
        value.YY_MOVE_OR_COPY< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_let_decl: // let-decl
      case symbol_kind::S_97_let_decl_start: // let-decl-start
      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_120_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_DECIMAL: // DECIMAL
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
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

      case symbol_kind::S_107_case_decl: // case-decl
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.YY_MOVE_OR_COPY< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_117_syntax_expression: // syntax-expression
      case symbol_kind::S_122_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
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
      case symbol_kind::S_121_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
      case symbol_kind::S_109_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_let_decl: // let-decl
      case symbol_kind::S_97_let_decl_start: // let-decl-start
      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_120_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
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

      case symbol_kind::S_107_case_decl: // case-decl
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_117_syntax_expression: // syntax-expression
      case symbol_kind::S_122_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
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
      case symbol_kind::S_121_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
      case symbol_kind::S_109_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_110_field_decl: // field-decl
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_96_let_decl: // let-decl
      case symbol_kind::S_97_let_decl_start: // let-decl-start
      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_120_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
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

      case symbol_kind::S_107_case_decl: // case-decl
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_117_syntax_expression: // syntax-expression
      case symbol_kind::S_122_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
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
      case symbol_kind::S_121_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
      case symbol_kind::S_109_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_110_field_decl: // field-decl
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
        value.move< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_96_let_decl: // let-decl
      case symbol_kind::S_97_let_decl_start: // let-decl-start
      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_120_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
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

      case symbol_kind::S_107_case_decl: // case-decl
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_117_syntax_expression: // syntax-expression
      case symbol_kind::S_122_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
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
#line 314 "bang.y"
                 { }
#line 1243 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 314 "bang.y"
                 { }
#line 1249 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 314 "bang.y"
                 { }
#line 1255 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 314 "bang.y"
                 { }
#line 1261 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 314 "bang.y"
                 { }
#line 1267 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 314 "bang.y"
                 { }
#line 1273 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 314 "bang.y"
                 { }
#line 1279 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 314 "bang.y"
                 { }
#line 1285 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 314 "bang.y"
                 { }
#line 1291 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 314 "bang.y"
                 { }
#line 1297 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 314 "bang.y"
                 { }
#line 1303 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 314 "bang.y"
                 { }
#line 1309 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 314 "bang.y"
                 { }
#line 1315 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 314 "bang.y"
                 { }
#line 1321 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 314 "bang.y"
                 { }
#line 1327 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 314 "bang.y"
                 { }
#line 1333 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 314 "bang.y"
                 { }
#line 1339 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 314 "bang.y"
                 { }
#line 1345 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 314 "bang.y"
                 { }
#line 1351 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 314 "bang.y"
                 { }
#line 1357 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 314 "bang.y"
                 { }
#line 1363 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 314 "bang.y"
                 { }
#line 1369 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 314 "bang.y"
                 { }
#line 1375 "bang.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 314 "bang.y"
                 { }
#line 1381 "bang.tab.cpp"
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
#line 314 "bang.y"
                 { }
#line 1387 "bang.tab.cpp"
        break;

      case symbol_kind::S_96_let_decl: // let-decl
#line 314 "bang.y"
                 { }
#line 1393 "bang.tab.cpp"
        break;

      case symbol_kind::S_97_let_decl_start: // let-decl-start
#line 314 "bang.y"
                 { }
#line 1399 "bang.tab.cpp"
        break;

      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 314 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 314 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
#line 314 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 314 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 314 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
#line 314 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
#line 314 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
#line 314 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_case_list: // case-list
#line 314 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_case_decl: // case-decl
#line 314 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
#line 314 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_field_list: // field-list
#line 314 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_field_decl: // field-decl
#line 314 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
#line 314 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
#line 314 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 314 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
#line 314 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
#line 314 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 314 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_syntax_expression: // syntax-expression
#line 314 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
#line 314 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
#line 314 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_parameter_list: // parameter-list
#line 314 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_type_expr: // type-expr
#line 314 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_compound_expression: // compound-expression
#line 314 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 314 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
#line 314 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
#line 314 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
#line 314 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
#line 314 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
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
      case symbol_kind::S_121_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_124_expression_list_any: // expression-list-any
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_108_field_list_opt: // field-list-opt
      case symbol_kind::S_109_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_110_field_decl: // field-decl
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
        yylhs.value.emplace< generic_declaration_t > ();
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
        yylhs.value.emplace< infunction_declaration_t > ();
        break;

      case symbol_kind::S_96_let_decl: // let-decl
      case symbol_kind::S_97_let_decl_start: // let-decl-start
      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement_decl_t > ();
        break;

      case symbol_kind::S_125_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_126_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_127_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_112_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_113_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_114_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_111_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_118_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_119_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_120_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_DECIMAL: // DECIMAL
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
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

      case symbol_kind::S_107_case_decl: // case-decl
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        yylhs.value.emplace< std::vector<infunction_declaration_t> > ();
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_115_concept_expression: // concept-expression
      case symbol_kind::S_116_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_117_syntax_expression: // syntax-expression
      case symbol_kind::S_122_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
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
#line 319 "bang.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 1969 "bang.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 324 "bang.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 1975 "bang.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any "`;`"
#line 326 "bang.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 1981 "bang.tab.cpp"
    break;

  case 5: // declaration_any: declaration_any generic-decl
#line 328 "bang.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < generic_declaration_t > ()));
        }
#line 1990 "bang.tab.cpp"
    break;

  case 6: // generic-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 348 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = extern_var{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; }
#line 1996 "bang.tab.cpp"
    break;

  case 7: // generic-decl: EXTERN fn-decl "`;`"
#line 350 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[1].value.as < fn_pure_t > ()); }
#line 2002 "bang.tab.cpp"
    break;

  case 8: // generic-decl: INCLUDE STRING
#line 352 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2008 "bang.tab.cpp"
    break;

  case 9: // generic-decl: enum-decl
#line 354 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2014 "bang.tab.cpp"
    break;

  case 10: // generic-decl: fn-decl "`{`" infunction_declaration_any "`}`"
#line 358 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = fn_decl_t{std::move(yystack_[3].value.as < fn_pure_t > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2020 "bang.tab.cpp"
    break;

  case 11: // generic-decl: let-decl
#line 360 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2026 "bang.tab.cpp"
    break;

  case 12: // generic-decl: compound-expression "`;`"
#line 362 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2032 "bang.tab.cpp"
    break;

  case 13: // let-decl: let-decl-start-with-opt-type "`;`"
#line 376 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[1].value.as < let_statement_decl_t > ()); }
#line 2038 "bang.tab.cpp"
    break;

  case 14: // let-decl: let-decl-start-with-opt-type "`=`" expression "`;`"
#line 378 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[3].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[1].value.as < syntax_expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2044 "bang.tab.cpp"
    break;

  case 15: // let-decl-start: LET identifier
#line 383 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2050 "bang.tab.cpp"
    break;

  case 16: // let-decl-start: LET "weak modifier" identifier
#line 385 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2056 "bang.tab.cpp"
    break;

  case 17: // let-decl-start-with-opt-type: let-decl-start
#line 389 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 2062 "bang.tab.cpp"
    break;

  case 18: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 391 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 2068 "bang.tab.cpp"
    break;

  case 19: // infunction_declaration_any: %empty
#line 396 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 2074 "bang.tab.cpp"
    break;

  case 20: // infunction_declaration_any: infunction_declaration_any "`;`"
#line 398 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()); }
#line 2080 "bang.tab.cpp"
    break;

  case 21: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 400 "bang.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 2089 "bang.tab.cpp"
    break;

  case 22: // opt-infunction-decl: let-decl
#line 408 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2095 "bang.tab.cpp"
    break;

  case 23: // opt-infunction-decl: RETURN expression "`;`"
#line 410 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = return_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2101 "bang.tab.cpp"
    break;

  case 24: // opt-infunction-decl: compound-expression "`;`"
#line 412 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2107 "bang.tab.cpp"
    break;

  case 25: // identifier: IDENTIFIER
#line 417 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2113 "bang.tab.cpp"
    break;

  case 26: // qname: "`::`" identifier
#line 426 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2119 "bang.tab.cpp"
    break;

  case 27: // qname: identifier
#line 428 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2125 "bang.tab.cpp"
    break;

  case 28: // qname: qname "`::`" identifier
#line 430 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2131 "bang.tab.cpp"
    break;

  case 29: // fn-decl: "`fn`" qname "`(`" parameter-list-opt "`)`"
#line 436 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > (), yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2137 "bang.tab.cpp"
    break;

  case 30: // fn-decl: "`fn`" qname "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 438 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[6].value.as < sonia::lang::lex::resource_location > (), yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2143 "bang.tab.cpp"
    break;

  case 31: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 444 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2151 "bang.tab.cpp"
    break;

  case 32: // case-list-opt: %empty
#line 450 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2157 "bang.tab.cpp"
    break;

  case 33: // case-list-opt: case-list
#line 452 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2163 "bang.tab.cpp"
    break;

  case 34: // case-list: case-decl
#line 457 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2169 "bang.tab.cpp"
    break;

  case 35: // case-list: case-list "," case-decl
#line 460 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2175 "bang.tab.cpp"
    break;

  case 36: // case-decl: identifier
#line 465 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2181 "bang.tab.cpp"
    break;

  case 37: // field-list-opt: %empty
#line 490 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2187 "bang.tab.cpp"
    break;

  case 38: // field-list-opt: field-list
#line 491 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2193 "bang.tab.cpp"
    break;

  case 39: // field-list: field-decl
#line 496 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2199 "bang.tab.cpp"
    break;

  case 40: // field-list: field-list "," field-decl
#line 498 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2205 "bang.tab.cpp"
    break;

  case 41: // field-decl: IDENTIFIER "`:`" type-expr
#line 503 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ ctx.make_identifier(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2211 "bang.tab.cpp"
    break;

  case 42: // field-decl: type-expr
#line 505 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2217 "bang.tab.cpp"
    break;

  case 43: // parameter-name-decl: IDENTIFIER INTERNAL_IDENTIFIER "`:`"
#line 510 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ()), ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())}; }
#line 2223 "bang.tab.cpp"
    break;

  case 44: // parameter-name-decl: IDENTIFIER "`:`"
#line 512 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ()), nullopt}; }
#line 2229 "bang.tab.cpp"
    break;

  case 45: // parameter-name-decl: INTERNAL_IDENTIFIER "`:`"
#line 514 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{nullopt, ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())}; }
#line 2235 "bang.tab.cpp"
    break;

  case 46: // parameter-constraint-modifier: TYPENAME
#line 519 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2241 "bang.tab.cpp"
    break;

  case 47: // parameter-constraint-modifier: "const modifier"
#line 521 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2247 "bang.tab.cpp"
    break;

  case 48: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 527 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2253 "bang.tab.cpp"
    break;

  case 49: // parameter-constraint-modifier-opt: %empty
#line 529 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2259 "bang.tab.cpp"
    break;

  case 50: // parameter-constraint-set: syntax-expression-wo-ii
#line 535 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2265 "bang.tab.cpp"
    break;

  case 51: // parameter-constraint-set: INTERNAL_IDENTIFIER
#line 537 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {}, { ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) } }; }
#line 2271 "bang.tab.cpp"
    break;

  case 52: // parameter-constraint-set: concept-expression
#line 539 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2277 "bang.tab.cpp"
    break;

  case 53: // parameter-constraint-set: parameter-constraint-set INTERNAL_IDENTIFIER
#line 541 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().bindings.emplace_back(ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()))); }
#line 2283 "bang.tab.cpp"
    break;

  case 54: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 543 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2289 "bang.tab.cpp"
    break;

  case 55: // concept-expression: "`@`" qname
#line 548 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2295 "bang.tab.cpp"
    break;

  case 56: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 554 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2301 "bang.tab.cpp"
    break;

  case 57: // syntax-expression-wo-ii: qname
#line 556 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2307 "bang.tab.cpp"
    break;

  case 58: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 558 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2313 "bang.tab.cpp"
    break;

  case 59: // syntax-expression: INTERNAL_IDENTIFIER
#line 563 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true}; }
#line 2319 "bang.tab.cpp"
    break;

  case 60: // syntax-expression: syntax-expression-wo-ii
#line 564 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2325 "bang.tab.cpp"
    break;

  case 61: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 589 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2331 "bang.tab.cpp"
    break;

  case 62: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 591 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2337 "bang.tab.cpp"
    break;

  case 63: // parameter-decl: parameter-constraint-set
#line 593 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ {}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2343 "bang.tab.cpp"
    break;

  case 64: // parameter-list-opt: %empty
#line 597 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2349 "bang.tab.cpp"
    break;

  case 65: // parameter-list-opt: parameter-list
#line 598 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2355 "bang.tab.cpp"
    break;

  case 66: // parameter-list: parameter-decl
#line 603 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2361 "bang.tab.cpp"
    break;

  case 67: // parameter-list: parameter-list "," parameter-decl
#line 605 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2367 "bang.tab.cpp"
    break;

  case 68: // type-expr: qname
#line 630 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()); }
#line 2373 "bang.tab.cpp"
    break;

  case 69: // type-expr: qname "`(`" opt-named-expr-list-any "`)`"
#line 632 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2379 "bang.tab.cpp"
    break;

  case 70: // type-expr: INTERNAL_IDENTIFIER
#line 634 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2385 "bang.tab.cpp"
    break;

  case 71: // type-expr: INTERNAL_IDENTIFIER "`(`" opt-named-expr-list-any "`)`"
#line 636 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[1].value.as < named_expression_term_list_t > ()); }
#line 2391 "bang.tab.cpp"
    break;

  case 72: // type-expr: "`[`" type-expr "`]`"
#line 638 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2397 "bang.tab.cpp"
    break;

  case 73: // type-expr: "`(`" field-list-opt "`)`"
#line 640 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2403 "bang.tab.cpp"
    break;

  case 74: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 642 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_decimal > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2409 "bang.tab.cpp"
    break;

  case 75: // type-expr: type-expr "`|`" type-expr
#line 644 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
        }
#line 2420 "bang.tab.cpp"
    break;

  case 76: // type-expr: type-expr "`->`" type-expr
#line 651 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2426 "bang.tab.cpp"
    break;

  case 77: // compound-expression: expression "`(`" opt-named-expr-list-any "`)`"
#line 668 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2432 "bang.tab.cpp"
    break;

  case 78: // compound-expression: expression "`{`" opt-named-expr-list-any "`}`"
#line 670 "bang.y"
        { 
           yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) };
            //$$ = function_call_t{}; IGNORE($1, $2, $3);
        }
#line 2441 "bang.tab.cpp"
    break;

  case 79: // expression: "true"
#line 680 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_bool{true, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2447 "bang.tab.cpp"
    break;

  case 80: // expression: "false"
#line 682 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_bool{false, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2453 "bang.tab.cpp"
    break;

  case 81: // expression: INTEGER
#line 684 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2459 "bang.tab.cpp"
    break;

  case 82: // expression: DECIMAL
#line 686 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2465 "bang.tab.cpp"
    break;

  case 83: // expression: STRING
#line 688 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2471 "bang.tab.cpp"
    break;

  case 84: // expression: "`.`" identifier
#line 690 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = case_expression { std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2477 "bang.tab.cpp"
    break;

  case 85: // expression: qname
#line 692 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false}; }
#line 2483 "bang.tab.cpp"
    break;

  case 86: // expression: INTERNAL_IDENTIFIER
#line 694 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2489 "bang.tab.cpp"
    break;

  case 87: // expression: RESERVED_IDENTIFIER
#line 696 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2495 "bang.tab.cpp"
    break;

  case 88: // expression: "`(`" expression "`)`"
#line 698 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2501 "bang.tab.cpp"
    break;

  case 89: // expression: "`[`" expression-list-any "`]`"
#line 706 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = expression_vector_t{ {std::move(yystack_[1].value.as < expression_list_t > ())}, std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2507 "bang.tab.cpp"
    break;

  case 90: // expression: "`!`" expression
#line 708 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true /* prefixed */, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2513 "bang.tab.cpp"
    break;

  case 91: // expression: expression "`?`"
#line 710 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = not_empty_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2519 "bang.tab.cpp"
    break;

  case 92: // expression: expression "`.`" identifier
#line 712 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = member_expression_t { std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2525 "bang.tab.cpp"
    break;

  case 93: // expression: expression "`&&`" expression
#line 716 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2531 "bang.tab.cpp"
    break;

  case 94: // expression: expression "`||`" expression
#line 718 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2537 "bang.tab.cpp"
    break;

  case 95: // expression: expression "`+`" expression
#line 720 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2543 "bang.tab.cpp"
    break;

  case 96: // expression: expression "`..`" expression
#line 722 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2549 "bang.tab.cpp"
    break;

  case 97: // expression: expression "`==`" expression
#line 726 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2555 "bang.tab.cpp"
    break;

  case 98: // expression: expression "`!=`" expression
#line 728 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2561 "bang.tab.cpp"
    break;

  case 99: // expression: compound-expression
#line 731 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2567 "bang.tab.cpp"
    break;

  case 100: // expression-list-any: %empty
#line 742 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{}; }
#line 2573 "bang.tab.cpp"
    break;

  case 101: // expression-list-any: expression
#line 744 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2579 "bang.tab.cpp"
    break;

  case 102: // expression-list-any: expression-list-any "," expression
#line 746 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2585 "bang.tab.cpp"
    break;

  case 103: // opt-named-expr-list-any: %empty
#line 751 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2591 "bang.tab.cpp"
    break;

  case 104: // opt-named-expr-list-any: opt-named-expr-list
#line 752 "bang.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2597 "bang.tab.cpp"
    break;

  case 105: // opt-named-expr-list: opt-named-expr
#line 757 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2603 "bang.tab.cpp"
    break;

  case 106: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 759 "bang.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2612 "bang.tab.cpp"
    break;

  case 107: // opt-named-expr: qname "`:`" expression
#line 767 "bang.y"
        { yystack_[2].value.as < sonia::lang::bang::annotated_qname > ().value.set_absolute(); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::tuple{std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[0].value.as < syntax_expression_t > ())} }; }
#line 2618 "bang.tab.cpp"
    break;

  case 108: // opt-named-expr: expression
#line 769 "bang.y"
        { yylhs.value.as < named_expression_term_t > () = named_expression_term_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2624 "bang.tab.cpp"
    break;


#line 2628 "bang.tab.cpp"

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

  const signed char parser::yytable_ninf_ = -61;

  const short
  parser::yypact_[] =
  {
     -98,    23,   104,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,    95,   171,   171,   -98,    95,   171,    -3,   -52,    15,
      25,    25,   -98,   -98,   -98,   -98,    79,    12,   -98,    80,
      76,   -98,    73,   247,   -98,   -98,   172,   247,   -22,   -98,
      96,    95,   -98,    95,    75,   -98,    27,    55,    85,   171,
     -98,    95,   -98,   -98,   171,   171,   171,   171,   171,   171,
     171,    95,   171,   -98,   -98,   -98,   171,   -98,    88,   -98,
       4,    95,    90,   110,    85,    37,    14,   213,   -98,   160,
     280,   280,   291,   276,   183,    77,   247,    93,    81,   -98,
      92,   -98,    96,   247,    85,     6,    58,   -98,    25,   -98,
     -98,    80,   -64,    30,    20,   -98,   115,   120,   -98,   114,
      91,   -98,    94,    97,   -98,   171,   118,   119,    99,   -98,
      14,    28,   171,    85,   163,    85,   -98,   -98,   -98,   171,
     -98,   -98,   122,   171,   -98,   171,   -98,    39,   134,   -98,
     -98,    80,   -98,    30,   152,    20,   -98,   -98,   -98,   167,
       4,   -98,    95,   143,    85,   -98,   110,   -98,   144,   -98,
     141,    19,   242,   -98,   247,   -98,   -98,   -98,    20,   100,
     -98,   -98,   -98,    14,   -98,   -98,   -98,   -98,   -98,   -98,
     120
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    83,    25,    86,    87,    81,
      82,     0,     0,   100,     4,     0,     0,     0,     0,     0,
       0,     0,    79,    80,     5,    11,    17,     0,    27,    85,
       0,     9,    99,     0,    26,    99,     0,   101,     0,    84,
      90,     0,    15,     0,     0,     8,     0,     0,     0,     0,
      13,     0,    19,    12,     0,     0,     0,     0,     0,   103,
     103,     0,     0,    91,    88,    89,     0,    16,     0,     7,
      64,    32,    70,    37,     0,    68,    18,     0,    28,     0,
      97,    98,    93,    94,    96,    85,   108,     0,   104,   105,
       0,    92,    95,   102,     0,    25,    51,    56,     0,    46,
      47,    57,    49,     0,    63,    52,    50,     0,    66,     0,
      65,    36,     0,    33,    34,   103,    25,     0,    38,    39,
      42,     0,   103,     0,     0,     0,    14,    10,    20,     0,
      22,    21,    99,     0,    77,     0,    78,     0,     0,    44,
      45,    55,    48,     0,    51,    62,    53,    54,    58,    29,
       0,    31,     0,     0,     0,    73,     0,    72,     0,    76,
       0,    75,     0,    24,   107,   106,     6,    43,    61,     0,
      67,    35,    71,    41,    40,    69,    74,    23,    59,    60,
      30
  };

  const short
  parser::yypgoto_[] =
  {
     -98,   -98,   -98,   -98,   111,   -98,   -98,   -98,   -98,    -9,
     -20,   173,   -98,   -98,   -98,    40,   -98,   -98,    48,   -98,
     103,   -98,   -94,   -97,    24,    41,    46,   -98,   -98,   -29,
       3,     2,   -98,   -38,   -98,    74
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    24,    25,    26,    27,    79,   131,    28,
      29,    30,    31,   112,   113,   114,   117,   118,   119,   102,
     103,   143,   104,   105,   106,   107,   108,   109,   110,   120,
      35,    86,    38,    87,    88,    89
  };

  const short
  parser::yytable_[] =
  {
      46,    47,    34,     6,    33,    32,    39,   147,    42,   145,
      95,    96,    97,   138,    36,    37,    43,    98,    40,    76,
      45,    99,    90,     3,    49,   100,    65,   146,    75,    20,
     123,     6,    67,    98,    68,   123,     6,   144,    97,    85,
      85,    66,    78,    98,   123,   121,    11,   139,   147,   168,
     101,    77,    91,    75,    75,   123,    80,    81,    82,    83,
      84,   124,   111,    50,    92,   137,   124,    11,    93,    51,
      70,   147,    11,   125,    75,   124,   157,   153,   141,    51,
     122,    33,   132,   101,   158,    41,   124,   125,   -59,    99,
     166,     6,    72,   100,   159,    85,   161,    51,   125,   140,
      71,     6,    85,    75,     4,    75,     6,   178,    97,     5,
       6,     7,     8,     9,    10,    85,   116,    72,   133,    51,
      48,    52,    51,   101,    53,   173,    69,    11,    73,    94,
     101,   162,    74,   115,    75,   164,    75,   134,   136,    59,
     151,    60,    11,   111,   135,   -60,    11,    12,    61,   101,
     148,    13,    11,    73,   150,    14,    15,    74,   149,   154,
     152,    63,   156,   155,    16,     5,     6,     7,     8,     9,
      10,    17,   160,   163,    18,   167,     5,     6,     7,     8,
       9,    10,   -59,   169,    19,    20,    21,   172,   175,   176,
     130,    44,   171,   179,    22,    23,   170,    54,    55,    56,
      57,    58,    11,    12,   174,   142,   127,    13,     0,   165,
     180,   128,    15,    11,    12,    59,    64,    60,    13,     0,
      16,     0,     0,    15,    61,    62,    59,    17,    60,   129,
       0,    16,     0,     0,     0,    61,    62,    63,    54,    55,
      56,    57,    58,     0,     0,     0,     0,     0,    63,     0,
      22,    23,     0,     0,     0,     0,    59,     0,    60,     0,
       0,    22,    23,     0,   126,    61,    62,    54,    55,    56,
      57,    58,    54,    55,    56,    57,    58,     0,    63,     0,
       0,     0,     0,     0,     0,    59,     0,    60,     0,     0,
      59,     0,    60,   177,    61,    62,     0,     0,     0,    61,
      62,    54,    55,    56,     0,    58,     0,    63,     0,    58,
       0,     0,    63,     0,     0,     0,    54,    55,     0,    59,
      58,    60,     0,    59,     0,    60,     0,     0,    61,    62,
       0,     0,    61,    62,    59,     0,    60,     0,     0,     0,
       0,    63,     0,    61,    62,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63
  };

  const short
  parser::yycheck_[] =
  {
      20,    21,    11,     6,     2,     2,    15,   104,    17,   103,
       6,     7,     8,     7,    12,    13,    68,    13,    16,    48,
       5,    85,    60,     0,    12,    89,    48,     7,    48,    81,
      16,     6,    41,    13,    43,    16,     6,     7,     8,    59,
      60,    63,    51,    13,    16,    74,    42,    41,   145,   143,
      70,    49,    61,    73,    74,    16,    54,    55,    56,    57,
      58,    47,    71,    51,    62,    94,    47,    42,    66,    42,
      43,   168,    42,    59,    94,    47,    48,   115,    98,    42,
      43,    79,    79,   103,   122,    88,    47,    59,    30,    85,
      51,     6,     7,    89,   123,   115,   125,    42,    59,    41,
      45,     6,   122,   123,     0,   125,     6,     7,     8,     5,
       6,     7,     8,     9,    10,   135,     6,     7,    41,    42,
      41,    45,    42,   143,    51,   154,    51,    42,    43,    41,
     150,   129,    47,    43,   154,   133,   156,    44,    46,    43,
      46,    45,    42,   152,    63,    30,    42,    43,    52,   169,
      30,    47,    42,    43,    63,    51,    52,    47,    44,    41,
      63,    65,    63,    44,    60,     5,     6,     7,     8,     9,
      10,    67,     9,    51,    70,    41,     5,     6,     7,     8,
       9,    10,    30,    16,    80,    81,    82,    44,    44,    48,
      79,    18,   152,   169,    90,    91,   150,    25,    26,    27,
      28,    29,    42,    43,   156,   102,    46,    47,    -1,   135,
     169,    51,    52,    42,    43,    43,    44,    45,    47,    -1,
      60,    -1,    -1,    52,    52,    53,    43,    67,    45,    69,
      -1,    60,    -1,    -1,    -1,    52,    53,    65,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      90,    91,    -1,    -1,    -1,    -1,    43,    -1,    45,    -1,
      -1,    90,    91,    -1,    51,    52,    53,    25,    26,    27,
      28,    29,    25,    26,    27,    28,    29,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,
      43,    -1,    45,    51,    52,    53,    -1,    -1,    -1,    52,
      53,    25,    26,    27,    -1,    29,    -1,    65,    -1,    29,
      -1,    -1,    65,    -1,    -1,    -1,    25,    26,    -1,    43,
      29,    45,    -1,    43,    -1,    45,    -1,    -1,    52,    53,
      -1,    -1,    52,    53,    43,    -1,    45,    -1,    -1,    -1,
      -1,    65,    -1,    52,    53,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65
  };

  const signed char
  parser::yystos_[] =
  {
       0,    93,    94,     0,     0,     5,     6,     7,     8,     9,
      10,    42,    43,    47,    51,    52,    60,    67,    70,    80,
      81,    82,    90,    91,    95,    96,    97,    98,   101,   102,
     103,   104,   122,   123,   101,   122,   123,   123,   124,   101,
     123,    88,   101,    68,   103,     5,   102,   102,    41,    12,
      51,    42,    45,    51,    25,    26,    27,    28,    29,    43,
      45,    52,    53,    65,    44,    48,    63,   101,   101,    51,
      43,    45,     7,    43,    47,   102,   121,   123,   101,    99,
     123,   123,   123,   123,   123,   102,   123,   125,   126,   127,
     125,   101,   123,   123,    41,     6,     7,     8,    13,    85,
      89,   102,   111,   112,   114,   115,   116,   117,   118,   119,
     120,   101,   105,   106,   107,    43,     6,   108,   109,   110,
     121,   121,    43,    16,    47,    59,    51,    46,    51,    69,
      96,   100,   122,    41,    44,    63,    46,   121,     7,    41,
      41,   102,   112,   113,     7,   114,     7,   115,    30,    44,
      63,    46,    63,   125,    41,    44,    63,    48,   125,   121,
       9,   121,   123,    51,   123,   127,    51,    41,   114,    16,
     118,   107,    44,   121,   110,    44,    48,    51,     7,   116,
     117
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    92,    93,    94,    94,    94,    95,    95,    95,    95,
      95,    95,    95,    96,    96,    97,    97,    98,    98,    99,
      99,    99,   100,   100,   100,   101,   102,   102,   102,   103,
     103,   104,   105,   105,   106,   106,   107,   108,   108,   109,
     109,   110,   110,   111,   111,   111,   112,   112,   113,   113,
     114,   114,   114,   114,   114,   115,   116,   116,   116,   117,
     117,   118,   118,   118,   119,   119,   120,   120,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   122,   122,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     124,   124,   124,   125,   125,   126,   126,   127,   127
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     6,     3,     2,     1,
       4,     1,     2,     2,     4,     2,     3,     1,     3,     0,
       2,     2,     1,     3,     2,     1,     2,     1,     3,     5,
       7,     5,     0,     1,     1,     3,     1,     0,     1,     1,
       3,     3,     1,     3,     2,     2,     1,     1,     1,     0,
       1,     1,     1,     2,     2,     2,     1,     1,     2,     1,
       1,     3,     2,     1,     0,     1,     1,     3,     1,     4,
       1,     4,     3,     3,     4,     3,     3,     4,     4,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     3,     3,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     1,
       0,     1,     3,     0,     1,     1,     3,     3,     1
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
  "\"`@`\"", "\"`_`\"", "\"`->*`\"", "\"`->`\"", "\"`~>`\"", "\"`.*`\"",
  "\"`++`\"", "\"`--`\"", "\"`<<`\"", "\"`>>`\"", "\"`<=`\"", "\"`>=`\"",
  "\"`==`\"", "\"`!=`\"", "\"`&&`\"", "\"`||`\"", "\"`..`\"", "\"`...`\"",
  "\"`+=`\"", "\"`-=`\"", "\"`*=`\"", "\"`/=`\"", "\"`%=`\"", "\"`<<=`\"",
  "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"", "\"`^=`\"", "\"`:`\"", "\"`::`\"",
  "\"`(`\"", "\"`)`\"", "\"`{`\"", "\"`}`\"", "\"`[`\"", "\"`]`\"",
  "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"", "\"`+`\"", "\"`-`\"",
  "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"", "\"`|`\"", "\"`!`\"",
  "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET",
  "VAR", "RETURN", "EXTERN", "FOR", "VOID_", "AUTO", "CONST_", "USING",
  "THROW", "DELETE_", "SIZEOF", "LOWEST", "INCLUDE", "\"`fn`\"", "ENUM",
  "TYPE", "EXTENDS", "TYPENAME", "INT", "FLOAT", "\"weak modifier\"",
  "\"const modifier\"", "\"true\"", "\"false\"", "$accept", "begin",
  "declaration_any", "generic-decl", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction_declaration_any",
  "opt-infunction-decl", "identifier", "qname", "fn-decl", "enum-decl",
  "case-list-opt", "case-list", "case-decl", "field-list-opt",
  "field-list", "field-decl", "parameter-name-decl",
  "parameter-constraint-modifier", "parameter-constraint-modifier-opt",
  "parameter-constraint-set", "concept-expression",
  "syntax-expression-wo-ii", "syntax-expression", "parameter-decl",
  "parameter-list-opt", "parameter-list", "type-expr",
  "compound-expression", "expression", "expression-list-any",
  "opt-named-expr-list-any", "opt-named-expr-list", "opt-named-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   319,   319,   323,   325,   327,   347,   349,   351,   353,
     357,   359,   361,   375,   377,   382,   384,   389,   390,   395,
     397,   399,   407,   409,   411,   416,   425,   427,   429,   435,
     437,   443,   450,   452,   456,   459,   464,   490,   491,   495,
     497,   502,   504,   509,   511,   513,   518,   520,   527,   528,
     534,   536,   538,   540,   542,   547,   553,   555,   557,   562,
     564,   588,   590,   592,   597,   598,   602,   604,   629,   631,
     633,   635,   637,   639,   641,   643,   650,   667,   669,   679,
     681,   683,   685,   687,   689,   691,   693,   695,   697,   705,
     707,   709,   711,   715,   717,   719,   721,   725,   727,   730,
     741,   743,   745,   750,   752,   756,   758,   766,   768
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
      85,    86,    87,    88,    89,    90,    91
    };
    // Last valid token kind.
    const int code_max = 346;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3327 "bang.tab.cpp"

#line 1104 "bang.y"

