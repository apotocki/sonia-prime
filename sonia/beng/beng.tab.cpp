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

#define IGNORE(...)


#line 70 "beng.tab.cpp"


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
#line 167 "beng.tab.cpp"

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
      case symbol_kind::S_116_type_expr: // type-expr
        value.copy< beng_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.copy< declaration_t > (YY_MOVE (that.value));
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

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
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
        value.copy< sonia::lang::beng::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::beng::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.copy< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
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
        value.move< beng_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.move< declaration_t > (YY_MOVE (s.value));
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

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
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
        value.move< sonia::lang::beng::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
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
        value.YY_MOVE_OR_COPY< beng_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.YY_MOVE_OR_COPY< declaration_t > (YY_MOVE (that.value));
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

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.YY_MOVE_OR_COPY< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
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
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.YY_MOVE_OR_COPY< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
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
        value.move< beng_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.move< declaration_t > (YY_MOVE (that.value));
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

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_97_let_decl: // let-decl
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
        value.move< sonia::lang::beng::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.move< sonia::lang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
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
        value.copy< beng_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.copy< declaration_t > (that.value);
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

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_97_let_decl: // let-decl
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
        value.copy< sonia::lang::beng::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.copy< sonia::lang::beng::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.copy< sonia::lang::beng::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.copy< sonia::lang::beng::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.copy< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
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
        value.move< beng_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        value.move< declaration_t > (that.value);
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

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_97_let_decl: // let-decl
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
        value.move< sonia::lang::beng::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        value.move< sonia::lang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
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
#line 286 "beng.y"
                 { }
#line 1153 "beng.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 286 "beng.y"
                 { }
#line 1159 "beng.tab.cpp"
        break;

      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
#line 286 "beng.y"
                 { }
#line 1165 "beng.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 286 "beng.y"
                 { }
#line 1171 "beng.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 286 "beng.y"
                 { }
#line 1177 "beng.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 286 "beng.y"
                 { }
#line 1183 "beng.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 286 "beng.y"
                 { }
#line 1189 "beng.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 286 "beng.y"
                 { }
#line 1195 "beng.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 286 "beng.y"
                 { }
#line 1201 "beng.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 286 "beng.y"
                 { }
#line 1207 "beng.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 286 "beng.y"
                 { }
#line 1213 "beng.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 286 "beng.y"
                 { }
#line 1219 "beng.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 286 "beng.y"
                 { }
#line 1225 "beng.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 286 "beng.y"
                 { }
#line 1231 "beng.tab.cpp"
        break;

      case symbol_kind::S_FN: // FN
#line 286 "beng.y"
                 { }
#line 1237 "beng.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // TRUE
#line 286 "beng.y"
                 { }
#line 1243 "beng.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // FALSE
#line 286 "beng.y"
                 { }
#line 1249 "beng.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 286 "beng.y"
                 { }
#line 1255 "beng.tab.cpp"
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
#line 286 "beng.y"
                 { }
#line 1261 "beng.tab.cpp"
        break;

      case symbol_kind::S_97_let_decl: // let-decl
#line 286 "beng.y"
                 { }
#line 1267 "beng.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 286 "beng.y"
                 { }
#line 1273 "beng.tab.cpp"
        break;

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
#line 286 "beng.y"
                 { }
#line 1279 "beng.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 286 "beng.y"
                 { }
#line 1285 "beng.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 286 "beng.y"
                 { }
#line 1291 "beng.tab.cpp"
        break;

      case symbol_kind::S_102_fn_decl: // fn-decl
#line 286 "beng.y"
                 { }
#line 1297 "beng.tab.cpp"
        break;

      case symbol_kind::S_103_enum_decl: // enum-decl
#line 286 "beng.y"
                 { }
#line 1303 "beng.tab.cpp"
        break;

      case symbol_kind::S_104_case_list_opt: // case-list-opt
#line 286 "beng.y"
                 { }
#line 1309 "beng.tab.cpp"
        break;

      case symbol_kind::S_105_case_list: // case-list
#line 286 "beng.y"
                 { }
#line 1315 "beng.tab.cpp"
        break;

      case symbol_kind::S_106_case_decl: // case-decl
#line 286 "beng.y"
                 { }
#line 1321 "beng.tab.cpp"
        break;

      case symbol_kind::S_107_type_decl: // type-decl
#line 286 "beng.y"
                 { }
#line 1327 "beng.tab.cpp"
        break;

      case symbol_kind::S_108_type_extension_any: // type-extension-any
#line 286 "beng.y"
                 { }
#line 1333 "beng.tab.cpp"
        break;

      case symbol_kind::S_109_type_extension_list: // type-extension-list
#line 286 "beng.y"
                 { }
#line 1339 "beng.tab.cpp"
        break;

      case symbol_kind::S_110_parameter_list_opt: // parameter-list-opt
#line 286 "beng.y"
                 { }
#line 1345 "beng.tab.cpp"
        break;

      case symbol_kind::S_111_parameter_list: // parameter-list
#line 286 "beng.y"
                 { }
