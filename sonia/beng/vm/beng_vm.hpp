//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/utility/invokation/invokable.hpp"
#include "sonia/utility/lang/vm.hpp"
#include <boost/unordered_map.hpp>
#include "../extern.hpp"
#include "../builtins.hpp"
#include "../terms.hpp"

namespace sonia::lang::beng::vm {

/*
class variable
{
    using variant_t = variant<smart_blob, shared_ptr<smart_blob>>;

public:
    variable() = default;
    variable(smart_blob sb) : value_{std::move(sb) } {}

    variable& operator= (smart_blob rhs)
    {
        if (smart_blob* psb = sonia::get<smart_blob>(&value_); psb) {
            *psb = std::move(rhs);
        } else {
            *sonia::get<shared_ptr<smart_blob>>(value_) = std::move(rhs);
        }
        return *this;
    }

    variable(variable const& rhs) = default;
    variable(variable && rhs) = default;
    variable& operator=(variable const& rhs) = default;
    variable& operator=(variable&& rhs) = default;

    //// copy/move construction and assisgnment don't copy 'referenceness' of a variable, just a value
    //variable(variable const& rhs) : value_{ smart_blob{rhs.get()} } {}
    //variable(variable&& rhs) : value_{ smart_blob{std::move(rhs.get())} } {}
    //variable& operator=(variable const& rhs) { return operator=(smart_blob{rhs.get()}); }
    //variable& operator=(variable&& rhs) { return operator=(smart_blob{ std::move(rhs.get()) }); }

    void referify()
    {
        if (smart_blob * psb = sonia::get<smart_blob>(&value_); psb) {
            auto p = make_shared<smart_blob>(std::move(*psb));
            value_ = std::move(p);
        }
    }

    smart_blob & get()
    {
        if (smart_blob* psb = sonia::get<smart_blob>(&value_); psb) return *psb;
        return *sonia::get<shared_ptr<smart_blob>>(value_);
    }

    blob_result const& get() const
    {
        if (smart_blob const* psb = sonia::get<smart_blob>(&value_); psb) return **psb;
        return **sonia::get<shared_ptr<smart_blob>>(value_);
    }

    bool is_nil() const { return get().type == blob_type::nil; }
    bool is_reference() const { return !!value_.which(); }
    
    blob_result const& operator*() const { return get(); }

    template <typename T>
    decltype(auto) as() const { return ::as<T>(get()); }

private:
    variant_t value_;
};


template <typename Elem, typename Traits>
inline std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, variable const& v)
{
    if (v.is_reference()) {
        return os << '&' << *v;
    }
    return os << *v;
}
*/

class context
{
    using call_stack_type = std::vector<uint32_t>;
    call_stack_type call_stack_;

    using stack_frames_type = std::vector<uint32_t>;
    stack_frames_type stack_frames_;

public:
    using variable_type = smart_blob;
    bool is_true(variable_type const& v) const noexcept;
    variable_type value_of(size_t val) const { return ui64_blob_result(val); }

    size_t callp(size_t ret_address);
    std::string callp_describe() const;

    void efn(size_t fn_index);
    std::string ecall_describe(size_t) const;

    using vm_t = sonia::vm::virtual_stack_machine<context>;

    explicit context(vm_t& vm, external_environment* penv = nullptr) : vm_{vm}, penv_{penv} {}
    
    context(context const&) = delete;
    context& operator=(context const&) = delete;

    // builtins
    void arrayify();
    void referify();
    void assign_extern_variable();
    void construct_extern_object();
    void construct_function();

    void call_function_object();

    static small_string camel2kebab(string_view cc);

    inline size_t stack_size() const noexcept { return vm_.stack().size(); }

    variable_type const& stack_at(size_t i) const
    {
        size_t ssz = stack_size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return vm_.stack()[i];
    }

    variable_type & stack_at(size_t i)
    {
        size_t ssz = stack_size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return vm_.stack()[i];
    }

    span<const variable_type> stack_span(size_t last_offset, size_t count) const
    {
        size_t ssz = stack_size();
        if (ssz < last_offset + count) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return span{ vm_.stack().data() + (ssz - last_offset - count), count };
    }

    variable_type const& stack_back(size_t i = 0) const
    {
        size_t ssz = stack_size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return vm_.stack()[ssz - 1 - i];
    }

    void stack_pop(size_t n = 1)
    {
        size_t ssz = stack_size();
        vm_.stack().resize(ssz - n);
    }

    template <typename T>
    requires (std::is_convertible_v<std::remove_cvref_t<T>, variable_type>)
    void stack_push(T && b)
    {
        vm_.stack().emplace_back(std::forward<T>(b));
    }

    void stack_truncate(size_t sz)
    {
        vm_.stack().resize(sz);
    }

    void call_stack_push(size_t address)
    {
        call_stack_.push_back(static_cast<uint32_t>(address));
    }

    optional<size_t> call_stack_pop()
    {
        if (call_stack_.empty()) return nullopt;
        size_t address = call_stack_.back();
        call_stack_.pop_back();
        return address;
    }

    optional<size_t> call_stack_back() const
    {
        return call_stack_.empty() ? optional<size_t>{} : call_stack_.back();
    }

    void frame_stack_push()
    {
        stack_frames_.emplace_back(static_cast<uint32_t>(stack_size()));
    }

    void frame_stack_pop()
    {
        if (!stack_frames_.empty()) stack_frames_.pop_back();
    }

    size_t frame_stack_back() const
    {
        return stack_frames_.empty() ? 0 : stack_frames_.back();
    }

    small_string generate_object_id() const;

private:
    mutable size_t id_counter_{ 0 };
    vm_t& vm_;
    external_environment* penv_;
};

}

namespace sonia::lang::beng {

class virtual_stack_machine : public sonia::vm::virtual_stack_machine<vm::context>
{
    using base_t = sonia::vm::virtual_stack_machine<vm::context>;

public:
    virtual_stack_machine();

    size_t push_on_stack(smart_blob&& value);
    void push_on_stack_and_push(smart_blob&& value);

    enum class builtin_fn
    {
        arrayify = 0, referify, function_constructor, extern_object_constructor,
        assign_extern_variable,
        eof_type
    };

    using base_t::append_ecall;
    void append_ecall(builtin_fn fn);

protected:

    /*
    void append_extern_assign();
    void append_object_constructor();
    void append_arrayify();
    void append_function_constructor();
    void append_print_string();
    */

    //void append_builtin(sonia::lang::beng::builtin_type);

    //optional<size_t> get_ecall(qname const& qn) const;

private:
    void do_vm_assign_variable();

    boost::unordered_map<blob_result, size_t> literals_;
    
    //std::vector<size_t> builtins_;

    //boost::unordered_map<qname, >
    /*
    size_t do_vm_assign_variable_id_;
    size_t do_vm_object_constructor_id_;
    size_t do_vm_arrayify_id_;
    size_t do_vm_function_constructor_id_;
    size_t do_vm_print_string_id_;
    */
};

}
