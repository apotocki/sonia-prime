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
#define yylex   beng_langlex

// First part of user prologue.
#line 24 "beng.y"

#include "sonia/config.hpp"
#include <iostream>
#include "parser.hpp"

using namespace sonia;
using namespace sonia::lang::beng;

#include "beng.tab.hpp"

using YYSTYPE = beng_lang::parser::semantic_type;
using YYLTYPE = beng_lang::parser::location_type;

int beng_langlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param, parser_context & ctx, void* yyscanner);
//#include "beng.yy.hpp"

void beng_lang::parser::error(const location_type& loc, const std::string& msg)
{
    ctx.append_error(("%1%(%2%,%3%-%4%,%5%): error: %6%"_fmt
		% ctx.get_resource()
		% loc.begin.line % loc.begin.column % loc.end.line % loc.end.column % msg).str());
}


#line 68 "beng.tab.cpp"


#include "beng.tab.hpp"




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
#if BENG_LANGDEBUG

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

#else // !BENG_LANGDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !BENG_LANGDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace beng_lang {
#line 165 "beng.tab.cpp"

  /// Build a parser object.
  parser::parser (void * scanner_yyarg, parser_context & ctx_yyarg)
#if BENG_LANGDEBUG
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
      case symbol_kind::S_113_type_expr: // type-expr
        value.copy< beng_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.copy< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
      case symbol_kind::S_106_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
        value.copy< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_108_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
        value.copy< parameter_woa_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::beng::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.copy< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
      case symbol_kind::S_102_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_type_decl: // type-decl
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
      case symbol_kind::S_113_type_expr: // type-expr
        value.move< beng_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.move< declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
      case symbol_kind::S_106_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_108_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
        value.move< parameter_woa_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
      case symbol_kind::S_102_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_104_type_decl: // type-decl
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
      case symbol_kind::S_113_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< beng_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.YY_MOVE_OR_COPY< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
      case symbol_kind::S_106_type_extension_list: // type-extension-list
        value.YY_MOVE_OR_COPY< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
        value.YY_MOVE_OR_COPY< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_108_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
        value.YY_MOVE_OR_COPY< parameter_woa_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.YY_MOVE_OR_COPY< sonia::decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.YY_MOVE_OR_COPY< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
      case symbol_kind::S_102_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_type_decl: // type-decl
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
      case symbol_kind::S_113_type_expr: // type-expr
        value.move< beng_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.move< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
      case symbol_kind::S_106_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_108_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
        value.move< parameter_woa_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
      case symbol_kind::S_102_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_type_decl: // type-decl
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
      case symbol_kind::S_113_type_expr: // type-expr
        value.copy< beng_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.copy< declaration_t > (that.value);
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (that.value);
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
      case symbol_kind::S_106_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (that.value);
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
        value.copy< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_108_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
        value.copy< parameter_woa_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::beng::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::beng::annotated_qname > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.copy< sonia::lang::beng::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_103_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
      case symbol_kind::S_102_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_104_type_decl: // type-decl
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
      case symbol_kind::S_113_type_expr: // type-expr
        value.move< beng_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        value.move< declaration_t > (that.value);
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (that.value);
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
      case symbol_kind::S_106_type_extension_list: // type-extension-list
        value.move< extension_list_t > (that.value);
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
        value.move< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_108_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
        value.move< parameter_woa_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::annotated_qname > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_103_case_decl: // case-decl
        value.move< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
      case symbol_kind::S_102_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_104_type_decl: // type-decl
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

#if BENG_LANGDEBUG
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
#line 272 "beng.y"
                 { }
#line 1061 "beng.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 272 "beng.y"
                 { }
#line 1067 "beng.tab.cpp"
        break;

      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
#line 272 "beng.y"
                 { }
#line 1073 "beng.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 272 "beng.y"
                 { }
#line 1079 "beng.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 272 "beng.y"
                 { }
#line 1085 "beng.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 272 "beng.y"
                 { }
#line 1091 "beng.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 272 "beng.y"
                 { }
#line 1097 "beng.tab.cpp"
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
#line 272 "beng.y"
                 { }
#line 1103 "beng.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 272 "beng.y"
                 { }
#line 1109 "beng.tab.cpp"
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
#line 272 "beng.y"
                 { }
#line 1115 "beng.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 272 "beng.y"
                 { }
#line 1121 "beng.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 272 "beng.y"
                 { }
#line 1127 "beng.tab.cpp"
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
#line 272 "beng.y"
                 { }
#line 1133 "beng.tab.cpp"
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
#line 272 "beng.y"
                 { }
