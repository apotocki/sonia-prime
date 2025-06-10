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

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.copy< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.copy< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.copy< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.copy< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.copy< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.copy< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_156_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.copy< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.copy< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
      case symbol_kind::S_MUT: // "mut modifier"
        value.copy< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.copy< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.copy< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
        value.copy< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_156_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
      case symbol_kind::S_MUT: // "mut modifier"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.YY_MOVE_OR_COPY< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.YY_MOVE_OR_COPY< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.YY_MOVE_OR_COPY< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.YY_MOVE_OR_COPY< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.YY_MOVE_OR_COPY< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.YY_MOVE_OR_COPY< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.YY_MOVE_OR_COPY< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.YY_MOVE_OR_COPY< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.YY_MOVE_OR_COPY< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.YY_MOVE_OR_COPY< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_156_pack_expression: // pack-expression
        value.YY_MOVE_OR_COPY< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.YY_MOVE_OR_COPY< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.YY_MOVE_OR_COPY< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.YY_MOVE_OR_COPY< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.YY_MOVE_OR_COPY< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
      case symbol_kind::S_MUT: // "mut modifier"
        value.YY_MOVE_OR_COPY< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.YY_MOVE_OR_COPY< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.YY_MOVE_OR_COPY< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.YY_MOVE_OR_COPY< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.YY_MOVE_OR_COPY< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.YY_MOVE_OR_COPY< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
        value.YY_MOVE_OR_COPY< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.move< enum_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.move< expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.move< field_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.move< finished_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.move< fn_pure_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.move< generic_statement_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_156_pack_expression: // pack-expression
        value.move< named_expression_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
      case symbol_kind::S_MUT: // "mut modifier"
        value.move< sonia::lang::lex::resource_location > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.move< statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.move< struct_decl > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
        value.move< syntax_expression_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.copy< enum_decl > (that.value);
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.copy< expression_list_t > (that.value);
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.copy< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.copy< field_t > (that.value);
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.copy< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.copy< fn_kind > (that.value);
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.copy< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.copy< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.copy< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.copy< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_156_pack_expression: // pack-expression
        value.copy< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< named_expression_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.copy< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.copy< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.copy< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.copy< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.copy< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.copy< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.copy< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
      case symbol_kind::S_MUT: // "mut modifier"
        value.copy< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.copy< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.copy< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.copy< statement > (that.value);
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.copy< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.copy< struct_decl > (that.value);
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
        value.copy< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.move< enum_decl > (that.value);
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.move< expression_list_t > (that.value);
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.move< field_list_t > (that.value);
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (that.value);
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.move< finished_statement_type > (that.value);
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (that.value);
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.move< fn_pure_t > (that.value);
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.move< generic_statement_type > (that.value);
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (that.value);
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (that.value);
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_156_pack_expression: // pack-expression
        value.move< named_expression_list_t > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (that.value);
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        value.move< parameter_constraint_set_t > (that.value);
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (that.value);
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (that.value);
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (that.value);
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (that.value);
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (that.value);
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
      case symbol_kind::S_MUT: // "mut modifier"
        value.move< sonia::lang::lex::resource_location > (that.value);
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (that.value);
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.move< statement > (that.value);
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (that.value);
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.move< struct_decl > (that.value);
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
        value.move< syntax_expression_t > (that.value);
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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
#line 321 "bang.y"
                 { }
#line 1524 "bang.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 321 "bang.y"
                 { }
#line 1530 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 321 "bang.y"
                 { }
#line 1536 "bang.tab.cpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 321 "bang.y"
                 { }
#line 1542 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 321 "bang.y"
                 { }
#line 1548 "bang.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 321 "bang.y"
                 { }
#line 1554 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 321 "bang.y"
                 { }
#line 1560 "bang.tab.cpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 321 "bang.y"
                 { }
#line 1566 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 321 "bang.y"
                 { }
#line 1572 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 321 "bang.y"
                 { }
#line 1578 "bang.tab.cpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 321 "bang.y"
                 { }
#line 1584 "bang.tab.cpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 321 "bang.y"
                 { }
#line 1590 "bang.tab.cpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 321 "bang.y"
                 { }
#line 1596 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 321 "bang.y"
                 { }
#line 1602 "bang.tab.cpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 321 "bang.y"
                 { }
#line 1608 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 321 "bang.y"
                 { }
#line 1614 "bang.tab.cpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 321 "bang.y"
                 { }
#line 1620 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 321 "bang.y"
                 { }
#line 1626 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 321 "bang.y"
                 { }
#line 1632 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 321 "bang.y"
                 { }
#line 1638 "bang.tab.cpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 321 "bang.y"
                 { }
#line 1644 "bang.tab.cpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 321 "bang.y"
                 { }
#line 1650 "bang.tab.cpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 321 "bang.y"
                 { }
#line 1656 "bang.tab.cpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 321 "bang.y"
                 { }
#line 1662 "bang.tab.cpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 321 "bang.y"
                 { }
#line 1668 "bang.tab.cpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 321 "bang.y"
                 { }
#line 1674 "bang.tab.cpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 321 "bang.y"
                 { }
#line 1680 "bang.tab.cpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 321 "bang.y"
                 { }
#line 1686 "bang.tab.cpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 321 "bang.y"
                 { }
#line 1692 "bang.tab.cpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 321 "bang.y"
                 { }
#line 1698 "bang.tab.cpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 321 "bang.y"
                 { }
#line 1704 "bang.tab.cpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 321 "bang.y"
                 { }
#line 1710 "bang.tab.cpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 321 "bang.y"
                 { }
#line 1716 "bang.tab.cpp"
        break;

      case symbol_kind::S_MUT: // "mut modifier"
#line 321 "bang.y"
                 { }
#line 1722 "bang.tab.cpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 321 "bang.y"
                 { }
#line 1728 "bang.tab.cpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 321 "bang.y"
                 { }
#line 1734 "bang.tab.cpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 321 "bang.y"
                 { }
#line 1740 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 321 "bang.y"
                 { }
#line 1746 "bang.tab.cpp"
        break;

      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
#line 321 "bang.y"
                 { }
#line 1752 "bang.tab.cpp"
        break;

      case symbol_kind::S_statement: // statement
#line 321 "bang.y"
                 { }
#line 1758 "bang.tab.cpp"
        break;

      case symbol_kind::S_108_let_decl: // let-decl
#line 321 "bang.y"
                 { }
#line 1764 "bang.tab.cpp"
        break;

      case symbol_kind::S_109_let_decl_start: // let-decl-start
#line 321 "bang.y"
                 { }
#line 1770 "bang.tab.cpp"
        break;

      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 321 "bang.y"
                 { }
#line 1776 "bang.tab.cpp"
        break;

      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
#line 321 "bang.y"
                 { }
#line 1782 "bang.tab.cpp"
        break;

      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
#line 321 "bang.y"
                 { }
#line 1788 "bang.tab.cpp"
        break;

      case symbol_kind::S_113_braced_statements: // braced-statements
#line 321 "bang.y"
                 { }
#line 1794 "bang.tab.cpp"
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
#line 321 "bang.y"
                 { }
#line 1800 "bang.tab.cpp"
        break;

      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
#line 321 "bang.y"
                 { }
#line 1806 "bang.tab.cpp"
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
#line 321 "bang.y"
                 { }
#line 1812 "bang.tab.cpp"
        break;

      case symbol_kind::S_117_infunction_statement: // infunction-statement
#line 321 "bang.y"
                 { }
#line 1818 "bang.tab.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 321 "bang.y"
                 { }
#line 1824 "bang.tab.cpp"
        break;

      case symbol_kind::S_119_internal_identifier: // internal-identifier
#line 321 "bang.y"
                 { }
#line 1830 "bang.tab.cpp"
        break;

      case symbol_kind::S_qname: // qname
#line 321 "bang.y"
                 { }
#line 1836 "bang.tab.cpp"
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
#line 321 "bang.y"
                 { }
#line 1842 "bang.tab.cpp"
        break;

      case symbol_kind::S_122_fn_name: // fn-name
#line 321 "bang.y"
                 { }
#line 1848 "bang.tab.cpp"
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
#line 321 "bang.y"
                 { }
#line 1854 "bang.tab.cpp"
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
#line 321 "bang.y"
                 { }
#line 1860 "bang.tab.cpp"
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
#line 321 "bang.y"
                 { }
#line 1866 "bang.tab.cpp"
        break;

      case symbol_kind::S_126_case_list: // case-list
#line 321 "bang.y"
                 { }
#line 1872 "bang.tab.cpp"
        break;

      case symbol_kind::S_127_case_decl: // case-decl
#line 321 "bang.y"
                 { }
#line 1878 "bang.tab.cpp"
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
#line 321 "bang.y"
                 { }
#line 1884 "bang.tab.cpp"
        break;

      case symbol_kind::S_129_using_decl: // using-decl
#line 321 "bang.y"
                 { }
#line 1890 "bang.tab.cpp"
        break;

      case symbol_kind::S_130_expression_list: // expression-list
#line 321 "bang.y"
                 { }
#line 1896 "bang.tab.cpp"
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
#line 321 "bang.y"
                 { }
#line 1902 "bang.tab.cpp"
        break;

      case symbol_kind::S_132_argument_list: // argument-list
#line 321 "bang.y"
                 { }
#line 1908 "bang.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 321 "bang.y"
                 { }
#line 1914 "bang.tab.cpp"
        break;

      case symbol_kind::S_134_argument_name: // argument-name
#line 321 "bang.y"
                 { }
#line 1920 "bang.tab.cpp"
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
#line 321 "bang.y"
                 { }
#line 1926 "bang.tab.cpp"
        break;

      case symbol_kind::S_136_field_list: // field-list
#line 321 "bang.y"
                 { }
#line 1932 "bang.tab.cpp"
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
#line 321 "bang.y"
                 { }
#line 1938 "bang.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 321 "bang.y"
                 { }
#line 1944 "bang.tab.cpp"
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
#line 321 "bang.y"
                 { }
#line 1950 "bang.tab.cpp"
        break;

      case symbol_kind::S_140_parameter_list: // parameter-list
#line 321 "bang.y"
                 { }
#line 1956 "bang.tab.cpp"
        break;

      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
#line 321 "bang.y"
                 { }
#line 1962 "bang.tab.cpp"
        break;

      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
#line 321 "bang.y"
                 { }
#line 1968 "bang.tab.cpp"
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
#line 321 "bang.y"
                 { }
#line 1974 "bang.tab.cpp"
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
#line 321 "bang.y"
                 { }
#line 1980 "bang.tab.cpp"
        break;

      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
