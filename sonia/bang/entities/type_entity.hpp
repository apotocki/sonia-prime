//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <ranges>
#include <expected>

#include <boost/unordered_set.hpp>

#include "sonia/shared_ptr.hpp"

#include "functional_entity.hpp"

//#include "sonia/utility/functional/hash/span.hpp"
namespace sonia::lang::bang {

class type_entity2 : public functional_entity
{
public:
    explicit type_entity2(entity_identifier eid, qname_identifier name) 
        : functional_entity{ std::move(eid), std::move(name) }
    {}

    std::expected<function_entity const*, error_storage> find_field_getter(fn_compiler_context&, annotated_identifier const&) const;
    std::expected<function_entity const*, error_storage> find_field_setter(fn_compiler_context&, annotated_identifier const&) const;

    std::expected<function_signature const*, error_storage> find(fn_compiler_context&, pure_call_t&) const override;

    bool try_cast(fn_compiler_context& ctx, bang_type const& rtype) const;

    // ast
    extension_list_t direct_bases;
    parameter_woa_list_t direct_parameters;
    
    //semantic
    std::vector<entity const*> bases;

    void treat(fn_compiler_context&);
    /*
    struct function_descriptor
    {
        shared_ptr<function_t> fn;
        std::vector<identifier> param_names;

        friend inline bool operator==(function_descriptor const& l, function_descriptor const& r)
        {
            return range_equal{}(l.param_names, r.param_names);
        }

        friend inline bool operator==(function_descriptor const& l, span<const identifier> r)
        {
            return range_equal{}(l.param_names, r);
        }

        friend inline bool operator==(span<const identifier> const& l, function_descriptor const& r)
        {
            return range_equal{}(l, r.param_names);
        }

        friend inline size_t hash_value(function_descriptor const& v)
        {
            return hasher{}(v.param_names);
        }
    };
    */
private:
    bool handling_ = false;
    bool handled_ = false;
    /*
    struct f_hasher
    {
        inline size_t operator()(shared_ptr<concrete_invoke_function> const& p) const noexcept { return hash_value(*p); }
    };

    struct f_equal_to
    {
        inline bool operator()(shared_ptr<concrete_invoke_function> const& l, shared_ptr<concrete_invoke_function> const& r) const noexcept { return *l == *r; }
        inline bool operator()(span<const identifier> l, shared_ptr<concrete_invoke_function> const& r) const noexcept { return l == *r; }
    };
    */
    //mutable boost::unordered_set<function_descriptor, hasher> funcions_;
};

// parameters: {argname, argvalue} ... pair_count
//class object_constructor_function : public function_t
//{
//public:
//
//};

}