#line 1351 "beng.tab.cpp"
        break;

      case symbol_kind::S_112_parameter_decl: // parameter-decl
#line 286 "beng.y"
                 { }
#line 1357 "beng.tab.cpp"
        break;

      case symbol_kind::S_113_parameter_woa_list_opt: // parameter-woa-list-opt
#line 286 "beng.y"
                 { }
#line 1363 "beng.tab.cpp"
        break;

      case symbol_kind::S_114_parameter_woa_list: // parameter-woa-list
#line 286 "beng.y"
                 { }
#line 1369 "beng.tab.cpp"
        break;

      case symbol_kind::S_115_parameter_woa_decl: // parameter-woa-decl
#line 286 "beng.y"
                 { }
#line 1375 "beng.tab.cpp"
        break;

      case symbol_kind::S_116_type_expr: // type-expr
#line 286 "beng.y"
                 { }
#line 1381 "beng.tab.cpp"
        break;

      case symbol_kind::S_117_compound_expression: // compound-expression
#line 286 "beng.y"
                 { }
#line 1387 "beng.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 286 "beng.y"
                 { }
#line 1393 "beng.tab.cpp"
        break;

      case symbol_kind::S_119_expression_list_any: // expression-list-any
#line 286 "beng.y"
                 { }
#line 1399 "beng.tab.cpp"
        break;

      case symbol_kind::S_120_opt_named_expr_list_any: // opt-named-expr-list-any
#line 286 "beng.y"
                 { }
#line 1405 "beng.tab.cpp"
        break;

      case symbol_kind::S_121_opt_named_expr_list: // opt-named-expr-list
#line 286 "beng.y"
                 { }
#line 1411 "beng.tab.cpp"
        break;

      case symbol_kind::S_122_opt_named_expr: // opt-named-expr
#line 286 "beng.y"
                 { }
#line 1417 "beng.tab.cpp"
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
      case symbol_kind::S_116_type_expr: // type-expr
        yylhs.value.emplace< beng_preliminary_type > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_96_generic_decl: // generic-decl
        yylhs.value.emplace< declaration_t > ();
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

      case symbol_kind::S_99_opt_infunction_decl: // opt-infunction-decl
        yylhs.value.emplace< infunction_declaration_t > ();
        break;

      case symbol_kind::S_97_let_decl: // let-decl
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
        yylhs.value.emplace< sonia::lang::beng::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
        yylhs.value.emplace< sonia::lang::beng::annotated_identifier > ();
        break;

      case symbol_kind::S_qname: // qname
        yylhs.value.emplace< sonia::lang::beng::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
        yylhs.value.emplace< sonia::lang::beng::annotated_string_view > ();
        break;

      case symbol_kind::S_106_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::identifier > ();
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_LOGIC_AND: // "`&&`"
      case symbol_kind::S_LOGIC_OR: // "`||`"
      case symbol_kind::S_CONCAT: // "`..`"
      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // FN
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
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
#line 291 "beng.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 1792 "beng.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 296 "beng.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 1798 "beng.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any "`;`"
#line 298 "beng.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 1804 "beng.tab.cpp"
    break;

  case 5: // declaration_any: declaration_any enum-decl
#line 300 "beng.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); yylhs.value.as < declaration_set_t > ().types.emplace_back(std::move(yystack_[0].value.as < enum_decl > ())); }
#line 1810 "beng.tab.cpp"
    break;

  case 6: // declaration_any: declaration_any type-decl
#line 302 "beng.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); yylhs.value.as < declaration_set_t > ().types.emplace_back(std::move(yystack_[0].value.as < type_decl > ())); }
#line 1816 "beng.tab.cpp"
    break;

  case 7: // declaration_any: declaration_any generic-decl
#line 304 "beng.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().generic.emplace_back(std::move(yystack_[0].value.as < declaration_t > ()));
        }
#line 1825 "beng.tab.cpp"
    break;

  case 8: // generic-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 324 "beng.y"
        { yylhs.value.as < declaration_t > () = extern_var{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[1].value.as < beng_preliminary_type > ()) }; }
#line 1831 "beng.tab.cpp"
    break;

  case 9: // generic-decl: EXTERN FN qname "`(`" parameter-woa-list-opt "`)`" "`;`"
