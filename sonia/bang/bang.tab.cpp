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
      case symbol_kind::S_116_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
      case symbol_kind::S_109_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
        value.copy< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_let_decl: // let-decl
      case symbol_kind::S_96_let_decl_start: // let-decl-start
      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_111_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
        value.copy< parameter_woa_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
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

      case symbol_kind::S_104_case_list_opt: // case-list-opt
      case symbol_kind::S_105_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_type_decl: // type-decl
        value.copy< type_decl > (YY_MOVE (that.value));
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
      case symbol_kind::S_116_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
      case symbol_kind::S_109_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_95_let_decl: // let-decl
      case symbol_kind::S_96_let_decl_start: // let-decl-start
      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_111_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
        value.move< parameter_woa_t > (YY_MOVE (s.value));
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
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
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

      case symbol_kind::S_104_case_list_opt: // case-list-opt
      case symbol_kind::S_105_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_type_decl: // type-decl
        value.move< type_decl > (YY_MOVE (s.value));
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
      case symbol_kind::S_116_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
      case symbol_kind::S_109_type_extension_list: // type-extension-list
        value.YY_MOVE_OR_COPY< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.YY_MOVE_OR_COPY< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.YY_MOVE_OR_COPY< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_let_decl: // let-decl
      case symbol_kind::S_96_let_decl_start: // let-decl-start
      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_111_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
        value.YY_MOVE_OR_COPY< parameter_woa_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
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

      case symbol_kind::S_104_case_list_opt: // case-list-opt
      case symbol_kind::S_105_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_type_decl: // type-decl
        value.YY_MOVE_OR_COPY< type_decl > (YY_MOVE (that.value));
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
      case symbol_kind::S_116_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
      case symbol_kind::S_109_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_let_decl: // let-decl
      case symbol_kind::S_96_let_decl_start: // let-decl-start
      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_111_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
        value.move< parameter_woa_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
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

      case symbol_kind::S_104_case_list_opt: // case-list-opt
      case symbol_kind::S_105_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_type_decl: // type-decl
        value.move< type_decl > (YY_MOVE (that.value));
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
      case symbol_kind::S_116_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (that.value);
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
      case symbol_kind::S_109_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (that.value);
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
        value.copy< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_95_let_decl: // let-decl
      case symbol_kind::S_96_let_decl_start: // let-decl-start
      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_111_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
        value.copy< parameter_woa_t > (that.value);
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
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
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

      case symbol_kind::S_104_case_list_opt: // case-list-opt
      case symbol_kind::S_105_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_107_type_decl: // type-decl
        value.copy< type_decl > (that.value);
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
      case symbol_kind::S_116_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (that.value);
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
      case symbol_kind::S_109_type_extension_list: // type-extension-list
        value.move< extension_list_t > (that.value);
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
        value.move< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.move< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_95_let_decl: // let-decl
      case symbol_kind::S_96_let_decl_start: // let-decl-start
      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_111_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
        value.move< parameter_woa_t > (that.value);
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
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.move< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
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

      case symbol_kind::S_104_case_list_opt: // case-list-opt
      case symbol_kind::S_105_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_107_type_decl: // type-decl
        value.move< type_decl > (that.value);
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
#line 285 "bang.y"
                 { }
#line 1171 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 285 "bang.y"
                 { }
#line 1177 "bang.tab.cpp"
        break;

      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
#line 285 "bang.y"
                 { }
#line 1183 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 285 "bang.y"
                 { }
#line 1189 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 285 "bang.y"
                 { }
#line 1195 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 285 "bang.y"
                 { }
#line 1201 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 285 "bang.y"
                 { }
#line 1207 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 285 "bang.y"
                 { }
#line 1213 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 285 "bang.y"
                 { }
#line 1219 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 285 "bang.y"
                 { }
#line 1225 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 285 "bang.y"
                 { }
#line 1231 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 285 "bang.y"
                 { }
#line 1237 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 285 "bang.y"
                 { }
#line 1243 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 285 "bang.y"
                 { }
#line 1249 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 285 "bang.y"
                 { }
#line 1255 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // FN
#line 285 "bang.y"
                 { }
#line 1261 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 285 "bang.y"
                 { }
#line 1267 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 285 "bang.y"
                 { }
#line 1273 "bang.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 285 "bang.y"
                 { }
#line 1279 "bang.tab.cpp"
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
#line 285 "bang.y"
                 { }
#line 1285 "bang.tab.cpp"
        break;

      case symbol_kind::S_95_let_decl: // let-decl
#line 285 "bang.y"
                 { }
#line 1291 "bang.tab.cpp"
        break;

      case symbol_kind::S_96_let_decl_start: // let-decl-start
#line 285 "bang.y"
                 { }
#line 1297 "bang.tab.cpp"
        break;

      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 285 "bang.y"
                 { }
