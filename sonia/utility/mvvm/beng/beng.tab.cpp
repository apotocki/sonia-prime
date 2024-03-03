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
      case symbol_kind::S_104_type_expr: // type-expr
        value.copy< beng_generic_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
      case symbol_kind::S_94_enum_decl: // enum-decl
      case symbol_kind::S_98_type_decl: // type-decl
        value.copy< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
      case symbol_kind::S_100_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_102_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
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

      case symbol_kind::S_97_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< std::vector<declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
      case symbol_kind::S_96_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
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
      case symbol_kind::S_104_type_expr: // type-expr
        value.move< beng_generic_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
      case symbol_kind::S_94_enum_decl: // enum-decl
      case symbol_kind::S_98_type_decl: // type-decl
        value.move< declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
      case symbol_kind::S_100_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_102_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
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

      case symbol_kind::S_97_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< std::vector<declaration_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
      case symbol_kind::S_96_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (s.value));
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
      case symbol_kind::S_104_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< beng_generic_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
      case symbol_kind::S_94_enum_decl: // enum-decl
      case symbol_kind::S_98_type_decl: // type-decl
        value.YY_MOVE_OR_COPY< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
      case symbol_kind::S_100_type_extension_list: // type-extension-list
        value.YY_MOVE_OR_COPY< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_102_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
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

      case symbol_kind::S_97_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< std::vector<declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
      case symbol_kind::S_96_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
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
      case symbol_kind::S_104_type_expr: // type-expr
        value.move< beng_generic_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
      case symbol_kind::S_94_enum_decl: // enum-decl
      case symbol_kind::S_98_type_decl: // type-decl
        value.move< declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
      case symbol_kind::S_100_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_102_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
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

      case symbol_kind::S_97_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< std::vector<declaration_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
      case symbol_kind::S_96_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
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
      case symbol_kind::S_104_type_expr: // type-expr
        value.copy< beng_generic_type > (that.value);
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
      case symbol_kind::S_94_enum_decl: // enum-decl
      case symbol_kind::S_98_type_decl: // type-decl
        value.copy< declaration_t > (that.value);
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (that.value);
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
      case symbol_kind::S_100_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_102_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
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

      case symbol_kind::S_97_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< std::vector<declaration_t> > (that.value);
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
      case symbol_kind::S_96_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (that.value);
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
      case symbol_kind::S_104_type_expr: // type-expr
        value.move< beng_generic_type > (that.value);
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
      case symbol_kind::S_94_enum_decl: // enum-decl
      case symbol_kind::S_98_type_decl: // type-decl
        value.move< declaration_t > (that.value);
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (that.value);
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
      case symbol_kind::S_100_type_extension_list: // type-extension-list
        value.move< extension_list_t > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_102_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
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

      case symbol_kind::S_97_case_decl: // case-decl
        value.move< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< std::vector<declaration_t> > (that.value);
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
      case symbol_kind::S_96_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (that.value);
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
#line 256 "beng.y"
                 { }
#line 869 "beng.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 256 "beng.y"
                 { }
#line 875 "beng.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 256 "beng.y"
                 { }
#line 881 "beng.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 256 "beng.y"
                 { }
#line 887 "beng.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 256 "beng.y"
                 { }
#line 893 "beng.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 256 "beng.y"
                 { }
#line 899 "beng.tab.cpp"
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
#line 256 "beng.y"
                 { }
#line 905 "beng.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 256 "beng.y"
                 { }
#line 911 "beng.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 256 "beng.y"
                 { }
#line 917 "beng.tab.cpp"
        break;

      case symbol_kind::S_94_enum_decl: // enum-decl
#line 256 "beng.y"
                 { }
#line 923 "beng.tab.cpp"
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
#line 256 "beng.y"
                 { }
#line 929 "beng.tab.cpp"
        break;

      case symbol_kind::S_96_case_list: // case-list
#line 256 "beng.y"
                 { }
#line 935 "beng.tab.cpp"
        break;

      case symbol_kind::S_97_case_decl: // case-decl
