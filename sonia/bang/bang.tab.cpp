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
#line 25 "bang.y"

#include "sonia/config.hpp"
#include <iostream>
#include "parser.hpp"

using namespace sonia;
using namespace sonia::lang::bang;

#include "bang.tab.hpp"

#include "unit.hpp"
#include "sonia/bang/utility/linked_list.ipp"

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

#define IGNORE_TERM(...)




#line 75 "bang.tab.cpp"


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
#line 172 "bang.tab.cpp"

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
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.copy< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
      case symbol_kind::S_131_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
      case symbol_kind::S_127_argument_list: // argument-list
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
        value.copy< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_135_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_129_argument_name: // argument-name
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_118_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_case_decl: // case-decl
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
      case symbol_kind::S_113_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
      case symbol_kind::S_122_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_143_concept_expression: // concept-expression
      case symbol_kind::S_144_syntax_expression: // syntax-expression
      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_147_new_expression: // new-expression
      case symbol_kind::S_148_call_expression: // call-expression
      case symbol_kind::S_149_lambda_expression: // lambda-expression
      case symbol_kind::S_150_compound_expression: // compound-expression
      case symbol_kind::S_151_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_using_decl: // using-decl
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
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.move< annotated_bool > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
      case symbol_kind::S_131_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
      case symbol_kind::S_127_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_135_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_129_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_118_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_case_decl: // case-decl
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
      case symbol_kind::S_113_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
      case symbol_kind::S_122_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_143_concept_expression: // concept-expression
      case symbol_kind::S_144_syntax_expression: // syntax-expression
      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_147_new_expression: // new-expression
      case symbol_kind::S_148_call_expression: // call-expression
      case symbol_kind::S_149_lambda_expression: // lambda-expression
      case symbol_kind::S_150_compound_expression: // compound-expression
      case symbol_kind::S_151_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_using_decl: // using-decl
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
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.YY_MOVE_OR_COPY< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
      case symbol_kind::S_131_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
      case symbol_kind::S_127_argument_list: // argument-list
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
        value.YY_MOVE_OR_COPY< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
        value.YY_MOVE_OR_COPY< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_135_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_129_argument_name: // argument-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_118_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_case_decl: // case-decl
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
      case symbol_kind::S_113_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
      case symbol_kind::S_122_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_143_concept_expression: // concept-expression
      case symbol_kind::S_144_syntax_expression: // syntax-expression
      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_147_new_expression: // new-expression
      case symbol_kind::S_148_call_expression: // call-expression
      case symbol_kind::S_149_lambda_expression: // lambda-expression
      case symbol_kind::S_150_compound_expression: // compound-expression
      case symbol_kind::S_151_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_using_decl: // using-decl
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
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.move< annotated_bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
      case symbol_kind::S_131_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
      case symbol_kind::S_127_argument_list: // argument-list
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_135_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_129_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_118_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_case_decl: // case-decl
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
      case symbol_kind::S_113_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
      case symbol_kind::S_122_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_143_concept_expression: // concept-expression
      case symbol_kind::S_144_syntax_expression: // syntax-expression
      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_147_new_expression: // new-expression
      case symbol_kind::S_148_call_expression: // call-expression
      case symbol_kind::S_149_lambda_expression: // lambda-expression
      case symbol_kind::S_150_compound_expression: // compound-expression
      case symbol_kind::S_151_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_using_decl: // using-decl
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
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.copy< annotated_bool > (that.value);
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
      case symbol_kind::S_131_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
      case symbol_kind::S_127_argument_list: // argument-list
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
        value.copy< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
        value.copy< parameter_name > (that.value);
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_135_parameter_list: // parameter-list
        value.copy< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_129_argument_name: // argument-name
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_118_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_123_case_decl: // case-decl
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
      case symbol_kind::S_113_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
      case symbol_kind::S_122_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_143_concept_expression: // concept-expression
      case symbol_kind::S_144_syntax_expression: // syntax-expression
      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_147_new_expression: // new-expression
      case symbol_kind::S_148_call_expression: // call-expression
      case symbol_kind::S_149_lambda_expression: // lambda-expression
      case symbol_kind::S_150_compound_expression: // compound-expression
      case symbol_kind::S_151_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_125_using_decl: // using-decl
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
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.move< annotated_bool > (that.value);
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
      case symbol_kind::S_131_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
      case symbol_kind::S_127_argument_list: // argument-list
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
        value.move< parameter_constraint_modifier_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (that.value);
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_135_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_129_argument_name: // argument-name
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_118_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_123_case_decl: // case-decl
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
      case symbol_kind::S_113_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
      case symbol_kind::S_122_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_143_concept_expression: // concept-expression
      case symbol_kind::S_144_syntax_expression: // syntax-expression
      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_147_new_expression: // new-expression
      case symbol_kind::S_148_call_expression: // call-expression
      case symbol_kind::S_149_lambda_expression: // lambda-expression
      case symbol_kind::S_150_compound_expression: // compound-expression
      case symbol_kind::S_151_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_125_using_decl: // using-decl
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
#line 331 "bang.y"
                 { }
#line 1446 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 331 "bang.y"
                 { }
#line 1452 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 331 "bang.y"
                 { }
#line 1458 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 331 "bang.y"
                 { }
#line 1464 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 331 "bang.y"
                 { }
#line 1470 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 331 "bang.y"
                 { }
#line 1476 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 331 "bang.y"
                 { }
#line 1482 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 331 "bang.y"
                 { }
#line 1488 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 331 "bang.y"
                 { }
#line 1494 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 331 "bang.y"
                 { }
#line 1500 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 331 "bang.y"
                 { }
#line 1506 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 331 "bang.y"
                 { }
#line 1512 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 331 "bang.y"
                 { }
#line 1518 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 331 "bang.y"
                 { }
#line 1524 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 331 "bang.y"
                 { }
