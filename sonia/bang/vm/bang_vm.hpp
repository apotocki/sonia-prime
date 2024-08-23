//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/utility/invocation/invocation.hpp"
#include "sonia/utility/invocation/invocable.hpp"
#include "sonia/utility/lang/vm.hpp"
#include <boost/unordered_map.hpp>
//#include "../extern.hpp"
#include "../terms.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang::vm {

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

    explicit context(unit& u, invocation::invocable* penv = nullptr);
    
    context(context const&) = delete;
    context& operator=(context const&) = delete;

    unit& get_unit() const { return unit_; }

    // builtins
    void is_nil();
    void arrayify();
    void unpack();
    void referify();
    void weak_create();
    void weak_lock();
    void extern_variable_get();
    void extern_variable_set();
    //void construct_extern_object();
    void extern_object_create();
    void extern_object_set_property();
    void extern_object_get_property();
    void extern_function_call();
    void construct_function();

    void call_function_object();

    static small_string camel2kebab(string_view cc);

    inline size_t statics_size() const noexcept { return vm_.statics().size(); }
    inline size_t stack_size() const noexcept { return vm_.stack().size(); }

    variable_type const& static_at(size_t i) const
    {
        size_t ssz = statics_size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong static var index");
        }
        return vm_.statics()[i];
    }

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

    variable_type & stack_back(size_t i = 0)
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

    void stack_collapse(size_t n = 1)
    {
        size_t ssz = stack_size();
        if (ssz <= n) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong collapse count");
        }
        vm_.stack().at(ssz - n - 1).swap(stack_back());
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
    unit& unit_;
    vm_t& vm_;
    invocation::invocable* penv_;
};

}

namespace sonia::lang::bang {

class virtual_stack_machine : public sonia::vm::virtual_stack_machine<vm::context>
{
    using base_t = sonia::vm::virtual_stack_machine<vm::context>;

public:
    virtual_stack_machine();

    size_t append_static_const(smart_blob&& value);
    void append_push_static_const(smart_blob&& value);

    enum class builtin_fn
    {
        is_nil = 0,
        arrayify, unpack,
        referify, weak_create, weak_lock,
        function_constructor,
        extern_object_create, extern_object_set_property, extern_object_get_property,
        extern_variable_get, extern_variable_set,
        extern_function_call,
        eof_type
    };

    using base_t::append_ecall;
    void append_ecall(builtin_fn fn);

protected:

private:
    boost::unordered_map<blob_result, size_t> literals_;
};

}