#line 1139 "beng.tab.cpp"
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
#line 272 "beng.y"
                 { }
#line 1145 "beng.tab.cpp"
        break;

      case symbol_kind::S_102_case_list: // case-list
#line 272 "beng.y"
                 { }
#line 1151 "beng.tab.cpp"
        break;

      case symbol_kind::S_103_case_decl: // case-decl
#line 272 "beng.y"
                 { }
#line 1157 "beng.tab.cpp"
        break;

      case symbol_kind::S_104_type_decl: // type-decl
#line 272 "beng.y"
                 { }
#line 1163 "beng.tab.cpp"
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
#line 272 "beng.y"
                 { }
#line 1169 "beng.tab.cpp"
        break;

      case symbol_kind::S_106_type_extension_list: // type-extension-list
#line 272 "beng.y"
                 { }
#line 1175 "beng.tab.cpp"
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
#line 272 "beng.y"
                 { }
#line 1181 "beng.tab.cpp"
        break;

      case symbol_kind::S_108_parameter_list: // parameter-list
#line 272 "beng.y"
                 { }
#line 1187 "beng.tab.cpp"
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
#line 272 "beng.y"
                 { }
#line 1193 "beng.tab.cpp"
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
#line 272 "beng.y"
                 { }
#line 1199 "beng.tab.cpp"
        break;

      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
#line 272 "beng.y"
                 { }
#line 1205 "beng.tab.cpp"
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
#line 272 "beng.y"
                 { }
#line 1211 "beng.tab.cpp"
        break;

      case symbol_kind::S_113_type_expr: // type-expr
#line 272 "beng.y"
                 { }
#line 1217 "beng.tab.cpp"
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
#line 272 "beng.y"
                 { }
#line 1223 "beng.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 272 "beng.y"
                 { }
#line 1229 "beng.tab.cpp"
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
#line 272 "beng.y"
                 { }
#line 1235 "beng.tab.cpp"
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
#line 272 "beng.y"
                 { }
#line 1241 "beng.tab.cpp"
        break;

      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
#line 272 "beng.y"
                 { }
#line 1247 "beng.tab.cpp"
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
#line 272 "beng.y"
                 { }
#line 1253 "beng.tab.cpp"
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

#if BENG_LANGDEBUG
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
#endif // BENG_LANGDEBUG

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
      case symbol_kind::S_113_type_expr: // type-expr
        yylhs.value.emplace< beng_preliminary_type > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_94_generic_decl: // generic-decl
        yylhs.value.emplace< declaration_t > ();
        break;

      case symbol_kind::S_100_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_116_expression_list_any: // expression-list-any
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_114_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< expression_t > ();
        break;

      case symbol_kind::S_105_type_extension_any: // type-extension-any
      case symbol_kind::S_106_type_extension_list: // type-extension-list
        yylhs.value.emplace< extension_list_t > ();
        break;

      case symbol_kind::S_99_fn_decl: // fn-decl
        yylhs.value.emplace< fn_decl_t > ();
        break;

      case symbol_kind::S_96_opt_infunction_decl: // opt-infunction-decl
        yylhs.value.emplace< infunction_declaration_t > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_117_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_118_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_119_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_107_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_108_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_109_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_110_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_111_parameter_woa_list: // parameter-woa-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_112_parameter_woa_decl: // parameter-woa-decl
        yylhs.value.emplace< parameter_woa_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        yylhs.value.emplace< sonia::decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
        yylhs.value.emplace< sonia::lang::beng::annotated_identifier > ();
        break;

      case symbol_kind::S_qname: // qname
        yylhs.value.emplace< sonia::lang::beng::annotated_qname > ();
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        yylhs.value.emplace< sonia::lang::beng::annotated_string_view > ();
        break;

      case symbol_kind::S_103_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::identifier > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        yylhs.value.emplace< std::vector<infunction_declaration_t> > ();
        break;

      case symbol_kind::S_101_case_list_opt: // case-list-opt
      case symbol_kind::S_102_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::identifier> > ();
        break;

      case symbol_kind::S_104_type_decl: // type-decl
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
#line 277 "beng.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 1613 "beng.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 282 "beng.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 1619 "beng.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any "`;`"
#line 284 "beng.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 1625 "beng.tab.cpp"
    break;

  case 5: // declaration_any: declaration_any enum-decl
#line 286 "beng.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); yylhs.value.as < declaration_set_t > ().types.emplace_back(std::move(yystack_[0].value.as < enum_decl > ())); }
#line 1631 "beng.tab.cpp"
    break;

  case 6: // declaration_any: declaration_any type-decl
