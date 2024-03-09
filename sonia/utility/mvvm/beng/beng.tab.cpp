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
      case symbol_kind::S_108_type_expr: // type-expr
        value.copy< beng_generic_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
        value.copy< beng_tuple_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
        value.copy< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
      case symbol_kind::S_104_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
        value.copy< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_106_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::beng::qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< std::vector<declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
      case symbol_kind::S_100_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_type_decl: // type-decl
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
      case symbol_kind::S_108_type_expr: // type-expr
        value.move< beng_generic_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
        value.move< beng_tuple_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
        value.move< declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
      case symbol_kind::S_104_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_106_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_101_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< std::vector<declaration_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
      case symbol_kind::S_100_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_102_type_decl: // type-decl
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
      case symbol_kind::S_108_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< beng_generic_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
        value.YY_MOVE_OR_COPY< beng_tuple_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
        value.YY_MOVE_OR_COPY< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
      case symbol_kind::S_104_type_extension_list: // type-extension-list
        value.YY_MOVE_OR_COPY< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
        value.YY_MOVE_OR_COPY< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_106_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.YY_MOVE_OR_COPY< sonia::decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.YY_MOVE_OR_COPY< sonia::lang::beng::qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< std::vector<declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.YY_MOVE_OR_COPY< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
      case symbol_kind::S_100_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_type_decl: // type-decl
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
      case symbol_kind::S_108_type_expr: // type-expr
        value.move< beng_generic_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
        value.move< beng_tuple_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
        value.move< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
      case symbol_kind::S_104_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_106_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< std::vector<declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
      case symbol_kind::S_100_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_type_decl: // type-decl
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
      case symbol_kind::S_108_type_expr: // type-expr
        value.copy< beng_generic_type > (that.value);
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
        value.copy< beng_tuple_t > (that.value);
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
        value.copy< declaration_t > (that.value);
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (that.value);
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
      case symbol_kind::S_104_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (that.value);
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
        value.copy< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_106_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::beng::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< sonia::lang::beng::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::beng::qname > (that.value);
        break;

      case symbol_kind::S_101_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< std::vector<declaration_t> > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.copy< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
      case symbol_kind::S_100_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_102_type_decl: // type-decl
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
      case symbol_kind::S_108_type_expr: // type-expr
        value.move< beng_generic_type > (that.value);
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
        value.move< beng_tuple_t > (that.value);
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
        value.move< declaration_t > (that.value);
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (that.value);
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
      case symbol_kind::S_104_type_extension_list: // type-extension-list
        value.move< extension_list_t > (that.value);
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
        value.move< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_106_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::qname > (that.value);
        break;

      case symbol_kind::S_101_case_decl: // case-decl
        value.move< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< std::vector<declaration_t> > (that.value);
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (that.value);
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
      case symbol_kind::S_100_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_102_type_decl: // type-decl
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
#line 265 "beng.y"
                 { }
#line 1025 "beng.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 265 "beng.y"
                 { }
#line 1031 "beng.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 265 "beng.y"
                 { }
#line 1037 "beng.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 265 "beng.y"
                 { }
#line 1043 "beng.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 265 "beng.y"
                 { }
#line 1049 "beng.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 265 "beng.y"
                 { }
#line 1055 "beng.tab.cpp"
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
#line 265 "beng.y"
                 { }
#line 1061 "beng.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 265 "beng.y"
                 { }
#line 1067 "beng.tab.cpp"
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
#line 265 "beng.y"
                 { }
#line 1073 "beng.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 265 "beng.y"
                 { }
#line 1079 "beng.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 265 "beng.y"
                 { }
#line 1085 "beng.tab.cpp"
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
#line 265 "beng.y"
                 { }
#line 1091 "beng.tab.cpp"
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
#line 265 "beng.y"
                 { }
#line 1097 "beng.tab.cpp"
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
#line 265 "beng.y"
                 { }
#line 1103 "beng.tab.cpp"
        break;

      case symbol_kind::S_100_case_list: // case-list
#line 265 "beng.y"
                 { }
#line 1109 "beng.tab.cpp"
        break;

      case symbol_kind::S_101_case_decl: // case-decl
#line 265 "beng.y"
                 { }
#line 1115 "beng.tab.cpp"
        break;

      case symbol_kind::S_102_type_decl: // type-decl
#line 265 "beng.y"
                 { }
#line 1121 "beng.tab.cpp"
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
#line 265 "beng.y"
                 { }