#line 1303 "bang.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 285 "bang.y"
                 { }
#line 1309 "bang.tab.cpp"
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
#line 285 "bang.y"
                 { }
#line 1315 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 285 "bang.y"
                 { }
#line 1321 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 285 "bang.y"
                 { }
#line 1327 "bang.tab.cpp"
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
#line 285 "bang.y"
                 { }
#line 1333 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
#line 285 "bang.y"
                 { }
#line 1339 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_case_list_opt: // case-list-opt
#line 285 "bang.y"
                 { }
#line 1345 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_case_list: // case-list
#line 285 "bang.y"
                 { }
#line 1351 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_case_decl: // case-decl
#line 285 "bang.y"
                 { }
#line 1357 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_type_decl: // type-decl
#line 285 "bang.y"
                 { }
#line 1363 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
#line 285 "bang.y"
                 { }
#line 1369 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_type_extension_list: // type-extension-list
#line 285 "bang.y"
                 { }
#line 1375 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
#line 285 "bang.y"
                 { }
#line 1381 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_parameter_list: // parameter-list
#line 285 "bang.y"
                 { }
#line 1387 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
#line 285 "bang.y"
                 { }
#line 1393 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
#line 285 "bang.y"
                 { }
#line 1399 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
#line 285 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
#line 285 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_type_expr: // type-expr
#line 285 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
#line 285 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 285 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
#line 285 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
#line 285 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
#line 285 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
#line 285 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
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
      case symbol_kind::S_116_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< expression_t > ();
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
      case symbol_kind::S_109_type_extension_list: // type-extension-list
        yylhs.value.emplace< extension_list_t > ();
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
        yylhs.value.emplace< fn_decl_t > ();
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        yylhs.value.emplace< generic_declaration_t > ();
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        yylhs.value.emplace< infunction_declaration_t > ();
        break;

      case symbol_kind::S_95_let_decl: // let-decl
      case symbol_kind::S_96_let_decl_start: // let-decl-start
      case symbol_kind::S_97_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement_decl_t > ();
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_111_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
        yylhs.value.emplace< parameter_woa_t > ();
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
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::identifier > ();
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_BRACE: // "`{`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
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

      case symbol_kind::S_104_case_list_opt: // case-list-opt
      case symbol_kind::S_105_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::identifier> > ();
        break;

      case symbol_kind::S_107_type_decl: // type-decl
        yylhs.value.emplace< type_decl > ();
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
#line 290 "bang.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 1831 "bang.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 295 "bang.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 1837 "bang.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any "`;`"
#line 297 "bang.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 1843 "bang.tab.cpp"
    break;

  case 5: // declaration_any: declaration_any generic-decl
#line 299 "bang.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < generic_declaration_t > ()));
        }
#line 1852 "bang.tab.cpp"
    break;

  case 6: // generic-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 319 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = extern_var{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; }
#line 1858 "bang.tab.cpp"
    break;

  case 7: // generic-decl: EXTERN FN qname "`(`" parameter-woa-list-opt "`)`" "`;`"
#line 321 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = fn_pure_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()), bang_preliminary_tuple_t{} }; IGNORE(yystack_[5].value.as < sonia::lang::lex::resource_location > (), yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 1864 "bang.tab.cpp"
    break;

  case 8: // generic-decl: EXTERN FN qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`;`"
#line 323 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = fn_pure_decl{ std::move(yystack_[6].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > ()); }
#line 1870 "bang.tab.cpp"
    break;

  case 9: // generic-decl: INCLUDE STRING
#line 325 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 1876 "bang.tab.cpp"
    break;

  case 10: // generic-decl: enum-decl
#line 327 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 1882 "bang.tab.cpp"
    break;

  case 11: // generic-decl: type-decl
#line 329 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < type_decl > ()); }
#line 1888 "bang.tab.cpp"
    break;

  case 12: // generic-decl: fn-decl
#line 331 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < fn_decl_t > ()); }
#line 1894 "bang.tab.cpp"
    break;

  case 13: // generic-decl: let-decl
#line 333 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 1900 "bang.tab.cpp"
    break;

  case 14: // generic-decl: compound-expression "`;`"
#line 335 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1906 "bang.tab.cpp"
    break;

  case 15: // let-decl: let-decl-start-with-opt-type "`;`"
#line 349 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[1].value.as < let_statement_decl_t > ()); }
#line 1912 "bang.tab.cpp"
    break;

  case 16: // let-decl: let-decl-start-with-opt-type "`=`" expression "`;`"
#line 351 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[3].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[1].value.as < expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 1918 "bang.tab.cpp"
    break;

  case 17: // let-decl-start: LET identifier
#line 356 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 1924 "bang.tab.cpp"
    break;

  case 18: // let-decl-start: LET "weak modifier" identifier
#line 358 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 1930 "bang.tab.cpp"
    break;

  case 19: // let-decl-start-with-opt-type: let-decl-start
