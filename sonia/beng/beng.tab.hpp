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
 ** \file beng.tab.hpp
 ** Define the beng_lang::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_BENG_LANG_BENG_TAB_HPP_INCLUDED
# define YY_BENG_LANG_BENG_TAB_HPP_INCLUDED


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
#ifndef BENG_LANGDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define BENG_LANGDEBUG 1
#  else
#   define BENG_LANGDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define BENG_LANGDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined BENG_LANGDEBUG */

namespace beng_lang {
#line 190 "beng.tab.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef BENG_LANGSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define BENG_LANGSTYPE in C++, use %define api.value.type"
# endif
    typedef BENG_LANGSTYPE value_type;
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
      // opt-type-list
      char dummy1[sizeof (beng_preliminary_tuple_t)];

      // type-expr
      char dummy2[sizeof (beng_preliminary_type)];

      // opt-decl
      char dummy3[sizeof (declaration_t)];

      // enum-decl
      char dummy4[sizeof (enum_decl)];

      // expression-list-any
      char dummy5[sizeof (expression_list_t)];

      // compound-expression
      // expression
      char dummy6[sizeof (expression_t)];

      // type-extension-any
      // type-extension-list
      char dummy7[sizeof (extension_list_t)];

      // fn-decl
      char dummy8[sizeof (fn_decl_t)];

      // opt-infunction-decl
      char dummy9[sizeof (infunction_declaration_t)];

      // INTEGER
      char dummy10[sizeof (int)];

      // opt-named-expr-list-any
      // opt-named-expr-list
      char dummy11[sizeof (named_expression_term_list_t)];

      // opt-named-expr
      char dummy12[sizeof (named_expression_term_t)];

      // parameter-list-opt
      // parameter-list
      char dummy13[sizeof (parameter_list_t)];

      // parameter-decl
      char dummy14[sizeof (parameter_t)];

      // DECIMAL
      char dummy15[sizeof (sonia::decimal)];

      // identifier
      char dummy16[sizeof (sonia::lang::beng::annotated_identifier)];

      // qname
      char dummy17[sizeof (sonia::lang::beng::annotated_qname)];

      // IDENTIFIER
      char dummy18[sizeof (sonia::lang::beng::annotated_string_view)];

      // case-decl
      char dummy19[sizeof (sonia::lang::identifier)];

      // STRING
      // OPERATOR_TERM
      char dummy20[sizeof (sonia::string_view)];

      // declaration_any
      char dummy21[sizeof (std::vector<declaration_t>)];

      // infunction_declaration_any
      char dummy22[sizeof (std::vector<infunction_declaration_t>)];

      // case-list-opt
      // case-list
      char dummy23[sizeof (std::vector<sonia::lang::identifier>)];