#line 1127 "beng.tab.cpp"
        break;

      case symbol_kind::S_104_type_extension_list: // type-extension-list
#line 265 "beng.y"
                 { }
#line 1133 "beng.tab.cpp"
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
#line 265 "beng.y"
                 { }
#line 1139 "beng.tab.cpp"
        break;

      case symbol_kind::S_106_parameter_list: // parameter-list
#line 265 "beng.y"
                 { }
#line 1145 "beng.tab.cpp"
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
#line 265 "beng.y"
                 { }
#line 1151 "beng.tab.cpp"
        break;

      case symbol_kind::S_108_type_expr: // type-expr
#line 265 "beng.y"
                 { }
#line 1157 "beng.tab.cpp"
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
#line 265 "beng.y"
                 { }
#line 1163 "beng.tab.cpp"
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
#line 265 "beng.y"
                 { }
#line 1169 "beng.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 265 "beng.y"
                 { }
#line 1175 "beng.tab.cpp"
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
#line 265 "beng.y"
                 { }
#line 1181 "beng.tab.cpp"
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
#line 265 "beng.y"
                 { }
#line 1187 "beng.tab.cpp"
        break;

      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
#line 265 "beng.y"
                 { }
#line 1193 "beng.tab.cpp"
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
#line 265 "beng.y"
                 { }
#line 1199 "beng.tab.cpp"
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
      case symbol_kind::S_108_type_expr: // type-expr
        yylhs.value.emplace< beng_generic_type > ();
        break;

      case symbol_kind::S_109_opt_type_list: // opt-type-list
        yylhs.value.emplace< beng_tuple_t > ();
        break;

      case symbol_kind::S_92_opt_decl: // opt-decl
        yylhs.value.emplace< declaration_t > ();
        break;

      case symbol_kind::S_98_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_112_expression_list_any: // expression-list-any
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_110_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< expression_t > ();
        break;

      case symbol_kind::S_103_type_extension_any: // type-extension-any
      case symbol_kind::S_104_type_extension_list: // type-extension-list
        yylhs.value.emplace< extension_list_t > ();
        break;

      case symbol_kind::S_97_fn_decl: // fn-decl
        yylhs.value.emplace< fn_decl_t > ();
        break;

      case symbol_kind::S_94_opt_infunction_decl: // opt-infunction-decl
        yylhs.value.emplace< infunction_declaration_t > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_113_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_114_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_115_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_105_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_106_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_107_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        yylhs.value.emplace< sonia::decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
        yylhs.value.emplace< sonia::lang::beng::annotated_identifier > ();
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        yylhs.value.emplace< sonia::lang::beng::annotated_string_view > ();
        break;

      case symbol_kind::S_qname: // qname
        yylhs.value.emplace< sonia::lang::beng::qname > ();
        break;

      case symbol_kind::S_101_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::identifier > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< std::vector<declaration_t> > ();
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        yylhs.value.emplace< std::vector<infunction_declaration_t> > ();
        break;

      case symbol_kind::S_99_case_list_opt: // case-list-opt
      case symbol_kind::S_100_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::identifier> > ();
        break;

      case symbol_kind::S_102_type_decl: // type-decl
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
#line 270 "beng.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < std::vector<declaration_t> > ())); }
#line 1553 "beng.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 275 "beng.y"
        { yylhs.value.as < std::vector<declaration_t> > () = {}; }
#line 1559 "beng.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any opt-decl
#line 278 "beng.y"
        {
	        yylhs.value.as < std::vector<declaration_t> > () = std::move(yystack_[1].value.as < std::vector<declaration_t> > ());
            if (yystack_[0].value.as < declaration_t > ().which()) yylhs.value.as < std::vector<declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < declaration_t > ()));
        }
#line 1568 "beng.tab.cpp"
    break;

  case 5: // opt-decl: "`;`"
#line 298 "beng.y"
        { yylhs.value.as < declaration_t > () = empty_t{}; }
#line 1574 "beng.tab.cpp"
    break;

  case 6: // opt-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 300 "beng.y"
        { yylhs.value.as < declaration_t > () = exten_var{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < beng_generic_type > ()) }; }
#line 1580 "beng.tab.cpp"
    break;

  case 7: // opt-decl: EXTERN FN qname "`(`" parameter-list-opt "`)`" "`;`"
