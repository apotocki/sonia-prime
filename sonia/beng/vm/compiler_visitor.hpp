//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"

#include "beng_vm.hpp"
#include "push_value_visitor.hpp"

#include "../entities/type_entity.hpp"
#include "../entities/functional_entity.hpp"

namespace sonia::beng::vm {

class compiler_visitor : public static_visitor<void>
{
public:
    lang::beng::unit & unit_;
    virtual_stack_machine& bvm_;
    optional<size_t> local_return_address;

    compiler_visitor(lang::beng::unit& u, virtual_stack_machine& bvm) 
        : unit_{ u }
        , bvm_{ bvm }
    {}

    void operator()(std::vector<lang::beng::semantic_expression_type> const& evec) const
    {
        for (auto const& e : evec) {
            apply_visitor(*this, e);
        }
    }

    void operator()(lang::beng::semantic::push_value const& pv) const
    {
        push_value_visitor vis{ bvm_ };
        size_t stack_pos = apply_visitor(vis, pv.value);
        bvm_.append_push(stack_pos);
    }

    void operator()(lang::beng::semantic::invoke_function const& invf) const
    {
        if (invf.entity == unit_.arrayify_entity_name()) {
            bvm_.append_arrayify();
        } else if (invf.entity == unit_.print_string_name()) {
            bvm_.append_print_string();
        } else if (auto eptr = unit_.eregistry().find(invf.entity); eptr) {
            if (auto pte = dynamic_pointer_cast<lang::beng::type_entity>(eptr); pte) {
                bvm_.append_object_constructor();
            } else if (auto fe = dynamic_pointer_cast<lang::beng::function_entity>(eptr); fe) {
                if (!fe->is_defined()) {
                    size_t pos = bvm_.push_on_stack(smart_blob{}); // just reserve
                    fe->set_variable_index(pos);
                }
                if (fe->is_inline()) {
                    for (auto const& e : fe->body) {
                        apply_visitor(*this, e);
                    }
                } else if (fe->is_variable_index()) {
                    bvm_.append_push(fe->get_address());
                    bvm_.appned_callp();
                } else {
                    bvm_.append_call(fe->get_address());
                }
                //THROW_NOT_IMPLEMENTED_ERROR("unimplemented function call name: '%1%'"_fmt % unit_.print(invf.entity));
            } else {
                THROW_NOT_IMPLEMENTED_ERROR("unknown entity found, unresolved function call name: '%1%'"_fmt % unit_.print(invf.entity));
            }
        } else {
            throw exception("unresolved name: '%1%'"_fmt % unit_.print(invf.entity));
        }
    }

    void operator()(lang::beng::semantic::set_variable const& pv) const
    {
        if (auto const* lve = dynamic_cast<lang::beng::local_variable_entity const*>(pv.entity); lve) {
            bvm_.append_fset(lve->index());
        } else { // extrenal variable case
            string_view varname = unit_.as_string(pv.entity->name().back());
            smart_blob strbr{ string_blob_result(varname) };
            strbr.allocate();
            size_t stack_pos = bvm_.push_on_stack(std::move(strbr));
            bvm_.append_push(stack_pos);
            bvm_.append_extern_assign();
        }
    }

    void operator()(lang::beng::semantic::push_variable const& pv) const
    {
        if (auto const* lve = dynamic_cast<lang::beng::local_variable_entity const*>(pv.entity); lve) {
            bvm_.append_fpush(lve->index());
            return;
        }
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    inline void operator()(empty_t const&) const
    { /* noop, do nothing */}

    inline void operator()(lang::beng::semantic::return_statement const&) const
    {
        if (!local_return_address) [[unlikely]] {
            throw internal_error("return code is not defined");
        }
        bvm_.append_jmp(*local_return_address);
    }

    template <typename T>
    void operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
