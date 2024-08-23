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
      case symbol_kind::S_122_type_expr: // type-expr
        value.copy< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
      case symbol_kind::S_110_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
      case symbol_kind::S_112_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_field_decl: // field-decl
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.copy< fn_decl_t > (YY_MOVE (that.value));
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

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
        value.copy< parameter_type_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_118_parameter_list: // parameter-list
      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_type_decl: // type-decl
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
      case symbol_kind::S_122_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
      case symbol_kind::S_110_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
      case symbol_kind::S_112_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_113_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (s.value));
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

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
        value.move< parameter_type_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_118_parameter_list: // parameter-list
      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_107_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_type_decl: // type-decl
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
      case symbol_kind::S_122_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.YY_MOVE_OR_COPY< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
      case symbol_kind::S_110_type_extension_list: // type-extension-list
        value.YY_MOVE_OR_COPY< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
      case symbol_kind::S_112_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_field_decl: // field-decl
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_decl_t > (YY_MOVE (that.value));
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

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
        value.YY_MOVE_OR_COPY< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
        value.YY_MOVE_OR_COPY< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
        value.YY_MOVE_OR_COPY< parameter_type_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_118_parameter_list: // parameter-list
      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_type_decl: // type-decl
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
      case symbol_kind::S_122_type_expr: // type-expr
        value.move< bang_preliminary_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
      case symbol_kind::S_110_type_extension_list: // type-extension-list
        value.move< extension_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
      case symbol_kind::S_112_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_113_field_decl: // field-decl
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.move< fn_decl_t > (YY_MOVE (that.value));
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

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
        value.move< parameter_type_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_118_parameter_list: // parameter-list
      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_107_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_type_decl: // type-decl
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
      case symbol_kind::S_122_type_expr: // type-expr
        value.copy< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.copy< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.copy< expression_t > (that.value);
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
      case symbol_kind::S_110_type_extension_list: // type-extension-list
        value.copy< extension_list_t > (that.value);
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
      case symbol_kind::S_112_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_113_field_decl: // field-decl
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.copy< fn_decl_t > (that.value);
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

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
        value.copy< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
        value.copy< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
        value.copy< parameter_type_t > (that.value);
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_118_parameter_list: // parameter-list
      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_107_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_108_type_decl: // type-decl
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
      case symbol_kind::S_122_type_expr: // type-expr
        value.move< bang_preliminary_type > (that.value);
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< declaration_set_t > (that.value);
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (that.value);
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
      case symbol_kind::S_110_type_extension_list: // type-extension-list
        value.move< extension_list_t > (that.value);
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
      case symbol_kind::S_112_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_113_field_decl: // field-decl
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        value.move< fn_decl_t > (that.value);
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

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (that.value);
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (that.value);
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
        value.move< parameter_type_t > (that.value);
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_118_parameter_list: // parameter-list
      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_107_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_108_type_decl: // type-decl
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
#line 294 "bang.y"
                 { }
#line 1267 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 294 "bang.y"
                 { }
#line 1273 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 294 "bang.y"
                 { }
#line 1279 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 294 "bang.y"
                 { }
#line 1285 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 294 "bang.y"
                 { }
#line 1291 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 294 "bang.y"
                 { }
#line 1297 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 294 "bang.y"
                 { }
#line 1303 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 294 "bang.y"
                 { }
#line 1309 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 294 "bang.y"
                 { }
#line 1315 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 294 "bang.y"
                 { }
#line 1321 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 294 "bang.y"
                 { }
#line 1327 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 294 "bang.y"
                 { }
#line 1333 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 294 "bang.y"
                 { }
#line 1339 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 294 "bang.y"
                 { }
#line 1345 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 294 "bang.y"
                 { }
#line 1351 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 294 "bang.y"
                 { }
#line 1357 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 294 "bang.y"
                 { }
#line 1363 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "reserved word `fn`"
#line 294 "bang.y"
                 { }
#line 1369 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 294 "bang.y"
                 { }
#line 1375 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 294 "bang.y"
                 { }
#line 1381 "bang.tab.cpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 294 "bang.y"
                 { }
#line 1387 "bang.tab.cpp"
        break;

      case symbol_kind::S_95_generic_decl: // generic-decl
#line 294 "bang.y"
                 { }
#line 1393 "bang.tab.cpp"
        break;

      case symbol_kind::S_96_let_decl: // let-decl
#line 294 "bang.y"
                 { }
#line 1399 "bang.tab.cpp"
        break;

      case symbol_kind::S_97_let_decl_start: // let-decl-start
#line 294 "bang.y"
                 { }
#line 1405 "bang.tab.cpp"
        break;

      case symbol_kind::S_98_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 294 "bang.y"
                 { }
#line 1411 "bang.tab.cpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 294 "bang.y"
                 { }
#line 1417 "bang.tab.cpp"
        break;

      case symbol_kind::S_100_opt_infunction_decl: // opt-infunction-decl
#line 294 "bang.y"
                 { }
