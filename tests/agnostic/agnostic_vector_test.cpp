// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#include <ntifs.h>

#include <cstddef>
#include <limits.h>
#include <float.h>

#include "sonia/sys/windows/driver/utility/cassert.hpp"

#include "sonia/sys/windows/driver/utility/memory.hpp"

using default_bare_allocator = sonia::wd::bare_allocator<>;

#include "agnostic/std/array.hpp"
#include "agnostic/std/compare/ordering.hpp"

#include "agnostic/list.hpp"
#include "agnostic/allocator/default_fised_size_allocator.hpp"

#define AGNOSTIC_ASSERT(exp) \
    ((!(exp)) ?              \
        (RtlAssert((PVOID)#exp, (PVOID)__FILE__, __LINE__, NULL), FALSE) : TRUE)

#define AGNOSTIC_CHECK(x) AGNOSTIC_ASSERT(x)
#define AGNOSTIC_CHECK_EQUAL(a, b) AGNOSTIC_ASSERT((a) == (b))

namespace std {

template <typename T>
using list = agnostic::list<T, agnostic::default_fixed_size_allocator_factory>;

}

#include "vector_test.ipp"