#line 256 "beng.y"
                 { }
#line 941 "beng.tab.cpp"
        break;

      case symbol_kind::S_98_type_decl: // type-decl
#line 256 "beng.y"
                 { }
#line 947 "beng.tab.cpp"
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
#line 256 "beng.y"
                 { }
#line 953 "beng.tab.cpp"
        break;

      case symbol_kind::S_100_type_extension_list: // type-extension-list
#line 256 "beng.y"
                 { }
#line 959 "beng.tab.cpp"
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
#line 256 "beng.y"
                 { }
#line 965 "beng.tab.cpp"
        break;

      case symbol_kind::S_102_parameter_list: // parameter-list
#line 256 "beng.y"
                 { }
#line 971 "beng.tab.cpp"
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
#line 256 "beng.y"
                 { }
#line 977 "beng.tab.cpp"
        break;

      case symbol_kind::S_104_type_expr: // type-expr
#line 256 "beng.y"
                 { }
#line 983 "beng.tab.cpp"
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
#line 256 "beng.y"
                 { }
#line 989 "beng.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 256 "beng.y"
                 { }
#line 995 "beng.tab.cpp"
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
#line 256 "beng.y"
                 { }
#line 1001 "beng.tab.cpp"
        break;

      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
#line 256 "beng.y"
                 { }
#line 1007 "beng.tab.cpp"
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
#line 256 "beng.y"
                 { }
#line 1013 "beng.tab.cpp"
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
      case symbol_kind::S_104_type_expr: // type-expr
        yylhs.value.emplace< beng_generic_type > ();
        break;

      case symbol_kind::S_91_opt_decl: // opt-decl
      case symbol_kind::S_94_enum_decl: // enum-decl
      case symbol_kind::S_98_type_decl: // type-decl
        yylhs.value.emplace< declaration_t > ();
        break;

      case symbol_kind::S_105_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< expression_t > ();
        break;

      case symbol_kind::S_99_type_extension_any: // type-extension-any
      case symbol_kind::S_100_type_extension_list: // type-extension-list
        yylhs.value.emplace< extension_list_t > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_107_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_108_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_109_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_101_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_102_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_103_parameter_decl: // parameter-decl
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

      case symbol_kind::S_97_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::identifier > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< std::vector<declaration_t> > ();
        break;

      case symbol_kind::S_95_case_list_opt: // case-list-opt
      case symbol_kind::S_96_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::identifier> > ();
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
#line 261 "beng.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < std::vector<declaration_t> > ())); }
#line 1341 "beng.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 266 "beng.y"
        { yylhs.value.as < std::vector<declaration_t> > () = {}; }
#line 1347 "beng.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any opt-decl
#line 269 "beng.y"
        {
	        yylhs.value.as < std::vector<declaration_t> > () = std::move(yystack_[1].value.as < std::vector<declaration_t> > ());
            if (yystack_[0].value.as < declaration_t > ().which()) yylhs.value.as < std::vector<declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < declaration_t > ()));
        }
#line 1356 "beng.tab.cpp"
    break;

  case 5: // opt-decl: "`;`"
#line 289 "beng.y"
        { yylhs.value.as < declaration_t > () = empty_t{}; }
#line 1362 "beng.tab.cpp"
    break;

  case 6: // opt-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 291 "beng.y"
        { yylhs.value.as < declaration_t > () = exten_var{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < beng_generic_type > ()) }; }
#line 1368 "beng.tab.cpp"
    break;

  case 7: // opt-decl: enum-decl
#line 293 "beng.y"
        { yylhs.value.as < declaration_t > () = yystack_[0].value.as < declaration_t > (); }
#line 1374 "beng.tab.cpp"
    break;

  case 8: // opt-decl: type-decl
#line 295 "beng.y"
        { yylhs.value.as < declaration_t > () = yystack_[0].value.as < declaration_t > (); }
#line 1380 "beng.tab.cpp"
    break;

  case 9: // opt-decl: LET identifier ASSIGN expression "`;`"