#line 326 "beng.y"
        { yylhs.value.as < declaration_t > () = fn_pure_decl{ std::move(yystack_[4].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()), beng_preliminary_tuple_t{} }; IGNORE(yystack_[5].value.as < sonia::lang::lex::resource_location > (), yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 1837 "beng.tab.cpp"
    break;

  case 10: // generic-decl: EXTERN FN qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`;`"
#line 328 "beng.y"
        { yylhs.value.as < declaration_t > () = fn_pure_decl{ std::move(yystack_[6].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), std::move(yystack_[1].value.as < beng_preliminary_type > ()) }; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > ()); }
#line 1843 "beng.tab.cpp"
    break;

  case 11: // generic-decl: fn-decl
#line 330 "beng.y"
        { yylhs.value.as < declaration_t > () = std::move(yystack_[0].value.as < fn_decl_t > ()); }
#line 1849 "beng.tab.cpp"
    break;

  case 12: // generic-decl: let-decl
#line 332 "beng.y"
        { yylhs.value.as < declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 1855 "beng.tab.cpp"
    break;

  case 13: // generic-decl: compound-expression "`;`"
#line 334 "beng.y"
        { yylhs.value.as < declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1861 "beng.tab.cpp"
    break;

  case 14: // let-decl: LET identifier "`:`" type-expr "`;`"
#line 348 "beng.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ()), nullopt, std::move(yystack_[1].value.as < beng_preliminary_type > ())}; }
#line 1867 "beng.tab.cpp"
    break;

  case 15: // let-decl: LET identifier "`:`" type-expr "`=`" expression "`;`"
#line 350 "beng.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[5].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[1].value.as < expression_t > ()), std::move(yystack_[3].value.as < beng_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 1873 "beng.tab.cpp"
    break;

  case 16: // let-decl: LET identifier "`=`" expression "`;`"
#line 352 "beng.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[3].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[1].value.as < expression_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 1879 "beng.tab.cpp"
    break;

  case 17: // infunction_declaration_any: %empty
#line 357 "beng.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 1885 "beng.tab.cpp"
    break;

  case 18: // infunction_declaration_any: infunction_declaration_any "`;`"
#line 359 "beng.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()); }
#line 1891 "beng.tab.cpp"
    break;

  case 19: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 361 "beng.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 1900 "beng.tab.cpp"
    break;

  case 20: // opt-infunction-decl: let-decl
#line 369 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 1906 "beng.tab.cpp"
    break;

  case 21: // opt-infunction-decl: RETURN expression "`;`"
#line 371 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = return_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1912 "beng.tab.cpp"
    break;

  case 22: // opt-infunction-decl: compound-expression "`;`"
#line 373 "beng.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1918 "beng.tab.cpp"
    break;

  case 23: // identifier: IDENTIFIER
#line 378 "beng.y"
      { yylhs.value.as < sonia::lang::beng::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::beng::annotated_string_view > ()); }
#line 1924 "beng.tab.cpp"
    break;

  case 24: // qname: DBLCOLON identifier
#line 387 "beng.y"
        { yylhs.value.as < sonia::lang::beng::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().location) }; }
#line 1930 "beng.tab.cpp"
    break;

  case 25: // qname: identifier
#line 389 "beng.y"
        { yylhs.value.as < sonia::lang::beng::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().location) }; }
#line 1936 "beng.tab.cpp"
    break;

  case 26: // qname: qname DBLCOLON identifier
#line 391 "beng.y"
        { yylhs.value.as < sonia::lang::beng::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::beng::annotated_qname > ()); yylhs.value.as < sonia::lang::beng::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().value)); }
#line 1942 "beng.tab.cpp"
    break;

  case 27: // fn-decl: FN qname "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 397 "beng.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[6].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > ());
            }
#line 1950 "beng.tab.cpp"
    break;

  case 28: // fn-decl: FN qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 401 "beng.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[8].value.as < sonia::lang::beng::annotated_qname > ()), std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < beng_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[9].value.as < sonia::lang::lex::resource_location > (), yystack_[7].value.as < sonia::lang::lex::resource_location > ());
            }
#line 1958 "beng.tab.cpp"
    break;

  case 29: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 408 "beng.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{annotated_qname{ctx.ns() + std::move(yystack_[3].value.as < sonia::lang::beng::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::beng::annotated_qname > ().location}, std::move(yystack_[1].value.as < std::vector<sonia::lang::identifier> > ())};
    }
#line 1966 "beng.tab.cpp"
    break;

  case 30: // case-list-opt: %empty
#line 414 "beng.y"
           { yylhs.value.as < std::vector<sonia::lang::identifier> > () = {}; }
#line 1972 "beng.tab.cpp"
    break;

  case 31: // case-list-opt: case-list
#line 416 "beng.y"
    { yylhs.value.as < std::vector<sonia::lang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::identifier> > (); }
#line 1978 "beng.tab.cpp"
    break;

  case 32: // case-list: case-decl
#line 421 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::vector<sonia::lang::identifier>{std::move(yystack_[0].value.as < sonia::lang::identifier > ())}; }
#line 1984 "beng.tab.cpp"
    break;

  case 33: // case-list: case-list "," case-decl
#line 424 "beng.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::identifier > ())); }
#line 1990 "beng.tab.cpp"
    break;

  case 34: // case-decl: identifier
#line 429 "beng.y"
        { yylhs.value.as < sonia::lang::identifier > () = yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ().value; }
#line 1996 "beng.tab.cpp"
    break;

  case 35: // type-decl: TYPE qname type-extension-any "`{`" parameter-woa-list-opt "`}`"
#line 434 "beng.y"
    {
        yylhs.value.as < type_decl > () = type_decl{annotated_qname{ctx.ns() + std::move(yystack_[4].value.as < sonia::lang::beng::annotated_qname > ().value), yystack_[4].value.as < sonia::lang::beng::annotated_qname > ().location}, std::move(yystack_[3].value.as < extension_list_t > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ())};
    }
