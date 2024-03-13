//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <vector>
#include "sonia/span.hpp"
#include "sonia/function.hpp"

#include <iostream>

namespace sonia::vm {

/*
class context
{
public:
    using variable_type = unspecified
    constexpr size_t command_max_size = unspecified;
    bool is_zero(variable_type const&) const; // for conditional jumps
    size_t to_address(variable_type const&) const; // for parametrized calls
}
*/

template <auto v> struct identity_type {};
template <auto v> inline constexpr identity_type<v> identity{};

template <typename ContextT>
class virtual_stack_machine
{
public:
    using var_t = typename ContextT::variable_type;

    using stack_type = std::vector<var_t>;
    using stack_frames_type = std::vector<uint32_t>;
    using call_stack_type = std::vector<uint32_t>;
    using ext_function_t = void(*)(ContextT&);

    enum class op : uint8_t {
        noop = 0, noop2 = 1, noop3 = 2, noop4 = 3, noop5 = 4, noop6 = 5, noop7 = 6, ret = 7,
        call1 = 8, call2 = 9, call3 = 10, call4 = 11, 
        jmp1 = 12, jmp2 = 13, jmp3 = 14, jmp4 = 15,
        jz1 = 16, jz2 = 17, jz3 = 18, jz4 = 19,
        jnz1 = 20, jnz2 = 21, jnz3 = 22, jnz4 = 23,
        push1 = 24, push2 = 25, push3 = 26, push4 = 27, // push INDEX:uint
        fpush1 = 28, fpush2 = 29, fpush3 = 30, fpush4 = 31, // push INDEX:int related to the current frame pointer
        pop0 = 32, pop1 = 33, pop2 = 34, pop3 = 35, // pop COUNT:uint, pop0 === pop 1
        set1 = 36, set2 = 37, set3 = 38, set4 = 39, // set INDEX:uint related to the current frame pointer
        fset1 = 40, fset2 = 41, fset3 = 42, fset4 = 43, // set INDEX:uint
        ecall1 = 44, ecall2 = 45, ecall3 = 46, ecall4 = 47,
        truncatefp1 = 48, truncatefp2 = 49, truncatefp3 = 50, truncatefp4 = 51,
        pushfp, popfp, truncatefp, callp
    };

private:
    stack_type stack_;
    call_stack_type call_stack_;
    stack_frames_type stack_frames_;
    std::vector<uint8_t> code_;
    std::vector<ext_function_t> efns_;

    template <int N>
    size_t calc_address(size_t address);

    template <int N>
    size_t calc_fsp(size_t address)
    {
        intptr_t curpos = stack_frames_.empty() ? 0 : static_cast<intptr_t>(stack_frames_.back());
        intptr_t result = curpos + calc_int<N>(address);
        if (result < 0) [[unlikely]] {
            throw internal_error("negative stack offset");
        }
        return result;
    }

    template <int N>
    size_t calc_uint(size_t address);

    template <int N>
    int32_t calc_int(size_t address);

    inline var_t const& stack_back() const
    {
        if (stack_.empty()) throw internal_error("no value on stack");
        return stack_.back();
    }

    void append_generic_int(op baseop, intptr_t val)
    {
        uint8_t base = static_cast<uint8_t>(baseop);
        if (val >= -128 && val <= 127) {
            code_.push_back(base);
            code_.push_back(static_cast<uint8_t>(val));
        } else if (val >= -32768 && val <= 32767) {
            code_.push_back(base + 1);
            uint16_t val16 = static_cast<uint16_t>(val);
            code_.push_back(static_cast<uint8_t>(val16));
            code_.push_back(static_cast<uint8_t>(val16 >> 8));
        } else if (val >= -8388608 && val <= 8388607) {
            code_.push_back(base + 2);
            uint32_t val32 = static_cast<uint32_t>(val);
            code_.push_back(static_cast<uint8_t>(val32));
            code_.push_back(static_cast<uint8_t>(val32 >> 8));
            code_.push_back(static_cast<uint8_t>(val32 >> 16));
        } else if (val >= -2147483648 && val <= 2147483647) {
            code_.push_back(base + 3);
            uint32_t val32 = static_cast<uint32_t>(val);
            code_.push_back(static_cast<uint8_t>(val32));
            code_.push_back(static_cast<uint8_t>(val32 >> 8));
            code_.push_back(static_cast<uint8_t>(val32 >> 16));
            code_.push_back(static_cast<uint8_t>(val32 >> 24));
        } else {
            throw internal_error("signed value is out of available range");
        }
    }

