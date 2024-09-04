//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/shared_ptr.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class basic_fn_pattern : public functional::pattern
{
    function_descriptor fd_;
    
    //entity_identifier fn_entity_id_;
    //entity_identifier result_type_;
    lex::resource_location declaration_location_;

public:
    basic_fn_pattern(fn_compiler_context&, fn_pure_t const&);

    error_storage is_matched(fn_compiler_context&, pure_call_t const&, functional::match_descriptor&) const override;

    inline qname_identifier fn_qname_id() const noexcept { return fd_.id(); }

private:
    entity_identifier build_fn_type(unit&, entity_signature&);
};

class generic_fn_pattern : public basic_fn_pattern
{
    std::vector<infunction_declaration_t> body_;
    bool building_ = false;

public:
    generic_fn_pattern(fn_compiler_context&, fn_decl_t const&);

    std::expected<entity_identifier, error_storage> apply(fn_compiler_context&, functional::match_descriptor&) const override;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, functional::match_descriptor&) const override;

private:
    shared_ptr<entity> build(unit&, functional::match_descriptor&) const;
};

}
