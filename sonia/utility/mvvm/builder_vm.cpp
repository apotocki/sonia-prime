//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "builder_vm.hpp"

namespace sonia::vm {

void builder_context::assign_variable()
{
    string_view propname = stack_back().as<string_view>();
    blob_result const& propvalue = *stack_back(1);
    set_property({}, propname, propvalue);
}

builder_virtual_stack_machine::builder_virtual_stack_machine()
{
    do_vm_assign_variable_id_ = push_external_fn([](builder_context & ctx) { ctx.assign_variable(); });
}

void builder_virtual_stack_machine::append_extern_assign()
{
    append_ecall(do_vm_assign_variable_id_);
}

size_t builder_virtual_stack_machine::push_on_stack(var_t value)
{
    auto it = literals_.find(*value);
    if (it == literals_.end()) {
        size_t index = base_t::push_on_stack(value);
        it = literals_.insert(it, std::pair{ *value, index });
    }
    return it->second;
}

}
