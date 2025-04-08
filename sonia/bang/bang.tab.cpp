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

      case symbol_kind::S_NIL_WORD: // "nil"
        value.copy< annotated_nil > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_157_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
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
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_158_compound_expression: // compound-expression
      case symbol_kind::S_159_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
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

      case symbol_kind::S_NIL_WORD: // "nil"
        value.move< annotated_nil > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_157_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
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
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_158_compound_expression: // compound-expression
      case symbol_kind::S_159_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
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

      case symbol_kind::S_NIL_WORD: // "nil"
        value.YY_MOVE_OR_COPY< annotated_nil > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_157_pack_expression: // pack-expression
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
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
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.YY_MOVE_OR_COPY< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_158_compound_expression: // compound-expression
      case symbol_kind::S_159_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
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

      case symbol_kind::S_NIL_WORD: // "nil"
        value.move< annotated_nil > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_157_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_case_decl: // case-decl
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
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_158_compound_expression: // compound-expression
      case symbol_kind::S_159_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_using_decl: // using-decl
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

      case symbol_kind::S_NIL_WORD: // "nil"
        value.copy< annotated_nil > (that.value);
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_157_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_128_case_decl: // case-decl
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
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_158_compound_expression: // compound-expression
      case symbol_kind::S_159_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_130_using_decl: // using-decl
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

      case symbol_kind::S_NIL_WORD: // "nil"
        value.move< annotated_nil > (that.value);
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_157_pack_expression: // pack-expression
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_128_case_decl: // case-decl
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
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_158_compound_expression: // compound-expression
      case symbol_kind::S_159_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_130_using_decl: // using-decl
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
#line 1512 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1518 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1524 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 325 "bang.y"
                 { }
#line 1530 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 325 "bang.y"
                 { }
#line 1536 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 325 "bang.y"
                 { }
#line 1542 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 325 "bang.y"
                 { }
#line 1548 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 325 "bang.y"
                 { }
#line 1554 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 325 "bang.y"
                 { }
#line 1560 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 325 "bang.y"
                 { }
#line 1566 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 325 "bang.y"
                 { }
#line 1572 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 325 "bang.y"
                 { }
#line 1578 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 325 "bang.y"
                 { }
#line 1584 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 325 "bang.y"
                 { }
#line 1590 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 325 "bang.y"
                 { }
#line 1596 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 325 "bang.y"
                 { }
#line 1602 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 325 "bang.y"
                 { }
#line 1608 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 325 "bang.y"
                 { }
#line 1614 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 325 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 325 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 325 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 325 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 325 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 325 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 325 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 325 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 325 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 325 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 325 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 325 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 325 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 325 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 325 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 325 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 325 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 325 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
#line 325 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 325 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl: // let-decl
#line 325 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start: // let-decl-start
#line 325 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 325 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
#line 325 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_braced_statements: // braced-statements
#line 325 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
#line 325 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
#line 325 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
#line 325 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_118_infunction_statement: // infunction-statement
#line 325 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 325 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_120_internal_identifier: // internal-identifier
#line 325 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 325 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
#line 325 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_name: // fn-name
#line 325 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
#line 325 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
#line 325 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
#line 325 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_list: // case-list
#line 325 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_case_decl: // case-decl
#line 325 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
#line 325 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_using_decl: // using-decl
#line 325 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_expression_list: // expression-list
#line 325 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
#line 325 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_133_argument_list: // argument-list
#line 325 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 325 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_argument_name: // argument-name
#line 325 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
#line 325 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_list: // field-list
#line 325 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
#line 325 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 325 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
#line 325 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_parameter_list: // parameter-list
#line 325 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
#line 325 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
#line 325 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
#line 325 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
#line 325 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
#line 325 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
#line 325 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
#line 325 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
#line 325 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
#line 325 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_syntax_expression: // syntax-expression
#line 325 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 325 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
#line 325 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_new_expression: // new-expression
#line 325 "bang.y"
                 { }
#line 2010 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_call_expression: // call-expression
#line 325 "bang.y"
                 { }
#line 2016 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_lambda_expression: // lambda-expression
#line 325 "bang.y"
                 { }
#line 2022 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_pack_expression: // pack-expression
#line 325 "bang.y"
                 { }
#line 2028 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_compound_expression: // compound-expression
#line 325 "bang.y"
                 { }
#line 2034 "bang.tab.cpp"
        break;

      case symbol_kind::S_159_type_expr: // type-expr
#line 325 "bang.y"
                 { }
#line 2040 "bang.tab.cpp"
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

      case symbol_kind::S_NIL_WORD: // "nil"
        yylhs.value.emplace< annotated_nil > ();
        break;

      case symbol_kind::S_125_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_131_expression_list: // expression-list
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_136_field_list_opt: // field-list-opt
      case symbol_kind::S_137_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_115_finished_statement: // finished-statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_122_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_124_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_117_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_109_let_decl: // let-decl
      case symbol_kind::S_110_let_decl_start: // let-decl-start
      case symbol_kind::S_111_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_107_finished_statement_any: // finished-statement-any
      case symbol_kind::S_112_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_113_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_114_braced_statements: // braced-statements
      case symbol_kind::S_116_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< managed_statement_list > ();
        break;

      case symbol_kind::S_132_argument_list_opt: // argument-list-opt
      case symbol_kind::S_133_argument_list: // argument-list
      case symbol_kind::S_157_pack_expression: // pack-expression
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_145_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_146_parameter_value_constraint_set: // parameter-value-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_140_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_141_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_144_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_120_internal_identifier: // internal-identifier
      case symbol_kind::S_135_argument_name: // argument-name
      case symbol_kind::S_142_internal_identifier_opt: // internal-identifier-opt
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_123_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_128_case_decl: // case-decl
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
      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
      case symbol_kind::S_POINT: // "`.`"
      case symbol_kind::S_PLUS: // "`+`"
      case symbol_kind::S_MINUS: // "`-`"
      case symbol_kind::S_ASTERISK: // "`*`"
      case symbol_kind::S_AMPERSAND: // "`&`"
      case symbol_kind::S_BITOR: // "`|`"
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_NEW: // "`new`"
      case symbol_kind::S_CONTINUE: // "`continue`"
      case symbol_kind::S_BREAK: // "`break`"
      case symbol_kind::S_FN: // "`fn`"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_138_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_143_parameter_default_value_opt: // parameter-default-value-opt
        yylhs.value.emplace< sonia::optional<syntax_expression_t> > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_118_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_126_case_list_opt: // case-list-opt
      case symbol_kind::S_127_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_129_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_147_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_148_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_149_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_151_syntax_expression: // syntax-expression
      case symbol_kind::S_152_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_153_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_154_new_expression: // new-expression
      case symbol_kind::S_155_call_expression: // call-expression
      case symbol_kind::S_156_lambda_expression: // lambda-expression
      case symbol_kind::S_158_compound_expression: // compound-expression
      case symbol_kind::S_159_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_130_using_decl: // using-decl
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
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2474 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 331 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2480 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 336 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2486 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 338 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2492 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 340 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2498 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 357 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2504 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 358 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2510 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 360 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2516 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 362 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2522 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 367 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2528 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 369 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2534 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 371 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2540 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-decl "`=>`" syntax-expression
#line 377 "bang.y"
        {
            yystack_[2].value.as < fn_pure_t > ().kind = yystack_[3].value.as < fn_kind > ();
            auto sts = ctx.new_statement_list();
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) });
            yylhs.value.as < statement > () = fn_decl_t{ std::move(yystack_[2].value.as < fn_pure_t > ()), ctx.push(std::move(sts)) };
        }
