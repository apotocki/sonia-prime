//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <atomic>

//#include "sonia/concurrency.hpp"
#include "sonia/shared_ptr.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/functional.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class functional_match_descriptor;

class struct_entity : public basic_signatured_entity
{
    friend class struct_builder_visitor;

    qname name_;
    variant<field_list_t, statement_span> body_;

    mutable entity_identifier underlying_tuple_eid_;
    
    enum class build_state : long
    {
        not_built,
        underlying_tuple_built = 1
    };

    mutable std::atomic<build_state> built_{ build_state::not_built };

    error_storage build(fn_compiler_context& struct_ctx, field_list_t const&, semantic::expression_list_t&) const;
    error_storage build(fn_compiler_context& struct_ctx, statement_span const&, semantic::expression_list_t&) const;

public:
    struct_entity(unit&, functional&, variant<field_list_t, statement_span> const&);
    struct_entity(qname qn, entity_signature&& sgn, variant<field_list_t, statement_span> const& body)
        : basic_signatured_entity{ std::move(sgn) }
        , name_{ std::move(qn) }
        , body_{ body }
    {}

    inline qname_view name() const noexcept { return name_; }

    //std::expected<functional::match, error_storage> find_init(fn_compiler_context&, pure_call_t const&) const;
    std::expected<entity_identifier, error_storage> underlying_tuple_eid(fn_compiler_context&) const;
    //std::expected<functional_match_descriptor const*, error_storage> underlying_tuple_initializer(fn_compiler_context&) const;

    error_storage build(fn_compiler_context& external_ctx, semantic::expression_list_t&) const;
    //std::expected<function_entity const*, error_storage> find_field_getter(fn_compiler_context&, annotated_identifier const&) const;
    //std::expected<function_entity const*, error_storage> find_field_setter(fn_compiler_context&, annotated_identifier const&) const;
    //std::expected<function_signature const*, error_storage> find(fn_compiler_context&, pure_call_t&) const override;
    //bool try_cast(fn_compiler_context& ctx, bang_type const& rtype) const;
    //void treat(fn_compiler_context&);
};

}
