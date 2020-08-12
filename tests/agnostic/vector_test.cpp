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
#define DO_NOT_USE_AGNOSTIC_IS_ARRAY
#define DO_NOT_USE_AGNOSTIC_IS_POINTER
#define DO_NOT_USE_AGNOSTIC_ADDRESSOF
#define DO_NOT_USE_AGNOSTIC_POINTER_TRAITS
#define DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_REMOVE_CV
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TAGS
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR

#include "agnostic/optimized/vector.hpp"

namespace {

struct test_element0
{
    alignas(16) char dummy[16];
};

}

BOOST_AUTO_TEST_CASE(agnostic_vector_test)
{
    using vec0_helper_t = agnostic::vector_detail::helper<int, 0>;
    static_assert(vec0_helper_t::begin_offs == 1);
    static_assert(vec0_helper_t::aligned_offs == 1);
    static_assert(vec0_helper_t::holder_sz == sizeof(uintptr_t));

    using vec01_helper_t = agnostic::vector_detail::helper<int, 1>;
    static_assert(vec01_helper_t::begin_offs == 1);
    static_assert(vec01_helper_t::aligned_offs == sizeof(int));
    static_assert(vec01_helper_t::holder_sz == sizeof(uintptr_t));

    using vec02_helper_t = agnostic::vector_detail::helper<int, 2>;
    static_assert(vec02_helper_t::begin_offs == 1);
    static_assert(vec02_helper_t::aligned_offs == sizeof(int));
    static_assert(vec02_helper_t::holder_sz == sizeof(uintptr_t) + sizeof(int));

    using vec1_helper_t = agnostic::vector_detail::helper<test_element0, 0>;
    static_assert(vec1_helper_t::begin_offs == 1);
    static_assert(vec1_helper_t::aligned_offs == 1);
    static_assert(vec1_helper_t::holder_sz == sizeof(uintptr_t));

    using vec11_helper_t = agnostic::vector_detail::helper<test_element0, 1>;
    static_assert(vec11_helper_t::begin_offs == 1);
    static_assert(vec11_helper_t::aligned_offs == 16);
    static_assert(vec11_helper_t::holder_sz == 32);

    using vec2_helper_t = agnostic::vector_detail::helper<char, 127>;
    static_assert(vec2_helper_t::begin_offs == 1);
    static_assert(vec2_helper_t::aligned_offs == 1);
    static_assert(vec2_helper_t::holder_sz == 128);

    using vec21_helper_t = agnostic::vector_detail::helper<char, 128>;
    static_assert(vec21_helper_t::begin_offs == 2);
    static_assert(vec21_helper_t::aligned_offs == 2);
    static_assert(vec21_helper_t::holder_sz == 130);

#if 0
    using vec0_t = agnostic::vector<int, 0, std::allocator<int>>;
    vec0_t vec0;
    //std::to_address
    //BOOST_CHECK(vec0.size() == 0);

    vec0.push_back(1);
    BOOST_CHECK(vec0.size() == 1);
    BOOST_CHECK_EQUAL(vec0[0], 1);
#endif
}