#line 1530 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 331 "bang.y"
                 { }
#line 1536 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 331 "bang.y"
                 { }
#line 1542 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 331 "bang.y"
                 { }
#line 1548 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 331 "bang.y"
                 { }
#line 1554 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 331 "bang.y"
                 { }
#line 1560 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 331 "bang.y"
                 { }
#line 1566 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 331 "bang.y"
                 { }
#line 1572 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 331 "bang.y"
                 { }
#line 1578 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 331 "bang.y"
                 { }
#line 1584 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 331 "bang.y"
                 { }
#line 1590 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 331 "bang.y"
                 { }
#line 1596 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 331 "bang.y"
                 { }
#line 1602 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 331 "bang.y"
                 { }
#line 1608 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 331 "bang.y"
                 { }
#line 1614 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 331 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 331 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement_any: // finished_statement_any
#line 331 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 331 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_let_decl: // let-decl
#line 331 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_let_decl_start: // let-decl-start
#line 331 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 331 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
#line 331 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
#line 331 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_braced_statements: // braced-statements
#line 331 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement: // finished_statement
#line 331 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
#line 331 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
#line 331 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_infunction_statement: // infunction-statement
#line 331 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 331 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_internal_identifier: // internal_identifier
#line 331 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 331 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
#line 331 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_fn_name: // fn-name
#line 331 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
#line 331 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
#line 331 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
#line 331 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_case_list: // case-list
#line 331 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_case_decl: // case-decl
#line 331 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
#line 331 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_using_decl: // using-decl
#line 331 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
#line 331 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_argument_list: // argument-list
#line 331 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 331 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_argument_name: // argument-name
#line 331 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
#line 331 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_field_list: // field-list
#line 331 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 331 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
#line 331 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
#line 331 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_parameter_list: // parameter-list
#line 331 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
#line 331 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
#line 331 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 331 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
#line 331 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
#line 331 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
#line 331 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
#line 331 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_concept_expression: // concept-expression
#line 331 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_syntax_expression: // syntax-expression
#line 331 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 331 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
#line 331 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_new_expression: // new-expression
#line 331 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_call_expression: // call-expression
#line 331 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_lambda_expression: // lambda-expression
#line 331 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_compound_expression: // compound-expression
#line 331 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_type_expr: // type-expr
#line 331 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
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
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        yylhs.value.emplace< annotated_bool > ();
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
      case symbol_kind::S_131_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_finished_statement: // finished_statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< managed_statement_list > ();
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
      case symbol_kind::S_127_argument_list: // argument-list
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
        yylhs.value.emplace< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
        yylhs.value.emplace< parameter_name > ();
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_135_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_internal_identifier: // internal_identifier
      case symbol_kind::S_129_argument_name: // argument-name
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_118_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_123_case_decl: // case-decl
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
      case symbol_kind::S_113_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
      case symbol_kind::S_122_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_143_concept_expression: // concept-expression
      case symbol_kind::S_144_syntax_expression: // syntax-expression
      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_147_new_expression: // new-expression
      case symbol_kind::S_148_call_expression: // call-expression
      case symbol_kind::S_149_lambda_expression: // lambda-expression
      case symbol_kind::S_150_compound_expression: // compound-expression
      case symbol_kind::S_151_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_125_using_decl: // using-decl
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
#line 336 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2349 "bang.tab.cpp"
    break;

  case 3: // begin: finished_statement_any "end of file"
#line 337 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2355 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 342 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2361 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 344 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2367 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished_statement_any statement
#line 346 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2373 "bang.tab.cpp"
    break;

  case 7: // finished_statement_any: finished_statement
#line 363 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2379 "bang.tab.cpp"
    break;

  case 8: // finished_statement_any: finished_statement_any "`;`"
#line 364 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2385 "bang.tab.cpp"
    break;

  case 9: // finished_statement_any: finished_statement_any finished_statement
#line 366 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2391 "bang.tab.cpp"
    break;

  case 10: // finished_statement_any: statement_any "`;`"
#line 368 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2397 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 373 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2403 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 375 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2409 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 377 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2415 "bang.tab.cpp"
    break;

  case 14: // statement: enum-decl
#line 379 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2421 "bang.tab.cpp"
    break;

  case 15: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 384 "bang.y"
        {
            yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > ();
            auto sts = ctx.new_statement_list();
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) });
            yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), ctx.push(std::move(sts)) };
        }
#line 2432 "bang.tab.cpp"
    break;

  case 16: // statement: generic-statement
#line 391 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2438 "bang.tab.cpp"
    break;

  case 17: // statement: STRUCT struct-decl
#line 393 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2444 "bang.tab.cpp"
    break;

  case 18: // statement: USING using-decl
#line 395 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2450 "bang.tab.cpp"
    break;

  case 19: // let-decl: let-decl-start-with-opt-type
#line 400 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2456 "bang.tab.cpp"
    break;

  case 20: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 402 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2462 "bang.tab.cpp"
    break;

  case 21: // let-decl-start: LET identifier
#line 407 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2468 "bang.tab.cpp"
    break;

  case 22: // let-decl-start: LET "weak modifier" identifier
#line 409 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2474 "bang.tab.cpp"
    break;

  case 23: // let-decl-start-with-opt-type: let-decl-start
#line 413 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2480 "bang.tab.cpp"
    break;

  case 24: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 415 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2486 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: %empty
#line 420 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2492 "bang.tab.cpp"
    break;

  case 26: // infunction-statement-any: infunction-statement
#line 422 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2498 "bang.tab.cpp"
    break;

  case 27: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 424 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2504 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished_statement
#line 429 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2510 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 430 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2516 "bang.tab.cpp"
    break;

  case 30: // finished-infunction-statement-any: finished-infunction-statement-any finished_statement