#line 288 "beng.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); yylhs.value.as < declaration_set_t > ().types.emplace_back(std::move(yystack_[0].value.as < type_decl > ())); }
#line 1637 "beng.tab.cpp"
    break;

  case 7: // declaration_any: declaration_any generic-decl
#line 290 "beng.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().generic.emplace_back(std::move(yystack_[0].value.as < declaration_t > ()));
        }
#line 1646 "beng.tab.cpp"
    break;

  case 8: // generic-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 310 "beng.y"
        { yylhs.value.as < declaration_t > () = exten_var{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < beng_preliminary_type > ()) }; }
#line 1652 "beng.tab.cpp"
    break;

  case 9: // generic-decl: EXTERN FN qname "`(`" parameter-woa-list-opt "`)`" "`;`"
#line 312 "beng.y"
        { yylhs.value.as < declaration_t > () = fn_pure_decl{ std::move(yystack_[4].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()), beng_preliminary_tuple_t{} }; }
#line 1658 "beng.tab.cpp"
    break;

  case 10: // generic-decl: EXTERN FN qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`;`"
#line 314 "beng.y"
        { yylhs.value.as < declaration_t > () = fn_pure_decl{ std::move(yystack_[6].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), std::move(yystack_[1].value.as < beng_preliminary_type > ()) }; }
#line 1664 "beng.tab.cpp"
    break;

  case 11: // generic-decl: fn-decl
#line 316 "beng.y"
        { yylhs.value.as < declaration_t > () = std::move(yystack_[0].value.as < fn_decl_t > ()); }
#line 1670 "beng.tab.cpp"
    break;

  case 12: // generic-decl: LET identifier ASSIGN expression "`;`"
#line 318 "beng.y"
        { yylhs.value.as < declaration_t > () = let_statement_decl{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1676 "beng.tab.cpp"
    break;

  case 13: // generic-decl: compound-expression "`;`"
#line 320 "beng.y"
        { yylhs.value.as < declaration_t > () = expression_decl{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1682 "beng.tab.cpp"
    break;

  case 14: // infunction_declaration_any: %empty
#line 334 "beng.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 1688 "beng.tab.cpp"
    break;

  case 15: // infunction_declaration_any: infunction_declaration_any "`;`"
#line 336 "beng.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()); }
#line 1694 "beng.tab.cpp"
    break;

  case 16: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 338 "beng.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 1703 "beng.tab.cpp"
    break;

  case 17: // opt-infunction-decl: LET identifier ASSIGN expression "`;`"
#line 346 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = let_statement_decl{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1709 "beng.tab.cpp"
    break;

  case 18: // opt-infunction-decl: RETURN expression "`;`"
#line 348 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = return_decl{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1715 "beng.tab.cpp"
    break;

  case 19: // opt-infunction-decl: compound-expression "`;`"
#line 350 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1721 "beng.tab.cpp"
    break;

  case 20: // identifier: IDENTIFIER
#line 355 "beng.y"
      { yylhs.value.as < sonia::lang::beng::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::beng::annotated_string_view > ()); }
#line 1727 "beng.tab.cpp"
    break;

  case 21: // qname: DBLCOLON identifier
#line 364 "beng.y"
        { yylhs.value.as < sonia::lang::beng::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id}, std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().location) }; }
#line 1733 "beng.tab.cpp"
    break;

  case 22: // qname: identifier
#line 366 "beng.y"
        { yylhs.value.as < sonia::lang::beng::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id, false}, std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().location) }; }
#line 1739 "beng.tab.cpp"
    break;

  case 23: // qname: qname DBLCOLON identifier
#line 368 "beng.y"
        { yylhs.value.as < sonia::lang::beng::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::beng::annotated_qname > ()); yylhs.value.as < sonia::lang::beng::annotated_qname > ().name.append(std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id)); }
#line 1745 "beng.tab.cpp"
    break;

  case 24: // fn-decl: FN qname "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 374 "beng.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[6].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())};
            }
#line 1753 "beng.tab.cpp"
    break;

  case 25: // fn-decl: FN qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 378 "beng.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[8].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < beng_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())};
            }
#line 1761 "beng.tab.cpp"
    break;

  case 26: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 385 "beng.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{annotated_qname{ctx.ns() + std::move(yystack_[3].value.as < sonia::lang::beng::annotated_qname > ().name), yystack_[3].value.as < sonia::lang::beng::annotated_qname > ().location}, std::move(yystack_[1].value.as < std::vector<sonia::lang::identifier> > ())};
    }
#line 1769 "beng.tab.cpp"
    break;

  case 27: // case-list-opt: %empty
