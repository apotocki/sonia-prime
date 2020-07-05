// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
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

#ifndef DO_NOT_USE_AGNOSTIC_DESTROY
#   include "memory/destroy.hpp"
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

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_DEFAULT_CONSTRUCT
#   include "memory/uninitialized_default_construct.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZEDVALUE_CONSTRUCT
#   include "memory/uninitialized_value_construct.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_FILL
#   include "memory/uninitialized_fill.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_MOVE
#   include "memory/uninitialized_move.hpp"
#endif
