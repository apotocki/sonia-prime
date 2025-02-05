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
        value.copy< fn_pure > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
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

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.copy< statement_set_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_149_compound_expression: // compound-expression
      case symbol_kind::S_150_type_expr: // type-expr
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
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
        value.move< fn_pure > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
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

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.move< statement_set_t > (YY_MOVE (s.value));
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
      case symbol_kind::S_149_compound_expression: // compound-expression
      case symbol_kind::S_150_type_expr: // type-expr
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
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
        value.YY_MOVE_OR_COPY< fn_pure > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
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

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< statement_set_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_149_compound_expression: // compound-expression
      case symbol_kind::S_150_type_expr: // type-expr
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
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
        value.move< fn_pure > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
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

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.move< statement_set_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_149_compound_expression: // compound-expression
      case symbol_kind::S_150_type_expr: // type-expr
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
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
        value.copy< fn_pure > (that.value);
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
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

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.copy< statement_set_t > (that.value);
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
      case symbol_kind::S_149_compound_expression: // compound-expression
      case symbol_kind::S_150_type_expr: // type-expr
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
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
        value.move< fn_pure > (that.value);
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
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

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        value.move< statement_set_t > (that.value);
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
      case symbol_kind::S_149_compound_expression: // compound-expression
      case symbol_kind::S_150_type_expr: // type-expr
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
#line 326 "bang.y"
                 { }
#line 1435 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1441 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1447 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 326 "bang.y"
                 { }
#line 1453 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 326 "bang.y"
                 { }
#line 1459 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 326 "bang.y"
                 { }
#line 1465 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 326 "bang.y"
                 { }
#line 1471 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 326 "bang.y"
                 { }
#line 1477 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 326 "bang.y"
                 { }
#line 1483 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 326 "bang.y"
                 { }
#line 1489 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 326 "bang.y"
                 { }
#line 1495 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 326 "bang.y"
                 { }
#line 1501 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 326 "bang.y"
                 { }
#line 1507 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 326 "bang.y"
                 { }
#line 1513 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 326 "bang.y"
                 { }
#line 1519 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 326 "bang.y"
                 { }
#line 1525 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 326 "bang.y"
                 { }
#line 1531 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 326 "bang.y"
                 { }
#line 1537 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 326 "bang.y"
                 { }
#line 1543 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 326 "bang.y"
                 { }
#line 1549 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 326 "bang.y"
                 { }
#line 1555 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 326 "bang.y"
                 { }
#line 1561 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 326 "bang.y"
                 { }
#line 1567 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 326 "bang.y"
                 { }
#line 1573 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 326 "bang.y"
                 { }
#line 1579 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 326 "bang.y"
                 { }
#line 1585 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 326 "bang.y"
                 { }
#line 1591 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 326 "bang.y"
                 { }
#line 1597 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 326 "bang.y"
                 { }
#line 1603 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 326 "bang.y"
                 { }
#line 1609 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 326 "bang.y"
                 { }
#line 1615 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement_any: // finished_statement_any
#line 326 "bang.y"
                 { }
#line 1621 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 326 "bang.y"
                 { }
#line 1627 "bang.tab.cpp"
        break;

      case symbol_kind::S_104_let_decl: // let-decl
#line 326 "bang.y"
                 { }
#line 1633 "bang.tab.cpp"
        break;

      case symbol_kind::S_105_let_decl_start: // let-decl-start
#line 326 "bang.y"
                 { }
#line 1639 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 326 "bang.y"
                 { }
#line 1645 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
#line 326 "bang.y"
                 { }
#line 1651 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
#line 326 "bang.y"
                 { }
#line 1657 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_braced_statements: // braced-statements
#line 326 "bang.y"
                 { }
#line 1663 "bang.tab.cpp"
        break;

      case symbol_kind::S_finished_statement: // finished_statement
#line 326 "bang.y"
                 { }
#line 1669 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
#line 326 "bang.y"
                 { }
#line 1675 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
#line 326 "bang.y"
                 { }
#line 1681 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_infunction_statement: // infunction-statement
#line 326 "bang.y"
                 { }
#line 1687 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 326 "bang.y"
                 { }
#line 1693 "bang.tab.cpp"
        break;

      case symbol_kind::S_internal_identifier: // internal_identifier
#line 326 "bang.y"
                 { }
#line 1699 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 326 "bang.y"
                 { }
#line 1705 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_fn_start_decl: // fn-start-decl
#line 326 "bang.y"
                 { }
#line 1711 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_fn_name: // fn-name
#line 326 "bang.y"
                 { }
#line 1717 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_fn_decl: // fn-decl
#line 326 "bang.y"
                 { }
#line 1723 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_enum_decl: // enum-decl
#line 326 "bang.y"
                 { }
#line 1729 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_case_list_opt: // case-list-opt
#line 326 "bang.y"
                 { }
#line 1735 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_case_list: // case-list
#line 326 "bang.y"
                 { }
#line 1741 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_case_decl: // case-decl
#line 326 "bang.y"
                 { }
#line 1747 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_struct_decl: // struct-decl
#line 326 "bang.y"
                 { }
#line 1753 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_using_decl: // using-decl
#line 326 "bang.y"
                 { }
#line 1759 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_argument_list_opt: // argument-list-opt
#line 326 "bang.y"
                 { }
#line 1765 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_argument_list: // argument-list
#line 326 "bang.y"
                 { }
#line 1771 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 326 "bang.y"
                 { }
#line 1777 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_argument_name: // argument-name
#line 326 "bang.y"
                 { }
#line 1783 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_field_list_opt: // field-list-opt
#line 326 "bang.y"
                 { }
