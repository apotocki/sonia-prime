//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/container/small_vector.hpp>

#include "sonia/variant.hpp"

#include "bang_vm.hpp"
#include "push_value_visitor.hpp"

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang::vm {

class compiler_visitor_base : public static_visitor<void>
{
protected:
    unit& unit_;
    internal_function_entity const* fn_context_;
    asm_builder_t::function_builder& fnbuilder_;

    using breaks_t = boost::container::small_vector<asm_builder_t::instruction_entry*, 4>;
    mutable boost::container::small_vector<std::pair<asm_builder_t::instruction_entry*, breaks_t*>, 4> loop_stack_; // [{loop start, [loop brakes]}]

public:
    compiler_visitor_base(unit& u, asm_builder_t::function_builder & b, internal_function_entity const& ife)
        : unit_{ u }
        , fnbuilder_{ b }
        , fn_context_{ &ife }
    {}

    compiler_visitor_base(unit& u, asm_builder_t::function_builder & b)
        : unit_{ u }
        , fnbuilder_{ b }
        , fn_context_{ nullptr }
    {}

    inline void operator()(empty_t const&) const
    { /* noop, do nothing */ }

    void operator()(semantic::push_value const& pv) const
    {
        push_value_visitor vis{ unit_, fnbuilder_ };
        apply_visitor(vis, pv.value);
    }

    void operator()(semantic::push_by_offset const& pv) const
    {
        fnbuilder_.append_pushr(pv.offset);
    }

    void operator()(semantic::set_by_offset const& sv) const
    {
        fnbuilder_.append_setr(sv.offset);
    }

    inline void operator()(semantic::truncate_values const& c) const
    {
        if (c.keep_back) {
            fnbuilder_.append_collapse(c.count);
        } else {
            fnbuilder_.append_pop(c.count);
        }
    }

    void operator()(semantic::set_variable const& pv) const
    {
        using namespace lang::bang;

        auto varkind = pv.entity->varkind();
        if (varkind == variable_entity::kind::LOCAL || varkind == variable_entity::kind::SCOPE_LOCAL) {
            fnbuilder_.append_fset(pv.entity->index());
        } else if (varkind == variable_entity::kind::EXTERN) {
            string_view varname = unit_.as_string(unit_.fregistry().resolve(pv.entity->name()).name().back());
            smart_blob strbr{ string_blob_result(varname) };
            strbr.allocate();
            fnbuilder_.append_push_pooled_const(std::move(strbr));
            fnbuilder_.append_ecall((size_t)virtual_stack_machine::builtin_fn::extern_variable_set);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    }

    void operator()(semantic::invoke_function const& invf) const;

    void operator()(semantic::expression_list_t const& evec) const
    {
        for (auto const& e : evec) {
            //GLOBAL_LOG_INFO() << unit_.print(e);
            apply(e);
        }
    }

    void operator()(semantic::loop_scope_t const& s) const
    {
        if (s.continue_branch) {
            fnbuilder_.append_noop();
        }
        auto scope_begin_pos = fnbuilder_.make_label();

        if (s.continue_branch) {
            //fnbuilder_.append_noop();
            //auto jmpentry = fnbuilder_.current_entry();
            for (auto const& e : s.continue_branch) {
                apply(e);
            }
            auto loop_begin_pos = fnbuilder_.make_label();
            scope_begin_pos->operation = asm_builder_t::op_t::jmp;
            scope_begin_pos->operand = loop_begin_pos;
        }
        
        breaks_t breaks;
        loop_stack_.emplace_back(scope_begin_pos, std::addressof(breaks));
        SCOPE_EXIT([this] { loop_stack_.pop_back(); });

        for (auto const& e : s.branch) {
            apply(e);
        }
        auto scope_end_pos = fnbuilder_.make_label();

        // optimization: remove redundant breaks
        auto bit = breaks.rbegin(), ebit = breaks.rend();
        while (bit != ebit) {
            asm_builder_t::instruction_entry* pbi = *bit;
            if (pbi == scope_end_pos) {
                fnbuilder_.remove(pbi);
                scope_end_pos = fnbuilder_.current_entry(); // just update actual value
                breaks.erase(std::next(bit).base());
                bit = breaks.rbegin();
            } else {
                ++bit;
            }
        }

        for (asm_builder_t::instruction_entry* pbi : breaks) {
            pbi->operation = asm_builder_t::op_t::jmp;
            pbi->operand = scope_end_pos;
        }
    }

    void operator()(semantic::loop_continuer const&) const
    {
        if (loop_stack_.empty()) {
            throw exception("there is no enclosing loop for the cotinue statement");
        }
        fnbuilder_.append_jmp(loop_stack_.back().first);
    }

    void operator()(semantic::loop_breaker const&) const
    {
        if (loop_stack_.empty()) {
            throw exception("there is no enclosing loop for the break statement");
        }
        fnbuilder_.append_noop();
        loop_stack_.back().second->emplace_back(fnbuilder_.current_entry());
    }

    inline void operator()(semantic::conditional_t const& c) const
    {
        if (!c.false_branch && !c.true_branch) return;
        fnbuilder_.append_noop();
        auto branch_pt = fnbuilder_.current_entry();
        if (!c.false_branch) {
            for (auto const& e : c.true_branch) {
                apply(e);
            }
            auto branch_end_pt = fnbuilder_.make_label();
            branch_pt->operation = asm_builder_t::op_t::jf;
            branch_pt->operand = branch_end_pt;
        } else if (!c.true_branch) {
            for (auto const& e : c.false_branch) {
                apply(e);
            }
            auto branch_end_pt = fnbuilder_.make_label();
            branch_pt->operation = asm_builder_t::op_t::jt;
            branch_pt->operand = branch_end_pt;
        } else {
            for (auto const& e : c.true_branch) {
                apply(e);
            }
            if (!c.true_branch_finished) {
                fnbuilder_.append_noop();
            }
            auto true_branch_end_pt = fnbuilder_.make_label();
            for (auto const& e : c.false_branch) {
                apply(e);
            }
            auto branch_end_pt = fnbuilder_.make_label();
            branch_pt->operation = asm_builder_t::op_t::jf;
            branch_pt->operand = true_branch_end_pt;
            if (!c.true_branch_finished) {
                true_branch_end_pt->operation = asm_builder_t::op_t::jmp;
                true_branch_end_pt->operand = branch_end_pt;
            }
        }
    }

    virtual void apply(semantic::expression_t const&) const = 0;
};

template <typename DerivedT>
class compiler_visitor_generic : public compiler_visitor_base
{
    
public:
    using generic_base_t = compiler_visitor_generic;
    using compiler_visitor_base::compiler_visitor_base;

    using compiler_visitor_base::operator();

    void apply(semantic::expression_t const& e) const override
    {
        apply_visitor(derived(), e);
    }

protected:
    inline DerivedT const& derived() const noexcept { return static_cast<DerivedT const&>(*this); }
};

class inline_compiler_visitor : public compiler_visitor_generic<inline_compiler_visitor>
{
    mutable boost::container::small_vector<asm_builder_t::instruction_entry*, 4> rpositions_;

public:
    using generic_base_t::generic_base_t;
    
    using generic_base_t::operator();

    inline void operator()(semantic::return_statement const&) const
    {
        fnbuilder_.append_noop();
        rpositions_.push_back(fnbuilder_.current_entry());
    }

    void finalize()
    {
        auto fin_pos = fnbuilder_.make_label();
        while (!rpositions_.empty() && rpositions_.back() == fin_pos) { // inline fn returns right at the code end
            fnbuilder_.remove(rpositions_.back()); // fin_pos label is moved here
            rpositions_.pop_back();
            fin_pos = fnbuilder_.current_entry(); // just update actual value
        }
        if (rpositions_.empty()) {
            fnbuilder_.remove_label(fin_pos);
        } else {
            for (auto rpos : rpositions_) {
                rpos->operation = asm_builder_t::op_t::jmp;
                rpos->operand = fin_pos;
            }
        }
        size_t param_count = fn_context_->parameter_count(); // including captured_variables
        if (!fn_context_->is_void()) {
            fnbuilder_.append_fset(-static_cast<intptr_t>(param_count));
            fnbuilder_.append_truncatefp(-static_cast<intptr_t>(param_count) + 1);
        } else {
            fnbuilder_.append_truncatefp(-static_cast<intptr_t>(param_count));
        }
    }

    template <typename T>
    void operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

class compiler_visitor : public compiler_visitor_generic<compiler_visitor>
{
public:
    mutable optional<asm_builder_t::instruction_entry*> local_return_position;

    using generic_base_t::generic_base_t;

    using generic_base_t::operator();

    inline void operator()(semantic::return_statement const&) const
    {
        if (local_return_position) {
            fnbuilder_.append_jmp(*local_return_position);
            return;
        } else if (fn_context_) {
            local_return_position = fnbuilder_.make_label();
            size_t param_count = fn_context_->parameter_count(); // including captured_variables
            if (!fn_context_->is_void()) {
                fnbuilder_.append_fset(-static_cast<intptr_t>(param_count));
                fnbuilder_.append_truncatefp(-static_cast<intptr_t>(param_count) + 1);
            } else {
                fnbuilder_.append_truncatefp(-static_cast<intptr_t>(param_count));
            }
            if (param_count) {
                fnbuilder_.append_popfp();
            }
        }
        fnbuilder_.append_ret();
    }


#if 0
    inline void operator()(semantic::conditional_t const& c) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("compiler_visitor conditional_t");

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
#endif


    inline void operator()(semantic::not_empty_condition_t const& n) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("compiler_visitor return_statement");
#if 0
        size_t branch_pt = unit_.bvm().get_ip();
        unit_.bvm().append_pop(1); // remove boolean value false = is_nil
        for (auto const& e : n.branch) {
            apply_visitor(*this, e);
        }
        size_t not_nil_branch_end_pt = unit_.bvm().get_ip();

        unit_.bvm().append_pop(1); // remove boolean value true = is_nil
        unit_.bvm().append_jmpx(not_nil_branch_end_pt - branch_pt);
        size_t nil_branch_end_pt = unit_.bvm().get_ip();
        unit_.bvm().swap_code_blocks(branch_pt, not_nil_branch_end_pt);
        
        bvm().append_ecall(virtual_stack_machine::builtin_fn::is_nil);
        unit_.bvm().append_jfx(nil_branch_end_pt - not_nil_branch_end_pt);
        unit_.bvm().swap_code_blocks(branch_pt, nil_branch_end_pt);

        //bvm().append_ecall(virtual_stack_machine::builtin_fn::is_nil);
        //unit_.bvm().append_jtx(branch_end_pt - branch_pt);
        //unit_.bvm().swap_code_blocks(branch_pt, branch_end_pt);
#endif
    }

    template <typename T>
    void operator()(T const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};


void compiler_visitor_base::operator()(semantic::invoke_function const& invf) const
{
    entity const& e = unit_.eregistry().get(invf.fn);
    if (auto fe = dynamic_cast<internal_function_entity const*>(&e); fe) {
        if (fe->is_inline()) {
            inline_compiler_visitor ivis{ unit_, fnbuilder_, *fe };
            fnbuilder_.append_pushfp();
            for (auto const& e : fe->body()) {
                //GLOBAL_LOG_INFO() << unit_.print(e);
                apply_visitor(ivis, e);
            }
            ivis.finalize();
            fnbuilder_.append_popfp();
        } else {
            vmasm::fn_identity fnident{ fe->id() };
            fnbuilder_.append_call(fnident);

            //bvm().append_push_static_const(i64_blob_result((fe->parameter_count() + 1) * (fe->is_void() ? -1 : 1)));
        }
    } else if (auto efe = dynamic_cast<external_function_entity const*>(&e); efe) {
        fnbuilder_.append_ecall(efe->extfnid());
    } else {
        THROW_NOT_IMPLEMENTED_ERROR("compiler_visitor invoke_function");
    }
#if 0
    /*
    if (auto optecall = bvm_.get_ecall(invf.function_entity_name); optecall) {
        bvm_.append_ecall(*optecall);
    }
    if (invf.function_entity_name == unit_.builtin(sonia::lang::bang::builtin_type::arrayify)) {
        bvm_.append_builtin(sonia::lang::bang::builtin_type::arrayify);
    } else if (invf.function_entity_name == unit_.builtin(sonia::lang::bang::builtin_type::print_string)) {
        bvm_.append_builtin(sonia::lang::bang::builtin_type::print_string);
    } else if (invf.function_entity_name == unit_.builtin(sonia::lang::bang::builtin_type::tostring)) {
        bvm_.append_builtin(sonia::lang::bang::builtin_type::tostring);
    }
    else 
    */
    if (auto eptr = unit_.eregistry().find(invf.varname); eptr) {
        // to do: visitor
        if (auto pefe = dynamic_pointer_cast<external_function_entity>(eptr); pefe) {
            bvm().append_ecall(pefe->fn_index);
        }
        //else if (auto pte = dynamic_pointer_cast<type_entity>(eptr); pte) {
        //    bvm().append_ecall(virtual_stack_machine::builtin_fn::extern_object_constructor);
        //}
        else if (auto fe = dynamic_pointer_cast<function_entity>(eptr); fe) {
            if (fe->is_inline()) {
                for (auto const& e : fe->body) {
                    apply_visitor(*this, e);
                }
            } else {
                bvm().append_push_static_const(i64_blob_result((fe->signature().parameters_count() + 1) * (fe->is_void() ? -1 : 1)));
                bvm().append_fpush(fe->index());
                bvm().append_callp();
            }
            //bvm_.append_builtin(sonia::lang::bang::builtin_type::call_function_object);
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
            THROW_NOT_IMPLEMENTED_ERROR("unknown entity found, unresolved function call name: '%1%'"_fmt % unit_.print(invf.varname));
        }
    } else {
        throw exception("unresolved name: '%1%'"_fmt % unit_.print(invf.varname));
    }
#endif
}

}