#line 297 "beng.y"
        { yylhs.value.as < declaration_t > () = let_statement_decl{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1386 "beng.tab.cpp"
    break;

  case 10: // opt-decl: compound-expression "`;`"
#line 299 "beng.y"
        { yylhs.value.as < declaration_t > () = expression_decl{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1392 "beng.tab.cpp"
    break;

  case 11: // identifier: IDENTIFIER
#line 313 "beng.y"
      { yylhs.value.as < sonia::lang::beng::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::beng::annotated_string_view > ()); }
#line 1398 "beng.tab.cpp"
    break;

  case 12: // qname: DBLCOLON identifier
#line 322 "beng.y"
        { yylhs.value.as < sonia::lang::beng::qname > () = sonia::lang::qname(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id); }
#line 1404 "beng.tab.cpp"
    break;

  case 13: // qname: identifier
#line 324 "beng.y"
        { yylhs.value.as < sonia::lang::beng::qname > () = sonia::lang::qname(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id); yylhs.value.as < sonia::lang::beng::qname > ().set_relative(); }
#line 1410 "beng.tab.cpp"
    break;

  case 14: // qname: qname DBLCOLON identifier
#line 326 "beng.y"
        { yylhs.value.as < sonia::lang::beng::qname > () = std::move(yystack_[2].value.as < sonia::lang::beng::qname > ()); yylhs.value.as < sonia::lang::beng::qname > ().append(std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id)); }
#line 1416 "beng.tab.cpp"
    break;

  case 15: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 332 "beng.y"
    {
        yylhs.value.as < declaration_t > () = enum_decl{ctx.ns() + std::move(yystack_[3].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::identifier> > ())};
    }
#line 1424 "beng.tab.cpp"
    break;

  case 16: // case-list-opt: %empty
#line 338 "beng.y"
           { yylhs.value.as < std::vector<sonia::lang::identifier> > () = {}; }
#line 1430 "beng.tab.cpp"
    break;

  case 17: // case-list-opt: case-list
#line 340 "beng.y"
    { yylhs.value.as < std::vector<sonia::lang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::identifier> > (); }
#line 1436 "beng.tab.cpp"
    break;

  case 18: // case-list: case-decl
#line 345 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::vector<sonia::lang::identifier>{std::move(yystack_[0].value.as < sonia::lang::identifier > ())}; }
#line 1442 "beng.tab.cpp"
    break;

  case 19: // case-list: case-list "," case-decl
#line 348 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::identifier > ())); }
#line 1448 "beng.tab.cpp"
    break;

  case 20: // case-decl: identifier
#line 353 "beng.y"
        { yylhs.value.as < sonia::lang::identifier > () = yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id; }
#line 1454 "beng.tab.cpp"
    break;

  case 21: // type-decl: TYPE qname type-extension-any "`{`" parameter-list-opt "`}`"
#line 358 "beng.y"
    {
        yylhs.value.as < declaration_t > () = type_decl{ctx.ns() + std::move(yystack_[4].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[3].value.as < extension_list_t > ()), std::move(yystack_[1].value.as < parameter_list_t > ())};
    }
#line 1462 "beng.tab.cpp"
    break;

  case 22: // type-extension-any: %empty
#line 364 "beng.y"
           { yylhs.value.as < extension_list_t > () = {}; }
#line 1468 "beng.tab.cpp"
    break;

  case 23: // type-extension-any: EXTENDS type-extension-list
#line 366 "beng.y"
                                { yylhs.value.as < extension_list_t > () = std::move(yystack_[0].value.as < extension_list_t > ()); }
#line 1474 "beng.tab.cpp"
    break;

  case 24: // type-extension-list: qname
#line 371 "beng.y"
        { yylhs.value.as < extension_list_t > () = extension_list_t{std::move(yystack_[0].value.as < sonia::lang::beng::qname > ())}; }
#line 1480 "beng.tab.cpp"
    break;

  case 25: // type-extension-list: type-extension-list "," qname
#line 374 "beng.y"
        { yylhs.value.as < extension_list_t > () = std::move(yystack_[2].value.as < extension_list_t > ()); yylhs.value.as < extension_list_t > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::beng::qname > ())); }
