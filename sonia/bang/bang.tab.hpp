// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file bang.tab.hpp
 ** Define the bang_lang::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_BANG_LANG_BANG_TAB_HPP_INCLUDED
# define YY_BANG_LANG_BANG_TAB_HPP_INCLUDED


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef BANG_LANGDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define BANG_LANGDEBUG 1
#  else
#   define BANG_LANGDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define BANG_LANGDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined BANG_LANGDEBUG */

namespace bang_lang {
#line 190 "bang.tab.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef BANG_LANGSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define BANG_LANGSTYPE in C++, use %define api.value.type"
# endif
    typedef BANG_LANGSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
    {
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {}

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // "true"
      // "false"
      char dummy1[sizeof (annotated_bool)];

      // "nil"
      char dummy2[sizeof (annotated_nil)];

      // concept-expression-list-opt
      // concept-expression-list
      char dummy3[sizeof (concept_expression_list_t)];

      // constraint-expression
      char dummy4[sizeof (constraint_expression_t)];

      // enum-decl
      char dummy5[sizeof (enum_decl)];

      // expression-list
      char dummy6[sizeof (expression_list_t)];

      // field-list-opt
      // field-list
      char dummy7[sizeof (field_list_t)];

      // field
      char dummy8[sizeof (field_t)];

      // finished-statement
      char dummy9[sizeof (finished_statement_type)];

      // fn-start-decl
      char dummy10[sizeof (fn_kind)];

      // fn-decl
      char dummy11[sizeof (fn_pure_t)];

      // generic-statement
      char dummy12[sizeof (generic_statement_type)];

      // let-decl
      // let-decl-start
      // let-decl-start-with-opt-type
      char dummy13[sizeof (let_statement)];

      // statement_any
      // finished-statement-any
      // infunction-statement-any
      // finished-infunction-statement-any
      // braced-statements
      // infunction-statement-set
      char dummy14[sizeof (managed_statement_list)];

      // argument-list-opt
      // argument-list
      // pack-expression
      char dummy15[sizeof (named_expression_list_t)];

      // argument
      char dummy16[sizeof (named_expression_t)];

      // parameter-list-opt
      // parameter-list
      char dummy17[sizeof (parameter_list_t)];

      // parameter-decl
      char dummy18[sizeof (parameter_t)];

      // subpatterns-opt
      // subpatterns
      // pattern-list
      char dummy19[sizeof (pattern_list_t)];

      // pattern
      char dummy20[sizeof (pattern_t)];

      // pattern-field
      char dummy21[sizeof (pattern_t::field)];

      // DECIMAL
      // DECIMAL_S
      char dummy22[sizeof (sonia::lang::bang::annotated_decimal)];

      // identifier
      // internal-identifier
      // argument-name
      // internal-identifier-opt
      char dummy23[sizeof (sonia::lang::bang::annotated_identifier)];

      // INTEGER
      char dummy24[sizeof (sonia::lang::bang::annotated_integer)];

      // qname
      // fn-name
      char dummy25[sizeof (sonia::lang::bang::annotated_qname)];

      // STRING
      // IDENTIFIER
      // CONTEXT_IDENTIFIER
      // RESERVED_IDENTIFIER
      // INTEGER_INDEX
      char dummy26[sizeof (sonia::lang::bang::annotated_string_view)];

      // case-decl
      char dummy27[sizeof (sonia::lang::bang::identifier)];

      // "`=`"
      // "`_`"
      // "`==`"
      // "`!=`"
      // "`&&`"
      // "`||`"
      // "`..`"
      // "`...`"
      // "`(`"
      // "`{`"
      // "`[`"
      // "`[[`"
      // "`.`"
      // "`+`"
      // "`-`"
      // "`*`"
      // "`&`"
      // "`|`"
      // "`!`"
      // "`new`"
      // "`continue`"
      // "`break`"
      // "`return`"
      // "`fn`"
      // "mut modifier"
      char dummy28[sizeof (sonia::lang::lex::resource_location)];

      // field-default-value-opt
      // parameter-default-value-opt
      char dummy29[sizeof (sonia::optional<syntax_expression_t>)];

      // OPERATOR_TERM
      char dummy30[sizeof (sonia::string_view)];

      // statement
      // infunction-statement
      char dummy31[sizeof (statement)];

      // case-list-opt
      // case-list
      char dummy32[sizeof (std::vector<sonia::lang::bang::identifier>)];

      // struct-decl
      char dummy33[sizeof (struct_decl)];

      // concept-expression
      // syntax-expression
      // syntax-expression-wo-ii
      // apostrophe-expression
      // new-expression
      // call-expression
      // lambda-expression
      // compound-expression
      // type-expr
      char dummy34[sizeof (syntax_expression_t)];

      // using-decl
      char dummy35[sizeof (using_decl)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        BANG_LANGEMPTY = -2,
    END = 0,                       // "end of file"
    BANG_LANGerror = 256,          // error
    BANG_LANGUNDEF = 257,          // "invalid token"
    COMMENT_BEGIN = 258,           // COMMENT_BEGIN
    COMMENT_END = 259,             // COMMENT_END
    STRING = 260,                  // STRING
    IDENTIFIER = 261,              // IDENTIFIER
    CONTEXT_IDENTIFIER = 262,      // CONTEXT_IDENTIFIER
    RESERVED_IDENTIFIER = 263,     // RESERVED_IDENTIFIER
    INTEGER_INDEX = 264,           // INTEGER_INDEX
    INTEGER = 265,                 // INTEGER
    DECIMAL = 266,                 // DECIMAL
    DECIMAL_S = 267,               // DECIMAL_S
    OPERATOR_TERM = 268,           // OPERATOR_TERM
    ASSIGN = 269,                  // "`=`"
    APOSTROPHE = 270,              // "APOSTROPHE"
    AT_SYMBOL = 271,               // "`@`"
    UNDERSCORE = 272,              // "`_`"
    ARROWAST = 273,                // "`->*`"
    ARROW = 274,                   // "`->`"
    ARROWEXPR = 275,               // "`=>`"
    FARROW = 276,                  // "`~>`"
    PTAST = 277,                   // "`.*`"
    DBLPLUS = 278,                 // "`++`"
    DBLMINUS = 279,                // "`--`"
    LEFTSHIFT = 280,               // "`<<`"
    RIGHTSHIFT = 281,              // "`>>`"
    LE = 282,                      // "`<=`"
    GE = 283,                      // "`>=`"
    EQ = 284,                      // "`==`"
    NE = 285,                      // "`!=`"
    LOGIC_AND = 286,               // "`&&`"
    LOGIC_OR = 287,                // "`||`"
    CONCAT = 288,                  // "`..`"
    ELLIPSIS = 289,                // "`...`"
    ADDASSIGN = 290,               // "`+=`"
    SUBASSIGN = 291,               // "`-=`"
    MULASSIGN = 292,               // "`*=`"
    DIVASSIGN = 293,               // "`/=`"
    MODASSIGN = 294,               // "`%=`"
    LSHIFTSSIGN = 295,             // "`<<=`"
    RSHIFTSSIGN = 296,             // "`>>=`"
    BITANDASSIGN = 297,            // "`&=`"
    BITORASSIGN = 298,             // "`|=`"
    BITXORASSIGN = 299,            // "`^=`"
    COLON = 300,                   // "`:`"
    DBLCOLON = 301,                // "`::`"
    OPEN_PARENTHESIS = 302,        // "`(`"
    CLOSE_PARENTHESIS = 303,       // "`)`"
    OPEN_BRACE = 304,              // "`{`"
    CLOSE_BRACE = 305,             // "`}`"
    OPEN_SQUARE_BRACKET = 306,     // "`[`"
    CLOSE_SQUARE_BRACKET = 307,    // "`]`"
    OPEN_SQUARE_DBL_BRACKET = 308, // "`[[`"
    CLOSE_SQUARE_DBL_BRACKET = 309, // "`]]`"
    OPEN_BROKET = 310,             // "`<`"
    CLOSE_BROKET = 311,            // "`>`"
    END_STATEMENT = 312,           // "`;`"
    POINT = 313,                   // "`.`"
    PLUS = 314,                    // "`+`"
    MINUS = 315,                   // "`-`"
    ASTERISK = 316,                // "`*`"
    SLASH = 317,                   // "`/`"
    PERCENT = 318,                 // "`%`"
    AMPERSAND = 319,               // "`&`"
    BITOR = 320,                   // "`|`"
    EXCLPT = 321,                  // "`!`"
    TILDA = 322,                   // "`~`"
    EXCL = 323,                    // "`^`"
    COMMA = 324,                   // ","
    DOLLAR = 325,                  // "`$`"
    QMARK = 326,                   // "`?`"
    HASHTAG = 327,                 // "`#`"
    LET = 328,                     // LET
    VAR = 329,                     // VAR
    EXTERN = 330,                  // EXTERN
    NEW = 331,                     // "`new`"
    WHILE = 332,                   // "`while`"
    FOR = 333,                     // "`for`"
    IN = 334,                      // "`in`"
    IF = 335,                      // "`if`"
    ELSE = 336,                    // "`else`"
    CONTINUE = 337,                // "`continue`"
    BREAK = 338,                   // "`break`"
    RETURN = 339,                  // "`return`"
    AUTO = 340,                    // AUTO
    USING = 341,                   // USING
    THROW = 342,                   // THROW
    SIZEOF = 343,                  // SIZEOF
    LOWEST = 344,                  // LOWEST
    DEREF = 345,                   // DEREF
    INCLUDE = 346,                 // INCLUDE
    INLINE = 347,                  // INLINE
    FN = 348,                      // "`fn`"
    ENUM = 349,                    // ENUM
    STRUCT = 350,                  // STRUCT
    EXTENDS = 351,                 // EXTENDS
    WEAK = 352,                    // "weak modifier"
    CONST = 353,                   // "const modifier"
    MUT = 354,                     // "mut modifier"
    NIL_WORD = 355,                // "nil"
    TRUE_WORD = 356,               // "true"
    FALSE_WORD = 357               // "false"
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 103, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_COMMENT_BEGIN = 3,                     // COMMENT_BEGIN
        S_COMMENT_END = 4,                       // COMMENT_END
        S_STRING = 5,                            // STRING
        S_IDENTIFIER = 6,                        // IDENTIFIER
        S_CONTEXT_IDENTIFIER = 7,                // CONTEXT_IDENTIFIER
        S_RESERVED_IDENTIFIER = 8,               // RESERVED_IDENTIFIER
        S_INTEGER_INDEX = 9,                     // INTEGER_INDEX
        S_INTEGER = 10,                          // INTEGER
        S_DECIMAL = 11,                          // DECIMAL
        S_DECIMAL_S = 12,                        // DECIMAL_S
        S_OPERATOR_TERM = 13,                    // OPERATOR_TERM
        S_ASSIGN = 14,                           // "`=`"
        S_APOSTROPHE = 15,                       // "APOSTROPHE"
        S_AT_SYMBOL = 16,                        // "`@`"
        S_UNDERSCORE = 17,                       // "`_`"
        S_ARROWAST = 18,                         // "`->*`"
        S_ARROW = 19,                            // "`->`"
        S_ARROWEXPR = 20,                        // "`=>`"
        S_FARROW = 21,                           // "`~>`"
        S_PTAST = 22,                            // "`.*`"
        S_DBLPLUS = 23,                          // "`++`"
        S_DBLMINUS = 24,                         // "`--`"
        S_LEFTSHIFT = 25,                        // "`<<`"
        S_RIGHTSHIFT = 26,                       // "`>>`"
        S_LE = 27,                               // "`<=`"
        S_GE = 28,                               // "`>=`"
        S_EQ = 29,                               // "`==`"
        S_NE = 30,                               // "`!=`"
        S_LOGIC_AND = 31,                        // "`&&`"
        S_LOGIC_OR = 32,                         // "`||`"
        S_CONCAT = 33,                           // "`..`"
        S_ELLIPSIS = 34,                         // "`...`"
        S_ADDASSIGN = 35,                        // "`+=`"
        S_SUBASSIGN = 36,                        // "`-=`"
        S_MULASSIGN = 37,                        // "`*=`"
        S_DIVASSIGN = 38,                        // "`/=`"
        S_MODASSIGN = 39,                        // "`%=`"
        S_LSHIFTSSIGN = 40,                      // "`<<=`"
        S_RSHIFTSSIGN = 41,                      // "`>>=`"
        S_BITANDASSIGN = 42,                     // "`&=`"
        S_BITORASSIGN = 43,                      // "`|=`"
        S_BITXORASSIGN = 44,                     // "`^=`"
        S_COLON = 45,                            // "`:`"
        S_DBLCOLON = 46,                         // "`::`"
        S_OPEN_PARENTHESIS = 47,                 // "`(`"
        S_CLOSE_PARENTHESIS = 48,                // "`)`"
        S_OPEN_BRACE = 49,                       // "`{`"
        S_CLOSE_BRACE = 50,                      // "`}`"
        S_OPEN_SQUARE_BRACKET = 51,              // "`[`"
        S_CLOSE_SQUARE_BRACKET = 52,             // "`]`"
        S_OPEN_SQUARE_DBL_BRACKET = 53,          // "`[[`"
        S_CLOSE_SQUARE_DBL_BRACKET = 54,         // "`]]`"
        S_OPEN_BROKET = 55,                      // "`<`"
        S_CLOSE_BROKET = 56,                     // "`>`"
        S_END_STATEMENT = 57,                    // "`;`"
        S_POINT = 58,                            // "`.`"
        S_PLUS = 59,                             // "`+`"
        S_MINUS = 60,                            // "`-`"
        S_ASTERISK = 61,                         // "`*`"
        S_SLASH = 62,                            // "`/`"
        S_PERCENT = 63,                          // "`%`"
        S_AMPERSAND = 64,                        // "`&`"
        S_BITOR = 65,                            // "`|`"
        S_EXCLPT = 66,                           // "`!`"
        S_TILDA = 67,                            // "`~`"
        S_EXCL = 68,                             // "`^`"
        S_COMMA = 69,                            // ","
        S_DOLLAR = 70,                           // "`$`"
        S_QMARK = 71,                            // "`?`"
        S_HASHTAG = 72,                          // "`#`"
        S_LET = 73,                              // LET
        S_VAR = 74,                              // VAR
        S_EXTERN = 75,                           // EXTERN
        S_NEW = 76,                              // "`new`"
        S_WHILE = 77,                            // "`while`"
        S_FOR = 78,                              // "`for`"
        S_IN = 79,                               // "`in`"
        S_IF = 80,                               // "`if`"
        S_ELSE = 81,                             // "`else`"
        S_CONTINUE = 82,                         // "`continue`"
        S_BREAK = 83,                            // "`break`"
        S_RETURN = 84,                           // "`return`"
        S_AUTO = 85,                             // AUTO
        S_USING = 86,                            // USING
        S_THROW = 87,                            // THROW
        S_SIZEOF = 88,                           // SIZEOF
        S_LOWEST = 89,                           // LOWEST
        S_DEREF = 90,                            // DEREF
        S_INCLUDE = 91,                          // INCLUDE
        S_INLINE = 92,                           // INLINE
        S_FN = 93,                               // "`fn`"
        S_ENUM = 94,                             // ENUM
        S_STRUCT = 95,                           // STRUCT
        S_EXTENDS = 96,                          // EXTENDS
        S_WEAK = 97,                             // "weak modifier"
        S_CONST = 98,                            // "const modifier"
        S_MUT = 99,                              // "mut modifier"
        S_NIL_WORD = 100,                        // "nil"
        S_TRUE_WORD = 101,                       // "true"
        S_FALSE_WORD = 102,                      // "false"
        S_YYACCEPT = 103,                        // $accept
        S_begin = 104,                           // begin
        S_statement_any = 105,                   // statement_any
        S_106_finished_statement_any = 106,      // finished-statement-any
        S_statement = 107,                       // statement
        S_108_let_decl = 108,                    // let-decl
        S_109_let_decl_start = 109,              // let-decl-start
        S_110_let_decl_start_with_opt_type = 110, // let-decl-start-with-opt-type
        S_111_infunction_statement_any = 111,    // infunction-statement-any
        S_112_finished_infunction_statement_any = 112, // finished-infunction-statement-any
        S_113_braced_statements = 113,           // braced-statements
        S_114_finished_statement = 114,          // finished-statement
        S_115_infunction_statement_set = 115,    // infunction-statement-set
        S_116_generic_statement = 116,           // generic-statement
        S_117_infunction_statement = 117,        // infunction-statement
        S_identifier = 118,                      // identifier
        S_119_internal_identifier = 119,         // internal-identifier
        S_qname = 120,                           // qname
        S_121_fn_start_decl = 121,               // fn-start-decl
        S_122_fn_name = 122,                     // fn-name
        S_123_fn_decl = 123,                     // fn-decl
        S_124_enum_decl = 124,                   // enum-decl
        S_125_case_list_opt = 125,               // case-list-opt
        S_126_case_list = 126,                   // case-list
        S_127_case_decl = 127,                   // case-decl
        S_128_struct_decl = 128,                 // struct-decl
        S_129_using_decl = 129,                  // using-decl
        S_130_expression_list = 130,             // expression-list
        S_131_argument_list_opt = 131,           // argument-list-opt
        S_132_argument_list = 132,               // argument-list
        S_argument = 133,                        // argument
        S_134_argument_name = 134,               // argument-name
        S_135_field_list_opt = 135,              // field-list-opt
        S_136_field_list = 136,                  // field-list
        S_137_field_default_value_opt = 137,     // field-default-value-opt
        S_field = 138,                           // field
        S_139_parameter_list_opt = 139,          // parameter-list-opt
        S_140_parameter_list = 140,              // parameter-list
        S_141_internal_identifier_opt = 141,     // internal-identifier-opt
        S_142_parameter_default_value_opt = 142, // parameter-default-value-opt
        S_143_parameter_decl = 143,              // parameter-decl
        S_144_constraint_expression = 144,       // constraint-expression
        S_145_subpatterns_opt = 145,             // subpatterns-opt
        S_subpatterns = 146,                     // subpatterns
        S_147_pattern_list = 147,                // pattern-list
        S_148_pattern_field = 148,               // pattern-field
        S_pattern = 149,                         // pattern
        S_150_concept_expression = 150,          // concept-expression
        S_151_concept_expression_list_opt = 151, // concept-expression-list-opt
        S_152_concept_expression_list = 152,     // concept-expression-list
        S_153_syntax_expression = 153,           // syntax-expression
        S_154_syntax_expression_wo_ii = 154,     // syntax-expression-wo-ii
        S_155_apostrophe_expression = 155,       // apostrophe-expression
        S_156_new_expression = 156,              // new-expression
        S_157_call_expression = 157,             // call-expression
        S_158_lambda_expression = 158,           // lambda-expression
        S_159_pack_expression = 159,             // pack-expression
        S_160_compound_expression = 160,         // compound-expression
        S_161_type_expr = 161                    // type-expr
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.move< annotated_bool > (std::move (that.value));
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.move< annotated_nil > (std::move (that.value));
        break;

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.move< concept_expression_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.move< constraint_expression_t > (std::move (that.value));
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.move< enum_decl > (std::move (that.value));
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.move< expression_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.move< field_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_field: // field
        value.move< field_t > (std::move (that.value));
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.move< finished_statement_type > (std::move (that.value));
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (std::move (that.value));
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.move< fn_pure_t > (std::move (that.value));
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.move< generic_statement_type > (std::move (that.value));
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement > (std::move (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.move< managed_statement_list > (std::move (that.value));
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.move< named_expression_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< named_expression_t > (std::move (that.value));
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.move< parameter_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.move< parameter_t > (std::move (that.value));
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.move< pattern_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_pattern: // pattern
        value.move< pattern_t > (std::move (that.value));
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.move< pattern_t::field > (std::move (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.move< sonia::lang::bang::annotated_decimal > (std::move (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.move< sonia::lang::bang::annotated_identifier > (std::move (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (std::move (that.value));
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.move< sonia::lang::bang::annotated_qname > (std::move (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.move< sonia::lang::bang::annotated_string_view > (std::move (that.value));
        break;

      case symbol_kind::S_127_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (std::move (that.value));
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
        value.move< sonia::lang::lex::resource_location > (std::move (that.value));
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.move< sonia::optional<syntax_expression_t> > (std::move (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (std::move (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.move< statement > (std::move (that.value));
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (std::move (that.value));
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.move< struct_decl > (std::move (that.value));
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.move< syntax_expression_t > (std::move (that.value));
        break;

      case symbol_kind::S_129_using_decl: // using-decl
        value.move< using_decl > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, annotated_bool&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const annotated_bool& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, annotated_nil&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const annotated_nil& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, concept_expression_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const concept_expression_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, constraint_expression_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const constraint_expression_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, enum_decl&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const enum_decl& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, expression_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const expression_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, field_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const field_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, field_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const field_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, finished_statement_type&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const finished_statement_type& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, fn_kind&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const fn_kind& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, fn_pure_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const fn_pure_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, generic_statement_type&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const generic_statement_type& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, let_statement&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const let_statement& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, managed_statement_list&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const managed_statement_list& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, named_expression_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const named_expression_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, named_expression_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const named_expression_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, parameter_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const parameter_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, parameter_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const parameter_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, pattern_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const pattern_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, pattern_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const pattern_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, pattern_t::field&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const pattern_t::field& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::bang::annotated_decimal&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::bang::annotated_decimal& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::bang::annotated_identifier&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::bang::annotated_identifier& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::bang::annotated_integer&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::bang::annotated_integer& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::bang::annotated_qname&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::bang::annotated_qname& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::bang::annotated_string_view&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::bang::annotated_string_view& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::bang::identifier&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::bang::identifier& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::lex::resource_location&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::lex::resource_location& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::optional<syntax_expression_t>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::optional<syntax_expression_t>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::string_view&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::string_view& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, statement&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const statement& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<sonia::lang::bang::identifier>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<sonia::lang::bang::identifier>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, struct_decl&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const struct_decl& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, syntax_expression_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const syntax_expression_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, using_decl&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const using_decl& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
      case symbol_kind::S_STRING: // STRING
#line 324 "bang.y"
                    { }
#line 1646 "bang.tab.hpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 324 "bang.y"
                    { }
#line 1652 "bang.tab.hpp"
        break;

      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
#line 324 "bang.y"
                    { }
#line 1658 "bang.tab.hpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 324 "bang.y"
                    { }
#line 1664 "bang.tab.hpp"
        break;

      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
#line 324 "bang.y"
                    { }
#line 1670 "bang.tab.hpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 324 "bang.y"
                    { }
#line 1676 "bang.tab.hpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 324 "bang.y"
                    { }
#line 1682 "bang.tab.hpp"
        break;

      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
#line 324 "bang.y"
                    { }
#line 1688 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 324 "bang.y"
                    { }
#line 1694 "bang.tab.hpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 324 "bang.y"
                    { }
#line 1700 "bang.tab.hpp"
        break;

      case symbol_kind::S_UNDERSCORE: // "`_`"
#line 324 "bang.y"
                    { }
#line 1706 "bang.tab.hpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 324 "bang.y"
                    { }
#line 1712 "bang.tab.hpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 324 "bang.y"
                    { }
#line 1718 "bang.tab.hpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 324 "bang.y"
                    { }
#line 1724 "bang.tab.hpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 324 "bang.y"
                    { }
#line 1730 "bang.tab.hpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 324 "bang.y"
                    { }
#line 1736 "bang.tab.hpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 324 "bang.y"
                    { }
#line 1742 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 324 "bang.y"
                    { }
#line 1748 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 324 "bang.y"
                    { }
#line 1754 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 324 "bang.y"
                    { }
#line 1760 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_DBL_BRACKET: // "`[[`"
#line 324 "bang.y"
                    { }
#line 1766 "bang.tab.hpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 324 "bang.y"
                    { }
#line 1772 "bang.tab.hpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 324 "bang.y"
                    { }
#line 1778 "bang.tab.hpp"
        break;

      case symbol_kind::S_MINUS: // "`-`"
#line 324 "bang.y"
                    { }
#line 1784 "bang.tab.hpp"
        break;

      case symbol_kind::S_ASTERISK: // "`*`"
#line 324 "bang.y"
                    { }
#line 1790 "bang.tab.hpp"
        break;

      case symbol_kind::S_AMPERSAND: // "`&`"
#line 324 "bang.y"
                    { }
#line 1796 "bang.tab.hpp"
        break;

      case symbol_kind::S_BITOR: // "`|`"
#line 324 "bang.y"
                    { }
#line 1802 "bang.tab.hpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 324 "bang.y"
                    { }
#line 1808 "bang.tab.hpp"
        break;

      case symbol_kind::S_NEW: // "`new`"
#line 324 "bang.y"
                    { }
#line 1814 "bang.tab.hpp"
        break;

      case symbol_kind::S_CONTINUE: // "`continue`"
#line 324 "bang.y"
                    { }
#line 1820 "bang.tab.hpp"
        break;

      case symbol_kind::S_BREAK: // "`break`"
#line 324 "bang.y"
                    { }
#line 1826 "bang.tab.hpp"
        break;

      case symbol_kind::S_RETURN: // "`return`"
#line 324 "bang.y"
                    { }
#line 1832 "bang.tab.hpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 324 "bang.y"
                    { }
#line 1838 "bang.tab.hpp"
        break;

      case symbol_kind::S_MUT: // "mut modifier"
#line 324 "bang.y"
                    { }
#line 1844 "bang.tab.hpp"
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
#line 324 "bang.y"
                    { }
#line 1850 "bang.tab.hpp"
        break;

      case symbol_kind::S_TRUE_WORD: // "true"
#line 324 "bang.y"
                    { }
#line 1856 "bang.tab.hpp"
        break;

      case symbol_kind::S_FALSE_WORD: // "false"
#line 324 "bang.y"
                    { }
#line 1862 "bang.tab.hpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 324 "bang.y"
                    { }
#line 1868 "bang.tab.hpp"
        break;

      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
#line 324 "bang.y"
                    { }
#line 1874 "bang.tab.hpp"
        break;

      case symbol_kind::S_statement: // statement
#line 324 "bang.y"
                    { }
#line 1880 "bang.tab.hpp"
        break;

      case symbol_kind::S_108_let_decl: // let-decl
#line 324 "bang.y"
                    { }
#line 1886 "bang.tab.hpp"
        break;

      case symbol_kind::S_109_let_decl_start: // let-decl-start
#line 324 "bang.y"
                    { }
#line 1892 "bang.tab.hpp"
        break;

      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 324 "bang.y"
                    { }
#line 1898 "bang.tab.hpp"
        break;

      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
#line 324 "bang.y"
                    { }
#line 1904 "bang.tab.hpp"
        break;

      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
#line 324 "bang.y"
                    { }
#line 1910 "bang.tab.hpp"
        break;

      case symbol_kind::S_113_braced_statements: // braced-statements
#line 324 "bang.y"
                    { }
#line 1916 "bang.tab.hpp"
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
#line 324 "bang.y"
                    { }
#line 1922 "bang.tab.hpp"
        break;

      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
#line 324 "bang.y"
                    { }
#line 1928 "bang.tab.hpp"
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
#line 324 "bang.y"
                    { }
#line 1934 "bang.tab.hpp"
        break;

      case symbol_kind::S_117_infunction_statement: // infunction-statement
#line 324 "bang.y"
                    { }
#line 1940 "bang.tab.hpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 324 "bang.y"
                    { }
#line 1946 "bang.tab.hpp"
        break;

      case symbol_kind::S_119_internal_identifier: // internal-identifier
#line 324 "bang.y"
                    { }
#line 1952 "bang.tab.hpp"
        break;

      case symbol_kind::S_qname: // qname
#line 324 "bang.y"
                    { }
#line 1958 "bang.tab.hpp"
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
#line 324 "bang.y"
                    { }
#line 1964 "bang.tab.hpp"
        break;

      case symbol_kind::S_122_fn_name: // fn-name
#line 324 "bang.y"
                    { }
#line 1970 "bang.tab.hpp"
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
#line 324 "bang.y"
                    { }
#line 1976 "bang.tab.hpp"
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
#line 324 "bang.y"
                    { }
#line 1982 "bang.tab.hpp"
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
#line 324 "bang.y"
                    { }
#line 1988 "bang.tab.hpp"
        break;

      case symbol_kind::S_126_case_list: // case-list
#line 324 "bang.y"
                    { }
#line 1994 "bang.tab.hpp"
        break;

      case symbol_kind::S_127_case_decl: // case-decl
#line 324 "bang.y"
                    { }
#line 2000 "bang.tab.hpp"
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
#line 324 "bang.y"
                    { }
#line 2006 "bang.tab.hpp"
        break;

      case symbol_kind::S_129_using_decl: // using-decl
#line 324 "bang.y"
                    { }
#line 2012 "bang.tab.hpp"
        break;

      case symbol_kind::S_130_expression_list: // expression-list
#line 324 "bang.y"
                    { }
#line 2018 "bang.tab.hpp"
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
#line 324 "bang.y"
                    { }
#line 2024 "bang.tab.hpp"
        break;

      case symbol_kind::S_132_argument_list: // argument-list
#line 324 "bang.y"
                    { }
#line 2030 "bang.tab.hpp"
        break;

      case symbol_kind::S_argument: // argument
#line 324 "bang.y"
                    { }
#line 2036 "bang.tab.hpp"
        break;

      case symbol_kind::S_134_argument_name: // argument-name
#line 324 "bang.y"
                    { }
#line 2042 "bang.tab.hpp"
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
#line 324 "bang.y"
                    { }
#line 2048 "bang.tab.hpp"
        break;

      case symbol_kind::S_136_field_list: // field-list
#line 324 "bang.y"
                    { }
#line 2054 "bang.tab.hpp"
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
#line 324 "bang.y"
                    { }
#line 2060 "bang.tab.hpp"
        break;

      case symbol_kind::S_field: // field
#line 324 "bang.y"
                    { }
#line 2066 "bang.tab.hpp"
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
#line 324 "bang.y"
                    { }
#line 2072 "bang.tab.hpp"
        break;

      case symbol_kind::S_140_parameter_list: // parameter-list
#line 324 "bang.y"
                    { }
#line 2078 "bang.tab.hpp"
        break;

      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
#line 324 "bang.y"
                    { }
#line 2084 "bang.tab.hpp"
        break;

      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
#line 324 "bang.y"
                    { }
#line 2090 "bang.tab.hpp"
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
#line 324 "bang.y"
                    { }
#line 2096 "bang.tab.hpp"
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
#line 324 "bang.y"
                    { }
#line 2102 "bang.tab.hpp"
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
#line 324 "bang.y"
                    { }
#line 2108 "bang.tab.hpp"
        break;

      case symbol_kind::S_subpatterns: // subpatterns
#line 324 "bang.y"
                    { }
#line 2114 "bang.tab.hpp"
        break;

      case symbol_kind::S_147_pattern_list: // pattern-list
#line 324 "bang.y"
                    { }
#line 2120 "bang.tab.hpp"
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
#line 324 "bang.y"
                    { }
#line 2126 "bang.tab.hpp"
        break;

      case symbol_kind::S_pattern: // pattern
#line 324 "bang.y"
                    { }
#line 2132 "bang.tab.hpp"
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
#line 324 "bang.y"
                    { }
#line 2138 "bang.tab.hpp"
        break;

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
#line 324 "bang.y"
                    { }
#line 2144 "bang.tab.hpp"
        break;

      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
#line 324 "bang.y"
                    { }
#line 2150 "bang.tab.hpp"
        break;

      case symbol_kind::S_153_syntax_expression: // syntax-expression
#line 324 "bang.y"
                    { }
#line 2156 "bang.tab.hpp"
        break;

      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 324 "bang.y"
                    { }
#line 2162 "bang.tab.hpp"
        break;

      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
#line 324 "bang.y"
                    { }
#line 2168 "bang.tab.hpp"
        break;

      case symbol_kind::S_156_new_expression: // new-expression
#line 324 "bang.y"
                    { }
#line 2174 "bang.tab.hpp"
        break;

      case symbol_kind::S_157_call_expression: // call-expression
#line 324 "bang.y"
                    { }
#line 2180 "bang.tab.hpp"
        break;

      case symbol_kind::S_158_lambda_expression: // lambda-expression
#line 324 "bang.y"
                    { }
#line 2186 "bang.tab.hpp"
        break;

      case symbol_kind::S_159_pack_expression: // pack-expression
#line 324 "bang.y"
                    { }
#line 2192 "bang.tab.hpp"
        break;

      case symbol_kind::S_160_compound_expression: // compound-expression
#line 324 "bang.y"
                    { }
#line 2198 "bang.tab.hpp"
        break;

      case symbol_kind::S_161_type_expr: // type-expr
#line 324 "bang.y"
                    { }
#line 2204 "bang.tab.hpp"
        break;

       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_TRUE_WORD: // "true"
      case symbol_kind::S_FALSE_WORD: // "false"
        value.template destroy< annotated_bool > ();
        break;

      case symbol_kind::S_NIL_WORD: // "nil"
        value.template destroy< annotated_nil > ();
        break;

      case symbol_kind::S_151_concept_expression_list_opt: // concept-expression-list-opt
      case symbol_kind::S_152_concept_expression_list: // concept-expression-list
        value.template destroy< concept_expression_list_t > ();
        break;

      case symbol_kind::S_144_constraint_expression: // constraint-expression
        value.template destroy< constraint_expression_t > ();
        break;

      case symbol_kind::S_124_enum_decl: // enum-decl
        value.template destroy< enum_decl > ();
        break;

      case symbol_kind::S_130_expression_list: // expression-list
        value.template destroy< expression_list_t > ();
        break;

      case symbol_kind::S_135_field_list_opt: // field-list-opt
      case symbol_kind::S_136_field_list: // field-list
        value.template destroy< field_list_t > ();
        break;

      case symbol_kind::S_field: // field
        value.template destroy< field_t > ();
        break;

      case symbol_kind::S_114_finished_statement: // finished-statement
        value.template destroy< finished_statement_type > ();
        break;

      case symbol_kind::S_121_fn_start_decl: // fn-start-decl
        value.template destroy< fn_kind > ();
        break;

      case symbol_kind::S_123_fn_decl: // fn-decl
        value.template destroy< fn_pure_t > ();
        break;

      case symbol_kind::S_116_generic_statement: // generic-statement
        value.template destroy< generic_statement_type > ();
        break;

      case symbol_kind::S_108_let_decl: // let-decl
      case symbol_kind::S_109_let_decl_start: // let-decl-start
      case symbol_kind::S_110_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.template destroy< let_statement > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
      case symbol_kind::S_106_finished_statement_any: // finished-statement-any
      case symbol_kind::S_111_infunction_statement_any: // infunction-statement-any
      case symbol_kind::S_112_finished_infunction_statement_any: // finished-infunction-statement-any
      case symbol_kind::S_113_braced_statements: // braced-statements
      case symbol_kind::S_115_infunction_statement_set: // infunction-statement-set
        value.template destroy< managed_statement_list > ();
        break;

      case symbol_kind::S_131_argument_list_opt: // argument-list-opt
      case symbol_kind::S_132_argument_list: // argument-list
      case symbol_kind::S_159_pack_expression: // pack-expression
        value.template destroy< named_expression_list_t > ();
        break;

      case symbol_kind::S_argument: // argument
        value.template destroy< named_expression_t > ();
        break;

      case symbol_kind::S_139_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_140_parameter_list: // parameter-list
        value.template destroy< parameter_list_t > ();
        break;

      case symbol_kind::S_143_parameter_decl: // parameter-decl
        value.template destroy< parameter_t > ();
        break;

      case symbol_kind::S_145_subpatterns_opt: // subpatterns-opt
      case symbol_kind::S_subpatterns: // subpatterns
      case symbol_kind::S_147_pattern_list: // pattern-list
        value.template destroy< pattern_list_t > ();
        break;

      case symbol_kind::S_pattern: // pattern
        value.template destroy< pattern_t > ();
        break;

      case symbol_kind::S_148_pattern_field: // pattern-field
        value.template destroy< pattern_t::field > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
      case symbol_kind::S_DECIMAL_S: // DECIMAL_S
        value.template destroy< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_119_internal_identifier: // internal-identifier
      case symbol_kind::S_134_argument_name: // argument-name
      case symbol_kind::S_141_internal_identifier_opt: // internal-identifier-opt
        value.template destroy< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.template destroy< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
      case symbol_kind::S_122_fn_name: // fn-name
        value.template destroy< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_CONTEXT_IDENTIFIER: // CONTEXT_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
      case symbol_kind::S_INTEGER_INDEX: // INTEGER_INDEX
        value.template destroy< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_127_case_decl: // case-decl
        value.template destroy< sonia::lang::bang::identifier > ();
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
        value.template destroy< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_137_field_default_value_opt: // field-default-value-opt
      case symbol_kind::S_142_parameter_default_value_opt: // parameter-default-value-opt
        value.template destroy< sonia::optional<syntax_expression_t> > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.template destroy< sonia::string_view > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_117_infunction_statement: // infunction-statement
        value.template destroy< statement > ();
        break;

      case symbol_kind::S_125_case_list_opt: // case-list-opt
      case symbol_kind::S_126_case_list: // case-list
        value.template destroy< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_128_struct_decl: // struct-decl
        value.template destroy< struct_decl > ();
        break;

      case symbol_kind::S_150_concept_expression: // concept-expression
      case symbol_kind::S_153_syntax_expression: // syntax-expression
      case symbol_kind::S_154_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_155_apostrophe_expression: // apostrophe-expression
      case symbol_kind::S_156_new_expression: // new-expression
      case symbol_kind::S_157_call_expression: // call-expression
      case symbol_kind::S_158_lambda_expression: // lambda-expression
      case symbol_kind::S_160_compound_expression: // compound-expression
      case symbol_kind::S_161_type_expr: // type-expr
        value.template destroy< syntax_expression_t > ();
        break;

      case symbol_kind::S_129_using_decl: // using-decl
        value.template destroy< using_decl > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, annotated_bool v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const annotated_bool& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, annotated_nil v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const annotated_nil& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, sonia::lang::bang::annotated_decimal v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const sonia::lang::bang::annotated_decimal& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, sonia::lang::bang::annotated_integer v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const sonia::lang::bang::annotated_integer& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, sonia::lang::bang::annotated_string_view v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const sonia::lang::bang::annotated_string_view& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, sonia::lang::lex::resource_location v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const sonia::lang::lex::resource_location& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, sonia::string_view v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const sonia::string_view& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
    };

    /// Build a parser object.
    parser (void * scanner_yyarg, parser_context & ctx_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if BANG_LANGDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BANG_LANGerror (location_type l)
      {
        return symbol_type (token::BANG_LANGerror, std::move (l));
      }
#else
      static
      symbol_type
      make_BANG_LANGerror (const location_type& l)
      {
        return symbol_type (token::BANG_LANGerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BANG_LANGUNDEF (location_type l)
      {
        return symbol_type (token::BANG_LANGUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_BANG_LANGUNDEF (const location_type& l)
      {
        return symbol_type (token::BANG_LANGUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMENT_BEGIN (location_type l)
      {
        return symbol_type (token::COMMENT_BEGIN, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMENT_BEGIN (const location_type& l)
      {
        return symbol_type (token::COMMENT_BEGIN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMENT_END (location_type l)
      {
        return symbol_type (token::COMMENT_END, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMENT_END (const location_type& l)
      {
        return symbol_type (token::COMMENT_END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRING (sonia::lang::bang::annotated_string_view v, location_type l)
      {
        return symbol_type (token::STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRING (const sonia::lang::bang::annotated_string_view& v, const location_type& l)
      {
        return symbol_type (token::STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (sonia::lang::bang::annotated_string_view v, location_type l)
      {
        return symbol_type (token::IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const sonia::lang::bang::annotated_string_view& v, const location_type& l)
      {
        return symbol_type (token::IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONTEXT_IDENTIFIER (sonia::lang::bang::annotated_string_view v, location_type l)
      {
        return symbol_type (token::CONTEXT_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONTEXT_IDENTIFIER (const sonia::lang::bang::annotated_string_view& v, const location_type& l)
      {
        return symbol_type (token::CONTEXT_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RESERVED_IDENTIFIER (sonia::lang::bang::annotated_string_view v, location_type l)
      {
        return symbol_type (token::RESERVED_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_RESERVED_IDENTIFIER (const sonia::lang::bang::annotated_string_view& v, const location_type& l)
      {
        return symbol_type (token::RESERVED_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTEGER_INDEX (sonia::lang::bang::annotated_string_view v, location_type l)
      {
        return symbol_type (token::INTEGER_INDEX, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTEGER_INDEX (const sonia::lang::bang::annotated_string_view& v, const location_type& l)
      {
        return symbol_type (token::INTEGER_INDEX, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTEGER (sonia::lang::bang::annotated_integer v, location_type l)
      {
        return symbol_type (token::INTEGER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTEGER (const sonia::lang::bang::annotated_integer& v, const location_type& l)
      {
        return symbol_type (token::INTEGER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DECIMAL (sonia::lang::bang::annotated_decimal v, location_type l)
      {
        return symbol_type (token::DECIMAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DECIMAL (const sonia::lang::bang::annotated_decimal& v, const location_type& l)
      {
        return symbol_type (token::DECIMAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DECIMAL_S (sonia::lang::bang::annotated_decimal v, location_type l)
      {
        return symbol_type (token::DECIMAL_S, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DECIMAL_S (const sonia::lang::bang::annotated_decimal& v, const location_type& l)
      {
        return symbol_type (token::DECIMAL_S, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPERATOR_TERM (sonia::string_view v, location_type l)
      {
        return symbol_type (token::OPERATOR_TERM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPERATOR_TERM (const sonia::string_view& v, const location_type& l)
      {
        return symbol_type (token::OPERATOR_TERM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASSIGN (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::ASSIGN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ASSIGN (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::ASSIGN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_APOSTROPHE (location_type l)
      {
        return symbol_type (token::APOSTROPHE, std::move (l));
      }
#else
      static
      symbol_type
      make_APOSTROPHE (const location_type& l)
      {
        return symbol_type (token::APOSTROPHE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AT_SYMBOL (location_type l)
      {
        return symbol_type (token::AT_SYMBOL, std::move (l));
      }
#else
      static
      symbol_type
      make_AT_SYMBOL (const location_type& l)
      {
        return symbol_type (token::AT_SYMBOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNDERSCORE (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::UNDERSCORE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_UNDERSCORE (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::UNDERSCORE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ARROWAST (location_type l)
      {
        return symbol_type (token::ARROWAST, std::move (l));
      }
#else
      static
      symbol_type
      make_ARROWAST (const location_type& l)
      {
        return symbol_type (token::ARROWAST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ARROW (location_type l)
      {
        return symbol_type (token::ARROW, std::move (l));
      }
#else
      static
      symbol_type
      make_ARROW (const location_type& l)
      {
        return symbol_type (token::ARROW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ARROWEXPR (location_type l)
      {
        return symbol_type (token::ARROWEXPR, std::move (l));
      }
#else
      static
      symbol_type
      make_ARROWEXPR (const location_type& l)
      {
        return symbol_type (token::ARROWEXPR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FARROW (location_type l)
      {
        return symbol_type (token::FARROW, std::move (l));
      }
#else
      static
      symbol_type
      make_FARROW (const location_type& l)
      {
        return symbol_type (token::FARROW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PTAST (location_type l)
      {
        return symbol_type (token::PTAST, std::move (l));
      }
#else
      static
      symbol_type
      make_PTAST (const location_type& l)
      {
        return symbol_type (token::PTAST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DBLPLUS (location_type l)
      {
        return symbol_type (token::DBLPLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_DBLPLUS (const location_type& l)
      {
        return symbol_type (token::DBLPLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DBLMINUS (location_type l)
      {
        return symbol_type (token::DBLMINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_DBLMINUS (const location_type& l)
      {
        return symbol_type (token::DBLMINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFTSHIFT (location_type l)
      {
        return symbol_type (token::LEFTSHIFT, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFTSHIFT (const location_type& l)
      {
        return symbol_type (token::LEFTSHIFT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHTSHIFT (location_type l)
      {
        return symbol_type (token::RIGHTSHIFT, std::move (l));
      }
#else
      static
      symbol_type
      make_RIGHTSHIFT (const location_type& l)
      {
        return symbol_type (token::RIGHTSHIFT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LE (location_type l)
      {
        return symbol_type (token::LE, std::move (l));
      }
#else
      static
      symbol_type
      make_LE (const location_type& l)
      {
        return symbol_type (token::LE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GE (location_type l)
      {
        return symbol_type (token::GE, std::move (l));
      }
#else
      static
      symbol_type
      make_GE (const location_type& l)
      {
        return symbol_type (token::GE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQ (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::EQ, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_EQ (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::EQ, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NE (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::NE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NE (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::NE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOGIC_AND (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::LOGIC_AND, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_LOGIC_AND (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::LOGIC_AND, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOGIC_OR (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::LOGIC_OR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_LOGIC_OR (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::LOGIC_OR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONCAT (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::CONCAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONCAT (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::CONCAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ELLIPSIS (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::ELLIPSIS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ELLIPSIS (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::ELLIPSIS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ADDASSIGN (location_type l)
      {
        return symbol_type (token::ADDASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_ADDASSIGN (const location_type& l)
      {
        return symbol_type (token::ADDASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SUBASSIGN (location_type l)
      {
        return symbol_type (token::SUBASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_SUBASSIGN (const location_type& l)
      {
        return symbol_type (token::SUBASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MULASSIGN (location_type l)
      {
        return symbol_type (token::MULASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_MULASSIGN (const location_type& l)
      {
        return symbol_type (token::MULASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIVASSIGN (location_type l)
      {
        return symbol_type (token::DIVASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_DIVASSIGN (const location_type& l)
      {
        return symbol_type (token::DIVASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MODASSIGN (location_type l)
      {
        return symbol_type (token::MODASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_MODASSIGN (const location_type& l)
      {
        return symbol_type (token::MODASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LSHIFTSSIGN (location_type l)
      {
        return symbol_type (token::LSHIFTSSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_LSHIFTSSIGN (const location_type& l)
      {
        return symbol_type (token::LSHIFTSSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RSHIFTSSIGN (location_type l)
      {
        return symbol_type (token::RSHIFTSSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_RSHIFTSSIGN (const location_type& l)
      {
        return symbol_type (token::RSHIFTSSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITANDASSIGN (location_type l)
      {
        return symbol_type (token::BITANDASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_BITANDASSIGN (const location_type& l)
      {
        return symbol_type (token::BITANDASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITORASSIGN (location_type l)
      {
        return symbol_type (token::BITORASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_BITORASSIGN (const location_type& l)
      {
        return symbol_type (token::BITORASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITXORASSIGN (location_type l)
      {
        return symbol_type (token::BITXORASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_BITXORASSIGN (const location_type& l)
      {
        return symbol_type (token::BITXORASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLON (location_type l)
      {
        return symbol_type (token::COLON, std::move (l));
      }
#else
      static
      symbol_type
      make_COLON (const location_type& l)
      {
        return symbol_type (token::COLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DBLCOLON (location_type l)
      {
        return symbol_type (token::DBLCOLON, std::move (l));
      }
#else
      static
      symbol_type
      make_DBLCOLON (const location_type& l)
      {
        return symbol_type (token::DBLCOLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPEN_PARENTHESIS (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::OPEN_PARENTHESIS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_PARENTHESIS (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::OPEN_PARENTHESIS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLOSE_PARENTHESIS (location_type l)
      {
        return symbol_type (token::CLOSE_PARENTHESIS, std::move (l));
      }
#else
      static
      symbol_type
      make_CLOSE_PARENTHESIS (const location_type& l)
      {
        return symbol_type (token::CLOSE_PARENTHESIS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPEN_BRACE (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::OPEN_BRACE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_BRACE (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::OPEN_BRACE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLOSE_BRACE (location_type l)
      {
        return symbol_type (token::CLOSE_BRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_CLOSE_BRACE (const location_type& l)
      {
        return symbol_type (token::CLOSE_BRACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPEN_SQUARE_BRACKET (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::OPEN_SQUARE_BRACKET, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_SQUARE_BRACKET (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::OPEN_SQUARE_BRACKET, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLOSE_SQUARE_BRACKET (location_type l)
      {
        return symbol_type (token::CLOSE_SQUARE_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_CLOSE_SQUARE_BRACKET (const location_type& l)
      {
        return symbol_type (token::CLOSE_SQUARE_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPEN_SQUARE_DBL_BRACKET (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::OPEN_SQUARE_DBL_BRACKET, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_SQUARE_DBL_BRACKET (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::OPEN_SQUARE_DBL_BRACKET, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLOSE_SQUARE_DBL_BRACKET (location_type l)
      {
        return symbol_type (token::CLOSE_SQUARE_DBL_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_CLOSE_SQUARE_DBL_BRACKET (const location_type& l)
      {
        return symbol_type (token::CLOSE_SQUARE_DBL_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPEN_BROKET (location_type l)
      {
        return symbol_type (token::OPEN_BROKET, std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_BROKET (const location_type& l)
      {
        return symbol_type (token::OPEN_BROKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLOSE_BROKET (location_type l)
      {
        return symbol_type (token::CLOSE_BROKET, std::move (l));
      }
#else
      static
      symbol_type
      make_CLOSE_BROKET (const location_type& l)
      {
        return symbol_type (token::CLOSE_BROKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END_STATEMENT (location_type l)
      {
        return symbol_type (token::END_STATEMENT, std::move (l));
      }
#else
      static
      symbol_type
      make_END_STATEMENT (const location_type& l)
      {
        return symbol_type (token::END_STATEMENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_POINT (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::POINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_POINT (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::POINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::PLUS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::PLUS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::MINUS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::MINUS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASTERISK (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::ASTERISK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ASTERISK (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::ASTERISK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SLASH (location_type l)
      {
        return symbol_type (token::SLASH, std::move (l));
      }
#else
      static
      symbol_type
      make_SLASH (const location_type& l)
      {
        return symbol_type (token::SLASH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERCENT (location_type l)
      {
        return symbol_type (token::PERCENT, std::move (l));
      }
#else
      static
      symbol_type
      make_PERCENT (const location_type& l)
      {
        return symbol_type (token::PERCENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AMPERSAND (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::AMPERSAND, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_AMPERSAND (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::AMPERSAND, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITOR (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::BITOR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BITOR (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::BITOR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXCLPT (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::EXCLPT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_EXCLPT (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::EXCLPT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TILDA (location_type l)
      {
        return symbol_type (token::TILDA, std::move (l));
      }
#else
      static
      symbol_type
      make_TILDA (const location_type& l)
      {
        return symbol_type (token::TILDA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXCL (location_type l)
      {
        return symbol_type (token::EXCL, std::move (l));
      }
#else
      static
      symbol_type
      make_EXCL (const location_type& l)
      {
        return symbol_type (token::EXCL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOLLAR (location_type l)
      {
        return symbol_type (token::DOLLAR, std::move (l));
      }
#else
      static
      symbol_type
      make_DOLLAR (const location_type& l)
      {
        return symbol_type (token::DOLLAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_QMARK (location_type l)
      {
        return symbol_type (token::QMARK, std::move (l));
      }
#else
      static
      symbol_type
      make_QMARK (const location_type& l)
      {
        return symbol_type (token::QMARK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HASHTAG (location_type l)
      {
        return symbol_type (token::HASHTAG, std::move (l));
      }
#else
      static
      symbol_type
      make_HASHTAG (const location_type& l)
      {
        return symbol_type (token::HASHTAG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LET (location_type l)
      {
        return symbol_type (token::LET, std::move (l));
      }
#else
      static
      symbol_type
      make_LET (const location_type& l)
      {
        return symbol_type (token::LET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VAR (location_type l)
      {
        return symbol_type (token::VAR, std::move (l));
      }
#else
      static
      symbol_type
      make_VAR (const location_type& l)
      {
        return symbol_type (token::VAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXTERN (location_type l)
      {
        return symbol_type (token::EXTERN, std::move (l));
      }
#else
      static
      symbol_type
      make_EXTERN (const location_type& l)
      {
        return symbol_type (token::EXTERN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NEW (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::NEW, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NEW (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::NEW, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WHILE (location_type l)
      {
        return symbol_type (token::WHILE, std::move (l));
      }
#else
      static
      symbol_type
      make_WHILE (const location_type& l)
      {
        return symbol_type (token::WHILE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FOR (location_type l)
      {
        return symbol_type (token::FOR, std::move (l));
      }
#else
      static
      symbol_type
      make_FOR (const location_type& l)
      {
        return symbol_type (token::FOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IN (location_type l)
      {
        return symbol_type (token::IN, std::move (l));
      }
#else
      static
      symbol_type
      make_IN (const location_type& l)
      {
        return symbol_type (token::IN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IF (location_type l)
      {
        return symbol_type (token::IF, std::move (l));
      }
#else
      static
      symbol_type
      make_IF (const location_type& l)
      {
        return symbol_type (token::IF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ELSE (location_type l)
      {
        return symbol_type (token::ELSE, std::move (l));
      }
#else
      static
      symbol_type
      make_ELSE (const location_type& l)
      {
        return symbol_type (token::ELSE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONTINUE (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::CONTINUE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONTINUE (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::CONTINUE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BREAK (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::BREAK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BREAK (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::BREAK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RETURN (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::RETURN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_RETURN (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::RETURN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AUTO (location_type l)
      {
        return symbol_type (token::AUTO, std::move (l));
      }
#else
      static
      symbol_type
      make_AUTO (const location_type& l)
      {
        return symbol_type (token::AUTO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_USING (location_type l)
      {
        return symbol_type (token::USING, std::move (l));
      }
#else
      static
      symbol_type
      make_USING (const location_type& l)
      {
        return symbol_type (token::USING, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_THROW (location_type l)
      {
        return symbol_type (token::THROW, std::move (l));
      }
#else
      static
      symbol_type
      make_THROW (const location_type& l)
      {
        return symbol_type (token::THROW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SIZEOF (location_type l)
      {
        return symbol_type (token::SIZEOF, std::move (l));
      }
#else
      static
      symbol_type
      make_SIZEOF (const location_type& l)
      {
        return symbol_type (token::SIZEOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOWEST (location_type l)
      {
        return symbol_type (token::LOWEST, std::move (l));
      }
#else
      static
      symbol_type
      make_LOWEST (const location_type& l)
      {
        return symbol_type (token::LOWEST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEREF (location_type l)
      {
        return symbol_type (token::DEREF, std::move (l));
      }
#else
      static
      symbol_type
      make_DEREF (const location_type& l)
      {
        return symbol_type (token::DEREF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INCLUDE (location_type l)
      {
        return symbol_type (token::INCLUDE, std::move (l));
      }
#else
      static
      symbol_type
      make_INCLUDE (const location_type& l)
      {
        return symbol_type (token::INCLUDE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INLINE (location_type l)
      {
        return symbol_type (token::INLINE, std::move (l));
      }
#else
      static
      symbol_type
      make_INLINE (const location_type& l)
      {
        return symbol_type (token::INLINE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FN (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::FN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FN (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::FN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ENUM (location_type l)
      {
        return symbol_type (token::ENUM, std::move (l));
      }
#else
      static
      symbol_type
      make_ENUM (const location_type& l)
      {
        return symbol_type (token::ENUM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRUCT (location_type l)
      {
        return symbol_type (token::STRUCT, std::move (l));
      }
#else
      static
      symbol_type
      make_STRUCT (const location_type& l)
      {
        return symbol_type (token::STRUCT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXTENDS (location_type l)
      {
        return symbol_type (token::EXTENDS, std::move (l));
      }
#else
      static
      symbol_type
      make_EXTENDS (const location_type& l)
      {
        return symbol_type (token::EXTENDS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WEAK (location_type l)
      {
        return symbol_type (token::WEAK, std::move (l));
      }
#else
      static
      symbol_type
      make_WEAK (const location_type& l)
      {
        return symbol_type (token::WEAK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONST (location_type l)
      {
        return symbol_type (token::CONST, std::move (l));
      }
#else
      static
      symbol_type
      make_CONST (const location_type& l)
      {
        return symbol_type (token::CONST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MUT (sonia::lang::lex::resource_location v, location_type l)
      {
        return symbol_type (token::MUT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_MUT (const sonia::lang::lex::resource_location& v, const location_type& l)
      {
        return symbol_type (token::MUT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NIL_WORD (annotated_nil v, location_type l)
      {
        return symbol_type (token::NIL_WORD, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NIL_WORD (const annotated_nil& v, const location_type& l)
      {
        return symbol_type (token::NIL_WORD, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TRUE_WORD (annotated_bool v, location_type l)
      {
        return symbol_type (token::TRUE_WORD, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TRUE_WORD (const annotated_bool& v, const location_type& l)
      {
        return symbol_type (token::TRUE_WORD, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FALSE_WORD (annotated_bool v, location_type l)
      {
        return symbol_type (token::FALSE_WORD, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FALSE_WORD (const annotated_bool& v, const location_type& l)
      {
        return symbol_type (token::FALSE_WORD, v, l);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const unsigned char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const unsigned char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const unsigned char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if BANG_LANGDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 1134,     ///< Last index in yytable_.
      yynnts_ = 59,  ///< Number of nonterminal symbols.
      yyfinal_ = 83 ///< Termination state number.
    };


    // User arguments.
    void * scanner;
    parser_context & ctx;

  };


} // bang_lang
#line 4502 "bang.tab.hpp"




#endif // !YY_BANG_LANG_BANG_TAB_HPP_INCLUDED
