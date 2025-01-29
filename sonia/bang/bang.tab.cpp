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

      case symbol_kind::S_119_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
      case symbol_kind::S_130_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
        value.copy< fn_pure > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_let_decl: // let-decl
      case symbol_kind::S_104_let_decl_start: // let-decl-start
      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
      case symbol_kind::S_126_argument_list: // argument-list
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_134_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_128_argument_name: // argument-name
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_117_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
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
      case symbol_kind::S_112_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_108_braced_statements: // braced-statements
      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
        value.copy< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
      case symbol_kind::S_121_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_142_concept_expression: // concept-expression
      case symbol_kind::S_143_syntax_expression: // syntax-expression
      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_146_new_expression: // new-expression
      case symbol_kind::S_147_call_expression: // call-expression
      case symbol_kind::S_148_compound_expression: // compound-expression
      case symbol_kind::S_149_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_using_decl: // using-decl
        value.copy< using_decl > (YY_MOVE (that.value));
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

      case symbol_kind::S_119_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
      case symbol_kind::S_130_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
        value.move< fn_pure > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_103_let_decl: // let-decl
      case symbol_kind::S_104_let_decl_start: // let-decl-start
      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
      case symbol_kind::S_126_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_134_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_128_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_117_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_122_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
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
      case symbol_kind::S_112_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_108_braced_statements: // braced-statements
      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
        value.move< statement_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
      case symbol_kind::S_121_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_142_concept_expression: // concept-expression
      case symbol_kind::S_143_syntax_expression: // syntax-expression
      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_146_new_expression: // new-expression
      case symbol_kind::S_147_call_expression: // call-expression
      case symbol_kind::S_148_compound_expression: // compound-expression
      case symbol_kind::S_149_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_using_decl: // using-decl
        value.move< using_decl > (YY_MOVE (s.value));
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

      case symbol_kind::S_119_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
      case symbol_kind::S_130_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_let_decl: // let-decl
      case symbol_kind::S_104_let_decl_start: // let-decl-start
      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
      case symbol_kind::S_126_argument_list: // argument-list
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_134_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_128_argument_name: // argument-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_117_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_case_decl: // case-decl
        value.YY_MOVE_OR_COPY< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
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
      case symbol_kind::S_112_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_108_braced_statements: // braced-statements
      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
      case symbol_kind::S_121_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_142_concept_expression: // concept-expression
      case symbol_kind::S_143_syntax_expression: // syntax-expression
      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_146_new_expression: // new-expression
      case symbol_kind::S_147_call_expression: // call-expression
      case symbol_kind::S_148_compound_expression: // compound-expression
      case symbol_kind::S_149_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_using_decl: // using-decl
        value.YY_MOVE_OR_COPY< using_decl > (YY_MOVE (that.value));
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

      case symbol_kind::S_119_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
      case symbol_kind::S_130_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
        value.move< fn_pure > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_103_let_decl: // let-decl
      case symbol_kind::S_104_let_decl_start: // let-decl-start
      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
      case symbol_kind::S_126_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_134_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_128_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_117_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
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
      case symbol_kind::S_112_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_108_braced_statements: // braced-statements
      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
        value.move< statement_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
      case symbol_kind::S_121_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_142_concept_expression: // concept-expression
      case symbol_kind::S_143_syntax_expression: // syntax-expression
      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_146_new_expression: // new-expression
      case symbol_kind::S_147_call_expression: // call-expression
      case symbol_kind::S_148_compound_expression: // compound-expression
      case symbol_kind::S_149_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_using_decl: // using-decl
        value.move< using_decl > (YY_MOVE (that.value));
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

      case symbol_kind::S_119_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
      case symbol_kind::S_130_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
        value.copy< fn_pure > (that.value);
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_103_let_decl: // let-decl
      case symbol_kind::S_104_let_decl_start: // let-decl-start
      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
      case symbol_kind::S_126_argument_list: // argument-list
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_134_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_128_argument_name: // argument-name
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_117_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_122_case_decl: // case-decl
        value.copy< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
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
      case symbol_kind::S_112_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_108_braced_statements: // braced-statements
      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
        value.copy< statement_set_t > (that.value);
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
      case symbol_kind::S_121_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_142_concept_expression: // concept-expression
      case symbol_kind::S_143_syntax_expression: // syntax-expression
      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_146_new_expression: // new-expression
      case symbol_kind::S_147_call_expression: // call-expression
      case symbol_kind::S_148_compound_expression: // compound-expression
      case symbol_kind::S_149_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_124_using_decl: // using-decl
        value.copy< using_decl > (that.value);
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

      case symbol_kind::S_119_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
      case symbol_kind::S_130_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
        value.move< fn_pure > (that.value);
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_103_let_decl: // let-decl
      case symbol_kind::S_104_let_decl_start: // let-decl-start
      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
      case symbol_kind::S_126_argument_list: // argument-list
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_134_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_128_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_117_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_122_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (that.value);
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
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
      case symbol_kind::S_112_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_108_braced_statements: // braced-statements
      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
        value.move< statement_set_t > (that.value);
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
      case symbol_kind::S_121_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_142_concept_expression: // concept-expression
      case symbol_kind::S_143_syntax_expression: // syntax-expression
      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_146_new_expression: // new-expression
      case symbol_kind::S_147_call_expression: // call-expression
      case symbol_kind::S_148_compound_expression: // compound-expression
      case symbol_kind::S_149_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_124_using_decl: // using-decl
        value.move< using_decl > (that.value);
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
#line 325 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 325 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 325 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 325 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 325 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 325 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 325 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 325 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 325 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 325 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 325 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 325 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 325 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 325 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 325 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 325 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 325 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 325 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 325 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 325 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 325 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 325 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 325 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 325 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 325 "bang.y"
                 { }
#line 1597 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 325 "bang.y"
                 { }
#line 1603 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 325 "bang.y"
                 { }
#line 1609 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 325 "bang.y"
                 { }
#line 1615 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement_any: // finished_statement_any
#line 325 "bang.y"
                 { }