#line 432 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2522 "bang.tab.cpp"
    break;

  case 31: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 434 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2528 "bang.tab.cpp"
    break;

  case 32: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 439 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2534 "bang.tab.cpp"
    break;

  case 33: // finished_statement: "`while`" syntax-expression braced-statements
#line 444 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2540 "bang.tab.cpp"
    break;

  case 34: // finished_statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 446 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2546 "bang.tab.cpp"
    break;

  case 35: // finished_statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 448 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2552 "bang.tab.cpp"
    break;

  case 36: // finished_statement: "`if`" syntax-expression braced-statements
#line 450 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2558 "bang.tab.cpp"
    break;

  case 37: // finished_statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 452 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2564 "bang.tab.cpp"
    break;

  case 38: // finished_statement: fn-start-decl fn-decl braced-statements
#line 454 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2570 "bang.tab.cpp"
    break;

  case 39: // finished_statement: STRUCT qname braced-statements
#line 456 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2576 "bang.tab.cpp"
    break;

  case 40: // finished_statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 458 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2582 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: infunction-statement-any
#line 463 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2588 "bang.tab.cpp"
    break;

  case 42: // infunction-statement-set: finished-infunction-statement-any
#line 465 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2594 "bang.tab.cpp"
    break;

  case 43: // generic-statement: let-decl
#line 470 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2600 "bang.tab.cpp"
    break;

  case 44: // generic-statement: "`return`" syntax-expression
#line 472 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2606 "bang.tab.cpp"
    break;

  case 45: // generic-statement: compound-expression
#line 474 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2612 "bang.tab.cpp"
    break;

  case 46: // generic-statement: syntax-expression "`=`" syntax-expression
#line 476 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2618 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: generic-statement
#line 481 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2624 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`break`"
#line 483 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2630 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`continue`"
#line 485 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2636 "bang.tab.cpp"
    break;

  case 50: // identifier: IDENTIFIER
#line 490 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2642 "bang.tab.cpp"
    break;

  case 51: // internal_identifier: INTERNAL_IDENTIFIER
#line 498 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2648 "bang.tab.cpp"
    break;

  case 52: // qname: "`::`" identifier
#line 513 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2654 "bang.tab.cpp"
    break;

  case 53: // qname: identifier
#line 515 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2660 "bang.tab.cpp"
    break;

  case 54: // qname: qname "`::`" identifier
#line 517 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2666 "bang.tab.cpp"
    break;

  case 55: // fn-start-decl: "`fn`"
#line 523 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2672 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: INLINE "`fn`"
#line 525 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2678 "bang.tab.cpp"
    break;

  case 57: // fn-name: qname
#line 529 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2684 "bang.tab.cpp"
    break;

  case 58: // fn-name: "`new`"
#line 530 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2690 "bang.tab.cpp"
    break;

  case 59: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 535 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2696 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" type-expr
#line 537 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2702 "bang.tab.cpp"
    break;

  case 61: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 543 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ ctx.make_qname_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ())), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2710 "bang.tab.cpp"
    break;

  case 62: // case-list-opt: %empty
#line 549 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2716 "bang.tab.cpp"
    break;

  case 63: // case-list-opt: case-list
#line 551 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2722 "bang.tab.cpp"
    break;

  case 64: // case-list: case-decl
#line 556 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2728 "bang.tab.cpp"
    break;

  case 65: // case-list: case-list "," case-decl
#line 559 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2734 "bang.tab.cpp"
    break;

  case 66: // case-decl: identifier
#line 564 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2740 "bang.tab.cpp"
    break;

  case 67: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 570 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2746 "bang.tab.cpp"
    break;

  case 68: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 572 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_woa_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2752 "bang.tab.cpp"
    break;

  case 69: // using-decl: qname "`=>`" syntax-expression