#line 1423 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 294 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 294 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
#line 294 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
#line 294 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_case_list_opt: // case-list-opt
#line 294 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_case_list: // case-list
#line 294 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_case_decl: // case-decl
#line 294 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_type_decl: // type-decl
#line 294 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
#line 294 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_type_extension_list: // type-extension-list
#line 294 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
#line 294 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_field_list: // field-list
#line 294 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_field_decl: // field-decl
#line 294 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
#line 294 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
#line 294 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
#line 294 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
#line 294 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_parameter_list: // parameter-list
#line 294 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
#line 294 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
#line 294 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
#line 294 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_type_expr: // type-expr
#line 294 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
#line 294 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 294 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
#line 294 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
#line 294 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
#line 294 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
#line 294 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
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
      case symbol_kind::S_122_type_expr: // type-expr
        yylhs.value.emplace< bang_preliminary_type > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        yylhs.value.emplace< declaration_set_t > ();
        break;

      case symbol_kind::S_104_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_125_expression_list_any: // expression-list-any
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_123_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< expression_t > ();
        break;

      case symbol_kind::S_109_type_extension_any: // type-extension-any
      case symbol_kind::S_110_type_extension_list: // type-extension-list
        yylhs.value.emplace< extension_list_t > ();
        break;

      case symbol_kind::S_111_field_list_opt: // field-list-opt
      case symbol_kind::S_112_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_113_field_decl: // field-decl
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_103_fn_decl: // fn-decl
        yylhs.value.emplace< fn_decl_t > ();
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

      case symbol_kind::S_126_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_127_opt_named_expr_list: // opt-named-expr-list
        yylhs.value.emplace< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_128_opt_named_expr: // opt-named-expr
        yylhs.value.emplace< named_expression_term_t > ();
        break;

      case symbol_kind::S_114_parameter_decl_name: // parameter-decl-name
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_116_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_115_parameter_decl_type: // parameter-decl-type
        yylhs.value.emplace< parameter_type_t > ();
        break;

      case symbol_kind::S_117_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_118_parameter_list: // parameter-list
      case symbol_kind::S_119_parameter_woa_list_opt: // parameter-woa-list-opt
      case symbol_kind::S_120_parameter_woa_list: // parameter-woa-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_121_parameter_woa_decl: // parameter-woa-decl
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
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_107_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::bang::identifier > ();
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
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

      case symbol_kind::S_105_case_list_opt: // case-list-opt
      case symbol_kind::S_106_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_108_type_decl: // type-decl
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
#line 299 "bang.y"
                            { ctx.set_declarations(std::move(yystack_[1].value.as < declaration_set_t > ())); }
#line 1985 "bang.tab.cpp"
    break;

  case 3: // declaration_any: %empty
#line 304 "bang.y"
        { yylhs.value.as < declaration_set_t > () = {}; }
#line 1991 "bang.tab.cpp"
    break;

  case 4: // declaration_any: declaration_any "`;`"
#line 306 "bang.y"
        { yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ()); }
#line 1997 "bang.tab.cpp"
    break;

  case 5: // declaration_any: declaration_any generic-decl
#line 308 "bang.y"
        {
	        yylhs.value.as < declaration_set_t > () = std::move(yystack_[1].value.as < declaration_set_t > ());
            yylhs.value.as < declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < generic_declaration_t > ()));
        }
#line 2006 "bang.tab.cpp"
    break;

  case 6: // generic-decl: EXTERN VAR identifier "`:`" type-expr "`;`"
#line 328 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = extern_var{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; }
#line 2012 "bang.tab.cpp"
    break;

  case 7: // generic-decl: EXTERN "reserved word `fn`" qname "`(`" parameter-list-opt "`)`" "`;`"
#line 330 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = fn_pure_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[5].value.as < sonia::lang::lex::resource_location > (), yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2018 "bang.tab.cpp"
    break;

  case 8: // generic-decl: EXTERN "reserved word `fn`" qname "`(`" parameter-list-opt "`)`" "`->`" type-expr "`;`"
#line 332 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = fn_pure_decl{ std::move(yystack_[6].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), std::move(yystack_[1].value.as < bang_preliminary_type > ()) }; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > ()); }
#line 2024 "bang.tab.cpp"
    break;

  case 9: // generic-decl: INCLUDE STRING
#line 334 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2030 "bang.tab.cpp"
    break;

  case 10: // generic-decl: enum-decl
#line 336 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2036 "bang.tab.cpp"
    break;

  case 11: // generic-decl: type-decl
#line 338 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < type_decl > ()); }
#line 2042 "bang.tab.cpp"
    break;

  case 12: // generic-decl: fn-decl
#line 340 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < fn_decl_t > ()); }
#line 2048 "bang.tab.cpp"
    break;

  case 13: // generic-decl: let-decl
#line 342 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2054 "bang.tab.cpp"
    break;

  case 14: // generic-decl: compound-expression "`;`"
#line 344 "bang.y"
        { yylhs.value.as < generic_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 2060 "bang.tab.cpp"
    break;

  case 15: // let-decl: let-decl-start-with-opt-type "`;`"
#line 358 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[1].value.as < let_statement_decl_t > ()); }
#line 2066 "bang.tab.cpp"
    break;

  case 16: // let-decl: let-decl-start-with-opt-type "`=`" expression "`;`"
#line 360 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[3].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().expression = std::move(yystack_[1].value.as < expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2072 "bang.tab.cpp"
    break;

  case 17: // let-decl-start: LET identifier
#line 365 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2078 "bang.tab.cpp"
    break;

  case 18: // let-decl-start: LET "weak modifier" identifier
#line 367 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = let_statement_decl_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2084 "bang.tab.cpp"
    break;

  case 19: // let-decl-start-with-opt-type: let-decl-start
#line 371 "bang.y"
      { yylhs.value.as < let_statement_decl_t > () = yystack_[0].value.as < let_statement_decl_t > (); }
#line 2090 "bang.tab.cpp"
    break;

  case 20: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 373 "bang.y"
        { yylhs.value.as < let_statement_decl_t > () = std::move(yystack_[2].value.as < let_statement_decl_t > ()); yylhs.value.as < let_statement_decl_t > ().type = std::move(yystack_[0].value.as < bang_preliminary_type > ()); }
#line 2096 "bang.tab.cpp"
    break;

  case 21: // infunction_declaration_any: %empty
#line 378 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = {}; }
#line 2102 "bang.tab.cpp"
    break;

  case 22: // infunction_declaration_any: infunction_declaration_any "`;`"
#line 380 "bang.y"
        { yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()); }
#line 2108 "bang.tab.cpp"
    break;

  case 23: // infunction_declaration_any: infunction_declaration_any opt-infunction-decl
#line 382 "bang.y"
        {
	        yylhs.value.as < std::vector<infunction_declaration_t> > () = std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ());
            yylhs.value.as < std::vector<infunction_declaration_t> > ().emplace_back(std::move(yystack_[0].value.as < infunction_declaration_t > ()));
        }