#line 1789 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_field_list: // field-list
#line 326 "bang.y"
                 { }
#line 1795 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 326 "bang.y"
                 { }
#line 1801 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_field_type_expr: // field-type-expr
#line 326 "bang.y"
                 { }
#line 1807 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_parameter_list_opt: // parameter-list-opt
#line 326 "bang.y"
                 { }
#line 1813 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_parameter_list: // parameter-list
#line 326 "bang.y"
                 { }
#line 1819 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_parameter_decl: // parameter-decl
#line 326 "bang.y"
                 { }
#line 1825 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_parameter_name_decl: // parameter-name-decl
#line 326 "bang.y"
                 { }
#line 1831 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 326 "bang.y"
                 { }
#line 1837 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_parameter_constraint_modifier: // parameter-constraint-modifier
#line 326 "bang.y"
                 { }
#line 1843 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_constraint_set: // parameter-constraint-set
#line 326 "bang.y"
                 { }
#line 1849 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_matched_type: // parameter-matched-type
#line 326 "bang.y"
                 { }
#line 1855 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_basic_parameter_matched_type: // basic-parameter-matched-type
#line 326 "bang.y"
                 { }
#line 1861 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_concept_expression: // concept-expression
#line 326 "bang.y"
                 { }
#line 1867 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_syntax_expression: // syntax-expression
#line 326 "bang.y"
                 { }
#line 1873 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 326 "bang.y"
                 { }
#line 1879 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_apostrophe_expression: // apostrophe-expression
#line 326 "bang.y"
                 { }
#line 1885 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_new_expression: // new-expression
#line 326 "bang.y"
                 { }
#line 1891 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_call_expression: // call-expression
#line 326 "bang.y"
                 { }
#line 1897 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_compound_expression: // compound-expression
#line 326 "bang.y"
                 { }
#line 1903 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_type_expr: // type-expr
#line 326 "bang.y"
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
        yylhs.value.emplace< fn_pure > ();
        break;

      case symbol_kind::S_112_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_104_let_decl: // let-decl
      case symbol_kind::S_105_let_decl_start: // let-decl-start
      case symbol_kind::S_106_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
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

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_finished_statement_any: // finished_statement_any
      case symbol_kind::S_107_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_108_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_109_braced_statements: // braced-statements
      case symbol_kind::S_111_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< statement_set_t > ();
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
      case symbol_kind::S_149_compound_expression: // compound-expression
      case symbol_kind::S_150_type_expr: // type-expr
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
#line 331 "bang.y"
                            { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2331 "bang.tab.cpp"
    break;

  case 3: // begin: finished_statement_any "end of file"
#line 332 "bang.y"
                                 { ctx.set_statements(std::move(yystack_[1].value.as < statement_set_t > ())); }
#line 2337 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 337 "bang.y"
        { yylhs.value.as < statement_set_t > () = {}; }
#line 2343 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 339 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < statement > ()) }; }
#line 2349 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished_statement_any statement
#line 341 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2355 "bang.tab.cpp"
    break;

  case 7: // finished_statement_any: finished_statement
#line 358 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2361 "bang.tab.cpp"
    break;

  case 8: // finished_statement_any: finished_statement_any "`;`"
#line 359 "bang.y"
                                           { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2367 "bang.tab.cpp"
    break;

  case 9: // finished_statement_any: finished_statement_any finished_statement
#line 361 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2373 "bang.tab.cpp"
    break;

  case 10: // finished_statement_any: statement_any "`;`"
#line 363 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2379 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 368 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2385 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN fn-decl
#line 370 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure > ()); }
#line 2391 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 372 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2397 "bang.tab.cpp"
    break;

  case 14: // statement: enum-decl
#line 374 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2403 "bang.tab.cpp"
    break;

  case 15: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 379 "bang.y"
        { yystack_[2].value.as < fn_pure > ().kind = yystack_[3].value.as < fn_kind > (); yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure > ()), { return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } } }; }
#line 2409 "bang.tab.cpp"
    break;

  case 16: // statement: generic-statement
#line 381 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2415 "bang.tab.cpp"
    break;

  case 17: // statement: STRUCT struct-decl
#line 383 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2421 "bang.tab.cpp"
    break;

  case 18: // statement: USING using-decl
#line 385 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2427 "bang.tab.cpp"
    break;

  case 19: // let-decl: let-decl-start-with-opt-type
#line 390 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2433 "bang.tab.cpp"
    break;

  case 20: // let-decl: let-decl-start-with-opt-type "`=`" syntax-expression
#line 392 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expression = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2439 "bang.tab.cpp"
    break;

  case 21: // let-decl-start: LET identifier
#line 397 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2445 "bang.tab.cpp"
    break;

  case 22: // let-decl-start: LET "weak modifier" identifier
#line 399 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2451 "bang.tab.cpp"
    break;

  case 23: // let-decl-start-with-opt-type: let-decl-start
#line 403 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2457 "bang.tab.cpp"
    break;

  case 24: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 405 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2463 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: %empty
#line 410 "bang.y"
        { yylhs.value.as < statement_set_t > () = {}; }
#line 2469 "bang.tab.cpp"
    break;

  case 26: // infunction-statement-any: infunction-statement
#line 412 "bang.y"
        { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < statement > ()) }; }
#line 2475 "bang.tab.cpp"
    break;

  case 27: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 414 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2481 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished_statement
#line 418 "bang.y"
                         { yylhs.value.as < statement_set_t > () = statement_set_t{ std::move(yystack_[0].value.as < finished_statement_type > ()) }; }
