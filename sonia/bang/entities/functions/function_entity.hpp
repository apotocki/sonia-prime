//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

// not a type, it's an instance that has type - function type
class function_entity : public signatured_entity
{
protected:
    qname name_;
    entity_signature sig_;
    
public:
    function_entity(qname && name, entity_signature&& sig)
        : name_{ std::move(name) }, sig_{ std::move(sig) }
    {}

    inline qname_view name() const noexcept { return name_; }

    //function_entity(qname_identifier name, shared_ptr<fieldset_t> fs, entity_identifier result_type)
    //    : name_{ name }, fs_{ std::move(fs) }, result_type_ { result_type }, is_inline_{ 0 }
    //    , is_built_{ 0 }, is_defined_ { 0 }, is_const_index_{ 0 }
    //{}

    inline entity_signature const* signature() const noexcept override final { return &sig_; }

    size_t parameter_count() const noexcept;

    //void set_fn_signature(unit&, entity_signature&& fnsig);
    //void build_fn_signature(unit& u, entity_identifier rt);

    field_descriptor result;

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


// it'snot a signatured entity becase an external function can have different signatures,
// depending on the values of arguments
class external_function_entity : public entity
{
    uint32_t extfnid_;

public:
    inline explicit external_function_entity(size_t fnid)
        : extfnid_{ static_cast<uint32_t>(fnid) }
    {}

    inline size_t extfnid() const noexcept { return extfnid_; }

    void visit(entity_visitor const& v) const override { v(*this); }

    inline size_t hash() const noexcept override final
    {
        return sonia::hasher{}(extfnid_);
    }

    inline bool equal(entity const& rhs) const noexcept override
    {
        if (auto const* pent = dynamic_cast<external_function_entity const*>(&rhs)) {
            return pent->extfnid_ == extfnid_;
        }
        return false;
    }

    inline std::ostream& print_to(std::ostream& os, unit const& u) const override
    {
        return os << "external fn(id: "sv << extfnid_ << ")"sv;
    }
};

}
