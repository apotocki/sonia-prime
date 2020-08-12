// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_TYPE_TRAITS_HPP
#define AGNOSTIC_STD_TYPE_TRAITS_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "type_traits/integral_constant.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF
#   include "agnostic/std/type_traits/alignment_of.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_CONDITIONAL
#   include "type_traits/conditional.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_CONDITIONAL_EXT
#   include "type_traits/conditional_ext.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_CONJUNCTION
#   include "type_traits/conjunction.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DISJUNCTION
#   include "type_traits/disjunction.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_NEGATION
#   include "type_traits/negation.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_SAME
#   include "type_traits/is_same.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ANY_OF
#   include "type_traits/is_any_of.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_VOID
#   include "type_traits/is_void.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_CONST
#   include "type_traits/is_const.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_VOLATILE
#   include "type_traits/is_volatile.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_FUNCTION
#   include "type_traits/is_function.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ARRAY
#   include "type_traits/is_array.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_POINTER
#   include "type_traits/is_pointer.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_REFERENCE
#   include "type_traits/is_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_LVALUE_REFERENCE
#   include "type_traits/is_lvalue_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_RVALUE_REFERENCE
#   include "type_traits/is_rvalue_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_EMPTY
#   include "type_traits/is_empty.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_CONSTRUCTIBLE
#   include "type_traits/is_trivially_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_COPYABLE
#   include "type_traits/is_trivially_copyable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_ASSIGNABLE
#   include "type_traits/is_trivially_assignable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_COPY_ASSIGNABLE
#   include "type_traits/is_trivially_copy_assignable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_DESTRUCTIBLE
#   include "type_traits/is_trivially_destructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIAL
#   include "type_traits/is_trivial.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_SWAPPABLE_WITH
#   include "type_traits/is_swappable_with.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_SWAPPABLE_WITH
#   include "type_traits/is_nothrow_swappable_with.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_SWAPPABLE
#   include "type_traits/is_nothrow_swappable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_EXTENT
#   include "type_traits/extent.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_RANK
#   include "type_traits/rank.hpp"
#endif


#ifndef DO_NOT_USE_AGNOSTIC_ADD_CONST
#   include "type_traits/add_const.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADD_VOLATILE
#   include "type_traits/add_volatile.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADD_CV
#   include "type_traits/add_cv.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADD_LVALUE_REFERENCE
#   include "type_traits/add_lvalue_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADD_LVALUE_REFERENCE
#   include "type_traits/add_rvalue_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADD_POINTER
#   include "type_traits/add_pointer.hpp"
#endif


#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_CONST
#   include "type_traits/remove_const.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_VOLATILE
#   include "type_traits/remove_volatile.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_CV
#   include "type_traits/remove_cv.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#   include "type_traits/remove_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_EXTENT
#   include "type_traits/remove_extent.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_ALL_EXTENTS
#   include "type_traits/remove_all_extents.hpp"
#endif


#ifndef DO_NOT_USE_AGNOSTIC_TYPE_IDENTITY
#   include "type_traits/type_identity.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_VOID
#   include "type_traits/void.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_CONSTRUCTIBLE
#   include "type_traits/is_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_CONSTRUCTIBLE
#   include "type_traits/is_nothrow_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_DEFAULT_CONSTRUCTIBLE
#   include "type_traits/is_default_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE
#   include "type_traits/is_nothrow_default_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE
#   include "type_traits/is_implicitly_default_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_COPY_CONSTRUCTIBLE
#   include "type_traits/is_copy_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_COPY_CONSTRUCTIBLE
#   include "type_traits/is_nothrow_copy_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ASSIGNABLE
#   include "type_traits/is_assignable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_ASSIGNABLE
#   include "type_traits/is_nothrow_assignable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_ASSIGNABLE
#   include "type_traits/is_nothrow_move_assignable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_CONVERTIBLE
#   include "type_traits/is_convertible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ENABLE_IF
#   include "type_traits/enable_if.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DISABLE_IF
#   include "type_traits/disable_if.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_AS_CONST
#   include "type_traits/as_const.hpp"
#endif


#ifndef DO_NOT_USE_AGNOSTIC_IS_INTEGRAL
#   include "type_traits/is_integral.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_FLOATING_POINT
#   include "type_traits/is_floating_point.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ARITHMETIC
#   include "type_traits/is_arithmetic.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_UNSIGNED
#   include "type_traits/is_unsigned.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ENUM
#   include "type_traits/is_enum.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_UNION
#   include "type_traits/is_union.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_CLASS
#   include "type_traits/is_class.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_OBJECT
#   include "type_traits/is_object.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_MEMBER_POINTER
#   include "type_traits/is_member_pointer.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NULL_POINTER
#   include "type_traits/is_null_pointer.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_SCALAR
#   include "type_traits/is_scalar.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_MAKE_UNSIGNED
#   include "type_traits/make_unsigned.hpp"
#endif

#endif // AGNOSTIC_STD_TYPE_TRAITS_HPP