#line 2551 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 384 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2557 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 386 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2563 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 391 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2569 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 393 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2575 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: LET identifier
#line 398 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2581 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: LET "weak modifier" identifier
#line 400 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2587 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 404 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2593 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 406 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2599 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 411 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2605 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 413 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2611 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 415 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2617 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 420 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2623 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 421 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2629 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 423 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2635 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 425 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2641 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 430 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2647 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 435 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2653 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 437 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2659 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 439 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2665 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 441 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2671 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 443 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2677 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 445 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2683 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 447 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2689 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 449 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2695 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 451 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2701 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 456 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2707 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 458 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2713 "bang.tab.cpp"
    break;

  case 42: // generic-statement: let-decl
#line 463 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2719 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 465 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2725 "bang.tab.cpp"
    break;

  case 44: // generic-statement: "`return`" syntax-expression
#line 467 "bang.y"
        { yylhs.value.as < generic_statement_type > () = return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2731 "bang.tab.cpp"
    break;

  case 45: // generic-statement: compound-expression
#line 469 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2737 "bang.tab.cpp"
    break;

  case 46: // generic-statement: syntax-expression "`=`" syntax-expression
#line 471 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2743 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: generic-statement
#line 476 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2749 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`break`"
#line 478 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2755 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`continue`"
#line 480 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2761 "bang.tab.cpp"
    break;

  case 50: // identifier: IDENTIFIER
#line 485 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2767 "bang.tab.cpp"
    break;

  case 51: // internal-identifier: INTERNAL_IDENTIFIER
#line 493 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2773 "bang.tab.cpp"
    break;

  case 52: // qname: "`::`" identifier
#line 508 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2779 "bang.tab.cpp"
    break;

  case 53: // qname: identifier
#line 510 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2785 "bang.tab.cpp"
    break;

  case 54: // qname: qname "`::`" identifier
#line 512 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2791 "bang.tab.cpp"
    break;

  case 55: // fn-start-decl: "`fn`"
#line 518 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2797 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: INLINE "`fn`"
#line 520 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2803 "bang.tab.cpp"
    break;

  case 57: // fn-name: qname
#line 524 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2809 "bang.tab.cpp"
    break;

  case 58: // fn-name: "`new`"
#line 525 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2815 "bang.tab.cpp"
    break;

  case 59: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 530 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < parameter_list_t > ()), nullopt }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2821 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" type-expr
#line 532 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2827 "bang.tab.cpp"
    break;

  case 61: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 538 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2833 "bang.tab.cpp"
    break;

  case 62: // case-list-opt: %empty
#line 542 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2839 "bang.tab.cpp"
    break;

  case 63: // case-list-opt: case-list
#line 543 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2845 "bang.tab.cpp"
    break;

  case 64: // case-list: case-decl
#line 548 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2851 "bang.tab.cpp"
    break;

  case 65: // case-list: case-list "," case-decl
#line 550 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2857 "bang.tab.cpp"
    break;

  case 66: // case-decl: identifier
#line 555 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2863 "bang.tab.cpp"
    break;

  case 67: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 561 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2869 "bang.tab.cpp"
    break;

  case 68: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 563 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2875 "bang.tab.cpp"
    break;

  case 69: // using-decl: qname "`=>`" syntax-expression
#line 586 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 2883 "bang.tab.cpp"
    break;

  case 70: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 590 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 2891 "bang.tab.cpp"
    break;

  case 71: // expression-list: syntax-expression
#line 604 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2897 "bang.tab.cpp"
    break;

  case 72: // expression-list: expression-list "," syntax-expression
#line 606 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2903 "bang.tab.cpp"
    break;

  case 73: // argument-list-opt: %empty
#line 611 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2909 "bang.tab.cpp"
    break;

  case 74: // argument-list-opt: argument-list
#line 612 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2915 "bang.tab.cpp"
    break;

  case 75: // argument-list: argument
#line 617 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2921 "bang.tab.cpp"
    break;

  case 76: // argument-list: argument-list "," argument
#line 619 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2927 "bang.tab.cpp"
    break;

  case 77: // argument: argument-name syntax-expression
#line 624 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2933 "bang.tab.cpp"
    break;

  case 78: // argument: syntax-expression
#line 626 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2939 "bang.tab.cpp"
    break;

  case 79: // argument-name: identifier "`=`"
#line 631 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2945 "bang.tab.cpp"
    break;

  case 80: // argument-name: identifier "`:`"
#line 633 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2951 "bang.tab.cpp"
    break;

  case 81: // field-list-opt: %empty
#line 638 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2957 "bang.tab.cpp"
    break;

  case 82: // field-list-opt: field-list
#line 639 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 2963 "bang.tab.cpp"
    break;

  case 83: // field-list: field
#line 644 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 2969 "bang.tab.cpp"
    break;

  case 84: // field-list: field-list "," field
#line 646 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 2975 "bang.tab.cpp"
    break;

  case 85: // field-default-value-opt: %empty
