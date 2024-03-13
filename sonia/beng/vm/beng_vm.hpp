//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/utility/invokation/invokable.hpp"
#include "sonia/utility/lang/vm.hpp"
#include <boost/unordered_map.hpp>
#include "../extern.hpp"

namespace sonia::beng::vm {

class context
{
public:
    using variable_type = smart_blob;
    constexpr static size_t command_max_size = sizeof(void*);
    bool is_zero(variable_type const& v) const noexcept { return v.is_nil(); }
    size_t to_address(variable_type const& v) const { return v.as<size_t>(); }

    using vm_t = sonia::vm::virtual_stack_machine<context>;

    explicit context(vm_t& vm, external_environment* penv = nullptr) : vm_{vm}, penv_{penv} {}
    
    context(context const&) = delete;
    context& operator=(context const&) = delete;

    void assign_variable();
    void construct_object();
    void arrayify();
    void print_string();

    static small_string camel2kebab(string_view cc);

    variable_type const& stack_back(size_t i = 0)
    {
        size_t ssz = vm_.stack().size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return vm_.stack()[ssz - 1 - i];
    }

    void stack_pop(size_t n = 1)
    {
        size_t ssz = vm_.stack().size();
        vm_.stack().resize(ssz - n);
    }

    void stack_push(smart_blob b)
    {
        vm_.stack().emplace_back(std::move(b));
    }

    //virtual vm_t& vm() = 0;
    //virtual shared_ptr<invokation::invokable> create(string_view type, string_view id) = 0;
    //virtual void set_property(string_view id, string_view propname, blob_result const& value) = 0;

    small_string generate_object_id() const;

private:
    mutable size_t id_counter_{ 0 };
    vm_t& vm_;
    external_environment* penv_;
};

class virtual_stack_machine : public sonia::vm::virtual_stack_machine<context>
{
    using base_t = sonia::vm::virtual_stack_machine<context>;

public:
    virtual_stack_machine();

    size_t push_on_stack(var_t value);
    void append_extern_assign();
    void append_object_constructor();
    void append_arrayify();
    void append_print_string();

private:
    void do_vm_assign_variable();

    boost::unordered_map<blob_result, size_t> literals_;

    size_t do_vm_assign_variable_id_;
    size_t do_vm_object_constructor_id_;
    size_t do_vm_arrayify_id_;
    size_t do_vm_print_string_id_;
};

}
