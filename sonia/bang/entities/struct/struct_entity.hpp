//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <atomic>

#include "sonia/concurrency.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class struct_entity : public basic_signatured_entity
{
    field_list_t fields_;

    mutable entity_identifier underlying_tuple_eid_;
    mutable entity_identifier underlying_tuple_constructor_eid_;
    
    // to do: something equivalent to the std::once_flag but for fibers
    mutable fibers::mutex mtx_;
    mutable std::atomic_bool built_ = false;

public:
    explicit struct_entity(entity_identifier type, entity_signature && sgn, field_list_t const& f)
        : basic_signatured_entity{ std::move(type), std::move(sgn) }
        , fields_{ f }
    {}

    std::expected<entity_identifier, error_storage> underlying_tuple_eid(fn_compiler_context&) const;
    std::expected<entity_identifier, error_storage> underlying_tuple_constructor_eid(fn_compiler_context&) const;

    error_storage build(fn_compiler_context&) const;
    //std::expected<function_entity const*, error_storage> find_field_getter(fn_compiler_context&, annotated_identifier const&) const;
    //std::expected<function_entity const*, error_storage> find_field_setter(fn_compiler_context&, annotated_identifier const&) const;
    //std::expected<function_signature const*, error_storage> find(fn_compiler_context&, pure_call_t&) const override;
    //bool try_cast(fn_compiler_context& ctx, bang_type const& rtype) const;
    //void treat(fn_compiler_context&);
};

}
