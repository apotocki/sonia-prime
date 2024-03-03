//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <vector>
#include "sonia/span.hpp"
#include "sonia/function.hpp"

//#include <boost/function_types/function_type.hpp>
//#include <boost/function_types/parameter_types.hpp>
//#include <boost/function_types/result_type.hpp>

//#include "sonia/utility/automatic_polymorphic.hpp"

namespace sonia::vm {

/*
template <typename ContextT, auto FuncV> struct machine_fd;

// , typename ... ArgsT
template <typename ContextT, typename RT>
struct machine_fd<ContextT, RT(ContextT::*)()>
{

};
*/
//
//template <auto FuncV>
//struct machine_fd
//{
//    using sig_t = decltype(FuncV);
//    using f_type = typename boost::function_types::function_type<sig_t>::type;
//    using result_type = typename boost::function_types::result_type<f_type>::type;
//};

/*
template <typename ContextT>
class command : public polymorphic_clonable, public polymorphic_movable
{
    using var_t = typename ContextT::variable_type;
public:
    // returns new stack size
    virtual size_t operator()(ContextT&, span<var_t> stack) const = 0;
};
*/

/*
class context {
public:
    using variable_type = unspecified
    constexpr size_t command_max_size = unspecified;
    bool is_zero(variable_type const&) const; // for conditional jumps
}
*/
template <typename ContextT>
class virtual_stack_machine
{
public:
    using var_t = typename ContextT::variable_type;
    //using cmd_t = automatic_polymorphic<command<ContextT>, ContextT::command_max_size>;

    using stack_type = std::vector<var_t>;
    using ext_function_t = void(*)(ContextT&);

private:
    stack_type stack_;
    std::vector<uint32_t> call_stack_;
    std::vector<uint8_t> code_;
    //std::vector<cmd_t> cmds_;
    std::vector<ext_function_t> efns_;

    enum class op : uint8_t {
        noop = 0, noop2 = 1, noop3 = 2, noop4 = 3, noop5 = 4, noop6 = 5, noop7 = 6, ret = 7,
        call1 = 8, call2 = 9, call3 = 10, call4 = 11, 
        jmp1 = 12, jmp2 = 13, jmp3 = 14, jmp4 = 15,
        jz1 = 16, jz2 = 17, jz3 = 18, jz4 = 19,
        jnz1 = 20, jnz2 = 21, jnz3 = 22, jnz4 = 23,
        push1 = 24, push2 = 25, push3 = 26, push4 = 27, // push INDEX:int
        pop0 = 28, pop1 = 29, pop2 = 30, pop3 = 31, // pop COUNT:int, pop0 === pop 1
        ecall1 = 32, ecall2 = 33, ecall3 = 34, ecall4 = 35
    };

    template <int N>
    size_t calc_address(size_t address);

    template <int N>
    size_t calc_uint(size_t address);

    inline var_t const& stack_back() const
    {
        if (stack_.empty()) throw internal_error("no value on stack");
        return stack_.back();
    }

    void append_generic_jump(op baseop, size_t address)
    {
        uint8_t base = static_cast<uint8_t>(baseop);
        intptr_t dist = static_cast<intptr_t>(address) - static_cast<intptr_t>(code_.size());
        if (dist >= -128 && dist <= 127) {
            code_.push_back(base);
            code_.push_back(static_cast<uint8_t>(dist));
        } else if (dist >= -32768 && dist <= 32767) {
            code_.push_back(base + 1);
            uint16_t dist16 = static_cast<uint16_t>(dist);
            code_.push_back(static_cast<uint8_t>(dist16));
            code_.push_back(static_cast<uint8_t>(dist16 >> 8));
        } else if (dist >= -8388608 && dist <= 8388607) {
            code_.push_back(base + 2);
            uint32_t dist32 = static_cast<uint32_t>(dist);
            code_.push_back(static_cast<uint8_t>(dist32));
            code_.push_back(static_cast<uint8_t>(dist32 >> 8));
            code_.push_back(static_cast<uint8_t>(dist32 >> 16));
        } else if (dist >= -2147483648 && dist <= 2147483647) {
            code_.push_back(base + 3);
            uint32_t dist32 = static_cast<uint32_t>(dist);
            code_.push_back(static_cast<uint8_t>(dist32));
            code_.push_back(static_cast<uint8_t>(dist32 >> 8));
            code_.push_back(static_cast<uint8_t>(dist32 >> 16));
            code_.push_back(static_cast<uint8_t>(dist32 >> 24));
        } else {
            throw internal_error("address is out of available range");
        }
    }

    void append_generic_uint(op baseop, size_t address)
    {
        uint8_t base = static_cast<uint8_t>(baseop);
        if (address <= 255) {
            code_.push_back(base);
            code_.push_back(static_cast<uint8_t>(address));
        } else if (address <= 65535) {
            code_.push_back(base + 1);
            uint16_t address16 = static_cast<uint16_t>(address);
            code_.push_back(static_cast<uint8_t>(address16));
            code_.push_back(static_cast<uint8_t>(address16 >> 8));
        } else if (address <= 16777215) {
            code_.push_back(base + 2);
            uint32_t address32 = static_cast<uint32_t>(address);
            code_.push_back(static_cast<uint8_t>(address32));
            code_.push_back(static_cast<uint8_t>(address32 >> 8));
            code_.push_back(static_cast<uint8_t>(address32 >> 16));
        } else if (address <= 4294967295) {
            code_.push_back(base + 3);
            uint32_t address32 = static_cast<uint32_t>(address);
            code_.push_back(static_cast<uint8_t>(address32));
            code_.push_back(static_cast<uint8_t>(address32 >> 8));
            code_.push_back(static_cast<uint8_t>(address32 >> 16));
            code_.push_back(static_cast<uint8_t>(address32 >> 24));
        } else {
            throw internal_error("address is out of available range");
        }
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

    void append_pop(size_t num)
    {
        if (num == 1) {
            code_.push_back(static_cast<uint8_t>(op::pop0));
        } else {
            append_generic_uint(op::pop1, num);
        }
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
};

template <typename ContextT>
template <int N>
inline size_t virtual_stack_machine<ContextT>::calc_address(size_t address)
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
    return static_cast<size_t>(static_cast<intptr_t>(address) + static_cast<int32_t>(uoffst));
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
void virtual_stack_machine<ContextT>::run(ContextT& ctx, size_t address)
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
        case op::ecall1:
            efns_.at(calc_uint<1>(address))(ctx);
            address += 2;
            continue;
        default:
            THROW_INTERNAL_ERROR();
        }
    }
}

}
