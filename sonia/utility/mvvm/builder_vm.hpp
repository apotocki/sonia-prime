//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/utility/invokation/invokable.hpp"
#include "sonia/utility/lang/vm.hpp"
#include <boost/unordered_map.hpp>

namespace sonia::vm {

class builder_context
{
public:
    using variable_type = smart_blob;
    constexpr static size_t command_max_size = sizeof(void*);
    bool is_zero(variable_type const& v) const noexcept { return v.is_nil(); }

    using vm_t = virtual_stack_machine<builder_context>;

    void assign_variable();
    void construct_object();

    static small_string camel2kebab(string_view cc);

    variable_type const& stack_back(size_t i = 0)
    {
        vm_t& v = vm();
        size_t ssz = v.stack().size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return v.stack()[ssz - 1 - i];
    }

    void stack_pop(size_t n = 1)
    {
        vm_t& v = vm();
        size_t ssz = v.stack().size();
        v.stack().resize(ssz - n);
    }

    void stack_push(smart_blob b)
    {
        vm_t& v = vm();
        v.stack().emplace_back(std::move(b));
    }

    virtual vm_t& vm() = 0;
    virtual shared_ptr<invokation::invokable> create(string_view type, string_view id) = 0;
    virtual void set_property(string_view id, string_view propname, blob_result const& value) = 0;

    std::string generate_object_id() const;

private:
    mutable int id_counter_{ 0 };
};

class builder_virtual_stack_machine : public virtual_stack_machine<builder_context>
{
    using base_t = virtual_stack_machine<builder_context>;

public:
    builder_virtual_stack_machine();

    size_t push_on_stack(var_t value);
    void append_extern_assign();
    void append_object_constructor();

private:
    void do_vm_assign_variable();

    boost::unordered_map<blob_result, size_t> literals_;

    size_t do_vm_assign_variable_id_;
    size_t do_vm_object_constructor_id_;
};

}
