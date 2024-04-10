//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/lang/entity.hpp"
#include "ast_terms.hpp"

namespace sonia::lang::beng {

using entity = sonia::lang::entity<identifier, lex::resource_location>;
class enum_entity;
class variable_entity;
class functional_entity;
class type_entity;

// ======================================================================== values
struct function_value { qname mangled_name; };
using value_t = make_recursive_variant<
    null_t, bool, decimal, small_u32string, // shared_ptr<beng_object>,
    function_value,
    std::vector<recursive_variant_>
>::type; // to do: tuples

}