#line 391 "beng.y"
           { yylhs.value.as < std::vector<sonia::lang::identifier> > () = {}; }
#line 1775 "beng.tab.cpp"
    break;

  case 28: // case-list-opt: case-list
#line 393 "beng.y"
    { yylhs.value.as < std::vector<sonia::lang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::identifier> > (); }
#line 1781 "beng.tab.cpp"
    break;

  case 29: // case-list: case-decl
#line 398 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::vector<sonia::lang::identifier>{std::move(yystack_[0].value.as < sonia::lang::identifier > ())}; }
#line 1787 "beng.tab.cpp"
    break;

  case 30: // case-list: case-list "," case-decl
#line 401 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::identifier > ())); }
#line 1793 "beng.tab.cpp"
    break;

  case 31: // case-decl: identifier
#line 406 "beng.y"
        { yylhs.value.as < sonia::lang::identifier > () = yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id; }
#line 1799 "beng.tab.cpp"
    break;

  case 32: // type-decl: TYPE qname type-extension-any "`{`" parameter-woa-list-opt "`}`"
#line 411 "beng.y"
    {
        yylhs.value.as < type_decl > () = type_decl{annotated_qname{ctx.ns() + std::move(yystack_[4].value.as < sonia::lang::beng::annotated_qname > ().name), yystack_[4].value.as < sonia::lang::beng::annotated_qname > ().location}, std::move(yystack_[3].value.as < extension_list_t > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ())};
    }
#line 1807 "beng.tab.cpp"
    break;

  case 33: // type-extension-any: %empty
#line 417 "beng.y"
           { yylhs.value.as < extension_list_t > () = {}; }
#line 1813 "beng.tab.cpp"
    break;

  case 34: // type-extension-any: EXTENDS type-extension-list
#line 419 "beng.y"
                                { yylhs.value.as < extension_list_t > () = std::move(yystack_[0].value.as < extension_list_t > ()); }
#line 1819 "beng.tab.cpp"
    break;

  case 35: // type-extension-list: qname
#line 424 "beng.y"
        { yylhs.value.as < extension_list_t > () = extension_list_t{std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ())}; }
#line 1825 "beng.tab.cpp"
    break;

  case 36: // type-extension-list: type-extension-list "," qname
#line 427 "beng.y"
        { yylhs.value.as < extension_list_t > () = std::move(yystack_[2].value.as < extension_list_t > ()); yylhs.value.as < extension_list_t > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ())); }
#line 1831 "beng.tab.cpp"
    break;

  case 37: // parameter-list-opt: %empty
#line 431 "beng.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 1837 "beng.tab.cpp"
    break;

  case 38: // parameter-list-opt: parameter-list
#line 432 "beng.y"
      { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 1843 "beng.tab.cpp"
    break;

  case 39: // parameter-list: parameter-decl
#line 436 "beng.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{std::move(yystack_[0].value.as < parameter_t > ())}; }
#line 1849 "beng.tab.cpp"
    break;

  case 40: // parameter-list: parameter-list "," parameter-decl
#line 438 "beng.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 1855 "beng.tab.cpp"
    break;

  case 41: // parameter-decl: identifier "`:`" type-expr
#line 442 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[0].value.as < beng_preliminary_type > ()) }; }
#line 1861 "beng.tab.cpp"
    break;

  case 42: // parameter-decl: type-expr
#line 444 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ nullopt, std::move(yystack_[0].value.as < beng_preliminary_type > ()) }; }
#line 1867 "beng.tab.cpp"
    break;

  case 43: // parameter-woa-list-opt: %empty
#line 450 "beng.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 1873 "beng.tab.cpp"
    break;

  case 44: // parameter-woa-list-opt: parameter-woa-list
#line 451 "beng.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 1879 "beng.tab.cpp"
    break;

  case 45: // parameter-woa-list: parameter-woa-decl
#line 455 "beng.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{std::move(yystack_[0].value.as < parameter_woa_t > ())}; }
#line 1885 "beng.tab.cpp"
    break;

  case 46: // parameter-woa-list: parameter-woa-list "," parameter-woa-decl
#line 457 "beng.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_woa_t > ())); }
#line 1891 "beng.tab.cpp"
    break;

  case 47: // parameter-woa-decl: parameter-decl
#line 461 "beng.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t { std::move(yystack_[0].value.as < parameter_t > ()) }; }
#line 1897 "beng.tab.cpp"
    break;

  case 48: // parameter-woa-decl: parameter-decl ASSIGN expression
#line 463 "beng.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t{ std::move(yystack_[2].value.as < parameter_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 1903 "beng.tab.cpp"
    break;

  case 49: // type-expr: BOOL
#line 468 "beng.y"
           { yylhs.value.as < beng_preliminary_type > () = beng_bool_t{}; }
#line 1909 "beng.tab.cpp"
    break;

  case 50: // type-expr: INT
#line 469 "beng.y"
          { yylhs.value.as < beng_preliminary_type > () = beng_int_t{}; }
#line 1915 "beng.tab.cpp"
    break;

  case 51: // type-expr: FLOAT
#line 470 "beng.y"
            { yylhs.value.as < beng_preliminary_type > () = beng_float_t{}; }
#line 1921 "beng.tab.cpp"
    break;

  case 52: // type-expr: STRING_WORD
#line 471 "beng.y"
                  { yylhs.value.as < beng_preliminary_type > () = beng_string_t{}; }
#line 1927 "beng.tab.cpp"
    break;

  case 53: // type-expr: DECIMAL_WORD
#line 472 "beng.y"
                   { yylhs.value.as < beng_preliminary_type > () = beng_decimal_t{}; }
#line 1933 "beng.tab.cpp"
    break;

  case 54: // type-expr: qname
#line 473 "beng.y"
            { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_object_t{ std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ()) }; }