#line 1621 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 325 "bang.y"
                 { }
#line 1627 "bang.tab.cpp"
        break;

      case symbol_kind::S_103_let_decl: // let-decl
#line 325 "bang.y"
                 { }
#line 1633 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_let_decl_start: // let-decl-start
#line 325 "bang.y"
                 { }
#line 1639 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 325 "bang.y"
                 { }
#line 1645 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1651 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1657 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_braced_statements: // braced-statements
#line 325 "bang.y"
                 { }
#line 1663 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement: // finished_statement
#line 325 "bang.y"
                 { }
#line 1669 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
#line 325 "bang.y"
                 { }
#line 1675 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
#line 325 "bang.y"
                 { }
#line 1681 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_infunction_statement: // infunction-statement
#line 325 "bang.y"
                 { }
#line 1687 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 325 "bang.y"
                 { }
#line 1693 "bang.tab.cpp"
        break;

      case symbol_kind::S_internal_identifier: // internal_identifier
#line 325 "bang.y"
                 { }
#line 1699 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 325 "bang.y"
                 { }
#line 1705 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
#line 325 "bang.y"
                 { }
#line 1711 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_fn_name: // fn-name
#line 325 "bang.y"
                 { }
#line 1717 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
#line 325 "bang.y"
                 { }
#line 1723 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_enum_decl: // enum-decl
#line 325 "bang.y"
                 { }
#line 1729 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
#line 325 "bang.y"
                 { }
#line 1735 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_case_list: // case-list
#line 325 "bang.y"
                 { }
#line 1741 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_case_decl: // case-decl
#line 325 "bang.y"
                 { }
#line 1747 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
#line 325 "bang.y"
                 { }
#line 1753 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_using_decl: // using-decl
#line 325 "bang.y"
                 { }
#line 1759 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
#line 325 "bang.y"
                 { }
#line 1765 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_argument_list: // argument-list
#line 325 "bang.y"
                 { }
#line 1771 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 325 "bang.y"
                 { }
#line 1777 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_argument_name: // argument-name
#line 325 "bang.y"
                 { }
#line 1783 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
#line 325 "bang.y"
                 { }
#line 1789 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_field_list: // field-list
#line 325 "bang.y"
                 { }
#line 1795 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 325 "bang.y"
                 { }
#line 1801 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
#line 325 "bang.y"
                 { }
#line 1807 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
#line 325 "bang.y"
                 { }
#line 1813 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_parameter_list: // parameter-list
#line 325 "bang.y"
                 { }
#line 1819 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
#line 325 "bang.y"
                 { }
#line 1825 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
#line 325 "bang.y"
                 { }
#line 1831 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 325 "bang.y"
                 { }
#line 1837 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
#line 325 "bang.y"
                 { }
#line 1843 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
#line 325 "bang.y"
                 { }
#line 1849 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
#line 325 "bang.y"
                 { }
#line 1855 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
#line 325 "bang.y"
                 { }
#line 1861 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_concept_expression: // concept-expression
#line 325 "bang.y"
                 { }
#line 1867 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_syntax_expression: // syntax-expression
#line 325 "bang.y"
                 { }
#line 1873 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 325 "bang.y"
                 { }
#line 1879 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
#line 325 "bang.y"
                 { }
#line 1885 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_new_expression: // new-expression
#line 325 "bang.y"
                 { }
#line 1891 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_call_expression: // call-expression
#line 325 "bang.y"
                 { }
#line 1897 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_compound_expression: // compound-expression
#line 325 "bang.y"
                 { }
#line 1903 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_type_expr: // type-expr
#line 325 "bang.y"
                 { }
#line 1909 "bang.tab.cpp"
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

      case symbol_kind::S_119_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_129_field_list_opt: // field-list-opt
      case symbol_kind::S_130_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_116_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_118_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure > ();
        break;

      case symbol_kind::S_111_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_103_let_decl: // let-decl
      case symbol_kind::S_104_let_decl_start: // let-decl-start
      case symbol_kind::S_105_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_125_argument_list_opt: // argument-list-opt
      case symbol_kind::S_126_argument_list: // argument-list
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_137_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_138_parameter_constraint_modifier: // parameter-constraint-modifier
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_139_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_136_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_135_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_133_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_134_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_128_argument_name: // argument-name
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_117_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_122_case_decl: // case-decl
        yylhs.value.emplace< sonia::lang::bang::identifier > ();
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
      case symbol_kind::S_UNDERSCORE: // "`_`"
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
      case symbol_kind::S_112_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_106_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_107_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_108_braced_statements: // braced-statements
      case symbol_kind::S_110_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< statement_set_t > ();
        break;

      case symbol_kind::S_120_case_list_opt: // case-list-opt
      case symbol_kind::S_121_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_123_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_132_field_type_expr: // field-type-expr
      case symbol_kind::S_140_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_141_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_142_concept_expression: // concept-expression
      case symbol_kind::S_143_syntax_expression: // syntax-expression
      case symbol_kind::S_144_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_145_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_146_new_expression: // new-expression
      case symbol_kind::S_147_call_expression: // call-expression
      case symbol_kind::S_148_compound_expression: // compound-expression
      case symbol_kind::S_149_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_124_using_decl: // using-decl
        yylhs.value.emplace< using_decl > ();
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
#line 330 "bang.y"
                            { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2331 "bang.tab.cpp"
    break;

  case 3: // begin: finished_statement_any "end of file"
#line 331 "bang.y"
                                 { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2337 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 336 "bang.y"
        { yylhs.value.as < statement_set_t > () = {}; }
#line 2343 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 338 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < statement > ()) }; }
#line 2349 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished_statement_any statement
#line 340 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2355 "bang.tab.cpp"
    break;

  case 7: // finished_statement_any: finished_statement
#line 357 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2361 "bang.tab.cpp"
    break;

  case 8: // finished_statement_any: finished_statement_any "`;`"
#line 358 "bang.y"
                                           { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2367 "bang.tab.cpp"
    break;

  case 9: // finished_statement_any: finished_statement_any finished_statement
#line 360 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2373 "bang.tab.cpp"
    break;

  case 10: // finished_statement_any: statement_any "`;`"
#line 362 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2379 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 367 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2385 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN fn-decl
#line 369 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure > ()); }
#line 2391 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 371 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2397 "bang.tab.cpp"
    break;

  case 14: // statement: enum-decl
#line 373 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2403 "bang.tab.cpp"
    break;

  case 15: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 378 "bang.y"
        { yystack_[2].value.as < fn_pure > ().kind = yystack_[3].value.as < fn_kind > (); yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure > ()), { return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } } }; }