#line 302 "beng.y"
        { yystack_[4].value.as < sonia::lang::beng::qname > ().set_absolute(); yylhs.value.as < declaration_t > () = fn_pure_decl{ std::move(yystack_[4].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), beng_tuple_t{} }; }
#line 1586 "beng.tab.cpp"
    break;

  case 8: // opt-decl: EXTERN FN qname "`(`" parameter-list-opt "`)`" "`->`" type-expr "`;`"
#line 304 "beng.y"
        { yystack_[6].value.as < sonia::lang::beng::qname > ().set_absolute(); yylhs.value.as < declaration_t > () = fn_pure_decl{ std::move(yystack_[6].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[1].value.as < beng_generic_type > ()) }; }
#line 1592 "beng.tab.cpp"
    break;

  case 9: // opt-decl: enum-decl
#line 306 "beng.y"
        { yylhs.value.as < declaration_t > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 1598 "beng.tab.cpp"
    break;

  case 10: // opt-decl: type-decl
#line 308 "beng.y"
        { yylhs.value.as < declaration_t > () = std::move(yystack_[0].value.as < type_decl > ()); }
#line 1604 "beng.tab.cpp"
    break;

  case 11: // opt-decl: fn-decl
#line 310 "beng.y"
        { yylhs.value.as < declaration_t > () = std::move(yystack_[0].value.as < fn_decl_t > ()); }
#line 1610 "beng.tab.cpp"
    break;

  case 12: // opt-decl: LET identifier ASSIGN expression "`;`"
#line 312 "beng.y"
        { yylhs.value.as < declaration_t > () = let_statement_decl{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1616 "beng.tab.cpp"
    break;

  case 13: // opt-decl: compound-expression "`;`"
#line 314 "beng.y"
        { yylhs.value.as < declaration_t > () = expression_decl{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1622 "beng.tab.cpp"
    break;

  case 14: // infunction_declaration_any: %empty
#line 328 "beng.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 1628 "beng.tab.cpp"
    break;

  case 15: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 330 "beng.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            if (yystack_[0].value.as < infunction_declaration_t > ().which()) yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 1637 "beng.tab.cpp"
    break;

  case 16: // opt-infunction-decl: "`;`"
#line 338 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = empty_t{}; }
#line 1643 "beng.tab.cpp"
    break;

  case 17: // opt-infunction-decl: LET identifier ASSIGN expression "`;`"
#line 340 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = let_statement_decl{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1649 "beng.tab.cpp"
    break;

  case 18: // opt-infunction-decl: compound-expression "`;`"
#line 342 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1655 "beng.tab.cpp"
    break;

  case 19: // identifier: IDENTIFIER
#line 347 "beng.y"
      { yylhs.value.as < sonia::lang::beng::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::beng::annotated_string_view > ()); }
#line 1661 "beng.tab.cpp"
    break;

  case 20: // qname: DBLCOLON identifier
#line 356 "beng.y"
        { yylhs.value.as < sonia::lang::beng::qname > () = sonia::lang::qname(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id); }
#line 1667 "beng.tab.cpp"
    break;

  case 21: // qname: identifier
#line 358 "beng.y"
        { yylhs.value.as < sonia::lang::beng::qname > () = sonia::lang::qname(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id); yylhs.value.as < sonia::lang::beng::qname > ().set_relative(); }
#line 1673 "beng.tab.cpp"
    break;

  case 22: // qname: qname DBLCOLON identifier
#line 360 "beng.y"
        { yylhs.value.as < sonia::lang::beng::qname > () = std::move(yystack_[2].value.as < sonia::lang::beng::qname > ()); yylhs.value.as < sonia::lang::beng::qname > ().append(std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id)); }
#line 1679 "beng.tab.cpp"
    break;

  case 23: // fn-decl: FN qname "`(`" parameter-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 366 "beng.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl{ctx.ns() + std::move(yystack_[6].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), beng_tuple_t{}, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())};
            }
#line 1687 "beng.tab.cpp"
    break;

  case 24: // fn-decl: FN qname "`(`" parameter-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 370 "beng.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl{ctx.ns() + std::move(yystack_[8].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[6].value.as < parameter_list_t > ()), std::move(yystack_[3].value.as < beng_generic_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())};
            }
#line 1695 "beng.tab.cpp"
    break;

  case 25: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 377 "beng.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.ns() + std::move(yystack_[3].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::identifier> > ())};
    }
#line 1703 "beng.tab.cpp"
    break;

  case 26: // case-list-opt: %empty