#line 579 "bang.y"
        { yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2758 "bang.tab.cpp"
    break;

  case 70: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 581 "bang.y"
        { yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2764 "bang.tab.cpp"
    break;

  case 71: // argument-list-opt: %empty
#line 611 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2770 "bang.tab.cpp"
    break;

  case 72: // argument-list-opt: argument-list
#line 612 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2776 "bang.tab.cpp"
    break;

  case 73: // argument-list: argument
#line 617 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2782 "bang.tab.cpp"
    break;

  case 74: // argument-list: argument-list "," argument
#line 619 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2788 "bang.tab.cpp"
    break;

  case 75: // argument: argument-name syntax-expression
#line 624 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2794 "bang.tab.cpp"
    break;

  case 76: // argument: syntax-expression
#line 626 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2800 "bang.tab.cpp"
    break;

  case 77: // argument-name: identifier "`=`"
#line 631 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2806 "bang.tab.cpp"
    break;

  case 78: // argument-name: identifier "`:`"
#line 633 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2812 "bang.tab.cpp"
    break;

  case 79: // field-list-opt: %empty
#line 638 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2818 "bang.tab.cpp"
    break;

  case 80: // field-list-opt: field-list
#line 639 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2824 "bang.tab.cpp"
    break;

  case 81: // field-list: field
#line 644 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2830 "bang.tab.cpp"
    break;

  case 82: // field-list: field-list "," field
#line 646 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2836 "bang.tab.cpp"
    break;

  case 83: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr
#line 651 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2842 "bang.tab.cpp"
    break;

  case 84: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr "`=`" syntax-expression
#line 653 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2848 "bang.tab.cpp"
    break;

  case 85: // field-type-expr: qname
#line 658 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2854 "bang.tab.cpp"
    break;

  case 86: // field-type-expr: qname "`(`" argument-list-opt "`)`"
#line 660 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2860 "bang.tab.cpp"
    break;

  case 87: // field-type-expr: INTERNAL_IDENTIFIER
#line 662 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2866 "bang.tab.cpp"
    break;

  case 88: // field-type-expr: field-type-expr "`|`" field-type-expr
#line 664 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2872 "bang.tab.cpp"
    break;

  case 89: // parameter-list-opt: %empty
#line 669 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2878 "bang.tab.cpp"
    break;

  case 90: // parameter-list-opt: parameter-list
#line 670 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2884 "bang.tab.cpp"
    break;

  case 91: // parameter-list: parameter-decl
#line 675 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2890 "bang.tab.cpp"
    break;

  case 92: // parameter-list: parameter-list "," parameter-decl
#line 677 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2896 "bang.tab.cpp"
    break;

  case 93: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 682 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2902 "bang.tab.cpp"
    break;

  case 94: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 684 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2908 "bang.tab.cpp"
    break;

  case 95: // parameter-decl: parameter-constraint-set
#line 686 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2914 "bang.tab.cpp"
    break;

  case 96: // parameter-name-decl: identifier internal_identifier "`:`"
#line 691 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }; }
#line 2920 "bang.tab.cpp"
    break;

  case 97: // parameter-name-decl: identifier "`:`"
#line 693 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), nullopt }; }
#line 2926 "bang.tab.cpp"
    break;

  case 98: // parameter-name-decl: INTERNAL_IDENTIFIER "`:`"
#line 695 "bang.y"
        { yylhs.value.as < parameter_name > () = unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2932 "bang.tab.cpp"
    break;

  case 99: // parameter-name-decl: INTERNAL_IDENTIFIER "`...`" "`:`"
#line 699 "bang.y"
        { yylhs.value.as < parameter_name > () = varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2938 "bang.tab.cpp"
    break;

  case 100: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 703 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2944 "bang.tab.cpp"
    break;

  case 101: // parameter-constraint-modifier-opt: %empty
#line 705 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2950 "bang.tab.cpp"
    break;

  case 102: // parameter-constraint-modifier: TYPENAME
#line 710 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2956 "bang.tab.cpp"
    break;

  case 103: // parameter-constraint-modifier: "const modifier"
#line 712 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2962 "bang.tab.cpp"
    break;

  case 104: // parameter-constraint-set: parameter-matched-type
#line 717 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2968 "bang.tab.cpp"
    break;

  case 105: // parameter-constraint-set: concept-expression
#line 719 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2974 "bang.tab.cpp"
    break;

  case 106: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 723 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2980 "bang.tab.cpp"
    break;

  case 107: // parameter-matched-type: basic-parameter-matched-type
#line 727 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2986 "bang.tab.cpp"
    break;

  case 108: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 729 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2992 "bang.tab.cpp"
    break;

  case 109: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 731 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2998 "bang.tab.cpp"
    break;

  case 110: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 733 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }}; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3004 "bang.tab.cpp"
    break;

  case 111: // basic-parameter-matched-type: "`_`"
#line 738 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3010 "bang.tab.cpp"
    break;

  case 112: // basic-parameter-matched-type: qname
#line 740 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3016 "bang.tab.cpp"
    break;

  case 113: // basic-parameter-matched-type: qname "`(`" argument-list-opt "`)`"
#line 742 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3022 "bang.tab.cpp"
    break;

  case 114: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 744 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3028 "bang.tab.cpp"
    break;

  case 115: // concept-expression: "`@`" qname
#line 767 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3034 "bang.tab.cpp"
    break;

  case 116: // syntax-expression: INTERNAL_IDENTIFIER
#line 772 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3040 "bang.tab.cpp"
    break;

  case 117: // syntax-expression: syntax-expression-wo-ii
#line 773 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3046 "bang.tab.cpp"
    break;

  case 118: // syntax-expression-wo-ii: "true"
#line 779 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3052 "bang.tab.cpp"
    break;

  case 119: // syntax-expression-wo-ii: "false"
#line 781 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3058 "bang.tab.cpp"
    break;

  case 120: // syntax-expression-wo-ii: INTEGER
#line 783 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3064 "bang.tab.cpp"
    break;

  case 121: // syntax-expression-wo-ii: DECIMAL
#line 785 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3070 "bang.tab.cpp"
    break;

  case 122: // syntax-expression-wo-ii: DECIMAL_S
#line 787 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3076 "bang.tab.cpp"
    break;

  case 123: // syntax-expression-wo-ii: INTEGER_INDEX
#line 789 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3082 "bang.tab.cpp"
    break;

  case 124: // syntax-expression-wo-ii: STRING
#line 791 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3088 "bang.tab.cpp"
    break;

  case 125: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 793 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3094 "bang.tab.cpp"
    break;

  case 126: // syntax-expression-wo-ii: qname
#line 795 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3100 "bang.tab.cpp"
    break;

  case 127: // syntax-expression-wo-ii: "`(`" argument-list "`)`"
#line 797 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) };
            }
        }
#line 3112 "bang.tab.cpp"
    break;

  case 128: // syntax-expression-wo-ii: "`.`" identifier
#line 805 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = context_identifier { std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3118 "bang.tab.cpp"
    break;

  case 129: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 809 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3124 "bang.tab.cpp"
    break;

  case 130: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 811 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3130 "bang.tab.cpp"
    break;

  case 131: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 813 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3136 "bang.tab.cpp"
    break;

  case 132: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 815 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3142 "bang.tab.cpp"
    break;

  case 133: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 821 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3148 "bang.tab.cpp"
    break;

  case 134: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 823 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3154 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: apostrophe-expression
#line 825 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3160 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: new-expression
#line 826 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3166 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: compound-expression
#line 827 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3172 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: lambda-expression
#line 828 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3178 "bang.tab.cpp"
    break;

  case 139: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 833 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3184 "bang.tab.cpp"
    break;

  case 140: // new-expression: "`new`" qname
#line 838 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3190 "bang.tab.cpp"
    break;

  case 141: // new-expression: "`new`" apostrophe-expression
