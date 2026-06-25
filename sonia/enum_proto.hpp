//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef __has_cpp_attribute
#   define __has_cpp_attribute(x) 0
#endif

/*
#if __has_cpp_attribute(clang::annotate)
#define DEFINE_BITMASK_PROTO(TypeName, UnderlyingType, ...) \
    struct [[clang::annotate("bitmask:" #TypeName ":" #UnderlyingType)]] \
    proto_##TypeName { \
        enum { __VA_ARGS__ }; \
    }; \
    enum class TypeName : UnderlyingType { __VA_ARGS__ };
#else
#define DEFINE_BITMASK_PROTO(...)
#endif
*/

#if __has_cpp_attribute(clang::annotate)
#define DEFINE_BITMASK_PROTO(TypeName, UnderlyingType, ...) \
    enum class [[clang::annotate("bitmask:" #TypeName ":" #UnderlyingType)]] \
    TypeName : UnderlyingType { __VA_ARGS__ };
#else
#define DEFINE_BITMASK_PROTO(TypeName, UnderlyingType, ...) \
    enum class TypeName : UnderlyingType;
#endif