#line 2004 "beng.tab.cpp"
    break;

  case 36: // type-extension-any: %empty
#line 440 "beng.y"
           { yylhs.value.as < extension_list_t > () = {}; }
#line 2010 "beng.tab.cpp"
    break;

  case 37: // type-extension-any: EXTENDS type-extension-list
#line 442 "beng.y"
                                { yylhs.value.as < extension_list_t > () = std::move(yystack_[0].value.as < extension_list_t > ()); }
#line 2016 "beng.tab.cpp"
    break;

  case 38: // type-extension-list: qname
#line 447 "beng.y"
        { yylhs.value.as < extension_list_t > () = extension_list_t{std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ())}; }
#line 2022 "beng.tab.cpp"
    break;

  case 39: // type-extension-list: type-extension-list "," qname
#line 450 "beng.y"
        { yylhs.value.as < extension_list_t > () = std::move(yystack_[2].value.as < extension_list_t > ()); yylhs.value.as < extension_list_t > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ())); }
#line 2028 "beng.tab.cpp"
    break;

  case 40: // parameter-list-opt: %empty
#line 454 "beng.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 2034 "beng.tab.cpp"
    break;

  case 41: // parameter-list-opt: parameter-list
#line 455 "beng.y"
      { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 2040 "beng.tab.cpp"
    break;

  case 42: // parameter-list: parameter-decl
#line 459 "beng.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{std::move(yystack_[0].value.as < parameter_t > ())}; }
#line 2046 "beng.tab.cpp"
    break;

  case 43: // parameter-list: parameter-list "," parameter-decl
#line 461 "beng.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2052 "beng.tab.cpp"
    break;

  case 44: // parameter-decl: identifier "`:`" type-expr
#line 465 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[0].value.as < beng_preliminary_type > ()) }; }
#line 2058 "beng.tab.cpp"
    break;

  case 45: // parameter-decl: type-expr
#line 467 "beng.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ nullopt, std::move(yystack_[0].value.as < beng_preliminary_type > ()) }; }
#line 2064 "beng.tab.cpp"
    break;

  case 46: // parameter-woa-list-opt: %empty
#line 473 "beng.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2070 "beng.tab.cpp"
    break;

  case 47: // parameter-woa-list-opt: parameter-woa-list
#line 474 "beng.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2076 "beng.tab.cpp"
    break;

  case 48: // parameter-woa-list: parameter-woa-decl
#line 478 "beng.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{std::move(yystack_[0].value.as < parameter_woa_t > ())}; }
#line 2082 "beng.tab.cpp"
    break;

  case 49: // parameter-woa-list: parameter-woa-list "," parameter-woa-decl
#line 480 "beng.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_woa_t > ())); }
#line 2088 "beng.tab.cpp"
    break;

  case 50: // parameter-woa-decl: parameter-decl
#line 484 "beng.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t { std::move(yystack_[0].value.as < parameter_t > ()) }; }
#line 2094 "beng.tab.cpp"
    break;

  case 51: // parameter-woa-decl: parameter-decl "`=`" expression
#line 486 "beng.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t{ std::move(yystack_[2].value.as < parameter_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2100 "beng.tab.cpp"
    break;

  case 52: // type-expr: BOOL
#line 491 "beng.y"
           { yylhs.value.as < beng_preliminary_type > () = beng_bool_t{}; }
#line 2106 "beng.tab.cpp"
    break;

  case 53: // type-expr: INT
#line 492 "beng.y"
          { yylhs.value.as < beng_preliminary_type > () = beng_int_t{}; }
#line 2112 "beng.tab.cpp"
    break;

  case 54: // type-expr: FLOAT
#line 493 "beng.y"
            { yylhs.value.as < beng_preliminary_type > () = beng_float_t{}; }
#line 2118 "beng.tab.cpp"
    break;

  case 55: // type-expr: STRING_WORD
#line 494 "beng.y"
                  { yylhs.value.as < beng_preliminary_type > () = beng_string_t{}; }
#line 2124 "beng.tab.cpp"
    break;

  case 56: // type-expr: DECIMAL_WORD
#line 495 "beng.y"
                   { yylhs.value.as < beng_preliminary_type > () = beng_decimal_t{}; }
#line 2130 "beng.tab.cpp"
    break;

  case 57: // type-expr: qname
#line 496 "beng.y"
            { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_object_t{ std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ()) }; }
#line 2136 "beng.tab.cpp"
    break;

  case 58: // type-expr: "`[`" type-expr "`]`"
#line 498 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_vector_t{std::move(yystack_[1].value.as < beng_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2142 "beng.tab.cpp"
    break;

  case 59: // type-expr: "`(`" parameter-list-opt "`)`"
#line 500 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_tuple_t { std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2148 "beng.tab.cpp"
    break;

  case 60: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 502 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_array_t{std::move(yystack_[3].value.as < beng_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::beng::annotated_decimal > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2154 "beng.tab.cpp"
    break;

  case 61: // type-expr: type-expr "`|`" type-expr
#line 504 "beng.y"
        {
            beng_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < beng_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < beng_preliminary_type > ()));
            yylhs.value.as < beng_preliminary_type > () = std::move(uni);
        }