#line 1939 "beng.tab.cpp"
    break;

  case 55: // type-expr: "`[`" type-expr "`]`"
#line 475 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_vector_t{std::move(yystack_[1].value.as < beng_preliminary_type > ())}; }
#line 1945 "beng.tab.cpp"
    break;

  case 56: // type-expr: "`(`" parameter-list-opt "`)`"
#line 477 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_tuple_t { std::move(yystack_[1].value.as < parameter_list_t > ()) }; }
#line 1951 "beng.tab.cpp"
    break;

  case 57: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 479 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_array_t{std::move(yystack_[3].value.as < beng_preliminary_type > ()), (size_t)std::move(yystack_[1].value.as < int > ())}; }
#line 1957 "beng.tab.cpp"
    break;

  case 58: // type-expr: type-expr "`|`" type-expr
#line 481 "beng.y"
        {
            beng_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < beng_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < beng_preliminary_type > ()));
            yylhs.value.as < beng_preliminary_type > () = std::move(uni);
        }
#line 1968 "beng.tab.cpp"
    break;

  case 59: // type-expr: type-expr "`->`" type-expr
#line 488 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_fn_t{std::move(yystack_[2].value.as < beng_preliminary_type > ()), std::move(yystack_[0].value.as < beng_preliminary_type > ())}; }
#line 1974 "beng.tab.cpp"
    break;

  case 60: // compound-expression: expression "`(`" opt-named-expr-list-any "`)`"
#line 505 "beng.y"
        { yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 1980 "beng.tab.cpp"
    break;

  case 61: // compound-expression: expression ASSIGN expression
#line 507 "beng.y"
        { yylhs.value.as < expression_t > () = assign_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 1986 "beng.tab.cpp"
    break;

  case 62: // expression: INTEGER
#line 512 "beng.y"
        { yylhs.value.as < expression_t > () =  decimal(yystack_[0].value.as < int > ()); }
#line 1992 "beng.tab.cpp"
    break;

  case 63: // expression: DECIMAL
#line 514 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::decimal > ()); }
#line 1998 "beng.tab.cpp"
    break;

  case 64: // expression: STRING
#line 516 "beng.y"
        { yylhs.value.as < expression_t > () = ctx.make_string(yystack_[0].value.as < sonia::string_view > ()); }
#line 2004 "beng.tab.cpp"
    break;

  case 65: // expression: "`.`" identifier
#line 518 "beng.y"
        { yylhs.value.as < expression_t > () = case_expression { std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id) }; }
#line 2010 "beng.tab.cpp"
    break;

  case 66: // expression: qname
#line 520 "beng.y"
        { yylhs.value.as < expression_t > () = variable_identifier{std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ().name), std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ().location), false}; }
#line 2016 "beng.tab.cpp"
    break;

  case 67: // expression: ARGIDENTIFIER
#line 522 "beng.y"
        { auto tmp = ctx.make_identifier(yystack_[0].value.as < sonia::lang::beng::annotated_string_view > ()); yylhs.value.as < expression_t > () = variable_identifier{ qname{tmp.id, false}, std::move(tmp.location), true }; }
#line 2022 "beng.tab.cpp"
    break;

  case 68: // expression: "`(`" expression "`)`"
#line 524 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[1].value.as < expression_t > ()); }
#line 2028 "beng.tab.cpp"
    break;

  case 69: // expression: "`[`" expression-list-any "`]`"
