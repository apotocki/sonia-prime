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
      case symbol_kind::S_115_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
      case symbol_kind::S_108_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
        value.copy< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_let_decl: // let-decl
      case symbol_kind::S_95_let_decl_start: // let-decl-start
      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_110_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
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

      case symbol_kind::S_105_case_decl: // case-decl
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
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // "reserved word `fn`"
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

      case symbol_kind::S_103_case_list_opt: // case-list-opt
      case symbol_kind::S_104_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_type_decl: // type-decl
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
      case symbol_kind::S_115_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
      case symbol_kind::S_108_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_94_let_decl: // let-decl
      case symbol_kind::S_95_let_decl_start: // let-decl-start
      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_110_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
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

      case symbol_kind::S_105_case_decl: // case-decl
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
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // "reserved word `fn`"
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

      case symbol_kind::S_103_case_list_opt: // case-list-opt
      case symbol_kind::S_104_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_106_type_decl: // type-decl
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
      case symbol_kind::S_115_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
      case symbol_kind::S_108_type_extension_list: // type-extension-list
        value.YY_MOVE_OR_COPY< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
        value.YY_MOVE_OR_COPY< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
        value.YY_MOVE_OR_COPY< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_let_decl: // let-decl
      case symbol_kind::S_95_let_decl_start: // let-decl-start
      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_110_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
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

      case symbol_kind::S_105_case_decl: // case-decl
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
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // "reserved word `fn`"
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

      case symbol_kind::S_103_case_list_opt: // case-list-opt
      case symbol_kind::S_104_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_type_decl: // type-decl
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
      case symbol_kind::S_115_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
      case symbol_kind::S_108_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
        value.move< generic_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_94_let_decl: // let-decl
      case symbol_kind::S_95_let_decl_start: // let-decl-start
      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_110_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
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

      case symbol_kind::S_105_case_decl: // case-decl
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
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // "reserved word `fn`"
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

      case symbol_kind::S_103_case_list_opt: // case-list-opt
      case symbol_kind::S_104_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_106_type_decl: // type-decl
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
      case symbol_kind::S_115_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (that.value);
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
      case symbol_kind::S_108_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (that.value);
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
        value.copy< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
        value.copy< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
        value.copy< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_94_let_decl: // let-decl
      case symbol_kind::S_95_let_decl_start: // let-decl-start
      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_110_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
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

      case symbol_kind::S_105_case_decl: // case-decl
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
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // "reserved word `fn`"
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

      case symbol_kind::S_103_case_list_opt: // case-list-opt
      case symbol_kind::S_104_case_list: // case-list
        value.copy< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_106_type_decl: // type-decl
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
      case symbol_kind::S_115_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (that.value);
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
      case symbol_kind::S_108_type_extension_list: // type-extension-list
        value.move< extension_list_t > (that.value);
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
        value.move< fn_decl_t > (that.value);
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
        value.move< generic_declaration_t > (that.value);
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (that.value);
        break;

      case symbol_kind::S_94_let_decl: // let-decl
      case symbol_kind::S_95_let_decl_start: // let-decl-start
      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (that.value);
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_110_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
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

      case symbol_kind::S_105_case_decl: // case-decl
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
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // "reserved word `fn`"
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

      case symbol_kind::S_103_case_list_opt: // case-list-opt
      case symbol_kind::S_104_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (that.value);
        break;

      case symbol_kind::S_106_type_decl: // type-decl
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
#line 286 "bang.y"
                 { }
#line 1177 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 286 "bang.y"
                 { }
#line 1183 "bang.tab.cpp"
        break;

      case symbol_kind::S_ARGIDENTIFIER: // ARGIDENTIFIER
#line 286 "bang.y"
                 { }
#line 1189 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 286 "bang.y"
                 { }
#line 1195 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 286 "bang.y"
                 { }
#line 1201 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 286 "bang.y"
                 { }
#line 1207 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 286 "bang.y"
                 { }
#line 1213 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 286 "bang.y"
                 { }
#line 1219 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 286 "bang.y"
                 { }
#line 1225 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 286 "bang.y"
                 { }
#line 1231 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 286 "bang.y"
                 { }
#line 1237 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 286 "bang.y"
                 { }
#line 1243 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 286 "bang.y"
                 { }
#line 1249 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 286 "bang.y"
                 { }
#line 1255 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 286 "bang.y"
                 { }
#line 1261 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 286 "bang.y"
                 { }
#line 1267 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "reserved word `fn`"
#line 286 "bang.y"
                 { }
#line 1273 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 286 "bang.y"
                 { }
#line 1279 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 286 "bang.y"
                 { }
#line 1285 "bang.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 286 "bang.y"
                 { }
