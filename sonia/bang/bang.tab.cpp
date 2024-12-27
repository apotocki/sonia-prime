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

      case symbol_kind::S_118_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
      case symbol_kind::S_128_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
        value.copy< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
        value.copy< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_let_decl: // let-decl
      case symbol_kind::S_103_let_decl_start: // let-decl-start
      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
      case symbol_kind::S_124_argument_list: // argument-list
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_132_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_126_argument_name: // argument-name
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_116_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_case_decl: // case-decl
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.copy< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
      case symbol_kind::S_120_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_140_concept_expression: // concept-expression
      case symbol_kind::S_141_syntax_expression: // syntax-expression
      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_144_new_expression: // new-expression
      case symbol_kind::S_145_call_expression: // call-expression
      case symbol_kind::S_146_compound_expression: // compound-expression
      case symbol_kind::S_147_type_expr: // type-expr
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

      case symbol_kind::S_118_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
      case symbol_kind::S_128_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
        value.move< infunction_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_102_let_decl: // let-decl
      case symbol_kind::S_103_let_decl_start: // let-decl-start
      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
      case symbol_kind::S_124_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_132_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_126_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_116_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_case_decl: // case-decl
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.move< statement_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
      case symbol_kind::S_120_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_140_concept_expression: // concept-expression
      case symbol_kind::S_141_syntax_expression: // syntax-expression
      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_144_new_expression: // new-expression
      case symbol_kind::S_145_call_expression: // call-expression
      case symbol_kind::S_146_compound_expression: // compound-expression
      case symbol_kind::S_147_type_expr: // type-expr
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

      case symbol_kind::S_118_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
      case symbol_kind::S_128_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_let_decl: // let-decl
      case symbol_kind::S_103_let_decl_start: // let-decl-start
      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
      case symbol_kind::S_124_argument_list: // argument-list
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_132_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_126_argument_name: // argument-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_116_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_case_decl: // case-decl
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.YY_MOVE_OR_COPY< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
      case symbol_kind::S_120_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_140_concept_expression: // concept-expression
      case symbol_kind::S_141_syntax_expression: // syntax-expression
      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_144_new_expression: // new-expression
      case symbol_kind::S_145_call_expression: // call-expression
      case symbol_kind::S_146_compound_expression: // compound-expression
      case symbol_kind::S_147_type_expr: // type-expr
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

      case symbol_kind::S_118_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
      case symbol_kind::S_128_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
        value.move< infunction_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_102_let_decl: // let-decl
      case symbol_kind::S_103_let_decl_start: // let-decl-start
      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
      case symbol_kind::S_124_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_132_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_126_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_116_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_case_decl: // case-decl
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.move< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
      case symbol_kind::S_120_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_140_concept_expression: // concept-expression
      case symbol_kind::S_141_syntax_expression: // syntax-expression
      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_144_new_expression: // new-expression
      case symbol_kind::S_145_call_expression: // call-expression
      case symbol_kind::S_146_compound_expression: // compound-expression
      case symbol_kind::S_147_type_expr: // type-expr
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

      case symbol_kind::S_118_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
      case symbol_kind::S_128_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
        value.copy< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
        value.copy< infunction_statement > (that.value);
        break;

      case symbol_kind::S_102_let_decl: // let-decl
      case symbol_kind::S_103_let_decl_start: // let-decl-start
      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
      case symbol_kind::S_124_argument_list: // argument-list
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_132_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_126_argument_name: // argument-name
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_116_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_121_case_decl: // case-decl
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.copy< statement_set_t > (that.value);
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
      case symbol_kind::S_120_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_140_concept_expression: // concept-expression
      case symbol_kind::S_141_syntax_expression: // syntax-expression
      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_144_new_expression: // new-expression
      case symbol_kind::S_145_call_expression: // call-expression
      case symbol_kind::S_146_compound_expression: // compound-expression
      case symbol_kind::S_147_type_expr: // type-expr
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

      case symbol_kind::S_118_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
      case symbol_kind::S_128_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
        value.move< infunction_declaration_set_t > (that.value);
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
        value.move< infunction_statement > (that.value);
        break;

      case symbol_kind::S_102_let_decl: // let-decl
      case symbol_kind::S_103_let_decl_start: // let-decl-start
      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
      case symbol_kind::S_124_argument_list: // argument-list
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_132_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_126_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_116_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_121_case_decl: // case-decl
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        value.move< statement_set_t > (that.value);
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
      case symbol_kind::S_120_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_140_concept_expression: // concept-expression
      case symbol_kind::S_141_syntax_expression: // syntax-expression
      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_144_new_expression: // new-expression
      case symbol_kind::S_145_call_expression: // call-expression
      case symbol_kind::S_146_compound_expression: // compound-expression
      case symbol_kind::S_147_type_expr: // type-expr
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
#line 326 "bang.y"
                 { }
#line 1429 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 326 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 326 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 326 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 326 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 326 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 326 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 326 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 326 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 326 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 326 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 326 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 326 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 326 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 326 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 326 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 326 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 326 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 326 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 326 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 326 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 326 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 326 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 326 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 326 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement_any: // finished_statement_any
#line 326 "bang.y"
                 { }
#line 1597 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 326 "bang.y"
                 { }
#line 1603 "bang.tab.cpp"
        break;

      case symbol_kind::S_102_let_decl: // let-decl
#line 326 "bang.y"
                 { }
#line 1609 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_let_decl_start: // let-decl-start
#line 326 "bang.y"
                 { }
#line 1615 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 326 "bang.y"
                 { }
#line 1621 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
#line 326 "bang.y"
                 { }
#line 1627 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
#line 326 "bang.y"
                 { }
