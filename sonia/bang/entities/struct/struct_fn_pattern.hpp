//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functions/basic_fn_pattern.hpp"

namespace sonia::lang::bang {

class struct_fn_pattern : public basic_fn_pattern
{
    field_list_t fields_;

public:
    struct_fn_pattern(fn_compiler_context&, functional const&, fn_pure const&, field_list_t const&);
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const override;

private:
    //std::expected<entity_identifier, error_storage> get_underlying_tuple_eid(fn_compiler_context& callee_ctx, qname_identifier fid) const;
    //std::expected<entity_identifier, error_storage> get_underlying_tuple_constructor_eid(fn_compiler_context& callee_ctx, qname_identifier fid) const;
    //error_storage get_underlying(fn_compiler_context& callee_ctx, qname_identifier fid) const;
    //void build(fn_compiler_context& callee_ctx, qname_identifier fid);
};

}