#line 2487 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 419 "bang.y"
                                                      { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2493 "bang.tab.cpp"
    break;

  case 30: // finished-infunction-statement-any: finished-infunction-statement-any finished_statement
#line 421 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); yylhs.value.as < statement_set_t > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2499 "bang.tab.cpp"
    break;

  case 31: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 422 "bang.y"
                                             { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); }
#line 2505 "bang.tab.cpp"
    break;

  case 32: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 427 "bang.y"
        { yylhs.value.as < statement_set_t > () = std::move(yystack_[1].value.as < statement_set_t > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2511 "bang.tab.cpp"
    break;

  case 33: // finished_statement: "`while`" syntax-expression braced-statements
#line 432 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2517 "bang.tab.cpp"
    break;

  case 34: // finished_statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 434 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2523 "bang.tab.cpp"
    break;

  case 35: // finished_statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 436 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2529 "bang.tab.cpp"
    break;

  case 36: // finished_statement: "`if`" syntax-expression braced-statements
#line 438 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2535 "bang.tab.cpp"
    break;

  case 37: // finished_statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 440 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < statement_set_t > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2541 "bang.tab.cpp"
    break;

  case 38: // finished_statement: fn-start-decl fn-decl braced-statements
#line 442 "bang.y"
        { yystack_[1].value.as < fn_pure > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure > ()), std::move(yystack_[0].value.as < statement_set_t > ()) };  }
#line 2547 "bang.tab.cpp"
    break;

  case 39: // finished_statement: STRUCT qname braced-statements
#line 444 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[0].value.as < statement_set_t > ()) }; }
#line 2553 "bang.tab.cpp"
    break;

  case 40: // finished_statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 446 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_woa_list_t > ()) }, std::move(yystack_[0].value.as < statement_set_t > ()) }; IGNORE(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2559 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: infunction-statement-any
#line 450 "bang.y"
    { yylhs.value.as < statement_set_t > () = yystack_[0].value.as < statement_set_t > (); }
#line 2565 "bang.tab.cpp"
    break;

  case 42: // infunction-statement-set: finished-infunction-statement-any
#line 450 "bang.y"
                               { yylhs.value.as < statement_set_t > () = yystack_[0].value.as < statement_set_t > (); }
#line 2571 "bang.tab.cpp"
    break;

  case 43: // generic-statement: let-decl
#line 454 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2577 "bang.tab.cpp"
    break;

  case 44: // generic-statement: "`return`" syntax-expression
#line 456 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2583 "bang.tab.cpp"
    break;

  case 45: // generic-statement: compound-expression
#line 458 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2589 "bang.tab.cpp"
    break;

  case 46: // generic-statement: syntax-expression "`=`" syntax-expression
#line 460 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2595 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: generic-statement
#line 465 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2601 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`break`"
#line 467 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2607 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`continue`"
#line 469 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2613 "bang.tab.cpp"
    break;

  case 50: // identifier: IDENTIFIER
#line 474 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2619 "bang.tab.cpp"
    break;

  case 51: // internal_identifier: INTERNAL_IDENTIFIER
#line 482 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2625 "bang.tab.cpp"
    break;

  case 52: // qname: "`::`" identifier
#line 497 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2631 "bang.tab.cpp"
    break;

  case 53: // qname: identifier
#line 499 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2637 "bang.tab.cpp"
    break;

  case 54: // qname: qname "`::`" identifier
#line 501 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2643 "bang.tab.cpp"
    break;

  case 55: // fn-start-decl: %empty
#line 507 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; }
#line 2649 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: INLINE
#line 509 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; }
#line 2655 "bang.tab.cpp"
    break;

  case 57: // fn-name: qname
#line 513 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2661 "bang.tab.cpp"
    break;

  case 58: // fn-name: "`new`"
#line 514 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2667 "bang.tab.cpp"
    break;

  case 59: // fn-decl: "`fn`" fn-name "`(`" parameter-list-opt "`)`"
#line 519 "bang.y"
        { yylhs.value.as < fn_pure > () = fn_pure{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_woa_list_t > ()), nullopt }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2673 "bang.tab.cpp"
    break;

  case 60: // fn-decl: "`fn`" fn-name "`(`" parameter-list-opt "`)`" "`->`" syntax-expression
#line 521 "bang.y"
        { yylhs.value.as < fn_pure > () = fn_pure{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); }
#line 2679 "bang.tab.cpp"
    break;

  case 61: // enum-decl: ENUM qname "`{`" case-list-opt "`}`"
#line 527 "bang.y"
    {
        yylhs.value.as < enum_decl > () = enum_decl{ ctx.make_qname_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ())), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ());
    }
#line 2687 "bang.tab.cpp"
    break;

  case 62: // case-list-opt: %empty
#line 533 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2693 "bang.tab.cpp"
    break;

  case 63: // case-list-opt: case-list
#line 535 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2699 "bang.tab.cpp"
    break;

  case 64: // case-list: case-decl
#line 540 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2705 "bang.tab.cpp"
    break;

  case 65: // case-list: case-list "," case-decl
#line 543 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2711 "bang.tab.cpp"
    break;

  case 66: // case-decl: identifier
#line 548 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2717 "bang.tab.cpp"
    break;

  case 67: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 554 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2723 "bang.tab.cpp"
    break;

  case 68: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 556 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_woa_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2729 "bang.tab.cpp"
    break;

  case 69: // using-decl: qname "`=>`" syntax-expression
