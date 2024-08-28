//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/bang/semantic.hpp"

namespace sonia::lang::bang {

class function_entity : public entity
{
    std::vector<infunction_declaration_t> decl_body_;
    std::vector<semantic::expression_t> body_;

    shared_ptr<fieldset> fs_;
    qname_identifier name_;
    entity_identifier result_type_;

    uint64_t address_ : 59;
    uint64_t is_built_ : 1;
    uint64_t is_inline_ : 1;
    uint64_t is_void_ : 1;
    uint64_t is_defined_ : 1; // key for extraction vm or execution related stuff
    uint64_t is_const_index_ : 1;

public:
    function_entity(qname_identifier name, shared_ptr<fieldset> fs, entity_identifier result_type)
        : name_{ name }, fs_{ std::move(fs) }, result_type_ { result_type }, is_inline_{ 0 }
        , is_built_{ 0 }, is_defined_ { 0 }, is_const_index_{ 0 }
    {}

    size_t hash() const noexcept override;
    bool equal(entity const&) const noexcept override;
    
    void visit(entity_visitor const& v) const override { v(*this); }

    inline qname_identifier name() const noexcept { return name_; }
    size_t parameter_count() const noexcept;

    void set_body(std::vector<infunction_declaration_t> body) { decl_body_ = std::move(body); }
    std::span<const semantic::expression_t> body() const { return body_; }

    inline entity_identifier get_result_type() const noexcept { return result_type_; }

    void build(unit&);
    void set_fn_type(unit&, entity_signature&); // result_type_ should be initialized

    bool is_void() const { return !!is_void_; }

    bool is_inline() const { return !!is_inline_; }
    void set_inline(bool val = true) { is_inline_ = val ? 1 : 0; }

    bool is_built() const { return !!is_built_; }

    /// to do: extract vm or execution related stuff
    bool is_defined() const { return !!is_defined_; }
    size_t get_address() const { BOOST_ASSERT(is_defined_); return address_; }
    size_t get_index() const { BOOST_ASSERT(is_const_index_); return address_; }
    void set_address(size_t address) { address_ = static_cast<uint64_t>(address); is_const_index_ = 0; is_defined_ = 1; }
    bool is_const_index() const { return !!is_const_index_; }
    void set_const_index(size_t index) { address_ = static_cast<uint64_t>(index); is_const_index_ = 1; is_defined_ = 0; }

#if 0
public:
    //function_entity(entity_identifier eid, qname_identifier name, function_signature && sig)
    //    : variable_entity{ std::move(eid), std::move(name), bang_type{sig.fn_type}, kind::LOCAL }
    //    , is_defined_{0}, is_inline_{0}
    //{
    //    signature_ = std::move(sig);
    //}

    function_signature const& fsignature() const { return signature_; }
    bang_type const& result_type() const { return fsignature().fn_type.result; }

    boost::container::small_vector<std::pair<variable_entity*, variable_entity*>, 16> captured_variables; // [(from, to)]
   
    void set_address(size_t address) { address_ = static_cast<uint64_t>(address); is_static_variable_index_ = 0; is_defined_ = 1; }

    void materialize_call(fn_compiler_context&, pure_call_t& call) const;

private:
    function_signature signature_;

    uint64_t address_ : 60;
    uint64_t is_defined_ : 1;
    uint64_t is_static_variable_index_ : 1;
    uint64_t is_inline_ : 1;
#endif
};

}