    void append_generic_uint(op baseop, size_t val)
    {
        uint8_t base = static_cast<uint8_t>(baseop);
        if (val <= 255) {
            code_.push_back(base);
            code_.push_back(static_cast<uint8_t>(val));
        } else if (val <= 65535) {
            code_.push_back(base + 1);
            uint16_t val16 = static_cast<uint16_t>(val);
            code_.push_back(static_cast<uint8_t>(val16));
            code_.push_back(static_cast<uint8_t>(val16 >> 8));
        } else if (val <= 16777215) {
            code_.push_back(base + 2);
            uint32_t val32 = static_cast<uint32_t>(val);
            code_.push_back(static_cast<uint8_t>(val32));
            code_.push_back(static_cast<uint8_t>(val32 >> 8));
            code_.push_back(static_cast<uint8_t>(val32 >> 16));
        } else if (val <= 4294967295) {
            code_.push_back(base + 3);
            uint32_t val32 = static_cast<uint32_t>(val);
            code_.push_back(static_cast<uint8_t>(val32));
            code_.push_back(static_cast<uint8_t>(val32 >> 8));
            code_.push_back(static_cast<uint8_t>(val32 >> 16));
            code_.push_back(static_cast<uint8_t>(val32 >> 24));
        } else {
            throw internal_error("unsigned value '%1%' is out of available range"_fmt % val);
        }
    }

    void append_generic_jump(op baseop, size_t address)
    {
        intptr_t dist = static_cast<intptr_t>(address) - static_cast<intptr_t>(code_.size());
        append_generic_int(baseop, dist);
    }

public:
    virtual_stack_machine() = default;

    size_t get_ip() const { return code_.size(); }

    stack_type& stack() { return stack_; }

    void append_noop(unsigned int skipsz)
    {
        if (skipsz > 7 || !skipsz) [[unlikely]] {
            throw internal_error("wrong noop skip size");
        }
        code_.push_back(static_cast<uint8_t>(skipsz - 1));
    }

    /*
    template <std::derived_from<command<ContextT>> CmdT, typename ... ArgsT>
    size_t add_command(ArgsT&& ... args)
    {
        cmds_.emplace_back(in_place_type<CmdT>, std::forward<ArgsT>(args) ...);
        return cmds_.size() - 1;
    }
    
    void append_command(size_t cmdid)
    {
        if (cmdid < 128) {
            code_.push_back(static_cast<uint8_t>(128 + cmdid));
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    }
    */

    void append_jmp(size_t address)
    {
        append_generic_jump(op::jmp1, address);
    }

    void append_jz(size_t address)
    {
        append_generic_jump(op::jz1, address);
    }

    void append_jnz(size_t address)
    {
        append_generic_jump(op::jnz1, address);
    }

    void append_call(size_t address)
    {
        append_generic_jump(op::call1, address);
    }

    void append_ecall(size_t num)
    {
        if (num < 128) {
            code_.push_back(static_cast<uint8_t>(128 + num));
        } else {
            append_generic_uint(op::ecall1, num);
        }
    }

    void append_ret()
    {
        code_.push_back(static_cast<uint8_t>(op::ret));
    }

    void append_push(size_t num)
    {
        append_generic_uint(op::push1, num);
    }

    void append_fpush(intptr_t fpos)
    {
        append_generic_int(op::fpush1, fpos);
    }

    void append_pushfp()
    {
        code_.push_back(static_cast<uint8_t>(op::pushfp));
    }

    void append_popfp()
    {
        code_.push_back(static_cast<uint8_t>(op::popfp));
    }

    void append_truncatefp(intptr_t offset)
    {
        append_generic_int(op::truncatefp1, offset);
    }

    void append_pop(size_t num)
    {
        if (num == 1) {
            code_.push_back(static_cast<uint8_t>(op::pop0));
        } else {
            append_generic_uint(op::pop1, num);
        }
    }

    void append_set(size_t num)
    {
        append_generic_uint(op::set1, num);
    }

    void append_fset(intptr_t fpos)
    {
        append_generic_int(op::fset1, fpos);
    }

    void appned_callp()
    {
        code_.push_back(static_cast<uint8_t>(op::callp));
    }

    size_t push_on_stack(var_t value)
    {
        size_t pos = stack_.size();
        stack_.emplace_back(std::move(value));
        return pos;
    }

    void set_at_stack(size_t pos, var_t value)
    {
        if (stack_.size() <= pos) {
            stack_.resize(pos + 1);
        }
        stack_[pos] = std::move(value);
    }

    size_t push_external_fn(ext_function_t && efn)
    {
        size_t pos = efns_.size();
        efns_.emplace_back(std::move(efn));
        return pos;
    }

    void run(ContextT& ctx, size_t address);
    //void run2(ContextT& ctx, size_t address);