#line 526 "beng.y"
        { yylhs.value.as < expression_t > () = expression_vector_t{ {std::move(yystack_[1].value.as < expression_list_t > ())} }; }
#line 2034 "beng.tab.cpp"
    break;

  case 70: // expression: compound-expression
#line 528 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 2040 "beng.tab.cpp"
    break;

  case 71: // expression-list-any: %empty
#line 538 "beng.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{}; }
#line 2046 "beng.tab.cpp"
    break;

  case 72: // expression-list-any: expression
#line 540 "beng.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 2052 "beng.tab.cpp"
    break;

  case 73: // expression-list-any: expression-list-any "," expression
#line 542 "beng.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < expression_t > ())); }
#line 2058 "beng.tab.cpp"
    break;

  case 74: // opt-named-expr-list-any: %empty
#line 547 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2064 "beng.tab.cpp"
    break;

  case 75: // opt-named-expr-list-any: opt-named-expr-list
#line 548 "beng.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2070 "beng.tab.cpp"
    break;

  case 76: // opt-named-expr-list: opt-named-expr
#line 553 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2076 "beng.tab.cpp"
    break;

  case 77: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 555 "beng.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2085 "beng.tab.cpp"
    break;

  case 78: // opt-named-expr: identifier "`:`" expression
#line 563 "beng.y"
        { yylhs.value.as < named_expression_term_t > () = std::tuple{std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[0].value.as < expression_t > ())}; }
#line 2091 "beng.tab.cpp"
    break;

  case 79: // opt-named-expr: expression