#line 650 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 2981 "bang.tab.cpp"
    break;

  case 86: // field-default-value-opt: "`=`" syntax-expression
#line 651 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2987 "bang.tab.cpp"
    break;

  case 87: // field: identifier "`:`" type-expr field-default-value-opt
#line 656 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2993 "bang.tab.cpp"
    break;

  case 88: // field: identifier "`=>`" type-expr field-default-value-opt
#line 658 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 2999 "bang.tab.cpp"
    break;

  case 89: // parameter-list-opt: %empty
#line 668 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3005 "bang.tab.cpp"
    break;

  case 90: // parameter-list-opt: parameter-list
#line 669 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3011 "bang.tab.cpp"
    break;

  case 91: // parameter-list: parameter-decl
#line 674 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3017 "bang.tab.cpp"
    break;

  case 92: // parameter-list: parameter-list "," parameter-decl
#line 676 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3023 "bang.tab.cpp"
    break;

  case 93: // internal-identifier-opt: %empty
#line 680 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3029 "bang.tab.cpp"
    break;

  case 94: // internal-identifier-opt: internal-identifier
#line 681 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3035 "bang.tab.cpp"
    break;

  case 95: // parameter-default-value-opt: %empty
#line 685 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3041 "bang.tab.cpp"
    break;

  case 96: // parameter-default-value-opt: "`=`" syntax-expression
#line 686 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3047 "bang.tab.cpp"
    break;

  case 97: // parameter-decl: identifier internal-identifier-opt "`:`" parameter-constraint-set parameter-default-value-opt
#line 691 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3053 "bang.tab.cpp"
    break;

  case 98: // parameter-decl: identifier internal-identifier-opt "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 693 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3059 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 695 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3065 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: identifier internal-identifier-opt "`=>`" parameter-value-constraint-set
#line 697 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3071 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: INTERNAL_IDENTIFIER "`:`" parameter-constraint-set parameter-default-value-opt
#line 700 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3077 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 702 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3083 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 704 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3089 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: INTERNAL_IDENTIFIER "`=>`" parameter-value-constraint-set
#line 706 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3095 "bang.tab.cpp"
    break;

  case 105: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" parameter-constraint-set
#line 709 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3101 "bang.tab.cpp"
    break;

  case 106: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "const modifier" parameter-constraint-set
#line 711 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3107 "bang.tab.cpp"
    break;

  case 107: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "mut modifier" parameter-constraint-set
#line 713 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3113 "bang.tab.cpp"
    break;

  case 108: // parameter-decl: parameter-constraint-set parameter-default-value-opt
#line 716 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3119 "bang.tab.cpp"
    break;

  case 109: // parameter-decl: "const modifier" parameter-constraint-set parameter-default-value-opt
#line 718 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3125 "bang.tab.cpp"
    break;

  case 110: // parameter-decl: "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 720 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3131 "bang.tab.cpp"
    break;

  case 111: // parameter-decl: "`=>`" parameter-value-constraint-set
#line 722 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3137 "bang.tab.cpp"
    break;

  case 112: // parameter-constraint-set: parameter-matched-type
#line 727 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3143 "bang.tab.cpp"
    break;

  case 113: // parameter-constraint-set: concept-expression
#line 729 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3149 "bang.tab.cpp"
    break;

  case 114: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 731 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3155 "bang.tab.cpp"
    break;

  case 115: // parameter-value-constraint-set: parameter-matched-value
#line 736 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()), {}, {} }; }
#line 3161 "bang.tab.cpp"
    break;

  case 116: // parameter-value-constraint-set: concept-expression
#line 738 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ nullopt, {std::move(yystack_[0].value.as < syntax_expression_t > ())}, {} }; }
#line 3167 "bang.tab.cpp"
    break;

  case 117: // parameter-value-constraint-set: parameter-value-constraint-set concept-expression
#line 740 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3173 "bang.tab.cpp"
    break;

  case 118: // parameter-matched-value: syntax-expression
#line 759 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3179 "bang.tab.cpp"
    break;

  case 119: // parameter-matched-type: basic-parameter-matched-type