    template <typename FunctorT>
    void traverse(ContextT& ctx, size_t address, FunctorT const& ftor);
};

template <typename ContextT>
template <int N>
inline int32_t virtual_stack_machine<ContextT>::calc_int(size_t address)
{
    if (address + N >= code_.size()) [[unlikely]] throw internal_error("unfinished command");
    uint32_t uoffst = static_cast<int32_t>(static_cast<int8_t>(code_[address + N]));
    if constexpr (N > 1) {
        uoffst <<= 8; uoffst += code_[address + N - 1];
    }
    if constexpr (N > 2) {
        uoffst <<= 8; uoffst += code_[address + N - 2];
    }
    if constexpr (N > 3) {
        uoffst <<= 8; uoffst += code_[address + N - 3];
    }
    return static_cast<int32_t>(uoffst);
}

template <typename ContextT>
template <int N>
inline size_t virtual_stack_machine<ContextT>::calc_uint(size_t address)
{
    if (address + N >= code_.size()) [[unlikely]] throw internal_error("unfinished command");
    uint32_t uval = code_[address + N];
    if constexpr (N > 1) {
        uval <<= 8; uval += code_[address + N - 1];
    }
    if constexpr (N > 2) {
        uval <<= 8; uval += code_[address + N - 2];
    }
    if constexpr (N > 3) {
        uval <<= 8; uval += code_[address + N - 3];
    }
    return uval;
}

template <typename ContextT>
template <int N>
inline size_t virtual_stack_machine<ContextT>::calc_address(size_t address)
{
    int32_t diff = calc_int<N>(address);
    return static_cast<size_t>(static_cast<intptr_t>(address) + diff);
}

template <typename ContextT>
struct printer
{
    using vm_t = virtual_stack_machine<ContextT>;
    using stack_type = typename vm_t::stack_type;
    using call_stack_type = typename vm_t::call_stack_type;
    using var_t = typename vm_t::var_t;
    using op = vm_t::op;

    std::ostream& ss;
    explicit printer(std::ostream & s) : ss{s} {}

    std::ostream& generic_print(size_t address, string_view cmd) const
    {
        return ss << "0x" << std::hex << std::setfill('0') << std::setw(4) << address << " " << cmd;
    }

    inline void operator()(identity_type<op::noop>, ContextT&, size_t address, uint8_t op_byte) const
    {
        generic_print(address, "noop"sv) << " " << (int)op_byte << "\n";
    }

    inline void operator()(ContextT&, size_t address, typename vm_t::ext_function_t& ef) const
    {
        generic_print(address, "ecall"sv) << " 0x" << (uintptr_t)&ef << "\n";
    }

    inline optional<size_t> operator()(identity_type<op::ret>, ContextT&, size_t address, call_stack_type& cs) const
    {
        generic_print(address, "ret"sv) << "\n";
        return nullopt;
    }

    inline size_t operator()(identity_type<op::call1>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        generic_print(address, "call1"sv) << " 0x" << call_address << "\n";
        return address + 2;
    }

    inline size_t operator()(identity_type<op::call2>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        generic_print(address, "call2"sv) << " 0x" << call_address << "\n";
        return address + 3;
    }

    inline size_t operator()(identity_type<op::call3>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        generic_print(address, "call3"sv) << " 0x" << call_address << "\n";
        return address + 4;
    }

    inline size_t operator()(identity_type<op::call4>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        generic_print(address, "call4"sv) << " 0x" << call_address << "\n";
        return address + 5;
    }

    inline size_t operator()(identity_type<op::jmp1>, ContextT&, size_t address, size_t jmp_address) const
    {
        generic_print(address, "jmp1"sv) << " 0x" << jmp_address << "\n";
        return address + 2;
    }

    inline size_t operator()(identity_type<op::jmp2>, ContextT&, size_t address, size_t jmp_address) const
    {
        generic_print(address, "jmp2"sv) << " 0x" << jmp_address << "\n";
        return address + 3;
    }

    inline size_t operator()(identity_type<op::jmp3>, ContextT&, size_t address, size_t jmp_address) const
    {
        generic_print(address, "jmp3"sv) << " 0x" << jmp_address << "\n";
        return address + 4;
    }

    inline size_t operator()(identity_type<op::jmp4>, ContextT&, size_t address, size_t jmp_address) const
    {
        generic_print(address, "jmp4"sv) << " 0x" << jmp_address << "\n";
        return address + 5;
    }

    inline size_t operator()(identity_type<op::jz1>, ContextT& ctx, size_t address, var_t const&, size_t jmp_address) const
    {
        generic_print(address, "jz1"sv) << " 0x" << jmp_address << "\n";
        return address + 2;
    }

    inline size_t operator()(identity_type<op::jz2>, ContextT& ctx, size_t address, var_t const&, size_t jmp_address) const
    {
        generic_print(address, "jz2"sv) << " 0x" << jmp_address << "\n";
        return address + 3;
    }

