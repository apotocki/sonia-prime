//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <list>
#include <expected>

#include "variable_entity.hpp"

#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

//class external_function_entity : public entity
//{
//    qname_identifier name_;
//
//public:
//    explicit external_function_entity(entity_identifier eid, qname_identifier name, size_t fni = -1)
//        : entity{ std::move(eid) }
//        , name_{ std::move(name) }
//        , fn_index{ fni }
//    {}
//
//    size_t fn_index;
//};



class functional_entity : public entity
{
    qname_identifier name_;
public:
    explicit functional_entity(entity_identifier eid, qname_identifier name)
        : entity { std::move(eid) }
        , name_{ std::move(name) }
    {}

    // used list to store just the reference in function_entity
    std::list<function_signature> signatures;

    function_signature& put_signature(function_signature&& sig);

    // looking for signature
    virtual function_signature const* find(fn_compiler_context&,
        span<const bang_type> position_params,
        span<const std::tuple<annotated_identifier, bang_type>> named_params) const;

    // looking by argument expressions
    virtual std::expected<function_signature const*, error_storage> find(fn_compiler_context&, pure_call_t& call) const;

    void visit(entity_visitor const& v) const override { v(*this); }
};

// returns no error if matched
error_storage is_matched(fn_compiler_context&,
    function_signature const&,
    pure_call_t& call);

 bool is_matched(fn_compiler_context& ctx,
    function_signature const& sig,
    span<const bang_type> positioned_params,
    span<const std::tuple<annotated_identifier, bang_type>> named_params);

}
