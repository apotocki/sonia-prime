// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ASSIGNABLE_FROM_CONCEPT

#include "agnostic/std/type_traits/is_lvalue_reference.hpp"
#include "agnostic/std/type_traits/remove_reference.hpp"
#include "agnostic/std/utility/forward.hpp"

#include "agnostic/std/concepts/same_as.hpp"
#include "agnostic/std/concepts/common_reference_with.hpp"

namespace std {

template< class LHS, class RHS >
concept assignable_from =
    is_lvalue_reference_v<LHS> &&
    common_reference_with<
        const remove_reference_t<LHS>&,
        const remove_reference_t<RHS>&
    > &&
    requires(LHS lhs, RHS && rhs) {
        { lhs = forward<RHS>(rhs) } -> same_as<LHS>;
    };

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
