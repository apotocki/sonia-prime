// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>

#define DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#define DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF
#define DO_NOT_USE_AGNOSTIC_ENABLE_IF
#define DO_NOT_USE_AGNOSTIC_PLACEMENT_NEW
#define DO_NOT_USE_AGNOSTIC_DESTROY_AT
#define DO_NOT_USE_AGNOSTIC_FORWARD
#define DO_NOT_USE_AGNOSTIC_CONDITIONAL
#define DO_NOT_USE_AGNOSTIC_TYPE_IDENTITY
#define DO_NOT_USE_AGNOSTIC_ADDRESSOF
#define DO_NOT_USE_AGNOSTIC_POINTER_TRAITS
#define DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_ADD_CONST
#define DO_NOT_USE_AGNOSTIC_ADD_LVALUE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_REMOVE_CV
#define DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TAGS
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR
#define DO_NOT_USE_AGNOSTIC_ALLOCATOR
#define DO_NOT_USE_AGNOSTIC_IS_ARRAY
#define DO_NOT_USE_AGNOSTIC_IS_POINTER
#define DO_NOT_USE_AGNOSTIC_IS_SAME
#define DO_NOT_USE_AGNOSTIC_IS_VOID
#define DO_NOT_USE_AGNOSTIC_IS_ENUM
#define DO_NOT_USE_AGNOSTIC_IS_FUNCTION
#define DO_NOT_USE_AGNOSTIC_IS_INTEGRAL
#define DO_NOT_USE_AGNOSTIC_IS_FLOATING_POINT
#define DO_NOT_USE_AGNOSTIC_IS_ARITHMETIC
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIAL
#define DO_NOT_USE_AGNOSTIC_IS_MEMBER_POINTER
#define DO_NOT_USE_AGNOSTIC_IS_NULL_POINTER
#define DO_NOT_USE_AGNOSTIC_IS_SCALAR
#define DO_NOT_USE_AGNOSTIC_IS_VOLATILE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_COPYABLE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_DESTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_DEFAULT_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_ASSIGNABLE
#define DO_NOT_USE_AGNOSTIC_IS_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_CONVERTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_COPY_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_COPY_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_ASSIGNABLE
#define DO_NOT_USE_AGNOSTIC_DISJUNCTION
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_COPY
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MOVE
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MAX_ELEMENT
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MAX
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_COPY
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_MOVE
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_DEFAULT_CONSTRUCT
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_VALUE_CONSTRUCT
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_FILL
#define DO_NOT_USE_AGNOSTIC_DESTROY
#define DO_NOT_USE_AGNOSTIC_ITERATOR_BEGIN
#define DO_NOT_USE_AGNOSTIC_ITERATOR_END
#define DO_NOT_USE_AGNOSTIC_ADD_RVALUE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_CONVERTIBLE_TO
#define DO_NOT_USE_AGNOSTIC_SAME_AS
#define DO_NOT_USE_AGNOSTIC_NOTHROW
#define DO_NOT_USE_AGNOSTIC_IN_PLACE
#define DO_NOT_USE_AGNOSTIC_FLOAT_ROUND_STYLE
#define DO_NOT_USE_AGNOSTIC_FLOAT_DENORM_STYLE
#define DO_NOT_USE_AGNOSTIC_NUMERIC_LIMITS
#define DO_NOT_USE_AGNOSTIC_INITIALIZER_LIST
#define DO_NOT_USE_AGNOSTIC_PAIR
#define DO_NOT_USE_AGNOSTIC_ORDERING
#define DO_NOT_USE_AGNOSTIC_UNDERLYING_TYPE
#define DO_NOT_USE_AGNOSTIC_LAUNDER
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MIN_ELEMENT
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MIN
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_ITER_SWAP
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_SWAP_RANGES
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MISMATCH
#define DO_NOT_USE_AGNOSTIC_SAME_AS_CONCEPT
#define DO_NOT_USE_AGNOSTIC_RANDOM_ACCESS_ITERATOR_CONCEPT

#include <compare>
#include <functional>

#include "agnostic/allocator/BareAllocator.hpp"
#include "agnostic/allocator/default_bare_allocator.hpp"
#include "agnostic/ipo/vector.hpp"


#include <set>
#include <vector>
#include <memory>
#include <array>
#include <list>

#define AGNOSTIC_CHECK(x) BOOST_CHECK(x)
#define AGNOSTIC_CHECK_EQUAL(a, b) BOOST_CHECK_EQUAL(a, b)

#include "vector_test.ipp"

BOOST_AUTO_TEST_CASE(agnostic_vector_test)
{
    //std::make_signed
    //shell<int> v;
    //std::RandomAccessIterator
    agnostic_vector_test_impl();
}