#line 565 "beng.y"
        { yylhs.value.as < named_expression_term_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 2097 "beng.tab.cpp"
    break;


#line 2101 "beng.tab.cpp"

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


  const signed char parser::yypact_ninf_ = -60;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -60,    12,   110,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     179,   179,   -60,    16,    16,   -52,    16,    -5,    -5,    -5,
     -60,   -60,   -40,   -60,   -60,   -60,   -17,   -14,   -60,   -20,
     -14,   -31,   -60,   -19,    16,    -5,   -60,   -29,   -12,   -56,
      16,   -60,   179,   179,   -60,   -60,   179,   179,    37,   -28,
      44,    16,    -5,    45,   -60,    52,   -14,    53,    35,   -60,
     -14,   -14,   -24,    44,    44,    44,    44,   -60,   -60,   -60,
     -60,   -60,    58,   -40,    23,    67,    50,   -60,    33,   -60,
      68,    54,   -60,   -40,    55,    44,   179,   -60,   179,   -60,
     108,    76,    85,    78,   -60,    30,    44,   179,     6,    44,
      44,   116,    44,   -60,    16,    -5,    96,   -14,   -60,   -60,
      24,   -60,    44,   -60,    33,   -14,    44,   -60,   -60,   -60,
      97,     7,   -60,   -40,   -60,    44,   -60,   -60,   161,   135,
     -60,   156,   -60,   -60,   -60,    16,   179,   -60,   100,   -60,
     158,    70,    -1,   -60,   -60,   179,   -60,    20,   -60
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    64,    20,    67,    63,    62,
       0,    71,     4,     0,     0,     0,     0,     0,     0,     0,
       7,    22,    66,    11,     5,     6,    70,     0,    70,     0,
      72,     0,    65,     0,     0,     0,    21,     0,     0,    33,
       0,    13,    74,     0,    68,    69,     0,     0,     0,     0,
      43,    27,     0,     0,    23,    22,    79,     0,    75,    76,
      61,    73,     0,     0,    43,    37,     0,    49,    50,    51,
      52,    53,    22,    54,    47,     0,    44,    45,    42,    31,
       0,    28,    29,    35,    34,    43,     0,    60,     0,    12,
       0,     0,     0,    38,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,    78,    77,     8,
       0,    56,     0,    55,    41,    48,     0,    14,    46,    59,
       0,    58,    30,    36,    32,     0,     9,    40,     0,     0,
      57,     0,    14,    24,    15,     0,     0,    16,    70,    10,
       0,     0,     0,    19,    25,     0,    18,     0,    17
  };

  const signed char
  parser::yypgoto_[] =
  {
     -60,   -60,   -60,   -60,    14,   -60,    -9,    -3,   -60,   -60,
     -60,   -60,    46,   -60,   -60,   -60,   -60,   -60,   -59,     2,
     -60,    59,     4,    -2,    -8,   -60,   -60,   -60,    64
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    20,   129,   137,    21,    22,    23,    24,
      80,    81,    82,    25,    53,    84,    92,    93,    74,    75,
      76,    77,    78,    28,    27,    31,    57,    58,    59
  };

  const unsigned char
  parser::yytable_[] =
  {
      26,     6,    29,    30,    32,    33,    94,    36,    50,    64,
      34,    45,     3,    42,    37,    38,    39,    42,    44,   116,
     100,    89,     6,    42,    40,    48,    46,    51,    41,    52,
      35,    54,    49,    55,    56,    60,    42,   125,    61,    62,
      40,    72,    79,   100,   146,   117,   100,    73,   101,    83,
       6,    40,    40,   127,    43,    72,    72,    42,    43,    47,
      73,    73,    73,    73,    43,   148,    91,    90,    40,   126,
      95,   101,   113,    63,   101,    16,    72,    43,   107,    55,
      56,    65,    73,   102,    85,    66,   102,   106,    86,   115,
      72,    87,    88,    73,    96,    79,    73,    73,    43,    73,
     114,    97,   123,    72,   119,    98,   121,    99,   103,    73,
       4,   104,   105,    73,   110,     5,     6,     7,     8,     9,
     128,   100,    73,   111,    16,   120,   141,   138,   142,   131,
      67,    68,    69,    70,    71,   112,   124,   147,   138,   130,
       5,     6,     7,     8,     9,   143,   140,    10,   145,   101,
     122,    11,   108,   109,     0,    12,    13,     0,   118,     0,
       0,   102,     0,     5,     6,     7,     8,     9,     0,   100,
       0,    14,    10,     0,   100,   133,    11,     0,     0,     0,
     134,    13,     0,    15,     5,     6,     7,     8,     9,     0,
      16,     0,    17,    18,    19,    10,   135,   101,   144,    11,
     132,   139,   101,   134,    13,     0,     0,   136,     0,   102,
       0,     0,     0,     0,   102,    16,    10,     0,     0,   135,
      11,     0,     0,     0,     0,    13,     0,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16
  };

  const short
  parser::yycheck_[] =
  {
       2,     6,    10,    11,    13,    14,    65,    16,    37,    37,
      62,    42,     0,    37,    17,    18,    19,    37,    38,    13,
      13,    45,     6,    37,    80,    34,    57,    39,    45,    85,
      82,    40,    35,    42,    42,    43,    37,    13,    46,    47,
      80,    50,    51,    13,    45,    39,    13,    50,    41,    52,
       6,    80,    80,   112,    78,    64,    65,    37,    78,    78,
      63,    64,    65,    66,    78,    45,    64,    63,    80,    45,
      66,    41,    42,    36,    41,    80,    85,    78,    86,    88,
      88,    37,    85,    53,    39,    41,    53,    85,    36,    97,
      99,    38,    57,    96,    36,   104,    99,   100,    78,   102,
      96,    78,   105,   112,   100,    38,   102,    57,    40,   112,
       0,    57,    57,   116,    38,     5,     6,     7,     8,     9,
     116,    13,   125,    38,    80,     9,   135,   129,   136,   125,
      86,    87,    88,    89,    90,    57,    40,   145,   140,    42,
       5,     6,     7,     8,     9,    45,   132,    37,    78,    41,
     104,    41,    88,    45,    -1,    45,    46,    -1,    99,    -1,
      -1,    53,    -1,     5,     6,     7,     8,     9,    -1,    13,
      -1,    61,    37,    -1,    13,    40,    41,    -1,    -1,    -1,
      45,    46,    -1,    73,     5,     6,     7,     8,     9,    -1,
      80,    -1,    82,    83,    84,    37,    61,    41,    40,    41,
      39,    45,    41,    45,    46,    -1,    -1,    72,    -1,    53,
      -1,    -1,    -1,    -1,    53,    80,    37,    -1,    -1,    61,
      41,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80
  };

  const signed char
  parser::yystos_[] =
  {
       0,    92,    93,     0,     0,     5,     6,     7,     8,     9,
      37,    41,    45,    46,    61,    73,    80,    82,    83,    84,
      94,    97,    98,    99,   100,   104,   114,   115,   114,   115,
     115,   116,    97,    97,    62,    82,    97,    98,    98,    98,
      80,    45,    37,    78,    38,    42,    57,    78,    97,    98,
      37,    39,    85,   105,    97,    97,   115,   117,   118,   119,
     115,   115,   115,    36,    37,    37,    41,    86,    87,    88,
      89,    90,    97,    98,   109,   110,   111,   112,   113,    97,
     101,   102,   103,    98,   106,    39,    36,    38,    57,    45,
     113,   110,   107,   108,   109,   113,    36,    78,    38,    57,
      13,    41,    53,    40,    57,    57,   110,   115,   119,    45,
      38,    38,    57,    42,   113,   115,    13,    39,   112,   113,
       9,   113,   103,    98,    40,    13,    45,   109,   113,    95,
      42,   113,    39,    40,    45,    61,    72,    96,   114,    45,
      95,    97,   115,    45,    40,    78,    45,   115,    45
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    91,    92,    93,    93,    93,    93,    93,    94,    94,
      94,    94,    94,    94,    95,    95,    95,    96,    96,    96,
      97,    98,    98,    98,    99,    99,   100,   101,   101,   102,
     102,   103,   104,   105,   105,   106,   106,   107,   107,   108,
     108,   109,   109,   110,   110,   111,   111,   112,   112,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     114,   114,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   116,   116,   116,   117,   117,   118,   118,   119,   119
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     2,     2,     6,     7,
       9,     1,     5,     2,     0,     2,     2,     5,     3,     2,
       1,     2,     1,     3,     8,    10,     5,     0,     1,     1,
       3,     1,     6,     0,     2,     1,     3,     0,     1,     1,
       3,     3,     1,     0,     1,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     3,     3,
       4,     3,     1,     1,     1,     2,     1,     1,     3,     3,
       1,     0,     1,     3,     0,     1,     1,     3,     3,     1
  };