      // type-decl
      char dummy24[sizeof (type_decl)];
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
        BENG_LANGEMPTY = -2,
    END = 0,                       // "end of file"
    BENG_LANGerror = 256,          // error
    BENG_LANGUNDEF = 257,          // "invalid token"
    COMMENT_BEGIN = 258,           // COMMENT_BEGIN
    COMMENT_END = 259,             // COMMENT_END
    STRING = 260,                  // STRING
    IDENTIFIER = 261,              // IDENTIFIER
    DECIMAL = 262,                 // DECIMAL
    INTEGER = 263,                 // INTEGER
    OPERATOR_TERM = 264,           // OPERATOR_TERM
    UNDERSCORE = 265,              // "`_`"
    ARROWAST = 266,                // "`->*`"
    ARROW = 267,                   // "`->`"
    FARROW = 268,                  // "`~>`"
    PTAST = 269,                   // "`.*`"
    DBLPLUS = 270,                 // "`++`"
    DBLMINUS = 271,                // "`--`"
    LEFTSHIFT = 272,               // "`<<`"
    RIGHTSHIFT = 273,              // "`>>`"
    LE = 274,                      // "`<=`"
    GE = 275,                      // "`>=`"
    EQ = 276,                      // "`==`"
    NE = 277,                      // "`!=`"
    AND = 278,                     // "`&&`"
    LOGIC_OR = 279,                // "`||`"
    ADDASSIGN = 280,               // "`+=`"
    SUBASSIGN = 281,               // "`-=`"
    MULASSIGN = 282,               // "`*=`"
    DIVASSIGN = 283,               // "`/=`"
    MODASSIGN = 284,               // "`%=`"
    LSHIFTSSIGN = 285,             // "`<<=`"
    RSHIFTSSIGN = 286,             // "`>>=`"
    BITANDASSIGN = 287,            // "`&=`"
    BITORASSIGN = 288,             // "`|=`"
    BITXORASSIGN = 289,            // "`^=`"
    COLON = 290,                   // "`:`"
    OPEN_PARENTHESIS = 291,        // "`(`"
    CLOSE_PARENTHESIS = 292,       // "`)`"
    OPEN_BRACE = 293,              // "`{`"
    CLOSE_BRACE = 294,             // "`}`"
    OPEN_SQUARE_BRACKET = 295,     // "`[`"
    CLOSE_SQUARE_BRACKET = 296,    // "`]`"
    OPEN_BROKET = 297,             // "`<`"
    CLOSE_BROKET = 298,            // "`>`"
    END_STATEMENT = 299,           // "`;`"
    POINT = 300,                   // "`.`"
    PLUS = 301,                    // "`+`"
    MINUS = 302,                   // "`-`"
    ASTERISK = 303,                // "`*`"
    SLASH = 304,                   // "`/`"
    PERCENT = 305,                 // "`%`"
    AMPERSAND = 306,               // "`&`"
    BITOR = 307,                   // "`|`"
    EXCLPT = 308,                  // "`!`"
    TILDA = 309,                   // "`~`"
    EXCL = 310,                    // "`^`"
    COMMA = 311,                   // ","
    DOLLAR = 312,                  // "`$`"
    QMARK = 313,                   // "`?`"
    HASHTAG = 314,                 // "`#`"
    LET = 315,                     // LET
    VAR = 316,                     // VAR
    NEW = 317,                     // NEW
    FOR = 318,                     // FOR
    VOID_ = 319,                   // VOID_
    AUTO = 320,                    // AUTO
    CONST_ = 321,                  // CONST_
    USING = 322,                   // USING
    THROW = 323,                   // THROW
    DELETE_ = 324,                 // DELETE_
    SIZEOF = 325,                  // SIZEOF
    RETURN = 326,                  // RETURN
    EXTERN = 327,                  // EXTERN
    PRIVATE = 328,                 // PRIVATE
    TEMPLATE = 329,                // TEMPLATE
    NAMESPACE = 330,               // NAMESPACE
    LOWEST = 331,                  // LOWEST
    ASSIGN = 332,                  // ASSIGN
    DBLCOLON = 334,                // DBLCOLON
    FN = 336,                      // FN
    ENUM = 337,                    // ENUM
    TYPE = 338,                    // TYPE
    EXTENDS = 339,                 // EXTENDS
    BOOL = 340,                    // BOOL
    INT = 341,                     // INT
    FLOAT = 342,                   // FLOAT
    STRING_WORD = 343,             // STRING_WORD
    DECIMAL_WORD = 344             // DECIMAL_WORD
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
        YYNTOKENS = 90, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_COMMENT_BEGIN = 3,                     // COMMENT_BEGIN
        S_COMMENT_END = 4,                       // COMMENT_END
        S_STRING = 5,                            // STRING
        S_IDENTIFIER = 6,                        // IDENTIFIER
        S_DECIMAL = 7,                           // DECIMAL
        S_INTEGER = 8,                           // INTEGER
        S_OPERATOR_TERM = 9,                     // OPERATOR_TERM
        S_UNDERSCORE = 10,                       // "`_`"
        S_ARROWAST = 11,                         // "`->*`"
        S_ARROW = 12,                            // "`->`"
        S_FARROW = 13,                           // "`~>`"
        S_PTAST = 14,                            // "`.*`"
        S_DBLPLUS = 15,                          // "`++`"
        S_DBLMINUS = 16,                         // "`--`"
        S_LEFTSHIFT = 17,                        // "`<<`"
        S_RIGHTSHIFT = 18,                       // "`>>`"
        S_LE = 19,                               // "`<=`"
        S_GE = 20,                               // "`>=`"
        S_EQ = 21,                               // "`==`"
        S_NE = 22,                               // "`!=`"
        S_AND = 23,                              // "`&&`"
        S_LOGIC_OR = 24,                         // "`||`"
        S_ADDASSIGN = 25,                        // "`+=`"
        S_SUBASSIGN = 26,                        // "`-=`"
        S_MULASSIGN = 27,                        // "`*=`"
        S_DIVASSIGN = 28,                        // "`/=`"
        S_MODASSIGN = 29,                        // "`%=`"
        S_LSHIFTSSIGN = 30,                      // "`<<=`"
        S_RSHIFTSSIGN = 31,                      // "`>>=`"
        S_BITANDASSIGN = 32,                     // "`&=`"
        S_BITORASSIGN = 33,                      // "`|=`"
        S_BITXORASSIGN = 34,                     // "`^=`"
        S_COLON = 35,                            // "`:`"
        S_OPEN_PARENTHESIS = 36,                 // "`(`"
        S_CLOSE_PARENTHESIS = 37,                // "`)`"
        S_OPEN_BRACE = 38,                       // "`{`"
        S_CLOSE_BRACE = 39,                      // "`}`"
        S_OPEN_SQUARE_BRACKET = 40,              // "`[`"
        S_CLOSE_SQUARE_BRACKET = 41,             // "`]`"
        S_OPEN_BROKET = 42,                      // "`<`"
        S_CLOSE_BROKET = 43,                     // "`>`"
        S_END_STATEMENT = 44,                    // "`;`"
        S_POINT = 45,                            // "`.`"
        S_PLUS = 46,                             // "`+`"
        S_MINUS = 47,                            // "`-`"
        S_ASTERISK = 48,                         // "`*`"
        S_SLASH = 49,                            // "`/`"
        S_PERCENT = 50,                          // "`%`"
        S_AMPERSAND = 51,                        // "`&`"
        S_BITOR = 52,                            // "`|`"
        S_EXCLPT = 53,                           // "`!`"
        S_TILDA = 54,                            // "`~`"
        S_EXCL = 55,                             // "`^`"
        S_COMMA = 56,                            // ","
        S_DOLLAR = 57,                           // "`$`"
        S_QMARK = 58,                            // "`?`"
        S_HASHTAG = 59,                          // "`#`"
        S_LET = 60,                              // LET
        S_VAR = 61,                              // VAR
        S_NEW = 62,                              // NEW
        S_FOR = 63,                              // FOR
        S_VOID_ = 64,                            // VOID_
        S_AUTO = 65,                             // AUTO
        S_CONST_ = 66,                           // CONST_
        S_USING = 67,                            // USING
        S_THROW = 68,                            // THROW
        S_DELETE_ = 69,                          // DELETE_
        S_SIZEOF = 70,                           // SIZEOF
        S_RETURN = 71,                           // RETURN
        S_EXTERN = 72,                           // EXTERN
        S_PRIVATE = 73,                          // PRIVATE
        S_TEMPLATE = 74,                         // TEMPLATE
        S_NAMESPACE = 75,                        // NAMESPACE
        S_LOWEST = 76,                           // LOWEST
        S_ASSIGN = 77,                           // ASSIGN
        S_78_ = 78,                              // "`=`"
        S_DBLCOLON = 79,                         // DBLCOLON
        S_80_ = 80,                              // "`::`"
        S_FN = 81,                               // FN
        S_ENUM = 82,                             // ENUM
        S_TYPE = 83,                             // TYPE
        S_EXTENDS = 84,                          // EXTENDS
        S_BOOL = 85,                             // BOOL
        S_INT = 86,                              // INT
        S_FLOAT = 87,                            // FLOAT
        S_STRING_WORD = 88,                      // STRING_WORD
        S_DECIMAL_WORD = 89,                     // DECIMAL_WORD
        S_YYACCEPT = 90,                         // $accept
        S_begin = 91,                            // begin
        S_declaration_any = 92,                  // declaration_any
        S_93_opt_decl = 93,                      // opt-decl
        S_infunction_declaration_any = 94,       // infunction_declaration_any
        S_95_opt_infunction_decl = 95,           // opt-infunction-decl
        S_identifier = 96,                       // identifier
        S_qname = 97,                            // qname
        S_98_fn_decl = 98,                       // fn-decl
        S_99_enum_decl = 99,                     // enum-decl
        S_100_case_list_opt = 100,               // case-list-opt
        S_101_case_list = 101,                   // case-list
        S_102_case_decl = 102,                   // case-decl
        S_103_type_decl = 103,                   // type-decl
        S_104_type_extension_any = 104,          // type-extension-any
        S_105_type_extension_list = 105,         // type-extension-list
        S_106_parameter_list_opt = 106,          // parameter-list-opt
        S_107_parameter_list = 107,              // parameter-list
        S_108_parameter_decl = 108,              // parameter-decl
        S_109_type_expr = 109,                   // type-expr
        S_110_opt_type_list = 110,               // opt-type-list
        S_111_compound_expression = 111,         // compound-expression
        S_expression = 112,                      // expression
        S_113_expression_list_any = 113,         // expression-list-any
        S_114_opt_named_expr_list_any = 114,     // opt-named-expr-list-any
        S_115_opt_named_expr_list = 115,         // opt-named-expr-list
        S_116_opt_named_expr = 116               // opt-named-expr
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
      case symbol_kind::S_110_opt_type_list: // opt-type-list
        value.move< beng_preliminary_tuple_t > (std::move (that.value));
        break;