#line 763 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3185 "bang.tab.cpp"
    break;

  case 120: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 765 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3191 "bang.tab.cpp"
    break;

  case 121: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 767 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3197 "bang.tab.cpp"
    break;

  case 122: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 769 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, variable_identifier{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3203 "bang.tab.cpp"
    break;

  case 123: // basic-parameter-matched-type: "`_`"
#line 775 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3209 "bang.tab.cpp"
    break;

  case 124: // basic-parameter-matched-type: qname
#line 781 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3215 "bang.tab.cpp"
    break;

  case 125: // basic-parameter-matched-type: qname "`(`" pack-expression "`)`"
#line 784 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3221 "bang.tab.cpp"
    break;

  case 126: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 786 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3227 "bang.tab.cpp"
    break;

  case 127: // concept-expression: "`@`" qname
#line 809 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_identifier{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3233 "bang.tab.cpp"
    break;

  case 128: // syntax-expression: INTERNAL_IDENTIFIER
#line 814 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3239 "bang.tab.cpp"
    break;

  case 129: // syntax-expression: syntax-expression-wo-ii
#line 815 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3245 "bang.tab.cpp"
    break;

  case 130: // syntax-expression-wo-ii: "nil"
#line 821 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3251 "bang.tab.cpp"
    break;

  case 131: // syntax-expression-wo-ii: "true"
#line 823 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3257 "bang.tab.cpp"
    break;

  case 132: // syntax-expression-wo-ii: "false"
#line 825 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3263 "bang.tab.cpp"
    break;

  case 133: // syntax-expression-wo-ii: INTEGER
#line 827 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3269 "bang.tab.cpp"
    break;

  case 134: // syntax-expression-wo-ii: DECIMAL
#line 829 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3275 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: DECIMAL_S
#line 831 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3281 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: INTEGER_INDEX
#line 833 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3287 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: STRING
#line 835 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3293 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 837 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3299 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: qname
#line 839 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3305 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 841 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3317 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 849 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3329 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 857 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3335 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 859 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3341 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: "`.`" identifier
#line 861 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3347 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 865 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3353 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 867 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3359 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 869 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3365 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 872 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3371 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 874 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3377 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 879 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3383 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 881 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3389 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 885 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3395 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 887 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3401 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 891 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3407 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 895 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3413 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: apostrophe-expression
#line 897 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3419 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: new-expression
#line 898 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3425 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: compound-expression
#line 899 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3431 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: lambda-expression
#line 900 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3437 "bang.tab.cpp"
    break;

  case 160: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 905 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3443 "bang.tab.cpp"
    break;

  case 161: // new-expression: "`new`" qname
#line 910 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3449 "bang.tab.cpp"
    break;

  case 162: // new-expression: "`new`" apostrophe-expression
#line 912 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3455 "bang.tab.cpp"
    break;

  case 163: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 914 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_identifier{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3461 "bang.tab.cpp"
    break;

  case 164: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 916 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3467 "bang.tab.cpp"
    break;

  case 165: // call-expression: qname "`(`" pack-expression "`)`"
#line 921 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3473 "bang.tab.cpp"
    break;

  case 166: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 923 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3479 "bang.tab.cpp"
    break;

  case 167: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 925 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3485 "bang.tab.cpp"
    break;

  case 168: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 927 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3491 "bang.tab.cpp"
    break;

  case 169: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 932 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3497 "bang.tab.cpp"
    break;

  case 170: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 934 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3503 "bang.tab.cpp"
    break;

  case 171: // pack-expression: syntax-expression
#line 939 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3513 "bang.tab.cpp"
    break;

  case 172: // pack-expression: identifier "`:`" syntax-expression
#line 945 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3523 "bang.tab.cpp"
    break;

  case 173: // pack-expression: pack-expression "," syntax-expression
#line 951 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3532 "bang.tab.cpp"
    break;

  case 174: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 956 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3541 "bang.tab.cpp"
    break;

  case 175: // compound-expression: syntax-expression "`...`"
#line 964 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3547 "bang.tab.cpp"
    break;

  case 176: // compound-expression: call-expression
#line 965 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3553 "bang.tab.cpp"
    break;

  case 177: // type-expr: qname
#line 1052 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3559 "bang.tab.cpp"
    break;

  case 178: // type-expr: call-expression
#line 1053 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3565 "bang.tab.cpp"
    break;

  case 179: // type-expr: INTERNAL_IDENTIFIER
#line 1055 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_identifier{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3571 "bang.tab.cpp"
    break;

  case 180: // type-expr: "`[`" type-expr "`]`"
#line 1059 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3577 "bang.tab.cpp"
    break;

  case 181: // type-expr: "`(`" "`)`"
#line 1061 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{} }; }
#line 3583 "bang.tab.cpp"
    break;

  case 182: // type-expr: "`(`" pack-expression "`)`"
#line 1063 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3589 "bang.tab.cpp"
    break;

  case 183: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1065 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3595 "bang.tab.cpp"
    break;

  case 184: // type-expr: type-expr "`|`" type-expr
#line 1067 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3607 "bang.tab.cpp"
    break;

  case 185: // type-expr: type-expr "`->`" type-expr
#line 1075 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 3616 "bang.tab.cpp"
    break;


#line 3620 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -202;

  const signed char parser::yytable_ninf_ = -94;

  const short
  parser::yypact_[] =
  {
     594,  -202,  -202,  -202,  -202,  -202,  -202,  -202,  -202,   892,
      26,   892,   892,   892,    26,   892,   892,     1,   170,    37,
     892,   892,   892,   892,    24,    52,   -26,  -202,    24,    24,
    -202,  -202,  -202,    71,    19,   396,  -202,  -202,    60,   122,
    -202,  -202,  -202,   249,     8,   697,  -202,   109,  -202,   146,
     164,    18,   220,   797,  -202,  -202,   124,   969,   116,   231,
     969,   184,  -202,   969,    40,    26,  -202,    26,    -2,   260,
     234,   329,   584,   912,   969,   162,  -202,  -202,  -202,   241,
    -202,   157,  -202,  -202,  -202,  -202,  -202,  -202,  -202,  -202,
     471,   892,    26,   892,   335,  -202,   183,   250,    62,  -202,
     892,   892,   892,  -202,   892,    54,   892,   892,   892,   892,
     892,   892,   892,  -202,   892,  -202,   892,  -202,   892,  -202,
    -202,   197,  -202,   892,   892,   693,   892,  -202,   892,   227,
     892,   335,    26,   254,   335,  -202,  -202,   792,   471,   249,
     109,   146,   164,    15,   247,  -202,   143,   181,    24,  -202,
     813,   182,   182,    33,   272,   274,   255,  -202,   242,  -202,
      22,  -202,   335,   892,  -202,   969,   172,   172,   922,  -202,
    -202,    40,    40,   507,    83,   185,   195,   199,   969,   278,
     969,   969,   471,    72,   280,   257,  -202,   892,   969,   281,
    -202,  -202,    24,   273,   495,  -202,   282,  -202,  -202,     8,
     912,   912,   286,   969,   289,  -202,   290,   275,  -202,    26,
     299,  -202,   209,    12,   471,   338,   471,  -202,   813,   304,
     393,   183,   344,  -202,  -202,   969,   320,   242,   242,  -202,
    -202,   152,   892,    39,   335,   892,  -202,  -202,  -202,    44,
     313,   969,  -202,  -202,  -202,  -202,   892,    15,  -202,  -202,
    -202,   892,   969,  -202,   268,  -202,  -202,  -202,  -202,  -202,
     286,  -202,  -202,  -202,   346,  -202,    26,   194,   321,   301,
    -202,   125,  -202,  -202,  -202,   316,    43,   344,   409,   182,
     182,   242,  -202,  -202,  -202,  -202,   813,   508,   224,   471,
    -202,  -202,   969,  -202,   353,   969,  -202,   335,   892,  -202,
     471,   471,  -202,    26,   326,  -202,  -202,   182,   182,   344,
     242,   242,  -202,   344,   182,   182,   242,  -202,   127,   471,
     327,   969,    14,    14,  -202,    26,   344,   344,  -202,  -202,
     242,   242,  -202,  -202,    15,   286,   892,  -202,  -202,   331,
    -202,  -202,   969,  -202
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   137,    50,   128,   138,   136,   133,   134,   135,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
     130,   131,   132,     0,     0,     0,     5,    42,    21,    17,
       7,    15,    53,   139,     0,     0,   129,   156,   157,   176,
     159,   158,     0,     0,   158,    52,    53,   171,     0,     0,
      71,     0,   144,   149,   148,     0,    19,     0,     0,   161,
     162,     0,     0,     0,    44,     0,    43,    13,    56,     0,
      39,     0,    16,     1,     2,    10,     3,     8,     6,     9,
       0,     0,     0,     0,    89,    58,    57,     0,     0,   147,
       0,     0,     0,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   160,     0,   140,     0,   141,     0,   142,
      20,     0,    12,    73,    73,    23,     0,    31,     0,    34,
       0,    89,    62,     0,    89,    37,   179,     0,     0,   177,
       0,   178,     0,    22,    18,    54,     0,   121,     0,   123,
       0,     0,     0,    53,   124,     0,    90,    91,    95,   112,
     119,   113,    89,     0,    36,    46,   152,   153,     0,   145,
     146,   150,   151,   154,   155,     0,     0,     0,   172,    53,
     173,    72,     0,    53,     0,    74,    75,     0,    78,     0,
      49,    48,     0,    40,    41,    26,     0,    47,    24,     0,
       0,     0,     0,    69,     0,    66,     0,    63,    64,    81,
       0,   181,     0,     0,     0,     0,     0,   165,     0,   122,
       0,   127,   111,   115,   116,   118,   121,    95,    95,    51,
      94,     0,     0,     0,     0,     0,   108,   114,   120,     0,
       0,    14,   143,   167,   166,   168,     0,    11,    79,    80,
     163,     0,    77,   164,     0,    29,    27,    28,    25,    30,
       0,    32,    33,    35,     0,    61,     0,     0,     0,    82,
      83,     0,   182,   180,   185,     0,   184,   104,     0,     0,
       0,    95,   117,   122,   109,   110,     0,     0,     0,     0,
     169,    92,    96,   126,    59,   174,    76,    89,     0,    65,
       0,     0,    67,     0,     0,    38,   183,     0,     0,   105,
      95,    95,   101,   100,     0,     0,    95,   125,     0,     0,
       0,    70,    85,    85,    84,    81,   106,   107,   102,   103,
      95,    95,    97,   170,    60,     0,     0,    88,    87,     0,
      98,    99,    86,    68
  };

  const short
  parser::yypgoto_[] =
  {
    -202,  -202,  -202,  -202,   339,  -202,  -202,  -202,  -202,  -202,
     -47,   -10,  -202,  -122,   188,    28,  -202,   151,     2,  -202,
     -67,  -202,  -202,  -202,   117,  -202,  -202,   371,   267,  -202,
     141,  -202,    70,  -202,    74,    95,  -126,  -202,  -202,    46,
     178,    -3,  -201,  -202,  -202,   174,   -61,     0,  -202,    -9,
    -202,   -79,    20,   -64,     6,    45
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    33,    34,    35,    36,    37,    38,    39,   193,   194,
     135,    40,   196,    41,   198,    42,   230,    43,    52,    97,
      98,    80,   206,   207,   208,    82,    76,    59,   184,   185,
     186,   187,   268,   269,   337,   270,   155,   156,   231,   236,
     157,   158,   222,   223,   159,   160,   161,    57,    46,    47,
      48,    49,    50,    58,    54,   143
  };

  const short
  parser::yytable_[] =
  {
      45,   122,    44,   197,     2,   204,    51,     2,   210,    53,
      70,   141,    60,    60,     2,    63,    64,   277,   -45,    84,
      71,    72,    73,    74,   127,    89,   129,   144,   336,   146,
       2,   214,     2,   214,   214,    45,   240,    44,    55,    56,
     229,    51,    62,     2,    10,    66,   175,   176,   177,    99,
       2,   164,     9,   -93,    10,    94,   238,    77,   289,   141,
       2,   149,   214,   215,   273,   215,   215,    78,   -45,     9,
      10,    83,   197,   212,    95,   -45,    85,   216,   -93,   216,
     216,   140,   163,    10,    95,   313,   248,   239,   125,   224,
      10,   104,    99,   120,   215,   121,   170,   237,   105,    65,
     165,   166,   167,   141,   168,    90,   171,   172,   173,   174,
     142,   125,   101,   102,   178,   195,   180,   249,   181,    56,
     145,    56,   153,   188,   188,    45,   200,   199,   201,   140,
     203,    51,   260,   169,   104,   141,    91,   141,    56,    56,
      56,   105,   106,   107,   179,   304,   214,   108,   227,   228,
     225,   183,   183,   261,   262,   263,   110,   224,   142,   153,
     205,   282,   153,   241,   115,    56,   237,   237,   288,   114,
      69,   320,   286,   140,   125,    75,   125,   133,   215,    79,
      81,    99,   130,   213,   257,   116,   290,   252,     2,   226,
     153,   217,   216,   111,    45,    96,   199,   287,   148,   149,
      51,   218,   142,    92,   134,   140,   125,   140,    92,   131,
     141,   112,   116,   164,   300,   219,   282,   281,   225,    96,
     237,   141,   141,   104,   305,   224,   220,   247,    10,    92,
     105,   106,   107,   243,   142,   292,   142,   267,   119,   301,
     141,   139,   182,   244,    67,   154,   295,   245,   237,   237,
     237,   188,   282,   118,   116,   237,   235,   272,   148,   274,
      56,   276,   153,    68,   116,   237,   237,    94,   116,   237,
     237,   333,   317,   284,   285,   309,   310,   311,   116,   183,
     140,   124,   154,   117,   316,   154,   225,    92,   305,   139,
     132,   140,   140,   116,   205,    92,    93,   162,   321,   221,
     118,   209,   154,   154,   326,   327,    92,   123,   202,   142,
     140,   330,   331,   154,    92,   297,   116,   125,    92,   232,
     142,   142,   233,   246,   234,   153,   251,   312,   250,   253,
     255,   267,   259,   139,   318,   125,   342,   264,    99,   142,
     265,     2,   147,   254,   266,   322,   323,   271,   275,   278,
      96,   148,   149,   267,   283,   150,   328,   329,   101,   102,
     148,   294,   332,   103,   334,   139,   298,   139,   306,   302,
     303,   154,   319,   325,    88,   335,   340,   341,   125,   343,
     104,    10,   258,   299,    61,   154,   126,   105,   106,   107,
     154,   189,   296,   108,   109,   339,    86,   338,   324,     2,
     226,     1,     2,     3,     4,     5,     6,     7,     8,   148,
     149,     9,   291,   293,     0,     2,   226,     0,     0,     0,
       0,     0,     0,     0,     0,   148,   149,     0,     0,   154,
     154,   154,     0,     0,   151,   152,     0,     0,   154,    10,
     139,     0,    10,    11,     0,     0,     0,    12,   154,    13,
       0,   139,   139,    87,    14,    10,     0,    15,   154,   154,
       0,     0,    16,     0,     0,   154,   154,     0,     0,    17,
     139,    18,    19,    20,    21,     0,    22,     2,   136,     0,
      23,     0,    24,     0,     0,     0,     9,    25,    26,    27,
      28,    29,   279,   280,     0,     0,     0,    30,    31,    32,
       1,     2,     3,     4,     5,     6,     7,     8,   307,   308,
       9,     0,     0,     0,     2,   226,    99,    10,   137,     0,
       0,     0,   138,     0,   148,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,   102,     0,     0,
       0,    10,    11,     0,     0,     0,    12,     0,    13,     0,
       0,     0,   256,    14,    10,     0,    15,     0,   104,     0,
       0,    16,     0,    26,    27,   105,   106,   107,    17,     0,
       0,    19,    20,    21,     0,    22,     0,   190,   191,    23,
       0,    24,     0,     0,     0,     0,     0,    26,    27,    28,
     192,     0,     0,    99,     0,     0,    30,    31,    32,     1,
       2,     3,     4,     5,     6,     7,     8,   314,   315,     9,
       0,     0,     0,   101,   102,     0,     0,     0,   103,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,     0,
      10,    11,   105,   106,   107,    12,     0,    13,   108,   109,
       0,     0,    14,     0,     0,    15,     0,     0,     0,     0,
      16,     0,     0,   128,     0,     0,     0,    17,     0,    18,
      19,    20,    21,     0,    22,     0,     0,     0,    23,     0,
      24,     0,     0,     0,     0,    25,    26,    27,    28,    29,
       0,     0,     0,     0,     0,    30,    31,    32,     1,     2,
       3,     4,     5,     6,     7,     8,    99,     0,     9,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,   102,     0,     0,
       0,   103,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,     0,     0,    12,     0,    13,     0,   104,     0,
       0,    14,     0,     0,    15,   105,   106,   107,     0,    16,
       0,   108,   109,     0,     0,     0,    17,     0,     0,    19,
      20,    21,     0,    22,     0,   190,   191,    23,     0,    24,
       0,     0,     0,     0,     0,    26,    27,    28,   192,     0,
       0,     0,     0,     0,    30,    31,    32,     1,     2,     3,
       4,     5,     6,     7,     8,     0,    99,     9,     0,     0,
       0,     0,   113,     0,     0,     0,     0,     0,     1,     2,
       3,     4,     5,     6,     7,     8,   101,   102,     9,   148,
       0,   103,     0,     0,     0,     0,     0,     0,    10,    11,
     211,     0,     0,    12,     0,    13,     0,     0,   104,     0,
      14,     0,     0,    15,     0,   105,   106,   107,    16,    10,
      11,   108,   109,     0,    12,     0,    13,     0,    19,     0,
       0,    14,     0,     0,    15,     0,     0,     0,     0,    16,
       0,     0,     0,     0,    26,    27,     0,     0,     0,    19,
       0,     0,     0,    30,    31,    32,     0,     1,     2,     3,
       4,     5,     6,     7,     8,    26,    27,     9,     0,     0,
       0,     0,     0,     0,    30,    31,    32,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,    10,    11,
       0,   101,   102,    12,     0,    13,   103,     0,     0,     0,
      14,   101,   102,    15,     0,     0,   103,     0,    16,     0,
       0,   125,     0,   104,     0,     0,     0,     0,    19,     0,
     105,   106,   107,   104,   242,     0,   108,   109,    99,     0,
     105,   106,   107,     0,    26,    27,   108,   109,     0,     0,
       0,     0,     0,    30,    31,    32,     0,     0,   101,   102,
       0,     0,     0,   103,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,   105,   106,   107,
       0,     0,     0,   108,   109
  };

  const short
  parser::yycheck_[] =
  {
       0,    68,     0,   125,     6,   131,     0,     6,   134,     9,
      19,    90,    12,    13,     6,    15,    16,   218,     0,     0,
      20,    21,    22,    23,    71,    35,    73,    91,    14,    93,
       6,    19,     6,    19,    19,    35,   162,    35,    10,    11,
       7,    35,    14,     6,    46,    17,   110,   111,   112,     9,
       6,    98,    15,    20,    46,    47,    34,     5,    19,   138,
       6,    17,    19,    51,    52,    51,    51,    93,    50,    15,
      46,     0,   194,   137,    76,    57,    57,    65,    45,    65,
      65,    90,    20,    46,    76,   286,    14,    65,    49,   150,
      46,    51,     9,    65,    51,    67,   105,   158,    58,    98,
     100,   101,   102,   182,   104,    45,   106,   107,   108,   109,
      90,    49,    29,    30,   114,   125,   116,    45,   118,    91,
      92,    93,    94,   123,   124,   125,   126,   125,   128,   138,
     130,   125,   199,   105,    51,   214,    14,   216,   110,   111,
     112,    58,    59,    60,   116,    20,    19,    64,   151,   152,
     150,   123,   124,   200,   201,   202,    47,   218,   138,   131,
     132,   222,   134,   163,    48,   137,   227,   228,   232,    45,
      19,   297,    20,   182,    49,    24,    49,    20,    51,    28,
      29,     9,    20,   138,   194,    69,   233,   187,     6,     7,
     162,    48,    65,    47,   194,    44,   194,    45,    16,    17,
     194,    20,   182,    46,    47,   214,    49,   216,    46,    47,
     289,    47,    69,   260,    20,    34,   277,   220,   218,    68,
     281,   300,   301,    51,   271,   286,    45,   182,    46,    46,
      58,    59,    60,    48,   214,   235,   216,   209,    54,    45,
     319,    90,    45,    48,    74,    94,   246,    48,   309,   310,
     311,   251,   313,    69,    69,   316,    14,    48,    16,   214,
     232,   216,   234,    93,    69,   326,   327,    47,    69,   330,
     331,   318,    48,   227,   228,   278,   279,   280,    69,   251,
     289,    47,   131,    52,   287,   134,   286,    46,   335,   138,
      49,   300,   301,    69,   266,    46,    47,    47,   298,   148,
      69,    47,   151,   152,   307,   308,    46,    47,    81,   289,
     319,   314,   315,   162,    46,    47,    69,    49,    46,    47,
     300,   301,    48,    45,    69,   297,    69,   281,    48,    48,
      57,   303,    50,   182,   289,    49,   336,    48,     9,   319,
      50,     6,     7,   192,    69,   300,   301,    48,    10,    45,
     199,    16,    17,   325,    34,    20,   310,   311,    29,    30,
      16,    48,   316,    34,   319,   214,    20,   216,    52,    48,
      69,   220,    19,    47,    35,    48,   330,   331,    49,    48,
      51,    46,   194,   266,    13,   234,    57,    58,    59,    60,
     239,   124,   251,    64,    65,   325,     0,   323,   303,     6,
       7,     5,     6,     7,     8,     9,    10,    11,    12,    16,
      17,    15,   234,   239,    -1,     6,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    16,    17,    -1,    -1,   278,
     279,   280,    -1,    -1,    99,   100,    -1,    -1,   287,    46,
     289,    -1,    46,    47,    -1,    -1,    -1,    51,   297,    53,
      -1,   300,   301,    57,    58,    46,    -1,    61,   307,   308,
      -1,    -1,    66,    -1,    -1,   314,   315,    -1,    -1,    73,
     319,    75,    76,    77,    78,    -1,    80,     6,     7,    -1,
      84,    -1,    86,    -1,    -1,    -1,    15,    91,    92,    93,
      94,    95,    99,   100,    -1,    -1,    -1,   101,   102,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    99,   100,
      15,    -1,    -1,    -1,     6,     7,     9,    46,    47,    -1,
      -1,    -1,    51,    -1,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
      -1,    46,    47,    -1,    -1,    -1,    51,    -1,    53,    -1,
      -1,    -1,    57,    58,    46,    -1,    61,    -1,    51,    -1,
      -1,    66,    -1,    92,    93,    58,    59,    60,    73,    -1,
      -1,    76,    77,    78,    -1,    80,    -1,    82,    83,    84,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,     9,    -1,    -1,   101,   102,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    99,   100,    15,
      -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      46,    47,    58,    59,    60,    51,    -1,    53,    64,    65,
      -1,    -1,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    79,    -1,    -1,    -1,    73,    -1,    75,
      76,    77,    78,    -1,    80,    -1,    -1,    -1,    84,    -1,
      86,    -1,    -1,    -1,    -1,    91,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     9,    -1,    15,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    -1,    51,    -1,    53,    -1,    51,    -1,
      -1,    58,    -1,    -1,    61,    58,    59,    60,    -1,    66,
      -1,    64,    65,    -1,    -1,    -1,    73,    -1,    -1,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,     9,    15,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,     5,     6,
       7,     8,     9,    10,    11,    12,    29,    30,    15,    16,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      48,    -1,    -1,    51,    -1,    53,    -1,    -1,    51,    -1,
      58,    -1,    -1,    61,    -1,    58,    59,    60,    66,    46,
      47,    64,    65,    -1,    51,    -1,    53,    -1,    76,    -1,
      -1,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    76,
      -1,    -1,    -1,   101,   102,   103,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    92,    93,    15,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      -1,    29,    30,    51,    -1,    53,    34,    -1,    -1,    -1,
      58,    29,    30,    61,    -1,    -1,    34,    -1,    66,    -1,
      -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    76,    -1,
      58,    59,    60,    51,    52,    -1,    64,    65,     9,    -1,
      58,    59,    60,    -1,    92,    93,    64,    65,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,    -1,    -1,    29,    30,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    51,    53,    58,    61,    66,    73,    75,    76,
      77,    78,    80,    84,    86,    91,    92,    93,    94,    95,
     101,   102,   103,   105,   106,   107,   108,   109,   110,   111,
     115,   117,   119,   121,   122,   151,   152,   153,   154,   155,
     156,   158,   122,   151,   158,   119,   119,   151,   157,   131,
     151,   131,   119,   151,   151,    98,   119,    74,    93,   121,
     153,   151,   151,   151,   151,   121,   130,     5,    93,   121,
     125,   121,   129,     0,     0,    57,     0,    57,   108,   115,
      45,    14,    46,    47,    47,    76,   121,   123,   124,     9,
      14,    29,    30,    34,    51,    58,    59,    60,    64,    65,
      47,    47,    47,    15,    45,    48,    69,    52,    69,    54,
     119,   119,   124,    47,    47,    49,    57,   114,    79,   114,
      20,    47,    49,    20,    47,   114,     7,    47,    51,   121,
     153,   155,   156,   159,   157,   119,   157,     7,    16,    17,
      20,    99,   100,   119,   121,   140,   141,   144,   145,   148,
     149,   150,    47,    20,   114,   151,   151,   151,   151,   119,
     153,   151,   151,   151,   151,   157,   157,   157,   151,   119,
     151,   151,    45,   119,   132,   133,   134,   135,   151,   132,
      82,    83,    95,   112,   113,   115,   116,   117,   118,   122,
     151,   151,    81,   151,   140,   119,   126,   127,   128,    47,
     140,    48,   157,   159,    19,    51,    65,    48,    20,    34,
      45,   121,   146,   147,   150,   151,     7,   145,   145,     7,
     120,   142,    47,    48,    69,    14,   143,   150,    34,    65,
     140,   151,    52,    48,    48,    48,    45,   159,    14,    45,
      48,    69,   151,    48,   121,    57,    57,   115,   118,    50,
     124,   114,   114,   114,    48,    50,    69,   119,   136,   137,
     139,    48,    48,    52,   159,    10,   159,   146,    45,    99,
     100,   145,   150,    34,   143,   143,    20,    45,   157,    19,
     114,   144,   151,   149,    48,   151,   134,    47,    20,   128,
      20,    45,    48,    69,    20,   114,    52,    99,   100,   145,
     145,   145,   143,   146,    99,   100,   145,    48,   159,    19,
     140,   151,   159,   159,   139,    47,   145,   145,   143,   143,
     145,   145,   143,   114,   159,    48,    14,   138,   138,   136,
     143,   143,   151,    48
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   104,   105,   105,   106,   106,   106,   107,   107,   107,
     107,   108,   108,   108,   108,   108,   108,   109,   109,   110,
     110,   111,   111,   112,   112,   112,   113,   113,   113,   113,
     114,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     116,   116,   117,   117,   117,   117,   117,   118,   118,   118,
     119,   120,   121,   121,   121,   122,   122,   123,   123,   124,
     124,   125,   126,   126,   127,   127,   128,   129,   129,   130,
     130,   131,   131,   132,   132,   133,   133,   134,   134,   135,
     135,   136,   136,   137,   137,   138,   138,   139,   139,   140,
     140,   141,   141,   142,   142,   143,   143,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   145,   145,   145,   146,   146,   146,   147,   148,
     148,   148,   148,   149,   149,   149,   149,   150,   151,   151,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     153,   154,   154,   154,   154,   155,   155,   155,   155,   156,
     156,   157,   157,   157,   157,   158,   158,   159,   159,   159,
     159,   159,   159,   159,   159,   159
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     3,     2,     4,     1,     2,     1,     3,     2,
       3,     1,     3,     0,     1,     2,     1,     2,     2,     2,
       3,     3,     5,     5,     3,     5,     3,     3,     6,     2,
       1,     1,     1,     2,     2,     1,     3,     1,     1,     1,
       1,     1,     2,     1,     3,     1,     2,     1,     1,     4,
       6,     4,     0,     1,     1,     3,     1,     5,     8,     3,
       6,     1,     3,     0,     1,     1,     3,     2,     1,     2,
       2,     0,     1,     1,     3,     0,     2,     4,     4,     0,
       1,     1,     3,     0,     1,     0,     2,     5,     6,     6,
       4,     4,     5,     5,     3,     4,     5,     5,     2,     3,
       3,     2,     1,     1,     2,     1,     1,     2,     1,     1,
       2,     1,     2,     1,     1,     4,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     4,     2,     3,     3,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     2,     2,     5,     5,     4,     4,     4,     4,     5,
       7,     1,     3,     3,     5,     2,     1,     1,     1,     1,
       3,     2,     3,     4,     3,     3
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
  "\"`[[`\"", "\"`]]`\"", "\"`<`\"", "\"`>`\"", "\"`;`\"", "\"`.`\"",
  "\"`+`\"", "\"`-`\"", "\"`*`\"", "\"`/`\"", "\"`%`\"", "\"`&`\"",
  "\"`|`\"", "\"`!`\"", "\"`~`\"", "\"`^`\"", "\",\"", "\"`$`\"",
  "\"`?`\"", "\"`#`\"", "LET", "VAR", "EXTERN", "\"`new`\"", "\"`while`\"",
  "\"`for`\"", "\"`in`\"", "\"`if`\"", "\"`else`\"", "\"`continue`\"",
  "\"`break`\"", "\"`return`\"", "AUTO", "USING", "THROW", "SIZEOF",
  "DEREF", "LOWEST", "INCLUDE", "INLINE", "\"`fn`\"", "ENUM", "STRUCT",
  "EXTENDS", "TYPENAME", "\"weak modifier\"", "\"const modifier\"",
  "\"mut modifier\"", "\"nil\"", "\"true\"", "\"false\"", "$accept",
  "begin", "statement_any", "finished-statement-any", "statement",
  "let-decl", "let-decl-start", "let-decl-start-with-opt-type",
  "infunction-statement-any", "finished-infunction-statement-any",
  "braced-statements", "finished-statement", "infunction-statement-set",
  "generic-statement", "infunction-statement", "identifier",
  "internal-identifier", "qname", "fn-start-decl", "fn-name", "fn-decl",
  "enum-decl", "case-list-opt", "case-list", "case-decl", "struct-decl",
  "using-decl", "expression-list", "argument-list-opt", "argument-list",
  "argument", "argument-name", "field-list-opt", "field-list",
  "field-default-value-opt", "field", "parameter-list-opt",
  "parameter-list", "internal-identifier-opt",
  "parameter-default-value-opt", "parameter-decl",
  "parameter-constraint-set", "parameter-value-constraint-set",
  "parameter-matched-value", "parameter-matched-type",
  "basic-parameter-matched-type", "concept-expression",
  "syntax-expression", "syntax-expression-wo-ii", "apostrophe-expression",
  "new-expression", "call-expression", "lambda-expression",
  "pack-expression", "compound-expression", "type-expr", YY_NULLPTR
  };
#endif


#if BANG_LANGDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   330,   330,   331,   335,   337,   339,   356,   358,   359,
     361,   366,   368,   370,   376,   383,   385,   390,   392,   397,
     399,   404,   405,   410,   412,   414,   419,   421,   422,   424,
     429,   434,   436,   438,   440,   442,   444,   446,   448,   450,
     455,   457,   462,   464,   466,   468,   470,   475,   477,   479,
     484,   492,   507,   509,   511,   517,   519,   524,   525,   529,
     531,   537,   542,   543,   547,   549,   554,   560,   562,   585,
     589,   603,   605,   611,   612,   616,   618,   623,   625,   630,
     632,   638,   639,   643,   645,   650,   651,   655,   657,   668,
     669,   673,   675,   680,   681,   685,   686,   690,   692,   694,
     696,   699,   701,   703,   705,   708,   710,   712,   715,   717,
     719,   721,   726,   728,   730,   735,   737,   739,   759,   763,
     764,   766,   768,   774,   780,   783,   785,   808,   813,   815,
     820,   822,   824,   826,   828,   830,   832,   834,   836,   838,
     840,   848,   856,   858,   860,   864,   866,   868,   871,   873,
     878,   880,   884,   886,   890,   894,   897,   898,   899,   900,
     904,   909,   911,   913,   915,   920,   922,   924,   926,   931,
     933,   938,   944,   950,   955,   963,   965,  1051,  1053,  1054,
    1058,  1060,  1062,  1064,  1066,  1074
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
      95,    96,    97,    98,    99,   100,   101,   102,   103
    };
    // Last valid token kind.
    const int code_max = 358;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4542 "bang.tab.cpp"

#line 1417 "bang.y"