#line 383 "beng.y"
           { yylhs.value.as < std::vector<sonia::lang::identifier> > () = {}; }
#line 1709 "beng.tab.cpp"
    break;

  case 27: // case-list-opt: case-list
#line 385 "beng.y"
    { yylhs.value.as < std::vector<sonia::lang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::identifier> > (); }
#line 1715 "beng.tab.cpp"
    break;

  case 28: // case-list: case-decl
#line 390 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::vector<sonia::lang::identifier>{std::move(yystack_[0].value.as < sonia::lang::identifier > ())}; }
#line 1721 "beng.tab.cpp"
    break;

  case 29: // case-list: case-list "," case-decl
#line 393 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::identifier > ())); }
#line 1727 "beng.tab.cpp"
    break;

  case 30: // case-decl: identifier
#line 398 "beng.y"
        { yylhs.value.as < sonia::lang::identifier > () = yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id; }
#line 1733 "beng.tab.cpp"
    break;

  case 31: // type-decl: TYPE qname type-extension-any "`{`" parameter-list-opt "`}`"
#line 403 "beng.y"
    {
        yylhs.value.as < type_decl > () = type_decl{ctx.ns() + std::move(yystack_[4].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[3].value.as < extension_list_t > ()), std::move(yystack_[1].value.as < parameter_list_t > ())};
    }
#line 1741 "beng.tab.cpp"
    break;

  case 32: // type-extension-any: %empty
#line 409 "beng.y"
           { yylhs.value.as < extension_list_t > () = {}; }
#line 1747 "beng.tab.cpp"
    break;

  case 33: // type-extension-any: EXTENDS type-extension-list
#line 411 "beng.y"
                                { yylhs.value.as < extension_list_t > () = std::move(yystack_[0].value.as < extension_list_t > ()); }
#line 1753 "beng.tab.cpp"
    break;

  case 34: // type-extension-list: qname
#line 416 "beng.y"
        { yylhs.value.as < extension_list_t > () = extension_list_t{std::move(yystack_[0].value.as < sonia::lang::beng::qname > ())}; }
#line 1759 "beng.tab.cpp"
    break;

  case 35: // type-extension-list: type-extension-list "," qname
#line 419 "beng.y"
        { yylhs.value.as < extension_list_t > () = std::move(yystack_[2].value.as < extension_list_t > ()); yylhs.value.as < extension_list_t > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::beng::qname > ())); }
#line 1765 "beng.tab.cpp"
    break;

  case 36: // parameter-list-opt: %empty
#line 423 "beng.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 1771 "beng.tab.cpp"
    break;

  case 37: // parameter-list-opt: parameter-list
#line 424 "beng.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 1777 "beng.tab.cpp"
    break;

  case 38: // parameter-list: parameter-decl
#line 429 "beng.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{std::move(yystack_[0].value.as < parameter_t > ())}; }
#line 1783 "beng.tab.cpp"
    break;

  case 39: // parameter-list: parameter-list "," parameter-decl
#line 431 "beng.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 1789 "beng.tab.cpp"
    break;

  case 40: // parameter-decl: identifier "`:`" type-expr
#line 436 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[0].value.as < beng_generic_type > ()) }; }
#line 1795 "beng.tab.cpp"
    break;

  case 41: // parameter-decl: identifier "`:`" type-expr ASSIGN expression
#line 438 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[4].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[2].value.as < beng_generic_type > ()), std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 1801 "beng.tab.cpp"
    break;

  case 42: // parameter-decl: type-expr
#line 440 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ sonia::lang::identifier{}, std::move(yystack_[0].value.as < beng_generic_type > ()) }; }
#line 1807 "beng.tab.cpp"
    break;

  case 43: // parameter-decl: type-expr ASSIGN expression
#line 442 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ sonia::lang::identifier{}, std::move(yystack_[2].value.as < beng_generic_type > ()), std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 1813 "beng.tab.cpp"
    break;

  case 44: // type-expr: BOOL
#line 450 "beng.y"
           { yylhs.value.as < beng_generic_type > () = beng_bool_t{}; }
#line 1819 "beng.tab.cpp"
    break;

  case 45: // type-expr: INT
#line 451 "beng.y"
          { yylhs.value.as < beng_generic_type > () = beng_int_t{}; }
#line 1825 "beng.tab.cpp"
    break;

  case 46: // type-expr: FLOAT
#line 452 "beng.y"
            { yylhs.value.as < beng_generic_type > () = beng_float_t{}; }