#line 2409 "bang.tab.cpp"
    break;

  case 16: // statement: generic-statement
#line 380 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2415 "bang.tab.cpp"
    break;

  case 17: // statement: STRUCT struct-decl
#line 382 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2421 "bang.tab.cpp"
    break;

  case 18: // statement: USING using-decl
#line 384 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2427 "bang.tab.cpp"
    break;

  case 19: // let-decl: let-decl-start-with-opt-type
#line 389 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2433 "bang.tab.cpp"
    break;

  case 20: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 391 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2439 "bang.tab.cpp"
    break;

  case 21: // let-decl-start: LET identifier
#line 396 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2445 "bang.tab.cpp"
    break;

  case 22: // let-decl-start: LET "weak modifier" identifier
#line 398 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2451 "bang.tab.cpp"
    break;

  case 23: // let-decl-start-with-opt-type: let-decl-start
#line 402 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2457 "bang.tab.cpp"
    break;

  case 24: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 404 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2463 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: %empty
#line 409 "bang.y"
        { yylhs.value.as < statement_set_t > () = {}; }
#line 2469 "bang.tab.cpp"
    break;

  case 26: // infunction-statement-any: infunction-statement
#line 411 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < statement > ()) }; }
#line 2475 "bang.tab.cpp"
    break;

  case 27: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 413 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2481 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished_statement
#line 417 "bang.y"
                         { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2487 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 418 "bang.y"
                                                      { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2493 "bang.tab.cpp"
    break;

  case 30: // finished-infunction-statement-any: finished-infunction-statement-any finished_statement
#line 420 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2499 "bang.tab.cpp"
    break;

  case 31: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 421 "bang.y"
                                             { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2505 "bang.tab.cpp"
    break;

  case 32: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 426 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2511 "bang.tab.cpp"
    break;

  case 33: // finished_statement: "`while`" syntax-expression braced-statements
#line 431 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2517 "bang.tab.cpp"
    break;

  case 34: // finished_statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 433 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2523 "bang.tab.cpp"
    break;

  case 35: // finished_statement: "`if`" syntax-expression braced-statements
#line 435 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2529 "bang.tab.cpp"
    break;

  case 36: // finished_statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 437 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < statement_set_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2535 "bang.tab.cpp"
    break;

  case 37: // finished_statement: fn-start-decl fn-decl braced-statements
#line 439 "bang.y"
        { yystack_[1].value.as < fn_pure > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure > ()), std::move(yystack_[0].value.as < statement_set_t > ()) };  }
#line 2541 "bang.tab.cpp"
    break;

  case 38: // infunction-statement-set: infunction-statement-any
#line 443 "bang.y"
    { yylhs.value.as < statement_set_t > () = yystack_[0].value.as < statement_set_t > (); }
#line 2547 "bang.tab.cpp"
    break;

  case 39: // infunction-statement-set: finished-infunction-statement-any
#line 443 "bang.y"
                               { yylhs.value.as < statement_set_t > () = yystack_[0].value.as < statement_set_t > (); }
#line 2553 "bang.tab.cpp"
    break;

  case 40: // generic-statement: let-decl
#line 447 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2559 "bang.tab.cpp"
    break;

  case 41: // generic-statement: "`return`" syntax-expression
#line 449 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2565 "bang.tab.cpp"
    break;

  case 42: // generic-statement: compound-expression
#line 451 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2571 "bang.tab.cpp"
    break;

  case 43: // generic-statement: syntax-expression "`=`" syntax-expression
#line 453 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2577 "bang.tab.cpp"
    break;

  case 44: // infunction-statement: generic-statement
#line 458 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2583 "bang.tab.cpp"
    break;

  case 45: // infunction-statement: "`break`"
#line 460 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2589 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: "`continue`"
#line 462 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2595 "bang.tab.cpp"
    break;

  case 47: // identifier: IDENTIFIER
#line 467 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2601 "bang.tab.cpp"
    break;

  case 48: // internal_identifier: INTERNAL_IDENTIFIER
#line 475 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2607 "bang.tab.cpp"
    break;

  case 49: // qname: "`::`" identifier
#line 490 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2613 "bang.tab.cpp"
    break;

  case 50: // qname: identifier
#line 492 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2619 "bang.tab.cpp"
    break;

  case 51: // qname: qname "`::`" identifier
#line 494 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2625 "bang.tab.cpp"
    break;

  case 52: // fn-start-decl: %empty
#line 500 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; }
#line 2631 "bang.tab.cpp"
    break;

  case 53: // fn-start-decl: INLINE
#line 502 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; }
#line 2637 "bang.tab.cpp"
    break;

  case 54: // fn-name: qname
#line 506 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2643 "bang.tab.cpp"
    break;

  case 55: // fn-name: "`new`"
#line 507 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2649 "bang.tab.cpp"
    break;

  case 56: // fn-decl: "`fn`" fn-name "`(`" parameter-list-opt "`)`"
#line 512 "bang.y"
        { yylhs.value.as < fn_pure > () = fn_pure{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2655 "bang.tab.cpp"
    break;

  case 57: // fn-decl: "`fn`" fn-name "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 514 "bang.y"
        { yylhs.value.as < fn_pure > () = fn_pure{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); }
#line 2661 "bang.tab.cpp"
    break;

  case 58: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 520 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ ctx.make_qname_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ())), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2669 "bang.tab.cpp"
    break;

  case 59: // case-list-opt: %empty
#line 526 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2675 "bang.tab.cpp"
    break;

  case 60: // case-list-opt: case-list
#line 528 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2681 "bang.tab.cpp"
    break;

  case 61: // case-list: case-decl
#line 533 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2687 "bang.tab.cpp"
    break;

  case 62: // case-list: case-list "," case-decl
#line 536 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2693 "bang.tab.cpp"
    break;

  case 63: // case-decl: identifier
#line 541 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2699 "bang.tab.cpp"
    break;

  case 64: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 547 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2705 "bang.tab.cpp"
    break;

  case 65: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 549 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_woa_list_t > ()), nullopt }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2711 "bang.tab.cpp"
    break;

  case 66: // using-decl: qname "`=>`" syntax-expression