#line 1486 "beng.tab.cpp"
    break;

  case 26: // parameter-list-opt: %empty
#line 378 "beng.y"
           { yylhs.value.as < parameter_list_t > () = {}; }
#line 1492 "beng.tab.cpp"
    break;

  case 27: // parameter-list-opt: parameter-list
#line 380 "beng.y"
    { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 1498 "beng.tab.cpp"
    break;

  case 28: // parameter-list: parameter-decl
#line 385 "beng.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{std::move(yystack_[0].value.as < parameter_t > ())}; }
#line 1504 "beng.tab.cpp"
    break;

  case 29: // parameter-list: parameter-list "," parameter-decl
#line 388 "beng.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 1510 "beng.tab.cpp"
    break;

  case 30: // parameter-decl: identifier "`:`" type-expr
#line 393 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ().id), std::move(yystack_[0].value.as < beng_generic_type > ()) }; }
#line 1516 "beng.tab.cpp"
    break;

  case 31: // parameter-decl: "`_`" "`:`" type-expr
#line 395 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ sonia::lang::identifier{}, std::move(yystack_[0].value.as < beng_generic_type > ()) }; }
#line 1522 "beng.tab.cpp"
    break;

  case 32: // type-expr: BOOL
#line 401 "beng.y"
           { yylhs.value.as < beng_generic_type > () = beng_bool_t{}; }
#line 1528 "beng.tab.cpp"
    break;

  case 33: // type-expr: INT
#line 402 "beng.y"
          { yylhs.value.as < beng_generic_type > () = beng_int_t{}; }
#line 1534 "beng.tab.cpp"
    break;

  case 34: // type-expr: FLOAT
#line 403 "beng.y"
            { yylhs.value.as < beng_generic_type > () = beng_float_t{}; }
#line 1540 "beng.tab.cpp"
    break;

  case 35: // type-expr: qname
#line 404 "beng.y"
            { yylhs.value.as < beng_generic_type > () = beng_object_t{std::move(yystack_[0].value.as < sonia::lang::beng::qname > ())}; }
#line 1546 "beng.tab.cpp"
    break;

  case 36: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 406 "beng.y"
        {
            yylhs.value.as < beng_generic_type > () = beng_array_t{std::move(yystack_[3].value.as < beng_generic_type > ()), (size_t)std::move(yystack_[1].value.as < int > ())};
        }
#line 1554 "beng.tab.cpp"
    break;

  case 37: // type-expr: type-expr "`||`" type-expr
#line 410 "beng.y"
        {
            beng_union_t<beng_generic_type> uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < beng_generic_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < beng_generic_type > ()));
            yylhs.value.as < beng_generic_type > () = std::move(uni);
        }
#line 1565 "beng.tab.cpp"
    break;

  case 38: // compound-expression: qname "`(`" opt-named-expr-list-any "`)`"
#line 425 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { procedure_t{ std::move(yystack_[3].value.as < sonia::lang::beng::qname > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) } }; }
#line 1571 "beng.tab.cpp"
    break;

  case 39: // compound-expression: expression ASSIGN expression
