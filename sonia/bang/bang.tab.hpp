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

      // type-expr
      char dummy2[sizeof (bang_preliminary_type)];

      // statement
      char dummy3[sizeof (declaration)];

      // statement_any
      char dummy4[sizeof (declaration_set_t)];

      // enum-decl
      char dummy5[sizeof (enum_decl)];

      // field-list-opt
      // field-list
      char dummy6[sizeof (field_list_t)];

      // field-decl
      char dummy7[sizeof (field_t)];

      // fn-start-decl
      char dummy8[sizeof (fn_kind)];

      // fn-decl
      char dummy9[sizeof (fn_pure_t)];

      // generic-statement
      char dummy10[sizeof (generic_statement)];

      // infunction-statement
      char dummy11[sizeof (infunction_declaration)];

      // infunction-statement-any
      char dummy12[sizeof (infunction_declaration_set_t)];

      // let-decl
      // let-decl-start
      // let-decl-start-with-opt-type
      char dummy13[sizeof (let_statement_decl_t)];

      // opt-named-expr-list-any
      // opt-named-expr-list
      char dummy14[sizeof (named_expression_term_list_t)];

      // opt-named-expr
      char dummy15[sizeof (named_expression_term_t)];

      // parameter-constraint-modifier
      // parameter-constraint-modifier-opt
      char dummy16[sizeof (parameter_constraint_modifier_t)];

      // parameter-constraint-set
      char dummy17[sizeof (parameter_constraint_set_t)];

      // parameter-name-decl
      char dummy18[sizeof (parameter_name)];

      // parameter-decl
      char dummy19[sizeof (parameter_t)];

      // parameter-list-opt
      // parameter-list
      char dummy20[sizeof (parameter_woa_list_t)];

      // DECIMAL
      char dummy21[sizeof (sonia::lang::bang::annotated_decimal)];

      // identifier
      char dummy22[sizeof (sonia::lang::bang::annotated_identifier)];

      // INTEGER
      char dummy23[sizeof (sonia::lang::bang::annotated_integer)];

      // qname
      char dummy24[sizeof (sonia::lang::bang::annotated_qname)];

      // STRING
      // IDENTIFIER
      // INTERNAL_IDENTIFIER
      // RESERVED_IDENTIFIER
      char dummy25[sizeof (sonia::lang::bang::annotated_string_view)];

      // case-decl
      char dummy26[sizeof (sonia::lang::bang::identifier)];

      // "`=`"
      // "`==`"
      // "`!=`"
      // "`&&`"
      // "`||`"
      // "`..`"
      // "`...`"
      // "`(`"
      // "`{`"
      // "`[`"
      // "`.`"
      // "`+`"
      // "`!`"
      // "continue"
      // "break"
      // "`fn`"
      char dummy27[sizeof (sonia::lang::lex::resource_location)];

      // OPERATOR_TERM
      char dummy28[sizeof (sonia::string_view)];

      // case-list-opt
      // case-list
      char dummy29[sizeof (std::vector<sonia::lang::bang::identifier>)];

      // concept-expression
      // syntax-expression-wo-ii
      // syntax-expression
      // compound-expression
      char dummy30[sizeof (syntax_expression_t)];
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
    INTERNAL_IDENTIFIER = 262,     // INTERNAL_IDENTIFIER
    RESERVED_IDENTIFIER = 263,     // RESERVED_IDENTIFIER
    INTEGER = 264,                 // INTEGER
    DECIMAL = 265,                 // DECIMAL
    OPERATOR_TERM = 266,           // OPERATOR_TERM
    ASSIGN = 267,                  // "`=`"
    AT_SYMBOL = 268,               // "`@`"
    UNDERSCORE = 269,              // "`_`"
    ARROWAST = 270,                // "`->*`"
    ARROW = 271,                   // "`->`"
    ARROWEXPR = 272,               // "`=>`"
    FARROW = 273,                  // "`~>`"
    PTAST = 274,                   // "`.*`"
    DBLPLUS = 275,                 // "`++`"
    DBLMINUS = 276,                // "`--`"
    LEFTSHIFT = 277,               // "`<<`"
    RIGHTSHIFT = 278,              // "`>>`"
    LE = 279,                      // "`<=`"
    GE = 280,                      // "`>=`"
    EQ = 281,                      // "`==`"
    NE = 282,                      // "`!=`"
    LOGIC_AND = 283,               // "`&&`"
    LOGIC_OR = 284,                // "`||`"
    CONCAT = 285,                  // "`..`"
    ELLIPSIS = 286,                // "`...`"
    ADDASSIGN = 287,               // "`+=`"
    SUBASSIGN = 288,               // "`-=`"
    MULASSIGN = 289,               // "`*=`"
    DIVASSIGN = 290,               // "`/=`"
    MODASSIGN = 291,               // "`%=`"
    LSHIFTSSIGN = 292,             // "`<<=`"
    RSHIFTSSIGN = 293,             // "`>>=`"
    BITANDASSIGN = 294,            // "`&=`"
    BITORASSIGN = 295,             // "`|=`"
    BITXORASSIGN = 296,            // "`^=`"
    COLON = 297,                   // "`:`"
    DBLCOLON = 298,                // "`::`"
    OPEN_PARENTHESIS = 299,        // "`(`"
    CLOSE_PARENTHESIS = 300,       // "`)`"
    OPEN_BRACE = 301,              // "`{`"
    CLOSE_BRACE = 302,             // "`}`"
    OPEN_SQUARE_BRACKET = 303,     // "`[`"
    CLOSE_SQUARE_BRACKET = 304,    // "`]`"
    OPEN_BROKET = 305,             // "`<`"
    CLOSE_BROKET = 306,            // "`>`"
    END_STATEMENT = 307,           // "`;`"
    POINT = 308,                   // "`.`"
    PLUS = 309,                    // "`+`"
    MINUS = 310,                   // "`-`"
    ASTERISK = 311,                // "`*`"
    SLASH = 312,                   // "`/`"
    PERCENT = 313,                 // "`%`"
    AMPERSAND = 314,               // "`&`"
    BITOR = 315,                   // "`|`"
    EXCLPT = 316,                  // "`!`"
    TILDA = 317,                   // "`~`"
    EXCL = 318,                    // "`^`"
    COMMA = 319,                   // ","
    DOLLAR = 320,                  // "`$`"
    QMARK = 321,                   // "`?`"
    HASHTAG = 322,                 // "`#`"
    LET = 323,                     // LET
    VAR = 324,                     // VAR
    EXTERN = 325,                  // EXTERN
    CONTINUE = 326,                // "continue"
    BREAK = 327,                   // "break"
    RETURN = 328,                  // RETURN
    FOR = 329,                     // FOR
    VOID_ = 330,                   // VOID_
    AUTO = 331,                    // AUTO
    CONST_ = 332,                  // CONST_
    USING = 333,                   // USING
    THROW = 334,                   // THROW
    DELETE_ = 335,                 // DELETE_
    SIZEOF = 336,                  // SIZEOF
    LOWEST = 337,                  // LOWEST
    INCLUDE = 338,                 // INCLUDE
    INLINE = 339,                  // INLINE
    FN = 340,                      // "`fn`"
    ENUM = 341,                    // ENUM
    TYPE = 342,                    // TYPE
    EXTENDS = 343,                 // EXTENDS
    TYPENAME = 344,                // TYPENAME
    WEAK = 345,                    // "weak modifier"
    CONST = 346,                   // "const modifier"
    WHILE = 347,                   // WHILE
    IF = 348,                      // IF
    TRUE = 349,                    // "true"
    FALSE = 350                    // "false"
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
        YYNTOKENS = 96, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_COMMENT_BEGIN = 3,                     // COMMENT_BEGIN
        S_COMMENT_END = 4,                       // COMMENT_END
        S_STRING = 5,                            // STRING
        S_IDENTIFIER = 6,                        // IDENTIFIER
        S_INTERNAL_IDENTIFIER = 7,               // INTERNAL_IDENTIFIER
        S_RESERVED_IDENTIFIER = 8,               // RESERVED_IDENTIFIER
        S_INTEGER = 9,                           // INTEGER
        S_DECIMAL = 10,                          // DECIMAL
        S_OPERATOR_TERM = 11,                    // OPERATOR_TERM
        S_ASSIGN = 12,                           // "`=`"
        S_AT_SYMBOL = 13,                        // "`@`"
        S_UNDERSCORE = 14,                       // "`_`"
        S_ARROWAST = 15,                         // "`->*`"
        S_ARROW = 16,                            // "`->`"
        S_ARROWEXPR = 17,                        // "`=>`"
        S_FARROW = 18,                           // "`~>`"
        S_PTAST = 19,                            // "`.*`"
        S_DBLPLUS = 20,                          // "`++`"
        S_DBLMINUS = 21,                         // "`--`"
        S_LEFTSHIFT = 22,                        // "`<<`"
        S_RIGHTSHIFT = 23,                       // "`>>`"
        S_LE = 24,                               // "`<=`"
        S_GE = 25,                               // "`>=`"
        S_EQ = 26,                               // "`==`"
        S_NE = 27,                               // "`!=`"
        S_LOGIC_AND = 28,                        // "`&&`"
        S_LOGIC_OR = 29,                         // "`||`"
        S_CONCAT = 30,                           // "`..`"
        S_ELLIPSIS = 31,                         // "`...`"
        S_ADDASSIGN = 32,                        // "`+=`"
        S_SUBASSIGN = 33,                        // "`-=`"
        S_MULASSIGN = 34,                        // "`*=`"
        S_DIVASSIGN = 35,                        // "`/=`"
        S_MODASSIGN = 36,                        // "`%=`"
        S_LSHIFTSSIGN = 37,                      // "`<<=`"
        S_RSHIFTSSIGN = 38,                      // "`>>=`"
        S_BITANDASSIGN = 39,                     // "`&=`"
        S_BITORASSIGN = 40,                      // "`|=`"
        S_BITXORASSIGN = 41,                     // "`^=`"
        S_COLON = 42,                            // "`:`"
        S_DBLCOLON = 43,                         // "`::`"
        S_OPEN_PARENTHESIS = 44,                 // "`(`"
        S_CLOSE_PARENTHESIS = 45,                // "`)`"
        S_OPEN_BRACE = 46,                       // "`{`"
        S_CLOSE_BRACE = 47,                      // "`}`"
        S_OPEN_SQUARE_BRACKET = 48,              // "`[`"
        S_CLOSE_SQUARE_BRACKET = 49,             // "`]`"
        S_OPEN_BROKET = 50,                      // "`<`"
        S_CLOSE_BROKET = 51,                     // "`>`"
        S_END_STATEMENT = 52,                    // "`;`"
        S_POINT = 53,                            // "`.`"
        S_PLUS = 54,                             // "`+`"
        S_MINUS = 55,                            // "`-`"
        S_ASTERISK = 56,                         // "`*`"
        S_SLASH = 57,                            // "`/`"
        S_PERCENT = 58,                          // "`%`"
        S_AMPERSAND = 59,                        // "`&`"
        S_BITOR = 60,                            // "`|`"
        S_EXCLPT = 61,                           // "`!`"
        S_TILDA = 62,                            // "`~`"
        S_EXCL = 63,                             // "`^`"
        S_COMMA = 64,                            // ","
        S_DOLLAR = 65,                           // "`$`"
        S_QMARK = 66,                            // "`?`"
        S_HASHTAG = 67,                          // "`#`"
        S_LET = 68,                              // LET
        S_VAR = 69,                              // VAR
        S_EXTERN = 70,                           // EXTERN
        S_CONTINUE = 71,                         // "continue"
        S_BREAK = 72,                            // "break"
        S_RETURN = 73,                           // RETURN
        S_FOR = 74,                              // FOR
        S_VOID_ = 75,                            // VOID_
        S_AUTO = 76,                             // AUTO
        S_CONST_ = 77,                           // CONST_
        S_USING = 78,                            // USING
        S_THROW = 79,                            // THROW
        S_DELETE_ = 80,                          // DELETE_
        S_SIZEOF = 81,                           // SIZEOF
        S_LOWEST = 82,                           // LOWEST
        S_INCLUDE = 83,                          // INCLUDE
        S_INLINE = 84,                           // INLINE
        S_FN = 85,                               // "`fn`"
        S_ENUM = 86,                             // ENUM
        S_TYPE = 87,                             // TYPE
        S_EXTENDS = 88,                          // EXTENDS
        S_TYPENAME = 89,                         // TYPENAME
        S_WEAK = 90,                             // "weak modifier"
        S_CONST = 91,                            // "const modifier"
        S_WHILE = 92,                            // WHILE
        S_IF = 93,                               // IF
        S_TRUE = 94,                             // "true"
        S_FALSE = 95,                            // "false"
        S_YYACCEPT = 96,                         // $accept
        S_begin = 97,                            // begin
        S_statement_any = 98,                    // statement_any
        S_statement = 99,                        // statement
        S_100_let_decl = 100,                    // let-decl
        S_101_let_decl_start = 101,              // let-decl-start
        S_102_let_decl_start_with_opt_type = 102, // let-decl-start-with-opt-type
        S_103_infunction_statement_any = 103,    // infunction-statement-any
        S_104_generic_statement = 104,           // generic-statement
        S_105_infunction_statement = 105,        // infunction-statement
        S_identifier = 106,                      // identifier
        S_qname = 107,                           // qname
        S_108_fn_start_decl = 108,               // fn-start-decl
        S_109_fn_decl = 109,                     // fn-decl
        S_110_enum_decl = 110,                   // enum-decl
        S_111_case_list_opt = 111,               // case-list-opt
        S_112_case_list = 112,                   // case-list
        S_113_case_decl = 113,                   // case-decl
        S_114_field_list_opt = 114,              // field-list-opt
        S_115_field_list = 115,                  // field-list
        S_116_field_decl = 116,                  // field-decl
        S_117_parameter_name_decl = 117,         // parameter-name-decl
        S_118_parameter_constraint_modifier = 118, // parameter-constraint-modifier
        S_119_parameter_constraint_modifier_opt = 119, // parameter-constraint-modifier-opt
        S_120_parameter_constraint_set = 120,    // parameter-constraint-set
        S_121_concept_expression = 121,          // concept-expression
        S_122_syntax_expression_wo_ii = 122,     // syntax-expression-wo-ii
        S_123_syntax_expression = 123,           // syntax-expression
        S_124_parameter_decl = 124,              // parameter-decl
        S_125_parameter_list_opt = 125,          // parameter-list-opt
        S_126_parameter_list = 126,              // parameter-list
        S_127_type_expr = 127,                   // type-expr
        S_128_compound_expression = 128,         // compound-expression
        S_129_opt_named_expr_list_any = 129,     // opt-named-expr-list-any
        S_130_opt_named_expr_list = 130,         // opt-named-expr-list
        S_131_opt_named_expr = 131               // opt-named-expr
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< annotated_bool > (std::move (that.value));
        break;

      case symbol_kind::S_127_type_expr: // type-expr
        value.move< bang_preliminary_type > (std::move (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< declaration > (std::move (that.value));
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.move< declaration_set_t > (std::move (that.value));
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.move< enum_decl > (std::move (that.value));
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.move< field_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.move< field_t > (std::move (that.value));
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.move< fn_kind > (std::move (that.value));
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.move< fn_pure_t > (std::move (that.value));
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.move< generic_statement > (std::move (that.value));
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.move< infunction_declaration > (std::move (that.value));
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.move< infunction_declaration_set_t > (std::move (that.value));
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.move< let_statement_decl_t > (std::move (that.value));
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (std::move (that.value));
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.move< parameter_constraint_modifier_t > (std::move (that.value));
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.move< parameter_constraint_set_t > (std::move (that.value));
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.move< parameter_name > (std::move (that.value));
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.move< parameter_t > (std::move (that.value));
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
        value.move< parameter_woa_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::lang::bang::annotated_decimal > (std::move (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::bang::annotated_identifier > (std::move (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< sonia::lang::bang::annotated_integer > (std::move (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::bang::annotated_qname > (std::move (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.move< sonia::lang::bang::annotated_string_view > (std::move (that.value));
        break;

      case symbol_kind::S_113_case_decl: // case-decl
        value.move< sonia::lang::bang::identifier > (std::move (that.value));
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
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.move< sonia::lang::lex::resource_location > (std::move (that.value));
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (std::move (that.value));
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.move< std::vector<sonia::lang::bang::identifier> > (std::move (that.value));
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
        value.move< syntax_expression_t > (std::move (that.value));
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
      basic_symbol (typename Base::kind_type t, bang_preliminary_type&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const bang_preliminary_type& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, declaration&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const declaration& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, declaration_set_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const declaration_set_t& v, const location_type& l)
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
      basic_symbol (typename Base::kind_type t, generic_statement&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const generic_statement& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, infunction_declaration&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const infunction_declaration& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, infunction_declaration_set_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const infunction_declaration_set_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, let_statement_decl_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const let_statement_decl_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, named_expression_term_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const named_expression_term_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, named_expression_term_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const named_expression_term_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, parameter_constraint_modifier_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const parameter_constraint_modifier_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, parameter_constraint_set_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const parameter_constraint_set_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, parameter_name&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const parameter_name& v, const location_type& l)
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
      basic_symbol (typename Base::kind_type t, parameter_woa_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const parameter_woa_list_t& v, const location_type& l)
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
#line 316 "bang.y"
                    { }
#line 1444 "bang.tab.hpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 316 "bang.y"
                    { }
#line 1450 "bang.tab.hpp"
        break;

      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
#line 316 "bang.y"
                    { }
#line 1456 "bang.tab.hpp"
        break;

      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
#line 316 "bang.y"
                    { }
#line 1462 "bang.tab.hpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 316 "bang.y"
                    { }
#line 1468 "bang.tab.hpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 316 "bang.y"
                    { }
#line 1474 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 316 "bang.y"
                    { }
#line 1480 "bang.tab.hpp"
        break;

      case symbol_kind::S_ASSIGN: // "`=`"
#line 316 "bang.y"
                    { }
#line 1486 "bang.tab.hpp"
        break;

      case symbol_kind::S_EQ: // "`==`"
#line 316 "bang.y"
                    { }
#line 1492 "bang.tab.hpp"
        break;

      case symbol_kind::S_NE: // "`!=`"
#line 316 "bang.y"
                    { }
#line 1498 "bang.tab.hpp"
        break;

      case symbol_kind::S_LOGIC_AND: // "`&&`"
#line 316 "bang.y"
                    { }
#line 1504 "bang.tab.hpp"
        break;

      case symbol_kind::S_LOGIC_OR: // "`||`"
#line 316 "bang.y"
                    { }
#line 1510 "bang.tab.hpp"
        break;

      case symbol_kind::S_CONCAT: // "`..`"
#line 316 "bang.y"
                    { }
#line 1516 "bang.tab.hpp"
        break;

      case symbol_kind::S_ELLIPSIS: // "`...`"
#line 316 "bang.y"
                    { }
#line 1522 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPEN_PARENTHESIS: // "`(`"
#line 316 "bang.y"
                    { }
#line 1528 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPEN_BRACE: // "`{`"
#line 316 "bang.y"
                    { }
#line 1534 "bang.tab.hpp"
        break;

      case symbol_kind::S_OPEN_SQUARE_BRACKET: // "`[`"
#line 316 "bang.y"
                    { }
#line 1540 "bang.tab.hpp"
        break;

      case symbol_kind::S_POINT: // "`.`"
#line 316 "bang.y"
                    { }
#line 1546 "bang.tab.hpp"
        break;

      case symbol_kind::S_PLUS: // "`+`"
#line 316 "bang.y"
                    { }
#line 1552 "bang.tab.hpp"
        break;

      case symbol_kind::S_EXCLPT: // "`!`"
#line 316 "bang.y"
                    { }
#line 1558 "bang.tab.hpp"
        break;

      case symbol_kind::S_CONTINUE: // "continue"
#line 316 "bang.y"
                    { }
#line 1564 "bang.tab.hpp"
        break;

      case symbol_kind::S_BREAK: // "break"
#line 316 "bang.y"
                    { }
#line 1570 "bang.tab.hpp"
        break;

      case symbol_kind::S_FN: // "`fn`"
#line 316 "bang.y"
                    { }
#line 1576 "bang.tab.hpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 316 "bang.y"
                    { }
#line 1582 "bang.tab.hpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 316 "bang.y"
                    { }
#line 1588 "bang.tab.hpp"
        break;

      case symbol_kind::S_statement_any: // statement_any
#line 316 "bang.y"
                    { }
#line 1594 "bang.tab.hpp"
        break;

      case symbol_kind::S_statement: // statement
#line 316 "bang.y"
                    { }
#line 1600 "bang.tab.hpp"
        break;

      case symbol_kind::S_100_let_decl: // let-decl
#line 316 "bang.y"
                    { }
#line 1606 "bang.tab.hpp"
        break;

      case symbol_kind::S_101_let_decl_start: // let-decl-start
#line 316 "bang.y"
                    { }
#line 1612 "bang.tab.hpp"
        break;

      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
#line 316 "bang.y"
                    { }
#line 1618 "bang.tab.hpp"
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
#line 316 "bang.y"
                    { }
#line 1624 "bang.tab.hpp"
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
#line 316 "bang.y"
                    { }
#line 1630 "bang.tab.hpp"
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
#line 316 "bang.y"
                    { }
#line 1636 "bang.tab.hpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 316 "bang.y"
                    { }
#line 1642 "bang.tab.hpp"
        break;

      case symbol_kind::S_qname: // qname
#line 316 "bang.y"
                    { }
#line 1648 "bang.tab.hpp"
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
#line 316 "bang.y"
                    { }
#line 1654 "bang.tab.hpp"
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
#line 316 "bang.y"
                    { }
#line 1660 "bang.tab.hpp"
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
#line 316 "bang.y"
                    { }
#line 1666 "bang.tab.hpp"
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
#line 316 "bang.y"
                    { }
#line 1672 "bang.tab.hpp"
        break;

      case symbol_kind::S_112_case_list: // case-list
#line 316 "bang.y"
                    { }
#line 1678 "bang.tab.hpp"
        break;

      case symbol_kind::S_113_case_decl: // case-decl
#line 316 "bang.y"
                    { }
#line 1684 "bang.tab.hpp"
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
#line 316 "bang.y"
                    { }
#line 1690 "bang.tab.hpp"
        break;

      case symbol_kind::S_115_field_list: // field-list
#line 316 "bang.y"
                    { }
#line 1696 "bang.tab.hpp"
        break;

      case symbol_kind::S_116_field_decl: // field-decl
#line 316 "bang.y"
                    { }
#line 1702 "bang.tab.hpp"
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
#line 316 "bang.y"
                    { }
#line 1708 "bang.tab.hpp"
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
#line 316 "bang.y"
                    { }
#line 1714 "bang.tab.hpp"
        break;

      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
#line 316 "bang.y"
                    { }
#line 1720 "bang.tab.hpp"
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
#line 316 "bang.y"
                    { }
#line 1726 "bang.tab.hpp"
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
#line 316 "bang.y"
                    { }
#line 1732 "bang.tab.hpp"
        break;

      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
#line 316 "bang.y"
                    { }
#line 1738 "bang.tab.hpp"
        break;

      case symbol_kind::S_123_syntax_expression: // syntax-expression
#line 316 "bang.y"
                    { }
#line 1744 "bang.tab.hpp"
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
#line 316 "bang.y"
                    { }
#line 1750 "bang.tab.hpp"
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
#line 316 "bang.y"
                    { }
#line 1756 "bang.tab.hpp"
        break;

      case symbol_kind::S_126_parameter_list: // parameter-list
#line 316 "bang.y"
                    { }
#line 1762 "bang.tab.hpp"
        break;

      case symbol_kind::S_127_type_expr: // type-expr
#line 316 "bang.y"
                    { }
#line 1768 "bang.tab.hpp"
        break;

      case symbol_kind::S_128_compound_expression: // compound-expression
#line 316 "bang.y"
                    { }
#line 1774 "bang.tab.hpp"
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
#line 316 "bang.y"
                    { }
#line 1780 "bang.tab.hpp"
        break;

      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
#line 316 "bang.y"
                    { }
#line 1786 "bang.tab.hpp"
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
#line 316 "bang.y"
                    { }
#line 1792 "bang.tab.hpp"
        break;

       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.template destroy< annotated_bool > ();
        break;

      case symbol_kind::S_127_type_expr: // type-expr
        value.template destroy< bang_preliminary_type > ();
        break;

      case symbol_kind::S_statement: // statement
        value.template destroy< declaration > ();
        break;

      case symbol_kind::S_statement_any: // statement_any
        value.template destroy< declaration_set_t > ();
        break;

      case symbol_kind::S_110_enum_decl: // enum-decl
        value.template destroy< enum_decl > ();
        break;

      case symbol_kind::S_114_field_list_opt: // field-list-opt
      case symbol_kind::S_115_field_list: // field-list
        value.template destroy< field_list_t > ();
        break;

      case symbol_kind::S_116_field_decl: // field-decl
        value.template destroy< field_t > ();
        break;

      case symbol_kind::S_108_fn_start_decl: // fn-start-decl
        value.template destroy< fn_kind > ();
        break;

      case symbol_kind::S_109_fn_decl: // fn-decl
        value.template destroy< fn_pure_t > ();
        break;

      case symbol_kind::S_104_generic_statement: // generic-statement
        value.template destroy< generic_statement > ();
        break;

      case symbol_kind::S_105_infunction_statement: // infunction-statement
        value.template destroy< infunction_declaration > ();
        break;

      case symbol_kind::S_103_infunction_statement_any: // infunction-statement-any
        value.template destroy< infunction_declaration_set_t > ();
        break;

      case symbol_kind::S_100_let_decl: // let-decl
      case symbol_kind::S_101_let_decl_start: // let-decl-start
      case symbol_kind::S_102_let_decl_start_with_opt_type: // let-decl-start-with-opt-type
        value.template destroy< let_statement_decl_t > ();
        break;

      case symbol_kind::S_129_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_130_opt_named_expr_list: // opt-named-expr-list
        value.template destroy< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_131_opt_named_expr: // opt-named-expr
        value.template destroy< named_expression_term_t > ();
        break;

      case symbol_kind::S_118_parameter_constraint_modifier: // parameter-constraint-modifier
      case symbol_kind::S_119_parameter_constraint_modifier_opt: // parameter-constraint-modifier-opt
        value.template destroy< parameter_constraint_modifier_t > ();
        break;

      case symbol_kind::S_120_parameter_constraint_set: // parameter-constraint-set
        value.template destroy< parameter_constraint_set_t > ();
        break;

      case symbol_kind::S_117_parameter_name_decl: // parameter-name-decl
        value.template destroy< parameter_name > ();
        break;

      case symbol_kind::S_124_parameter_decl: // parameter-decl
        value.template destroy< parameter_t > ();
        break;

      case symbol_kind::S_125_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_126_parameter_list: // parameter-list
        value.template destroy< parameter_woa_list_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.template destroy< sonia::lang::bang::annotated_decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
        value.template destroy< sonia::lang::bang::annotated_identifier > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.template destroy< sonia::lang::bang::annotated_integer > ();
        break;

      case symbol_kind::S_qname: // qname
        value.template destroy< sonia::lang::bang::annotated_qname > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTERNAL_IDENTIFIER: // INTERNAL_IDENTIFIER
      case symbol_kind::S_RESERVED_IDENTIFIER: // RESERVED_IDENTIFIER
        value.template destroy< sonia::lang::bang::annotated_string_view > ();
        break;

      case symbol_kind::S_113_case_decl: // case-decl
        value.template destroy< sonia::lang::bang::identifier > ();
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
      case symbol_kind::S_EXCLPT: // "`!`"
      case symbol_kind::S_CONTINUE: // "continue"
      case symbol_kind::S_BREAK: // "break"
      case symbol_kind::S_FN: // "`fn`"
        value.template destroy< sonia::lang::lex::resource_location > ();
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.template destroy< sonia::string_view > ();
        break;

      case symbol_kind::S_111_case_list_opt: // case-list-opt
      case symbol_kind::S_112_case_list: // case-list
        value.template destroy< std::vector<sonia::lang::bang::identifier> > ();
        break;

      case symbol_kind::S_121_concept_expression: // concept-expression
      case symbol_kind::S_122_syntax_expression_wo_ii: // syntax-expression-wo-ii
      case symbol_kind::S_123_syntax_expression: // syntax-expression
      case symbol_kind::S_128_compound_expression: // compound-expression
        value.template destroy< syntax_expression_t > ();
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
      make_INTERNAL_IDENTIFIER (sonia::lang::bang::annotated_string_view v, location_type l)
      {
        return symbol_type (token::INTERNAL_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTERNAL_IDENTIFIER (const sonia::lang::bang::annotated_string_view& v, const location_type& l)
      {
        return symbol_type (token::INTERNAL_IDENTIFIER, v, l);
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
      make_UNDERSCORE (location_type l)
      {
        return symbol_type (token::UNDERSCORE, std::move (l));
      }
#else
      static
      symbol_type
      make_UNDERSCORE (const location_type& l)
      {
        return symbol_type (token::UNDERSCORE, l);
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
      make_MINUS (location_type l)
      {
        return symbol_type (token::MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASTERISK (location_type l)
      {
        return symbol_type (token::ASTERISK, std::move (l));
      }
#else
      static
      symbol_type
      make_ASTERISK (const location_type& l)
      {
        return symbol_type (token::ASTERISK, l);
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
      make_AMPERSAND (location_type l)
      {
        return symbol_type (token::AMPERSAND, std::move (l));
      }
#else
      static
      symbol_type
      make_AMPERSAND (const location_type& l)
      {
        return symbol_type (token::AMPERSAND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITOR (location_type l)
      {
        return symbol_type (token::BITOR, std::move (l));
      }
#else
      static
      symbol_type
      make_BITOR (const location_type& l)
      {
        return symbol_type (token::BITOR, l);
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
      make_RETURN (location_type l)
      {
        return symbol_type (token::RETURN, std::move (l));
      }
#else
      static
      symbol_type
      make_RETURN (const location_type& l)
      {
        return symbol_type (token::RETURN, l);
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
      make_VOID_ (location_type l)
      {
        return symbol_type (token::VOID_, std::move (l));
      }
#else
      static
      symbol_type
      make_VOID_ (const location_type& l)
      {
        return symbol_type (token::VOID_, l);
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
      make_CONST_ (location_type l)
      {
        return symbol_type (token::CONST_, std::move (l));
      }
#else
      static
      symbol_type
      make_CONST_ (const location_type& l)
      {
        return symbol_type (token::CONST_, l);
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
      make_DELETE_ (location_type l)
      {
        return symbol_type (token::DELETE_, std::move (l));
      }
#else
      static
      symbol_type
      make_DELETE_ (const location_type& l)
      {
        return symbol_type (token::DELETE_, l);
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
      make_TYPE (location_type l)
      {
        return symbol_type (token::TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TYPE (const location_type& l)
      {
        return symbol_type (token::TYPE, l);
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
      make_TYPENAME (location_type l)
      {
        return symbol_type (token::TYPENAME, std::move (l));
      }
#else
      static
      symbol_type
      make_TYPENAME (const location_type& l)
      {
        return symbol_type (token::TYPENAME, l);
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
      make_TRUE (annotated_bool v, location_type l)
      {
        return symbol_type (token::TRUE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TRUE (const annotated_bool& v, const location_type& l)
      {
        return symbol_type (token::TRUE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FALSE (annotated_bool v, location_type l)
      {
        return symbol_type (token::FALSE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FALSE (const annotated_bool& v, const location_type& l)
      {
        return symbol_type (token::FALSE, v, l);
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
    typedef unsigned char state_type;

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

    static const signed char yypact_ninf_;
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
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

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
      yylast_ = 394,     ///< Last index in yytable_.
      yynnts_ = 36,  ///< Number of nonterminal symbols.
      yyfinal_ = 3 ///< Termination state number.
    };


    // User arguments.
    void * scanner;
    parser_context & ctx;

  };


} // bang_lang
#line 3927 "bang.tab.hpp"




#endif // !YY_BANG_LANG_BANG_TAB_HPP_INCLUDED
