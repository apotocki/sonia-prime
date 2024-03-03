//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include "beng.hpp"
#include "ast.hpp"
#include "semantic.hpp"

namespace sonia::lang::beng {

class procedure_entity : public entity
{
public:
    explicit procedure_entity(qname name) : entity { std::move(name) } {}

    struct procedure_signature
    {
        beng_generic_type result_type;
        std::vector<std::pair<identifier, beng_generic_type>> named_arguments;
        std::vector<beng_generic_type> position_arguments;
    };

    std::vector<procedure_signature> signatures;

    virtual bool find(compiler_context&,
        span<const expression_t> positioned_args,
        span<const std::tuple<annotated_identifier, expression_t>> named_args,
        std::vector<semantic_expression_type> & result, beng_generic_type & rtype) const;
};

}

#include "compiler.hpp"

namespace sonia::lang::beng {

bool procedure_entity::find(compiler_context&,
    span<const expression_t> positioned_args,
    span<const std::tuple<annotated_identifier, expression_t>> named_args,
    std::vector<semantic_expression_type>& result, beng_generic_type& rtype) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

}