#line 2117 "bang.tab.cpp"
    break;

  case 24: // opt-infunction-decl: let-decl
#line 390 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = std::move(yystack_[0].value.as < let_statement_decl_t > ()); }
#line 2123 "bang.tab.cpp"
    break;

  case 25: // opt-infunction-decl: RETURN expression "`;`"
#line 392 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = return_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 2129 "bang.tab.cpp"
    break;

  case 26: // opt-infunction-decl: compound-expression "`;`"
#line 394 "bang.y"
        { yylhs.value.as < infunction_declaration_t > () = expression_decl_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 2135 "bang.tab.cpp"
    break;

  case 27: // identifier: IDENTIFIER
#line 399 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2141 "bang.tab.cpp"
    break;

  case 28: // qname: DBLCOLON identifier
#line 408 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2147 "bang.tab.cpp"
    break;

  case 29: // qname: identifier
#line 410 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2153 "bang.tab.cpp"
    break;

  case 30: // qname: qname DBLCOLON identifier
#line 412 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2159 "bang.tab.cpp"
    break;

  case 31: // fn-decl: "reserved word `fn`" qname "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 418 "bang.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[6].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[5].value.as < sonia::lang::lex::resource_location > (), yystack_[2].value.as < sonia::lang::lex::resource_location > ());
            }
#line 2167 "bang.tab.cpp"
    break;

  case 32: // fn-decl: "reserved word `fn`" qname "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 422 "bang.y"
            {
                yylhs.value.as < fn_decl_t > () = fn_decl_t{std::move(yystack_[8].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < bang_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ())}; IGNORE(yystack_[9].value.as < sonia::lang::lex::resource_location > (), yystack_[7].value.as < sonia::lang::lex::resource_location > (), yystack_[2].value.as < sonia::lang::lex::resource_location > ());
            }
#line 2175 "bang.tab.cpp"
    break;

  case 33: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 429 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2183 "bang.tab.cpp"
    break;

  case 34: // case-list-opt: %empty
#line 435 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2189 "bang.tab.cpp"
    break;

  case 35: // case-list-opt: case-list
#line 437 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2195 "bang.tab.cpp"
    break;

  case 36: // case-list: case-decl
#line 442 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2201 "bang.tab.cpp"
    break;

  case 37: // case-list: case-list "," case-decl
#line 445 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2207 "bang.tab.cpp"
    break;

  case 38: // case-decl: identifier
#line 450 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2213 "bang.tab.cpp"
    break;

  case 39: // type-decl: TYPE qname type-extension-any "`{`" parameter-woa-list-opt "`}`"
#line 455 "bang.y"
    {
        yylhs.value.as < type_decl > () = type_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[4].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[3].value.as < extension_list_t > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2221 "bang.tab.cpp"
    break;

  case 40: // type-extension-any: %empty
#line 461 "bang.y"
           { yylhs.value.as < extension_list_t > () = {}; }
#line 2227 "bang.tab.cpp"
    break;

  case 41: // type-extension-any: EXTENDS type-extension-list
#line 463 "bang.y"
                                { yylhs.value.as < extension_list_t > () = std::move(yystack_[0].value.as < extension_list_t > ()); }
#line 2233 "bang.tab.cpp"
    break;

  case 42: // type-extension-list: qname
#line 468 "bang.y"
        { yylhs.value.as < extension_list_t > () = extension_list_t{ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()))}; }
#line 2239 "bang.tab.cpp"
    break;

  case 43: // type-extension-list: type-extension-list "," qname
#line 471 "bang.y"
        { yylhs.value.as < extension_list_t > () = std::move(yystack_[2].value.as < extension_list_t > ()); yylhs.value.as < extension_list_t > ().emplace_back(ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()))); }
#line 2245 "bang.tab.cpp"
    break;

  case 44: // field-list-opt: %empty
#line 475 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2251 "bang.tab.cpp"
    break;

  case 45: // field-list-opt: field-list
#line 476 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2257 "bang.tab.cpp"
    break;

  case 46: // field-list: field-decl
#line 481 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2263 "bang.tab.cpp"
    break;

  case 47: // field-list: field-list "," field-decl
#line 483 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2269 "bang.tab.cpp"
    break;

  case 48: // field-decl: IDENTIFIER "`:`" type-expr
#line 488 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ ctx.make_identifier(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2275 "bang.tab.cpp"
    break;

  case 49: // field-decl: type-expr
#line 490 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[0].value.as < bang_preliminary_type > ()) }; }
#line 2281 "bang.tab.cpp"
    break;

  case 50: // parameter-decl-name: IDENTIFIER INTERNAL_IDENTIFIER "`:`"
#line 495 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ()), ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())}; }
#line 2287 "bang.tab.cpp"
    break;

  case 51: // parameter-decl-name: IDENTIFIER "`:`"
#line 497 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ()), nullopt}; }
#line 2293 "bang.tab.cpp"
    break;

  case 52: // parameter-decl-name: INTERNAL_IDENTIFIER "`:`"
#line 499 "bang.y"
        { yylhs.value.as < parameter_name > () = parameter_name{nullopt, ctx.make_identifier(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())}; }
#line 2299 "bang.tab.cpp"
    break;

  case 53: // parameter-decl-type: "const modifier" type-expr "`...`"
#line 504 "bang.y"
        { yylhs.value.as < parameter_type_t > () = parameter_type_t{bang_preliminary_parameter_pack_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}, true}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2305 "bang.tab.cpp"
    break;

  case 54: // parameter-decl-type: "const modifier" type-expr
#line 506 "bang.y"
        { yylhs.value.as < parameter_type_t > () = parameter_type_t{std::move(yystack_[0].value.as < bang_preliminary_type > ()), true}; }
#line 2311 "bang.tab.cpp"
    break;

  case 55: // parameter-decl-type: type-expr "`...`"