#line 2165 "beng.tab.cpp"
    break;

  case 62: // type-expr: type-expr "`->`" type-expr
#line 511 "beng.y"
        { yylhs.value.as < beng_preliminary_type > () = beng_preliminary_fn_t{std::move(yystack_[2].value.as < beng_preliminary_type > ()), std::move(yystack_[0].value.as < beng_preliminary_type > ())}; }
#line 2171 "beng.tab.cpp"
    break;

  case 63: // compound-expression: expression "`(`" opt-named-expr-list-any "`)`"
#line 528 "beng.y"
        { yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2177 "beng.tab.cpp"
    break;

  case 64: // compound-expression: expression "`=`" expression
#line 530 "beng.y"
        { yylhs.value.as < expression_t > () = assign_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2183 "beng.tab.cpp"
    break;

  case 65: // expression: TRUE
#line 535 "beng.y"
        { yylhs.value.as < expression_t > () = annotated_bool{true, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2189 "beng.tab.cpp"
    break;

  case 66: // expression: FALSE
#line 537 "beng.y"
        { yylhs.value.as < expression_t > () = annotated_bool{false, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2195 "beng.tab.cpp"
    break;

  case 67: // expression: INTEGER
#line 539 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::beng::annotated_decimal > ()); }
#line 2201 "beng.tab.cpp"
    break;

  case 68: // expression: DECIMAL
#line 541 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::beng::annotated_decimal > ()); }
#line 2207 "beng.tab.cpp"
    break;

  case 69: // expression: STRING
#line 543 "beng.y"
        { yylhs.value.as < expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::beng::annotated_string_view > ())); }
#line 2213 "beng.tab.cpp"
    break;

  case 70: // expression: "`.`" identifier
#line 545 "beng.y"
        { yylhs.value.as < expression_t > () = case_expression { std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2219 "beng.tab.cpp"
    break;

  case 71: // expression: qname
#line 547 "beng.y"
        { yylhs.value.as < expression_t > () = variable_identifier{std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ().value), std::move(yystack_[0].value.as < sonia::lang::beng::annotated_qname > ().location), false}; }
#line 2225 "beng.tab.cpp"
    break;

  case 72: // expression: ARGIDENTIFIER
#line 549 "beng.y"
        { auto tmp = ctx.make_identifier(yystack_[0].value.as < sonia::lang::beng::annotated_string_view > ()); yylhs.value.as < expression_t > () = variable_identifier{ qname{tmp.value, false}, std::move(tmp.location), true }; }
#line 2231 "beng.tab.cpp"
    break;

  case 73: // expression: "`(`" expression "`)`"
#line 551 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[1].value.as < expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2237 "beng.tab.cpp"
    break;

  case 74: // expression: FN "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 553 "beng.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[6].value.as < sonia::lang::lex::resource_location > ())}; }
#line 2243 "beng.tab.cpp"
    break;

  case 75: // expression: FN "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 555 "beng.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[7].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < beng_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[8].value.as < sonia::lang::lex::resource_location > ())}; }
#line 2249 "beng.tab.cpp"
    break;

  case 76: // expression: "`[`" expression-list-any "`]`"
#line 557 "beng.y"
        { yylhs.value.as < expression_t > () = expression_vector_t{ {std::move(yystack_[1].value.as < expression_list_t > ())}, std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2255 "beng.tab.cpp"
    break;

  case 77: // expression: "`!`" expression
#line 559 "beng.y"
                { yylhs.value.as < expression_t > () = negate_expression_t{ std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2261 "beng.tab.cpp"
    break;

  case 78: // expression: expression "`.`" identifier
#line 561 "beng.y"
        { yylhs.value.as < expression_t > () = member_expression_t { std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2267 "beng.tab.cpp"
    break;

  case 79: // expression: expression "`?`" "`.`" identifier
#line 563 "beng.y"
        { yylhs.value.as < expression_t > () = member_expression_t { std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::beng::annotated_identifier > ()), true }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2273 "beng.tab.cpp"
    break;

  case 80: // expression: expression "`&&`" expression
#line 567 "beng.y"
        { yylhs.value.as < expression_t > () = logic_and_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2279 "beng.tab.cpp"
    break;

  case 81: // expression: expression "`||`" expression
#line 569 "beng.y"
        { yylhs.value.as < expression_t > () = logic_or_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2285 "beng.tab.cpp"
    break;

  case 82: // expression: expression "`..`" expression
#line 571 "beng.y"
        { yylhs.value.as < expression_t > () = concat_expression_t{ std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2291 "beng.tab.cpp"
    break;

  case 83: // expression: compound-expression
#line 574 "beng.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 2297 "beng.tab.cpp"
    break;

  case 84: // expression-list-any: %empty
#line 584 "beng.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{}; }
#line 2303 "beng.tab.cpp"
    break;

  case 85: // expression-list-any: expression
#line 586 "beng.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 2309 "beng.tab.cpp"
    break;

  case 86: // expression-list-any: expression-list-any "," expression
#line 588 "beng.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < expression_t > ())); }
#line 2315 "beng.tab.cpp"
    break;

  case 87: // opt-named-expr-list-any: %empty
#line 593 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2321 "beng.tab.cpp"
    break;

  case 88: // opt-named-expr-list-any: opt-named-expr-list
#line 594 "beng.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2327 "beng.tab.cpp"
    break;

  case 89: // opt-named-expr-list: opt-named-expr
#line 599 "beng.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2333 "beng.tab.cpp"
    break;

  case 90: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 601 "beng.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2342 "beng.tab.cpp"
    break;

  case 91: // opt-named-expr: identifier "`:`" expression
#line 609 "beng.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::tuple{std::move(yystack_[2].value.as < sonia::lang::beng::annotated_identifier > ()), std::move(yystack_[0].value.as < expression_t > ())}, std::move(loc)}; }
#line 2348 "beng.tab.cpp"
    break;

  case 92: // opt-named-expr: expression
#line 611 "beng.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::move(yystack_[0].value.as < expression_t > ()), std::move(loc)}; }
#line 2354 "beng.tab.cpp"
    break;


#line 2358 "beng.tab.cpp"

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
     -60,    16,   129,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     314,   314,   -60,    21,   314,    21,   -44,    21,     0,     2,
       2,   -60,   -60,   -60,   -60,   -60,     6,   -60,   -60,   -60,
      44,   221,    84,   -60,   162,   221,    56,   -60,   -13,    65,
      21,     2,   -60,   288,   -25,    13,   -57,    21,   -60,   314,
     314,   314,   314,   314,    21,    -3,   -60,   -60,   314,   314,
     288,    88,   -15,   288,   288,   -60,   -60,   -60,   -60,   -60,
      90,     6,   121,    99,    81,   -60,     3,   288,    21,     2,
     102,   -60,   221,   -17,   -18,   -13,   107,   221,   106,    89,
     -60,   -60,    21,   221,    58,     4,   288,   288,   109,    92,
     -60,    19,   288,   314,    61,   288,   288,   147,   288,   117,
     -60,   118,   103,   -60,     6,   105,   288,   314,   -60,   314,
     -60,   -60,   314,   -60,     5,   130,   -60,   288,   -60,     3,
     221,   288,   -60,   -60,   -60,   114,   -11,    73,   -60,    21,
       2,   132,   221,   -60,   361,   -60,    74,   -60,    23,   188,
     -60,   288,   -60,   -60,     6,   -60,   -60,   288,   -60,   -60,
     -60,   -60,   314,   -60,   -60,   131,   111,   210,   116,   269,
     386,   -60,   -60,   -60,   -60,   -60,   -60,   291,   -60
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    69,    23,    72,    67,    68,
       0,    84,     4,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,     7,    12,    25,    71,    11,     5,     6,
      83,     0,     0,    83,     0,    85,     0,    70,    77,     0,
       0,     0,    24,    46,     0,     0,    36,     0,    13,     0,
       0,     0,     0,    87,     0,     0,    73,    76,     0,     0,
       0,     0,     0,    40,     0,    52,    53,    54,    55,    56,
      25,    57,    50,     0,    47,    48,    45,    46,    30,     0,
       0,    26,    64,    80,    81,    82,    25,    92,     0,    88,
      89,    78,     0,    86,     0,     0,     0,    46,     0,    41,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    31,    32,    38,    37,    46,     0,    63,     0,
      79,    16,     0,    14,     0,     0,    59,     0,    58,    44,
      51,     0,    17,    49,    62,     0,    61,     0,    29,     0,
       0,     0,    91,    90,     0,     8,     0,    43,     0,     0,
      60,     0,    17,    33,    39,    35,    15,     0,     9,    17,
      74,    18,     0,    20,    19,    83,     0,     0,     0,     0,
       0,    22,    17,    27,    10,    75,    21,     0,    28
  };

  const short
  parser::yypgoto_[] =
  {
     -60,   -60,   -60,   -60,   179,    27,   -60,    64,    -7,   -60,
     -60,   -60,   -60,    43,   -60,   -60,   -60,   -60,   -60,   -59,
      15,   -60,    79,   -35,    -2,    -9,   -60,   -60,   -60,    71
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    23,   163,   149,   164,    25,    26,    27,
      28,   111,   112,   113,    29,    80,   115,    98,    99,    72,
      73,    74,    75,    76,    33,    31,    36,    88,    89,    90
  };

  const unsigned char
  parser::yytable_[] =
  {
      30,    34,    35,   106,   100,    38,     6,    50,     6,    52,
      52,    44,    45,    46,    77,   122,     3,   106,   106,   106,
      40,    53,    53,    47,    97,    95,    53,     6,    79,   101,
      54,    54,   107,   106,    62,    54,    71,   106,    41,    43,
      82,    83,    84,    85,    87,    92,   107,   107,   107,    93,
      94,   123,   145,    71,    78,    47,    71,    71,   108,   108,
     108,   124,   107,   128,   159,    47,   107,   129,   147,    49,
      71,   134,   114,   136,   108,   131,    59,    37,   108,    39,
      17,    42,    17,    50,    51,    52,    47,   151,   157,    71,
      71,    48,   109,    47,   130,    71,   148,    53,    71,    71,
      57,    71,   132,    60,    61,   121,    54,    70,   142,    71,
      87,    81,   125,   144,   152,    58,   166,    86,    91,    55,
      71,   158,   168,    43,    71,   106,    96,    70,   102,     4,
     106,   141,   103,   154,     5,     6,     7,     8,     9,   104,
     105,    70,   110,   116,    71,   117,   118,   165,   119,   126,
      71,   127,   172,   170,   107,   135,   120,   137,   150,   107,
     138,    70,   139,   174,   140,   165,   108,   165,    10,    70,
     146,   108,    11,    49,   155,   165,    12,    13,   171,   167,
      70,    24,   153,    86,   133,    14,   169,    50,    51,    52,
     143,    70,    15,     5,     6,     7,     8,     9,     0,   177,
       0,    53,    56,   110,    16,     0,     0,     0,     0,    17,
      54,    18,    19,    20,     0,     5,     6,     7,     8,     9,
      21,    22,     0,    55,     0,     0,     0,    10,     0,     0,
     160,    11,    49,     0,     0,   161,    13,     0,     0,     0,
       0,     0,     0,     0,    14,     0,    50,    51,    52,    10,
       0,    15,   173,    11,     0,     0,     0,   161,    13,     0,
      53,     0,   162,     0,     0,     0,    14,     0,    17,    54,
      32,     0,     0,    15,     5,     6,     7,     8,     9,    21,
      22,     0,    55,     0,   162,     0,     0,     0,     0,     0,
      17,     0,    32,     0,     6,     0,     5,     6,     7,     8,
       9,    21,    22,     0,     0,     0,     0,     0,    10,     0,
       0,   175,    11,     0,     0,     0,   161,    13,     0,     5,
       6,     7,     8,     9,     0,    14,     0,    63,     0,     0,
      10,    64,    15,   178,    11,     0,     0,     0,   161,    13,
       0,     0,     0,   162,     0,     0,     0,    14,     0,    17,
       0,    32,     0,    10,    15,     0,     0,    11,     0,     0,
      21,    22,    13,     0,     0,   162,     0,     0,    17,     0,
      14,    17,    49,    32,    65,    66,    67,    68,    69,     0,
       0,     0,    21,    22,     0,     0,    50,    51,    52,     0,
       0,     0,     0,     0,    17,     0,    32,    49,     0,     0,
      53,     0,     0,     0,     0,    21,    22,     0,   156,    54,
       0,    50,    51,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   176,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55
  };

  const short
  parser::yycheck_[] =
  {
       2,    10,    11,    14,    63,    14,     6,    25,     6,    27,
      27,    18,    19,    20,    39,    11,     0,    14,    14,    14,
      64,    39,    39,    80,    39,    60,    39,     6,    85,    64,
      48,    48,    43,    14,    41,    48,    43,    14,    82,    39,
      49,    50,    51,    52,    53,    48,    43,    43,    43,    58,
      59,    47,    47,    60,    41,    80,    63,    64,    55,    55,
      55,    96,    43,    44,    41,    80,    43,   102,   127,    11,
      77,   106,    79,   108,    55,    14,    11,    13,    55,    15,
      80,    17,    80,    25,    26,    27,    80,    14,    14,    96,
      97,    47,    77,    80,   103,   102,   131,    39,   105,   106,
      44,   108,    41,    38,    40,    47,    48,    43,   117,   116,
     119,    47,    97,   122,    41,    59,   151,    53,    54,    61,
     127,    47,   157,    39,   131,    14,    38,    63,    38,     0,
      14,   116,    11,   140,     5,     6,     7,     8,     9,    40,
      59,    77,    78,    41,   151,    38,    40,   149,    59,    40,
     157,    59,    41,   162,    43,     8,    92,    40,    44,    43,
      42,    97,    59,    47,    59,   167,    55,   169,    39,   105,
      40,    55,    43,    11,    42,   177,    47,    48,    47,   152,
     116,     2,   139,   119,   105,    56,   159,    25,    26,    27,
     119,   127,    63,     5,     6,     7,     8,     9,    -1,   172,
      -1,    39,    40,   139,    75,    -1,    -1,    -1,    -1,    80,
      48,    82,    83,    84,    -1,     5,     6,     7,     8,     9,
      91,    92,    -1,    61,    -1,    -1,    -1,    39,    -1,    -1,
      42,    43,    11,    -1,    -1,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    25,    26,    27,    39,
      -1,    63,    42,    43,    -1,    -1,    -1,    47,    48,    -1,
      39,    -1,    74,    -1,    -1,    -1,    56,    -1,    80,    48,
      82,    -1,    -1,    63,     5,     6,     7,     8,     9,    91,
      92,    -1,    61,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    82,    -1,     6,    -1,     5,     6,     7,     8,
       9,    91,    92,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    42,    43,    -1,    -1,    -1,    47,    48,    -1,     5,
       6,     7,     8,     9,    -1,    56,    -1,    39,    -1,    -1,
      39,    43,    63,    42,    43,    -1,    -1,    -1,    47,    48,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    56,    -1,    80,
      -1,    82,    -1,    39,    63,    -1,    -1,    43,    -1,    -1,
      91,    92,    48,    -1,    -1,    74,    -1,    -1,    80,    -1,
      56,    80,    11,    82,    86,    87,    88,    89,    90,    -1,
      -1,    -1,    91,    92,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    82,    11,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    91,    92,    -1,    47,    48,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61
  };

  const signed char
  parser::yystos_[] =
  {
       0,    94,    95,     0,     0,     5,     6,     7,     8,     9,
      39,    43,    47,    48,    56,    63,    75,    80,    82,    83,
      84,    91,    92,    96,    97,   100,   101,   102,   103,   107,
     117,   118,    82,   117,   118,   118,   119,   100,   118,   100,
      64,    82,   100,    39,   101,   101,   101,    80,    47,    11,
      25,    26,    27,    39,    48,    61,    40,    44,    59,    11,
      38,   100,   101,    39,    43,    86,    87,    88,    89,    90,
     100,   101,   112,   113,   114,   115,   116,    39,    41,    85,
     108,   100,   118,   118,   118,   118,   100,   118,   120,   121,
     122,   100,    48,   118,   118,   116,    38,    39,   110,   111,
     112,   116,    38,    11,    40,    59,    14,    43,    55,   113,
     100,   104,   105,   106,   101,   109,    41,    38,    40,    59,
     100,    47,    11,    47,   116,   113,    40,    59,    44,   116,
     118,    14,    41,   115,   116,     8,   116,    40,    42,    59,
      59,   113,   118,   122,   118,    47,    40,   112,   116,    98,
      44,    14,    41,   106,   101,    42,    47,    14,    47,    41,
      42,    47,    74,    97,    99,   117,   116,    98,   116,    98,
     118,    47,    41,    42,    47,    42,    47,    98,    42
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    93,    94,    95,    95,    95,    95,    95,    96,    96,
      96,    96,    96,    96,    97,    97,    97,    98,    98,    98,
      99,    99,    99,   100,   101,   101,   101,   102,   102,   103,
     104,   104,   105,   105,   106,   107,   108,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   113,   113,   114,   114,
     115,   115,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   117,   117,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   119,   119,   119,   120,   120,   121,
     121,   122,   122
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     2,     2,     6,     7,
       9,     1,     1,     2,     5,     7,     5,     0,     2,     2,
       1,     3,     2,     1,     2,     1,     3,     8,    10,     5,
       0,     1,     1,     3,     1,     6,     0,     2,     1,     3,
       0,     1,     1,     3,     3,     1,     0,     1,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     3,     3,
       4,     3,     3,     4,     3,     1,     1,     1,     1,     1,
       2,     1,     1,     3,     7,     9,     3,     2,     3,     4,
       3,     3,     3,     1,     0,     1,     3,     0,     1,     1,
       3,     3,     1
  };


#if BENG_LANGDEBUG || 1
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
  "\"`#`\"", "LET", "VAR", "NEW", "FOR", "VOID_", "AUTO", "CONST_",
  "USING", "THROW", "DELETE_", "SIZEOF", "RETURN", "EXTERN", "PRIVATE",
  "TEMPLATE", "NAMESPACE", "LOWEST", "DBLCOLON", "\"`::`\"", "FN", "ENUM",
  "TYPE", "EXTENDS", "BOOL", "INT", "FLOAT", "STRING_WORD", "DECIMAL_WORD",
  "TRUE", "FALSE", "$accept", "begin", "declaration_any", "generic-decl",
  "let-decl", "infunction_declaration_any", "opt-infunction-decl",
  "identifier", "qname", "fn-decl", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "type-decl", "type-extension-any",
  "type-extension-list", "parameter-list-opt", "parameter-list",
  "parameter-decl", "parameter-woa-list-opt", "parameter-woa-list",
  "parameter-woa-decl", "type-expr", "compound-expression", "expression",
  "expression-list-any", "opt-named-expr-list-any", "opt-named-expr-list",
  "opt-named-expr", YY_NULLPTR
  };
#endif


#if BENG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   291,   291,   295,   297,   299,   301,   303,   323,   325,
     327,   329,   331,   333,   347,   349,   351,   356,   358,   360,
     368,   370,   372,   377,   386,   388,   390,   396,   400,   407,
     414,   416,   420,   423,   428,   433,   440,   442,   446,   449,
     454,   455,   458,   460,   464,   466,   473,   474,   477,   479,
     483,   485,   491,   492,   493,   494,   495,   496,   497,   499,
     501,   503,   510,   527,   529,   534,   536,   538,   540,   542,
     544,   546,   548,   550,   552,   554,   556,   558,   560,   562,
     566,   568,   570,   573,   583,   585,   587,   592,   594,   598,
     600,   608,   610
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

} // beng_lang
#line 3064 "beng.tab.cpp"

#line 952 "beng.y"