#line 1291 "bang.tab.cpp"
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
#line 286 "bang.y"
                 { }
#line 1297 "bang.tab.cpp"
        break;

      case symbol_kind::S_94_let_decl: // let-decl
#line 286 "bang.y"
                 { }
#line 1303 "bang.tab.cpp"
        break;

      case symbol_kind::S_95_let_decl_start: // let-decl-start
#line 286 "bang.y"
                 { }
#line 1309 "bang.tab.cpp"
        break;

      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 286 "bang.y"
                 { }
#line 1315 "bang.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 286 "bang.y"
                 { }
#line 1321 "bang.tab.cpp"
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
#line 286 "bang.y"
                 { }
#line 1327 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 286 "bang.y"
                 { }
#line 1333 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 286 "bang.y"
                 { }
#line 1339 "bang.tab.cpp"
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
#line 286 "bang.y"
                 { }
#line 1345 "bang.tab.cpp"
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
#line 286 "bang.y"
                 { }
#line 1351 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_case_list_opt: // case-list-opt
#line 286 "bang.y"
                 { }
#line 1357 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_case_list: // case-list
#line 286 "bang.y"
                 { }
#line 1363 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_case_decl: // case-decl
#line 286 "bang.y"
                 { }
#line 1369 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_type_decl: // type-decl
#line 286 "bang.y"
                 { }
#line 1375 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
#line 286 "bang.y"
                 { }
#line 1381 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_type_extension_list: // type-extension-list
#line 286 "bang.y"
                 { }
#line 1387 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
#line 286 "bang.y"
                 { }
#line 1393 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_parameter_list: // parameter-list
#line 286 "bang.y"
                 { }
#line 1399 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
#line 286 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
#line 286 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
#line 286 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
#line 286 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_type_expr: // type-expr
#line 286 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
#line 286 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 286 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
#line 286 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
#line 286 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
#line 286 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
#line 286 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
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
      case symbol_kind::S_115_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_102_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_118_expression_list_any: // expression-list-any
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_116_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< expression_t > ();
        break;

      case symbol_kind::S_107_type_extension_any: // type-extension-any
      case symbol_kind::S_108_type_extension_list: // type-extension-list
        yylhs.value.emplace< extension_list_t > ();
        break;

      case symbol_kind::S_101_fn_decl: // fn-decl
        yylhs.value.emplace< fn_decl_t > ();
        break;

      case symbol_kind::S_93_generic_decl: // generic-decl
        yylhs.value.emplace< generic_declaration_t > ();
        break;

      case symbol_kind::S_98_opt_infunction_decl: // opt-infunction-decl
        yylhs.value.emplace< infunction_declaration_t > ();
        break;

      case symbol_kind::S_94_let_decl: // let-decl
      case symbol_kind::S_95_let_decl_start: // let-decl-start
      case symbol_kind::S_96_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement_decl_t > ();
        break;

      case symbol_kind::S_119_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_120_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_121_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_109_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_110_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_111_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_112_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_113_parameter_woa_list: // parameter-woa-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_114_parameter_woa_decl: // parameter-woa-decl
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

      case symbol_kind::S_105_case_decl: // case-decl
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
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_FN: // "reserved word `fn`"
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

      case symbol_kind::S_103_case_list_opt: // case-list-opt
      case symbol_kind::S_104_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::identifier> > ();
        break;

      case symbol_kind::S_106_type_decl: // type-decl
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
#line 291 "bang.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 1844 "bang.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 296 "bang.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 1850 "bang.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any "`;`"
#line 298 "bang.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 1856 "bang.tab.cpp"
    break;

  case 5: // declaration_any: declaration_any generic-decl
#line 300 "bang.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < generic_declaration_t > ()));
        }
#line 1865 "bang.tab.cpp"
    break;

  case 6: // generic-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 320 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = extern_var{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; }
#line 1871 "bang.tab.cpp"
    break;

  case 7: // generic-decl: EXTERN "reserved word `fn`" qname "`(`" parameter-woa-list-opt "`)`" "`;`"
#line 322 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = fn_pure_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()), bang_preliminary_tuple_t{} }; IGNORE(yystack_[5].value.as < sonia::lang::lex::resource_location > (), yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 1877 "bang.tab.cpp"
    break;

  case 8: // generic-decl: EXTERN "reserved word `fn`" qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`;`"
#line 324 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = fn_pure_decl{ std::move(yystack_[6].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > ()); }
#line 1883 "bang.tab.cpp"
    break;

  case 9: // generic-decl: INCLUDE STRING
#line 326 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 1889 "bang.tab.cpp"
    break;

  case 10: // generic-decl: enum-decl
#line 328 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 1895 "bang.tab.cpp"
    break;

  case 11: // generic-decl: type-decl
