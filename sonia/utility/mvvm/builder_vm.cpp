//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "builder_vm.hpp"
#include <sstream>
#include <boost/container/small_vector.hpp>

namespace sonia::vm {

std::string builder_context::generate_object_id() const
{
    std::ostringstream namess;
    namess << "_id" << id_counter_++;
    return namess.str();
}

small_string builder_context::camel2kebab(string_view cc)
{
    boost::container::small_vector<char, 64> buff;
    bool first = true;
    for (char c : cc) {
        if (std::isupper(c)) {
            if (!first)  {
                buff.emplace_back('-');
            }
            buff.emplace_back(std::tolower(c));
        } else {
            buff.emplace_back(c);
        }
        first = false;
    }
    return small_string{ buff.data(), buff.size() };
}

void builder_context::assign_variable()
{
    string_view propname = stack_back().as<string_view>();
    blob_result const& propvalue = *stack_back(1);
    set_property({}, propname, propvalue);
    stack_pop(); // assign value on stack
}

void builder_context::construct_object()
{
    string_view name = stack_back().as<string_view>();
    if (name.starts_with("::"sv)) {
        name = name.substr(2);
    }
    uint32_t argcount = stack_back(1).as<uint32_t>();
    
    shared_ptr<invokation::invokable> obj;
    // find id
    for (uint32_t i = 0; i < argcount; ++i) {
        if (stack_back(2 + 2 * i).as<string_view>() == "id") {
            string_view idval = stack_back(3 + 2 * i).as<string_view>();
            if (!idval.empty()) {
                obj = create(camel2kebab(name), idval);
            }
        }
    }
    if (!obj) {
        obj = create(camel2kebab(name), generate_object_id());
    }

    for (uint32_t i = 0; i < argcount; ++i) {
        string_view arg_name = stack_back(2 + 2 * i).as<string_view>();
        if (arg_name == "id"sv) continue;
        auto const& arg_value = stack_back(3 + 2 * i);
        obj->set_property(camel2kebab(arg_name), *arg_value);
    }
    stack_pop(argcount * 2 + 2);

    return stack_push(smart_blob{ object_blob_result(obj) });
}

builder_virtual_stack_machine::builder_virtual_stack_machine()
{
    do_vm_assign_variable_id_ = push_external_fn([](builder_context & ctx) { ctx.assign_variable(); });
    do_vm_object_constructor_id_ = push_external_fn([](builder_context& ctx) { ctx.construct_object(); });
}

void builder_virtual_stack_machine::append_extern_assign()
{
    append_ecall(do_vm_assign_variable_id_);
}

void builder_virtual_stack_machine::append_object_constructor()
{
    append_ecall(do_vm_object_constructor_id_);
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