#line 840 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3196 "bang.tab.cpp"
    break;

  case 142: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 842 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3202 "bang.tab.cpp"
    break;

  case 143: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 844 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3208 "bang.tab.cpp"
    break;

  case 144: // call-expression: qname "`(`" argument-list-opt "`)`"
#line 849 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3214 "bang.tab.cpp"
    break;

  case 145: // call-expression: call-expression "`(`" argument-list-opt "`)`"
#line 851 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3220 "bang.tab.cpp"
    break;

  case 146: // call-expression: apostrophe-expression "`(`" argument-list-opt "`)`"
#line 853 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3226 "bang.tab.cpp"
    break;

  case 147: // call-expression: lambda-expression "`(`" argument-list-opt "`)`"
#line 855 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3232 "bang.tab.cpp"
    break;

  case 148: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 860 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; IGNORE_TERM(yystack_[4].value.as < fn_kind > ());  }
#line 3238 "bang.tab.cpp"
    break;

  case 149: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 862 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE_TERM(yystack_[6].value.as < fn_kind > ()); }
#line 3244 "bang.tab.cpp"
    break;

  case 150: // compound-expression: call-expression
#line 866 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3250 "bang.tab.cpp"
    break;

  case 151: // compound-expression: syntax-expression "`...`"
#line 868 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3256 "bang.tab.cpp"
    break;

  case 152: // type-expr: qname
#line 954 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3262 "bang.tab.cpp"
    break;

  case 153: // type-expr: call-expression
#line 955 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3268 "bang.tab.cpp"
    break;

  case 154: // type-expr: INTERNAL_IDENTIFIER
#line 959 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3274 "bang.tab.cpp"
    break;

  case 155: // type-expr: "`[`" type-expr "`]`"
#line 963 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3280 "bang.tab.cpp"
    break;

  case 156: // type-expr: "`(`" argument-list-opt "`)`"