    inline size_t operator()(identity_type<op::jz3>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        generic_print(address, "jz3"sv) << " 0x" << jmp_address << "\n";
        return address + 4;
    }

    inline size_t operator()(identity_type<op::jz4>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        generic_print(address, "jz4"sv) << " 0x" << jmp_address << "\n";
        return address + 5;
    }

    inline size_t operator()(identity_type<op::jnz1>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        generic_print(address, "jnz1"sv) << " 0x" << jmp_address << "\n";
        return address + 2;
    }

    inline size_t operator()(identity_type<op::jnz2>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        generic_print(address, "jnz2"sv) << " 0x" << jmp_address << "\n";
        return address + 3;
    }

    inline size_t operator()(identity_type<op::jnz3>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        generic_print(address, "jnz3"sv) << " 0x" << jmp_address << "\n";
        return address + 4;
    }

    inline size_t operator()(identity_type<op::jnz4>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        generic_print(address, "jnz4"sv) << " 0x" << jmp_address << "\n";
        return address + 5;
    }

    inline void operator()(identity_type<op::pop0>, ContextT& ctx, size_t address, stack_type& s) const
    {
        generic_print(address, "pop0"sv) << "\n";
    }

    inline void operator()(identity_type<op::pop1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "pop1"sv) << " " << n << "\n";
    }

