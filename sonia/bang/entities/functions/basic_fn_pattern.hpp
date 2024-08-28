//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/shared_ptr.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class basic_fn_pattern : public fieldset_pattern
{
    shared_ptr<fieldset> fs_;
    
    qname_identifier fn_qname_id_;
    entity_identifier fn_entity_id_;
    entity_identifier result_type_;
    lex::resource_location declaration_location_;
    bool building_ = false;

public:
    basic_fn_pattern(fn_compiler_context&, fn_decl_t&);
    std::expected<entity_identifier, error_storage> apply(fn_compiler_context& ctx) const override;

    inline qname_identifier fn_qname_id() const noexcept { return fn_qname_id_; }
    fieldset const& get_fieldset() const noexcept override { return *fs_; }

    void build(unit&);

private:
    entity_identifier build_fn_type(unit&, entity_signature&);
};

}