#line 1633 "bang.tab.cpp"
        break;

      case symbol_kind::S_braced_statements: // braced_statements
#line 326 "bang.y"
                 { }
#line 1639 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement: // finished_statement
#line 326 "bang.y"
                 { }
#line 1645 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
#line 326 "bang.y"
                 { }
#line 1651 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
#line 326 "bang.y"
                 { }
#line 1657 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
#line 326 "bang.y"
                 { }
#line 1663 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 326 "bang.y"
                 { }
#line 1669 "bang.tab.cpp"
        break;

      case symbol_kind::S_internal_identifier: // internal_identifier
#line 326 "bang.y"
                 { }
#line 1675 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 326 "bang.y"
                 { }
#line 1681 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
#line 326 "bang.y"
                 { }
#line 1687 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_fn_name: // fn-name
#line 326 "bang.y"
                 { }
#line 1693 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
#line 326 "bang.y"
                 { }
#line 1699 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_enum_decl: // enum-decl
#line 326 "bang.y"
                 { }
#line 1705 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
#line 326 "bang.y"
                 { }
#line 1711 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_case_list: // case-list
#line 326 "bang.y"
                 { }
#line 1717 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_case_decl: // case-decl
#line 326 "bang.y"
                 { }
#line 1723 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
#line 326 "bang.y"
                 { }
#line 1729 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
#line 326 "bang.y"
                 { }
#line 1735 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_argument_list: // argument-list
#line 326 "bang.y"
                 { }
#line 1741 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 326 "bang.y"
                 { }
#line 1747 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_argument_name: // argument-name
#line 326 "bang.y"
                 { }
#line 1753 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
#line 326 "bang.y"
                 { }
#line 1759 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_field_list: // field-list
#line 326 "bang.y"
                 { }
#line 1765 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 326 "bang.y"
                 { }
#line 1771 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
#line 326 "bang.y"
                 { }
#line 1777 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
#line 326 "bang.y"
                 { }
#line 1783 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_parameter_list: // parameter-list
#line 326 "bang.y"
                 { }
#line 1789 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
#line 326 "bang.y"
                 { }
#line 1795 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
#line 326 "bang.y"
                 { }
#line 1801 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 326 "bang.y"
                 { }
#line 1807 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
#line 326 "bang.y"
                 { }
#line 1813 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
#line 326 "bang.y"
                 { }
#line 1819 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
#line 326 "bang.y"
                 { }
#line 1825 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
#line 326 "bang.y"
                 { }
#line 1831 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_concept_expression: // concept-expression
#line 326 "bang.y"
                 { }
#line 1837 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_syntax_expression: // syntax-expression
#line 326 "bang.y"
                 { }
#line 1843 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 326 "bang.y"
                 { }
#line 1849 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
#line 326 "bang.y"
                 { }
#line 1855 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_new_expression: // new-expression
#line 326 "bang.y"
                 { }
#line 1861 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_call_expression: // call-expression
#line 326 "bang.y"
                 { }
#line 1867 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_compound_expression: // compound-expression
#line 326 "bang.y"
                 { }
#line 1873 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_type_expr: // type-expr
#line 326 "bang.y"
                 { }
#line 1879 "bang.tab.cpp"
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

      case symbol_kind::S_118_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_127_field_list_opt: // field-list-opt
      case symbol_kind::S_128_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_115_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_117_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_110_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_105_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_106_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_braced_statements: // braced_statements
      case symbol_kind::S_109_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< infunction_declaration_set_t > ();
        break;

      case symbol_kind::S_111_infunction_statement: // infunction-statement
        yylhs.value.emplace< infunction_statement > ();
        break;

      case symbol_kind::S_102_let_decl: // let-decl
      case symbol_kind::S_103_let_decl_start: // let-decl-start
      case symbol_kind::S_104_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_123_argument_list_opt: // argument-list-opt
      case symbol_kind::S_124_argument_list: // argument-list
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_135_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_136_parameter_constraint_modifier: // parameter-constraint-modifier
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_137_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_134_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_133_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_131_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_132_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_126_argument_name: // argument-name
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_116_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_121_case_decl: // case-decl
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
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
        yylhs.value.emplace< statement_set_t > ();
        break;

      case symbol_kind::S_119_case_list_opt: // case-list-opt
      case symbol_kind::S_120_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_122_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_130_field_type_expr: // field-type-expr
      case symbol_kind::S_138_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_139_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_140_concept_expression: // concept-expression
      case symbol_kind::S_141_syntax_expression: // syntax-expression
      case symbol_kind::S_142_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_143_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_144_new_expression: // new-expression
      case symbol_kind::S_145_call_expression: // call-expression
      case symbol_kind::S_146_compound_expression: // compound-expression
      case symbol_kind::S_147_type_expr: // type-expr
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
#line 331 "bang.y"
                            { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2300 "bang.tab.cpp"
    break;

  case 3: // begin: finished_statement_any "end of file"
#line 332 "bang.y"
                                 { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2306 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 337 "bang.y"
        { yylhs.value.as < statement_set_t > () = {}; }
#line 2312 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 339 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < statement > ()) }; }
#line 2318 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished_statement_any statement
#line 341 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2324 "bang.tab.cpp"
    break;

  case 7: // finished_statement_any: finished_statement
#line 358 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2330 "bang.tab.cpp"
    break;

  case 8: // finished_statement_any: finished_statement_any "`;`"
#line 359 "bang.y"
                                           { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2336 "bang.tab.cpp"
    break;

  case 9: // finished_statement_any: finished_statement_any finished_statement
#line 361 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2342 "bang.tab.cpp"
    break;

  case 10: // finished_statement_any: statement_any "`;`"
#line 363 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2348 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 368 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2354 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN fn-decl
#line 370 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); }
#line 2360 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 372 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2366 "bang.tab.cpp"
    break;

  case 14: // statement: enum-decl
#line 374 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2372 "bang.tab.cpp"
    break;

  case 15: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 379 "bang.y"
        { yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > (); yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), { return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } } }; }