#line 556 "bang.y"
        { yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2717 "bang.tab.cpp"
    break;

  case 67: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 558 "bang.y"
        { yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2723 "bang.tab.cpp"
    break;

  case 68: // argument-list-opt: %empty
#line 588 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2729 "bang.tab.cpp"
    break;

  case 69: // argument-list-opt: argument-list
#line 589 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2735 "bang.tab.cpp"
    break;

  case 70: // argument-list: argument
#line 594 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2741 "bang.tab.cpp"
    break;

  case 71: // argument-list: argument-list "," argument
#line 596 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2747 "bang.tab.cpp"
    break;

  case 72: // argument: argument-name syntax-expression
#line 601 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2753 "bang.tab.cpp"
    break;

  case 73: // argument: syntax-expression
#line 603 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2759 "bang.tab.cpp"
    break;

  case 74: // argument-name: identifier "`=`"
#line 608 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2765 "bang.tab.cpp"
    break;

  case 75: // argument-name: identifier "`:`"
#line 610 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2771 "bang.tab.cpp"
    break;

  case 76: // field-list-opt: %empty
#line 615 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2777 "bang.tab.cpp"
    break;

  case 77: // field-list-opt: field-list
#line 616 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2783 "bang.tab.cpp"
    break;

  case 78: // field-list: field
#line 621 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2789 "bang.tab.cpp"
    break;

  case 79: // field-list: field-list "," field
#line 623 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2795 "bang.tab.cpp"
    break;

  case 80: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr
#line 628 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2801 "bang.tab.cpp"
    break;

  case 81: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr "`=`" syntax-expression
#line 630 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2807 "bang.tab.cpp"
    break;

  case 82: // field-type-expr: qname
#line 635 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2813 "bang.tab.cpp"
    break;

  case 83: // field-type-expr: qname "`(`" argument-list-opt "`)`"
#line 637 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2819 "bang.tab.cpp"
    break;

  case 84: // field-type-expr: INTERNAL_IDENTIFIER
#line 639 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2825 "bang.tab.cpp"
    break;

  case 85: // field-type-expr: field-type-expr "`|`" field-type-expr
#line 641 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2831 "bang.tab.cpp"
    break;

  case 86: // parameter-list-opt: %empty
#line 646 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2837 "bang.tab.cpp"
    break;

  case 87: // parameter-list-opt: parameter-list
#line 647 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2843 "bang.tab.cpp"
    break;

  case 88: // parameter-list: parameter-decl
#line 652 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2849 "bang.tab.cpp"
    break;

  case 89: // parameter-list: parameter-list "," parameter-decl
#line 654 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2855 "bang.tab.cpp"
    break;

  case 90: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 659 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2861 "bang.tab.cpp"
    break;

  case 91: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 661 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2867 "bang.tab.cpp"
    break;

  case 92: // parameter-decl: parameter-constraint-set
#line 663 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2873 "bang.tab.cpp"
    break;

  case 93: // parameter-name-decl: identifier internal_identifier "`:`"
#line 668 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }; }
#line 2879 "bang.tab.cpp"
    break;

  case 94: // parameter-name-decl: identifier "`:`"
#line 670 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), nullopt }; }
#line 2885 "bang.tab.cpp"
    break;

  case 95: // parameter-name-decl: INTERNAL_IDENTIFIER "`:`"
#line 672 "bang.y"
        { yylhs.value.as < parameter_name > () = unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2891 "bang.tab.cpp"
    break;

  case 96: // parameter-name-decl: INTERNAL_IDENTIFIER "`...`" "`:`"
#line 676 "bang.y"
        { yylhs.value.as < parameter_name > () = varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2897 "bang.tab.cpp"
    break;

  case 97: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 680 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2903 "bang.tab.cpp"
    break;

  case 98: // parameter-constraint-modifier-opt: %empty
#line 682 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2909 "bang.tab.cpp"
    break;

  case 99: // parameter-constraint-modifier: TYPENAME
#line 687 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2915 "bang.tab.cpp"
    break;

  case 100: // parameter-constraint-modifier: "const modifier"
#line 689 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2921 "bang.tab.cpp"
    break;

  case 101: // parameter-constraint-set: parameter-matched-type
#line 694 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2927 "bang.tab.cpp"
    break;

  case 102: // parameter-constraint-set: concept-expression
#line 696 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2933 "bang.tab.cpp"
    break;

  case 103: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 700 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2939 "bang.tab.cpp"
    break;

  case 104: // parameter-matched-type: basic-parameter-matched-type
#line 704 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2945 "bang.tab.cpp"
    break;

  case 105: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 706 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2951 "bang.tab.cpp"
    break;

  case 106: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 708 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2957 "bang.tab.cpp"
    break;

  case 107: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 710 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2963 "bang.tab.cpp"
    break;

  case 108: // basic-parameter-matched-type: "`_`"
#line 715 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2969 "bang.tab.cpp"
    break;

  case 109: // basic-parameter-matched-type: qname
#line 717 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2975 "bang.tab.cpp"
    break;

  case 110: // basic-parameter-matched-type: qname "`(`" argument-list-opt "`)`"
#line 719 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2981 "bang.tab.cpp"
    break;

  case 111: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 721 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2987 "bang.tab.cpp"
    break;

  case 112: // concept-expression: "`@`" qname
#line 744 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 2993 "bang.tab.cpp"
    break;

  case 113: // syntax-expression: INTERNAL_IDENTIFIER
#line 749 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2999 "bang.tab.cpp"
    break;

  case 114: // syntax-expression: syntax-expression-wo-ii
#line 750 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3005 "bang.tab.cpp"
    break;

  case 115: // syntax-expression-wo-ii: "true"
#line 756 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3011 "bang.tab.cpp"
    break;

  case 116: // syntax-expression-wo-ii: "false"
#line 758 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3017 "bang.tab.cpp"
    break;

  case 117: // syntax-expression-wo-ii: INTEGER
#line 760 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3023 "bang.tab.cpp"
    break;

  case 118: // syntax-expression-wo-ii: DECIMAL
#line 762 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3029 "bang.tab.cpp"
    break;

  case 119: // syntax-expression-wo-ii: DECIMAL_S
#line 764 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3035 "bang.tab.cpp"
    break;

  case 120: // syntax-expression-wo-ii: INTEGER_INDEX
#line 766 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3041 "bang.tab.cpp"
    break;

  case 121: // syntax-expression-wo-ii: STRING
#line 768 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3047 "bang.tab.cpp"
    break;

  case 122: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 770 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3053 "bang.tab.cpp"
    break;

  case 123: // syntax-expression-wo-ii: qname
#line 772 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3059 "bang.tab.cpp"
    break;

  case 124: // syntax-expression-wo-ii: "`(`" argument-list "`)`"
#line 774 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) };
            }
        }
#line 3071 "bang.tab.cpp"
    break;

  case 125: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 784 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3077 "bang.tab.cpp"
    break;

  case 126: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 786 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3083 "bang.tab.cpp"
    break;

  case 127: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 788 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3089 "bang.tab.cpp"
    break;

  case 128: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 790 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3095 "bang.tab.cpp"
    break;

  case 129: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 792 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3101 "bang.tab.cpp"
    break;

  case 130: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 798 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3107 "bang.tab.cpp"
    break;

  case 131: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 800 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3113 "bang.tab.cpp"
    break;

  case 132: // syntax-expression-wo-ii: apostrophe-expression
#line 802 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3119 "bang.tab.cpp"
    break;

  case 133: // syntax-expression-wo-ii: new-expression
#line 803 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3125 "bang.tab.cpp"
    break;

  case 134: // syntax-expression-wo-ii: compound-expression
#line 804 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3131 "bang.tab.cpp"
    break;

  case 135: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 809 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3137 "bang.tab.cpp"
    break;

  case 136: // new-expression: "`new`" qname
#line 814 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3143 "bang.tab.cpp"
    break;

  case 137: // new-expression: "`new`" apostrophe-expression
#line 816 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3149 "bang.tab.cpp"
    break;

  case 138: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 818 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3155 "bang.tab.cpp"
    break;

  case 139: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 820 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3161 "bang.tab.cpp"
    break;

  case 140: // call-expression: qname "`(`" argument-list-opt "`)`"
#line 825 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3167 "bang.tab.cpp"
    break;

  case 141: // call-expression: call-expression "`(`" argument-list-opt "`)`"
#line 827 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3173 "bang.tab.cpp"
    break;

  case 142: // call-expression: apostrophe-expression "`(`" argument-list-opt "`)`"
#line 829 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3179 "bang.tab.cpp"
    break;

  case 143: // compound-expression: call-expression
#line 833 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3185 "bang.tab.cpp"
    break;

  case 144: // type-expr: qname
#line 919 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3191 "bang.tab.cpp"
    break;

  case 145: // type-expr: INTERNAL_IDENTIFIER
#line 923 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3197 "bang.tab.cpp"
    break;

  case 146: // type-expr: "`[`" type-expr "`]`"
#line 927 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3203 "bang.tab.cpp"
    break;

  case 147: // type-expr: "`(`" argument-list-opt "`)`"
#line 929 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_tuple_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3209 "bang.tab.cpp"
    break;

  case 148: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 931 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_array_t{std::move(yystack_[3].value.as < syntax_expression_t > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3215 "bang.tab.cpp"
    break;

  case 149: // type-expr: type-expr "`|`" type-expr
#line 933 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3227 "bang.tab.cpp"
    break;

  case 150: // type-expr: type-expr "`->`" type-expr
#line 941 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3233 "bang.tab.cpp"
    break;


#line 3237 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -153;

  const signed char parser::yytable_ninf_ = -53;

  const short
  parser::yypact_[] =
  {
     283,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,   435,
      30,   435,   435,     1,   137,    41,   435,   435,   435,    53,
      38,  -153,    53,    53,  -153,  -153,    46,    19,   190,  -153,
    -153,   -25,    52,  -153,  -153,  -153,   125,    42,  -153,   129,
    -153,    72,  -153,    93,    13,   136,  -153,  -153,    89,   106,
    -153,   435,   219,    26,    30,  -153,    30,     2,  -153,   178,
     102,   126,   182,   219,   183,  -153,  -153,   171,   186,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,    90,   435,    30,
     435,    69,  -153,   435,   435,   435,  -153,    30,   435,   435,
     435,  -153,  -153,  -153,  -153,   435,   219,  -153,   131,  -153,
     138,   141,   435,   435,   422,   435,  -153,   117,   435,    33,
      30,   157,    33,  -153,   435,    90,   138,    10,   219,  -153,
     165,   151,   435,  -153,   219,    14,    14,  -153,    26,   174,
     179,  -153,    90,    33,   187,   199,  -153,  -153,   196,   329,
    -153,   205,  -153,  -153,    42,   182,   208,   219,   135,    53,
    -153,  -153,  -153,    57,   194,   210,   192,  -153,   -51,   161,
     244,  -153,    99,  -153,  -153,   217,   201,  -153,    30,   221,
     223,     6,    90,   262,    90,  -153,   219,  -153,  -153,    10,
     226,  -153,  -153,  -153,  -153,  -153,  -153,  -153,   208,  -153,
    -153,   234,  -153,   138,  -153,  -153,   237,   435,   263,    33,
     161,  -153,   250,   244,  -153,  -153,    35,  -153,    30,   240,
     248,   230,  -153,   279,  -153,  -153,  -153,   249,   110,   281,
    -153,  -153,   254,   435,  -153,   244,  -153,  -153,  -153,   -51,
    -153,    30,   256,  -153,   435,  -153,   219,    48,  -153,    30,
     219,  -153,   197,    -3,   257,   435,   435,    48,  -153,   258,
     219,  -153,  -153
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   121,    47,   113,   122,   120,   117,   118,   119,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,   115,   116,     0,     0,    52,     5,
      40,    23,    19,     7,    16,    50,   123,     0,    14,     0,
     114,   132,   133,   143,   134,     0,   134,    49,    50,     0,
      70,     0,    73,   127,     0,    21,     0,     0,    12,   136,
     137,     0,     0,    41,     0,    18,    13,     0,     0,    17,
       1,     2,    10,     3,     8,     6,     9,     0,     0,     0,
      68,     0,   126,     0,     0,     0,   128,     0,     0,    68,
      68,   135,    74,    75,   124,     0,    72,    22,     0,    55,
      54,     0,    68,    68,    25,     0,    33,    35,     0,    86,
      59,     0,    86,   145,    68,     0,   144,    24,    20,    51,
       0,    69,     0,    37,    43,   130,   131,   125,   129,     0,
       0,    71,     0,    86,     0,     0,    46,    45,    38,    39,
      28,     0,    44,    26,     0,     0,     0,    66,   106,     0,
     108,    99,   100,    50,   109,     0,    87,    88,    98,     0,
      92,   101,   104,   102,    63,     0,    60,    61,    76,     0,
       0,     0,     0,     0,     0,   140,    15,   142,   141,    11,
       0,   138,   139,    31,    29,    30,    27,    32,     0,    34,
      36,   107,    95,   112,    48,    94,     0,    68,     0,     0,
       0,    97,   106,    91,   103,   105,     0,    58,     0,     0,
       0,    77,    78,     0,   147,   146,   150,     0,   149,    56,
      96,    93,     0,     0,    89,    90,   107,   111,    62,    98,
      64,     0,     0,   148,     0,   110,    67,     0,    79,    76,
      57,    84,    82,    80,     0,    68,     0,     0,    65,     0,
      81,    85,    83
  };

  const short
  parser::yypgoto_[] =
  {
    -153,  -153,  -153,  -153,   276,  -153,  -153,  -153,  -153,  -153,
     -29,   -24,  -153,   -77,   168,    11,  -153,    15,     7,  -153,
     -32,  -153,  -153,  -153,   100,  -153,  -153,   -88,   298,   215,
    -153,    73,  -153,    80,    66,    20,  -153,   115,  -153,    86,
    -152,  -114,  -153,   111,  -150,     0,  -153,   301,  -153,  -153,
       3,   -22
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,    26,    27,    28,    29,    30,    31,    32,   138,   139,
     123,    33,   141,    34,   143,    35,   196,    36,    37,   101,
      58,    38,   165,   166,   167,    69,    65,   120,   121,    50,
      51,   210,   211,   212,   243,   155,   156,   157,   158,   200,
     159,   160,   161,   162,   163,    52,    40,    41,    42,    43,
      46,   117
  };

  const short
  parser::yytable_[] =
  {
      39,   129,   130,    44,    76,    81,   201,     2,     2,    45,
     204,   246,    53,   -42,   134,   135,    61,    62,    63,    71,
      77,    47,    48,    82,    55,   172,   170,   142,    39,   172,
      59,    44,   106,   107,    64,    82,     2,    67,    68,     2,
     148,     2,   151,    66,   152,   203,    70,     2,    10,   149,
     150,    96,   150,   204,     2,   241,     9,   173,   215,     2,
     247,   173,   142,   -42,   194,    97,    78,    98,   -42,   174,
      87,    88,   100,   174,    72,   204,    99,   201,   118,    10,
     140,    10,    87,   124,   125,   126,   225,    10,   128,   122,
     119,    48,   116,   171,    10,    54,     2,   113,   127,    10,
      48,    48,   195,    92,    39,   145,    48,    44,   147,   222,
     179,   144,   188,    48,    48,   185,   189,   190,   104,    89,
     153,   164,   176,   153,   154,    48,   151,   154,   152,   172,
     116,    57,   169,   205,    93,    82,    10,   114,    82,    39,
      90,   115,    44,    83,   153,    82,   144,   116,   154,   103,
     216,    91,   218,   180,    94,    84,    85,   249,    84,    85,
      86,   173,   206,    86,   193,    84,    85,     2,   202,   191,
      86,    79,    80,    95,   154,   104,   132,   149,   150,   209,
     192,   105,    87,    88,    79,    87,    88,   116,   133,   116,
      73,    82,    87,    88,   146,     1,     2,     3,     4,     5,
       6,     7,     8,   108,   168,     9,   111,    10,    48,    56,
     153,    84,    85,   175,   154,   154,    86,    79,    95,   164,
     110,   154,   177,   236,    79,   102,    57,   178,    82,    79,
     109,   104,    79,   112,   240,   181,    10,    11,    87,    88,
      79,   197,   209,    79,   245,    74,   250,   182,    84,    85,
     209,   183,   242,    86,    12,   187,    48,   104,   198,   199,
     149,    13,   242,    14,    15,    16,    17,   207,   208,   213,
      18,   214,   217,    19,   219,    87,    88,    20,    21,   220,
      22,    23,   221,   223,   226,   229,    24,    25,     1,     2,
       3,     4,     5,     6,     7,     8,   230,   231,     9,   232,
     234,   233,   235,   239,    75,   248,   252,   186,   228,    49,
     131,   238,   244,   251,   224,   237,    60,   227,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,     0,     0,     1,     2,     3,     4,     5,     6,
       7,     8,     0,     0,     9,     0,     0,    12,     0,     0,
       0,     0,     0,     0,    13,     0,    14,    15,    16,    17,
       0,     0,     0,    18,     0,     0,    19,     0,     0,     0,
      20,    21,   -52,    22,    23,    10,    11,     0,     0,    24,
      25,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,    12,     0,     0,     0,     0,     0,     0,
      13,     0,     0,    15,    16,    17,     0,   136,   137,    18,
       0,     0,     0,     0,     0,     0,     0,    21,   -52,     0,
       0,     0,     0,     0,     0,    24,    25,     1,     2,     3,
       4,     5,     6,     7,     8,     0,     0,     9,     0,     0,
       1,     2,     3,     4,     5,     6,     7,     8,     0,     0,
       9,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,    11,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,    11,     0,     0,     0,    12,     0,     0,     0,
       0,     0,     0,    13,     0,     0,    15,    16,    17,    12,
     136,   137,    18,     0,     0,     0,     0,     0,     0,    15,
      21,   -52,     0,     0,     0,     0,     0,     0,    24,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,    25
  };

  const short
  parser::yycheck_[] =
  {
       0,    89,    90,     0,    28,    37,   158,     6,     6,     9,
     160,    14,    12,     0,   102,   103,    16,    17,    18,     0,
      45,    10,    11,     9,    13,    19,   114,   104,    28,    19,
      15,    28,    61,    62,    19,     9,     6,    22,    23,     6,
       7,     6,    93,     5,    95,   159,     0,     6,    46,    16,
      17,    51,    17,   203,     6,     7,    15,    51,    52,     6,
      63,    51,   139,    50,     7,    54,    14,    56,    55,    63,
      56,    57,    57,    63,    55,   225,    74,   229,    78,    46,
     104,    46,    56,    83,    84,    85,   200,    46,    88,    20,
      79,    80,    77,   115,    46,    94,     6,     7,    87,    46,
      89,    90,    45,    14,   104,   105,    95,   104,   108,   197,
     132,   104,   144,   102,   103,   139,   145,   146,    49,    47,
     109,   110,   122,   112,   109,   114,    93,   112,    95,    19,
     115,    89,   112,    34,    45,     9,    46,    47,     9,   139,
      47,    51,   139,    14,   133,     9,   139,   132,   133,    47,
     172,    15,   174,   133,    48,    29,    30,   245,    29,    30,
      34,    51,    63,    34,   149,    29,    30,     6,     7,    34,
      34,    46,    47,    67,   159,    49,    45,    16,    17,   168,
      45,    55,    56,    57,    46,    56,    57,   172,    47,   174,
       0,     9,    56,    57,    77,     5,     6,     7,     8,     9,
      10,    11,    12,    20,    47,    15,    20,    46,   197,    72,
     199,    29,    30,    48,   199,   200,    34,    46,    67,   208,
      49,   206,    48,   223,    46,    47,    89,    48,     9,    46,
      47,    49,    46,    47,   234,    48,    46,    47,    56,    57,
      46,    47,   231,    46,    47,    55,   246,    48,    29,    30,
     239,    55,   237,    34,    64,    50,   245,    49,    48,    67,
      16,    71,   247,    73,    74,    75,    76,    50,    67,    48,
      80,    48,    10,    83,    48,    56,    57,    87,    88,    45,
      90,    91,    45,    20,    34,    45,    96,    97,     5,     6,
       7,     8,     9,    10,    11,    12,    48,    67,    15,    20,
      19,    52,    48,    47,    28,    48,    48,   139,   208,    11,
      95,   231,   239,   247,   199,   229,    15,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    -1,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    -1,    15,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    74,    75,    76,
      -1,    -1,    -1,    80,    -1,    -1,    83,    -1,    -1,    -1,
      87,    88,    89,    90,    91,    46,    47,    -1,    -1,    96,
      97,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    74,    75,    76,    -1,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    74,    75,    76,    64,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    64,    71,    73,    74,    75,    76,    80,    83,
      87,    88,    90,    91,    96,    97,    99,   100,   101,   102,
     103,   104,   105,   109,   111,   113,   115,   116,   119,   143,
     144,   145,   146,   147,   148,   143,   148,   113,   113,   126,
     127,   128,   143,   143,    94,   113,    72,    89,   118,   115,
     145,   143,   143,   143,   115,   124,     5,   115,   115,   123,
       0,     0,    55,     0,    55,   102,   109,    45,    14,    46,
      47,   118,     9,    14,    29,    30,    34,    56,    57,    47,
      47,    15,    14,    45,    48,    67,   143,   113,   113,    74,
     115,   117,    47,    47,    49,    55,   108,   108,    20,    47,
      49,    20,    47,     7,    47,    51,   115,   149,   143,   113,
     125,   126,    20,   108,   143,   143,   143,   113,   143,   125,
     125,   127,    45,    47,   125,   125,    78,    79,   106,   107,
     109,   110,   111,   112,   116,   143,    77,   143,     7,    16,
      17,    93,    95,   113,   115,   133,   134,   135,   136,   138,
     139,   140,   141,   142,   113,   120,   121,   122,    47,   133,
     125,   149,    19,    51,    63,    48,   143,    48,    48,   149,
     133,    48,    48,    55,    55,   109,   112,    50,   118,   108,
     108,    34,    45,   115,     7,    45,   114,    47,    48,    67,
     137,   138,     7,   139,   142,    34,    63,    50,    67,   113,
     129,   130,   131,    48,    48,    52,   149,    10,   149,    48,
      45,    45,   125,    20,   135,   139,    34,   141,   122,    45,
      48,    67,    20,    52,    19,    48,   143,   137,   131,    47,
     143,     7,   115,   132,   129,    47,    14,    63,    48,   125,
     143,   132,    48
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    98,    99,    99,   100,   100,   100,   101,   101,   101,
     101,   102,   102,   102,   102,   102,   102,   102,   102,   103,
     103,   104,   104,   105,   105,   106,   106,   106,   107,   107,
     107,   107,   108,   109,   109,   109,   109,   109,   110,   110,
     111,   111,   111,   111,   112,   112,   112,   113,   114,   115,
     115,   115,   116,   116,   117,   117,   118,   118,   119,   120,
     120,   121,   121,   122,   123,   123,   124,   124,   125,   125,
     126,   126,   127,   127,   128,   128,   129,   129,   130,   130,
     131,   131,   132,   132,   132,   132,   133,   133,   134,   134,
     135,   135,   135,   136,   136,   136,   136,   137,   137,   138,
     138,   139,   139,   139,   140,   140,   140,   140,   141,   141,
     141,   141,   142,   143,   143,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   145,   146,   146,   146,   146,
     147,   147,   147,   148,   149,   149,   149,   149,   149,   149,
     149
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     2,     2,     1,     4,     1,     2,     2,     1,
       3,     2,     3,     1,     3,     0,     1,     2,     1,     2,
       2,     2,     3,     3,     5,     3,     5,     3,     1,     1,
       1,     2,     1,     3,     1,     1,     1,     1,     1,     2,
       1,     3,     0,     1,     1,     1,     5,     7,     5,     0,
       1,     1,     3,     1,     5,     8,     3,     6,     0,     1,
       1,     3,     2,     1,     2,     2,     0,     1,     1,     3,
       4,     6,     1,     4,     1,     3,     0,     1,     1,     3,
       3,     2,     1,     3,     2,     2,     3,     1,     0,     1,
       1,     1,     1,     2,     1,     2,     1,     2,     1,     1,
       4,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     2,     2,     3,
       3,     3,     1,     1,     1,     3,     2,     2,     5,     5,
       4,     4,     4,     1,     1,     1,     3,     3,     4,     3,
       3
  };


#if BANG_LANGDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "COMMENT_BEGIN",
  "COMMENT_END", "STRING", "IDENTIFIER", "INTERNAL_IDENTIFIER",
  "RESERVED_IDENTIFIER", "INTEGER_INDEX", "INTEGER", "DECIMAL",
  "DECIMAL_S", "OPERATOR_TERM", "\"`=`\"", "\"APOSTROPHE\"", "\"`@`\"",
  "\"`_`\"", "\"`->*`\"", "\"`->`\"", "\"`=>`\"", "\"`~>`\"", "\"`.*`\"",
  "\"`++`\"", "\"`--`\"", "\"`<<`\"", "\"`>>`\"", "\"`<=`\"", "\"`>=`\"",
  "\"`==`\"", "\"`!=`\"", "\"`&&`\"", "\"`||`\"", "\"`..`\"", "\"`...`\"",
  "\"`+=`\"", "\"`-=`\"", "\"`*=`\"", "\"`/=`\"", "\"`%=`\"", "\"`<<=`\"",
  "\"`>>=`\"", "\"`&=`\"", "\"`|=`\"", "\"`^=`\"", "\"`:`\"", "\"`::`\"",
  "\"`(`\"", "\"`)`\"", "\"`{`\"", "\"`}`\"", "\"`[`\"", "\"`]`\"",
  "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"", "\"`+`\"", "\"`-`\"",
  "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"", "\"`|`\"", "\"`!`\"",
  "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"", "\"`?`\"", "\"`#`\"", "LET",
  "VAR", "EXTERN", "\"`new`\"", "\"`while`\"", "\"`if`\"", "\"`else`\"",
  "\"`continue`\"", "\"`break`\"", "\"`return`\"", "FOR", "AUTO", "USING",
  "THROW", "SIZEOF", "LOWEST", "INCLUDE", "INLINE", "\"`fn`\"", "ENUM",
  "STRUCT", "EXTENDS", "TYPENAME", "\"weak modifier\"",
  "\"const modifier\"", "\"true\"", "\"false\"", "$accept", "begin",
  "statement_any", "finished_statement_any", "statement", "let-decl",
  "let-decl-start", "let-decl-start-with-opt-type",
  "infunction-statement-any", "finished-infunction-statement-any",
  "braced-statements", "finished_statement", "infunction-statement-set",
  "generic-statement", "infunction-statement", "identifier",
  "internal_identifier", "qname", "fn-start-decl", "fn-name", "fn-decl",
  "enum-decl", "case-list-opt", "case-list", "case-decl", "struct-decl",
  "using-decl", "argument-list-opt", "argument-list", "argument",
  "argument-name", "field-list-opt", "field-list", "field",
  "field-type-expr", "parameter-list-opt", "parameter-list",
  "parameter-decl", "parameter-name-decl",
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
       0,   330,   330,   331,   335,   337,   339,   356,   358,   359,
     361,   366,   368,   370,   372,   377,   379,   381,   383,   388,
     390,   395,   397,   402,   403,   408,   410,   412,   417,   418,
     419,   421,   425,   430,   432,   434,   436,   438,   443,   443,
     446,   448,   450,   452,   457,   459,   461,   466,   474,   489,
     491,   493,   499,   501,   506,   507,   511,   513,   519,   526,
     528,   532,   535,   540,   546,   548,   555,   557,   588,   589,
     593,   595,   600,   602,   607,   609,   615,   616,   620,   622,
     627,   629,   634,   636,   638,   640,   646,   647,   651,   653,
     658,   660,   662,   667,   669,   671,   675,   680,   681,   686,
     688,   693,   695,   699,   704,   705,   707,   709,   714,   716,
     718,   720,   743,   748,   750,   755,   757,   759,   761,   763,
     765,   767,   769,   771,   773,   783,   785,   787,   789,   791,
     797,   799,   802,   803,   804,   808,   813,   815,   817,   819,
     824,   826,   828,   833,   918,   922,   926,   928,   930,   932,
     940
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
      95,    96,    97
    };
    // Last valid token kind.
    const int code_max = 352;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4020 "bang.tab.cpp"

#line 1325 "bang.y"