#line 362 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 1936 "bang.tab.cpp"
    break;

  case 20: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 364 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 1942 "bang.tab.cpp"
    break;

  case 21: // infunction_declaration_any: %empty
#line 369 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 1948 "bang.tab.cpp"
    break;

  case 22: // infunction_declaration_any: infunction_declaration_any "`;`"
#line 371 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()); }
#line 1954 "bang.tab.cpp"
    break;

  case 23: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 373 "bang.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 1963 "bang.tab.cpp"
    break;

  case 24: // opt-infunction-decl: let-decl
#line 381 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 1969 "bang.tab.cpp"
    break;

  case 25: // opt-infunction-decl: RETURN expression "`;`"
#line 383 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = return_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1975 "bang.tab.cpp"
    break;

  case 26: // opt-infunction-decl: compound-expression "`;`"
#line 385 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1981 "bang.tab.cpp"
    break;

  case 27: // identifier: IDENTIFIER
#line 390 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 1987 "bang.tab.cpp"
    break;

  case 28: // qname: DBLCOLON identifier
#line 399 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 1993 "bang.tab.cpp"
    break;

  case 29: // qname: identifier
#line 401 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 1999 "bang.tab.cpp"
    break;

  case 30: // qname: qname DBLCOLON identifier
#line 403 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2005 "bang.tab.cpp"
    break;

  case 31: // fn-decl: FN qname "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 409 "bang.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[6].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > (), yystack_[2].value.as < sonia::lang::lex::resource_location > ());
            }
#line 2013 "bang.tab.cpp"
    break;

  case 32: // fn-decl: FN qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 413 "bang.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[8].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < bang_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[9].value.as < sonia::lang::lex::resource_location > (), yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[2].value.as < sonia::lang::lex::resource_location > ());
            }
#line 2021 "bang.tab.cpp"
    break;

  case 33: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 420 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() + std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2029 "bang.tab.cpp"
    break;

  case 34: // case-list-opt: %empty
#line 426 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::identifier> > () = {}; }
#line 2035 "bang.tab.cpp"
    break;

  case 35: // case-list-opt: case-list
#line 428 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::identifier> > (); }
#line 2041 "bang.tab.cpp"
    break;

  case 36: // case-list: case-decl
#line 433 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::vector<sonia::lang::identifier>{std::move(yystack_[0].value.as < sonia::lang::identifier > ())}; }
#line 2047 "bang.tab.cpp"
    break;

  case 37: // case-list: case-list "," case-decl
#line 436 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::identifier > ())); }
#line 2053 "bang.tab.cpp"
    break;

  case 38: // case-decl: identifier
#line 441 "bang.y"
        { yylhs.value.as < sonia::lang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2059 "bang.tab.cpp"
    break;

  case 39: // type-decl: TYPE qname type-extension-any "`{`" parameter-woa-list-opt "`}`"
#line 446 "bang.y"
    {
        yylhs.value.as < type_decl > () = type_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() + std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[3].value.as < extension_list_t > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2067 "bang.tab.cpp"
    break;

  case 40: // type-extension-any: %empty
#line 452 "bang.y"
           { yylhs.value.as < extension_list_t > () = {}; }
#line 2073 "bang.tab.cpp"
    break;

  case 41: // type-extension-any: EXTENDS type-extension-list
#line 454 "bang.y"
                                { yylhs.value.as < extension_list_t > () = std::move(yystack_[0].value.as < extension_list_t > ()); }
#line 2079 "bang.tab.cpp"
    break;

  case 42: // type-extension-list: qname
#line 459 "bang.y"
        { yylhs.value.as < extension_list_t > () = extension_list_t{ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()))}; }
#line 2085 "bang.tab.cpp"
    break;

  case 43: // type-extension-list: type-extension-list "," qname
#line 462 "bang.y"
        { yylhs.value.as < extension_list_t > () = std::move(yystack_[2].value.as < extension_list_t > ()); yylhs.value.as < extension_list_t > ().emplace_back(ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()))); }
#line 2091 "bang.tab.cpp"
    break;

  case 44: // parameter-list-opt: %empty
#line 466 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 2097 "bang.tab.cpp"
    break;

  case 45: // parameter-list-opt: parameter-list