#line 1831 "beng.tab.cpp"
    break;

  case 47: // type-expr: STRING_WORD
#line 453 "beng.y"
                  { yylhs.value.as < beng_generic_type > () = beng_string_t{}; }
#line 1837 "beng.tab.cpp"
    break;

  case 48: // type-expr: qname
#line 454 "beng.y"
            { yylhs.value.as < beng_generic_type > () = beng_object_t{std::move(yystack_[0].value.as < sonia::lang::beng::qname > ())}; }
#line 1843 "beng.tab.cpp"
    break;

  case 49: // type-expr: "`[`" type-expr "`]`"
#line 456 "beng.y"
        { yylhs.value.as < beng_generic_type > () = beng_vector_t{std::move(yystack_[1].value.as < beng_generic_type > ())}; }
#line 1849 "beng.tab.cpp"
    break;

  case 50: // type-expr: "`(`" opt-type-list "`)`"
#line 458 "beng.y"
        { yylhs.value.as < beng_generic_type > () = std::move(yystack_[1].value.as < beng_tuple_t > ()); }
#line 1855 "beng.tab.cpp"
    break;

  case 51: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 460 "beng.y"
        { yylhs.value.as < beng_generic_type > () = beng_array_t{std::move(yystack_[3].value.as < beng_generic_type > ()), (size_t)std::move(yystack_[1].value.as < int > ())}; }
#line 1861 "beng.tab.cpp"
    break;

  case 52: // type-expr: type-expr "`||`" type-expr
#line 462 "beng.y"
        {
            beng_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < beng_generic_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < beng_generic_type > ()));
            yylhs.value.as < beng_generic_type > () = std::move(uni);
        }
#line 1872 "beng.tab.cpp"
    break;

  case 53: // type-expr: type-expr "`->`" type-expr
#line 469 "beng.y"
        { yylhs.value.as < beng_generic_type > () = beng_fn_t{std::move(yystack_[2].value.as < beng_generic_type > ()), std::move(yystack_[0].value.as < beng_generic_type > ())}; }
#line 1878 "beng.tab.cpp"
    break;

  case 54: // opt-type-list: %empty
#line 474 "beng.y"
        { yylhs.value.as < beng_tuple_t > () = beng_tuple_t{}; }
#line 1884 "beng.tab.cpp"
    break;

  case 55: // opt-type-list: type-expr
#line 476 "beng.y"
        { yylhs.value.as < beng_tuple_t > () = beng_tuple_t{}; yylhs.value.as < beng_tuple_t > ().fields.emplace_back(std::move(yystack_[0].value.as < beng_generic_type > ())); }
#line 1890 "beng.tab.cpp"
    break;

  case 56: // opt-type-list: opt-type-list "," type-expr
#line 478 "beng.y"
        { yylhs.value.as < beng_tuple_t > () = std::move(yystack_[2].value.as < beng_tuple_t > ()); yylhs.value.as < beng_tuple_t > ().fields.emplace_back(std::move(yystack_[0].value.as < beng_generic_type > ())); }
#line 1896 "beng.tab.cpp"
    break;

  case 57: // compound-expression: qname "`(`" opt-named-expr-list-any "`)`"
#line 487 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { function_call_t{ std::move(yystack_[3].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) } }; }
#line 1902 "beng.tab.cpp"
    break;

  case 58: // compound-expression: expression ASSIGN expression