#line 965 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_tuple_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3286 "bang.tab.cpp"
    break;

  case 157: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 967 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_array_t{std::move(yystack_[3].value.as < syntax_expression_t > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3292 "bang.tab.cpp"
    break;

  case 158: // type-expr: type-expr "`|`" type-expr
#line 969 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3304 "bang.tab.cpp"
    break;

  case 159: // type-expr: type-expr "`->`" type-expr
#line 977 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3310 "bang.tab.cpp"
    break;


#line 3314 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -147;

  const signed char parser::yytable_ninf_ = -46;

  const short
  parser::yypact_[] =
  {
     416,  -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,   198,
      17,   198,    17,   198,     2,   -39,    44,   198,   198,   198,
     198,    86,     1,   -51,  -147,    86,    86,  -147,  -147,    47,
      45,   322,  -147,  -147,    82,   142,  -147,  -147,  -147,   127,
      61,  -147,   161,  -147,   115,  -147,   155,   169,    32,   175,
     326,  -147,  -147,    60,   112,  -147,   198,   317,  -147,    20,
      17,  -147,    17,     6,   183,   177,   264,   212,   315,   317,
     168,  -147,  -147,  -147,   147,   262,  -147,  -147,  -147,  -147,
    -147,  -147,  -147,  -147,   213,   198,    17,   198,    55,  -147,
     180,   187,   103,  -147,   198,   198,   198,  -147,    17,   198,
     198,   198,   198,  -147,  -147,  -147,  -147,   198,   317,  -147,
     192,  -147,   198,   198,   604,   198,  -147,   198,   164,   198,
      55,    17,   203,    55,  -147,  -147,   198,   213,   127,   115,
     155,   169,     3,   317,  -147,   204,   196,   123,    86,  -147,
    -147,  -147,    23,   185,   222,   219,  -147,    98,   268,   255,
    -147,    12,  -147,    55,   198,  -147,   317,   107,   107,  -147,
      20,   232,   243,   249,  -147,   213,   251,   253,  -147,  -147,
      86,   250,   510,  -147,   257,  -147,  -147,    61,   315,   315,
     261,   317,   267,  -147,   275,   245,  -147,    17,   270,   288,
      -3,   213,   328,   213,  -147,   294,  -147,   180,  -147,  -147,
     295,   198,    99,    55,   268,  -147,   308,   255,  -147,  -147,
      87,   300,   317,  -147,  -147,  -147,     3,  -147,  -147,   134,
    -147,  -147,  -147,  -147,  -147,   261,  -147,  -147,  -147,   323,
    -147,    17,   305,   304,   286,  -147,   122,  -147,  -147,  -147,
     302,    62,  -147,  -147,   309,   213,  -147,  -147,   255,  -147,
    -147,   340,    55,   198,  -147,    98,  -147,    17,   314,  -147,
    -147,  -147,   216,   213,   318,   317,    37,  -147,    17,  -147,
       3,   261,  -147,   201,    10,   319,   198,   198,    37,  -147,
     327,   317,  -147,  -147
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   124,    50,   116,   125,   123,   120,   121,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,   118,   119,     0,
       0,     0,     5,    43,    23,    19,     7,    16,    53,   126,
       0,    14,     0,   117,   135,   136,   150,   138,   137,     0,
       0,   137,    52,    53,     0,    73,     0,    76,   128,   131,
       0,    21,     0,     0,   140,   141,     0,     0,     0,    44,
       0,    18,    13,    56,     0,     0,    17,     1,     2,    10,
       3,     8,     6,     9,     0,     0,     0,    71,    89,    58,
      57,     0,     0,   130,     0,     0,     0,   151,     0,     0,
      71,    71,    71,   139,    77,    78,   127,     0,    75,    22,
       0,    12,    71,    71,    25,     0,    33,     0,    36,     0,
      89,    62,     0,    89,    39,   154,    71,     0,   152,     0,
     153,     0,    24,    20,    54,     0,    72,   109,     0,   111,
     102,   103,    53,   112,     0,    90,    91,   101,     0,    95,
     104,   107,   105,    89,     0,    38,    46,   133,   134,   129,
     132,     0,     0,     0,    74,     0,     0,     0,    49,    48,
       0,    41,    42,    28,     0,    47,    26,     0,     0,     0,
       0,    69,     0,    66,     0,    63,    64,    79,     0,     0,
       0,     0,     0,     0,   144,   110,    98,   115,    51,    97,
       0,    71,     0,     0,     0,   100,   109,    94,   106,   108,
       0,     0,    15,   146,   145,   147,    11,   142,   143,     0,
      31,    29,    30,    27,    32,     0,    34,    35,    37,     0,
      61,     0,     0,     0,    80,    81,     0,   156,   155,   159,
       0,   158,    99,    96,     0,     0,   148,    92,    93,   110,
     114,    59,    89,     0,    65,   101,    67,     0,     0,    40,
     157,   113,     0,     0,     0,    70,     0,    82,    79,   149,
      60,     0,    87,    85,    83,     0,    71,     0,     0,    68,
       0,    84,    88,    86
  };

  const short
  parser::yypgoto_[] =
  {
    -147,  -147,  -147,  -147,   331,  -147,  -147,  -147,  -147,  -147,
     -13,   -26,  -147,  -103,   191,    24,  -147,   113,    26,  -147,
     -22,  -147,  -147,  -147,   139,  -147,  -147,   -98,   369,   274,
    -147,   116,  -147,   128,   109,   -83,  -147,   186,  -147,   133,
    -146,  -106,  -147,   182,  -142,     0,  -147,    -6,  -147,   -63,
      -7,    27,  -102
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    29,    30,    31,    32,    33,    34,    35,   171,   172,
     124,    36,   174,    37,   176,    38,   200,    39,    49,    91,
      92,    41,   184,   185,   186,    76,    71,   135,   136,    55,
      56,   233,   234,   235,   274,   144,   145,   146,   147,   204,
     148,   149,   150,   151,   152,    57,    43,    44,    45,    46,
      47,    51,   132
  };

  const short
  parser::yytable_[] =
  {
      42,   205,   161,   162,   163,    83,    72,   208,     2,    50,
      65,   175,     2,    59,   166,   167,   191,    66,    67,    68,
      69,   130,   191,     2,   277,   190,    40,    48,   189,    93,
     198,    42,   -45,    62,    52,    53,    58,   182,    61,    73,
     188,   111,   207,     2,   272,    78,   209,    77,   192,   238,
       2,    63,    10,   116,   192,   118,   108,    40,    48,     9,
     193,     2,   137,   216,   130,   208,   193,     2,   199,   175,
     211,   138,   139,   278,   104,   210,    98,   131,   129,   155,
      89,   191,   -45,    10,   109,   133,   110,   -45,   173,   239,
      10,   241,     2,     2,   156,   157,   158,    60,   248,   160,
      79,    10,   130,   244,   139,   105,   208,    10,    88,   205,
     134,    53,   142,   192,    42,   178,    93,   179,   245,   181,
     131,   129,   159,   154,    53,    53,    53,    84,   130,    64,
     130,    53,    10,    10,    70,    89,    53,    53,    74,    75,
     177,    48,   258,   262,   142,   183,   222,   142,   114,   140,
      53,   141,   114,    90,   212,   225,    85,   195,   131,   129,
     106,   270,   100,    98,    99,   226,   227,   228,   196,   264,
      93,   114,    42,    86,    87,    94,    90,   142,   280,   107,
      86,   252,   130,   114,   131,   129,   131,   129,   119,   246,
      95,    96,   140,    86,   141,    97,   121,   128,   177,    48,
     130,   143,   101,     1,     2,     3,     4,     5,     6,     7,
       8,   232,   155,     9,    86,   120,   102,    98,    99,     2,
     125,    93,    88,   259,   113,    53,    86,   142,     9,    86,
     112,    86,   201,   143,   153,   191,   143,   165,   131,   129,
     128,    95,    96,   180,    10,    11,    97,    86,   276,   269,
     187,   197,   194,   265,    12,   183,   131,   129,   259,    10,
     126,   143,    13,   107,   127,   114,   143,   192,    98,    99,
     202,   138,    16,    93,     2,   206,   142,   281,   128,   193,
     213,   232,   122,   219,   138,   139,   203,    23,    24,   117,
      90,   214,   232,    95,    96,    27,    28,   215,    97,   217,
      53,   218,    23,    24,   128,   220,   128,   224,    86,   123,
     114,   114,   231,   114,    10,   229,   143,   143,   236,   115,
      98,    99,    80,   143,    93,   230,    93,     1,     2,     3,
       4,     5,     6,     7,     8,    93,   237,     9,   240,   242,
     243,   103,   249,   253,    95,    96,    95,    96,   251,    97,
     255,    97,   256,   257,   260,    95,    96,   261,   128,   263,
      97,   268,    82,   223,   114,   143,   271,   279,    10,    11,
     254,    98,    99,    98,    99,   283,   128,    81,    12,   273,
      54,   164,    98,    99,   275,   267,    13,   282,   266,   247,
       0,   273,   250,    14,     0,    15,    16,    17,    18,     0,
      19,     0,     0,     0,    20,     0,    21,     0,     0,     0,
      22,    23,    24,    25,    26,     0,     0,     0,     0,    27,
      28,     1,     2,     3,     4,     5,     6,     7,     8,     0,
       0,     9,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    10,    11,     0,     0,     0,     0,     0,     0,
       0,     0,    12,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     0,     0,     0,     0,     0,    14,     0,    15,
      16,    17,    18,     0,    19,     0,     0,     0,    20,     0,
      21,     0,     0,     0,    22,    23,    24,    25,    26,     0,
       0,     0,     0,    27,    28,     1,     2,     3,     4,     5,
       6,     7,     8,     0,     0,     9,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
       0,     0,     0,     0,     0,   221,    12,     0,     0,     0,
       0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
       0,    14,     0,     0,    16,    17,    18,     0,    19,     0,
     168,   169,    20,     0,     0,     0,     0,     0,     0,    23,
      24,     0,   170,     0,     0,     0,     0,    27,    28,     1,
       2,     3,     4,     5,     6,     7,     8,     0,     0,     9,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      10,    11,     0,     0,     0,     0,     0,     0,     0,     0,
      12,     0,     0,     0,     0,     0,     0,     0,    13,     0,
       0,     0,     0,     0,     0,    14,     0,     0,    16,    17,
      18,     0,    19,     0,   168,   169,    20,     0,     0,     0,
       0,     0,     0,    23,    24,     0,   170,     0,     0,     0,
       0,    27,    28
  };

  const short
  parser::yycheck_[] =
  {
       0,   147,   100,   101,   102,    31,     5,   149,     6,     9,
      16,   114,     6,    13,   112,   113,    19,    17,    18,    19,
      20,    84,    19,     6,    14,   127,     0,     0,   126,     9,
       7,    31,     0,    72,    10,    11,    12,   120,    14,    90,
     123,    63,   148,     6,     7,     0,    34,     0,    51,    52,
       6,    90,    46,    66,    51,    68,    56,    31,    31,    15,
      63,     6,     7,   165,   127,   207,    63,     6,    45,   172,
     153,    16,    17,    63,    14,    63,    56,    84,    84,    92,
      74,    19,    50,    46,    60,    85,    62,    55,   114,   191,
      46,   193,     6,     6,    94,    95,    96,    95,   204,    99,
      55,    46,   165,   201,    17,    45,   248,    46,    47,   255,
      86,    87,    88,    51,   114,   115,     9,   117,    19,   119,
     127,   127,    98,    20,   100,   101,   102,    45,   191,    16,
     193,   107,    46,    46,    21,    74,   112,   113,    25,    26,
     114,   114,    20,   245,   120,   121,   172,   123,    49,    94,
     126,    96,    49,    40,   154,   177,    14,    34,   165,   165,
      48,   263,    47,    56,    57,   178,   179,   180,    45,   252,
       9,    49,   172,    46,    47,    14,    63,   153,   276,    67,
      46,    47,   245,    49,   191,   191,   193,   193,    20,   202,
      29,    30,    94,    46,    96,    34,    49,    84,   172,   172,
     263,    88,    47,     5,     6,     7,     8,     9,    10,    11,
      12,   187,   225,    15,    46,    47,    47,    56,    57,     6,
       7,     9,    47,   236,    47,   201,    46,   203,    15,    46,
      47,    46,    47,   120,    47,    19,   123,    45,   245,   245,
     127,    29,    30,    79,    46,    47,    34,    46,    47,   262,
      47,   138,    48,   253,    56,   231,   263,   263,   271,    46,
      47,   148,    64,    67,    51,    49,   153,    51,    56,    57,
      48,    16,    74,     9,     6,     7,   252,   277,   165,    63,
      48,   257,    20,   170,    16,    17,    67,    89,    90,    77,
     177,    48,   268,    29,    30,    97,    98,    48,    34,    48,
     276,    48,    89,    90,   191,    55,   193,    50,    46,    47,
      49,    49,    67,    49,    46,    48,   203,   204,    48,    55,
      56,    57,     0,   210,     9,    50,     9,     5,     6,     7,
       8,     9,    10,    11,    12,     9,    48,    15,    10,    45,
      45,    15,    34,    20,    29,    30,    29,    30,    48,    34,
      45,    34,    48,    67,    52,    29,    30,    48,   245,    19,
      34,    47,    31,   172,    49,   252,    48,    48,    46,    47,
     231,    56,    57,    56,    57,    48,   263,    55,    56,   266,
      11,   107,    56,    57,   268,   257,    64,   278,   255,   203,
      -1,   278,   210,    71,    -1,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    -1,    82,    -1,    84,    -1,    -1,    -1,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    97,
      98,     5,     6,     7,     8,     9,    10,    11,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      74,    75,    76,    -1,    78,    -1,    -1,    -1,    82,    -1,
      84,    -1,    -1,    -1,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    97,    98,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    74,    75,    76,    -1,    78,    -1,
      80,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    92,    -1,    -1,    -1,    -1,    97,    98,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    74,    75,
      76,    -1,    78,    -1,    80,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    92,    -1,    -1,    -1,
      -1,    97,    98
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    56,    64,    71,    73,    74,    75,    76,    78,
      82,    84,    88,    89,    90,    91,    92,    97,    98,   100,
     101,   102,   103,   104,   105,   106,   110,   112,   114,   116,
     117,   120,   144,   145,   146,   147,   148,   149,   150,   117,
     144,   150,   114,   114,   127,   128,   129,   144,   114,   144,
      95,   114,    72,    90,   116,   146,   144,   144,   144,   144,
     116,   125,     5,    90,   116,   116,   124,     0,     0,    55,
       0,    55,   103,   110,    45,    14,    46,    47,    47,    74,
     116,   118,   119,     9,    14,    29,    30,    34,    56,    57,
      47,    47,    47,    15,    14,    45,    48,    67,   144,   114,
     114,   119,    47,    47,    49,    55,   109,    77,   109,    20,
      47,    49,    20,    47,   109,     7,    47,    51,   116,   146,
     148,   149,   151,   144,   114,   126,   127,     7,    16,    17,
      94,    96,   114,   116,   134,   135,   136,   137,   139,   140,
     141,   142,   143,    47,    20,   109,   144,   144,   144,   114,
     144,   126,   126,   126,   128,    45,   126,   126,    80,    81,
      92,   107,   108,   110,   111,   112,   113,   117,   144,   144,
      79,   144,   134,   114,   121,   122,   123,    47,   134,   126,
     151,    19,    51,    63,    48,    34,    45,   116,     7,    45,
     115,    47,    48,    67,   138,   139,     7,   140,   143,    34,
      63,   134,   144,    48,    48,    48,   151,    48,    48,   116,
      55,    55,   110,   113,    50,   119,   109,   109,   109,    48,
      50,    67,   114,   130,   131,   132,    48,    48,    52,   151,
      10,   151,    45,    45,   126,    19,   109,   136,   140,    34,
     142,    48,    47,    20,   123,    45,    48,    67,    20,   109,
      52,    48,   151,    19,   134,   144,   138,   132,    47,   109,
     151,    48,     7,   116,   133,   130,    47,    14,    63,    48,
     126,   144,   133,    48
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    99,   100,   100,   101,   101,   101,   102,   102,   102,
     102,   103,   103,   103,   103,   103,   103,   103,   103,   104,
     104,   105,   105,   106,   106,   107,   107,   107,   108,   108,
     108,   108,   109,   110,   110,   110,   110,   110,   110,   110,
     110,   111,   111,   112,   112,   112,   112,   113,   113,   113,
     114,   115,   116,   116,   116,   117,   117,   118,   118,   119,
     119,   120,   121,   121,   122,   122,   123,   124,   124,   125,
     125,   126,   126,   127,   127,   128,   128,   129,   129,   130,
     130,   131,   131,   132,   132,   133,   133,   133,   133,   134,
     134,   135,   135,   136,   136,   136,   137,   137,   137,   137,
     138,   138,   139,   139,   140,   140,   140,   141,   141,   141,
     141,   142,   142,   142,   142,   143,   144,   144,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   146,
     147,   147,   147,   147,   148,   148,   148,   148,   149,   149,
     150,   150,   151,   151,   151,   151,   151,   151,   151,   151
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     3,     2,     1,     4,     1,     2,     2,     1,
       3,     2,     3,     1,     3,     0,     1,     2,     1,     2,
       2,     2,     3,     3,     5,     5,     3,     5,     3,     3,
       6,     1,     1,     1,     2,     1,     3,     1,     1,     1,
       1,     1,     2,     1,     3,     1,     2,     1,     1,     4,
       6,     5,     0,     1,     1,     3,     1,     5,     8,     3,
       6,     0,     1,     1,     3,     2,     1,     2,     2,     0,
       1,     1,     3,     4,     6,     1,     4,     1,     3,     0,
       1,     1,     3,     3,     2,     1,     3,     2,     2,     3,
       1,     0,     1,     1,     1,     1,     2,     1,     2,     1,
       2,     1,     1,     4,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     3,
       2,     2,     3,     3,     3,     1,     1,     1,     1,     3,
       2,     2,     5,     5,     4,     4,     4,     4,     5,     7,
       1,     2,     1,     1,     1,     3,     3,     4,     3,     3
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
  "VAR", "EXTERN", "\"`new`\"", "\"`while`\"", "\"`for`\"", "\"`in`\"",
  "\"`if`\"", "\"`else`\"", "\"`continue`\"", "\"`break`\"",
  "\"`return`\"", "AUTO", "USING", "THROW", "SIZEOF", "LOWEST", "INCLUDE",
  "INLINE", "\"`fn`\"", "ENUM", "STRUCT", "EXTENDS", "TYPENAME",
  "\"weak modifier\"", "\"const modifier\"", "\"true\"", "\"false\"",
  "$accept", "begin", "statement_any", "finished_statement_any",
  "statement", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction-statement-any",
  "finished-infunction-statement-any", "braced-statements",
  "finished_statement", "infunction-statement-set", "generic-statement",
  "infunction-statement", "identifier", "internal_identifier", "qname",
  "fn-start-decl", "fn-name", "fn-decl", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "struct-decl", "using-decl",
  "argument-list-opt", "argument-list", "argument", "argument-name",
  "field-list-opt", "field-list", "field", "field-type-expr",
  "parameter-list-opt", "parameter-list", "parameter-decl",
  "parameter-name-decl", "parameter-constraint-modifier-opt",
  "parameter-constraint-modifier", "parameter-constraint-set",
  "parameter-matched-type", "basic-parameter-matched-type",
  "concept-expression", "syntax-expression", "syntax-expression-wo-ii",
  "apostrophe-expression", "new-expression", "call-expression",
  "lambda-expression", "compound-expression", "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   336,   336,   337,   341,   343,   345,   362,   364,   365,
     367,   372,   374,   376,   378,   383,   390,   392,   394,   399,
     401,   406,   408,   413,   414,   419,   421,   423,   428,   430,
     431,   433,   438,   443,   445,   447,   449,   451,   453,   455,
     457,   462,   464,   469,   471,   473,   475,   480,   482,   484,
     489,   497,   512,   514,   516,   522,   524,   529,   530,   534,
     536,   542,   549,   551,   555,   558,   563,   569,   571,   578,
     580,   611,   612,   616,   618,   623,   625,   630,   632,   638,
     639,   643,   645,   650,   652,   657,   659,   661,   663,   669,
     670,   674,   676,   681,   683,   685,   690,   692,   694,   698,
     703,   704,   709,   711,   716,   718,   722,   727,   728,   730,
     732,   737,   739,   741,   743,   766,   771,   773,   778,   780,
     782,   784,   786,   788,   790,   792,   794,   796,   804,   808,
     810,   812,   814,   820,   822,   825,   826,   827,   828,   832,
     837,   839,   841,   843,   848,   850,   852,   854,   859,   861,
     866,   867,   953,   955,   958,   962,   964,   966,   968,   976
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
      95,    96,    97,    98
    };
    // Last valid token kind.
    const int code_max = 353;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4141 "bang.tab.cpp"

#line 1353 "bang.y"

