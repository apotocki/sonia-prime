//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <list>

#include "sonia/beng/ast.hpp"
#include "sonia/beng/semantic.hpp"

namespace sonia::lang::beng {

class function_entity : public entity
{
public:
    explicit function_entity(qname name, function_signature const& sig) 
        : entity{ std::move(name) }
        , signature_ { sig }
        , is_defined_{0}, is_inline_{0}
    {}

    function_signature const& signature() const { return signature_; }

    std::vector<semantic_expression_type> body;

    bool is_defined() const { return !!is_defined_; }
    bool is_variable_index() const { return !!is_variable_index_; }
    bool is_inline() const { return !!is_inline_; }
    bool is_void() const { return !!is_void_; }
    size_t get_address() const { return address_; }
    void set_inline(bool val = true) { is_inline_ = val ? 1 : 0; is_defined_ = 1; }
    void set_variable_index(size_t index) { address_ = static_cast<uint64_t>(index); is_variable_index_ = 1; is_defined_ = 1; }
    void set_address(size_t address) { address_ = static_cast<uint64_t>(address); is_variable_index_ = 0; is_defined_ = 1; }
    void set_void(bool val = true) { is_void_ = val ? 1 : 0; }

private:
    function_signature const& signature_;

    uint64_t address_ : 60;
    uint64_t is_defined_ : 1;
    uint64_t is_variable_index_ : 1;
    uint64_t is_inline_ : 1;
    uint64_t is_void_ : 1;
};

class functional_entity : public entity
{
public:
    explicit functional_entity(qname name) : entity { std::move(name) } {}

    // used list to store just the reference in function_entity
    std::list<function_signature> signatures;

    function_signature& put_signature(function_signature&& sig);

    virtual bool find(compiler_context&,
        span<const expression_t> positioned_args,
        span<const std::tuple<annotated_identifier, expression_t>> named_args,
        std::vector<semantic_expression_type> & result, beng_type & rtype) const;

    bool is_matched(compiler_context& ctx,
        function_signature const& sig,
        span<const expression_t> positioned_args,
        span<const std::tuple<annotated_identifier, expression_t>> named_args,
        std::vector<semantic_expression_type>& result) const;
};

}