#line 427 "beng.y"
        { yylhs.value.as < expression_t > () = assign_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 1577 "beng.tab.cpp"
    break;

  case 40: // expression: INTEGER
#line 432 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { decimal(yystack_[0].value.as < int > ()) }; }
#line 1583 "beng.tab.cpp"
    break;

  case 41: // expression: DECIMAL
#line 434 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { std::move(yystack_[0].value.as < sonia::decimal > ()) }; }
#line 1589 "beng.tab.cpp"
    break;

  case 42: // expression: STRING
#line 436 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { ctx.make_string(yystack_[0].value.as < sonia::string_view > ()) }; }
#line 1595 "beng.tab.cpp"
    break;

  case 43: // expression: "`.`" identifier
#line 438 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { case_expression { std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().id) } }; }
#line 1601 "beng.tab.cpp"
    break;

  case 44: // expression: qname
#line 440 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { std::move(yystack_[0].value.as < sonia::lang::beng::qname > ()) }; }
#line 1607 "beng.tab.cpp"
    break;

  case 45: // expression: "`(`" expression "`)`"
#line 442 "beng.y"
        { yylhs.value.as < expression_t > () = expression_t { std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1613 "beng.tab.cpp"
    break;

  case 46: // expression: compound-expression
#line 444 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 1619 "beng.tab.cpp"
    break;

  case 47: // opt-named-expr-list-any: %empty
#line 454 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 1625 "beng.tab.cpp"
    break;

  case 48: // opt-named-expr-list-any: opt-named-expr-list
#line 455 "beng.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 1631 "beng.tab.cpp"
    break;

  case 49: // opt-named-expr-list: opt-named-expr
#line 460 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 1637 "beng.tab.cpp"
    break;

  case 50: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 462 "beng.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 1646 "beng.tab.cpp"
    break;

  case 51: // opt-named-expr: identifier "`:`" expression
#line 470 "beng.y"
        { yylhs.value.as < named_expression_term_t > () = std::tuple{std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[0].value.as < expression_t > ())}; }
#line 1652 "beng.tab.cpp"
    break;

  case 52: // opt-named-expr: expression
#line 472 "beng.y"
        { yylhs.value.as < named_expression_term_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 1658 "beng.tab.cpp"
    break;


#line 1662 "beng.tab.cpp"

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

  const signed char
  parser::yypact_[] =
  {
     -60,    20,     0,   -60,   -60,   -60,   -60,   -60,   -60,    -4,
     -60,    19,    19,   -35,    19,     5,     5,   -60,   -60,   -27,
     -60,   -60,   -16,   -41,   -60,   -15,   -60,   -40,    19,   -60,
     -21,   -53,    -4,    19,   -60,    -4,   -60,    -4,    16,    19,
       5,    17,    21,   -41,    25,    -2,   -60,   -60,   -41,   -28,
       4,   -60,    18,     8,   -60,   -14,     9,    42,    -4,   -60,
      -4,   -60,   -60,   -60,   -60,   -14,    -5,   -60,    19,     5,
      33,    36,    35,    22,   -60,   -41,   -60,     4,    64,   -60,
     -60,   -14,     4,     4,   -60,    42,    39,    34,     6,     6,
     -60,   -60
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    42,    11,    41,    40,     0,
       5,     0,     0,     0,     0,     0,     0,     4,    13,    44,
       7,     8,    46,     0,    46,     0,    43,     0,     0,    12,
       0,    22,    47,     0,    10,     0,    45,     0,     0,    16,
       0,     0,    13,    52,     0,    48,    49,    14,    39,     0,
       0,    20,     0,    17,    18,    24,    23,    26,     0,    38,
       0,     9,    32,    33,    34,    35,     0,    15,     0,     0,
       0,     0,     0,    27,    28,    51,    50,     0,     0,     6,
      19,    25,     0,     0,    21,     0,    37,     0,    31,    30,
      29,    36
  };

  const signed char
  parser::yypgoto_[] =
  {
     -60,   -60,   -60,   -60,     1,    27,   -60,   -60,   -60,    13,
     -60,   -60,   -60,   -60,   -60,     2,   -59,    86,    12,   -60,
     -60,    32
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    17,    18,    19,    20,    52,    53,    54,
      21,    41,    56,    72,    73,    74,    66,    24,    43,    44,
      45,    46
  };

  const signed char
  parser::yytable_[] =
  {
       4,     5,     6,     7,     8,     5,     6,     7,     8,    32,
       6,     6,    26,    27,    23,    29,    61,    39,    86,    77,
       3,    25,    36,    88,    89,     6,    28,    33,    34,    38,
      77,    40,     9,    42,    47,    78,     9,    35,    37,    79,
      51,    11,    30,    31,    10,    11,    78,    48,     6,    49,
      35,    50,    70,    33,    60,    57,    58,    67,    71,    33,
      12,    42,    59,    35,    68,    69,    33,    55,    82,    51,
      75,    83,    87,    13,    84,    91,    14,    65,    85,    78,
      14,    80,    15,    16,    14,    14,    71,    90,    22,    62,
      63,    64,    76,     0,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,    65,
      65
  };

  const signed char
  parser::yycheck_[] =
  {
       0,     5,     6,     7,     8,     5,     6,     7,     8,    36,
       6,     6,    11,    12,     2,    14,    44,    38,    77,    24,
       0,     9,    37,    82,    83,     6,    61,    80,    44,    28,
      24,    84,    36,    32,    33,    40,    36,    78,    78,    44,
      39,    45,    15,    16,    44,    45,    40,    35,     6,    37,
      78,    35,    10,    80,    56,    38,    35,    39,    57,    80,
      60,    60,    37,    78,    56,    56,    80,    40,    35,    68,
      58,    35,     8,    73,    39,    41,    80,    50,    56,    40,
      80,    68,    82,    83,    80,    80,    85,    85,     2,    85,
      86,    87,    60,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    82,
      83
  };

  const signed char
  parser::yystos_[] =
  {
       0,    89,    90,     0,     0,     5,     6,     7,     8,    36,
      44,    45,    60,    73,    80,    82,    83,    91,    92,    93,
      94,    98,   105,   106,   105,   106,    92,    92,    61,    92,
      93,    93,    36,    80,    44,    78,    37,    78,    92,    38,
      84,    99,    92,   106,   107,   108,   109,    92,   106,   106,
      35,    92,    95,    96,    97,    93,   100,    38,    35,    37,
      56,    44,    85,    86,    87,    93,   104,    39,    56,    56,
      10,    92,   101,   102,   103,   106,   109,    24,    40,    44,
      97,    93,    35,    35,    39,    56,   104,     8,   104,   104,
     103,    41
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    88,    89,    90,    90,    91,    91,    91,    91,    91,
      91,    92,    93,    93,    93,    94,    95,    95,    96,    96,
      97,    98,    99,    99,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   104,   104,   104,   104,   105,   105,
     106,   106,   106,   106,   106,   106,   106,   107,   107,   108,
     108,   109,   109
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     1,     6,     1,     1,     5,
       2,     1,     2,     1,     3,     5,     0,     1,     1,     3,
       1,     6,     0,     2,     1,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     4,     3,     4,     3,
       1,     1,     1,     2,     1,     3,     1,     0,     1,     1,
       3,     3,     1
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
  "\",\"", "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET", "VAR", "FN", "NEW",
  "FOR", "VOID_", "AUTO", "CONST_", "USING", "THROW", "DELETE_", "SIZEOF",
  "RETURN", "EXTERN", "PRIVATE", "TEMPLATE", "NAMESPACE", "LOWEST",
  "ASSIGN", "\"`=`\"", "DBLCOLON", "\"`::`\"", "ENUM", "TYPE", "EXTENDS",
  "BOOL", "INT", "FLOAT", "$accept", "begin", "declaration_any",
  "opt-decl", "identifier", "qname", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "type-decl", "type-extension-any",
  "type-extension-list", "parameter-list-opt", "parameter-list",
  "parameter-decl", "type-expr", "compound-expression", "expression",
  "opt-named-expr-list-any", "opt-named-expr-list", "opt-named-expr", YY_NULLPTR
  };
#endif


#if BENG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   261,   261,   265,   268,   288,   290,   292,   294,   296,
     298,   312,   321,   323,   325,   331,   338,   340,   344,   347,
     352,   357,   364,   366,   370,   373,   378,   380,   384,   387,
     392,   394,   401,   402,   403,   404,   405,   409,   424,   426,
     431,   433,   435,   437,   439,   441,   443,   453,   455,   459,
     461,   469,   471
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
      85,    86,    87
    };
    // Last valid token kind.
    const int code_max = 342;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // beng_lang
#line 2262 "beng.tab.cpp"

#line 840 "beng.y"