    inline void operator()(identity_type<op::pop2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "pop2"sv) << " " << n << "\n";
    }

    inline void operator()(identity_type<op::pop3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "pop3"sv) << " " << n << "\n";
    }

    inline void operator()(identity_type<op::push1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "push1"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::push2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "push2"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::push3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "push3"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::push4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "push4"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::fpush1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fpush1"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::fpush2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fpush2"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::fpush3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fpush3"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::fpush4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fpush4"sv) << " [" << std::dec << n << "] (" << s.at(n) << ")" << "\n";
    }

    inline void operator()(identity_type<op::set1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "set1"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::set2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "set2"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::set3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "set3"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::set4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "set4"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::fset1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fset1"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::fset2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fset2"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::fset3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fset3"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::fset4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const
    {
        generic_print(address, "fset4"sv) << " [" << std::dec << n << "] (" << s.back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::ecall1>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const
    {
        generic_print(address, "ecall1"sv) << " 0x" << (uintptr_t)&ef << "\n";
    }

    inline void operator()(identity_type<op::ecall2>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const
    {
        generic_print(address, "ecall2"sv) << " 0x" << (uintptr_t)&ef << "\n";
    }

    inline void operator()(identity_type<op::ecall3>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const
    {
        generic_print(address, "ecall3"sv) << " 0x" << (uintptr_t)&ef << "\n";
    }

    inline void operator()(identity_type<op::ecall4>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const
    {
        generic_print(address, "ecall4"sv) << " 0x" << (uintptr_t)&ef << "\n";
    }

    inline void operator()(identity_type<op::pushfp>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, uint32_t ssz) const
    {
        generic_print(address, "pushfp"sv) << "\n";
    }

    inline void operator()(identity_type<op::popfp>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf) const
    {
        generic_print(address, "popfp"sv) << "\n";
    }

    inline void operator()(identity_type<op::truncatefp1>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        generic_print(address, "truncatefp1"sv) << " " << std::dec << offs << "\n";
    }
    inline void operator()(identity_type<op::truncatefp2>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        generic_print(address, "truncatefp2"sv) << " " << std::dec << offs << "\n";
    }
    inline void operator()(identity_type<op::truncatefp3>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        generic_print(address, "truncatefp3"sv) << " " << std::dec << offs << "\n";
    }
    inline void operator()(identity_type<op::truncatefp4>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        generic_print(address, "truncatefp4"sv) << " " << std::dec << offs << "\n";
    }

    inline size_t operator()(identity_type<op::callp>, ContextT& ctx, size_t address, stack_type& s, call_stack_type& cs) const
    {
        generic_print(address, "callp"sv) << " " << std::hex << "0x" << ctx.to_address(s.back()) << "\n";
        return address + 1;
    }
};

template <typename ContextT>
struct runner
{
    using vm_t = virtual_stack_machine<ContextT>;
    using stack_type = typename vm_t::stack_type;
    using call_stack_type = typename vm_t::call_stack_type;
    using var_t = typename vm_t::var_t;
    using op = vm_t::op;
    
    inline void operator()(identity_type<op::noop>, ContextT &, size_t address, uint8_t op_byte) const {}
    inline void operator()(ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const { ef(ctx); }

    inline optional<size_t> operator()(identity_type<op::ret>, ContextT&, size_t address, call_stack_type & cs) const
    {
        if (cs.empty()) return nullopt;
        address = cs.back();
        cs.pop_back();
        return address;
    }

    inline size_t operator()(identity_type<op::call1>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        cs.push_back(static_cast<uint32_t>(address + 2));
        return call_address;
    }

    inline size_t operator()(identity_type<op::call2>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        cs.push_back(static_cast<uint32_t>(address + 3));
        return call_address;
    }

    inline size_t operator()(identity_type<op::call3>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        cs.push_back(static_cast<uint32_t>(address + 4));
        return call_address;
    }

    inline size_t operator()(identity_type<op::call4>, ContextT&, size_t address, call_stack_type& cs, size_t call_address) const
    {
        cs.push_back(static_cast<uint32_t>(address + 5));
        return call_address;
    }

    inline size_t operator()(identity_type<op::jmp1>, ContextT&, size_t address, size_t call_address) const { return call_address; }
    inline size_t operator()(identity_type<op::jmp2>, ContextT&, size_t address, size_t call_address) const { return call_address; }
    inline size_t operator()(identity_type<op::jmp3>, ContextT&, size_t address, size_t call_address) const { return call_address; }
    inline size_t operator()(identity_type<op::jmp4>, ContextT&, size_t address, size_t call_address) const { return call_address; }
    
    inline size_t operator()(identity_type<op::jz1>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? jmp_address : (address + 2);
    }

    inline size_t operator()(identity_type<op::jz2>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? jmp_address : (address + 3);
    }

    inline size_t operator()(identity_type<op::jz3>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? jmp_address : (address + 4);
    }

    inline size_t operator()(identity_type<op::jz4>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? jmp_address : (address + 5);
    }

    inline size_t operator()(identity_type<op::jnz1>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? (address + 2) : jmp_address;
    }

    inline size_t operator()(identity_type<op::jnz2>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? (address + 3) : jmp_address;
    }

    inline size_t operator()(identity_type<op::jnz3>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? (address + 4) : jmp_address;
    }

    inline size_t operator()(identity_type<op::jnz4>, ContextT& ctx, size_t address, var_t const& val, size_t jmp_address) const
    {
        return ctx.is_zero(val) ? (address + 5) : jmp_address;
    }

    inline void operator()(identity_type<op::pop0>, ContextT& ctx, size_t address, stack_type& s) const { s.pop_back(); }
    inline void operator()(identity_type<op::pop1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.resize(s.size() - n); }
    inline void operator()(identity_type<op::pop2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.resize(s.size() - n); }
    inline void operator()(identity_type<op::pop3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.resize(s.size() - n); }
    
    inline void operator()(identity_type<op::push1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::push2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::push3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::push4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::fpush1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::fpush2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::fpush3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::fpush4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const {
        s.emplace_back();
        s.back() = s.at(n);
    }

    inline void operator()(identity_type<op::set1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }
    inline void operator()(identity_type<op::set2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }
    inline void operator()(identity_type<op::set3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }
    inline void operator()(identity_type<op::set4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }

    inline void operator()(identity_type<op::fset1>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }
    inline void operator()(identity_type<op::fset2>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }
    inline void operator()(identity_type<op::fset3>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }
    inline void operator()(identity_type<op::fset4>, ContextT& ctx, size_t address, stack_type& s, size_t n) const { s.at(n) = s.back(); }

    inline void operator()(identity_type<op::ecall1>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const { ef(ctx); }
    inline void operator()(identity_type<op::ecall2>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const { ef(ctx); }
    inline void operator()(identity_type<op::ecall3>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const { ef(ctx); }
    inline void operator()(identity_type<op::ecall4>, ContextT& ctx, size_t address, typename vm_t::ext_function_t& ef) const { ef(ctx); }

    inline void operator()(identity_type<op::pushfp>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, uint32_t ssz) const { sf.emplace_back(ssz); }
    inline void operator()(identity_type<op::popfp>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf) const { if (!sf.empty()) sf.pop_back(); }
    
    inline void truncatefp(ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        int32_t threshold = static_cast<int32_t>(sf.back()) + offs;
        if (threshold < 0) throw internal_error("wrong truncation argument: %1%"_fmt % threshold);
        s.resize(threshold);
    }
    inline void operator()(identity_type<op::truncatefp1>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        return truncatefp(ctx, address, sf, s, offs);
    }
    inline void operator()(identity_type<op::truncatefp2>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        return truncatefp(ctx, address, sf, s, offs);
    }
    inline void operator()(identity_type<op::truncatefp3>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        return truncatefp(ctx, address, sf, s, offs);
    }
    inline void operator()(identity_type<op::truncatefp4>, ContextT& ctx, size_t address, typename vm_t::stack_frames_type& sf, stack_type& s, int32_t offs) const
    {
        return truncatefp(ctx, address, sf, s, offs);
    }

    inline size_t operator()(identity_type<op::callp>, ContextT& ctx, size_t address, stack_type& s, call_stack_type& cs) const {
        cs.push_back(static_cast<uint32_t>(address + 1));
        size_t call_address = ctx.to_address(s.back());
        s.pop_back();
        return call_address;
    }
};

template <typename FirstRunnerT, typename SecondRunnerT>
struct sequence_runner
{
    FirstRunnerT const& first;
    SecondRunnerT const& second;

    sequence_runner(FirstRunnerT const& f, SecondRunnerT const& s) : first{f}, second{s} {}

    template <typename ... ArgsT>
    inline auto operator()(ArgsT&& ... args) const
    {
        first(std::forward<ArgsT>(args)...);
        return second(std::forward<ArgsT>(args)...);
    }
};

template <typename ContextT>
template <typename FunctorT>
void virtual_stack_machine<ContextT>::traverse(ContextT& ctx, size_t address, FunctorT const& ftor)
{
    while (address < code_.size()) {
        uint8_t op_byte = code_[address];
        if (op_byte < 7) { // noops
            ftor(identity<op::noop>, ctx, address, op_byte);
            address += op_byte + 1;
            continue;
        } else if (op_byte >= 128) {
            auto cmdoffset = op_byte - 128;
            if (cmdoffset >= efns_.size()) [[unlikely]] {
                throw internal_error("wrong command");
            }
            ftor(ctx, address, efns_[cmdoffset]);
            ++address;
            continue;
        }
        switch (static_cast<op>(op_byte)) {
        case op::ret:
            if (auto optaddress = ftor(identity<op::ret>, ctx, address, call_stack_); optaddress) {
                address = *optaddress;
                continue;
            }
            return;
            
        case op::call1:
            address = ftor(identity<op::call1>, ctx, address, call_stack_, calc_address<1>(address));
            continue;
        case op::call2:
            address = ftor(identity<op::call2>, ctx, address, call_stack_, calc_address<2>(address));
            continue;
        case op::call3:
            address = ftor(identity<op::call3>, ctx, address, call_stack_, calc_address<3>(address));
            continue;
        case op::call4:
            address = ftor(identity<op::call4>, ctx, address, call_stack_, calc_address<4>(address));
            continue;
        case op::jmp1:
            address = ftor(identity<op::jmp1>, ctx, address, calc_address<1>(address));
            continue;
        case op::jmp2:
            address = ftor(identity<op::jmp2>, ctx, address, calc_address<2>(address));
            continue;
        case op::jmp3:
            address = ftor(identity<op::jmp3>, ctx, address, calc_address<3>(address));
            continue;
        case op::jmp4:
            address = ftor(identity<op::jmp4>, ctx, address, calc_address<4>(address));
            continue;

        case op::jz1:
            address = ftor(identity<op::jz1>, ctx, address, stack_back(), calc_address<1>(address));
            continue;
        case op::jz2:
            address = ftor(identity<op::jz2>, ctx, address, stack_back(), calc_address<2>(address));
            continue;
        case op::jz3:
            address = ftor(identity<op::jz3>, ctx, address, stack_back(), calc_address<3>(address));
            continue;
        case op::jz4:
            address = ftor(identity<op::jz4>, ctx, address, stack_back(), calc_address<4>(address));
            continue;

        case op::jnz1:
            address = ftor(identity<op::jnz1>, ctx, address, stack_back(), calc_address<1>(address));
            continue;
        case op::jnz2:
            address = ftor(identity<op::jnz2>, ctx, address, stack_back(), calc_address<2>(address));
            continue;
        case op::jnz3:
            address = ftor(identity<op::jnz3>, ctx, address, stack_back(), calc_address<3>(address));
            continue;
        case op::jnz4:
            address = ftor(identity<op::jnz4>, ctx, address, stack_back(), calc_address<4>(address));
            continue;

        case op::pop0:
            ftor(identity<op::pop0>, ctx, address, stack_);
            ++address;
            continue;
        case op::pop1:
            ftor(identity<op::pop1>, ctx, address, stack_, calc_uint<1>(address));
            address += 2;
            continue;
        case op::pop2:
            ftor(identity<op::pop2>, ctx, address, stack_, calc_uint<2>(address));
            address += 3;
            continue;
        case op::pop3:
            ftor(identity<op::pop3>, ctx, address, stack_, calc_uint<3>(address));
            address += 4;
            continue;
        case op::push1:
            ftor(identity<op::push1>, ctx, address, stack_, calc_uint<1>(address));
            address += 2;
            continue;
        case op::push2:
            ftor(identity<op::push2>, ctx, address, stack_, calc_uint<2>(address));
            address += 3;
            continue;
        case op::push3:
            ftor(identity<op::push3>, ctx, address, stack_, calc_uint<3>(address));
            address += 4;
            continue;
        case op::push4:
            ftor(identity<op::push4>, ctx, address, stack_, calc_uint<4>(address));
            address += 5;
            continue;

        case op::fpush1:
            ftor(identity<op::fpush1>, ctx, address, stack_, calc_fsp<1>(address));
            address += 2;
            continue;
        case op::fpush2:
            ftor(identity<op::fpush2>, ctx, address, stack_, calc_fsp<2>(address));
            address += 3;
            continue;
        case op::fpush3:
            ftor(identity<op::fpush3>, ctx, address, stack_, calc_fsp<3>(address));
            address += 4;
            continue;
        case op::fpush4:
            ftor(identity<op::fpush4>, ctx, address, stack_, calc_fsp<4>(address));
            address += 5;
            continue;

        case op::set1:
            ftor(identity<op::set1>, ctx, address, stack_, calc_uint<1>(address));
            address += 2;
            continue;
        case op::set2:
            ftor(identity<op::set2>, ctx, address, stack_, calc_uint<2>(address));
            address += 3;
            continue;
        case op::set3:
            ftor(identity<op::set3>, ctx, address, stack_, calc_uint<3>(address));
            address += 4;
            continue;
        case op::set4:
            ftor(identity<op::set4>, ctx, address, stack_, calc_uint<4>(address));
            address += 5;
            continue;

        case op::fset1:
            ftor(identity<op::fset1>, ctx, address, stack_, calc_fsp<1>(address));
            address += 2;
            continue;
        case op::fset2:
            ftor(identity<op::fset2>, ctx, address, stack_, calc_fsp<2>(address));
            address += 3;
            continue;
        case op::fset3:
            ftor(identity<op::fset3>, ctx, address, stack_, calc_fsp<3>(address));
            address += 4;
            continue;
        case op::fset4:
            ftor(identity<op::fset4>, ctx, address, stack_, calc_fsp<4>(address));
            address += 5;
            continue;

        case op::ecall1:
            ftor(identity<op::ecall1>, ctx, address, efns_.at(calc_uint<1>(address)));
            address += 2;
            continue;
        case op::ecall2:
            ftor(identity<op::ecall2>, ctx, address, efns_.at(calc_uint<2>(address)));
            address += 3;
            continue;
        case op::ecall3:
            ftor(identity<op::ecall3>, ctx, address, efns_.at(calc_uint<3>(address)));
            address += 4;
            continue;
        case op::ecall4:
            ftor(identity<op::ecall4>, ctx, address, efns_.at(calc_uint<4>(address)));
            address += 5;
            continue;

        case op::truncatefp1:
            ftor(identity<op::truncatefp1>, ctx, address, stack_frames_, stack_, calc_int<1>(address));
            address += 2;
            continue;
        case op::truncatefp2:
            ftor(identity<op::truncatefp2>, ctx, address, stack_frames_, stack_, calc_int<2>(address));
            address += 3;
            continue;
        case op::truncatefp3:
            ftor(identity<op::truncatefp2>, ctx, address, stack_frames_, stack_, calc_int<3>(address));
            address += 4;
            continue;
        case op::truncatefp4:
            ftor(identity<op::truncatefp2>, ctx, address, stack_frames_, stack_, calc_int<4>(address));
            address += 5;
            continue;

        case op::pushfp:
            ftor(identity<op::pushfp>, ctx, address, stack_frames_, static_cast<uint32_t>(stack_.size()));
            ++address;
            continue;
        case op::popfp:
            ftor(identity<op::popfp>, ctx, address, stack_frames_);
            ++address;
            continue;

        
        case op::callp:
            address = ftor(identity<op::callp>, ctx, address, stack_, call_stack_);
            continue;
        default:
            THROW_INTERNAL_ERROR();
        }
    }
}

template <typename ContextT>
void virtual_stack_machine<ContextT>::run(ContextT& ctx, size_t address)
{
    sequence_runner<printer<ContextT>, runner<ContextT>> rn{ printer<ContextT>{std::cout}, {}};
    //printer<ContextT> rn{ std::cout };
    //runner<ContextT> rn;
    traverse(ctx, address, rn);
}

/*
template <typename ContextT>
void virtual_stack_machine<ContextT>::run2(ContextT& ctx, size_t address)
{
    while (address < code_.size()) {
        uint8_t op_byte = code_[address];
        if (op_byte < 7) { // noops
            address += op_byte + 1;
            continue;
        } else if (op_byte >= 128) {
            auto cmdoffset = op_byte - 128;
            if (cmdoffset >= efns_.size()) [[unlikely]] {
                throw internal_error("wrong command");
            }
            efns_[cmdoffset](ctx);
            ++address;
            continue;
        }
        switch (static_cast<op>(op_byte)) {
        case op::ret:
            if (call_stack_.empty()) return;
            address = call_stack_.back();
            call_stack_.pop_back();
            continue;
        case op::call1:
            call_stack_.push_back(static_cast<uint32_t>(address + 2));
        case op::jmp1:
            address = calc_address<1>(address);
            continue;
        case op::call2:
            call_stack_.push_back(static_cast<uint32_t>(address + 3));
        case op::jmp2:
            address = calc_address<2>(address);
            continue;
        case op::call3:
            call_stack_.push_back(static_cast<uint32_t>(address + 4));
        case op::jmp3:
            address = calc_address<3>(address);
            continue;
        case op::call4:
            call_stack_.push_back(static_cast<uint32_t>(address + 5));
        case op::jmp4:
            address = calc_address<4>(address);
            continue;

        case op::jz1:
        case op::jnz1:
            if (ctx.is_zero(stack_back()) ^ static_cast<bool>(op_byte & 4)) {
                address = calc_address<1>(address);
            } else {
                address += 2;
            }
            continue;
        case op::jz2:
        case op::jnz2:
            if (ctx.is_zero(stack_back()) ^ static_cast<bool>(op_byte & 4)) {
                address = calc_address<2>(address);
            } else {
                address += 3;
            }
            continue;
        case op::jz3:
        case op::jnz3:
            if (ctx.is_zero(stack_back()) ^ static_cast<bool>(op_byte & 4)) {
                address = calc_address<3>(address);
            } else {
                address += 4;
            }
            continue;
        case op::jz4:
        case op::jnz4:
            if (ctx.is_zero(stack_back()) ^ static_cast<bool>(op_byte & 4)) {
                address = calc_address<4>(address);
            } else {
                address += 5;
            }
            continue;
        case op::pop0:
            stack_.pop_back();
            ++address;
            continue;
        case op::pop1:
            stack_.resize(stack_.size() - calc_uint<1>(address));
            address += 2;
            continue;
        case op::pop2:
            stack_.resize(stack_.size() - calc_uint<2>(address));
            address += 3;
            continue;
        case op::pop3:
            stack_.resize(stack_.size() - calc_uint<3>(address));
            address += 4;
            continue;
        case op::push1:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_uint<1>(address));
            address += 2;
            continue;
        case op::push2:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_uint<2>(address));
            address += 3;
            continue;
        case op::push3:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_uint<3>(address));
            address += 4;
            continue;
        case op::push4:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_uint<4>(address));
            address += 5;
            continue;
        case op::fpush1:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_fsp<1>(address));
            address += 2;
            continue;
        case op::fpush2:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_fsp<2>(address));
            address += 3;
            continue;
        case op::fpush3:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_fsp<3>(address));
            address += 4;
            continue;
        case op::fpush4:
            stack_.emplace_back();
            stack_.back() = stack_.at(calc_fsp<4>(address));
            address += 5;
            continue;
        case op::set1:
            stack_.at(calc_uint<1>(address)) = stack_.back();
            address += 2;
            continue;
        case op::set2:
            stack_.at(calc_uint<2>(address)) = stack_.back();
            address += 3;
            continue;
        case op::set3:
            stack_.at(calc_uint<3>(address)) = stack_.back();
            address += 4;
            continue;
        case op::set4:
            stack_.at(calc_uint<4>(address)) = stack_.back();
            address += 5;
            continue;
        case op::fset1:
            stack_.at(calc_fsp<1>(address)) = stack_.back();
            address += 2;
            continue;
        case op::fset2:
            stack_.at(calc_fsp<2>(address)) = stack_.back();
            address += 3;
            continue;
        case op::fset3:
            stack_.at(calc_fsp<3>(address)) = stack_.back();
            address += 4;
            continue;
        case op::fset4:
            stack_.at(calc_fsp<4>(address)) = stack_.back();
            address += 5;
            continue;
        case op::ecall1:
            efns_.at(calc_uint<1>(address))(ctx);
            address += 2;
            continue;
        case op::pushfp:
            stack_frames_.emplace_back(static_cast<uint32_t>(stack_.size()));
            ++address;
            continue;
        case op::popfp:
            if (!stack_frames_.empty()) stack_frames_.pop_back();
            ++address;
        case op::truncatefp:
            stack_.resize(stack_frames_.empty() ? 0 : stack_frames_.back());
            ++address;
            continue;
        default:
            THROW_INTERNAL_ERROR();
        }
    }
}
*/

}
