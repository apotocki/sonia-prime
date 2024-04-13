//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/span.hpp"
#include "sonia/optional.hpp"
#include "sonia/function.hpp"
#include "sonia/beng/errors.hpp"
#include "sonia/beng/ast/fn_compiler_context.hpp"

namespace sonia::lang::beng {

optional<error_storage> cast_elements(fn_compiler_context& ctx, span<const beng_type> from, beng_type to, function<error_context(size_t)> const& cl);

}