#line 330 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < type_decl > ()); }
#line 1901 "bang.tab.cpp"
    break;

  case 12: // generic-decl: fn-decl
#line 332 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < fn_decl_t > ()); }
#line 1907 "bang.tab.cpp"
    break;

  case 13: // generic-decl: let-decl
#line 334 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 1913 "bang.tab.cpp"
    break;

  case 14: // generic-decl: compound-expression "`;`"
#line 336 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1919 "bang.tab.cpp"
    break;

  case 15: // let-decl: let-decl-start-with-opt-type "`;`"
#line 350 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[1].value.as < let_statement_decl_t > ()); }
#line 1925 "bang.tab.cpp"
    break;

  case 16: // let-decl: let-decl-start-with-opt-type "`=`" expression "`;`"
#line 352 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[3].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[1].value.as < expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 1931 "bang.tab.cpp"
    break;

  case 17: // let-decl-start: LET identifier
#line 357 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 1937 "bang.tab.cpp"
    break;

  case 18: // let-decl-start: LET "weak modifier" identifier
#line 359 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 1943 "bang.tab.cpp"
    break;

  case 19: // let-decl-start-with-opt-type: let-decl-start
#line 363 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 1949 "bang.tab.cpp"
    break;

  case 20: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 365 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 1955 "bang.tab.cpp"
    break;

  case 21: // infunction_declaration_any: %empty
#line 370 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 1961 "bang.tab.cpp"
    break;

  case 22: // infunction_declaration_any: infunction_declaration_any "`;`"
#line 372 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()); }
#line 1967 "bang.tab.cpp"
    break;

  case 23: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 374 "bang.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 1976 "bang.tab.cpp"
    break;

  case 24: // opt-infunction-decl: let-decl
#line 382 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 1982 "bang.tab.cpp"
    break;

  case 25: // opt-infunction-decl: RETURN expression "`;`"
#line 384 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = return_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1988 "bang.tab.cpp"
    break;

  case 26: // opt-infunction-decl: compound-expression "`;`"
#line 386 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 1994 "bang.tab.cpp"
    break;

  case 27: // identifier: IDENTIFIER
#line 391 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2000 "bang.tab.cpp"
    break;

  case 28: // qname: DBLCOLON identifier
#line 400 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2006 "bang.tab.cpp"
    break;

  case 29: // qname: identifier
#line 402 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2012 "bang.tab.cpp"
    break;

  case 30: // qname: qname DBLCOLON identifier
#line 404 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2018 "bang.tab.cpp"
    break;

  case 31: // fn-decl: "reserved word `fn`" qname "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 410 "bang.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[6].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > (), yystack_[2].value.as < sonia::lang::lex::resource_location > ());
            }
#line 2026 "bang.tab.cpp"
    break;

  case 32: // fn-decl: "reserved word `fn`" qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 414 "bang.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[8].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < bang_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[9].value.as < sonia::lang::lex::resource_location > (), yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[2].value.as < sonia::lang::lex::resource_location > ());
            }
#line 2034 "bang.tab.cpp"
    break;

  case 33: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 421 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() + std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2042 "bang.tab.cpp"
    break;

  case 34: // case-list-opt: %empty
#line 427 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::identifier> > () = {}; }
#line 2048 "bang.tab.cpp"
    break;

  case 35: // case-list-opt: case-list
#line 429 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::identifier> > (); }
#line 2054 "bang.tab.cpp"
    break;

  case 36: // case-list: case-decl
#line 434 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::vector<sonia::lang::identifier>{std::move(yystack_[0].value.as < sonia::lang::identifier > ())}; }
#line 2060 "bang.tab.cpp"
    break;

  case 37: // case-list: case-list "," case-decl
#line 437 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::identifier > ())); }
#line 2066 "bang.tab.cpp"
    break;

  case 38: // case-decl: identifier
#line 442 "bang.y"
        { yylhs.value.as < sonia::lang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2072 "bang.tab.cpp"
    break;

  case 39: // type-decl: TYPE qname type-extension-any "`{`" parameter-woa-list-opt "`}`"
#line 447 "bang.y"
    {
        yylhs.value.as < type_decl > () = type_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() + std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[3].value.as < extension_list_t > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2080 "bang.tab.cpp"
    break;

  case 40: // type-extension-any: %empty
#line 453 "bang.y"
           { yylhs.value.as < extension_list_t > () = {}; }
#line 2086 "bang.tab.cpp"
    break;

  case 41: // type-extension-any: EXTENDS type-extension-list
#line 455 "bang.y"
                                { yylhs.value.as < extension_list_t > () = std::move(yystack_[0].value.as < extension_list_t > ()); }
#line 2092 "bang.tab.cpp"
    break;

  case 42: // type-extension-list: qname
#line 460 "bang.y"
        { yylhs.value.as < extension_list_t > () = extension_list_t{ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()))}; }