      case symbol_kind::S_109_type_expr: // type-expr
        value.move< beng_preliminary_type > (std::move (that.value));
        break;

      case symbol_kind::S_93_opt_decl: // opt-decl
        value.move< declaration_t > (std::move (that.value));
        break;

      case symbol_kind::S_99_enum_decl: // enum-decl
        value.move< enum_decl > (std::move (that.value));
        break;

      case symbol_kind::S_113_expression_list_any: // expression-list-any
        value.move< expression_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_111_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.move< expression_t > (std::move (that.value));
        break;

      case symbol_kind::S_104_type_extension_any: // type-extension-any
      case symbol_kind::S_105_type_extension_list: // type-extension-list
        value.move< extension_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_98_fn_decl: // fn-decl
        value.move< fn_decl_t > (std::move (that.value));
        break;

      case symbol_kind::S_95_opt_infunction_decl: // opt-infunction-decl
        value.move< infunction_declaration_t > (std::move (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (std::move (that.value));
        break;

      case symbol_kind::S_114_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_115_opt_named_expr_list: // opt-named-expr-list
        value.move< named_expression_term_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_116_opt_named_expr: // opt-named-expr
        value.move< named_expression_term_t > (std::move (that.value));
        break;

      case symbol_kind::S_106_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_107_parameter_list: // parameter-list
        value.move< parameter_list_t > (std::move (that.value));
        break;

      case symbol_kind::S_108_parameter_decl: // parameter-decl
        value.move< parameter_t > (std::move (that.value));
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.move< sonia::decimal > (std::move (that.value));
        break;

      case symbol_kind::S_identifier: // identifier
        value.move< sonia::lang::beng::annotated_identifier > (std::move (that.value));
        break;

      case symbol_kind::S_qname: // qname
        value.move< sonia::lang::beng::annotated_qname > (std::move (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< sonia::lang::beng::annotated_string_view > (std::move (that.value));
        break;

      case symbol_kind::S_102_case_decl: // case-decl
        value.move< sonia::lang::identifier > (std::move (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.move< sonia::string_view > (std::move (that.value));
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.move< std::vector<declaration_t> > (std::move (that.value));
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.move< std::vector<infunction_declaration_t> > (std::move (that.value));
        break;

      case symbol_kind::S_100_case_list_opt: // case-list-opt
      case symbol_kind::S_101_case_list: // case-list
        value.move< std::vector<sonia::lang::identifier> > (std::move (that.value));
        break;

      case symbol_kind::S_103_type_decl: // type-decl
        value.move< type_decl > (std::move (that.value));
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
      basic_symbol (typename Base::kind_type t, beng_preliminary_tuple_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const beng_preliminary_tuple_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, beng_preliminary_type&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const beng_preliminary_type& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, declaration_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const declaration_t& v, const location_type& l)
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
      basic_symbol (typename Base::kind_type t, expression_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const expression_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, extension_list_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const extension_list_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, fn_decl_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const fn_decl_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, infunction_declaration_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const infunction_declaration_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
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
      basic_symbol (typename Base::kind_type t, sonia::decimal&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::decimal& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::beng::annotated_identifier&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::beng::annotated_identifier& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::beng::annotated_qname&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::beng::annotated_qname& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::beng::annotated_string_view&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::beng::annotated_string_view& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sonia::lang::identifier&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sonia::lang::identifier& v, const location_type& l)
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
      basic_symbol (typename Base::kind_type t, std::vector<declaration_t>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<declaration_t>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<infunction_declaration_t>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<infunction_declaration_t>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<sonia::lang::identifier>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<sonia::lang::identifier>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, type_decl&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const type_decl& v, const location_type& l)
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
#line 265 "beng.y"
                    { }
#line 1249 "beng.tab.hpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 265 "beng.y"
                    { }
#line 1255 "beng.tab.hpp"
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
#line 265 "beng.y"
                    { }
#line 1261 "beng.tab.hpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 265 "beng.y"
                    { }
#line 1267 "beng.tab.hpp"
        break;

      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
#line 265 "beng.y"
                    { }
#line 1273 "beng.tab.hpp"
        break;

      case symbol_kind::S_declaration_any: // declaration_any
#line 265 "beng.y"
                    { }
#line 1279 "beng.tab.hpp"
        break;

      case symbol_kind::S_93_opt_decl: // opt-decl
#line 265 "beng.y"
                    { }
#line 1285 "beng.tab.hpp"
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
#line 265 "beng.y"
                    { }
#line 1291 "beng.tab.hpp"
        break;

      case symbol_kind::S_95_opt_infunction_decl: // opt-infunction-decl
#line 265 "beng.y"
                    { }
#line 1297 "beng.tab.hpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 265 "beng.y"
                    { }
#line 1303 "beng.tab.hpp"
        break;

      case symbol_kind::S_qname: // qname
#line 265 "beng.y"
                    { }
#line 1309 "beng.tab.hpp"
        break;

      case symbol_kind::S_98_fn_decl: // fn-decl
#line 265 "beng.y"
                    { }
#line 1315 "beng.tab.hpp"
        break;

      case symbol_kind::S_99_enum_decl: // enum-decl
#line 265 "beng.y"
                    { }
#line 1321 "beng.tab.hpp"
        break;

      case symbol_kind::S_100_case_list_opt: // case-list-opt
#line 265 "beng.y"
                    { }
#line 1327 "beng.tab.hpp"
        break;

      case symbol_kind::S_101_case_list: // case-list
#line 265 "beng.y"
                    { }
#line 1333 "beng.tab.hpp"
        break;

      case symbol_kind::S_102_case_decl: // case-decl
#line 265 "beng.y"
                    { }
#line 1339 "beng.tab.hpp"
        break;

      case symbol_kind::S_103_type_decl: // type-decl
#line 265 "beng.y"
                    { }
#line 1345 "beng.tab.hpp"
        break;

      case symbol_kind::S_104_type_extension_any: // type-extension-any
#line 265 "beng.y"
                    { }
#line 1351 "beng.tab.hpp"
        break;

      case symbol_kind::S_105_type_extension_list: // type-extension-list
#line 265 "beng.y"
                    { }
#line 1357 "beng.tab.hpp"
        break;

      case symbol_kind::S_106_parameter_list_opt: // parameter-list-opt
#line 265 "beng.y"
                    { }
#line 1363 "beng.tab.hpp"
        break;

      case symbol_kind::S_107_parameter_list: // parameter-list
#line 265 "beng.y"
                    { }
#line 1369 "beng.tab.hpp"
        break;

      case symbol_kind::S_108_parameter_decl: // parameter-decl
#line 265 "beng.y"
                    { }
#line 1375 "beng.tab.hpp"
        break;

      case symbol_kind::S_109_type_expr: // type-expr
#line 265 "beng.y"
                    { }
#line 1381 "beng.tab.hpp"
        break;

      case symbol_kind::S_110_opt_type_list: // opt-type-list
#line 265 "beng.y"
                    { }
#line 1387 "beng.tab.hpp"
        break;

      case symbol_kind::S_111_compound_expression: // compound-expression
#line 265 "beng.y"
                    { }
#line 1393 "beng.tab.hpp"
        break;

      case symbol_kind::S_expression: // expression
#line 265 "beng.y"
                    { }
#line 1399 "beng.tab.hpp"
        break;

      case symbol_kind::S_113_expression_list_any: // expression-list-any
#line 265 "beng.y"
                    { }
#line 1405 "beng.tab.hpp"
        break;

      case symbol_kind::S_114_opt_named_expr_list_any: // opt-named-expr-list-any
#line 265 "beng.y"
                    { }
#line 1411 "beng.tab.hpp"
        break;

      case symbol_kind::S_115_opt_named_expr_list: // opt-named-expr-list
#line 265 "beng.y"
                    { }
#line 1417 "beng.tab.hpp"
        break;

      case symbol_kind::S_116_opt_named_expr: // opt-named-expr
#line 265 "beng.y"
                    { }
#line 1423 "beng.tab.hpp"
        break;

       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_110_opt_type_list: // opt-type-list
        value.template destroy< beng_preliminary_tuple_t > ();
        break;

      case symbol_kind::S_109_type_expr: // type-expr
        value.template destroy< beng_preliminary_type > ();
        break;

      case symbol_kind::S_93_opt_decl: // opt-decl
        value.template destroy< declaration_t > ();
        break;

      case symbol_kind::S_99_enum_decl: // enum-decl
        value.template destroy< enum_decl > ();
        break;

      case symbol_kind::S_113_expression_list_any: // expression-list-any
        value.template destroy< expression_list_t > ();
        break;

      case symbol_kind::S_111_compound_expression: // compound-expression
      case symbol_kind::S_expression: // expression
        value.template destroy< expression_t > ();
        break;

      case symbol_kind::S_104_type_extension_any: // type-extension-any
      case symbol_kind::S_105_type_extension_list: // type-extension-list
        value.template destroy< extension_list_t > ();
        break;

      case symbol_kind::S_98_fn_decl: // fn-decl
        value.template destroy< fn_decl_t > ();
        break;

      case symbol_kind::S_95_opt_infunction_decl: // opt-infunction-decl
        value.template destroy< infunction_declaration_t > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.template destroy< int > ();
        break;

      case symbol_kind::S_114_opt_named_expr_list_any: // opt-named-expr-list-any
      case symbol_kind::S_115_opt_named_expr_list: // opt-named-expr-list
        value.template destroy< named_expression_term_list_t > ();
        break;

      case symbol_kind::S_116_opt_named_expr: // opt-named-expr
        value.template destroy< named_expression_term_t > ();
        break;

      case symbol_kind::S_106_parameter_list_opt: // parameter-list-opt
      case symbol_kind::S_107_parameter_list: // parameter-list
        value.template destroy< parameter_list_t > ();
        break;

      case symbol_kind::S_108_parameter_decl: // parameter-decl
        value.template destroy< parameter_t > ();
        break;

      case symbol_kind::S_DECIMAL: // DECIMAL
        value.template destroy< sonia::decimal > ();
        break;

      case symbol_kind::S_identifier: // identifier
        value.template destroy< sonia::lang::beng::annotated_identifier > ();
        break;

      case symbol_kind::S_qname: // qname
        value.template destroy< sonia::lang::beng::annotated_qname > ();
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.template destroy< sonia::lang::beng::annotated_string_view > ();
        break;

      case symbol_kind::S_102_case_decl: // case-decl
        value.template destroy< sonia::lang::identifier > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPERATOR_TERM: // OPERATOR_TERM
        value.template destroy< sonia::string_view > ();
        break;

      case symbol_kind::S_declaration_any: // declaration_any
        value.template destroy< std::vector<declaration_t> > ();
        break;

      case symbol_kind::S_infunction_declaration_any: // infunction_declaration_any
        value.template destroy< std::vector<infunction_declaration_t> > ();
        break;

      case symbol_kind::S_100_case_list_opt: // case-list-opt
      case symbol_kind::S_101_case_list: // case-list
        value.template destroy< std::vector<sonia::lang::identifier> > ();
        break;

      case symbol_kind::S_103_type_decl: // type-decl
        value.template destroy< type_decl > ();
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
      symbol_type (int tok, int v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, sonia::decimal v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const sonia::decimal& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, sonia::lang::beng::annotated_string_view v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const sonia::lang::beng::annotated_string_view& v, const location_type& l)
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

#if BENG_LANGDEBUG
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
      make_BENG_LANGerror (location_type l)
      {
        return symbol_type (token::BENG_LANGerror, std::move (l));
      }
#else
      static
      symbol_type
      make_BENG_LANGerror (const location_type& l)
      {
        return symbol_type (token::BENG_LANGerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BENG_LANGUNDEF (location_type l)
      {
        return symbol_type (token::BENG_LANGUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_BENG_LANGUNDEF (const location_type& l)
      {
        return symbol_type (token::BENG_LANGUNDEF, l);
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
      make_STRING (sonia::string_view v, location_type l)
      {
        return symbol_type (token::STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRING (const sonia::string_view& v, const location_type& l)
      {
        return symbol_type (token::STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (sonia::lang::beng::annotated_string_view v, location_type l)
      {
        return symbol_type (token::IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const sonia::lang::beng::annotated_string_view& v, const location_type& l)
      {
        return symbol_type (token::IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DECIMAL (sonia::decimal v, location_type l)
      {
        return symbol_type (token::DECIMAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DECIMAL (const sonia::decimal& v, const location_type& l)
      {
        return symbol_type (token::DECIMAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTEGER (int v, location_type l)
      {
        return symbol_type (token::INTEGER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTEGER (const int& v, const location_type& l)
      {
        return symbol_type (token::INTEGER, v, l);
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
      make_EQ (location_type l)
      {
        return symbol_type (token::EQ, std::move (l));
      }
#else
      static
      symbol_type
      make_EQ (const location_type& l)
      {
        return symbol_type (token::EQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NE (location_type l)
      {
        return symbol_type (token::NE, std::move (l));
      }
#else
      static
      symbol_type
      make_NE (const location_type& l)
      {
        return symbol_type (token::NE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AND (location_type l)
      {
        return symbol_type (token::AND, std::move (l));
      }
#else
      static
      symbol_type
      make_AND (const location_type& l)
      {
        return symbol_type (token::AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOGIC_OR (location_type l)
      {
        return symbol_type (token::LOGIC_OR, std::move (l));
      }
#else
      static
      symbol_type
      make_LOGIC_OR (const location_type& l)
      {
        return symbol_type (token::LOGIC_OR, l);
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
      make_OPEN_PARENTHESIS (location_type l)
      {
        return symbol_type (token::OPEN_PARENTHESIS, std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_PARENTHESIS (const location_type& l)
      {
        return symbol_type (token::OPEN_PARENTHESIS, l);
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
      make_OPEN_BRACE (location_type l)
      {
        return symbol_type (token::OPEN_BRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_BRACE (const location_type& l)
      {
        return symbol_type (token::OPEN_BRACE, l);
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
      make_OPEN_SQUARE_BRACKET (location_type l)
      {
        return symbol_type (token::OPEN_SQUARE_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_OPEN_SQUARE_BRACKET (const location_type& l)
      {
        return symbol_type (token::OPEN_SQUARE_BRACKET, l);
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
      make_POINT (location_type l)
      {
        return symbol_type (token::POINT, std::move (l));
      }
#else
      static
      symbol_type
      make_POINT (const location_type& l)
      {
        return symbol_type (token::POINT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::PLUS, l);
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
      make_EXCLPT (location_type l)
      {
        return symbol_type (token::EXCLPT, std::move (l));
      }
#else
      static
      symbol_type
      make_EXCLPT (const location_type& l)
      {
        return symbol_type (token::EXCLPT, l);
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
      make_NEW (location_type l)
      {
        return symbol_type (token::NEW, std::move (l));
      }
#else
      static
      symbol_type
      make_NEW (const location_type& l)
      {
        return symbol_type (token::NEW, l);
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
      make_PRIVATE (location_type l)
      {
        return symbol_type (token::PRIVATE, std::move (l));
      }
#else
      static
      symbol_type
      make_PRIVATE (const location_type& l)
      {
        return symbol_type (token::PRIVATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEMPLATE (location_type l)
      {
        return symbol_type (token::TEMPLATE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEMPLATE (const location_type& l)
      {
        return symbol_type (token::TEMPLATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NAMESPACE (location_type l)
      {
        return symbol_type (token::NAMESPACE, std::move (l));
      }
#else
      static
      symbol_type
      make_NAMESPACE (const location_type& l)
      {
        return symbol_type (token::NAMESPACE, l);
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
      make_ASSIGN (location_type l)
      {
        return symbol_type (token::ASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_ASSIGN (const location_type& l)
      {
        return symbol_type (token::ASSIGN, l);
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
      make_FN (location_type l)
      {
        return symbol_type (token::FN, std::move (l));
      }
#else
      static
      symbol_type
      make_FN (const location_type& l)
      {
        return symbol_type (token::FN, l);
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
      make_BOOL (location_type l)
      {
        return symbol_type (token::BOOL, std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL (const location_type& l)
      {
        return symbol_type (token::BOOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT (location_type l)
      {
        return symbol_type (token::INT, std::move (l));
      }
#else
      static
      symbol_type
      make_INT (const location_type& l)
      {
        return symbol_type (token::INT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT (location_type l)
      {
        return symbol_type (token::FLOAT, std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT (const location_type& l)
      {
        return symbol_type (token::FLOAT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRING_WORD (location_type l)
      {
        return symbol_type (token::STRING_WORD, std::move (l));
      }
#else
      static
      symbol_type
      make_STRING_WORD (const location_type& l)
      {
        return symbol_type (token::STRING_WORD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DECIMAL_WORD (location_type l)
      {
        return symbol_type (token::DECIMAL_WORD, std::move (l));
      }
#else
      static
      symbol_type
      make_DECIMAL_WORD (const location_type& l)
      {
        return symbol_type (token::DECIMAL_WORD, l);
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
    static const unsigned char yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if BENG_LANGDEBUG
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
      yylast_ = 253,     ///< Last index in yytable_.
      yynnts_ = 27,  ///< Number of nonterminal symbols.
      yyfinal_ = 3 ///< Termination state number.
    };


    // User arguments.
    void * scanner;
    parser_context & ctx;

  };


} // beng_lang
#line 3375 "beng.tab.hpp"




#endif // !YY_BENG_LANG_BENG_TAB_HPP_INCLUDED
