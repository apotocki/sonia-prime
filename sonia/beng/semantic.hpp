//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <vector>
#include "sonia/variant.hpp"
#include "sonia/string.hpp"

#include "ast/terms.hpp"

namespace sonia::lang::beng {

class variable_entity;
class functional_entity;

namespace semantic {

struct push_variable { variable_entity const* entity; };
struct push_value { value_t value; };
struct set_variable { variable_entity const* entity; };
struct return_statement {};

struct invoke_function
{
    qname entity;
    uint32_t paramcount;
};

template <typename SemanticExpressionT>
struct conditional
{
    std::vector<SemanticExpressionT> true_branch;
    std::vector<SemanticExpressionT> false_branch;
};

}

// make_recursive_variant<
using semantic_expression_type = make_recursive_variant<
    empty_t, // no op
    semantic::push_variable, semantic::push_value,
    semantic::set_variable, semantic::invoke_function, semantic::return_statement,
    std::vector<recursive_variant_>,
    semantic::conditional<recursive_variant_>
>::type;

using semantic_expression_pair = std::pair<semantic_expression_type, beng_type>;

class function_scope_type
{

};

/*
class function_t
{
public:
    virtual ~function_t() = default;
};

class implemented_function : public function_t
{
public:
    qname name;
    std::vector<semantic_expression_type> body;
    bool is_inline = false;
};
*/

}