#line 508 "bang.y"
        { yylhs.value.as < parameter_type_t > () = parameter_type_t{bang_preliminary_parameter_pack_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}, false}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2317 "bang.tab.cpp"
    break;

  case 56: // parameter-decl-type: type-expr
#line 510 "bang.y"
        { yylhs.value.as < parameter_type_t > () = parameter_type_t{std::move(yystack_[0].value.as < bang_preliminary_type > ()), false}; }
#line 2323 "bang.tab.cpp"
    break;

  case 57: // parameter-decl: parameter-decl-name parameter-decl-type
#line 515 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[1].value.as < parameter_name > ()), std::move(yystack_[0].value.as < parameter_type_t > ()) }; }
#line 2329 "bang.tab.cpp"
    break;

  case 58: // parameter-decl: parameter-decl-type
#line 517 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[0].value.as < parameter_type_t > ()) }; }
#line 2335 "bang.tab.cpp"
    break;

  case 59: // parameter-list-opt: %empty
#line 521 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2341 "bang.tab.cpp"
    break;

  case 60: // parameter-list-opt: parameter-list
#line 522 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2347 "bang.tab.cpp"
    break;

  case 61: // parameter-list: parameter-decl
#line 527 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2353 "bang.tab.cpp"
    break;

  case 62: // parameter-list: parameter-list "," parameter-decl
#line 529 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2359 "bang.tab.cpp"
    break;

  case 63: // parameter-woa-list-opt: %empty
#line 533 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2365 "bang.tab.cpp"
    break;

  case 64: // parameter-woa-list-opt: parameter-woa-list
#line 534 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2371 "bang.tab.cpp"
    break;

  case 65: // parameter-woa-list: parameter-woa-decl
#line 539 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{std::move(yystack_[0].value.as < parameter_woa_t > ())}; }
#line 2377 "bang.tab.cpp"
    break;

  case 66: // parameter-woa-list: parameter-woa-list "," parameter-woa-decl
#line 541 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_woa_t > ())); }
#line 2383 "bang.tab.cpp"
    break;

  case 67: // parameter-woa-decl: parameter-decl
#line 546 "bang.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t { std::move(yystack_[0].value.as < parameter_t > ()) }; }
#line 2389 "bang.tab.cpp"
    break;

  case 68: // parameter-woa-decl: parameter-decl "`=`" expression
