// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_MEMORY_HPP
#define AGNOSTIC_STD_MEMORY_HPP

#pragma once

#include "utility.hpp"
#include "new.hpp"

#ifndef DO_NOT_USE_AGNOSTIC_ADDRESSOF
#   include "memory/addressof.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_TO_ADDRESS
#   include "memory/to_address.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_CONSTRUCT_AT
#   include "memory/construct_at.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DESTROY_AT
#   include "memory/destroy_at.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_POINTER_TRAITS
#   include "memory/pointer_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#   include "memory/allocator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR
#   include "memory/allocator.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DEFAULT_DELETE
#   include "memory/default_delete.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_DELETE
#   include "memory/allocator_delete.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNIQUE_PTR
#   include "memory/unique_ptr.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATE_UNIQUE_PTR
#   include "memory/allocate_unique.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_COPY
#   include "memory/uninitialized_copy.hpp"
#endif

#endif // AGNOSTIC_STD_MEMORY_HPP