#line 2378 "bang.tab.cpp"
    break;

  case 16: // statement: generic-statement
#line 381 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2384 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 394 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2390 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 396 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2396 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: LET identifier
#line 401 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2402 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: LET "weak modifier" identifier
#line 403 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2408 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 407 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2414 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 409 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2420 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 414 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = {}; }
#line 2426 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 416 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = infunction_declaration_set_t{std::move(yystack_[0].value.as < infunction_statement > ())}; }
#line 2432 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 418 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); yylhs.value.as < infunction_declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < infunction_statement > ())); }
#line 2438 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished_statement
#line 422 "bang.y"
                         { yylhs.value.as < infunction_declaration_set_t > () = infunction_declaration_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2444 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 423 "bang.y"
                                                      { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); }
#line 2450 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished_statement
#line 425 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); yylhs.value.as < infunction_declaration_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2456 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 426 "bang.y"
                                             { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); }
#line 2462 "bang.tab.cpp"
    break;

  case 30: // braced_statements: "`{`" infunction-statement-set "`}`"
#line 431 "bang.y"
        { yylhs.value.as < infunction_declaration_set_t > () = std::move(yystack_[1].value.as < infunction_declaration_set_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2468 "bang.tab.cpp"
    break;

  case 31: // finished_statement: "`while`" syntax-expression braced_statements
#line 436 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2474 "bang.tab.cpp"
    break;

  case 32: // finished_statement: "`while`" syntax-expression "`;`" syntax-expression braced_statements
#line 438 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2480 "bang.tab.cpp"
    break;

  case 33: // finished_statement: "`if`" syntax-expression braced_statements
#line 440 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2486 "bang.tab.cpp"
    break;

  case 34: // finished_statement: "`if`" syntax-expression braced_statements "`else`" braced_statements
#line 442 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < infunction_declaration_set_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) }; }
#line 2492 "bang.tab.cpp"
    break;

  case 35: // finished_statement: fn-start-decl fn-decl braced_statements
#line 444 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), std::move(yystack_[0].value.as < infunction_declaration_set_t > ()) };  }
#line 2498 "bang.tab.cpp"
    break;

  case 36: // finished_statement: STRUCT struct-decl
#line 446 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2504 "bang.tab.cpp"
    break;

  case 37: // infunction-statement-set: infunction-statement-any
#line 450 "bang.y"
    { yylhs.value.as < infunction_declaration_set_t > () = yystack_[0].value.as < infunction_declaration_set_t > (); }
#line 2510 "bang.tab.cpp"
    break;

  case 38: // infunction-statement-set: finished-infunction-statement-any
#line 450 "bang.y"
                               { yylhs.value.as < infunction_declaration_set_t > () = yystack_[0].value.as < infunction_declaration_set_t > (); }
#line 2516 "bang.tab.cpp"
    break;

  case 39: // generic-statement: let-decl
#line 454 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2522 "bang.tab.cpp"
    break;

  case 40: // generic-statement: "`return`" syntax-expression
#line 456 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2528 "bang.tab.cpp"
    break;

  case 41: // generic-statement: compound-expression
#line 458 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2534 "bang.tab.cpp"
    break;

  case 42: // generic-statement: syntax-expression "`=`" syntax-expression
#line 460 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2540 "bang.tab.cpp"
    break;

  case 43: // infunction-statement: generic-statement
#line 465 "bang.y"
        { yylhs.value.as < infunction_statement > () = apply_visitor(statement_adopt_visitor<infunction_statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2546 "bang.tab.cpp"
    break;

  case 44: // infunction-statement: "`break`"
#line 467 "bang.y"
        { yylhs.value.as < infunction_statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2552 "bang.tab.cpp"
    break;

  case 45: // infunction-statement: "`continue`"
#line 469 "bang.y"
        { yylhs.value.as < infunction_statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2558 "bang.tab.cpp"
    break;

  case 46: // identifier: IDENTIFIER
#line 474 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2564 "bang.tab.cpp"
    break;

  case 47: // internal_identifier: INTERNAL_IDENTIFIER
#line 482 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2570 "bang.tab.cpp"
    break;

  case 48: // qname: "`::`" identifier
#line 497 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2576 "bang.tab.cpp"
    break;

  case 49: // qname: identifier
#line 499 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2582 "bang.tab.cpp"
    break;

  case 50: // qname: qname "`::`" identifier
#line 501 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2588 "bang.tab.cpp"
    break;

  case 51: // fn-start-decl: %empty
#line 507 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; }
#line 2594 "bang.tab.cpp"
    break;

  case 52: // fn-start-decl: INLINE
#line 509 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; }
#line 2600 "bang.tab.cpp"
    break;

  case 53: // fn-name: qname
#line 513 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2606 "bang.tab.cpp"
    break;

  case 54: // fn-name: "`new`"
#line 514 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2612 "bang.tab.cpp"
    break;

  case 55: // fn-decl: "`fn`" fn-name "`(`" parameter-list-opt "`)`"
#line 519 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2618 "bang.tab.cpp"
    break;

  case 56: // fn-decl: "`fn`" fn-name "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 521 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); }
#line 2624 "bang.tab.cpp"
    break;

  case 57: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 527 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ctx.make_qname_identifier(annotated_qname{ctx.ns() / std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().value), yystack_[3].value.as < sonia::lang::bang::annotated_qname > ().location}), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2632 "bang.tab.cpp"
    break;

  case 58: // case-list-opt: %empty
#line 533 "bang.y"
           { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2638 "bang.tab.cpp"
    break;

  case 59: // case-list-opt: case-list
#line 535 "bang.y"
    { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2644 "bang.tab.cpp"
    break;

  case 60: // case-list: case-decl
#line 540 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2650 "bang.tab.cpp"
    break;

  case 61: // case-list: case-list "," case-decl
#line 543 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2656 "bang.tab.cpp"
    break;

  case 62: // case-decl: identifier
#line 548 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2662 "bang.tab.cpp"
    break;

  case 63: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 554 "bang.y"
    {
        yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2670 "bang.tab.cpp"
    break;

  case 64: // argument-list-opt: %empty
#line 585 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2676 "bang.tab.cpp"
    break;

  case 65: // argument-list-opt: argument-list
#line 586 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2682 "bang.tab.cpp"
    break;

  case 66: // argument-list: argument
#line 591 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2688 "bang.tab.cpp"
    break;

  case 67: // argument-list: argument-list "," argument
#line 593 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2694 "bang.tab.cpp"
    break;

  case 68: // argument: argument-name syntax-expression
#line 598 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2700 "bang.tab.cpp"
    break;

  case 69: // argument: syntax-expression
#line 600 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2706 "bang.tab.cpp"
    break;

  case 70: // argument-name: identifier "`=`"
#line 605 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2712 "bang.tab.cpp"
    break;

  case 71: // argument-name: identifier "`:`"
#line 607 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2718 "bang.tab.cpp"
    break;

  case 72: // field-list-opt: %empty
#line 612 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2724 "bang.tab.cpp"
    break;

  case 73: // field-list-opt: field-list
#line 613 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2730 "bang.tab.cpp"
    break;

  case 74: // field-list: field
#line 618 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2736 "bang.tab.cpp"
    break;

  case 75: // field-list: field-list "," field
#line 620 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2742 "bang.tab.cpp"
    break;

  case 76: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr
#line 625 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2748 "bang.tab.cpp"
    break;

  case 77: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr "`=`" syntax-expression
#line 627 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2754 "bang.tab.cpp"
    break;

  case 78: // field-type-expr: qname
#line 632 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2760 "bang.tab.cpp"
    break;

  case 79: // field-type-expr: qname "`(`" argument-list-opt "`)`"
#line 634 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2766 "bang.tab.cpp"
    break;

  case 80: // field-type-expr: INTERNAL_IDENTIFIER
#line 636 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2772 "bang.tab.cpp"
    break;

  case 81: // field-type-expr: field-type-expr "`|`" field-type-expr
#line 638 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2778 "bang.tab.cpp"
    break;

  case 82: // parameter-list-opt: %empty
#line 643 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2784 "bang.tab.cpp"
    break;

  case 83: // parameter-list-opt: parameter-list
#line 644 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2790 "bang.tab.cpp"
    break;

  case 84: // parameter-list: parameter-decl
#line 649 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2796 "bang.tab.cpp"
    break;

  case 85: // parameter-list: parameter-list "," parameter-decl
#line 651 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2802 "bang.tab.cpp"
    break;

  case 86: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 656 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2808 "bang.tab.cpp"
    break;

  case 87: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 658 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2814 "bang.tab.cpp"
    break;

  case 88: // parameter-decl: parameter-constraint-set
#line 660 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2820 "bang.tab.cpp"
    break;

  case 89: // parameter-name-decl: identifier internal_identifier "`:`"
#line 665 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }; }
#line 2826 "bang.tab.cpp"
    break;

  case 90: // parameter-name-decl: identifier "`:`"
#line 667 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), nullopt }; }
#line 2832 "bang.tab.cpp"
    break;

  case 91: // parameter-name-decl: internal_identifier "`:`"
#line 669 "bang.y"
        { yylhs.value.as < parameter_name > () = unnamed_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }; }
#line 2838 "bang.tab.cpp"
    break;

  case 92: // parameter-name-decl: internal_identifier "`...`" "`:`"
#line 671 "bang.y"
        { yylhs.value.as < parameter_name > () = varnamed_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ())}; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2844 "bang.tab.cpp"
    break;

  case 93: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 675 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2850 "bang.tab.cpp"
    break;

  case 94: // parameter-constraint-modifier-opt: %empty
#line 677 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2856 "bang.tab.cpp"
    break;

  case 95: // parameter-constraint-modifier: TYPENAME
#line 682 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2862 "bang.tab.cpp"
    break;

  case 96: // parameter-constraint-modifier: "const modifier"
#line 684 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2868 "bang.tab.cpp"
    break;

  case 97: // parameter-constraint-set: parameter-matched-type
#line 689 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2874 "bang.tab.cpp"
    break;

  case 98: // parameter-constraint-set: concept-expression
#line 691 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2880 "bang.tab.cpp"
    break;

  case 99: // parameter-constraint-set: "`_`"
#line 693 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{}; }
#line 2886 "bang.tab.cpp"
    break;

  case 100: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 695 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2892 "bang.tab.cpp"
    break;

  case 101: // parameter-matched-type: basic-parameter-matched-type
#line 699 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2898 "bang.tab.cpp"
    break;

  case 102: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 701 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2904 "bang.tab.cpp"
    break;

  case 103: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 703 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2910 "bang.tab.cpp"
    break;

  case 104: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 705 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2916 "bang.tab.cpp"
    break;

  case 105: // basic-parameter-matched-type: qname
#line 710 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2922 "bang.tab.cpp"
    break;

  case 106: // basic-parameter-matched-type: qname "`(`" argument-list-opt "`)`"
#line 712 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2928 "bang.tab.cpp"
    break;

  case 107: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 714 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2934 "bang.tab.cpp"
    break;

  case 108: // concept-expression: "`@`" qname
#line 737 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 2940 "bang.tab.cpp"
    break;

  case 109: // syntax-expression: INTERNAL_IDENTIFIER
#line 742 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2946 "bang.tab.cpp"
    break;

  case 110: // syntax-expression: syntax-expression-wo-ii
#line 743 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2952 "bang.tab.cpp"
    break;

  case 111: // syntax-expression-wo-ii: "true"
#line 749 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2958 "bang.tab.cpp"
    break;

  case 112: // syntax-expression-wo-ii: "false"
#line 751 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 2964 "bang.tab.cpp"
    break;

  case 113: // syntax-expression-wo-ii: INTEGER
#line 753 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 2970 "bang.tab.cpp"
    break;

  case 114: // syntax-expression-wo-ii: DECIMAL
#line 755 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 2976 "bang.tab.cpp"
    break;

  case 115: // syntax-expression-wo-ii: STRING
#line 757 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 2982 "bang.tab.cpp"
    break;

  case 116: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 759 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2988 "bang.tab.cpp"
    break;

  case 117: // syntax-expression-wo-ii: qname
#line 761 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2994 "bang.tab.cpp"
    break;

  case 118: // syntax-expression-wo-ii: "`(`" argument-list "`)`"
#line 763 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) };
            }
        }
#line 3006 "bang.tab.cpp"
    break;

  case 119: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 773 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3012 "bang.tab.cpp"
    break;

  case 120: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 775 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3018 "bang.tab.cpp"
    break;

  case 121: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 777 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3024 "bang.tab.cpp"
    break;

  case 122: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 779 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3030 "bang.tab.cpp"
    break;

  case 123: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 785 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3036 "bang.tab.cpp"
    break;

  case 124: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 787 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3042 "bang.tab.cpp"
    break;

  case 125: // syntax-expression-wo-ii: apostrophe-expression
#line 789 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3048 "bang.tab.cpp"
    break;

  case 126: // syntax-expression-wo-ii: new-expression
#line 790 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3054 "bang.tab.cpp"
    break;

  case 127: // syntax-expression-wo-ii: compound-expression
#line 791 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3060 "bang.tab.cpp"
    break;

  case 128: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 796 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3066 "bang.tab.cpp"
    break;

  case 129: // new-expression: "`new`" qname
#line 801 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3072 "bang.tab.cpp"
    break;

  case 130: // new-expression: "`new`" apostrophe-expression
#line 803 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3078 "bang.tab.cpp"
    break;

  case 131: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 805 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3084 "bang.tab.cpp"
    break;

  case 132: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 807 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3090 "bang.tab.cpp"
    break;

  case 133: // call-expression: qname "`(`" argument-list-opt "`)`"
#line 812 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3096 "bang.tab.cpp"
    break;

  case 134: // call-expression: call-expression "`(`" argument-list-opt "`)`"
#line 814 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3102 "bang.tab.cpp"
    break;

  case 135: // call-expression: apostrophe-expression "`(`" argument-list-opt "`)`"
#line 816 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3108 "bang.tab.cpp"
    break;

  case 136: // compound-expression: call-expression
#line 820 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3114 "bang.tab.cpp"
    break;

  case 137: // type-expr: qname
#line 906 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3120 "bang.tab.cpp"
    break;

  case 138: // type-expr: INTERNAL_IDENTIFIER
#line 910 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3126 "bang.tab.cpp"
    break;

  case 139: // type-expr: "`[`" type-expr "`]`"
#line 914 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3132 "bang.tab.cpp"
    break;

  case 140: // type-expr: "`(`" argument-list-opt "`)`"
#line 916 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_tuple_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3138 "bang.tab.cpp"
    break;

  case 141: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 918 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_array_t{std::move(yystack_[3].value.as < syntax_expression_t > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3144 "bang.tab.cpp"
    break;

  case 142: // type-expr: type-expr "`|`" type-expr
#line 920 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3156 "bang.tab.cpp"
    break;

  case 143: // type-expr: type-expr "`->`" type-expr
#line 928 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3162 "bang.tab.cpp"
    break;


#line 3166 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -155;

  const signed char parser::yytable_ninf_ = -52;

  const short
  parser::yypact_[] =
  {
     243,  -155,  -155,  -155,  -155,  -155,  -155,   392,    26,   392,
     392,    -3,   -33,    94,   392,   392,   392,    40,  -155,    45,
      45,  -155,  -155,    67,    13,   169,  -155,  -155,    84,    98,
    -155,  -155,  -155,   120,    61,  -155,   129,  -155,   110,  -155,
     114,     8,   112,  -155,  -155,   103,    86,  -155,   392,   247,
     116,    26,  -155,    26,     3,  -155,   128,   123,   153,   235,
     247,  -155,   106,    15,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,    36,   392,    26,   392,   113,   392,   392,   392,
    -155,    26,   392,   392,   392,  -155,  -155,  -155,  -155,   392,
     247,  -155,   149,  -155,   150,   151,   392,   392,   348,   392,
    -155,   122,    26,   156,  -155,   392,    36,   150,    27,   247,
    -155,   157,   130,   392,  -155,   247,   132,   132,  -155,   116,
     158,   163,  -155,    36,    24,   165,   171,  -155,  -155,   162,
     290,  -155,   154,  -155,  -155,    61,   235,   172,  -155,   173,
     155,  -155,    26,   178,    74,    36,   216,    36,  -155,   247,
    -155,  -155,    27,    14,    45,  -155,  -155,  -155,    19,   115,
     144,   180,   164,  -155,   -39,   105,   218,  -155,   101,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,   172,  -155,  -155,
    -155,    26,   187,   188,   168,  -155,  -155,  -155,  -155,   185,
      77,  -155,   150,  -155,  -155,   193,   194,  -155,   392,   227,
      24,   105,  -155,   213,   218,  -155,  -155,    45,  -155,   -39,
    -155,    26,  -155,  -155,  -155,   200,   392,  -155,   218,  -155,
      28,  -155,  -155,   247,  -155,   146,    10,   392,   392,    28,
     212,   247,  -155,  -155
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   115,    46,   109,   116,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,   111,   112,     0,     0,    51,     5,    39,    21,    17,
       7,    16,    49,   117,     0,    14,     0,   110,   125,   126,
     136,   127,     0,   127,    48,    49,     0,    66,     0,    69,
     120,     0,    19,     0,     0,    12,   129,   130,     0,     0,
      40,    13,     0,     0,    36,     1,     2,    10,     3,     8,
       6,     9,     0,     0,     0,    64,     0,     0,     0,     0,
     121,     0,     0,    64,    64,   128,    70,    71,   118,     0,
      68,    20,     0,    54,    53,     0,    64,    64,    23,     0,
      31,    33,    58,     0,   138,    64,     0,   137,    22,    18,
      50,     0,    65,     0,    35,    42,   123,   124,   119,   122,
       0,     0,    67,     0,    82,     0,     0,    45,    44,    37,
      38,    26,     0,    43,    24,     0,     0,     0,    62,     0,
      59,    60,    72,     0,     0,     0,     0,     0,   133,    15,
     135,   134,    11,   103,     0,    99,    95,    96,    49,     0,
     105,     0,    83,    84,    94,     0,    88,    97,   101,    98,
     131,   132,    29,    27,    28,    25,    30,     0,    32,    34,
      57,     0,     0,     0,    73,    74,   140,   139,   143,     0,
     142,   104,   108,    47,    90,     0,     0,    91,    64,    55,
       0,     0,    93,   103,    87,   100,   102,     0,    61,    94,
      63,     0,   141,    89,    92,     0,     0,    85,    86,   107,
       0,    75,   106,    56,    80,    78,    76,    64,     0,     0,
       0,    77,    81,    79
  };

  const short
  parser::yypgoto_[] =
  {
    -155,  -155,  -155,  -155,   236,  -155,  -155,  -155,  -155,  -155,
     -53,   -24,  -155,    -1,   136,    12,   102,    -2,     7,  -155,
     -32,  -155,  -155,  -155,    87,  -155,   -56,   260,   181,  -155,
    -155,  -155,    60,    43,  -155,  -155,    73,  -155,    68,  -140,
     -73,  -155,    69,  -154,     0,  -155,   265,  -155,  -155,     4,
     -87
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,    23,    24,    25,    26,    27,    28,    29,   129,   130,
     114,    30,   132,    31,   134,    32,   159,    33,    34,    95,
      55,    35,   139,   140,   141,    64,   111,   112,    47,    48,
     183,   184,   185,   226,   161,   162,   163,   164,   201,   165,
     166,   167,   168,   169,    49,    37,    38,    39,    40,    43,
     108
  };

  const short
  parser::yytable_[] =
  {
      36,    71,    76,     2,    41,   100,   101,    42,   -41,     2,
      50,    56,   205,    66,    58,    59,    60,    62,    63,   144,
      44,    45,   228,    52,   202,    36,   193,   120,   121,    41,
       2,   153,     2,   103,     2,   224,   152,    53,   154,   155,
     125,   126,     2,   104,   145,    61,   -47,     8,    90,   143,
     205,     2,    94,   156,    54,   157,   -41,   -47,   188,    74,
     190,   -41,   194,    91,   205,    92,    67,    65,     8,   202,
     107,   229,     8,   109,   131,    93,   146,   115,   116,   117,
       8,   105,   119,   178,   179,   106,   110,    45,   147,     8,
      51,   145,   204,   118,   145,    45,    45,   133,    36,   136,
       2,    45,    41,   177,   107,   135,   174,     7,    45,    45,
      73,     2,   203,   149,   138,    86,   156,    45,   157,   154,
     155,   107,   160,   146,   187,    85,   146,    72,   218,   133,
      36,   113,    88,   206,    41,   147,   158,   135,     8,    78,
      79,    77,   215,   107,    80,   107,    87,   196,    54,     8,
      74,    89,   192,   102,   182,    83,    78,    79,   197,    84,
      98,    80,   207,   160,    74,    75,    81,    82,    97,    68,
      81,   230,    74,    96,     1,     2,     3,     4,     5,     6,
      78,    79,     7,    81,    82,    80,    81,    82,    74,   198,
      74,   227,   123,   138,    74,    89,   124,   137,   160,   160,
      98,   142,   176,   148,   150,   160,    99,    81,    82,   151,
      45,   170,   158,     8,     9,   172,   223,   171,   225,    98,
     181,   180,    69,   182,   186,   189,   199,   225,   231,   200,
     209,    10,   154,   211,   210,   212,   213,   214,    11,    45,
      12,    13,    14,    15,   216,   191,   222,    16,     1,     2,
       3,     4,     5,     6,    17,    18,     7,    19,   233,    20,
     195,    70,    78,    79,    21,    22,   175,    80,   208,    46,
     122,   221,   232,   217,    78,    79,   219,   220,    57,    80,
       0,     0,    98,     0,     0,     0,     0,     8,     9,    81,
      82,     0,     0,     0,     0,     1,     2,     3,     4,     5,
       6,    81,    82,     7,     0,    10,     0,     0,     0,     0,
       0,     0,    11,     0,    12,    13,    14,    15,     0,     0,
       0,    16,     0,     0,     0,     0,     0,     0,    17,    18,
     -51,    19,     0,    20,     8,     9,     0,     0,    21,    22,
       0,     0,     0,   173,     0,     0,     0,     0,     0,     0,
       0,     0,    10,     1,     2,     3,     4,     5,     6,    11,
       0,     7,    13,    14,    15,     0,   127,   128,    16,     0,
       0,     0,     0,     0,     0,     0,    18,   -51,     0,     0,
      20,     0,     0,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     8,     9,     0,     0,     0,     1,     2,     3,
       4,     5,     6,     0,     0,     7,     0,     0,     0,     0,
      10,     0,     0,     0,     0,     0,     0,    11,     0,     0,
      13,    14,    15,     0,   127,   128,    16,     0,     0,     0,
       0,     0,     0,     0,    18,   -51,     8,     9,    20,     0,
       0,     0,     0,    21,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    22
  };

  const short
  parser::yycheck_[] =
  {
       0,    25,    34,     6,     0,    58,    59,     7,     0,     6,
      10,    13,   166,     0,    14,    15,    16,    19,    20,   106,
       8,     9,    12,    11,   164,    25,     7,    83,    84,    25,
       6,     7,     6,    18,     6,     7,   123,    70,    14,    15,
      96,    97,     6,     7,    17,     5,    32,    44,    48,   105,
     204,     6,    54,    92,    87,    94,    48,    43,   145,    44,
     147,    53,    43,    51,   218,    53,    53,     0,    44,   209,
      72,    61,    44,    73,    98,    72,    49,    77,    78,    79,
      44,    45,    82,   136,   137,    49,    74,    75,    61,    44,
      93,    17,   165,    81,    17,    83,    84,    98,    98,    99,
       6,    89,    98,   135,   106,    98,   130,    13,    96,    97,
      12,     6,     7,   113,   102,    12,    92,   105,    94,    14,
      15,   123,   124,    49,    50,    13,    49,    43,   201,   130,
     130,    18,    46,    32,   130,    61,   124,   130,    44,    27,
      28,    12,   198,   145,    32,   147,    43,    32,    87,    44,
      44,    65,   154,    47,   142,    45,    27,    28,    43,    45,
      47,    32,    61,   165,    44,    45,    54,    55,    45,     0,
      54,   227,    44,    45,     5,     6,     7,     8,     9,    10,
      27,    28,    13,    54,    55,    32,    54,    55,    44,    45,
      44,    45,    43,   181,    44,    65,    45,    75,   200,   201,
      47,    45,    48,    46,    46,   207,    53,    54,    55,    46,
     198,    46,   200,    44,    45,    53,   216,    46,   220,    47,
      65,    48,    53,   211,    46,     9,    46,   229,   228,    65,
      43,    62,    14,    65,    46,    50,    43,    43,    69,   227,
      71,    72,    73,    74,    17,    32,    46,    78,     5,     6,
       7,     8,     9,    10,    85,    86,    13,    88,    46,    90,
     158,    25,    27,    28,    95,    96,   130,    32,   181,     9,
      89,   211,   229,   200,    27,    28,   207,   209,    13,    32,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    44,    45,    54,
      55,    -1,    -1,    -1,    -1,     5,     6,     7,     8,     9,
      10,    54,    55,    13,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    -1,    90,    44,    45,    -1,    -1,    95,    96,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,     5,     6,     7,     8,     9,    10,    69,
      -1,    13,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    44,    45,    90,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    13,    44,    45,
      62,    69,    71,    72,    73,    74,    78,    85,    86,    88,
      90,    95,    96,    98,    99,   100,   101,   102,   103,   104,
     108,   110,   112,   114,   115,   118,   141,   142,   143,   144,
     145,   146,   141,   146,   112,   112,   124,   125,   126,   141,
     141,    93,   112,    70,    87,   117,   114,   143,   141,   141,
     141,     5,   114,   114,   122,     0,     0,    53,     0,    53,
     101,   108,    43,    12,    44,    45,   117,    12,    27,    28,
      32,    54,    55,    45,    45,    13,    12,    43,    46,    65,
     141,   112,   112,    72,   114,   116,    45,    45,    47,    53,
     107,   107,    47,    18,     7,    45,    49,   114,   147,   141,
     112,   123,   124,    18,   107,   141,   141,   141,   112,   141,
     123,   123,   125,    43,    45,   123,   123,    76,    77,   105,
     106,   108,   109,   110,   111,   115,   141,    75,   112,   119,
     120,   121,    45,   123,   147,    17,    49,    61,    46,   141,
      46,    46,   147,     7,    14,    15,    92,    94,   112,   113,
     114,   131,   132,   133,   134,   136,   137,   138,   139,   140,
      46,    46,    53,    53,   108,   111,    48,   117,   107,   107,
      48,    65,   112,   127,   128,   129,    46,    50,   147,     9,
     147,    32,   114,     7,    43,   113,    32,    43,    45,    46,
      65,   135,   136,     7,   137,   140,    32,    61,   121,    43,
      46,    65,    50,    43,    43,   123,    17,   133,   137,   139,
     135,   129,    46,   141,     7,   114,   130,    45,    12,    61,
     123,   141,   130,    46
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    97,    98,    98,    99,    99,    99,   100,   100,   100,
     100,   101,   101,   101,   101,   101,   101,   102,   102,   103,
     103,   104,   104,   105,   105,   105,   106,   106,   106,   106,
     107,   108,   108,   108,   108,   108,   108,   109,   109,   110,
     110,   110,   110,   111,   111,   111,   112,   113,   114,   114,
     114,   115,   115,   116,   116,   117,   117,   118,   119,   119,
     120,   120,   121,   122,   123,   123,   124,   124,   125,   125,
     126,   126,   127,   127,   128,   128,   129,   129,   130,   130,
     130,   130,   131,   131,   132,   132,   133,   133,   133,   134,
     134,   134,   134,   135,   135,   136,   136,   137,   137,   137,
     137,   138,   138,   138,   138,   139,   139,   139,   140,   141,
     141,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   143,   144,
     144,   144,   144,   145,   145,   145,   146,   147,   147,   147,
     147,   147,   147,   147
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     2,     2,     1,     4,     1,     1,     3,     2,
       3,     1,     3,     0,     1,     2,     1,     2,     2,     2,
       3,     3,     5,     3,     5,     3,     2,     1,     1,     1,
       2,     1,     3,     1,     1,     1,     1,     1,     2,     1,
       3,     0,     1,     1,     1,     5,     7,     5,     0,     1,
       1,     3,     1,     5,     0,     1,     1,     3,     2,     1,
       2,     2,     0,     1,     1,     3,     4,     6,     1,     4,
       1,     3,     0,     1,     1,     3,     3,     2,     1,     3,
       2,     2,     3,     1,     0,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     2,     1,     4,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     2,     3,     3,     3,     1,     1,     1,     3,     2,
       2,     5,     5,     4,     4,     4,     1,     1,     1,     3,
       3,     4,     3,     3
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
  "\"APOSTROPHE\"", "\"`@`\"", "\"`_`\"", "\"`->*`\"", "\"`->`\"",
  "\"`=>`\"", "\"`~>`\"", "\"`.*`\"", "\"`++`\"", "\"`--`\"", "\"`<<`\"",
  "\"`>>`\"", "\"`<=`\"", "\"`>=`\"", "\"`==`\"", "\"`!=`\"", "\"`&&`\"",
  "\"`||`\"", "\"`..`\"", "\"`...`\"", "\"`+=`\"", "\"`-=`\"", "\"`*=`\"",
  "\"`/=`\"", "\"`%=`\"", "\"`<<=`\"", "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"",
  "\"`^=`\"", "\"`:`\"", "\"`::`\"", "\"`(`\"", "\"`)`\"", "\"`{`\"",
  "\"`}`\"", "\"`[`\"", "\"`]`\"", "\"`<`\"", "\"`>`\"", "\"`;`\"",
  "\"`.`\"", "\"`+`\"", "\"`-`\"", "\"`*`\"", "\"`/`\"", "\"`%`\"",
  "\"`&`\"", "\"`|`\"", "\"`!`\"", "\"`~`\"", "\"`^`\"", "\",\"",
  "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET", "VAR", "EXTERN", "\"`new`\"",
  "\"`while`\"", "\"`if`\"", "\"`else`\"", "\"`continue`\"", "\"`break`\"",
  "\"`return`\"", "FOR", "AUTO", "USING", "THROW", "SIZEOF", "LOWEST",
  "INCLUDE", "INLINE", "\"`fn`\"", "ENUM", "TYPE", "STRUCT", "EXTENDS",
  "TYPENAME", "\"weak modifier\"", "\"const modifier\"", "\"true\"",
  "\"false\"", "$accept", "begin", "statement_any",
  "finished_statement_any", "statement", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction-statement-any",
  "finished-infunction-statement-any", "braced_statements",
  "finished_statement", "infunction-statement-set", "generic-statement",
  "infunction-statement", "identifier", "internal_identifier", "qname",
  "fn-start-decl", "fn-name", "fn-decl", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "struct-decl", "argument-list-opt",
  "argument-list", "argument", "argument-name", "field-list-opt",
  "field-list", "field", "field-type-expr", "parameter-list-opt",
  "parameter-list", "parameter-decl", "parameter-name-decl",
  "parameter-constraint-modifier-opt", "parameter-constraint-modifier",
  "parameter-constraint-set", "parameter-matched-type",
  "basic-parameter-matched-type", "concept-expression",
  "syntax-expression", "syntax-expression-wo-ii", "apostrophe-expression",
  "new-expression", "call-expression", "compound-expression", "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   331,   331,   332,   336,   338,   340,   357,   359,   360,
     362,   367,   369,   371,   373,   378,   380,   393,   395,   400,
     402,   407,   408,   413,   415,   417,   422,   423,   424,   426,
     430,   435,   437,   439,   441,   443,   445,   450,   450,   453,
     455,   457,   459,   464,   466,   468,   473,   481,   496,   498,
     500,   506,   508,   513,   514,   518,   520,   526,   533,   535,
     539,   542,   547,   553,   585,   586,   590,   592,   597,   599,
     604,   606,   612,   613,   617,   619,   624,   626,   631,   633,
     635,   637,   643,   644,   648,   650,   655,   657,   659,   664,
     666,   668,   670,   675,   676,   681,   683,   688,   690,   692,
     694,   699,   700,   702,   704,   709,   711,   713,   736,   741,
     743,   748,   750,   752,   754,   756,   758,   760,   762,   772,
     774,   776,   778,   784,   786,   789,   790,   791,   795,   800,
     802,   804,   806,   811,   813,   815,   820,   905,   909,   913,
     915,   917,   919,   927
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
      95,    96
    };
    // Last valid token kind.
    const int code_max = 351;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 3929 "bang.tab.cpp"

#line 1318 "bang.y"