#if BENG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "ARGIDENTIFIER", "DECIMAL",
  "INTEGER", "OPERATOR_TERM", "\"`_`\"", "\"`->*`\"", "\"`->`\"",
  "\"`~>`\"", "\"`.*`\"", "\"`++`\"", "\"`--`\"", "\"`<<`\"", "\"`>>`\"",
  "\"`<=`\"", "\"`>=`\"", "\"`==`\"", "\"`!=`\"", "\"`&&`\"", "\"`||`\"",
  "\"`+=`\"", "\"`-=`\"", "\"`*=`\"", "\"`/=`\"", "\"`%=`\"", "\"`<<=`\"",
  "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"", "\"`^=`\"", "\"`:`\"", "\"`(`\"",
  "\"`)`\"", "\"`{`\"", "\"`}`\"", "\"`[`\"", "\"`]`\"", "\"`<`\"",
  "\"`>`\"", "\"`;`\"", "\"`.`\"", "\"`+`\"", "\"`-`\"", "\"`*`\"",
  "\"`/`\"", "\"`%`\"", "\"`&`\"", "\"`|`\"", "\"`!`\"", "\"`~`\"",
  "\"`^`\"", "\",\"", "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET", "VAR", "NEW",
  "FOR", "VOID_", "AUTO", "CONST_", "USING", "THROW", "DELETE_", "SIZEOF",
  "RETURN", "EXTERN", "PRIVATE", "TEMPLATE", "NAMESPACE", "LOWEST",
  "ASSIGN", "\"`=`\"", "DBLCOLON", "\"`::`\"", "FN", "ENUM", "TYPE",
  "EXTENDS", "BOOL", "INT", "FLOAT", "STRING_WORD", "DECIMAL_WORD",
  "$accept", "begin", "declaration_any", "generic-decl",
  "infunction_declaration_any", "opt-infunction-decl", "identifier",
  "qname", "fn-decl", "enum-decl", "case-list-opt", "case-list",
  "case-decl", "type-decl", "type-extension-any", "type-extension-list",
  "parameter-list-opt", "parameter-list", "parameter-decl",
  "parameter-woa-list-opt", "parameter-woa-list", "parameter-woa-decl",
  "type-expr", "compound-expression", "expression", "expression-list-any",
  "opt-named-expr-list-any", "opt-named-expr-list", "opt-named-expr", YY_NULLPTR
  };
#endif


#if BENG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   277,   277,   281,   283,   285,   287,   289,   309,   311,
     313,   315,   317,   319,   333,   335,   337,   345,   347,   349,
     354,   363,   365,   367,   373,   377,   384,   391,   393,   397,
     400,   405,   410,   417,   419,   423,   426,   431,   432,   435,
     437,   441,   443,   450,   451,   454,   456,   460,   462,   468,
     469,   470,   471,   472,   473,   474,   476,   478,   480,   487,
     504,   506,   511,   513,   515,   517,   519,   521,   523,   525,
     527,   537,   539,   541,   546,   548,   552,   554,   562,   564
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
#endif // BENG_LANGDEBUG

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

} // beng_lang
#line 2753 "beng.tab.cpp"

#line 892 "beng.y"