#line 563 "bang.y"
        { yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2735 "bang.tab.cpp"
    break;

  case 70: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 565 "bang.y"
        { yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_woa_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2741 "bang.tab.cpp"
    break;

  case 71: // argument-list-opt: %empty
#line 595 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2747 "bang.tab.cpp"
    break;

  case 72: // argument-list-opt: argument-list
#line 596 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2753 "bang.tab.cpp"
    break;

  case 73: // argument-list: argument
#line 601 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2759 "bang.tab.cpp"
    break;

  case 74: // argument-list: argument-list "," argument
#line 603 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2765 "bang.tab.cpp"
    break;

  case 75: // argument: argument-name syntax-expression
#line 608 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2771 "bang.tab.cpp"
    break;

  case 76: // argument: syntax-expression
#line 610 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2777 "bang.tab.cpp"
    break;

  case 77: // argument-name: identifier "`=`"
#line 615 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2783 "bang.tab.cpp"
    break;

  case 78: // argument-name: identifier "`:`"
#line 617 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2789 "bang.tab.cpp"
    break;

  case 79: // field-list-opt: %empty
#line 622 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2795 "bang.tab.cpp"
    break;

  case 80: // field-list-opt: field-list
#line 623 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2801 "bang.tab.cpp"
    break;

  case 81: // field-list: field
#line 628 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2807 "bang.tab.cpp"
    break;

  case 82: // field-list: field-list "," field
#line 630 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2813 "bang.tab.cpp"
    break;

  case 83: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr
#line 635 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2819 "bang.tab.cpp"
    break;

  case 84: // field: identifier "`:`" parameter-constraint-modifier-opt field-type-expr "`=`" syntax-expression
#line 637 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2825 "bang.tab.cpp"
    break;

  case 85: // field-type-expr: qname
#line 642 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2831 "bang.tab.cpp"
    break;

  case 86: // field-type-expr: qname "`(`" argument-list-opt "`)`"
#line 644 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2837 "bang.tab.cpp"
    break;

  case 87: // field-type-expr: INTERNAL_IDENTIFIER
#line 646 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2843 "bang.tab.cpp"
    break;

  case 88: // field-type-expr: field-type-expr "`|`" field-type-expr
#line 648 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2849 "bang.tab.cpp"
    break;

  case 89: // parameter-list-opt: %empty
#line 653 "bang.y"
              { yylhs.value.as < parameter_woa_list_t > () = {}; }
#line 2855 "bang.tab.cpp"
    break;

  case 90: // parameter-list-opt: parameter-list
#line 654 "bang.y"
       { yylhs.value.as < parameter_woa_list_t > () = yystack_[0].value.as < parameter_woa_list_t > (); }
#line 2861 "bang.tab.cpp"
    break;

  case 91: // parameter-list: parameter-decl
#line 659 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = parameter_woa_list_t{ parameter_woa_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 2867 "bang.tab.cpp"
    break;

  case 92: // parameter-list: parameter-list "," parameter-decl
#line 661 "bang.y"
        { yylhs.value.as < parameter_woa_list_t > () = std::move(yystack_[2].value.as < parameter_woa_list_t > ()); yylhs.value.as < parameter_woa_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 2873 "bang.tab.cpp"
    break;

  case 93: // parameter-decl: parameter-name-decl parameter-constraint-modifier-opt parameter-constraint-set
#line 666 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ std::move(yystack_[2].value.as < parameter_name > ()), std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2879 "bang.tab.cpp"
    break;

  case 94: // parameter-decl: parameter-constraint-modifier parameter-constraint-set
#line 668 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, std::move(yystack_[1].value.as < parameter_constraint_modifier_t > ()), std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2885 "bang.tab.cpp"
    break;

  case 95: // parameter-decl: parameter-constraint-set
#line 670 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type_constraint, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 2891 "bang.tab.cpp"
    break;

  case 96: // parameter-name-decl: identifier internal_identifier "`:`"
#line 675 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()) }; }
#line 2897 "bang.tab.cpp"
    break;

  case 97: // parameter-name-decl: identifier "`:`"
#line 677 "bang.y"
        { yylhs.value.as < parameter_name > () = named_parameter_name{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), nullopt }; }
#line 2903 "bang.tab.cpp"
    break;

  case 98: // parameter-name-decl: INTERNAL_IDENTIFIER "`:`"
#line 679 "bang.y"
        { yylhs.value.as < parameter_name > () = unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2909 "bang.tab.cpp"
    break;

  case 99: // parameter-name-decl: INTERNAL_IDENTIFIER "`...`" "`:`"
#line 683 "bang.y"
        { yylhs.value.as < parameter_name > () = varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2915 "bang.tab.cpp"
    break;

  case 100: // parameter-constraint-modifier-opt: parameter-constraint-modifier
#line 687 "bang.y"
      { yylhs.value.as < parameter_constraint_modifier_t > () = yystack_[0].value.as < parameter_constraint_modifier_t > (); }
#line 2921 "bang.tab.cpp"
    break;

  case 101: // parameter-constraint-modifier-opt: %empty
#line 689 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_type_constraint; }
#line 2927 "bang.tab.cpp"
    break;

  case 102: // parameter-constraint-modifier: TYPENAME
#line 694 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::typename_constraint; }
#line 2933 "bang.tab.cpp"
    break;

  case 103: // parameter-constraint-modifier: "const modifier"
#line 696 "bang.y"
        { yylhs.value.as < parameter_constraint_modifier_t > () = parameter_constraint_modifier_t::value_constraint; }
#line 2939 "bang.tab.cpp"
    break;

  case 104: // parameter-constraint-set: parameter-matched-type
#line 701 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 2945 "bang.tab.cpp"
    break;

  case 105: // parameter-constraint-set: concept-expression
#line 703 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 2951 "bang.tab.cpp"
    break;

  case 106: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 707 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2957 "bang.tab.cpp"
    break;

  case 107: // parameter-matched-type: basic-parameter-matched-type
#line 711 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 2963 "bang.tab.cpp"
    break;

  case 108: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 713 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()) }; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2969 "bang.tab.cpp"
    break;

  case 109: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 715 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 2975 "bang.tab.cpp"
    break;

  case 110: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 717 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2981 "bang.tab.cpp"
    break;

  case 111: // basic-parameter-matched-type: "`_`"
#line 722 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2987 "bang.tab.cpp"
    break;

  case 112: // basic-parameter-matched-type: qname
#line 724 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 2993 "bang.tab.cpp"
    break;

  case 113: // basic-parameter-matched-type: qname "`(`" argument-list-opt "`)`"
#line 726 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 2999 "bang.tab.cpp"
    break;

  case 114: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 728 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3005 "bang.tab.cpp"
    break;

  case 115: // concept-expression: "`@`" qname
#line 751 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3011 "bang.tab.cpp"
    break;

  case 116: // syntax-expression: INTERNAL_IDENTIFIER
#line 756 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3017 "bang.tab.cpp"
    break;

  case 117: // syntax-expression: syntax-expression-wo-ii
#line 757 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3023 "bang.tab.cpp"
    break;

  case 118: // syntax-expression-wo-ii: "true"
#line 763 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3029 "bang.tab.cpp"
    break;

  case 119: // syntax-expression-wo-ii: "false"
#line 765 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3035 "bang.tab.cpp"
    break;

  case 120: // syntax-expression-wo-ii: INTEGER
#line 767 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3041 "bang.tab.cpp"
    break;

  case 121: // syntax-expression-wo-ii: DECIMAL
#line 769 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3047 "bang.tab.cpp"
    break;

  case 122: // syntax-expression-wo-ii: DECIMAL_S
#line 771 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3053 "bang.tab.cpp"
    break;

  case 123: // syntax-expression-wo-ii: INTEGER_INDEX
#line 773 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3059 "bang.tab.cpp"
    break;

  case 124: // syntax-expression-wo-ii: STRING
#line 775 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3065 "bang.tab.cpp"
    break;

  case 125: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 777 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3071 "bang.tab.cpp"
    break;

  case 126: // syntax-expression-wo-ii: qname
#line 779 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3077 "bang.tab.cpp"
    break;

  case 127: // syntax-expression-wo-ii: "`(`" argument-list "`)`"
#line 781 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = opt_named_syntax_expression_list_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) };
            }
        }