#line 489 "beng.y"
        { yylhs.value.as < expression_t > () = assign_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 1908 "beng.tab.cpp"
    break;

  case 59: // expression: INTEGER
#line 494 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { decimal(yystack_[0].value.as < int > ()) }; }
#line 1914 "beng.tab.cpp"
    break;

  case 60: // expression: DECIMAL
#line 496 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { std::move(yystack_[0].value.as < sonia::decimal > ()) }; }
#line 1920 "beng.tab.cpp"
    break;

  case 61: // expression: STRING
#line 498 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { ctx.make_string(yystack_[0].value.as < sonia::string_view > ()) }; }
#line 1926 "beng.tab.cpp"
    break;

  case 62: // expression: "`.`" identifier
#line 500 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { case_expression { std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id) } }; }
#line 1932 "beng.tab.cpp"
    break;

  case 63: // expression: qname
#line 502 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { std::move(yystack_[0].value.as < sonia::lang::beng::qname > ()) }; }
#line 1938 "beng.tab.cpp"
    break;

  case 64: // expression: "`(`" expression "`)`"
#line 504 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1944 "beng.tab.cpp"
    break;

  case 65: // expression: "`[`" expression-list-any "`]`"
#line 506 "beng.y"
        { yylhs.value.as < expression_t > () = expression_vector_t{ {std::move(yystack_[1].value.as < expression_list_t > ())} }; }
#line 1950 "beng.tab.cpp"
    break;

  case 66: // expression: compound-expression
#line 508 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 1956 "beng.tab.cpp"
    break;

  case 67: // expression-list-any: %empty
#line 518 "beng.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{}; }
#line 1962 "beng.tab.cpp"
    break;

  case 68: // expression-list-any: expression
#line 520 "beng.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 1968 "beng.tab.cpp"
    break;

  case 69: // expression-list-any: expression-list-any "," expression
#line 522 "beng.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < expression_t > ())); }
#line 1974 "beng.tab.cpp"
    break;

  case 70: // opt-named-expr-list-any: %empty
#line 527 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 1980 "beng.tab.cpp"
    break;

  case 71: // opt-named-expr-list-any: opt-named-expr-list
#line 528 "beng.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 1986 "beng.tab.cpp"
    break;

  case 72: // opt-named-expr-list: opt-named-expr
#line 533 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 1992 "beng.tab.cpp"
    break;

  case 73: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 535 "beng.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2001 "beng.tab.cpp"
    break;

  case 74: // opt-named-expr: identifier "`:`" expression
#line 543 "beng.y"
        { yylhs.value.as < named_expression_term_t > () = std::tuple{std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[0].value.as < expression_t > ())}; }
#line 2007 "beng.tab.cpp"
    break;

  case 75: // opt-named-expr: expression
