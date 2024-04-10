//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"

#include "beng_vm.hpp"
#include "push_value_visitor.hpp"

#include "../entities/type_entity.hpp"
#include "../entities/functional_entity.hpp"

namespace sonia::lang::beng::vm {

class compiler_visitor : public static_visitor<void>
{
public:
    unit & unit_;
    optional<size_t> local_return_address;

    auto& bvm() const { return unit_.bvm(); }

    explicit compiler_visitor(unit& u) 
        : unit_{ u }
    {}

    void operator()(std::vector<semantic_expression_type> const& evec) const
    {
        for (auto const& e : evec) {
            apply_visitor(*this, e);
        }
    }

    void operator()(semantic::push_value const& pv) const
    {
        push_value_visitor vis{ unit_ };
        apply_visitor(vis, pv.value);
    }

    void operator()(semantic::invoke_function const& invf) const
    {
        /*
        if (auto optecall = bvm_.get_ecall(invf.function_entity_name); optecall) {
            bvm_.append_ecall(*optecall);
        }
        if (invf.function_entity_name == unit_.builtin(sonia::lang::beng::builtin_type::arrayify)) {
            bvm_.append_builtin(sonia::lang::beng::builtin_type::arrayify);
        } else if (invf.function_entity_name == unit_.builtin(sonia::lang::beng::builtin_type::print_string)) {
            bvm_.append_builtin(sonia::lang::beng::builtin_type::print_string);
        } else if (invf.function_entity_name == unit_.builtin(sonia::lang::beng::builtin_type::tostring)) {
            bvm_.append_builtin(sonia::lang::beng::builtin_type::tostring);
        }
        else 
        */
        if (auto eptr = unit_.eregistry().find(invf.function_entity_name()); eptr) {
            // to do: visitor
            if (auto pefe = dynamic_pointer_cast<external_function_entity>(eptr); pefe) {
                bvm().append_ecall(pefe->fn_index);
            } else if (auto pte = dynamic_pointer_cast<type_entity>(eptr); pte) {
                bvm().append_ecall(virtual_stack_machine::builtin_fn::extern_object_constructor);
            } else if (auto fe = dynamic_pointer_cast<function_entity>(eptr); fe) {
                if (fe->is_inline()) {
                    for (auto const& e : fe->body) {
                        apply_visitor(*this, e);
                    }
                } else {
                    bvm().push_on_stack_and_push(i64_blob_result((fe->signature().parameters_count() + 1) * (fe->is_void() ? -1 : 1)));
                    bvm().append_fpush(fe->index());
                    bvm().append_callp();
                }
                //bvm_.append_builtin(sonia::lang::beng::builtin_type::call_function_object);
                /*
                // assigned captured variables (if exist)
                for (auto const& pair : fe->captured_variables) {
                    // refiry
                    bvm_.append_freferify(std::get<0>(pair)->index());
                    bvm_.append_fpush(std::get<0>(pair)->index());
                }
                //
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
                */
                //THROW_NOT_IMPLEMENTED_ERROR("unimplemented function call name: '%1%'"_fmt % unit_.print(invf.entity));
            } else {
                THROW_NOT_IMPLEMENTED_ERROR("unknown entity found, unresolved function call name: '%1%'"_fmt % unit_.print(invf.function_entity_name()));
            }
        } else {
            throw exception("unresolved name: '%1%'"_fmt % unit_.print(invf.function_entity_name()));
        }
    }

    void operator()(semantic::set_variable const& pv) const
    {
        using namespace lang::beng;

        auto varkind = pv.entity->varkind();
        if (varkind == variable_entity::kind::LOCAL || varkind == variable_entity::kind::SCOPE_LOCAL) {
            bvm().append_fset(pv.entity->index());
        } else if (varkind == variable_entity::kind::EXTERN) {
            string_view varname = unit_.as_string(pv.entity->name().back());
            smart_blob strbr{ string_blob_result(varname) };
            strbr.allocate();
            size_t stack_pos = bvm().push_on_stack(std::move(strbr));
            bvm().append_push(stack_pos);
            bvm().append_ecall(virtual_stack_machine::builtin_fn::assign_extern_variable);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    }

    void operator()(semantic::push_variable const& pv) const
    {
        auto varkind = pv.entity->varkind();
        if (varkind == variable_entity::kind::LOCAL || varkind == variable_entity::kind::SCOPE_LOCAL) {
            unit_.bvm().append_fpush(pv.entity->index());
            return;
        }
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    inline void operator()(empty_t const&) const
    { /* noop, do nothing */}

    inline void operator()(semantic::return_statement const&) const
    {
        if (!local_return_address) [[unlikely]] {
            throw internal_error("return code is not defined");
        }
        unit_.bvm().append_jmp(*local_return_address);
    }

    inline void operator()(semantic::conditional<semantic_expression_type> const& c) const
    {
        size_t branch_pt = unit_.bvm().get_ip();
        if (c.false_branch.empty() && c.true_branch.empty()) return;
        if (c.false_branch.empty()) {
            for (auto const& e : c.true_branch) {
                apply_visitor(*this, e);
            }
            size_t branch_end_pt = unit_.bvm().get_ip();
            unit_.bvm().append_jfx(branch_end_pt - branch_pt);
            unit_.bvm().swap_code_blocks(branch_pt, branch_end_pt);
        } else if (c.true_branch.empty()) {
            for (auto const& e : c.false_branch) {
                apply_visitor(*this, e);
            }
            size_t branch_end_pt = unit_.bvm().get_ip();
            unit_.bvm().append_jtx(branch_end_pt - branch_pt);
            unit_.bvm().swap_code_blocks(branch_pt, branch_end_pt);
        } else {
            for (auto const& e : c.true_branch) {
                apply_visitor(*this, e);
            }
            size_t true_branch_end_pt = unit_.bvm().get_ip();
            for (auto const& e : c.false_branch) {
                apply_visitor(*this, e);
            }
            size_t false_branch_end_pt = unit_.bvm().get_ip();
            unit_.bvm().append_jmpx(false_branch_end_pt - true_branch_end_pt);
            size_t jmpxsz = unit_.bvm().get_ip() - false_branch_end_pt;
            unit_.bvm().swap_code_blocks(true_branch_end_pt, false_branch_end_pt);

            unit_.bvm().append_jfx(true_branch_end_pt + jmpxsz - branch_pt);
            unit_.bvm().swap_code_blocks(branch_pt, true_branch_end_pt + jmpxsz);
        }
    }

    inline void operator()(semantic::truncate_values const& c) const
    {
        if (c.keep_back) {
            unit_.bvm().append_collapse(c.count);
        } else {
            unit_.bvm().append_pop(c.count);
        }
    }

    template <typename T>
    void operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
