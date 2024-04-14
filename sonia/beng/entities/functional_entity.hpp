//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <list>
#include <expected>

#include "variable_entity.hpp"

#include "sonia/beng/errors.hpp"

namespace sonia::lang::beng {

class external_function_entity : public entity
{
public:
    explicit external_function_entity(qname_type name, size_t fni = -1)
        : entity{ std::move(name) }
        , fn_index{ fni }
    {}

    size_t fn_index;
};

class function_entity : public variable_entity
{
public:
    function_entity(qname name, function_signature && sig)
        : variable_entity{ std::move(name), beng_type{sig.fn_type}, kind::LOCAL }
        , is_defined_{0}, is_inline_{0}
    {
        signature_ = std::move(sig);
    }

    function_signature const& signature() const { return signature_; }
    beng_type const& result_type() const { return signature().fn_type.result; }

    boost::container::small_vector<std::pair<variable_entity*, variable_entity*>, 16> captured_variables; // [(from, to)]
    std::vector<semantic::expression_type> body;

    bool is_defined() const { return !!is_defined_; }
    bool is_variable_index() const { return !!is_variable_index_; }
    bool is_inline() const { return !!is_inline_; }
    bool is_void() const { return signature_.fn_type.result == beng_tuple_t{}; }
    size_t get_address() const { return address_; }
    void set_inline(bool val = true) { is_inline_ = val ? 1 : 0; is_defined_ = 1; }
    void set_variable_index(size_t index) { address_ = static_cast<uint64_t>(index); is_variable_index_ = 1; is_defined_ = 1; }
    void set_address(size_t address) { address_ = static_cast<uint64_t>(address); is_variable_index_ = 0; is_defined_ = 1; }

    void materialize_call(fn_compiler_context&, pure_call_t & call) const;

private:
    function_signature signature_;
    
    uint64_t address_ : 60;
    uint64_t is_defined_ : 1;
    uint64_t is_variable_index_ : 1;
    uint64_t is_inline_ : 1;
};

class functional_entity : public entity
{
public:
    explicit functional_entity(qname name) : entity { std::move(name) } {}

    // used list to store just the reference in function_entity
    std::list<function_signature> signatures;

    function_signature& put_signature(function_signature&& sig);

    // looking for signature
    virtual function_signature const* find(fn_compiler_context&,
        span<const beng_type> position_params,
        span<const std::tuple<annotated_identifier, beng_type>> named_params) const;

    // looking by argument expressions
    virtual std::expected<beng_type, error_storage> find(fn_compiler_context&, pure_call_t& call) const;
};

bool is_matched(fn_compiler_context&,
    function_signature const&,
    pure_call_t& call);

bool is_matched(fn_compiler_context& ctx,
    function_signature const& sig,
    span<const beng_type> positioned_params,
    span<const std::tuple<annotated_identifier, beng_type>> named_params);

}