#line 545 "beng.y"
        { yylhs.value.as < named_expression_term_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 2013 "beng.tab.cpp"
    break;


#line 2017 "beng.tab.cpp"

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


  const signed char parser::yypact_ninf_ = -53;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -53,    22,    19,   -53,   -53,   -53,   -53,   -53,   -53,     5,
       5,   -53,    17,    17,   -52,    17,    -3,    -3,    -3,   -53,
     -53,   -28,   -53,   -53,   -53,   -14,   -21,   -53,   -33,   -21,
      48,   -53,    -9,    17,    -3,   -53,   -22,   -18,   -12,     5,
      17,   -53,     5,   -53,   -53,     5,     5,    57,    -1,    33,
      17,    -3,    49,    70,   -21,    73,    61,   -53,   -53,   -21,
     -21,   -37,    33,    33,    33,    33,   -53,   -53,   -53,   -53,
      87,    68,   112,    82,   -53,    -7,   -53,   114,    98,   -53,
      68,   102,    33,     5,   -53,     5,   -53,   131,   127,    59,
      25,   145,    33,    -6,    33,    33,    33,   152,     5,   -53,
      17,    -3,   134,   -21,   -53,   -53,    16,   -53,    33,   -53,
      34,    33,   -53,   -53,   -53,    74,   125,   -21,   -53,    68,
     -53,    33,   -53,    59,     5,   153,   101,   -53,   132,   -21,
     -53,   -53,   -53,    17,   -53,   126,   -53,   123,    97,   -53,
     -53,     5,   -23,   -53
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    61,    19,    60,    59,     0,
      67,     5,     0,     0,     0,     0,     0,     0,     0,     4,
      21,    63,    11,     9,    10,    66,     0,    66,     0,    68,
       0,    62,     0,     0,     0,    20,     0,     0,    32,    70,
       0,    13,     0,    64,    65,     0,     0,     0,     0,    36,
      26,     0,     0,    21,    75,     0,    71,    72,    22,    58,
      69,     0,     0,    36,    54,     0,    44,    45,    46,    47,
      21,    48,     0,    37,    38,    42,    30,     0,    27,    28,
      34,    33,    36,     0,    57,     0,    12,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,    74,    73,     6,     0,    50,     0,    49,
      40,     0,    14,    39,    53,    52,     0,    43,    29,    35,
      31,     0,     7,    56,     0,     0,     0,    51,     0,    41,
      14,    23,    16,     0,    15,    66,     8,     0,     0,    18,
      24,     0,     0,    17
  };

  const signed char
  parser::yypgoto_[] =
  {
     -53,   -53,   -53,   -53,    54,   -53,     3,    31,   -53,   -53,
     -53,   -53,    79,   -53,   -53,   -53,    52,   -53,    93,    86,
     -53,    -2,    -8,   -53,   -53,   -53,   103
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    19,   126,   134,    20,    21,    22,    23,
      77,    78,    79,    24,    52,    81,    72,    73,    74,    75,
      90,    27,    26,    30,    55,    56,    57
  };

  const unsigned char
  parser::yytable_[] =
  {
      25,    28,    29,     6,    43,    95,   111,    86,    39,    33,
       5,     6,     7,     8,    49,    31,    32,    96,    35,     4,
      50,   143,     3,     6,     5,     6,     7,     8,   121,    34,
      41,    54,   112,    97,    59,    63,    47,    60,    61,     6,
      42,     9,    53,    58,    42,    10,    95,    36,    37,    38,
      12,    40,    70,    76,    42,     9,    42,    40,    96,    10,
     122,    40,   107,    11,    12,    48,    70,    40,    46,    64,
      98,    95,    51,    65,    97,   103,    15,    54,    40,    13,
      71,   108,    80,    96,    15,    70,    95,    82,    53,    44,
     117,    14,    62,    71,    71,    71,    71,    70,    15,    97,
      16,    17,    18,    76,    45,    83,     5,     6,     7,     8,
      84,   124,    15,    71,    97,    88,   129,    85,    66,    67,
      68,    69,    92,    71,   135,    71,    71,    71,     5,     6,
       7,     8,   119,   142,   102,   135,   138,     9,    94,    71,
     131,    10,    71,    95,    95,   132,    12,    40,    87,    93,
      89,    91,    71,    99,   100,    96,    96,    95,   101,     9,
     116,   133,   140,    10,   106,    95,   127,   132,    12,    96,
     139,    97,    97,   120,   141,   105,   136,    96,   110,   118,
      15,   114,   115,   133,   137,    97,   109,   113,   104,     0,
       0,   130,     0,    97,   123,     0,     0,   125,     0,     0,
       0,     0,    15,     0,     0,     0,     0,   128
  };

  const short
  parser::yycheck_[] =
  {
       2,     9,    10,     6,    37,    12,    12,    44,    36,    61,
       5,     6,     7,     8,    36,    12,    13,    24,    15,     0,
      38,    44,     0,     6,     5,     6,     7,     8,    12,    81,
      44,    39,    38,    40,    42,    36,    33,    45,    46,     6,
      77,    36,    39,    40,    77,    40,    12,    16,    17,    18,
      45,    79,    49,    50,    77,    36,    77,    79,    24,    40,
      44,    79,    37,    44,    45,    34,    63,    79,    77,    36,
      77,    12,    84,    40,    40,    83,    79,    85,    79,    60,
      49,    56,    51,    24,    79,    82,    12,    38,    85,    41,
      98,    72,    35,    62,    63,    64,    65,    94,    79,    40,
      81,    82,    83,   100,    56,    35,     5,     6,     7,     8,
      37,    77,    79,    82,    40,    63,   124,    56,    85,    86,
      87,    88,    35,    92,   126,    94,    95,    96,     5,     6,
       7,     8,   101,   141,    82,   137,   133,    36,    56,   108,
      39,    40,   111,    12,    12,    44,    45,    79,    62,    37,
      64,    65,   121,    39,    56,    24,    24,    12,    56,    36,
       8,    60,    39,    40,    37,    12,    41,    44,    45,    24,
      44,    40,    40,    39,    77,    44,    44,    24,    92,   100,
      79,    95,    96,    60,   130,    40,    41,    94,    85,    -1,
      -1,    38,    -1,    40,   108,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,   121
  };

  const signed char
  parser::yystos_[] =
  {
       0,    90,    91,     0,     0,     5,     6,     7,     8,    36,
      40,    44,    45,    60,    72,    79,    81,    82,    83,    92,
      95,    96,    97,    98,   102,   110,   111,   110,   111,   111,
     112,    95,    95,    61,    81,    95,    96,    96,    96,    36,
      79,    44,    77,    37,    41,    56,    77,    95,    96,    36,
      38,    84,   103,    95,   111,   113,   114,   115,    95,   111,
     111,   111,    35,    36,    36,    40,    85,    86,    87,    88,
      95,    96,   105,   106,   107,   108,    95,    99,   100,   101,
      96,   104,    38,    35,    37,    56,    44,   108,   105,   108,
     109,   108,    35,    37,    56,    12,    24,    40,    77,    39,
      56,    56,   105,   111,   115,    44,    37,    37,    56,    41,
     108,    12,    38,   107,   108,   108,     8,   111,   101,    96,
      39,    12,    44,   108,    77,   108,    93,    41,   108,   111,
      38,    39,    44,    60,    94,   110,    44,    93,    95,    44,
      39,    77,   111,    44
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    89,    90,    91,    91,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    93,    93,    94,    94,    94,    95,
      96,    96,    96,    97,    97,    98,    99,    99,   100,   100,
     101,   102,   103,   103,   104,   104,   105,   105,   106,   106,
     107,   107,   107,   107,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   109,   109,   109,   110,   110,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   112,
     113,   113,   114,   114,   115,   115
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     1,     6,     7,     9,     1,
       1,     1,     5,     2,     0,     2,     1,     5,     2,     1,
       2,     1,     3,     8,    10,     5,     0,     1,     1,     3,
       1,     6,     0,     2,     1,     3,     0,     1,     1,     3,
       3,     5,     1,     3,     1,     1,     1,     1,     1,     3,
       3,     4,     3,     3,     0,     1,     3,     4,     3,     1,
       1,     1,     2,     1,     3,     3,     1,     0,     1,     3,
       0,     1,     1,     3,     3,     1
  };


