//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "beng_vm.hpp"

#include <boost/container/small_vector.hpp>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/mp/integer_view.hpp" 

namespace sonia::beng::vm {

small_string context::generate_object_id() const
{
    using buff_t = boost::container::small_vector<char, 16>;
    buff_t tailored_name = { '_', 'i', 'd' };
    bool reversed;
    mp::to_string_converter(std::span{ &id_counter_, 1 }, std::back_inserter(tailored_name), reversed);
    if (reversed) std::reverse(tailored_name.begin() + 1, tailored_name.end());
    return small_string{ tailored_name.data(), tailored_name.size() };
}

small_string context::camel2kebab(string_view cc)
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

void context::assign_variable()
{
    SCOPE_EXIT([this] { stack_pop(); }); // assign value on stack after that
    string_view propname = stack_back().as<string_view>();
    blob_result const& propvalue = *stack_back(1);
    if (penv_) {
        penv_->set_property(propname, propvalue);
    } else {
        throw exception("can't set property '%1%' to '%2', no external environment was provided"_fmt % propname % propvalue);
    }
}

void context::construct_object()
{
    string_view name = stack_back().as<string_view>();
    if (name.starts_with("::"sv)) {
        name = name.substr(2);
    }
    uint32_t argcount = stack_back(1).as<uint32_t>();
    
    if (!penv_) {
        SCOPE_EXIT([this, argcount] { stack_pop(argcount * 2 + 2); });
        throw exception("can't construct the object '%1%', no external environment was provided"_fmt % name);
    }

    shared_ptr<invokation::invokable> obj;
    // find id
    for (uint32_t i = 0; i < argcount; ++i) {
        if (stack_back(2 + 2 * i).as<string_view>() == "id"sv) {
            string_view idval = stack_back(3 + 2 * i).as<string_view>();
            if (!idval.empty()) {
                obj = penv_->create(camel2kebab(name), idval);
            }
        }
    }
    if (!obj) {
        obj = penv_->create(camel2kebab(name), generate_object_id());
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

void context::arrayify()
{
    uint32_t argcount = stack_back().as<uint32_t>();
    boost::container::small_vector<blob_result, 4> elements;
    EXCEPTIONAL_SCOPE_EXIT([&elements]() {
        for (auto& e : elements) blob_result_unpin(&e);
    });
    for (uint32_t i = argcount; i > 0; --i) {
        elements.emplace_back(*stack_back(i));
        blob_result_pin(&elements.back());
    }
    smart_blob r{ array_blob_result(span{elements.data(), elements.size()}) };
    r.allocate();
    stack_pop(argcount + 1);
    return stack_push(std::move(r));
}

void context::print_string()
{
    string_view str = stack_back().as<string_view>();
    GLOBAL_LOG_INFO() << str;
}

virtual_stack_machine::virtual_stack_machine()
{
    do_vm_assign_variable_id_ = push_external_fn([](context & ctx) { ctx.assign_variable(); });
    do_vm_object_constructor_id_ = push_external_fn([](context& ctx) { ctx.construct_object(); });
    do_vm_arrayify_id_ = push_external_fn([](context& ctx) { ctx.arrayify(); });
    do_vm_print_string_id_ = push_external_fn([](context& ctx) { ctx.print_string(); });
}

void virtual_stack_machine::append_extern_assign()
{
    append_ecall(do_vm_assign_variable_id_);
}

void virtual_stack_machine::append_object_constructor()
{
    append_ecall(do_vm_object_constructor_id_);
}

void virtual_stack_machine::append_arrayify()
{
    append_ecall(do_vm_arrayify_id_);
}

void virtual_stack_machine::append_print_string()
{
    append_ecall(do_vm_print_string_id_);
}

size_t virtual_stack_machine::push_on_stack(var_t value)
{
    auto it = literals_.find(*value);
    if (it == literals_.end()) {
        size_t index = base_t::push_on_stack(value);
        it = literals_.insert(it, std::pair{ *value, index });
    }
    return it->second;
}

}