#line 321 "bang.y"
                 { }
#line 1986 "bang.tab.cpp"
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
#line 321 "bang.y"
                 { }
#line 1992 "bang.tab.cpp"
        break;

      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
#line 321 "bang.y"
                 { }
#line 1998 "bang.tab.cpp"
        break;

      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
#line 321 "bang.y"
                 { }
#line 2004 "bang.tab.cpp"
        break;

      case symbol_kind::S_149_concept_expression: // concept-expression
#line 321 "bang.y"
                 { }
#line 2010 "bang.tab.cpp"
        break;

      case symbol_kind::S_150_syntax_expression: // syntax-expression
#line 321 "bang.y"
                 { }
#line 2016 "bang.tab.cpp"
        break;

      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 321 "bang.y"
                 { }
#line 2022 "bang.tab.cpp"
        break;

      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
#line 321 "bang.y"
                 { }
#line 2028 "bang.tab.cpp"
        break;

      case symbol_kind::S_153_new_expression: // new-expression
#line 321 "bang.y"
                 { }
#line 2034 "bang.tab.cpp"
        break;

      case symbol_kind::S_154_call_expression: // call-expression
#line 321 "bang.y"
                 { }
#line 2040 "bang.tab.cpp"
        break;

      case symbol_kind::S_155_lambda_expression: // lambda-expression
#line 321 "bang.y"
                 { }
#line 2046 "bang.tab.cpp"
        break;

      case symbol_kind::S_156_pack_expression: // pack-expression
#line 321 "bang.y"
                 { }
#line 2052 "bang.tab.cpp"
        break;

      case symbol_kind::S_157_compound_expression: // compound-expression
#line 321 "bang.y"
                 { }
#line 2058 "bang.tab.cpp"
        break;

      case symbol_kind::S_158_type_expr: // type-expr
#line 321 "bang.y"
                 { }
#line 2064 "bang.tab.cpp"
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

      case symbol_kind::S_124_enum_decl: // enum-decl
        yylhs.value.emplace< enum_decl > ();
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        yylhs.value.emplace< expression_list_t > ();
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        yylhs.value.emplace< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        yylhs.value.emplace< field_t > ();
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        yylhs.value.emplace< finished_statement_type > ();
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        yylhs.value.emplace< fn_kind > ();
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        yylhs.value.emplace< fn_pure_t > ();
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        yylhs.value.emplace< generic_statement_type > ();
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        yylhs.value.emplace< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        yylhs.value.emplace< managed_statement_list > ();
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_156_pack_expression: // pack-expression
        yylhs.value.emplace< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< named_expression_t > ();
        break;

      case symbol_kind::S_144_parameter_constraint_set: // parameter-constraint-set
      case symbol_kind::S_145_parameter_value_constraint_set: // parameter-value-constraint-set
        yylhs.value.emplace< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        yylhs.value.emplace< parameter_list_t > ();
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        yylhs.value.emplace< parameter_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        yylhs.value.emplace< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        yylhs.value.emplace< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        yylhs.value.emplace< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        yylhs.value.emplace< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_127_case_decl: // case-decl
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
      case symbol_kind::S_RETURN: // "`return`"
      case symbol_kind::S_FN: // "`fn`"
      case symbol_kind::S_MUT: // "mut modifier"
        yylhs.value.emplace< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        yylhs.value.emplace< sonia::optional<syntax_expression_t> > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        yylhs.value.emplace< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        yylhs.value.emplace< statement > ();
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        yylhs.value.emplace< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        yylhs.value.emplace< struct_decl > ();
        break;

      case symbol_kind::S_146_parameter_matched_value: // parameter-matched-value
      case symbol_kind::S_147_parameter_matched_type: // parameter-matched-type
      case symbol_kind::S_148_basic_parameter_matched_type: // basic-parameter-matched-type
      case symbol_kind::S_149_concept_expression: // concept-expression
      case symbol_kind::S_150_syntax_expression: // syntax-expression
      case symbol_kind::S_151_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_152_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_153_new_expression: // new-expression
      case symbol_kind::S_154_call_expression: // call-expression
      case symbol_kind::S_155_lambda_expression: // lambda-expression
      case symbol_kind::S_157_compound_expression: // compound-expression
      case symbol_kind::S_158_type_expr: // type-expr
        yylhs.value.emplace< syntax_expression_t > ();
        break;

      case symbol_kind::S_129_using_decl: // using-decl
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
#line 326 "bang.y"
                            { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2500 "bang.tab.cpp"
    break;

  case 3: // begin: finished-statement-any "end of file"
#line 327 "bang.y"
                                 { ctx.set_root_statements(std::move(yystack_[1].value.as < managed_statement_list > ())); }
#line 2506 "bang.tab.cpp"
    break;

  case 4: // statement_any: %empty
#line 332 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2512 "bang.tab.cpp"
    break;

  case 5: // statement_any: statement
#line 334 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2518 "bang.tab.cpp"
    break;

  case 6: // statement_any: finished-statement-any statement
#line 336 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2524 "bang.tab.cpp"
    break;

  case 7: // finished-statement-any: finished-statement
#line 353 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2530 "bang.tab.cpp"
    break;

  case 8: // finished-statement-any: finished-statement-any "`;`"
#line 354 "bang.y"
                                                { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2536 "bang.tab.cpp"
    break;

  case 9: // finished-statement-any: finished-statement-any finished-statement
#line 356 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2542 "bang.tab.cpp"
    break;

  case 10: // finished-statement-any: statement_any "`;`"
#line 358 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2548 "bang.tab.cpp"
    break;

  case 11: // statement: EXTERN VAR identifier "`:`" type-expr
#line 363 "bang.y"
        { yylhs.value.as < statement > () = extern_var{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), &ctx.push(std::move(yystack_[0].value.as < syntax_expression_t > ())) }; }
#line 2554 "bang.tab.cpp"
    break;

  case 12: // statement: EXTERN "`fn`" fn-decl
#line 365 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < fn_pure_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2560 "bang.tab.cpp"
    break;

  case 13: // statement: INCLUDE STRING
#line 367 "bang.y"
        { yylhs.value.as < statement > () = include_decl{ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())) }; }
#line 2566 "bang.tab.cpp"
    break;

  case 14: // statement: fn-start-decl fn-name "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 369 "bang.y"
        { 
            auto sts = ctx.new_statement_list();
            sts.emplace_back(return_decl_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) });
            yylhs.value.as < statement > () = fn_decl_t{ fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .kind = yystack_[6].value.as < fn_kind > () }, ctx.push(std::move(sts)) };
            IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
            //     $$ = fn_decl_t{ fn_pure_t{ .aname = std::move($name), .parameters = std::move($parameters), .result = std::move($value), .is_type_expression_result = false, .kind = $fnkind } }; IGNORE_TERM($OPEN_PARENTHESIS); }
        }
#line 2578 "bang.tab.cpp"
    break;

  case 15: // statement: generic-statement
#line 377 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2584 "bang.tab.cpp"
    break;

  case 16: // statement: STRUCT struct-decl
#line 379 "bang.y"
        { yylhs.value.as < statement > () = std::move(yystack_[0].value.as < struct_decl > ()); }
#line 2590 "bang.tab.cpp"
    break;

  case 17: // let-decl: let-decl-start-with-opt-type
#line 384 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2596 "bang.tab.cpp"
    break;

  case 18: // let-decl: let-decl-start-with-opt-type "`=`" pack-expression
#line 386 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().expressions = std::move(yystack_[0].value.as < named_expression_list_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 2602 "bang.tab.cpp"
    break;

  case 19: // let-decl-start: identifier
#line 391 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, false }; }
#line 2608 "bang.tab.cpp"
    break;

  case 20: // let-decl-start: "weak modifier" identifier
#line 393 "bang.y"
        { yylhs.value.as < let_statement > () = let_statement{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()), {}, {}, true }; }
#line 2614 "bang.tab.cpp"
    break;

  case 21: // let-decl-start-with-opt-type: let-decl-start
#line 397 "bang.y"
      { yylhs.value.as < let_statement > () = yystack_[0].value.as < let_statement > (); }
#line 2620 "bang.tab.cpp"
    break;

  case 22: // let-decl-start-with-opt-type: let-decl-start "`:`" type-expr
#line 399 "bang.y"
        { yylhs.value.as < let_statement > () = std::move(yystack_[2].value.as < let_statement > ()); yylhs.value.as < let_statement > ().type = std::move(yystack_[0].value.as < syntax_expression_t > ()); }
#line 2626 "bang.tab.cpp"
    break;

  case 23: // infunction-statement-any: %empty
#line 404 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); }
#line 2632 "bang.tab.cpp"
    break;

  case 24: // infunction-statement-any: infunction-statement
#line 406 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); }
#line 2638 "bang.tab.cpp"
    break;

  case 25: // infunction-statement-any: finished-infunction-statement-any infunction-statement
#line 408 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < statement > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2644 "bang.tab.cpp"
    break;

  case 26: // finished-infunction-statement-any: finished-statement
#line 413 "bang.y"
        { yylhs.value.as < managed_statement_list > () = ctx.new_statement_list(); yylhs.value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); }
#line 2650 "bang.tab.cpp"
    break;

  case 27: // finished-infunction-statement-any: finished-infunction-statement-any "`;`"