#if BENG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "DECIMAL", "INTEGER",
  "OPERATOR_TERM", "\"`_`\"", "\"`->*`\"", "\"`->`\"", "\"`~>`\"",
  "\"`.*`\"", "\"`++`\"", "\"`--`\"", "\"`<<`\"", "\"`>>`\"", "\"`<=`\"",
  "\"`>=`\"", "\"`==`\"", "\"`!=`\"", "\"`&&`\"", "\"`||`\"", "\"`+=`\"",
  "\"`-=`\"", "\"`*=`\"", "\"`/=`\"", "\"`%=`\"", "\"`<<=`\"", "\"`>>=`\"",
  "\"`&=`\"", "\"`|=`\"", "\"`^=`\"", "\"`:`\"", "\"`(`\"", "\"`)`\"",
  "\"`{`\"", "\"`}`\"", "\"`[`\"", "\"`]`\"", "\"`<`\"", "\"`>`\"",
  "\"`;`\"", "\"`.`\"", "\"`+`\"", "\"`-`\"", "\"`*`\"", "\"`/`\"",
  "\"`%`\"", "\"`&`\"", "\"`|`\"", "\"`!`\"", "\"`~`\"", "\"`^`\"",
  "\",\"", "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET", "VAR", "NEW", "FOR",
  "VOID_", "AUTO", "CONST_", "USING", "THROW", "DELETE_", "SIZEOF",
  "RETURN", "EXTERN", "PRIVATE", "TEMPLATE", "NAMESPACE", "LOWEST",
  "ASSIGN", "\"`=`\"", "DBLCOLON", "\"`::`\"", "FN", "ENUM", "TYPE",
  "EXTENDS", "BOOL", "INT", "FLOAT", "STRING_WORD", "$accept", "begin",
  "declaration_any", "opt-decl", "infunction_declaration_any",
  "opt-infunction-decl", "identifier", "qname", "fn-decl", "enum-decl",
  "case-list-opt", "case-list", "case-decl", "type-decl",
  "type-extension-any", "type-extension-list", "parameter-list-opt",
  "parameter-list", "parameter-decl", "type-expr", "opt-type-list",
  "compound-expression", "expression", "expression-list-any",
  "opt-named-expr-list-any", "opt-named-expr-list", "opt-named-expr", YY_NULLPTR
  };
#endif


#if BENG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   270,   270,   274,   277,   297,   299,   301,   303,   305,
     307,   309,   311,   313,   327,   329,   337,   339,   341,   346,
     355,   357,   359,   365,   369,   376,   383,   385,   389,   392,
     397,   402,   409,   411,   415,   418,   423,   424,   428,   430,
     435,   437,   439,   441,   450,   451,   452,   453,   454,   455,
     457,   459,   461,   468,   473,   475,   477,   486,   488,   493,
     495,   497,   499,   501,   503,   505,   507,   517,   519,   521,
     526,   528,   532,   534,   542,   544
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
      85,    86,    87,    88
    };
    // Last valid token kind.
    const int code_max = 343;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // beng_lang
#line 2658 "beng.tab.cpp"

#line 913 "beng.y"