#line 467 "bang.y"
      { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 2103 "bang.tab.cpp"
    break;

  case 46: // parameter-list: parameter-decl
#line 472 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{std::move(yystack_[0].value.as < parameter_t > ())}; }
#line 2109 "bang.tab.cpp"
    break;

  case 47: // parameter-list: parameter-list "," parameter-decl
#line 474 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2115 "bang.tab.cpp"
    break;

  case 48: // parameter-decl: identifier "`:`" type-expr
#line 478 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2121 "bang.tab.cpp"
    break;

  case 49: // parameter-decl: type-expr
#line 480 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ nullopt, std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2127 "bang.tab.cpp"
    break;

  case 50: // parameter-woa-list-opt: %empty
#line 486 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2133 "bang.tab.cpp"
    break;

  case 51: // parameter-woa-list-opt: parameter-woa-list
#line 487 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2139 "bang.tab.cpp"
    break;

  case 52: // parameter-woa-list: parameter-woa-decl
#line 491 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{std::move(yystack_[0].value.as < parameter_woa_t > ())}; }
#line 2145 "bang.tab.cpp"
    break;

  case 53: // parameter-woa-list: parameter-woa-list "," parameter-woa-decl
#line 493 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_woa_t > ())); }
#line 2151 "bang.tab.cpp"
    break;

  case 54: // parameter-woa-decl: parameter-decl
#line 497 "bang.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t { std::move(yystack_[0].value.as < parameter_t > ()) }; }
#line 2157 "bang.tab.cpp"
    break;

  case 55: // parameter-woa-decl: parameter-decl "`=`" expression
#line 499 "bang.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t{ std::move(yystack_[2].value.as < parameter_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2163 "bang.tab.cpp"
    break;

  case 56: // type-expr: BOOL
#line 504 "bang.y"
           { yylhs.value.as < bang_preliminary_type > () = bang_bool_t{}; }
#line 2169 "bang.tab.cpp"
    break;

  case 57: // type-expr: INT
#line 505 "bang.y"
          { yylhs.value.as < bang_preliminary_type > () = bang_int_t{}; }
#line 2175 "bang.tab.cpp"
    break;

  case 58: // type-expr: FLOAT
#line 506 "bang.y"
            { yylhs.value.as < bang_preliminary_type > () = bang_float_t{}; }
#line 2181 "bang.tab.cpp"
    break;

  case 59: // type-expr: STRING_WORD
#line 507 "bang.y"
                  { yylhs.value.as < bang_preliminary_type > () = bang_string_t{}; }
#line 2187 "bang.tab.cpp"
    break;

  case 60: // type-expr: DECIMAL_WORD
#line 508 "bang.y"
                   { yylhs.value.as < bang_preliminary_type > () = bang_decimal_t{}; }
#line 2193 "bang.tab.cpp"
    break;

  case 61: // type-expr: qname
#line 509 "bang.y"
            { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_object_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2199 "bang.tab.cpp"
    break;

  case 62: // type-expr: "`[`" type-expr "`]`"
#line 511 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2205 "bang.tab.cpp"
    break;

  case 63: // type-expr: "`(`" parameter-list-opt "`)`"
#line 513 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2211 "bang.tab.cpp"
    break;

  case 64: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 515 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_decimal > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2217 "bang.tab.cpp"
    break;

  case 65: // type-expr: type-expr "`|`" type-expr
#line 517 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
        }
#line 2228 "bang.tab.cpp"
    break;

  case 66: // type-expr: type-expr "`->`" type-expr
#line 524 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2234 "bang.tab.cpp"
    break;

  case 67: // compound-expression: expression "`(`" opt-named-expr-list-any "`)`"
#line 541 "bang.y"
        { yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2240 "bang.tab.cpp"
    break;

  case 68: // compound-expression: expression "`{`" opt-named-expr-list-any "`}`"
#line 543 "bang.y"
        { 
           yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) };
            //$$ = function_call_t{}; IGNORE($1, $2, $3);
        }
#line 2249 "bang.tab.cpp"
    break;

  case 69: // compound-expression: expression "`=`" expression