#line 548 "bang.y"
        { yylhs.value.as < parameter_woa_t > () = parameter_woa_t{ std::move(yystack_[2].value.as < parameter_t > ()), std::move(yystack_[0].value.as < expression_t > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2395 "bang.tab.cpp"
    break;

  case 69: // type-expr: BOOL
#line 553 "bang.y"
           { yylhs.value.as < bang_preliminary_type > () = bang_bool_t{}; }
#line 2401 "bang.tab.cpp"
    break;

  case 70: // type-expr: INT
#line 554 "bang.y"
          { yylhs.value.as < bang_preliminary_type > () = bang_int_t{}; }
#line 2407 "bang.tab.cpp"
    break;

  case 71: // type-expr: FLOAT
#line 555 "bang.y"
            { yylhs.value.as < bang_preliminary_type > () = bang_float_t{}; }
#line 2413 "bang.tab.cpp"
    break;

  case 72: // type-expr: qname
#line 558 "bang.y"
            { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_object_t{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2419 "bang.tab.cpp"
    break;

  case 73: // type-expr: "`[`" type-expr "`]`"
#line 560 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_vector_t{std::move(yystack_[1].value.as < bang_preliminary_type > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2425 "bang.tab.cpp"
    break;

  case 74: // type-expr: "`(`" field-list-opt "`)`"
#line 562 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_tuple_t { std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2431 "bang.tab.cpp"
    break;

  case 75: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 564 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_array_t{std::move(yystack_[3].value.as < bang_preliminary_type > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_decimal > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2437 "bang.tab.cpp"
    break;

  case 76: // type-expr: type-expr "`|`" type-expr
#line 566 "bang.y"
        {
            bang_preliminary_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < bang_preliminary_type > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < bang_preliminary_type > ()));
            yylhs.value.as < bang_preliminary_type > () = std::move(uni);
        }
#line 2448 "bang.tab.cpp"
    break;

  case 77: // type-expr: type-expr "`->`" type-expr
#line 573 "bang.y"
        { yylhs.value.as < bang_preliminary_type > () = bang_preliminary_fn_t{std::move(yystack_[2].value.as < bang_preliminary_type > ()), std::move(yystack_[0].value.as < bang_preliminary_type > ())}; }
#line 2454 "bang.tab.cpp"
    break;

  case 78: // compound-expression: expression "`(`" opt-named-expr-list-any "`)`"
#line 590 "bang.y"
        { yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) }; }
#line 2460 "bang.tab.cpp"
    break;

  case 79: // compound-expression: expression "`{`" opt-named-expr-list-any "`}`"
#line 592 "bang.y"
        { 
           yylhs.value.as < expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < expression_t > ()), std::move(yystack_[1].value.as < named_expression_term_list_t > ()) };
            //$$ = function_call_t{}; IGNORE($1, $2, $3);
        }
#line 2469 "bang.tab.cpp"
    break;

  case 80: // compound-expression: expression "`=`" expression
#line 597 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2475 "bang.tab.cpp"
    break;

  case 81: // expression: "true"
#line 602 "bang.y"
        { yylhs.value.as < expression_t > () = annotated_bool{true, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2481 "bang.tab.cpp"
    break;

  case 82: // expression: "false"
#line 604 "bang.y"
        { yylhs.value.as < expression_t > () = annotated_bool{false, yystack_[0].value.as < sonia::lang::lex::resource_location > ()}; }
#line 2487 "bang.tab.cpp"
    break;

  case 83: // expression: INTEGER
#line 606 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2493 "bang.tab.cpp"
    break;

  case 84: // expression: DECIMAL
#line 608 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2499 "bang.tab.cpp"
    break;

  case 85: // expression: STRING
#line 610 "bang.y"
        { yylhs.value.as < expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2505 "bang.tab.cpp"
    break;

  case 86: // expression: "`.`" identifier
#line 612 "bang.y"
        { yylhs.value.as < expression_t > () = case_expression { std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2511 "bang.tab.cpp"
    break;

  case 87: // expression: qname
#line 614 "bang.y"
        { yylhs.value.as < expression_t > () = variable_identifier{ ctx.make_qname_identifier(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ())), false}; }
#line 2517 "bang.tab.cpp"
    break;

  case 88: // expression: INTERNAL_IDENTIFIER
#line 616 "bang.y"
        { yylhs.value.as < expression_t > () = variable_identifier{ ctx.make_qname_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > (), false), true }; }
#line 2523 "bang.tab.cpp"
    break;

  case 89: // expression: "`(`" expression "`)`"
#line 618 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[1].value.as < expression_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2529 "bang.tab.cpp"
    break;

  case 90: // expression: "reserved word `fn`" "`(`" parameter-woa-list-opt "`)`" "`{`" infunction_declaration_any "`}`"
#line 620 "bang.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[4].value.as < parameter_woa_list_t > ()), nullopt, std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[6].value.as < sonia::lang::lex::resource_location > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2535 "bang.tab.cpp"
    break;

  case 91: // expression: "reserved word `fn`" "`(`" parameter-woa-list-opt "`)`" "`->`" type-expr "`{`" infunction_declaration_any "`}`"
#line 624 "bang.y"
        { yylhs.value.as < expression_t > () = lambda_t{annotated_qname{qname{ctx.new_identifier()}, std::move(yystack_[7].value.as < sonia::lang::lex::resource_location > ())}, std::move(yystack_[6].value.as < parameter_woa_list_t > ()), std::move(yystack_[3].value.as < bang_preliminary_type > ()), std::move(yystack_[1].value.as < std::vector<infunction_declaration_t> > ()), std::move(yystack_[8].value.as < sonia::lang::lex::resource_location > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2541 "bang.tab.cpp"
    break;

  case 92: // expression: "`[`" expression-list-any "`]`"
#line 626 "bang.y"
        { yylhs.value.as < expression_t > () = expression_vector_t{ {std::move(yystack_[1].value.as < expression_list_t > ())}, std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2547 "bang.tab.cpp"
    break;

  case 93: // expression: "`!`" expression
#line 628 "bang.y"
                { yylhs.value.as < expression_t > () = negate_expression_t{ std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2553 "bang.tab.cpp"
    break;

  case 94: // expression: expression "`?`"
#line 630 "bang.y"
        { yylhs.value.as < expression_t > () = not_empty_expression_t{ std::move(yystack_[1].value.as < expression_t > ()) }; }
#line 2559 "bang.tab.cpp"
    break;

  case 95: // expression: expression "`.`" identifier
#line 632 "bang.y"
        { yylhs.value.as < expression_t > () = member_expression_t { std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2565 "bang.tab.cpp"
    break;

  case 96: // expression: expression "`&&`" expression
#line 636 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2571 "bang.tab.cpp"
    break;

  case 97: // expression: expression "`||`" expression
#line 638 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2577 "bang.tab.cpp"
    break;

  case 98: // expression: expression "`+`" expression
#line 640 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2583 "bang.tab.cpp"
    break;

  case 99: // expression: expression "`..`" expression
#line 642 "bang.y"
        { yylhs.value.as < expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < expression_t > ()), std::move(yystack_[0].value.as < expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2589 "bang.tab.cpp"
    break;

  case 100: // expression: compound-expression
#line 645 "bang.y"
        { yylhs.value.as < expression_t > () = std::move(yystack_[0].value.as < expression_t > ()); }
#line 2595 "bang.tab.cpp"
    break;

  case 101: // expression-list-any: %empty
#line 655 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{}; }
#line 2601 "bang.tab.cpp"
    break;

  case 102: // expression-list-any: expression
#line 657 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < expression_t > ()) }; }
#line 2607 "bang.tab.cpp"
    break;

  case 103: // expression-list-any: expression-list-any "," expression
#line 659 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < expression_t > ())); }
#line 2613 "bang.tab.cpp"
    break;

  case 104: // opt-named-expr-list-any: %empty
#line 664 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{}; }
#line 2619 "bang.tab.cpp"
    break;

  case 105: // opt-named-expr-list-any: opt-named-expr-list
#line 665 "bang.y"
      { yylhs.value.as < named_expression_term_list_t > () = yystack_[0].value.as < named_expression_term_list_t > (); }
#line 2625 "bang.tab.cpp"
    break;

  case 106: // opt-named-expr-list: opt-named-expr
#line 670 "bang.y"
        { yylhs.value.as < named_expression_term_list_t > () = named_expression_term_list_t{std::move(yystack_[0].value.as < named_expression_term_t > ())}; }
#line 2631 "bang.tab.cpp"
    break;

  case 107: // opt-named-expr-list: opt-named-expr-list "," opt-named-expr
#line 672 "bang.y"
        {
            yylhs.value.as < named_expression_term_list_t > () = std::move(yystack_[2].value.as < named_expression_term_list_t > ());
            yylhs.value.as < named_expression_term_list_t > ().push_back(std::move(yystack_[0].value.as < named_expression_term_t > ()));
		}
#line 2640 "bang.tab.cpp"
    break;

  case 108: // opt-named-expr: qname "`:`" expression
#line 680 "bang.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yystack_[2].value.as < sonia::lang::bang::annotated_qname > ().value.set_absolute(); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::tuple{std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[0].value.as < expression_t > ())}, std::move(loc)}; }
#line 2646 "bang.tab.cpp"
    break;

  case 109: // opt-named-expr: expression
#line 682 "bang.y"
        { auto loc = get_start_location(yystack_[0].value.as < expression_t > ()); yylhs.value.as < named_expression_term_t > () = named_expression_term_t{std::move(yystack_[0].value.as < expression_t > ()), std::move(loc)}; }
#line 2652 "bang.tab.cpp"
    break;


#line 2656 "bang.tab.cpp"

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


  const signed char parser::yypact_ninf_ = -95;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -95,    23,   136,   -95,   -95,   -95,   -95,   -95,   -95,   -95,
     309,   309,   -95,    19,   309,    -4,   -52,    19,    26,     0,
       8,     8,   -95,   -95,   -95,   -95,    -3,   103,   -95,   -42,
     -95,   -95,   -95,    -7,   409,     7,   -95,   365,   409,   -23,
     -95,   213,    19,   -95,    19,     8,   -95,   -95,     4,    57,
     -16,   -44,    80,   309,   -95,    19,   -95,   309,   309,   309,
     309,   309,   309,    19,   309,   -95,   -95,   -95,   309,   -95,
      13,    75,    99,    43,    86,    80,   -95,   -95,   -95,    80,
     -42,    69,   -95,    27,    59,    44,   -95,    66,     4,    19,
       8,    61,   185,   368,   -95,   409,   218,   283,   242,   -35,
     409,    67,    52,   -95,    82,   -95,   213,   409,    80,     4,
      77,   -95,   -95,    79,   119,   101,   -95,   185,    51,    73,
     -95,   309,    91,     4,    80,   -95,   154,    80,   123,   -95,
     125,   114,   -95,   -42,   115,     4,   -95,   309,   -95,   309,
     -95,    84,   -95,   137,   117,   -95,    80,   -95,    86,   -95,
     -95,   409,    80,   -95,   -95,   -95,   134,     2,    97,   -95,
      19,     8,   140,   409,   -95,   -95,    16,     4,   185,   -95,
     167,   199,   -95,    80,   -95,   -95,   -42,   -95,    80,   -95,
     -95,   -95,   -95,   -95,   309,   -95,   -95,   138,   177,   229,
     196,   291,   398,   -95,   -95,   -95,   -95,   -95,   -95,   321,
     -95
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     2,    85,    27,    88,    83,    84,
       0,   101,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    81,    82,     5,    13,    19,     0,    29,    87,
      12,    10,    11,   100,     0,     0,   100,     0,   102,     0,
      86,    93,     0,    17,     0,     0,    28,     9,    63,     0,
       0,    40,     0,     0,    15,     0,    14,     0,     0,     0,
       0,   104,   104,     0,     0,    94,    89,    92,     0,    18,
       0,     0,    27,     0,    44,     0,    69,    70,    71,     0,
      72,     0,    58,    67,     0,    64,    65,    56,    63,    34,
       0,     0,    20,     0,    30,    80,    96,    97,    99,    87,
     109,     0,   105,   106,     0,    95,    98,   103,     0,    59,
       0,    51,    52,    27,     0,    45,    46,    49,     0,    54,
      57,     0,     0,     0,     0,    55,     0,     0,     0,    38,
       0,    35,    36,    42,    41,    63,    16,     0,    78,     0,
      79,     0,    61,     0,    60,    50,     0,    74,     0,    73,
      53,    68,     0,    21,    66,    77,     0,    76,     0,    33,
       0,     0,     0,   108,   107,     6,     0,     0,    48,    47,
       0,     0,    75,     0,    21,    37,    43,    39,     0,     7,
      62,    21,    90,    22,     0,    24,    23,   100,     0,     0,
       0,     0,     0,    26,    21,    31,     8,    91,    25,     0,
      32
  };

  const short
  parser::yypgoto_[] =
  {
     -95,   -95,   -95,   -95,   190,   -95,   -95,    76,   -95,   -10,
      -2,   -95,   -95,   -95,   -95,    35,   -95,   -95,   -95,   -95,
     -95,    48,   -95,   120,   -94,   -95,   -95,   -59,   -95,    89,
     -25,    -1,    10,   -95,   152,   -95,    63
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    24,   185,    26,    27,   171,   186,    28,
      80,    30,    31,   130,   131,   132,    32,    91,   134,   114,
     115,   116,    81,    82,    83,   143,   144,    84,    85,    86,
      87,    36,    34,    39,   101,   102,   103
  };

  const unsigned char
  parser::yytable_[] =
  {
      29,    33,     6,    40,   137,    43,     6,    46,    29,    29,
      72,    73,    29,    44,     6,   142,   124,    49,    50,    51,
      37,    38,    67,     3,    41,     6,    89,    92,    45,   128,
     178,    47,    69,    55,    70,    55,    52,    68,   121,    90,
      48,    56,    55,    71,    74,    94,   126,    48,    75,   117,
     118,    29,   108,   105,   119,    29,    29,    29,    29,    99,
      99,    55,    29,    93,   179,   124,    29,    95,    96,    97,
      98,   100,   100,   180,   106,     6,   162,    17,   107,   129,
     124,    17,   112,   141,    42,    17,     6,   124,   133,    76,
      77,    78,   113,    79,   125,   126,   149,    88,   124,   155,
     122,   150,   157,   135,   123,   152,   110,   127,   138,    74,
     126,   173,   139,    75,    53,   109,   145,   126,   146,    29,
      74,   168,   127,   117,    75,   140,    74,   170,   126,   127,
      75,   151,   165,   153,    55,    29,     4,    99,   111,   174,
     127,     5,     6,     7,     8,     9,    17,   163,   188,   100,
     129,    54,    55,   190,    76,    77,    78,    17,    79,   176,
     147,   148,   156,    17,   158,    76,    77,    78,   159,    29,
     187,    76,    77,    78,   160,   161,    10,   167,   166,   172,
      11,   124,    29,   177,    12,    13,   193,    29,   187,    29,
     187,   124,    25,    14,   192,   175,   169,    29,   187,   124,
      15,   120,   164,    16,     5,     6,     7,     8,     9,   181,
     124,   126,   154,    17,   104,    18,    19,    20,    21,   194,
       0,   126,     0,   127,     0,     0,    22,    23,     0,   126,
       0,     0,     0,   127,     5,     6,     7,     8,     9,    10,
     126,   127,   182,    11,   196,    60,     0,   183,    13,     0,
     189,     0,   127,    61,     0,    62,    14,   191,    61,     0,
      62,     0,    63,    15,     0,   184,     0,    63,    64,    10,
     199,     0,   195,    11,     0,    65,    17,   183,    13,    35,
      65,     0,    61,     0,    62,     0,    14,     0,     0,    22,
      23,    63,    64,    15,     0,   184,     5,     6,     7,     8,
       9,     0,     0,     0,    65,     0,    17,     0,    58,    35,
      60,     0,     0,     0,     5,     6,     7,     8,     9,    22,
      23,     0,     0,    61,     0,    62,     5,     6,     7,     8,
       9,    10,    63,    64,   197,    11,     0,     0,     0,   183,
      13,     0,     0,     0,     0,    65,     0,     0,    14,    10,
       0,     0,     0,    11,     0,    15,     0,   184,    13,     0,
       0,    10,     0,     0,   200,    11,    14,     0,    17,   183,
      13,    35,     0,     0,     0,     0,    57,     0,    14,    57,
       0,    22,    23,     0,     0,    15,    17,   184,     0,    35,
      58,    59,    60,    58,    59,    60,     0,     0,    17,    22,
      23,    35,     0,     0,     0,    61,    66,    62,    61,    57,
      62,    22,    23,     0,    63,    64,   136,    63,    64,     0,
      57,     0,     0,    58,    59,    60,     0,    65,     0,     0,
      65,     0,     0,     0,    58,    59,    60,     0,    61,     0,
      62,     0,     0,     0,     0,     0,   198,    63,    64,    61,
       0,    62,     0,     0,     0,     0,     0,     0,    63,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65
  };

  const short
  parser::yycheck_[] =
  {
       2,     2,     6,    13,    39,    15,     6,    17,    10,    11,
       6,     7,    14,    65,     6,   109,    14,    19,    20,    21,
      10,    11,    45,     0,    14,     6,    42,    52,    80,    88,
      14,     5,    42,    77,    44,    77,    39,    60,    11,    83,
      40,    48,    77,    45,    40,    55,    44,    40,    44,    74,
      75,    53,    39,    63,    79,    57,    58,    59,    60,    61,
      62,    77,    64,    53,    48,    14,    68,    57,    58,    59,
      60,    61,    62,   167,    64,     6,   135,    77,    68,    89,
      14,    77,    39,   108,    88,    77,     6,    14,    90,    85,
      86,    87,     6,    89,    28,    44,    45,    40,    14,   124,
      41,    28,   127,    42,    60,    14,     7,    56,    41,    40,
      44,    14,    60,    44,    11,    40,    39,    44,    39,   121,
      40,   146,    56,   148,    44,    43,    40,   152,    44,    56,
      44,   121,    48,    42,    77,   137,     0,   139,    39,    42,
      56,     5,     6,     7,     8,     9,    77,   137,   173,   139,
     160,    48,    77,   178,    85,    86,    87,    77,    89,   161,
      41,    60,     8,    77,    41,    85,    86,    87,    43,   171,
     171,    85,    86,    87,    60,    60,    40,    60,    41,    45,
      44,    14,   184,    43,    48,    49,    48,   189,   189,   191,
     191,    14,     2,    57,   184,   160,   148,   199,   199,    14,
      64,    81,   139,    67,     5,     6,     7,     8,     9,    42,
      14,    44,   123,    77,    62,    79,    80,    81,    82,    42,
      -1,    44,    -1,    56,    -1,    -1,    90,    91,    -1,    44,
      -1,    -1,    -1,    56,     5,     6,     7,     8,     9,    40,
      44,    56,    43,    44,    48,    27,    -1,    48,    49,    -1,
     174,    -1,    56,    40,    -1,    42,    57,   181,    40,    -1,
      42,    -1,    49,    64,    -1,    66,    -1,    49,    50,    40,
     194,    -1,    43,    44,    -1,    62,    77,    48,    49,    80,
      62,    -1,    40,    -1,    42,    -1,    57,    -1,    -1,    90,
      91,    49,    50,    64,    -1,    66,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    62,    -1,    77,    -1,    25,    80,
      27,    -1,    -1,    -1,     5,     6,     7,     8,     9,    90,
      91,    -1,    -1,    40,    -1,    42,     5,     6,     7,     8,
       9,    40,    49,    50,    43,    44,    -1,    -1,    -1,    48,
      49,    -1,    -1,    -1,    -1,    62,    -1,    -1,    57,    40,
      -1,    -1,    -1,    44,    -1,    64,    -1,    66,    49,    -1,
      -1,    40,    -1,    -1,    43,    44,    57,    -1,    77,    48,
      49,    80,    -1,    -1,    -1,    -1,    11,    -1,    57,    11,
      -1,    90,    91,    -1,    -1,    64,    77,    66,    -1,    80,
      25,    26,    27,    25,    26,    27,    -1,    -1,    77,    90,
      91,    80,    -1,    -1,    -1,    40,    41,    42,    40,    11,
      42,    90,    91,    -1,    49,    50,    48,    49,    50,    -1,
      11,    -1,    -1,    25,    26,    27,    -1,    62,    -1,    -1,
      62,    -1,    -1,    -1,    25,    26,    27,    -1,    40,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    40,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    93,    94,     0,     0,     5,     6,     7,     8,     9,
      40,    44,    48,    49,    57,    64,    67,    77,    79,    80,
      81,    82,    90,    91,    95,    96,    97,    98,   101,   102,
     103,   104,   108,   123,   124,    80,   123,   124,   124,   125,
     101,   124,    88,   101,    65,    80,   101,     5,    40,   102,
     102,   102,    39,    11,    48,    77,    48,    11,    25,    26,
      27,    40,    42,    49,    50,    62,    41,    45,    60,   101,
     101,   102,     6,     7,    40,    44,    85,    86,    87,    89,
     102,   114,   115,   116,   119,   120,   121,   122,    40,    42,
      83,   109,   122,   124,   101,   124,   124,   124,   124,   102,
     124,   126,   127,   128,   126,   101,   124,   124,    39,    40,
       7,    39,    39,     6,   111,   112,   113,   122,   122,   122,
     115,    11,    41,    60,    14,    28,    44,    56,   119,   101,
     105,   106,   107,   102,   110,    42,    48,    39,    41,    60,
      43,   122,   116,   117,   118,    39,    39,    41,    60,    45,
      28,   124,    14,    42,   121,   122,     8,   122,    41,    43,
      60,    60,   119,   124,   128,    48,    41,    60,   122,   113,
     122,    99,    45,    14,    42,   107,   102,    43,    14,    48,
     116,    42,    43,    48,    66,    96,   100,   123,   122,    99,
     122,    99,   124,    48,    42,    43,    48,    43,    48,    99,
      43
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    92,    93,    94,    94,    94,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    96,    96,    97,    97,    98,
      98,    99,    99,    99,   100,   100,   100,   101,   102,   102,
     102,   103,   103,   104,   105,   105,   106,   106,   107,   108,
     109,   109,   110,   110,   111,   111,   112,   112,   113,   113,
     114,   114,   114,   115,   115,   115,   115,   116,   116,   117,
     117,   118,   118,   119,   119,   120,   120,   121,   121,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   123,   123,
     123,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   125,   125,   125,   126,   126,   127,   127,   128,   128
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     6,     7,     9,     2,
       1,     1,     1,     1,     2,     2,     4,     2,     3,     1,
       3,     0,     2,     2,     1,     3,     2,     1,     2,     1,
       3,     8,    10,     5,     0,     1,     1,     3,     1,     6,
       0,     2,     1,     3,     0,     1,     1,     3,     3,     1,
       3,     2,     2,     3,     2,     2,     1,     2,     1,     0,
       1,     1,     3,     0,     1,     1,     3,     1,     3,     1,
       1,     1,     1,     3,     3,     4,     3,     3,     4,     4,
       3,     1,     1,     1,     1,     1,     2,     1,     1,     3,
       7,     9,     3,     2,     2,     3,     3,     3,     3,     3,
       1,     0,     1,     3,     0,     1,     1,     3,     3,     1
  };


#if BANG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "INTERNAL_IDENTIFIER", "INTEGER",
  "DECIMAL", "OPERATOR_TERM", "\"`=`\"", "\"`_`\"", "\"`->*`\"",
  "\"`->`\"", "\"`~>`\"", "\"`.*`\"", "\"`++`\"", "\"`--`\"", "\"`<<`\"",
  "\"`>>`\"", "\"`<=`\"", "\"`>=`\"", "\"`==`\"", "\"`!=`\"", "\"`&&`\"",
  "\"`||`\"", "\"`..`\"", "\"`...`\"", "\"`+=`\"", "\"`-=`\"", "\"`*=`\"",
  "\"`/=`\"", "\"`%=`\"", "\"`<<=`\"", "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"",
  "\"`^=`\"", "\"`:`\"", "\"`(`\"", "\"`)`\"", "\"`{`\"", "\"`}`\"",
  "\"`[`\"", "\"`]`\"", "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"",
  "\"`+`\"", "\"`-`\"", "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"",
  "\"`|`\"", "\"`!`\"", "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"",
  "\"`?`\"", "\"`#`\"", "LET", "VAR", "RETURN", "EXTERN", "FOR", "VOID_",
  "AUTO", "CONST_", "USING", "THROW", "DELETE_", "SIZEOF", "LOWEST",
  "DBLCOLON", "\"`::`\"", "INCLUDE", "\"reserved word `fn`\"", "ENUM",
  "TYPE", "EXTENDS", "TYPENAME", "BOOL", "INT", "FLOAT",
  "\"weak modifier\"", "\"const modifier\"", "\"true\"", "\"false\"",
  "$accept", "begin", "declaration_any", "generic-decl", "let-decl",
  "let-decl-start", "let-decl-start-with-opt-type",
  "infunction_declaration_any", "opt-infunction-decl", "identifier",
  "qname", "fn-decl", "enum-decl", "case-list-opt", "case-list",
  "case-decl", "type-decl", "type-extension-any", "type-extension-list",
  "field-list-opt", "field-list", "field-decl", "parameter-decl-name",
  "parameter-decl-type", "parameter-decl", "parameter-list-opt",
  "parameter-list", "parameter-woa-list-opt", "parameter-woa-list",
  "parameter-woa-decl", "type-expr", "compound-expression", "expression",
  "expression-list-any", "opt-named-expr-list-any", "opt-named-expr-list",
  "opt-named-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   299,   299,   303,   305,   307,   327,   329,   331,   333,
     335,   337,   339,   341,   343,   357,   359,   364,   366,   371,
     372,   377,   379,   381,   389,   391,   393,   398,   407,   409,
     411,   417,   421,   428,   435,   437,   441,   444,   449,   454,
     461,   463,   467,   470,   475,   476,   480,   482,   487,   489,
     494,   496,   498,   503,   505,   507,   509,   514,   516,   521,
     522,   526,   528,   533,   534,   538,   540,   545,   547,   553,
     554,   555,   558,   559,   561,   563,   565,   572,   589,   591,
     596,   601,   603,   605,   607,   609,   611,   613,   615,   617,
     619,   623,   625,   627,   629,   631,   635,   637,   639,   641,
     644,   654,   656,   658,   663,   665,   669,   671,   679,   681
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
#line 3385 "bang.tab.cpp"

#line 1016 "bang.y"

