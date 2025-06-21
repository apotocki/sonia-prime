//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/ast_terms.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;
class functional_binding_set;

class pattern_matcher
{
    fn_compiler_context& ctx_;
    functional_binding_set& binding_;
    semantic::expression_list_t& expressions_;

public:
    inline pattern_matcher(fn_compiler_context& ctx, functional_binding_set& binding, semantic::expression_list_t& expressions)
        : ctx_{ ctx }, binding_{ binding }, expressions_{ expressions }
    {}

    error_storage match(pattern_t const&, annotated_entity_identifier const& type) const;

private:
    error_storage do_match(pattern_t::signature_descriptor const&, pattern_t const& pattern, annotated_entity_identifier const&) const;

    error_storage do_match_context_identifier(context_identifier, pattern_t const& pattern, annotated_entity_identifier const&) const;
    error_storage do_match_concepts(span<const syntax_expression_t>, annotated_entity_identifier const&) const;

    //error_storage match_signature_name(
    //    variant<placeholder, annotated_qname, context_identifier, syntax_expression_t> const& name,
    //    entity_signature const& sig, 
    //    annotated_entity_identifier const& type) const;
};

}