#line 548 "bang.y"
        { yylhs.value.as < expression_t > () = assign_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2255 "bang.tab.cpp"
    break;

  case 70: // expression: "true"
#line 553 "bang.y"
        { yylhs.value.as < expression_t > () = annotated_bool{true, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2261 "bang.tab.cpp"
    break;

  case 71: // expression: "false"
#line 555 "bang.y"
        { yylhs.value.as < expression_t > () = annotated_bool{false, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2267 "bang.tab.cpp"
    break;

  case 72: // expression: INTEGER
#line 557 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2273 "bang.tab.cpp"
    break;

  case 73: // expression: DECIMAL
#line 559 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2279 "bang.tab.cpp"
    break;

  case 74: // expression: STRING
#line 561 "bang.y"
        { yylhs.value.as < expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2285 "bang.tab.cpp"
    break;

  case 75: // expression: "`.`" identifier
#line 563 "bang.y"
        { yylhs.value.as < expression_t > () = case_expression { std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2291 "bang.tab.cpp"
    break;

  case 76: // expression: qname
#line 565 "bang.y"
        { yylhs.value.as < expression_t > () = variable_identifier{ ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ())), false}; }
#line 2297 "bang.tab.cpp"
    break;

  case 77: // expression: ARGIDENTIFIER
#line 567 "bang.y"
        { yylhs.value.as < expression_t > () = variable_identifier{ ctx.make_qname_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > (), false), true }; }
#line 2303 "bang.tab.cpp"
    break;

  case 78: // expression: "`(`" expression "`)`"
#line 569 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[1].value.as < expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2309 "bang.tab.cpp"
    break;

  case 79: // expression: FN "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 571 "bang.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[6].value.as < sonia::lang::lex::resource_location > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2315 "bang.tab.cpp"
    break;

  case 80: // expression: FN "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 575 "bang.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[7].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < bang_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[8].value.as < sonia::lang::lex::resource_location > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2321 "bang.tab.cpp"
    break;

  case 81: // expression: "`[`" expression-list-any "`]`"
#line 577 "bang.y"
        { yylhs.value.as < expression_t > () = expression_vector_t{ {std::move(yystack_[1].value.as < expression_list_t > ())}, std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2327 "bang.tab.cpp"
    break;

  case 82: // expression: "`!`" expression
#line 579 "bang.y"
                { yylhs.value.as < expression_t > () = negate_expression_t{ std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2333 "bang.tab.cpp"
    break;

  case 83: // expression: expression "`.`" identifier
#line 581 "bang.y"
        { yylhs.value.as < expression_t > () = member_expression_t { std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2339 "bang.tab.cpp"
    break;

  case 84: // expression: expression "`?`" "`.`" identifier
#line 583 "bang.y"
        { yylhs.value.as < expression_t > () = member_expression_t { std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), true }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2345 "bang.tab.cpp"
    break;

  case 85: // expression: expression "`&&`" expression
#line 587 "bang.y"
        { yylhs.value.as < expression_t > () = logic_and_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2351 "bang.tab.cpp"
    break;

  case 86: // expression: expression "`||`" expression
#line 589 "bang.y"
        { yylhs.value.as < expression_t > () = logic_or_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2357 "bang.tab.cpp"
    break;

  case 87: // expression: expression "`..`" expression
#line 591 "bang.y"
        { yylhs.value.as < expression_t > () = concat_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2363 "bang.tab.cpp"
    break;

  case 88: // expression: compound-expression
#line 594 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 2369 "bang.tab.cpp"
    break;

  case 89: // expression-list-any: %empty
#line 604 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{}; }
#line 2375 "bang.tab.cpp"
    break;

  case 90: // expression-list-any: expression
#line 606 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 2381 "bang.tab.cpp"
    break;

  case 91: // expression-list-any: expression-list-any "," expression
#line 608 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < expression_t > ())); }
#line 2387 "bang.tab.cpp"
    break;

  case 92: // opt-named-expr-list-any: %empty
#line 613 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2393 "bang.tab.cpp"
    break;

  case 93: // opt-named-expr-list-any: opt-named-expr-list
#line 614 "bang.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2399 "bang.tab.cpp"
    break;

  case 94: // opt-named-expr-list: opt-named-expr
#line 619 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2405 "bang.tab.cpp"
    break;

  case 95: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 621 "bang.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2414 "bang.tab.cpp"
    break;

  case 96: // opt-named-expr: qname "`:`" expression
#line 629 "bang.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yystack_[2].value.as < sonia::lang::bang::annotated_qname > ().value.set_absolute(); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::tuple{std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[0].value.as < expression_t > ())}, std::move(loc)}; }
#line 2420 "bang.tab.cpp"
    break;

  case 97: // opt-named-expr: expression
#line 631 "bang.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::move(yystack_[0].value.as < expression_t > ()), std::move(loc)}; }
#line 2426 "bang.tab.cpp"
    break;


#line 2430 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -148;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
    -148,    20,   141,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
     321,   321,  -148,    15,   321,    -3,   -53,    15,    17,     1,
       0,     0,  -148,  -148,  -148,  -148,   -10,    21,  -148,   -47,
    -148,  -148,  -148,   -11,   365,    -4,  -148,   313,   365,   -25,
    -148,    97,    15,  -148,    15,     0,  -148,  -148,   203,   -14,
     -31,   -59,   203,   321,  -148,    15,  -148,   321,   321,   321,
     321,   321,   321,    15,    39,  -148,  -148,   321,  -148,    -1,
     -12,   203,   203,  -148,  -148,  -148,  -148,  -148,    51,   -47,
      85,    66,   -21,  -148,    61,   203,    15,     0,    89,    61,
     324,  -148,   365,   129,    53,    97,   -29,   365,    67,    62,
    -148,    90,  -148,    15,   365,   203,   203,    86,    75,  -148,
      16,   203,   321,     2,   203,   203,   132,   203,   104,  -148,
      95,    94,  -148,   -47,    96,   203,  -148,   321,  -148,   321,
    -148,  -148,    27,   121,  -148,   203,  -148,    61,   365,   203,
    -148,  -148,  -148,   118,    69,    72,  -148,    15,     0,   125,
     365,  -148,  -148,    70,  -148,    47,   193,  -148,   203,  -148,
    -148,   -47,  -148,   203,  -148,  -148,  -148,  -148,   321,  -148,
    -148,   116,    84,   205,   117,   257,   355,  -148,  -148,  -148,
    -148,  -148,  -148,   269,  -148
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    74,    27,    77,    72,    73,
       0,    89,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    70,    71,     5,    13,    19,     0,    29,    76,
      12,    10,    11,    88,     0,     0,    88,     0,    90,     0,
      75,    82,     0,    17,     0,     0,    28,     9,    50,     0,
       0,    40,     0,     0,    15,     0,    14,     0,     0,     0,
       0,    92,    92,     0,     0,    78,    81,     0,    18,     0,
       0,    44,     0,    56,    57,    58,    59,    60,    29,    61,
      54,     0,    51,    52,    49,    50,    34,     0,     0,    20,
       0,    30,    69,    85,    86,    87,    76,    97,     0,    93,
      94,     0,    83,     0,    91,     0,    50,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
       0,    35,    36,    42,    41,    50,    16,     0,    67,     0,
      68,    84,     0,     0,    63,     0,    62,    48,    55,     0,
      21,    53,    66,     0,    65,     0,    33,     0,     0,     0,
      96,    95,     6,     0,    47,     0,     0,    64,     0,    21,
      37,    43,    39,     0,     7,    21,    79,    22,     0,    24,
      23,    88,     0,     0,     0,     0,     0,    26,    21,    31,
       8,    80,    25,     0,    32
  };

  const short
  parser::yypgoto_[] =
  {
    -148,  -148,  -148,  -148,   167,  -148,  -148,  -147,  -148,    80,
      -6,  -148,  -148,  -148,  -148,    28,  -148,  -148,  -148,  -148,
    -148,   -63,   -52,  -148,    60,   -48,    -2,    -9,  -148,   114,
    -148,    49
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    24,   169,    26,    27,   156,   170,    28,
      29,    30,    31,   120,   121,   122,    32,    88,   124,   107,
     108,    80,    81,    82,    83,    84,    36,    34,    39,    98,
      99,   100
  };

  const unsigned char
  parser::yytable_[] =
  {
      33,    37,    38,     6,    89,    41,     6,     6,   109,   127,
      86,    44,   173,    49,    50,    51,   139,    55,   175,    66,
       3,     6,    47,    87,   110,    85,    45,   106,    52,    55,
     115,   183,    53,   118,    67,    48,    56,   105,   114,    70,
      48,   115,    79,   140,    90,    55,    79,    55,    92,    93,
      94,    95,    97,    97,   133,    96,    96,   132,   104,   116,
     136,   115,    55,   137,    55,    79,    79,   142,    54,   144,
     116,   117,   154,   149,   152,   115,    17,    17,    58,    79,
      60,   123,   117,   115,   163,    42,   158,   103,   165,   111,
     116,   155,    61,    40,    62,    43,   112,    46,   115,    79,
      79,    63,   117,   138,   116,    79,   113,   128,    79,    79,
     172,    79,   116,   159,    64,   174,   117,   164,   150,    79,
      97,   129,    68,    96,    69,   178,   134,   116,    78,    79,
     125,   115,   130,    79,   135,    91,    61,   146,    62,   117,
     143,     4,   161,   102,   145,    63,     5,     6,     7,     8,
       9,    78,    79,   147,   171,   148,    60,    79,    64,   176,
     116,   153,   157,   177,   180,    78,   119,   162,    61,    25,
      62,   171,   117,   171,   141,   160,   101,    63,   151,     0,
      10,   171,     0,   131,    11,     0,    78,     0,    12,    13,
      64,     0,     0,     0,    78,     0,     0,    14,     5,     6,
       7,     8,     9,     0,    15,    78,     0,    16,     0,     6,
       5,     6,     7,     8,     9,    78,     0,    17,     0,    18,
      19,    20,    21,     0,     0,     0,     0,   119,     0,     0,
      22,    23,    10,     0,     0,   166,    11,     0,     0,     0,
     167,    13,    71,     0,    10,     0,    72,   179,    11,    14,
       0,     0,   167,    13,     0,     0,    15,     0,   168,     0,
       0,    14,     5,     6,     7,     8,     9,     0,    15,    17,
     168,     0,    35,     0,     5,     6,     7,     8,     9,    17,
       0,    17,    22,    23,    35,     0,    73,    74,    75,    76,
      77,     0,     0,     0,    22,    23,    10,     0,     0,   181,
      11,     0,     0,     0,   167,    13,     0,     0,    10,     0,
       0,   184,    11,    14,     0,     0,   167,    13,     0,     0,
      15,     0,   168,     0,    57,    14,     5,     6,     7,     8,
       9,     0,    15,    17,   168,    57,    35,     0,    58,    59,
      60,     0,     0,     0,     0,    17,    22,    23,    35,    58,
      59,    60,    61,    65,    62,     0,     0,     0,    22,    23,
      10,    63,     0,    61,    11,    62,    57,     0,     0,    13,
       0,   126,    63,     0,    64,     0,    57,    14,     0,     0,
      58,    59,    60,     0,     0,    64,     0,     0,     0,     0,
      58,    59,    60,     0,    61,     0,    62,    17,     0,     0,
      35,     0,   182,    63,    61,     0,    62,     0,     0,     0,
      22,    23,     0,    63,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64
  };

  const short
  parser::yycheck_[] =
  {
       2,    10,    11,     6,    52,    14,     6,     6,    71,    38,
      41,    64,   159,    19,    20,    21,    14,    76,   165,    44,
       0,     6,     5,    82,    72,    39,    79,    39,    38,    76,
      14,   178,    11,    85,    59,    39,    47,    38,    59,    45,
      39,    14,    48,    41,    53,    76,    52,    76,    57,    58,
      59,    60,    61,    62,   106,    61,    62,   105,    67,    43,
      44,    14,    76,   111,    76,    71,    72,   115,    47,   117,
      43,    55,   135,   125,    47,    14,    76,    76,    25,    85,
      27,    87,    55,    14,    14,    88,    14,    48,    41,    38,
      43,   139,    39,    13,    41,    15,    11,    17,    14,   105,
     106,    48,    55,   112,    43,   111,    40,    40,   114,   115,
     158,   117,    43,    41,    61,   163,    55,    47,   127,   125,
     129,    59,    42,   129,    44,    41,    40,    43,    48,   135,
      41,    14,    42,   139,    59,    55,    39,    42,    41,    55,
       8,     0,   148,    63,    40,    48,     5,     6,     7,     8,
       9,    71,   158,    59,   156,    59,    27,   163,    61,   168,
      43,    40,    44,    47,    47,    85,    86,    42,    39,     2,
      41,   173,    55,   175,   114,   147,    62,    48,   129,    -1,
      39,   183,    -1,   103,    43,    -1,   106,    -1,    47,    48,
      61,    -1,    -1,    -1,   114,    -1,    -1,    56,     5,     6,
       7,     8,     9,    -1,    63,   125,    -1,    66,    -1,     6,
       5,     6,     7,     8,     9,   135,    -1,    76,    -1,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      89,    90,    39,    -1,    -1,    42,    43,    -1,    -1,    -1,
      47,    48,    39,    -1,    39,    -1,    43,    42,    43,    56,
      -1,    -1,    47,    48,    -1,    -1,    63,    -1,    65,    -1,
      -1,    56,     5,     6,     7,     8,     9,    -1,    63,    76,
      65,    -1,    79,    -1,     5,     6,     7,     8,     9,    76,
      -1,    76,    89,    90,    79,    -1,    83,    84,    85,    86,
      87,    -1,    -1,    -1,    89,    90,    39,    -1,    -1,    42,
      43,    -1,    -1,    -1,    47,    48,    -1,    -1,    39,    -1,
      -1,    42,    43,    56,    -1,    -1,    47,    48,    -1,    -1,
      63,    -1,    65,    -1,    11,    56,     5,     6,     7,     8,
       9,    -1,    63,    76,    65,    11,    79,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    76,    89,    90,    79,    25,
      26,    27,    39,    40,    41,    -1,    -1,    -1,    89,    90,
      39,    48,    -1,    39,    43,    41,    11,    -1,    -1,    48,
      -1,    47,    48,    -1,    61,    -1,    11,    56,    -1,    -1,
      25,    26,    27,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      25,    26,    27,    -1,    39,    -1,    41,    76,    -1,    -1,
      79,    -1,    47,    48,    39,    -1,    41,    -1,    -1,    -1,
      89,    90,    -1,    48,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61
  };

  const signed char
  parser::yystos_[] =
  {
       0,    92,    93,     0,     0,     5,     6,     7,     8,     9,
      39,    43,    47,    48,    56,    63,    66,    76,    78,    79,
      80,    81,    89,    90,    94,    95,    96,    97,   100,   101,
     102,   103,   107,   117,   118,    79,   117,   118,   118,   119,
     100,   118,    88,   100,    64,    79,   100,     5,    39,   101,
     101,   101,    38,    11,    47,    76,    47,    11,    25,    26,
      27,    39,    41,    48,    61,    40,    44,    59,   100,   100,
     101,    39,    43,    83,    84,    85,    86,    87,   100,   101,
     112,   113,   114,   115,   116,    39,    41,    82,   108,   116,
     118,   100,   118,   118,   118,   118,   101,   118,   120,   121,
     122,   120,   100,    48,   118,    38,    39,   110,   111,   112,
     116,    38,    11,    40,    59,    14,    43,    55,   113,   100,
     104,   105,   106,   101,   109,    41,    47,    38,    40,    59,
      42,   100,   116,   113,    40,    59,    44,   116,   118,    14,
      41,   115,   116,     8,   116,    40,    42,    59,    59,   113,
     118,   122,    47,    40,   112,   116,    98,    44,    14,    41,
     106,   101,    42,    14,    47,    41,    42,    47,    65,    95,
      99,   117,   116,    98,   116,    98,   118,    47,    41,    42,
      47,    42,    47,    98,    42
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    91,    92,    93,    93,    93,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    95,    95,    96,    96,    97,
      97,    98,    98,    98,    99,    99,    99,   100,   101,   101,
     101,   102,   102,   103,   104,   104,   105,   105,   106,   107,
     108,   108,   109,   109,   110,   110,   111,   111,   112,   112,
     113,   113,   114,   114,   115,   115,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   117,   117,   117,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   119,
     119,   119,   120,   120,   121,   121,   122,   122
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     6,     7,     9,     2,
       1,     1,     1,     1,     2,     2,     4,     2,     3,     1,
       3,     0,     2,     2,     1,     3,     2,     1,     2,     1,
       3,     8,    10,     5,     0,     1,     1,     3,     1,     6,
       0,     2,     1,     3,     0,     1,     1,     3,     3,     1,
       0,     1,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     3,     3,     4,     3,     3,     4,     4,     3,
       1,     1,     1,     1,     1,     2,     1,     1,     3,     7,
       9,     3,     2,     3,     4,     3,     3,     3,     1,     0,
       1,     3,     0,     1,     1,     3,     3,     1
  };


#if BANG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "ARGIDENTIFIER", "INTEGER",
  "DECIMAL", "OPERATOR_TERM", "\"`=`\"", "\"`_`\"", "\"`->*`\"",
  "\"`->`\"", "\"`~>`\"", "\"`.*`\"", "\"`++`\"", "\"`--`\"", "\"`<<`\"",
  "\"`>>`\"", "\"`<=`\"", "\"`>=`\"", "\"`==`\"", "\"`!=`\"", "\"`&&`\"",
  "\"`||`\"", "\"`..`\"", "\"`+=`\"", "\"`-=`\"", "\"`*=`\"", "\"`/=`\"",
  "\"`%=`\"", "\"`<<=`\"", "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"", "\"`^=`\"",
  "\"`:`\"", "\"`(`\"", "\"`)`\"", "\"`{`\"", "\"`}`\"", "\"`[`\"",
  "\"`]`\"", "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"", "\"`+`\"",
  "\"`-`\"", "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"", "\"`|`\"",
  "\"`!`\"", "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"", "\"`?`\"",
  "\"`#`\"", "LET", "VAR", "RETURN", "EXTERN", "FOR", "VOID_", "AUTO",
  "CONST_", "USING", "THROW", "DELETE_", "SIZEOF", "LOWEST", "DBLCOLON",
  "\"`::`\"", "INCLUDE", "FN", "ENUM", "TYPE", "EXTENDS", "BOOL", "INT",
  "FLOAT", "STRING_WORD", "DECIMAL_WORD", "\"weak modifier\"", "\"true\"",
  "\"false\"", "$accept", "begin", "declaration_any", "generic-decl",
  "let-decl", "let-decl-start", "let-decl-start-with-opt-type",
  "infunction_declaration_any", "opt-infunction-decl", "identifier",
  "qname", "fn-decl", "enum-decl", "case-list-opt", "case-list",
  "case-decl", "type-decl", "type-extension-any", "type-extension-list",
  "parameter-list-opt", "parameter-list", "parameter-decl",
  "parameter-woa-list-opt", "parameter-woa-list", "parameter-woa-decl",
  "type-expr", "compound-expression", "expression", "expression-list-any",
  "opt-named-expr-list-any", "opt-named-expr-list", "opt-named-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   290,   290,   294,   296,   298,   318,   320,   322,   324,
     326,   328,   330,   332,   334,   348,   350,   355,   357,   362,
     363,   368,   370,   372,   380,   382,   384,   389,   398,   400,
     402,   408,   412,   419,   426,   428,   432,   435,   440,   445,
     452,   454,   458,   461,   466,   467,   471,   473,   477,   479,
     486,   487,   490,   492,   496,   498,   504,   505,   506,   507,
     508,   509,   510,   512,   514,   516,   523,   540,   542,   547,
     552,   554,   556,   558,   560,   562,   564,   566,   568,   570,
     574,   576,   578,   580,   582,   586,   588,   590,   593,   603,
     605,   607,   612,   614,   618,   620,   628,   630
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
      85,    86,    87,    88,    89,    90
    };
    // Last valid token kind.
    const int code_max = 345;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3137 "bang.tab.cpp"

#line 980 "bang.y"

