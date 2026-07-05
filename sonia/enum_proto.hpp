//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef __has_cpp_attribute
#   define __has_cpp_attribute(x) 0
#endif

#if __has_cpp_attribute(clang::annotate)
#define ENUM_VALUE_STR(str) [[clang::annotate("str:" str)]]
#define ENUM_VALUE_SKIP [[clang::annotate("skip")]]
#define ENUM_VALUE_ALIAS(str) [[clang::annotate("alias:" str)]]
#define DEFINE_BITMASK_PROTO(TypeName, UnderlyingType, ...) \
    enum class [[clang::annotate("bitmask")]] \
    TypeName : UnderlyingType { __VA_ARGS__ };
#define DEFINE_ENUM_PROTO(TypeName, UnderlyingType, ...) \
    enum class [[clang::annotate("enum")]] \
    TypeName : UnderlyingType { __VA_ARGS__ };
#define DEFINE_ENUM_PROTO_EX(TypeName, UnderlyingType, Flags, ...) \
    enum class [[clang::annotate("enum:" Flags)]] \
    TypeName : UnderlyingType { __VA_ARGS__ };
#define DEFINE_BITMASK_PROTO_EX(TypeName, UnderlyingType, Flags, ...) \
    enum class [[clang::annotate("bitmask:" Flags)]] \
    TypeName : UnderlyingType { __VA_ARGS__ };
#else
#define ENUM_VALUE_STR(str)
#define ENUM_VALUE_SKIP
#define ENUM_VALUE_ALIAS(str)
#define DEFINE_BITMASK_PROTO(TypeName, UnderlyingType, ...) \
    enum class TypeName : UnderlyingType;
#define DEFINE_ENUM_PROTO(TypeName, UnderlyingType, ...) \
    enum class TypeName : UnderlyingType { __VA_ARGS__ };
#define DEFINE_ENUM_PROTO_EX(TypeName, UnderlyingType, Flags, ...) \
    enum class TypeName : UnderlyingType { __VA_ARGS__ };
#define DEFINE_BITMASK_PROTO_EX(TypeName, UnderlyingType, Flags, ...) \
    enum class TypeName : UnderlyingType;
#endif