#line 414 "bang.y"
                                                           { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2656 "bang.tab.cpp"
    break;

  case 28: // finished-infunction-statement-any: finished-infunction-statement-any finished-statement
#line 416 "bang.y"
        { yystack_[1].value.as < managed_statement_list > ().emplace_back(std::move(yystack_[0].value.as < finished_statement_type > ())); yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2662 "bang.tab.cpp"
    break;

  case 29: // finished-infunction-statement-any: infunction-statement-any "`;`"
#line 418 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); }
#line 2668 "bang.tab.cpp"
    break;

  case 30: // braced-statements: "`{`" infunction-statement-set "`}`"
#line 423 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[1].value.as < managed_statement_list > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2674 "bang.tab.cpp"
    break;

  case 31: // finished-statement: "`while`" syntax-expression braced-statements
#line 428 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2680 "bang.tab.cpp"
    break;

  case 32: // finished-statement: "`while`" syntax-expression "`;`" syntax-expression braced-statements
#line 430 "bang.y"
        { yylhs.value.as < finished_statement_type > () = while_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 2686 "bang.tab.cpp"
    break;

  case 33: // finished-statement: "`for`" syntax-expression "`in`" syntax-expression braced-statements
#line 432 "bang.y"
        { yylhs.value.as < finished_statement_type > () = for_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2692 "bang.tab.cpp"
    break;

  case 34: // finished-statement: "`if`" syntax-expression braced-statements
#line 434 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[1].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2698 "bang.tab.cpp"
    break;

  case 35: // finished-statement: "`if`" syntax-expression braced-statements "`else`" braced-statements
#line 436 "bang.y"
        { yylhs.value.as < finished_statement_type > () = if_decl{ std::move(yystack_[3].value.as < syntax_expression_t > ()), ctx.push(std::move(yystack_[2].value.as < managed_statement_list > ())), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2704 "bang.tab.cpp"
    break;

  case 36: // finished-statement: fn-start-decl fn-decl braced-statements
#line 438 "bang.y"
        { yystack_[1].value.as < fn_pure_t > ().kind = yystack_[2].value.as < fn_kind > (); yylhs.value.as < finished_statement_type > () = fn_decl_t{ std::move(yystack_[1].value.as < fn_pure_t > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2710 "bang.tab.cpp"
    break;

  case 37: // finished-statement: STRUCT qname braced-statements
#line 440 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_qname > ()), ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; }
#line 2716 "bang.tab.cpp"
    break;

  case 38: // finished-statement: STRUCT qname "`(`" parameter-list-opt "`)`" braced-statements
#line 442 "bang.y"
        { yylhs.value.as < finished_statement_type > () = struct_decl{ fn_pure_t{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[2].value.as < parameter_list_t > ()) }, ctx.push(std::move(yystack_[0].value.as < managed_statement_list > ())) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 2722 "bang.tab.cpp"
    break;

  case 39: // finished-statement: ENUM enum-decl
#line 444 "bang.y"
        { yylhs.value.as < finished_statement_type > () = std::move(yystack_[0].value.as < enum_decl > ()); }
#line 2728 "bang.tab.cpp"
    break;

  case 40: // infunction-statement-set: infunction-statement-any
#line 449 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2734 "bang.tab.cpp"
    break;

  case 41: // infunction-statement-set: finished-infunction-statement-any
#line 451 "bang.y"
        { yylhs.value.as < managed_statement_list > () = std::move(yystack_[0].value.as < managed_statement_list > ()); }
#line 2740 "bang.tab.cpp"
    break;

  case 42: // generic-statement: LET let-decl
#line 456 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < let_statement > ()); }
#line 2746 "bang.tab.cpp"
    break;

  case 43: // generic-statement: USING using-decl
#line 458 "bang.y"
        { yylhs.value.as < generic_statement_type > () = std::move(yystack_[0].value.as < using_decl > ()); }
#line 2752 "bang.tab.cpp"
    break;

  case 44: // generic-statement: compound-expression
#line 460 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2758 "bang.tab.cpp"
    break;

  case 45: // generic-statement: syntax-expression "`=`" syntax-expression
#line 462 "bang.y"
        { yylhs.value.as < generic_statement_type > () = expression_statement_t{ binary_expression_t{ binary_operator_type::ASSIGN, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) } }; }
#line 2764 "bang.tab.cpp"
    break;

  case 46: // infunction-statement: generic-statement
#line 467 "bang.y"
        { yylhs.value.as < statement > () = apply_visitor(statement_adopt_visitor<statement>{}, yystack_[0].value.as < generic_statement_type > ()); }
#line 2770 "bang.tab.cpp"
    break;

  case 47: // infunction-statement: "`break`"
#line 469 "bang.y"
        { yylhs.value.as < statement > () = break_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2776 "bang.tab.cpp"
    break;

  case 48: // infunction-statement: "`continue`"
#line 471 "bang.y"
        { yylhs.value.as < statement > () = continue_statement_t{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2782 "bang.tab.cpp"
    break;

  case 49: // infunction-statement: "`return`"
#line 473 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .location = std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2788 "bang.tab.cpp"
    break;

  case 50: // infunction-statement: "`return`" syntax-expression
#line 475 "bang.y"
        { yylhs.value.as < statement > () = return_decl_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()), .location = std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 2794 "bang.tab.cpp"
    break;

  case 51: // identifier: IDENTIFIER
#line 480 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2800 "bang.tab.cpp"
    break;

  case 52: // internal-identifier: INTERNAL_IDENTIFIER
#line 488 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = ctx.make_identifier(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 2806 "bang.tab.cpp"
    break;

  case 53: // qname: "`::`" identifier
#line 503 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2812 "bang.tab.cpp"
    break;

  case 54: // qname: identifier
#line 505 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = annotated_qname{ qname{yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value, false}, std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().location) }; }
#line 2818 "bang.tab.cpp"
    break;

  case 55: // qname: qname "`::`" identifier
#line 507 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_qname > () = std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()); yylhs.value.as < sonia::lang::bang::annotated_qname > ().value.append(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value)); }
#line 2824 "bang.tab.cpp"
    break;

  case 56: // fn-start-decl: "`fn`"
#line 513 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::DEFAULT; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2830 "bang.tab.cpp"
    break;

  case 57: // fn-start-decl: INLINE "`fn`"
#line 515 "bang.y"
        { yylhs.value.as < fn_kind > () = fn_kind::INLINE; IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2836 "bang.tab.cpp"
    break;

  case 58: // fn-name: qname
#line 519 "bang.y"
      { yylhs.value.as < sonia::lang::bang::annotated_qname > () = yystack_[0].value.as < sonia::lang::bang::annotated_qname > (); }
#line 2842 "bang.tab.cpp"
    break;

  case 59: // fn-name: "`new`"
#line 520 "bang.y"
          { yylhs.value.as < sonia::lang::bang::annotated_qname > () = ctx.make_qname(annotated_string_view{ "new"sv, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }); }
#line 2848 "bang.tab.cpp"
    break;

  case 60: // fn-decl: fn-name "`(`" parameter-list-opt "`)`"
#line 525 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[1].value.as < parameter_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2854 "bang.tab.cpp"
    break;

  case 61: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`~>`" type-expr
#line 527 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()), .is_type_expression_result = true }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2860 "bang.tab.cpp"
    break;

  case 62: // fn-decl: fn-name "`(`" parameter-list-opt "`)`" "`->`" type-expr
#line 529 "bang.y"
        { yylhs.value.as < fn_pure_t > () = fn_pure_t{ .aname = std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), .parameters = std::move(yystack_[3].value.as < parameter_list_t > ()), .result = std::move(yystack_[0].value.as < syntax_expression_t > ()), .is_type_expression_result = true, .is_not_a_pattern_result = true }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ()); }
#line 2866 "bang.tab.cpp"
    break;

  case 63: // enum-decl: qname "`{`" case-list-opt "`}`"
#line 535 "bang.y"
        { yylhs.value.as < enum_decl > () = enum_decl{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < std::vector<sonia::lang::bang::identifier> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2872 "bang.tab.cpp"
    break;

  case 64: // case-list-opt: %empty
#line 539 "bang.y"
             { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = {}; }
#line 2878 "bang.tab.cpp"
    break;

  case 65: // case-list-opt: case-list
#line 540 "bang.y"
      { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = yystack_[0].value.as < std::vector<sonia::lang::bang::identifier> > (); }
#line 2884 "bang.tab.cpp"
    break;

  case 66: // case-list: case-decl
#line 545 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::vector<sonia::lang::bang::identifier>{std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())}; }
#line 2890 "bang.tab.cpp"
    break;

  case 67: // case-list: case-list "," case-decl
#line 547 "bang.y"
        { yylhs.value.as < std::vector<sonia::lang::bang::identifier> > () = std::move(yystack_[2].value.as < std::vector<sonia::lang::bang::identifier> > ()); yylhs.value.as < std::vector<sonia::lang::bang::identifier> > ().emplace_back(std::move(yystack_[0].value.as < sonia::lang::bang::identifier > ())); }
#line 2896 "bang.tab.cpp"
    break;

  case 68: // case-decl: identifier
#line 552 "bang.y"
        { yylhs.value.as < sonia::lang::bang::identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ().value; }
#line 2902 "bang.tab.cpp"
    break;

  case 69: // struct-decl: qname "`=>`" "`(`" field-list-opt "`)`"
#line 558 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2908 "bang.tab.cpp"
    break;

  case 70: // struct-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" "`(`" field-list-opt "`)`"
#line 560 "bang.y"
        { yylhs.value.as < struct_decl > () = struct_decl{ fn_pure_t{ std::move(yystack_[7].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[5].value.as < parameter_list_t > ()) }, std::move(yystack_[1].value.as < field_list_t > ()) }; IGNORE_TERM(yystack_[6].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 2914 "bang.tab.cpp"
    break;

  case 71: // using-decl: qname "`=>`" syntax-expression
#line 583 "bang.y"
        { 
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[2].value.as < sonia::lang::bang::annotated_qname > ()), nullopt, std::move(yystack_[0].value.as < syntax_expression_t > ()) };
        }
#line 2922 "bang.tab.cpp"
    break;

  case 72: // using-decl: qname "`(`" parameter-list-opt "`)`" "`=>`" syntax-expression
#line 587 "bang.y"
        {
            yylhs.value.as < using_decl > () = using_decl{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[3].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[4].value.as < sonia::lang::lex::resource_location > ());
        }
#line 2930 "bang.tab.cpp"
    break;

  case 73: // expression-list: syntax-expression
#line 601 "bang.y"
        { yylhs.value.as < expression_list_t > () = expression_list_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2936 "bang.tab.cpp"
    break;

  case 74: // expression-list: expression-list "," syntax-expression
#line 603 "bang.y"
        { yylhs.value.as < expression_list_t > () = std::move(yystack_[2].value.as < expression_list_t > ()); yylhs.value.as < expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 2942 "bang.tab.cpp"
    break;

  case 75: // argument-list-opt: %empty
#line 608 "bang.y"
              { yylhs.value.as < named_expression_list_t > () = {}; }
#line 2948 "bang.tab.cpp"
    break;

  case 76: // argument-list-opt: argument-list
#line 609 "bang.y"
      { yylhs.value.as < named_expression_list_t > () = yystack_[0].value.as < named_expression_list_t > (); }
#line 2954 "bang.tab.cpp"
    break;

  case 77: // argument-list: argument
#line 614 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{std::move(yystack_[0].value.as < named_expression_t > ())}; }
#line 2960 "bang.tab.cpp"
    break;

  case 78: // argument-list: argument-list "," argument
#line 616 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ()); yylhs.value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < named_expression_t > ())); }
#line 2966 "bang.tab.cpp"
    break;

  case 79: // argument: argument-name syntax-expression
#line 621 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2972 "bang.tab.cpp"
    break;

  case 80: // argument: syntax-expression
#line 623 "bang.y"
        { yylhs.value.as < named_expression_t > () = named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 2978 "bang.tab.cpp"
    break;

  case 81: // argument-name: identifier "`=`"
#line 628 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[0].value.as < sonia::lang::lex::resource_location > ()); }
#line 2984 "bang.tab.cpp"
    break;

  case 82: // argument-name: identifier "`:`"
#line 630 "bang.y"
        { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = std::move(yystack_[1].value.as < sonia::lang::bang::annotated_identifier > ()); }
#line 2990 "bang.tab.cpp"
    break;

  case 83: // field-list-opt: %empty
#line 635 "bang.y"
              { yylhs.value.as < field_list_t > () = {}; }
#line 2996 "bang.tab.cpp"
    break;

  case 84: // field-list-opt: field-list
#line 636 "bang.y"
      { yylhs.value.as < field_list_t > () = yystack_[0].value.as < field_list_t > (); }
#line 3002 "bang.tab.cpp"
    break;

  case 85: // field-list: field
#line 641 "bang.y"
        { yylhs.value.as < field_list_t > () = field_list_t{std::move(yystack_[0].value.as < field_t > ())}; }
#line 3008 "bang.tab.cpp"
    break;

  case 86: // field-list: field-list "," field
#line 643 "bang.y"
        { yylhs.value.as < field_list_t > () = std::move(yystack_[2].value.as < field_list_t > ()); yylhs.value.as < field_list_t > ().emplace_back(std::move(yystack_[0].value.as < field_t > ())); }
#line 3014 "bang.tab.cpp"
    break;

  case 87: // field-default-value-opt: %empty
#line 647 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3020 "bang.tab.cpp"
    break;

  case 88: // field-default-value-opt: "`=`" syntax-expression
#line 648 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3026 "bang.tab.cpp"
    break;

  case 89: // field: identifier "`:`" type-expr field-default-value-opt
#line 653 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3032 "bang.tab.cpp"
    break;

  case 90: // field: identifier "`=>`" type-expr field-default-value-opt
#line 655 "bang.y"
        { yylhs.value.as < field_t > () = field_t{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), field_modifier_t::const_value, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3038 "bang.tab.cpp"
    break;

  case 91: // parameter-list-opt: %empty
#line 665 "bang.y"
              { yylhs.value.as < parameter_list_t > () = {}; }
#line 3044 "bang.tab.cpp"
    break;

  case 92: // parameter-list-opt: parameter-list
#line 666 "bang.y"
       { yylhs.value.as < parameter_list_t > () = yystack_[0].value.as < parameter_list_t > (); }
#line 3050 "bang.tab.cpp"
    break;

  case 93: // parameter-list: parameter-decl
#line 671 "bang.y"
        { yylhs.value.as < parameter_list_t > () = parameter_list_t{ parameter_t{std::move(yystack_[0].value.as < parameter_t > ())} }; }
#line 3056 "bang.tab.cpp"
    break;

  case 94: // parameter-list: parameter-list "," parameter-decl
#line 673 "bang.y"
        { yylhs.value.as < parameter_list_t > () = std::move(yystack_[2].value.as < parameter_list_t > ()); yylhs.value.as < parameter_list_t > ().emplace_back(std::move(yystack_[0].value.as < parameter_t > ())); }
#line 3062 "bang.tab.cpp"
    break;

  case 95: // internal-identifier-opt: %empty
#line 677 "bang.y"
             { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = annotated_identifier{}; }
#line 3068 "bang.tab.cpp"
    break;

  case 96: // internal-identifier-opt: internal-identifier
#line 678 "bang.y"
                                 { yylhs.value.as < sonia::lang::bang::annotated_identifier > () = yystack_[0].value.as < sonia::lang::bang::annotated_identifier > (); }
#line 3074 "bang.tab.cpp"
    break;

  case 97: // parameter-default-value-opt: %empty
#line 682 "bang.y"
             { yylhs.value.as < sonia::optional<syntax_expression_t> > () = nullopt; }
#line 3080 "bang.tab.cpp"
    break;

  case 98: // parameter-default-value-opt: "`=`" syntax-expression
#line 683 "bang.y"
                                      { yylhs.value.as < sonia::optional<syntax_expression_t> > () = std::move(yystack_[0].value.as < syntax_expression_t > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3086 "bang.tab.cpp"
    break;

  case 99: // parameter-decl: identifier internal-identifier-opt "`:`" parameter-default-value-opt
#line 688 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::value_type, parameter_constraint_set_t{ .expression = placeholder{} }, std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3092 "bang.tab.cpp"
    break;

  case 100: // parameter-decl: identifier internal-identifier-opt "`:`" parameter-constraint-set parameter-default-value-opt
#line 690 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3098 "bang.tab.cpp"
    break;

  case 101: // parameter-decl: identifier internal-identifier-opt "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 692 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3104 "bang.tab.cpp"
    break;

  case 102: // parameter-decl: identifier internal-identifier-opt "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 694 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[5].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[4].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3110 "bang.tab.cpp"
    break;

  case 103: // parameter-decl: identifier internal-identifier-opt "`=>`" parameter-value-constraint-set
#line 696 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ named_parameter_name{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3116 "bang.tab.cpp"
    break;

  case 104: // parameter-decl: INTERNAL_IDENTIFIER "`:`" parameter-default-value-opt
#line 699 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, parameter_constraint_set_t{ .expression = placeholder{} }, std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3122 "bang.tab.cpp"
    break;

  case 105: // parameter-decl: INTERNAL_IDENTIFIER "`:`" parameter-constraint-set parameter-default-value-opt
#line 701 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3128 "bang.tab.cpp"
    break;

  case 106: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "const modifier" parameter-constraint-set parameter-default-value-opt
#line 703 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3134 "bang.tab.cpp"
    break;

  case 107: // parameter-decl: INTERNAL_IDENTIFIER "`:`" "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 705 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3140 "bang.tab.cpp"
    break;

  case 108: // parameter-decl: INTERNAL_IDENTIFIER "`=>`" parameter-value-constraint-set
#line 707 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3146 "bang.tab.cpp"
    break;

  case 109: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" parameter-constraint-set
#line 710 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[3].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3152 "bang.tab.cpp"
    break;

  case 110: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "const modifier" parameter-constraint-set
#line 712 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); }
#line 3158 "bang.tab.cpp"
    break;

  case 111: // parameter-decl: INTERNAL_IDENTIFIER "`...`" "`:`" "mut modifier" parameter-constraint-set
#line 714 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ varnamed_parameter_name{ ctx.make_identifier(std::move(yystack_[4].value.as < sonia::lang::bang::annotated_string_view > ())) }, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; IGNORE_TERM(yystack_[3].value.as < sonia::lang::lex::resource_location > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3164 "bang.tab.cpp"
    break;

  case 112: // parameter-decl: parameter-constraint-set parameter-default-value-opt
#line 717 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3170 "bang.tab.cpp"
    break;

  case 113: // parameter-decl: "const modifier" parameter-constraint-set parameter-default-value-opt
#line 719 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; }
#line 3176 "bang.tab.cpp"
    break;

  case 114: // parameter-decl: "mut modifier" parameter-constraint-set parameter-default-value-opt
#line 721 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::mutable_value_type, std::move(yystack_[1].value.as < parameter_constraint_set_t > ()), std::move(yystack_[0].value.as < sonia::optional<syntax_expression_t> > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3182 "bang.tab.cpp"
    break;

  case 115: // parameter-decl: "`=>`" parameter-value-constraint-set
#line 723 "bang.y"
        { yylhs.value.as < parameter_t > () = parameter_t{ unnamed_parameter_name{}, parameter_constraint_modifier_t::const_value, std::move(yystack_[0].value.as < parameter_constraint_set_t > ()) }; }
#line 3188 "bang.tab.cpp"
    break;

  case 116: // parameter-constraint-set: parameter-matched-type
#line 728 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3194 "bang.tab.cpp"
    break;

  case 117: // parameter-constraint-set: concept-expression
#line 730 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ .concepts = { std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3200 "bang.tab.cpp"
    break;

  case 118: // parameter-constraint-set: parameter-constraint-set concept-expression
#line 732 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3206 "bang.tab.cpp"
    break;

  case 119: // parameter-value-constraint-set: parameter-matched-value
#line 737 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ .expression = std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3212 "bang.tab.cpp"
    break;

  case 120: // parameter-value-constraint-set: concept-expression
#line 739 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = parameter_constraint_set_t{ .concepts = { std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3218 "bang.tab.cpp"
    break;

  case 121: // parameter-value-constraint-set: parameter-value-constraint-set concept-expression
#line 741 "bang.y"
        { yylhs.value.as < parameter_constraint_set_t > () = std::move(yystack_[1].value.as < parameter_constraint_set_t > ()); yylhs.value.as < parameter_constraint_set_t > ().concepts.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ())); }
#line 3224 "bang.tab.cpp"
    break;

  case 122: // parameter-matched-value: syntax-expression
#line 745 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3230 "bang.tab.cpp"
    break;

  case 123: // parameter-matched-type: basic-parameter-matched-type
#line 749 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3236 "bang.tab.cpp"
    break;

  case 124: // parameter-matched-type: "`...`"
#line 751 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, placeholder{ yystack_[0].value.as < sonia::lang::lex::resource_location > () }, yystack_[0].value.as < sonia::lang::lex::resource_location > () }; }
#line 3242 "bang.tab.cpp"
    break;

  case 125: // parameter-matched-type: basic-parameter-matched-type "`...`"
#line 753 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3248 "bang.tab.cpp"
    break;

  case 126: // parameter-matched-type: INTERNAL_IDENTIFIER
#line 755 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3254 "bang.tab.cpp"
    break;

  case 127: // parameter-matched-type: INTERNAL_IDENTIFIER "`...`"
#line 757 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, variable_reference{ ctx.make_qname(std::move(yystack_[1].value.as < sonia::lang::bang::annotated_string_view > ())), true }, std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3260 "bang.tab.cpp"
    break;

  case 128: // basic-parameter-matched-type: "`_`"
#line 763 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = placeholder{ std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3266 "bang.tab.cpp"
    break;

  case 129: // basic-parameter-matched-type: qname
#line 769 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3272 "bang.tab.cpp"
    break;

  case 130: // basic-parameter-matched-type: qname "`(`" pack-expression "`)`"
#line 772 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3278 "bang.tab.cpp"
    break;

  case 131: // basic-parameter-matched-type: basic-parameter-matched-type "`|`" basic-parameter-matched-type
#line 774 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3284 "bang.tab.cpp"
    break;

  case 132: // concept-expression: "`@`" qname
#line 797 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = syntax_expression_t{ variable_reference{std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()), false} }; }
#line 3290 "bang.tab.cpp"
    break;

  case 133: // syntax-expression: INTERNAL_IDENTIFIER
#line 802 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3296 "bang.tab.cpp"
    break;

  case 134: // syntax-expression: syntax-expression-wo-ii
#line 803 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3302 "bang.tab.cpp"
    break;

  case 135: // syntax-expression-wo-ii: "nil"
#line 809 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_nil > ()); }
#line 3308 "bang.tab.cpp"
    break;

  case 136: // syntax-expression-wo-ii: "true"
#line 811 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3314 "bang.tab.cpp"
    break;

  case 137: // syntax-expression-wo-ii: "false"
#line 813 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < annotated_bool > ()); }
#line 3320 "bang.tab.cpp"
    break;

  case 138: // syntax-expression-wo-ii: INTEGER
#line 815 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_integer > ()); }
#line 3326 "bang.tab.cpp"
    break;

  case 139: // syntax-expression-wo-ii: DECIMAL
#line 817 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3332 "bang.tab.cpp"
    break;

  case 140: // syntax-expression-wo-ii: DECIMAL_S
#line 819 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_decimal > ()); }
#line 3338 "bang.tab.cpp"
    break;

  case 141: // syntax-expression-wo-ii: INTEGER_INDEX
#line 821 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = annotated_decimal{ ctx.make_decimal(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location }; }
#line 3344 "bang.tab.cpp"
    break;

  case 142: // syntax-expression-wo-ii: STRING
#line 823 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_string(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())); }
#line 3350 "bang.tab.cpp"
    break;

  case 143: // syntax-expression-wo-ii: RESERVED_IDENTIFIER
#line 825 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3356 "bang.tab.cpp"
    break;

  case 144: // syntax-expression-wo-ii: qname
#line 827 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3362 "bang.tab.cpp"
    break;

  case 145: // syntax-expression-wo-ii: "`(`" pack-expression "`)`"
#line 829 "bang.y"
        {
            if (yystack_[1].value.as < named_expression_list_t > ().size() == 1 && !yystack_[1].value.as < named_expression_list_t > ().front().has_name()) { // single unnamed expression => extract
                yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < named_expression_list_t > ().front().value());
            } else {
                BOOST_ASSERT(!yystack_[1].value.as < named_expression_list_t > ().empty());
                yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
            }
        }
#line 3375 "bang.tab.cpp"
    break;

  case 146: // syntax-expression-wo-ii: "`[`" expression-list "`]`"
#line 838 "bang.y"
        { 
            if (yystack_[1].value.as < expression_list_t > ().size() == 1) {
                yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > ().front()) };
            } else {
                yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )};
            }
        }
#line 3387 "bang.tab.cpp"
    break;

  case 147: // syntax-expression-wo-ii: "`[[`" expression-list "`]]`"
#line 846 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = array_expression_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < expression_list_t > () )}; }
#line 3393 "bang.tab.cpp"
    break;

  case 148: // syntax-expression-wo-ii: syntax-expression "`[`" syntax-expression "`]`"
#line 848 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3399 "bang.tab.cpp"
    break;

  case 149: // syntax-expression-wo-ii: "`.`" identifier
#line 850 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()); IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3405 "bang.tab.cpp"
    break;

  case 150: // syntax-expression-wo-ii: syntax-expression "`.`" identifier
#line 854 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::bang::annotated_identifier > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3411 "bang.tab.cpp"
    break;

  case 151: // syntax-expression-wo-ii: syntax-expression "`.`" apostrophe-expression
#line 856 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ()); }
#line 3417 "bang.tab.cpp"
    break;

  case 152: // syntax-expression-wo-ii: syntax-expression INTEGER_INDEX
#line 858 "bang.y"
         { yylhs.value.as < syntax_expression_t > () = member_expression_t{ std::move(yystack_[1].value.as < syntax_expression_t > ()), annotated_integer{ ctx.make_integer(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().value.substr(1)), yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ().location } }; IGNORE_TERM(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ()); }
#line 3423 "bang.tab.cpp"
    break;

  case 153: // syntax-expression-wo-ii: "`!`" syntax-expression
#line 861 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::NEGATE, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3429 "bang.tab.cpp"
    break;

  case 154: // syntax-expression-wo-ii: "`*`" syntax-expression
#line 863 "bang.y"
                { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::DEREF, true, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3435 "bang.tab.cpp"
    break;

  case 155: // syntax-expression-wo-ii: syntax-expression "`+`" syntax-expression
#line 868 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::PLUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3441 "bang.tab.cpp"
    break;

  case 156: // syntax-expression-wo-ii: syntax-expression "`-`" syntax-expression
#line 870 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::MINUS, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3447 "bang.tab.cpp"
    break;

  case 157: // syntax-expression-wo-ii: syntax-expression "`==`" syntax-expression
#line 874 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::EQ, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3453 "bang.tab.cpp"
    break;

  case 158: // syntax-expression-wo-ii: syntax-expression "`!=`" syntax-expression
#line 876 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::NE, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3459 "bang.tab.cpp"
    break;

  case 159: // syntax-expression-wo-ii: syntax-expression "`..`" syntax-expression
#line 878 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::CONCAT, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3465 "bang.tab.cpp"
    break;

  case 160: // syntax-expression-wo-ii: syntax-expression "`&`" syntax-expression
#line 881 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3471 "bang.tab.cpp"
    break;

  case 161: // syntax-expression-wo-ii: syntax-expression "`|`" syntax-expression
#line 885 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::BIT_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3477 "bang.tab.cpp"
    break;

  case 162: // syntax-expression-wo-ii: syntax-expression "`&&`" syntax-expression
#line 888 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_AND, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3483 "bang.tab.cpp"
    break;

  case 163: // syntax-expression-wo-ii: syntax-expression "`||`" syntax-expression
#line 891 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = binary_expression_t{ binary_operator_type::LOGIC_OR, std::move(yystack_[2].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3489 "bang.tab.cpp"
    break;

  case 164: // syntax-expression-wo-ii: apostrophe-expression
#line 893 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3495 "bang.tab.cpp"
    break;

  case 165: // syntax-expression-wo-ii: new-expression
#line 894 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3501 "bang.tab.cpp"
    break;

  case 166: // syntax-expression-wo-ii: compound-expression
#line 895 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3507 "bang.tab.cpp"
    break;

  case 167: // syntax-expression-wo-ii: lambda-expression
#line 896 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3513 "bang.tab.cpp"
    break;

  case 168: // apostrophe-expression: "APOSTROPHE" syntax-expression "APOSTROPHE"
#line 901 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = std::move(yystack_[1].value.as < syntax_expression_t > ()); }
#line 3519 "bang.tab.cpp"
    break;

  case 169: // new-expression: "`new`" qname
#line 906 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) } }; }
#line 3525 "bang.tab.cpp"
    break;

  case 170: // new-expression: "`new`" apostrophe-expression
#line 908 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()) }; }
#line 3531 "bang.tab.cpp"
    break;

  case 171: // new-expression: "`new`" qname "`(`" argument-list-opt "`)`"
#line 910 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), variable_reference{ std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3537 "bang.tab.cpp"
    break;

  case 172: // new-expression: "`new`" apostrophe-expression "`(`" argument-list-opt "`)`"
#line 912 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = new_expression_t{ std::move(yystack_[4].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3543 "bang.tab.cpp"
    break;

  case 173: // call-expression: qname "`(`" pack-expression "`)`"
#line 917 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < sonia::lang::bang::annotated_qname > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3549 "bang.tab.cpp"
    break;

  case 174: // call-expression: "mut modifier" "`(`" pack-expression "`)`"
#line 919 "bang.y"
        { 
            auto aid = ctx.make_identifier(annotated_string_view{ "mut"sv, std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()) });
            yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{ qname{ aid.value, true }, std::move(aid.location) }, std::move(yystack_[1].value.as < named_expression_list_t > ()) };
        }
#line 3558 "bang.tab.cpp"
    break;

  case 175: // call-expression: call-expression "`(`" pack-expression "`)`"
#line 924 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3564 "bang.tab.cpp"
    break;

  case 176: // call-expression: apostrophe-expression "`(`" pack-expression "`)`"
#line 926 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3570 "bang.tab.cpp"
    break;

  case 177: // call-expression: lambda-expression "`(`" pack-expression "`)`"
#line 928 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3576 "bang.tab.cpp"
    break;

  case 178: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" braced-statements
#line 933 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[4].value.as < fn_kind > (), std::move(yystack_[3].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[2].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()) }; }
#line 3582 "bang.tab.cpp"
    break;

  case 179: // lambda-expression: fn-start-decl "`(`" parameter-list-opt "`)`" "`->`" type-expr braced-statements
#line 935 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = lambda_t{ yystack_[6].value.as < fn_kind > (), std::move(yystack_[5].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[4].value.as < parameter_list_t > ()), std::move(yystack_[0].value.as < managed_statement_list > ()), std::move(yystack_[1].value.as < syntax_expression_t > ()) }; }
#line 3588 "bang.tab.cpp"
    break;

  case 180: // pack-expression: syntax-expression
#line 940 "bang.y"
        { yylhs.value.as < named_expression_list_t > () = named_expression_list_t{ named_expression_t{ std::move(yystack_[0].value.as < syntax_expression_t > ()) } }; }
#line 3594 "bang.tab.cpp"
    break;

  case 181: // pack-expression: identifier "`:`" syntax-expression
#line 949 "bang.y"
        {
            named_expression_list_t list{};
            list.emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(list);
        }
#line 3604 "bang.tab.cpp"
    break;

  case 182: // pack-expression: pack-expression "," syntax-expression
#line 955 "bang.y"
        {
            yystack_[2].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[2].value.as < named_expression_list_t > ());
        }
#line 3613 "bang.tab.cpp"
    break;

  case 183: // pack-expression: pack-expression "," identifier "`:`" syntax-expression
#line 960 "bang.y"
        {
            yystack_[4].value.as < named_expression_list_t > ().emplace_back(std::move(yystack_[2].value.as < sonia::lang::bang::annotated_identifier > ()), std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < named_expression_list_t > () = std::move(yystack_[4].value.as < named_expression_list_t > ());
        }
#line 3622 "bang.tab.cpp"
    break;

  case 184: // compound-expression: syntax-expression "`...`"
#line 968 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = unary_expression_t{ unary_operator_type::ELLIPSIS, false, std::move(yystack_[1].value.as < syntax_expression_t > ()), std::move(yystack_[0].value.as < sonia::lang::lex::resource_location > ()) }; }
#line 3628 "bang.tab.cpp"
    break;

  case 185: // compound-expression: call-expression
#line 969 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3634 "bang.tab.cpp"
    break;

  case 186: // type-expr: qname
#line 1056 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ std::move(yystack_[0].value.as < sonia::lang::bang::annotated_qname > ()) }; }
#line 3640 "bang.tab.cpp"
    break;

  case 187: // type-expr: call-expression
#line 1057 "bang.y"
      { yylhs.value.as < syntax_expression_t > () = yystack_[0].value.as < syntax_expression_t > (); }
#line 3646 "bang.tab.cpp"
    break;

  case 188: // type-expr: INTERNAL_IDENTIFIER
#line 1059 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = variable_reference{ ctx.make_qname(std::move(yystack_[0].value.as < sonia::lang::bang::annotated_string_view > ())), true }; }
#line 3652 "bang.tab.cpp"
    break;

  case 189: // type-expr: "`[`" type-expr "`]`"
#line 1063 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = bang_vector_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), std::move(yystack_[1].value.as < syntax_expression_t > ())}; }
#line 3658 "bang.tab.cpp"
    break;

  case 190: // type-expr: "`(`" "`)`"
#line 1065 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = ctx.make_void(std::move(yystack_[1].value.as < sonia::lang::lex::resource_location > ())); }
#line 3664 "bang.tab.cpp"
    break;

  case 191: // type-expr: "`(`" pack-expression "`)`"
#line 1068 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = function_call_t{ std::move(yystack_[2].value.as < sonia::lang::lex::resource_location > ()), annotated_qname{}, std::move(yystack_[1].value.as < named_expression_list_t > ()) }; }
#line 3670 "bang.tab.cpp"
    break;

  case 192: // type-expr: type-expr "`[`" INTEGER "`]`"
#line 1070 "bang.y"
        { yylhs.value.as < syntax_expression_t > () = index_expression_t{ std::move(yystack_[3].value.as < syntax_expression_t > ()), std::move(yystack_[1].value.as < sonia::lang::bang::annotated_integer > ()) }; IGNORE_TERM(yystack_[2].value.as < sonia::lang::lex::resource_location > ()); }
#line 3676 "bang.tab.cpp"
    break;

  case 193: // type-expr: type-expr "`|`" type-expr
#line 1072 "bang.y"
        {
            bang_union_t uni{};
            uni.members.emplace_back(std::move(yystack_[2].value.as < syntax_expression_t > ()));
            uni.members.emplace_back(std::move(yystack_[0].value.as < syntax_expression_t > ()));
            yylhs.value.as < syntax_expression_t > () = std::move(uni);
            IGNORE_TERM(yystack_[1].value.as < sonia::lang::lex::resource_location > ());
        }
#line 3688 "bang.tab.cpp"
    break;

  case 194: // type-expr: type-expr "`->`" type-expr
#line 1080 "bang.y"
        { 
            auto loc = get_start_location(yystack_[2].value.as < syntax_expression_t > ());
            yylhs.value.as < syntax_expression_t > () = bang_fn_type_t{ named_expression_list_t{ named_expression_t{ std::move(yystack_[2].value.as < syntax_expression_t > ()) } }, std::move(yystack_[0].value.as < syntax_expression_t > ()), std::move(loc) };
        }
#line 3697 "bang.tab.cpp"
    break;


#line 3701 "bang.tab.cpp"

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


  const short parser::yypact_ninf_ = -217;

  const signed char parser::yytable_ninf_ = -96;

  const short
  parser::yypact_[] =
  {
     738,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,   859,
      32,   859,   859,   859,    32,   859,   859,    -2,    80,    24,
     859,   859,   859,     8,    46,   -75,  -217,     8,     8,    39,
    -217,  -217,  -217,    75,    11,   444,  -217,  -217,  -217,  -217,
     262,    20,   912,  -217,   136,  -217,   140,   155,     6,   175,
     953,  -217,  -217,   181,  1001,    21,   194,  1001,    19,  -217,
      97,    97,    32,  -217,   190,   231,  -217,    32,    28,   278,
     214,   645,   532,   728,    98,  -217,  -217,  -217,   -18,  -217,
     267,  -217,   859,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
      32,   859,    26,  -217,   202,   224,   215,  -217,   859,   859,
     859,   859,   859,   859,  -217,   859,   201,   859,   859,   859,
     859,   859,   859,   859,  -217,   859,  -217,   859,  -217,   859,
    -217,  -217,   311,   859,   233,   238,  -217,   859,   859,   640,
     859,  -217,   859,   210,   859,    26,    32,   247,    26,  -217,
      90,  -217,   124,   213,     8,  -217,   759,  -217,   380,   380,
      42,   298,   264,   234,  -217,   100,  -217,   -10,  -217,    26,
    -217,  1001,   221,   221,   743,   334,   372,   964,  -217,  -217,
      97,    97,   630,   864,   162,   172,   196,  1001,   275,  1001,
    1001,  -217,   838,   311,   262,   136,   140,   155,   219,   253,
     311,    26,    13,   279,   282,  -217,   859,  1001,   304,  -217,
    -217,   859,     8,   302,   542,  -217,   305,  -217,  -217,    20,
     728,   728,   215,  1001,   313,  -217,   319,   301,  -217,    32,
     324,  -217,  -217,   759,   330,   395,   202,   361,  -217,  -217,
    1001,   344,   100,   100,  -217,  -217,   180,   859,    33,    26,
     859,  -217,  -217,  -217,   165,   332,  -217,  -217,  -217,  -217,
     859,  -217,   205,   133,   311,   373,   311,   219,   340,  -217,
    -217,  -217,   859,  1001,  -217,  1001,   255,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,   362,  -217,    32,   183,   343,
     331,  -217,    16,   361,    77,   380,   380,  -217,   100,  -217,
    -217,  -217,  -217,   759,   482,   207,   311,  -217,  -217,  1001,
    -217,   316,  1001,  -217,  -217,  -217,   354,    38,   148,  -217,
      26,   859,  -217,   311,   311,  -217,    32,   366,  -217,   380,
     380,   361,   100,   100,  -217,   361,   380,   380,  -217,   100,
    -217,   217,   311,   859,   311,  -217,   360,  1001,   167,   167,
    -217,    32,   361,   361,  -217,  -217,   100,   100,  -217,  -217,
     219,  1001,   219,   215,   859,  -217,  -217,   367,  -217,  -217,
    1001,  -217
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   142,    51,   133,   143,   141,   138,   139,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
     135,   136,   137,     0,     0,     0,     5,     7,    15,    54,
     144,     0,     0,   134,   164,   165,   185,   167,   166,     0,
       0,   166,    53,    54,   180,     0,     0,    73,     0,   149,
     154,   153,     0,    42,    21,    17,    19,     0,     0,   169,
     170,     0,     0,     0,     0,    43,    13,    57,     0,    39,
       0,    16,     0,     1,     2,    10,     3,     8,     6,     9,
       0,     0,    91,    59,    58,     0,     0,   152,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   168,     0,   145,     0,   146,     0,
     147,    20,     0,     0,     0,     0,    12,    75,    75,    23,
       0,    31,     0,    34,     0,    91,    64,     0,    91,    37,
       0,    55,     0,   126,     0,   128,     0,   124,     0,     0,
      54,   129,     0,    92,    93,    97,   116,   123,   117,    91,
      36,    45,   157,   158,   162,   163,   159,     0,   150,   151,
     155,   156,   160,   161,     0,     0,     0,   181,    54,   182,
      74,   188,     0,     0,   186,     0,   187,     0,    22,    18,
       0,    91,    54,     0,    76,    77,     0,    80,     0,    48,
      47,    49,     0,    40,    41,    26,     0,    46,    24,     0,
       0,     0,     0,    71,     0,    68,     0,    65,    66,    83,
       0,   174,   173,     0,   127,    97,   132,   115,   119,   120,
     122,   126,    97,    97,    52,    96,     0,     0,     0,     0,
       0,   112,   118,   125,     0,     0,   148,   176,   175,   177,
       0,   190,     0,     0,     0,     0,     0,    11,     0,    81,
      82,   171,     0,    79,   172,    50,     0,    29,    27,    28,
      25,    30,    32,    33,    35,     0,    63,     0,     0,     0,
      84,    85,     0,   108,     0,     0,     0,   104,    97,   121,
     127,   113,   114,     0,    97,     0,     0,   178,    94,    98,
     131,    60,   183,   191,   189,   194,     0,   193,    60,    78,
      91,     0,    67,     0,     0,    69,     0,     0,    38,     0,
       0,   109,    97,    97,   105,   103,     0,     0,    99,    97,
     130,     0,     0,     0,     0,   192,     0,    72,    87,    87,
      86,    83,   110,   111,   106,   107,    97,    97,   100,   179,
      62,    14,    61,     0,     0,    90,    89,     0,   101,   102,
      88,    70
  };

  const short
  parser::yypgoto_[] =
  {
    -217,  -217,  -217,  -217,   381,  -217,  -217,  -217,  -217,  -217,
     -48,   -16,  -217,  -128,   216,    30,  -217,   151,     2,   383,
     359,  -217,  -217,  -217,   145,  -217,  -217,   415,   306,  -217,
     171,  -217,    99,  -217,   107,   132,  -130,  -217,  -217,    27,
     218,    12,  -216,  -217,  -217,   222,    96,     0,  -217,    34,
    -217,  -119,  -105,   -32,    10,   -57
  };

  const short
  parser::yydefgoto_[] =
  {
       0,    33,    34,    35,    36,    63,    64,    65,   203,   204,
     139,    37,   206,    38,   208,    39,   235,    40,    49,   125,
      96,    79,   216,   217,   218,    81,    75,    56,   193,   194,
     195,   196,   279,   280,   355,   281,   152,   153,   236,   241,
     154,   155,   227,   228,   156,   157,   158,    54,    43,    44,
      45,    46,    47,    55,    51,   188
  };

  const short
  parser::yytable_[] =
  {
      42,   207,    41,   186,     2,   214,   -44,   283,   220,    50,
      48,    84,    57,    57,     2,    60,    61,   187,    77,    89,
      71,    72,    73,   131,   243,   133,     2,   259,    90,   245,
       2,   136,     2,   143,     2,    42,   317,    41,     2,     9,
      52,    53,   144,   145,    59,    48,   146,    66,   160,   234,
     140,    76,   296,    70,    10,   244,   -44,   254,   260,   142,
     147,   258,   -95,   -44,   186,   129,    10,    92,    85,   116,
      10,   186,    10,   120,    10,    83,   207,   325,   187,   174,
     175,   176,   129,     2,   231,   187,    82,   -95,   119,   255,
     117,   189,   121,   144,   145,    62,    93,   124,   161,   162,
     163,   164,   165,   166,    93,   167,    97,   170,   171,   172,
     173,   147,    53,   205,   240,   177,   144,   179,   134,   180,
     141,    53,   150,    10,   148,   149,   253,   197,   197,    42,
     210,   209,   211,   257,   213,   186,   168,   186,   221,    48,
     169,    53,    53,    53,    90,   135,   230,   178,   105,   187,
     252,   187,   254,    53,    67,   106,   185,   192,   192,   117,
     232,   233,   272,   273,   274,   150,   215,   332,   150,   334,
      69,     2,   222,    68,    74,   319,   320,   186,    78,    80,
     336,   354,   145,   111,   255,   304,   254,   112,   269,   150,
     297,   187,    94,   117,   186,   186,   263,   305,   256,   307,
     293,   265,   113,   313,    42,   295,   209,     2,   187,   187,
     247,    10,    53,   186,    48,   186,     9,   185,   255,    94,
     248,   150,    92,   230,   185,   294,   115,   187,   314,   187,
      97,   117,   256,   223,   318,   122,   254,   288,   254,   331,
     299,   117,   229,   151,   249,   123,   118,   224,    90,   278,
     302,   242,   287,   303,   103,   330,   338,   339,   225,   291,
     292,   128,   197,   119,   129,   117,   129,    53,   255,   150,
     255,   159,   105,   184,   117,   350,   117,   352,   190,   106,
     107,   108,   256,   349,   256,   191,   151,   137,   185,   151,
     185,   212,   192,   230,   219,   226,   321,   322,   323,   151,
     151,    90,   310,   239,   129,   318,   329,   215,    90,    91,
     151,   337,   238,    90,   138,   324,   129,     2,   181,   229,
     250,   328,   117,   289,    90,   127,     9,   261,   242,   242,
     185,   342,   343,   351,   184,   332,   333,   334,   346,   347,
     150,   184,   151,    97,    90,   237,   278,   185,   185,   344,
     345,   262,   264,   266,   360,   271,   348,    10,   182,   267,
      94,   275,   183,    99,   100,   101,   185,   103,   185,   276,
     277,   278,   282,   358,   359,   284,   151,   144,   290,   289,
     301,    97,   311,   306,   242,   105,     2,   231,   308,   229,
     151,   315,   106,   107,   108,   151,   144,   145,   109,   110,
     316,     2,   231,    25,    26,   184,   335,   184,   353,   240,
      29,   144,   145,   341,   147,   361,    88,   242,   242,   242,
     270,   289,   312,   105,    95,   242,    10,   126,    58,   147,
     106,   107,   108,   309,   198,   151,   151,   151,   242,   242,
     357,    10,   242,   242,    86,   151,   356,   184,   340,     1,
       2,     3,     4,     5,     6,     7,     8,   298,     0,     9,
       0,   151,     0,     0,   184,   184,   300,     0,     0,     0,
     151,   151,     0,     0,     0,     0,     0,   151,   151,     0,
       0,     0,     0,   184,     0,   184,     0,     0,     2,   231,
      10,    11,     0,   285,   286,    12,   240,    13,   144,   145,
       0,    87,    14,     0,     0,    15,     0,     0,     0,     0,
      16,     0,     0,     0,     0,     0,   147,    17,     0,    18,
      19,    20,    21,     0,    22,     0,     0,     0,    10,     0,
      23,     0,     0,     0,     0,    24,    25,    26,    27,    28,
       0,    97,     0,    29,    30,    31,    32,     1,     2,     3,
       4,     5,     6,     7,     8,     0,     0,     9,     0,     0,
       0,    99,   100,   101,   102,   103,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,     0,   105,     0,     0,     0,     0,    10,    11,
     106,   107,   108,    12,     0,    13,   109,   110,     0,   268,
      14,     0,     0,    15,     0,     0,     0,     0,    16,     0,
       0,   132,     0,     0,     0,    17,     0,     0,    19,    20,
      21,     0,    22,     0,   199,   200,   201,     0,    23,     0,
       0,     0,     0,     0,    25,    26,    27,   202,     0,    97,
       0,    29,    30,    31,    32,     1,     2,     3,     4,     5,
       6,     7,     8,     0,    97,     9,     0,     0,     0,    99,
     100,     0,     0,   103,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,   100,   101,   102,   103,   104,
       0,   105,     0,     0,     0,     0,    10,    11,   106,   107,
     108,    12,     0,    13,   129,     0,   105,     0,    14,     0,
       0,    15,   130,   106,   107,   108,    16,     0,     0,   109,
     110,     0,     0,    17,     0,     0,    19,    20,    21,     0,
      22,     0,   199,   200,   201,     0,    23,     0,     0,     0,
       0,     0,    25,    26,    27,   202,     0,    97,     0,    29,
      30,    31,    32,     1,     2,     3,     4,     5,     6,     7,
       8,     0,    97,     9,     0,     0,     0,    99,   100,   101,
     102,   103,   104,     0,     1,     2,     3,     4,     5,     6,
       7,     8,    99,   100,     9,   144,   103,   129,     0,   105,
       0,     0,     0,     0,    10,    11,   106,   107,   108,    12,
       0,    13,   109,   110,   105,     0,    14,     0,     0,    15,
       0,   106,   107,   108,    16,    10,    11,   109,   110,     0,
      12,    17,    13,    18,    19,    20,    21,    14,    22,     0,
      15,     0,     0,     0,    23,    16,     0,     0,     0,    24,
      25,    26,    27,    28,     0,    19,     0,    29,    30,    31,
      32,     0,     0,     1,     2,     3,     4,     5,     6,     7,
       8,    25,    26,     9,     0,     0,     0,     0,    29,    30,
      31,    32,     0,     0,     1,     2,     3,     4,     5,     6,
       7,     8,     0,    97,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    11,   251,     0,     0,    12,
       0,    13,     0,    99,   100,     0,    14,   103,     0,    15,
       0,     0,     0,     0,    16,    10,    11,     0,     0,     0,
      12,     0,    13,     0,    19,   105,     0,    14,     0,     0,
      15,    97,   106,   107,   108,    16,    98,     0,   109,     0,
      25,    26,     0,     0,     0,    19,     0,    29,    30,    31,
      32,    99,   100,   101,   102,   103,   104,     0,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    29,    30,
      31,    32,    97,   105,     0,     0,     0,     0,   114,     0,
     106,   107,   108,    97,     0,     0,   109,   110,     0,     0,
       0,     0,    99,   100,   101,   102,   103,   104,     0,     0,
       0,     0,     0,    99,   100,   101,   102,   103,   104,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
      97,   106,   107,   108,     0,   105,   246,   109,   110,     0,
       0,     0,   106,   107,   108,     0,     0,     0,   109,   110,
      99,   100,   101,   102,   103,   104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,     0,     0,   106,
     107,   108,     0,     0,     0,   109,   110
  };

  const short
  parser::yycheck_[] =
  {
       0,   129,     0,   122,     6,   135,     0,   223,   138,     9,
       0,     0,    12,    13,     6,    15,    16,   122,    93,    35,
      20,    21,    22,    71,    34,    73,     6,    14,    46,   159,
       6,    49,     6,     7,     6,    35,    20,    35,     6,    15,
      10,    11,    16,    17,    14,    35,    20,    17,    96,     7,
      82,     5,    19,    19,    46,    65,    50,    19,    45,    91,
      34,   191,    20,    57,   183,    49,    46,    47,    57,    48,
      46,   190,    46,    54,    46,     0,   204,   293,   183,   111,
     112,   113,    49,     6,     7,   190,    47,    45,    69,    51,
      69,   123,    62,    16,    17,    97,    76,    67,    98,    99,
     100,   101,   102,   103,    76,   105,     9,   107,   108,   109,
     110,    34,    82,   129,    14,   115,    16,   117,    20,   119,
      90,    91,    92,    46,    98,    99,   183,   127,   128,   129,
     130,   129,   132,   190,   134,   254,   106,   256,    48,   129,
     106,   111,   112,   113,    46,    47,   146,   117,    51,   254,
     182,   256,    19,   123,    74,    58,   122,   127,   128,    69,
     148,   149,   210,   211,   212,   135,   136,    19,   138,    21,
      19,     6,    48,    93,    23,    98,    99,   296,    27,    28,
     310,    14,    17,    47,    51,    52,    19,    47,   204,   159,
     238,   296,    41,    69,   313,   314,   196,   254,    65,   256,
      20,   201,    47,    20,   204,   237,   204,     6,   313,   314,
      48,    46,   182,   332,   204,   334,    15,   183,    51,    68,
      48,   191,    47,   223,   190,    45,    45,   332,    45,   334,
       9,    69,    65,    20,   282,    45,    19,   225,    19,   296,
     240,    69,   146,    92,    48,    14,    52,    34,    46,   219,
     250,   155,   225,    48,    33,    48,   313,   314,    45,   232,
     233,    47,   262,    69,    49,    69,    49,   237,    51,   239,
      51,    47,    51,   122,    69,   332,    69,   334,    45,    58,
      59,    60,    65,   331,    65,    47,   135,    20,   254,   138,
     256,    81,   262,   293,    47,   144,   284,   285,   286,   148,
     149,    46,    47,    69,    49,   353,   294,   277,    46,    47,
     159,   311,    48,    46,    47,   288,    49,     6,     7,   223,
      45,   294,    69,   227,    46,    47,    15,    48,   232,   233,
     296,   319,   320,   333,   183,    19,    20,    21,   326,   327,
     310,   190,   191,     9,    46,    47,   316,   313,   314,   322,
     323,    69,    48,   202,   354,    50,   329,    46,    47,    57,
     209,    48,    51,    29,    30,    31,   332,    33,   334,    50,
      69,   341,    48,   346,   347,    45,   225,    16,    34,   283,
      48,     9,    20,    10,   288,    51,     6,     7,    48,   293,
     239,    48,    58,    59,    60,   244,    16,    17,    64,    65,
      69,     6,     7,    92,    93,   254,    52,   256,    48,    14,
      99,    16,    17,    47,    34,    48,    35,   321,   322,   323,
     204,   325,   277,    51,    41,   329,    46,    68,    13,    34,
      58,    59,    60,   262,   128,   284,   285,   286,   342,   343,
     341,    46,   346,   347,     0,   294,   339,   296,   316,     5,
       6,     7,     8,     9,    10,    11,    12,   239,    -1,    15,
      -1,   310,    -1,    -1,   313,   314,   244,    -1,    -1,    -1,
     319,   320,    -1,    -1,    -1,    -1,    -1,   326,   327,    -1,
      -1,    -1,    -1,   332,    -1,   334,    -1,    -1,     6,     7,
      46,    47,    -1,    98,    99,    51,    14,    53,    16,    17,
      -1,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    34,    73,    -1,    75,
      76,    77,    78,    -1,    80,    -1,    -1,    -1,    46,    -1,
      86,    -1,    -1,    -1,    -1,    91,    92,    93,    94,    95,
      -1,     9,    -1,    99,   100,   101,   102,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    51,    -1,    -1,    -1,    -1,    46,    47,
      58,    59,    60,    51,    -1,    53,    64,    65,    -1,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    79,    -1,    -1,    -1,    73,    -1,    -1,    76,    77,
      78,    -1,    80,    -1,    82,    83,    84,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,     9,
      -1,    99,   100,   101,   102,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,     9,    15,    -1,    -1,    -1,    29,
      30,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,    34,
      -1,    51,    -1,    -1,    -1,    -1,    46,    47,    58,    59,
      60,    51,    -1,    53,    49,    -1,    51,    -1,    58,    -1,
      -1,    61,    57,    58,    59,    60,    66,    -1,    -1,    64,
      65,    -1,    -1,    73,    -1,    -1,    76,    77,    78,    -1,
      80,    -1,    82,    83,    84,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    -1,     9,    -1,    99,
     100,   101,   102,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,     9,    15,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    -1,     5,     6,     7,     8,     9,    10,
      11,    12,    29,    30,    15,    16,    33,    49,    -1,    51,
      -1,    -1,    -1,    -1,    46,    47,    58,    59,    60,    51,
      -1,    53,    64,    65,    51,    -1,    58,    -1,    -1,    61,
      -1,    58,    59,    60,    66,    46,    47,    64,    65,    -1,
      51,    73,    53,    75,    76,    77,    78,    58,    80,    -1,
      61,    -1,    -1,    -1,    86,    66,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    -1,    76,    -1,    99,   100,   101,
     102,    -1,    -1,     5,     6,     7,     8,     9,    10,    11,
      12,    92,    93,    15,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,     9,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    48,    -1,    -1,    51,
      -1,    53,    -1,    29,    30,    -1,    58,    33,    -1,    61,
      -1,    -1,    -1,    -1,    66,    46,    47,    -1,    -1,    -1,
      51,    -1,    53,    -1,    76,    51,    -1,    58,    -1,    -1,
      61,     9,    58,    59,    60,    66,    14,    -1,    64,    -1,
      92,    93,    -1,    -1,    -1,    76,    -1,    99,   100,   101,
     102,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,     9,    51,    -1,    -1,    -1,    -1,    15,    -1,
      58,    59,    60,     9,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
       9,    58,    59,    60,    -1,    51,    52,    64,    65,    -1,
      -1,    -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    -1,    -1,    64,    65
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      46,    47,    51,    53,    58,    61,    66,    73,    75,    76,
      77,    78,    80,    86,    91,    92,    93,    94,    95,    99,
     100,   101,   102,   104,   105,   106,   107,   114,   116,   118,
     120,   121,   150,   151,   152,   153,   154,   155,   157,   121,
     150,   157,   118,   118,   150,   156,   130,   150,   130,   118,
     150,   150,    97,   108,   109,   110,   118,    74,    93,   120,
     152,   150,   150,   150,   120,   129,     5,    93,   120,   124,
     120,   128,    47,     0,     0,    57,     0,    57,   107,   114,
      46,    47,    47,    76,   120,   122,   123,     9,    14,    29,
      30,    31,    32,    33,    34,    51,    58,    59,    60,    64,
      65,    47,    47,    47,    15,    45,    48,    69,    52,    69,
      54,   118,    45,    14,   118,   122,   123,    47,    47,    49,
      57,   113,    79,   113,    20,    47,    49,    20,    47,   113,
     156,   118,   156,     7,    16,    17,    20,    34,    98,    99,
     118,   120,   139,   140,   143,   144,   147,   148,   149,    47,
     113,   150,   150,   150,   150,   150,   150,   150,   118,   152,
     150,   150,   150,   150,   156,   156,   156,   150,   118,   150,
     150,     7,    47,    51,   120,   152,   154,   155,   158,   156,
      45,    47,   118,   131,   132,   133,   134,   150,   131,    82,
      83,    84,    95,   111,   112,   114,   115,   116,   117,   121,
     150,   150,    81,   150,   139,   118,   125,   126,   127,    47,
     139,    48,    48,    20,    34,    45,   120,   145,   146,   149,
     150,     7,   144,   144,     7,   119,   141,    47,    48,    69,
      14,   142,   149,    34,    65,   139,    52,    48,    48,    48,
      45,    48,   156,   158,    19,    51,    65,   158,   139,    14,
      45,    48,    69,   150,    48,   150,   120,    57,    57,   114,
     117,    50,   113,   113,   113,    48,    50,    69,   118,   135,
     136,   138,    48,   145,    45,    98,    99,   142,   144,   149,
      34,   142,   142,    20,    45,   156,    19,   113,   143,   150,
     148,    48,   150,    48,    52,   158,    10,   158,    48,   133,
      47,    20,   127,    20,    45,    48,    69,    20,   113,    98,
      99,   144,   144,   144,   142,   145,    98,    99,   142,   144,
      48,   158,    19,    20,    21,    52,   139,   150,   158,   158,
     138,    47,   144,   144,   142,   142,   144,   144,   142,   113,
     158,   150,   158,    48,    14,   137,   137,   135,   142,   142,
     150,    48
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   103,   104,   104,   105,   105,   105,   106,   106,   106,
     106,   107,   107,   107,   107,   107,   107,   108,   108,   109,
     109,   110,   110,   111,   111,   111,   112,   112,   112,   112,
     113,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     115,   115,   116,   116,   116,   116,   117,   117,   117,   117,
     117,   118,   119,   120,   120,   120,   121,   121,   122,   122,
     123,   123,   123,   124,   125,   125,   126,   126,   127,   128,
     128,   129,   129,   130,   130,   131,   131,   132,   132,   133,
     133,   134,   134,   135,   135,   136,   136,   137,   137,   138,
     138,   139,   139,   140,   140,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   144,   144,   144,   145,
     145,   145,   146,   147,   147,   147,   147,   147,   148,   148,
     148,   148,   149,   150,   150,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   152,   153,
     153,   153,   153,   154,   154,   154,   154,   154,   155,   155,
     156,   156,   156,   156,   157,   157,   158,   158,   158,   158,
     158,   158,   158,   158,   158
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     1,     2,     1,     2,     2,
       2,     5,     3,     2,     7,     1,     2,     1,     3,     1,
       2,     1,     3,     0,     1,     2,     1,     2,     2,     2,
       3,     3,     5,     5,     3,     5,     3,     3,     6,     2,
       1,     1,     2,     2,     1,     3,     1,     1,     1,     1,
       2,     1,     1,     2,     1,     3,     1,     2,     1,     1,
       4,     6,     6,     4,     0,     1,     1,     3,     1,     5,
       8,     3,     6,     1,     3,     0,     1,     1,     3,     2,
       1,     2,     2,     0,     1,     1,     3,     0,     2,     4,
       4,     0,     1,     1,     3,     0,     1,     0,     2,     4,
       5,     6,     6,     4,     3,     4,     5,     5,     3,     4,
       5,     5,     2,     3,     3,     2,     1,     1,     2,     1,
       1,     2,     1,     1,     1,     2,     1,     2,     1,     1,
       4,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     4,     2,
       3,     3,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     3,     2,
       2,     5,     5,     4,     4,     4,     4,     4,     5,     7,
       1,     3,     3,     5,     2,     1,     1,     1,     1,     3,
       2,     3,     4,     3,     3
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
  "LOWEST", "DEREF", "INCLUDE", "INLINE", "\"`fn`\"", "ENUM", "STRUCT",
  "EXTENDS", "\"weak modifier\"", "\"const modifier\"", "\"mut modifier\"",
  "\"nil\"", "\"true\"", "\"false\"", "$accept", "begin", "statement_any",
  "finished-statement-any", "statement", "let-decl", "let-decl-start",
  "let-decl-start-with-opt-type", "infunction-statement-any",
  "finished-infunction-statement-any", "braced-statements",
  "finished-statement", "infunction-statement-set", "generic-statement",
  "infunction-statement", "identifier", "internal-identifier", "qname",
  "fn-start-decl", "fn-name", "fn-decl", "enum-decl", "case-list-opt",
  "case-list", "case-decl", "struct-decl", "using-decl", "expression-list",
  "argument-list-opt", "argument-list", "argument", "argument-name",
  "field-list-opt", "field-list", "field-default-value-opt", "field",
  "parameter-list-opt", "parameter-list", "internal-identifier-opt",
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
       0,   326,   326,   327,   331,   333,   335,   352,   354,   355,
     357,   362,   364,   366,   368,   376,   378,   383,   385,   390,
     392,   397,   398,   403,   405,   407,   412,   414,   415,   417,
     422,   427,   429,   431,   433,   435,   437,   439,   441,   443,
     448,   450,   455,   457,   459,   461,   466,   468,   470,   472,
     474,   479,   487,   502,   504,   506,   512,   514,   519,   520,
     524,   526,   528,   534,   539,   540,   544,   546,   551,   557,
     559,   582,   586,   600,   602,   608,   609,   613,   615,   620,
     622,   627,   629,   635,   636,   640,   642,   647,   648,   652,
     654,   665,   666,   670,   672,   677,   678,   682,   683,   687,
     689,   691,   693,   695,   698,   700,   702,   704,   706,   709,
     711,   713,   716,   718,   720,   722,   727,   729,   731,   736,
     738,   740,   745,   749,   750,   752,   754,   756,   762,   768,
     771,   773,   796,   801,   803,   808,   810,   812,   814,   816,
     818,   820,   822,   824,   826,   828,   837,   845,   847,   849,
     853,   855,   857,   860,   862,   867,   869,   873,   875,   877,
     880,   884,   887,   890,   893,   894,   895,   896,   900,   905,
     907,   909,   911,   916,   918,   923,   925,   927,   932,   934,
     939,   948,   954,   959,   967,   969,  1055,  1057,  1058,  1062,
    1064,  1067,  1069,  1071,  1079
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
      95,    96,    97,    98,    99,   100,   101,   102
    };
    // Last valid token kind.
    const int code_max = 357;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // bang_lang
#line 4637 "bang.tab.cpp"

#line 1411 "bang.y"