#line 2098 "bang.tab.cpp"
    break;

  case 43: // type-extension-list: type-extension-list "," qname
#line 463 "bang.y"
        { yylhs.value.as < extension_list_t > () = std::move(yystack_[2].value.as < extension_list_t > ()); yylhs.value.as < extension_list_t > ().emplace_back(ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()))); }
#line 2104 "bang.tab.cpp"
    break;

  case 44: // parameter-list-opt: %empty
#line 467 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 2110 "bang.tab.cpp"
    break;

  case 45: // parameter-list-opt: parameter-list
#line 468 "bang.y"
      { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 2116 "bang.tab.cpp"
    break;

  case 46: // parameter-list: parameter-decl
#line 473 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{std::move(yystack_[0].value.as < parameter_t > ())}; }
#line 2122 "bang.tab.cpp"
    break;

  case 47: // parameter-list: parameter-list "," parameter-decl
#line 475 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2128 "bang.tab.cpp"
    break;

  case 48: // parameter-decl: identifier "`:`" type-expr
#line 479 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2134 "bang.tab.cpp"
    break;

  case 49: // parameter-decl: type-expr
#line 481 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ nullopt, std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2140 "bang.tab.cpp"
    break;

  case 50: // parameter-woa-list-opt: %empty
#line 487 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2146 "bang.tab.cpp"
    break;

  case 51: // parameter-woa-list-opt: parameter-woa-list
#line 488 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2152 "bang.tab.cpp"
    break;

  case 52: // parameter-woa-list: parameter-woa-decl
#line 492 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{std::move(yystack_[0].value.as < parameter_woa_t > ())}; }
#line 2158 "bang.tab.cpp"
    break;

  case 53: // parameter-woa-list: parameter-woa-list "," parameter-woa-decl
#line 494 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_woa_t > ())); }
#line 2164 "bang.tab.cpp"
    break;

  case 54: // parameter-woa-decl: parameter-decl
#line 498 "bang.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t { std::move(yystack_[0].value.as < parameter_t > ()) }; }
#line 2170 "bang.tab.cpp"
    break;

  case 55: // parameter-woa-decl: parameter-decl "`=`" expression
#line 500 "bang.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t{ std::move(yystack_[2].value.as < parameter_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2176 "bang.tab.cpp"
    break;

  case 56: // type-expr: BOOL
#line 505 "bang.y"
           { yylhs.value.as < bang_preliminary_type > () = bang_bool_t{}; }
#line 2182 "bang.tab.cpp"
    break;

  case 57: // type-expr: INT
#line 506 "bang.y"
          { yylhs.value.as < bang_preliminary_type > () = bang_int_t{}; }
#line 2188 "bang.tab.cpp"
    break;

  case 58: // type-expr: FLOAT
#line 507 "bang.y"
            { yylhs.value.as < bang_preliminary_type > () = bang_float_t{}; }
#line 2194 "bang.tab.cpp"
    break;

  case 59: // type-expr: STRING_WORD
#line 508 "bang.y"
                  { yylhs.value.as < bang_preliminary_type > () = bang_string_t{}; }
#line 2200 "bang.tab.cpp"
    break;

  case 60: // type-expr: qname
#line 510 "bang.y"
            { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_object_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2206 "bang.tab.cpp"
    break;

  case 61: // type-expr: "`[`" type-expr "`]`"
#line 512 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2212 "bang.tab.cpp"
    break;

  case 62: // type-expr: "`(`" parameter-list-opt "`)`"
#line 514 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2218 "bang.tab.cpp"
    break;

  case 63: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 516 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_decimal > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2224 "bang.tab.cpp"
    break;

  case 64: // type-expr: type-expr "`|`" type-expr
#line 518 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
        }
#line 2235 "bang.tab.cpp"
    break;

  case 65: // type-expr: type-expr "`->`" type-expr
#line 525 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2241 "bang.tab.cpp"
    break;

  case 66: // compound-expression: expression "`(`" opt-named-expr-list-any "`)`"
#line 542 "bang.y"
        { yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2247 "bang.tab.cpp"
    break;

  case 67: // compound-expression: expression "`{`" opt-named-expr-list-any "`}`"
#line 544 "bang.y"
        { 
           yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) };
            //$$ = function_call_t{}; IGNORE($1, $2, $3);
        }
#line 2256 "bang.tab.cpp"
    break;

  case 68: // compound-expression: expression "`=`" expression
#line 549 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2262 "bang.tab.cpp"
    break;

  case 69: // expression: "true"