#line 3089 "bang.tab.cpp"
    break;

  case 128: // syntax-expression-wo-ii: "`.`" identifier
#line 789 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = context_identifier { std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3095 "bang.tab.cpp"
    break;

  case 129: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 793 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3101 "bang.tab.cpp"
    break;

  case 130: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 795 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3107 "bang.tab.cpp"
    break;

  case 131: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 797 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3113 "bang.tab.cpp"
    break;

  case 132: // syntax-expression-wo-ii: syntax-expression "`...`"
#line 799 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_parameter_pack_t{ std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 3119 "bang.tab.cpp"
    break;

  case 133: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 801 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3125 "bang.tab.cpp"
    break;

  case 134: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 807 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3131 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 809 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3137 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: apostrophe-expression
#line 811 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3143 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: new-expression
#line 812 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3149 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: compound-expression
#line 813 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3155 "bang.tab.cpp"
    break;

  case 139: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 818 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3161 "bang.tab.cpp"
    break;

  case 140: // new-expression: "`new`" qname
#line 823 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3167 "bang.tab.cpp"
    break;

  case 141: // new-expression: "`new`" apostrophe-expression
#line 825 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3173 "bang.tab.cpp"
    break;

  case 142: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 827 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3179 "bang.tab.cpp"
    break;

  case 143: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 829 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3185 "bang.tab.cpp"
    break;

  case 144: // call-expression: qname "`(`" argument-list-opt "`)`"
#line 834 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3191 "bang.tab.cpp"
    break;

  case 145: // call-expression: call-expression "`(`" argument-list-opt "`)`"
#line 836 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3197 "bang.tab.cpp"
    break;

  case 146: // call-expression: apostrophe-expression "`(`" argument-list-opt "`)`"
#line 838 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3203 "bang.tab.cpp"
    break;

  case 147: // compound-expression: call-expression
#line 842 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3209 "bang.tab.cpp"
    break;

  case 148: // type-expr: qname
#line 928 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3215 "bang.tab.cpp"
    break;

  case 149: // type-expr: INTERNAL_IDENTIFIER
#line 932 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3221 "bang.tab.cpp"
    break;

  case 150: // type-expr: "`[`" type-expr "`]`"
#line 936 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{std::move(yystack_[1].value.as < syntax_expression_t > ())}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3227 "bang.tab.cpp"
    break;

  case 151: // type-expr: "`(`" argument-list-opt "`)`"