#line 554 "bang.y"
        { yylhs.value.as < expression_t > () = annotated_bool{true, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2268 "bang.tab.cpp"
    break;

  case 70: // expression: "false"
#line 556 "bang.y"
        { yylhs.value.as < expression_t > () = annotated_bool{false, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2274 "bang.tab.cpp"
    break;

  case 71: // expression: INTEGER
#line 558 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2280 "bang.tab.cpp"
    break;

  case 72: // expression: DECIMAL
#line 560 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2286 "bang.tab.cpp"
    break;

  case 73: // expression: STRING
#line 562 "bang.y"
        { yylhs.value.as < expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2292 "bang.tab.cpp"
    break;

  case 74: // expression: "`.`" identifier
#line 564 "bang.y"
        { yylhs.value.as < expression_t > () = case_expression { std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2298 "bang.tab.cpp"
    break;

  case 75: // expression: qname
#line 566 "bang.y"
        { yylhs.value.as < expression_t > () = variable_identifier{ ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ())), false}; }
#line 2304 "bang.tab.cpp"
    break;

  case 76: // expression: ARGIDENTIFIER
#line 568 "bang.y"
        { yylhs.value.as < expression_t > () = variable_identifier{ ctx.make_qname_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > (), false), true }; }
#line 2310 "bang.tab.cpp"
    break;

  case 77: // expression: "`(`" expression "`)`"
#line 570 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[1].value.as < expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2316 "bang.tab.cpp"
    break;

  case 78: // expression: "reserved word `fn`" "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 572 "bang.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[6].value.as < sonia::lang::lex::resource_location > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2322 "bang.tab.cpp"
    break;

  case 79: // expression: "reserved word `fn`" "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 576 "bang.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[7].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < bang_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[8].value.as < sonia::lang::lex::resource_location > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2328 "bang.tab.cpp"
    break;

  case 80: // expression: "`[`" expression-list-any "`]`"
#line 578 "bang.y"
        { yylhs.value.as < expression_t > () = expression_vector_t{ {std::move(yystack_[1].value.as < expression_list_t > ())}, std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2334 "bang.tab.cpp"
    break;

  case 81: // expression: "`!`" expression
#line 580 "bang.y"
                { yylhs.value.as < expression_t > () = negate_expression_t{ std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2340 "bang.tab.cpp"
    break;

  case 82: // expression: expression "`?`"
#line 582 "bang.y"
        { yylhs.value.as < expression_t > () = not_empty_expression_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 2346 "bang.tab.cpp"
    break;

  case 83: // expression: expression "`.`" identifier
#line 584 "bang.y"
        { yylhs.value.as < expression_t > () = member_expression_t { std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2352 "bang.tab.cpp"
    break;

  case 84: // expression: expression "`&&`" expression
#line 588 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2358 "bang.tab.cpp"
    break;

  case 85: // expression: expression "`||`" expression
#line 590 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2364 "bang.tab.cpp"
    break;

  case 86: // expression: expression "`+`" expression
#line 592 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2370 "bang.tab.cpp"
    break;

  case 87: // expression: expression "`..`" expression
#line 594 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2376 "bang.tab.cpp"
    break;

  case 88: // expression: compound-expression
#line 597 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 2382 "bang.tab.cpp"
    break;

  case 89: // expression-list-any: %empty
#line 607 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{}; }
#line 2388 "bang.tab.cpp"
    break;

  case 90: // expression-list-any: expression
#line 609 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 2394 "bang.tab.cpp"
    break;

  case 91: // expression-list-any: expression-list-any "," expression
#line 611 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < expression_t > ())); }
#line 2400 "bang.tab.cpp"
    break;

  case 92: // opt-named-expr-list-any: %empty
#line 616 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2406 "bang.tab.cpp"
    break;

  case 93: // opt-named-expr-list-any: opt-named-expr-list
#line 617 "bang.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2412 "bang.tab.cpp"
    break;

  case 94: // opt-named-expr-list: opt-named-expr
#line 622 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2418 "bang.tab.cpp"
    break;

  case 95: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 624 "bang.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2427 "bang.tab.cpp"
    break;

  case 96: // opt-named-expr: qname "`:`" expression
#line 632 "bang.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yystack_[2].value.as < sonia::lang::bang::annotated_qname > ().value.set_absolute(); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::tuple{std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[0].value.as < expression_t > ())}, std::move(loc)}; }
#line 2433 "bang.tab.cpp"
    break;

  case 97: // opt-named-expr: expression
#line 634 "bang.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::move(yystack_[0].value.as < expression_t > ()), std::move(loc)}; }
#line 2439 "bang.tab.cpp"
    break;


#line 2443 "bang.tab.cpp"

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
    -148,    12,   176,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
     264,   264,  -148,     9,   264,    -3,   -51,     9,    18,     0,
       1,     1,  -148,  -148,  -148,  -148,   -14,    16,  -148,   -50,
    -148,  -148,  -148,   -22,   369,    -8,  -148,    -7,   369,   -30,
    -148,   102,     9,  -148,     9,     1,  -148,  -148,   118,   -29,
     -33,    10,   118,   264,  -148,     9,  -148,   264,   264,   264,
     264,   264,   264,     9,   264,  -148,  -148,  -148,   264,  -148,
      29,   -18,   118,   118,  -148,  -148,  -148,  -148,    34,   -50,
      63,    35,    24,  -148,   154,   118,     9,     1,    46,   154,
      88,  -148,   369,   373,   173,   192,   -16,   369,    56,    48,
    -148,    69,  -148,   102,   369,   118,   118,    77,    74,  -148,
       2,   118,   264,    68,   118,   118,   122,   118,    86,  -148,
     112,    85,  -148,   -50,    97,   118,  -148,   264,  -148,   264,
    -148,   105,   107,  -148,   118,  -148,   154,   369,   118,  -148,
    -148,  -148,   121,    67,    71,  -148,     9,     1,   120,   369,
    -148,  -148,    59,  -148,    50,   220,  -148,   118,  -148,  -148,
     -50,  -148,   118,  -148,  -148,  -148,  -148,   264,  -148,  -148,
     124,   152,   239,   163,   283,   338,  -148,  -148,  -148,  -148,
    -148,  -148,   327,  -148
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    73,    27,    76,    71,    72,
       0,    89,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,     5,    13,    19,     0,    29,    75,
      12,    10,    11,    88,     0,     0,    88,     0,    90,     0,
      74,    81,     0,    17,     0,     0,    28,     9,    50,     0,
       0,    40,     0,     0,    15,     0,    14,     0,     0,     0,
       0,    92,    92,     0,     0,    82,    77,    80,     0,    18,
       0,     0,    44,     0,    56,    57,    58,    59,    29,    60,
      54,     0,    51,    52,    49,    50,    34,     0,     0,    20,
       0,    30,    68,    84,    85,    87,    75,    97,     0,    93,
      94,     0,    83,    86,    91,     0,    50,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
       0,    35,    36,    42,    41,    50,    16,     0,    66,     0,
      67,     0,     0,    62,     0,    61,    48,    55,     0,    21,
      53,    65,     0,    64,     0,    33,     0,     0,     0,    96,
      95,     6,     0,    47,     0,     0,    63,     0,    21,    37,
      43,    39,     0,     7,    21,    78,    22,     0,    24,    23,
      88,     0,     0,     0,     0,     0,    26,    21,    31,     8,
      79,    25,     0,    32
  };

  const short
  parser::yypgoto_[] =
  {
    -148,  -148,  -148,  -148,   167,  -148,  -148,  -147,  -148,    53,
      17,  -148,  -148,  -148,  -148,    27,  -148,  -148,  -148,  -148,
    -148,   -63,   -45,  -148,    61,   -17,    -2,    -9,  -148,   126,
    -148,    57
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    24,   168,    26,    27,   155,   169,    28,
      29,    30,    31,   120,   121,   122,    32,    88,   124,   107,
     108,    80,    81,    82,    83,    84,    36,    34,    39,    98,
      99,   100
  };

  const unsigned char
  parser::yytable_[] =
  {
      33,    37,    38,     6,    57,    41,     6,     6,    86,   109,
      85,   172,     3,    44,    67,     6,   115,   174,    58,    59,
      60,   106,   127,    47,    52,    56,    55,    53,    45,    68,
     182,    48,    61,    66,    62,    89,    49,    50,    51,    48,
     118,    63,    64,    55,    90,   116,   135,    55,    92,    93,
      94,    95,    97,    97,    65,   103,   110,   117,    55,   104,
      55,   132,    71,    54,   115,    79,    40,   105,    43,    79,
      46,   153,   111,   162,   112,   113,    17,    17,    96,    96,
     148,   115,   138,   114,    42,   157,    55,   125,   131,    79,
      79,   164,    87,   116,   136,    69,   128,    70,   141,    57,
     143,    78,    79,   137,   123,   117,   163,   129,    91,   139,
     116,   130,   158,    58,    59,    60,   102,   133,   149,   115,
      97,   154,    79,    79,     6,    78,   144,    61,    79,    62,
     142,    79,    79,   134,    79,   126,    63,    64,    78,   119,
     171,    61,    79,    62,   146,   173,    96,   152,   116,    65,
      63,    79,   151,   170,   145,    79,   147,    72,   175,    78,
     117,    73,   161,    65,   160,   156,   115,    78,   115,    25,
     170,   176,   170,   159,    79,   140,     4,   115,    78,    79,
     170,     5,     6,     7,     8,     9,   150,    78,   101,     0,
       0,     0,     0,   177,    17,   116,     0,   116,    58,   119,
      60,    74,    75,    76,    77,     0,   116,   117,     0,   117,
     179,     0,    61,     0,    62,    10,     0,     0,   117,    11,
       0,    63,    64,    12,    13,     5,     6,     7,     8,     9,
       0,    61,    14,    62,    65,     0,     0,     0,     0,    15,
      63,    64,    16,     0,     5,     6,     7,     8,     9,     0,
       0,     0,    17,    65,    18,    19,    20,    21,     0,    10,
       0,     0,   165,    11,    22,    23,     0,   166,    13,     5,
       6,     7,     8,     9,     0,     0,    14,     0,    10,     0,
       0,   178,    11,    15,     0,   167,   166,    13,     5,     6,
       7,     8,     9,     0,     0,    14,    17,     0,     0,    35,
       0,     0,    15,    10,   167,     0,     0,    11,    22,    23,
       0,     0,    13,     0,     0,    17,     0,     0,    35,     0,
      14,     0,    10,     0,     0,   180,    11,    22,    23,     0,
     166,    13,     5,     6,     7,     8,     9,     0,     0,    14,
      17,     0,     0,    35,     0,     0,    15,     0,   167,    57,
       0,     0,    22,    23,     0,     0,     0,     0,     0,    17,
       0,     0,    35,    58,    59,    60,    10,     0,     0,   183,
      11,    22,    23,     0,   166,    13,     0,    61,     0,    62,
      57,     0,     0,    14,     0,   181,    63,    64,     0,     0,
      15,     0,   167,     0,    58,    59,    60,     0,     0,    65,
      60,     0,     0,    17,     0,     0,    35,     0,    61,     0,
      62,     0,    61,     0,    62,    22,    23,    63,    64,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,    65
  };

  const short
  parser::yycheck_[] =
  {
       2,    10,    11,     6,    11,    14,     6,     6,    41,    72,
      39,   158,     0,    64,    44,     6,    14,   164,    25,    26,
      27,    39,    38,     5,    38,    47,    76,    11,    79,    59,
     177,    39,    39,    40,    41,    52,    19,    20,    21,    39,
      85,    48,    49,    76,    53,    43,    44,    76,    57,    58,
      59,    60,    61,    62,    61,    64,    73,    55,    76,    68,
      76,   106,    45,    47,    14,    48,    13,    38,    15,    52,
      17,   134,    38,    14,    11,    40,    76,    76,    61,    62,
     125,    14,    14,    59,    87,    14,    76,    41,   105,    72,
      73,    41,    82,    43,   111,    42,    40,    44,   115,    11,
     117,    48,    85,   112,    87,    55,    47,    59,    55,    41,
      43,    42,    41,    25,    26,    27,    63,    40,   127,    14,
     129,   138,   105,   106,     6,    72,    40,    39,   111,    41,
       8,   114,   115,    59,   117,    47,    48,    49,    85,    86,
     157,    39,   125,    41,    59,   162,   129,    40,    43,    61,
      48,   134,    47,   155,    42,   138,    59,    39,   167,   106,
      55,    43,    42,    61,   147,    44,    14,   114,    14,     2,
     172,    47,   174,   146,   157,   114,     0,    14,   125,   162,
     182,     5,     6,     7,     8,     9,   129,   134,    62,    -1,
      -1,    -1,    -1,    41,    76,    43,    -1,    43,    25,   146,
      27,    83,    84,    85,    86,    -1,    43,    55,    -1,    55,
      47,    -1,    39,    -1,    41,    39,    -1,    -1,    55,    43,
      -1,    48,    49,    47,    48,     5,     6,     7,     8,     9,
      -1,    39,    56,    41,    61,    -1,    -1,    -1,    -1,    63,
      48,    49,    66,    -1,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    76,    61,    78,    79,    80,    81,    -1,    39,
      -1,    -1,    42,    43,    88,    89,    -1,    47,    48,     5,
       6,     7,     8,     9,    -1,    -1,    56,    -1,    39,    -1,
      -1,    42,    43,    63,    -1,    65,    47,    48,     5,     6,
       7,     8,     9,    -1,    -1,    56,    76,    -1,    -1,    79,
      -1,    -1,    63,    39,    65,    -1,    -1,    43,    88,    89,
      -1,    -1,    48,    -1,    -1,    76,    -1,    -1,    79,    -1,
      56,    -1,    39,    -1,    -1,    42,    43,    88,    89,    -1,
      47,    48,     5,     6,     7,     8,     9,    -1,    -1,    56,
      76,    -1,    -1,    79,    -1,    -1,    63,    -1,    65,    11,
      -1,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    79,    25,    26,    27,    39,    -1,    -1,    42,
      43,    88,    89,    -1,    47,    48,    -1,    39,    -1,    41,
      11,    -1,    -1,    56,    -1,    47,    48,    49,    -1,    -1,
      63,    -1,    65,    -1,    25,    26,    27,    -1,    -1,    61,
      27,    -1,    -1,    76,    -1,    -1,    79,    -1,    39,    -1,
      41,    -1,    39,    -1,    41,    88,    89,    48,    49,    -1,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    61
  };

  const signed char
  parser::yystos_[] =
  {
       0,    91,    92,     0,     0,     5,     6,     7,     8,     9,
      39,    43,    47,    48,    56,    63,    66,    76,    78,    79,
      80,    81,    88,    89,    93,    94,    95,    96,    99,   100,
     101,   102,   106,   116,   117,    79,   116,   117,   117,   118,
      99,   117,    87,    99,    64,    79,    99,     5,    39,   100,
     100,   100,    38,    11,    47,    76,    47,    11,    25,    26,
      27,    39,    41,    48,    49,    61,    40,    44,    59,    99,
      99,   100,    39,    43,    83,    84,    85,    86,    99,   100,
     111,   112,   113,   114,   115,    39,    41,    82,   107,   115,
     117,    99,   117,   117,   117,   117,   100,   117,   119,   120,
     121,   119,    99,   117,   117,    38,    39,   109,   110,   111,
     115,    38,    11,    40,    59,    14,    43,    55,   112,    99,
     103,   104,   105,   100,   108,    41,    47,    38,    40,    59,
      42,   115,   112,    40,    59,    44,   115,   117,    14,    41,
     114,   115,     8,   115,    40,    42,    59,    59,   112,   117,
     121,    47,    40,   111,   115,    97,    44,    14,    41,   105,
     100,    42,    14,    47,    41,    42,    47,    65,    94,    98,
     116,   115,    97,   115,    97,   117,    47,    41,    42,    47,
      42,    47,    97,    42
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    90,    91,    92,    92,    92,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    94,    94,    95,    95,    96,
      96,    97,    97,    97,    98,    98,    98,    99,   100,   100,
     100,   101,   101,   102,   103,   103,   104,   104,   105,   106,
     107,   107,   108,   108,   109,   109,   110,   110,   111,   111,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   116,   116,   116,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   118,
     118,   118,   119,   119,   120,   120,   121,   121
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
       1,     3,     3,     4,     3,     3,     4,     4,     3,     1,
       1,     1,     1,     1,     2,     1,     1,     3,     7,     9,
       3,     2,     2,     3,     3,     3,     3,     3,     1,     0,
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
  "\"`::`\"", "INCLUDE", "\"reserved word `fn`\"", "ENUM", "TYPE",
  "EXTENDS", "BOOL", "INT", "FLOAT", "STRING_WORD", "\"weak modifier\"",
  "\"true\"", "\"false\"", "$accept", "begin", "declaration_any",
  "generic-decl", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction_declaration_any",
  "opt-infunction-decl", "identifier", "qname", "fn-decl", "enum-decl",
  "case-list-opt", "case-list", "case-decl", "type-decl",
  "type-extension-any", "type-extension-list", "parameter-list-opt",
  "parameter-list", "parameter-decl", "parameter-woa-list-opt",
  "parameter-woa-list", "parameter-woa-decl", "type-expr",
  "compound-expression", "expression", "expression-list-any",
  "opt-named-expr-list-any", "opt-named-expr-list", "opt-named-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   291,   291,   295,   297,   299,   319,   321,   323,   325,
     327,   329,   331,   333,   335,   349,   351,   356,   358,   363,
     364,   369,   371,   373,   381,   383,   385,   390,   399,   401,
     403,   409,   413,   420,   427,   429,   433,   436,   441,   446,
     453,   455,   459,   462,   467,   468,   472,   474,   478,   480,
     487,   488,   491,   493,   497,   499,   505,   506,   507,   508,
     510,   511,   513,   515,   517,   524,   541,   543,   548,   553,
     555,   557,   559,   561,   563,   565,   567,   569,   571,   575,
     577,   579,   581,   583,   587,   589,   591,   593,   596,   606,
     608,   610,   615,   617,   621,   623,   631,   633
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
      85,    86,    87,    88,    89
    };
    // Last valid token kind.
    const int code_max = 344;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3153 "bang.tab.cpp"

#line 968 "bang.y"