#line 938 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_tuple_t{ std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3233 "bang.tab.cpp"
    break;

  case 152: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 940 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_array_t{std::move(yystack_[3].value.as < syntax_expression_t > ()), (size_t)yystack_[1].value.as < sonia::lang::bang::annotated_integer > ().value}; IGNORE(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3239 "bang.tab.cpp"
    break;

  case 153: // type-expr: type-expr "`|`" type-expr
#line 942 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3251 "bang.tab.cpp"
    break;

  case 154: // type-expr: type-expr "`->`" type-expr
#line 950 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ())}; }
#line 3257 "bang.tab.cpp"
    break;


#line 3261 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -163;

  const signed char parser::yytable_ninf_ = -56;

  const short
  parser::yypact_[] =
  {
     385,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,   211,
      33,   211,    33,   211,     8,   -23,    39,   211,   211,   211,
     211,    45,    22,  -163,    45,    45,  -163,  -163,    55,    21,
     291,  -163,  -163,    67,    90,  -163,  -163,  -163,   143,    32,
    -163,   174,  -163,    70,  -163,   122,     6,   176,  -163,  -163,
      87,   112,  -163,   211,   136,  -163,    17,    33,  -163,    33,
      16,  -163,   167,   131,   125,     7,   283,   136,   148,  -163,
    -163,   138,   227,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,   190,   211,    33,   211,    82,  -163,   211,   211,   211,
    -163,    33,   211,   211,   211,  -163,  -163,  -163,  -163,   211,
     136,  -163,    92,  -163,   154,   155,   211,   211,   573,   211,
    -163,   211,   130,   211,    53,    33,   164,    53,  -163,  -163,
     211,   190,   154,    75,   136,  -163,   123,   171,   211,  -163,
     136,    24,    24,  -163,    17,   194,   196,  -163,   190,    53,
     200,   202,  -163,  -163,    45,   160,   479,  -163,   212,  -163,
    -163,    32,   283,   283,   210,   136,   127,    45,  -163,  -163,
    -163,    27,   206,   213,   197,  -163,    68,   265,   247,  -163,
      89,  -163,  -163,   215,   201,  -163,    33,   222,   229,   188,
     190,   269,   190,  -163,   136,  -163,  -163,    75,   235,  -163,
    -163,   178,  -163,  -163,  -163,  -163,  -163,   210,  -163,  -163,
    -163,   239,  -163,   154,  -163,  -163,   241,   211,   267,    53,
     265,  -163,   255,   247,  -163,  -163,    25,  -163,    33,   245,
     246,   226,  -163,   124,  -163,  -163,  -163,   252,    88,   286,
      53,  -163,  -163,   259,   211,  -163,   247,  -163,  -163,  -163,
      68,  -163,    33,   263,  -163,  -163,   211,   266,  -163,   136,
      59,  -163,    33,   136,   210,  -163,   209,    14,   268,   211,
     211,    59,  -163,   270,   136,  -163,  -163
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   124,    50,   116,   125,   123,   120,   121,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,   118,   119,     0,     0,
      55,     5,    43,    23,    19,     7,    16,    53,   126,     0,
      14,     0,   117,   136,   137,   147,   138,     0,   138,    52,
      53,     0,    73,     0,    76,   128,   131,     0,    21,     0,
       0,    12,   140,   141,     0,     0,     0,    44,     0,    18,
      13,     0,     0,    17,     1,     2,    10,     3,     8,     6,
       9,     0,     0,     0,    71,     0,   130,     0,     0,     0,
     132,     0,     0,    71,    71,   139,    77,    78,   127,     0,
      75,    22,     0,    58,    57,     0,    71,    71,    25,     0,
      33,     0,    36,     0,    89,    62,     0,    89,    39,   149,
      71,     0,   148,    24,    20,    54,     0,    72,     0,    38,
      46,   134,   135,   129,   133,     0,     0,    74,     0,    89,
       0,     0,    49,    48,     0,    41,    42,    28,     0,    47,
      26,     0,     0,     0,     0,    69,   109,     0,   111,   102,
     103,    53,   112,     0,    90,    91,   101,     0,    95,   104,
     107,   105,    66,     0,    63,    64,    79,     0,     0,     0,
       0,     0,     0,   144,    15,   146,   145,    11,     0,   142,
     143,     0,    31,    29,    30,    27,    32,     0,    34,    35,
      37,   110,    98,   115,    51,    97,     0,    71,     0,     0,
       0,   100,   109,    94,   106,   108,     0,    61,     0,     0,
       0,    80,    81,     0,   151,   150,   154,     0,   153,    59,
      89,    99,    96,     0,     0,    92,    93,   110,   114,    65,
     101,    67,     0,     0,    40,   152,     0,     0,   113,    70,
       0,    82,    79,    60,     0,    87,    85,    83,     0,    71,
       0,     0,    68,     0,    84,    88,    86
  };

  const short
  parser::yypgoto_[] =
  {
    -163,  -163,  -163,  -163,   285,  -163,  -163,  -163,  -163,  -163,
     -56,   -25,  -163,   -40,   173,    36,  -163,    19,   -22,  -163,
     -36,  -163,  -163,  -163,   102,  -163,  -163,   -82,   310,   223,
    -163,    71,  -163,    83,    63,  -116,  -163,   117,  -163,    91,
    -162,  -152,  -163,   111,  -161,     0,  -163,   312,  -163,  -163,
       2,   -64
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    28,    29,    30,    31,    32,    33,    34,   145,   146,
     118,    35,   148,    36,   150,    37,   206,    38,    39,   105,
      61,    40,   173,   174,   175,    73,    69,   126,   127,    52,
      53,   220,   221,   222,   257,   163,   164,   165,   166,   210,
     167,   168,   169,   170,   171,    54,    42,    43,    44,    45,
      48,   123
  };

  const short
  parser::yytable_[] =
  {
      41,   177,    46,    85,   211,    80,   -45,   214,   110,    47,
     112,   135,   136,    56,     2,   213,    86,    64,    65,    66,
      67,    75,     2,   188,   140,   141,    86,    70,   260,   129,
      41,     2,    46,    86,   204,    62,    88,    89,   178,     2,
      68,    90,   158,    71,    72,     2,    49,    50,    55,    59,
      58,     2,   214,   100,     9,    74,   -45,   179,   236,     2,
     156,   -45,    10,    91,    92,     2,   255,    60,   149,   157,
     158,    10,   205,    91,   187,   214,    76,   261,   211,   104,
      91,    92,   124,   147,   111,    10,   151,   130,   131,   132,
     103,    10,   134,   101,   180,   102,   198,   199,   200,    10,
     122,    96,   128,    57,    82,    10,   149,   180,    41,   152,
      46,   153,    81,   155,   247,   197,   226,    93,   228,   125,
      50,   194,    60,   215,   151,   233,   181,   133,   184,    50,
      50,   108,    97,   162,    86,    50,   162,   138,   182,   181,
     122,   129,    50,    50,   243,    86,    41,   159,    46,   160,
     161,   172,   216,   161,    88,    89,    50,   122,   162,    90,
      98,   201,   159,   191,   160,    88,    89,   244,   113,    94,
      90,   183,   202,   108,   108,   161,   203,   263,   107,    99,
     109,    91,    92,    86,    83,    86,   162,   115,    87,    83,
      84,    95,    91,    92,    83,   114,     2,   119,   244,   122,
      83,   122,   139,    88,    89,    88,    89,   180,    90,   154,
      90,   176,   219,    83,   106,   192,     1,     2,     3,     4,
       5,     6,     7,     8,    83,   230,     9,   108,   162,   162,
      91,    92,    91,    92,   249,   162,    10,   120,    99,   181,
     225,   121,   185,    50,   186,   161,   253,   116,   189,   162,
     190,   182,    83,   207,   172,    83,   259,    10,    11,   108,
     264,   208,   196,   157,   209,   217,   161,    12,   218,   256,
     223,     2,   212,    83,   117,    13,   108,   224,   219,   227,
     256,   157,   158,   229,   231,    16,   232,   234,   219,   237,
     240,    77,    86,   242,   241,    50,     1,     2,     3,     4,
       5,     6,     7,     8,   245,   246,     9,   248,    26,    27,
     252,    10,    88,    89,   254,    79,   262,    90,   266,   195,
     239,    51,   137,   258,   265,   251,   235,   238,    63,     0,
       0,   250,   108,     0,     0,     0,     0,    10,    11,    91,
      92,     0,     0,     0,     0,     0,    78,    12,     0,     0,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
       0,     0,    14,     0,    15,    16,    17,    18,     0,    19,
       0,     0,     0,    20,     0,    21,     0,     0,     0,    22,
      23,     0,    24,    25,     0,     0,     0,     0,    26,    27,
       1,     2,     3,     4,     5,     6,     7,     8,     0,     0,
       9,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,    11,     0,     0,     0,     0,     0,     0,     0,
       0,    12,     0,     0,     0,     0,     0,     0,     0,    13,
       0,     0,     0,     0,     0,     0,    14,     0,    15,    16,
      17,    18,     0,    19,     0,     0,     0,    20,     0,    21,
       0,     0,     0,    22,    23,   -55,    24,    25,     0,     0,
       0,     0,    26,    27,     1,     2,     3,     4,     5,     6,
       7,     8,     0,     0,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,    11,     0,     0,     0,
       0,     0,     0,     0,   193,    12,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,     0,     0,     0,
      14,     0,     0,    16,    17,    18,     0,    19,     0,   142,
     143,    20,     0,     0,     0,     0,     0,     0,    23,   -55,
       0,   144,     0,     0,     0,     0,    26,    27,     1,     2,
       3,     4,     5,     6,     7,     8,     0,     0,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,     0,     0,     0,     0,     0,     0,     0,    12,
       0,     0,     0,     0,     0,     0,     0,    13,     0,     0,
       0,     0,     0,     0,    14,     0,     0,    16,    17,    18,
       0,    19,     0,   142,   143,    20,     0,     0,     0,     0,
       0,     0,    23,   -55,     0,   144,     0,     0,     0,     0,
      26,    27
  };

  const short
  parser::yycheck_[] =
  {
       0,   117,     0,    39,   166,    30,     0,   168,    64,     9,
      66,    93,    94,    13,     6,   167,     9,    17,    18,    19,
      20,     0,     6,   139,   106,   107,     9,     5,    14,    85,
      30,     6,    30,     9,     7,    16,    29,    30,   120,     6,
      21,    34,    17,    24,    25,     6,    10,    11,    12,    72,
      14,     6,   213,    53,    15,     0,    50,   121,   210,     6,
       7,    55,    46,    56,    57,     6,     7,    90,   108,    16,
      17,    46,    45,    56,   138,   236,    55,    63,   240,    60,
      56,    57,    82,   108,    77,    46,   108,    87,    88,    89,
      74,    46,    92,    57,    19,    59,   152,   153,   154,    46,
      81,    14,    20,    95,    14,    46,   146,    19,   108,   109,
     108,   111,    45,   113,   230,   151,   180,    47,   182,    83,
      84,   146,    90,    34,   146,   207,    51,    91,   128,    93,
      94,    49,    45,   114,     9,    99,   117,    45,    63,    51,
     121,   197,   106,   107,    20,     9,   146,    94,   146,    96,
     114,   115,    63,   117,    29,    30,   120,   138,   139,    34,
      48,    34,    94,   144,    96,    29,    30,   223,    20,    47,
      34,    48,    45,    49,    49,   139,   157,   259,    47,    67,
      55,    56,    57,     9,    46,     9,   167,    49,    14,    46,
      47,    15,    56,    57,    46,    47,     6,     7,   254,   180,
      46,   182,    47,    29,    30,    29,    30,    19,    34,    79,
      34,    47,   176,    46,    47,    55,     5,     6,     7,     8,
       9,    10,    11,    12,    46,    47,    15,    49,   209,   210,
      56,    57,    56,    57,   234,   216,    46,    47,    67,    51,
      52,    51,    48,   207,    48,   209,   246,    20,    48,   230,
      48,    63,    46,    47,   218,    46,    47,    46,    47,    49,
     260,    48,    50,    16,    67,    50,   230,    56,    67,   250,
      48,     6,     7,    46,    47,    64,    49,    48,   242,    10,
     261,    16,    17,    48,    45,    74,    45,    20,   252,    34,
      45,     0,     9,    67,    48,   259,     5,     6,     7,     8,
       9,    10,    11,    12,    52,    19,    15,    48,    97,    98,
      47,    46,    29,    30,    48,    30,    48,    34,    48,   146,
     218,    11,    99,   252,   261,   242,   209,   216,    16,    -1,
      -1,   240,    49,    -1,    -1,    -1,    -1,    46,    47,    56,
      57,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    74,    75,    76,    -1,    78,
      -1,    -1,    -1,    82,    -1,    84,    -1,    -1,    -1,    88,
      89,    -1,    91,    92,    -1,    -1,    -1,    -1,    97,    98,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    74,
      75,    76,    -1,    78,    -1,    -1,    -1,    82,    -1,    84,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    97,    98,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    74,    75,    76,    -1,    78,    -1,    80,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      -1,    92,    -1,    -1,    -1,    -1,    97,    98,     5,     6,
       7,     8,     9,    10,    11,    12,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    74,    75,    76,
      -1,    78,    -1,    80,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    92,    -1,    -1,    -1,    -1,
      97,    98
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    56,    64,    71,    73,    74,    75,    76,    78,
      82,    84,    88,    89,    91,    92,    97,    98,   100,   101,
     102,   103,   104,   105,   106,   110,   112,   114,   116,   117,
     120,   144,   145,   146,   147,   148,   149,   144,   149,   114,
     114,   127,   128,   129,   144,   114,   144,    95,   114,    72,
      90,   119,   116,   146,   144,   144,   144,   144,   116,   125,
       5,   116,   116,   124,     0,     0,    55,     0,    55,   103,
     110,    45,    14,    46,    47,   119,     9,    14,    29,    30,
      34,    56,    57,    47,    47,    15,    14,    45,    48,    67,
     144,   114,   114,    74,   116,   118,    47,    47,    49,    55,
     109,    77,   109,    20,    47,    49,    20,    47,   109,     7,
      47,    51,   116,   150,   144,   114,   126,   127,    20,   109,
     144,   144,   144,   114,   144,   126,   126,   128,    45,    47,
     126,   126,    80,    81,    92,   107,   108,   110,   111,   112,
     113,   117,   144,   144,    79,   144,     7,    16,    17,    94,
      96,   114,   116,   134,   135,   136,   137,   139,   140,   141,
     142,   143,   114,   121,   122,   123,    47,   134,   126,   150,
      19,    51,    63,    48,   144,    48,    48,   150,   134,    48,
      48,   116,    55,    55,   110,   113,    50,   119,   109,   109,
     109,    34,    45,   116,     7,    45,   115,    47,    48,    67,
     138,   139,     7,   140,   143,    34,    63,    50,    67,   114,
     130,   131,   132,    48,    48,    52,   150,    10,   150,    48,
      47,    45,    45,   126,    20,   136,   140,    34,   142,   123,
      45,    48,    67,    20,   109,    52,    19,   134,    48,   144,
     138,   132,    47,   144,    48,     7,   116,   133,   130,    47,
      14,    63,    48,   126,   144,   133,    48
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
     147,   147,   147,   147,   148,   148,   148,   149,   150,   150,
     150,   150,   150,   150,   150
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     2,     2,     1,     4,     1,     2,     2,     1,
       3,     2,     3,     1,     3,     0,     1,     2,     1,     2,
       2,     2,     3,     3,     5,     5,     3,     5,     3,     3,
       6,     1,     1,     1,     2,     1,     3,     1,     1,     1,
       1,     1,     2,     1,     3,     0,     1,     1,     1,     5,
       7,     5,     0,     1,     1,     3,     1,     5,     8,     3,
       6,     0,     1,     1,     3,     2,     1,     2,     2,     0,
       1,     1,     3,     4,     6,     1,     4,     1,     3,     0,
       1,     1,     3,     3,     2,     1,     3,     2,     2,     3,
       1,     0,     1,     1,     1,     1,     2,     1,     2,     1,
       2,     1,     1,     4,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     3,
       2,     2,     2,     3,     3,     3,     1,     1,     1,     3,
       2,     2,     5,     5,     4,     4,     4,     1,     1,     1,
       3,     3,     4,     3,     3
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
  "compound-expression", "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   331,   331,   332,   336,   338,   340,   357,   359,   360,
     362,   367,   369,   371,   373,   378,   380,   382,   384,   389,
     391,   396,   398,   403,   404,   409,   411,   413,   418,   419,
     420,   422,   426,   431,   433,   435,   437,   439,   441,   443,
     445,   450,   450,   453,   455,   457,   459,   464,   466,   468,
     473,   481,   496,   498,   500,   506,   508,   513,   514,   518,
     520,   526,   533,   535,   539,   542,   547,   553,   555,   562,
     564,   595,   596,   600,   602,   607,   609,   614,   616,   622,
     623,   627,   629,   634,   636,   641,   643,   645,   647,   653,
     654,   658,   660,   665,   667,   669,   674,   676,   678,   682,
     687,   688,   693,   695,   700,   702,   706,   711,   712,   714,
     716,   721,   723,   725,   727,   750,   755,   757,   762,   764,
     766,   768,   770,   772,   774,   776,   778,   780,   788,   792,
     794,   796,   798,   800,   806,   808,   811,   812,   813,   817,
     822,   824,   826,   828,   833,   835,   837,   842,   927,   931,
     935,   937,   939,   941,   949
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
#line 4076 "bang.tab.cpp"

#line 1332 "bang.y"

