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
    bool is_true(variable_type const&) const; // for conditional jumps
    variable_type value_of(size_t val) const; // for pushi
    
    variable_type const& static_at(size_t index) const;

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
    
    using ext_function_t = void(*)(ContextT&);

    enum class op : uint8_t {
        noop = 0,
        jmp = 1, jt = 2, jf = 3,
        jmpp = 4, jmpn = 5,
        jtp = 6, jtn = 7,
        jfp = 8, jfn = 9,
        call = 10,  // call(address) -> call address
        callp = 11, // call() -> call stack[stack_back_pos()]
        ecall = 12, // ecall(index) -> call efn[index]
        //ecall1 = 7, // call(fn_index), call(param index, fn_index)
        //fpecall1 = 8, fnecall1 = 9, //call(param +-offset, fn_index)
        ret = 13,

        // data move
        push = 16, // push on stack stack[uint]
        pushr = 17, // push on stack stack[stack_back_pos() - uint] // r aka relative
        fppush = 18, fnpush = 19, // push on stack stack[fp +- uint]
        pushi = 20, fppushi = 21, fnpushi = 22, // push index on stack // positive/negative index
        pushc = 23, // push const on stack: consts_[uint] -> on stack
        set = 24, // set stack[uint] = stack_top_value
        setr = 25, // set stack[stack_back_pos() - uint] = stack_top_value // r aka relative
        fpset = 26, fnset = 27, // set stack[fp +- uint] = stack_top_value
        pop, popn, // pop COUNT:uint, pop0 === pop 1
        collapse, // pop COUNT before the back
        // frame data pointer
        pushfp, popfp, truncatefpp, truncatefpn
    };

protected:
    std::vector<var_t> consts_;
    stack_type stack_;
    
    std::vector<uint8_t> code_;
    std::vector<std::pair<ext_function_t, small_string>> efns_;
    //std::vector<ext1_function_t> e1fns_;

    size_t read_uint(size_t& address) const
    {
        size_t result = 0;
        while (code_.size() > address) {
            uint8_t b = code_[address++];
            result |= b & 0x7f;
            if (b & 0x80) [[unlikely]] {
                result <<= 7;
            } else {
                return result;
            }
        }
        THROW_INTERNAL_ERROR("incomplete uint");
    }

    void append_uint(size_t uval)
    {
        uint8_t b = static_cast<uint8_t>(uval & 0x7f);
        bool has_more = !!(uval - b);
        code_.push_back(b);
        if (!has_more) return;
        size_t pos = code_.size() - 1;
        for (;;) {
            uval >>= 7;
            uint8_t b = static_cast<uint8_t>(uval & 0x7f);
            bool has_more = !!(uval - b);
            code_.push_back(b + 0x80);
            if (!has_more) {
                std::reverse(code_.begin() + pos, code_.end());
                return;
            }
        }
    }

    inline var_t const& stack_back() const
    {
        if (stack_.empty()) throw internal_error("no value on stack");
        return stack_.back();
    }

public:
    virtual_stack_machine() = default;

    size_t get_ip() const { return code_.size(); }

    void swap_code_blocks(size_t first_begin, size_t first_end)
    {
        std::vector<uint8_t> tmp;
        tmp.insert(tmp.end(), code_.begin() + first_end, code_.end());
        code_.resize(first_end);
        code_.insert(code_.begin() + first_begin, tmp.begin(), tmp.end());
    }

    auto& consts() { return consts_; }
    auto& stack() { return stack_; }
    auto& efns() const { return efns_; }

    void set_efn(size_t idx, ext_function_t pfn, small_string descr = {})
    {
        if (efns_.size() <= idx) {
            efns_.resize(idx + 1);
        }
        efns_[idx] = std::pair{ pfn, descr };
    }

    //void set_efn1(size_t idx, ext1_function_t pfn)
    //{
    //    if (efns_.size() <= idx) {
    //        e1fns_.resize(idx + 1);
    //    }
    //    e1fns_[idx] = pfn;
    //}

    void append(span<uint8_t> fragment)
    {
        code_.insert(code_.end(), fragment.begin(), fragment.end());
    }

    void append_noop()
    {
        code_.push_back(static_cast<uint8_t>(op::noop));
    }

    void append_jmp(size_t address)
    {
        code_.push_back(static_cast<uint8_t>(op::jmp));
        append_uint(address);
    }

    void append_jmpx(intptr_t offset)
    {
        if (offset >= 0) {
            code_.push_back(static_cast<uint8_t>(op::jmpp));
            append_uint(static_cast<size_t>(offset));
        } else {
            code_.push_back(static_cast<uint8_t>(op::jmpn));
            append_uint(static_cast<size_t>(-offset));
        }
    }

    void append_jt(size_t address)
    {
        code_.push_back(static_cast<uint8_t>(op::jt));
        append_uint(address);
    }

    void append_jtx(intptr_t offset)
    {
        if (offset >= 0) {
            code_.push_back(static_cast<uint8_t>(op::jtp));
            append_uint(static_cast<size_t>(offset));
        } else {
            code_.push_back(static_cast<uint8_t>(op::jtn));
            append_uint(static_cast<size_t>(-offset));
        }
    }

    void append_jf(size_t address)
    {
        code_.push_back(static_cast<uint8_t>(op::jf));
        append_uint(address);
    }

    void append_jfx(intptr_t offset)
    {
        if (offset >= 0) {
            code_.push_back(static_cast<uint8_t>(op::jfp));
            append_uint(static_cast<size_t>(offset));
        } else {
            code_.push_back(static_cast<uint8_t>(op::jfn));
            append_uint(static_cast<size_t>(-offset));
        }
    }

    void append_call(size_t address)
    {
        code_.push_back(static_cast<uint8_t>(op::call));
        append_uint(address);
    }

    void append_callp()
    {
        code_.push_back(static_cast<uint8_t>(op::callp));
    }

    void append_ecall(size_t fnindex)
    {
        if (fnindex < 128) {
            code_.push_back(static_cast<uint8_t>(128 + fnindex));
        } else {
            code_.push_back(static_cast<uint8_t>(op::ecall));
            append_uint(fnindex);
        }
    }

    //void append_ecall(size_t fnindex, size_t argindex)
    //{
    //    code_.push_back(static_cast<uint8_t>(op::ecall1));
    //    append_uint(argindex);
    //    append_uint(fnindex);
    //}

    //void append_fecall(size_t fnindex, intptr_t argoffset)
    //{
    //    if (argoffset >= 0) {
    //        code_.push_back(static_cast<uint8_t>(op::fpecall1));
    //        append_uint(static_cast<size_t>(argoffset));
    //    } else {
    //        code_.push_back(static_cast<uint8_t>(op::fnecall1));
    //        append_uint(static_cast<size_t>(-argoffset));
    //    }
    //    append_uint(fnindex);
    //}

    void append_ret()
    {
        code_.push_back(static_cast<uint8_t>(op::ret));
    }

    void append_push(size_t num)
    {
        code_.push_back(static_cast<uint8_t>(op::push));
        append_uint(num);
    }

    void append_pushc(size_t num)
    {
        code_.push_back(static_cast<uint8_t>(op::pushc));
        append_uint(num);
    }

    void append_pushr(size_t offset)
    {
        code_.push_back(static_cast<uint8_t>(op::pushr));
        append_uint(offset);
    }

    void append_fpush(intptr_t fpos)
    {
        if (fpos >= 0) {
            code_.push_back(static_cast<uint8_t>(op::fppush));
            append_uint(static_cast<size_t>(fpos));
        } else {
            code_.push_back(static_cast<uint8_t>(op::fnpush));
            append_uint(static_cast<size_t>(-fpos));
        }
    }

    void append_pushi(size_t num)
    {
        code_.push_back(static_cast<uint8_t>(op::pushi));
        append_uint(num);
    }

    void append_fpushi(intptr_t fpos)
    {
        if (fpos >= 0) {
            code_.push_back(static_cast<uint8_t>(op::fppushi));
            append_uint(static_cast<size_t>(fpos));
        } else {
            code_.push_back(static_cast<uint8_t>(op::fnpushi));
            append_uint(static_cast<size_t>(-fpos));
        }
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
        if (offset >= 0) {
            code_.push_back(static_cast<uint8_t>(op::truncatefpp));
            append_uint(static_cast<size_t>(offset));
        } else {
            code_.push_back(static_cast<uint8_t>(op::truncatefpn));
            append_uint(static_cast<size_t>(-offset));
        }
    }

    void append_pop(size_t num)
    {
        if (!num) [[unlikely]] {
            code_.push_back(static_cast<uint8_t>(op::noop));
        } else if (num == 1) {
            code_.push_back(static_cast<uint8_t>(op::pop));
        } else {
            code_.push_back(static_cast<uint8_t>(op::popn));
            append_uint(num);
        }
    }

    void append_collapse(size_t num)
    {
        if (!num) [[unlikely]] {
            code_.push_back(static_cast<uint8_t>(op::noop));
        } else {
            code_.push_back(static_cast<uint8_t>(op::collapse));
            append_uint(num);
        }
    }

    void append_set(size_t num)
    {
        code_.push_back(static_cast<uint8_t>(op::set));
        append_uint(num);
    }

    void append_setr(size_t offset)
    {
        code_.push_back(static_cast<uint8_t>(op::setr));
        append_uint(offset);
    }

    void append_fset(intptr_t fpos)
    {
        if (fpos >= 0) {
            code_.push_back(static_cast<uint8_t>(op::fpset));
            append_uint(static_cast<size_t>(fpos));
        } else {
            code_.push_back(static_cast<uint8_t>(op::fnset));
            append_uint(static_cast<size_t>(-fpos));
        }
    }

    template <typename T>
    requires (std::is_convertible_v<std::remove_cvref_t<T>, var_t>)
    size_t add_const(T&& value)
    {
        size_t pos = consts_.size();
        consts_.emplace_back(std::forward<T>(value));
        return pos;
    }

    template <typename T>
    requires (std::is_convertible_v<std::remove_cvref_t<T>, var_t>)
    void set_const(size_t index, T&& value)
    {
        consts_[index].replace(std::move(value));
    }

    template <typename T>
    requires (std::is_convertible_v<std::remove_cvref_t<T>, var_t>)
    size_t push_on_stack(T && value)
    {
        size_t pos = stack_.size();
        stack_.emplace_back(std::forward<T>(value));
        return pos;
    }

    void set_at_stack(size_t pos, var_t value)
    {
        if (stack_.size() <= pos) {
            stack_.resize(pos + 1);
        }
        stack_[pos] = std::move(value);
    }

    size_t push_external_fn(ext_function_t efn, string_view descr = {})
    {
        size_t pos = efns_.size();
        efns_.emplace_back(efn, descr);
        return pos;
    }

    void run(ContextT& ctx, size_t address);
    //void run2(ContextT& ctx, size_t address);

    template <typename FunctorT>
    void traverse(ContextT& ctx, size_t address, FunctorT const& ftor);
};

template <typename ContextT>
struct printer
{
    using vm_t = virtual_stack_machine<ContextT>;
    using stack_type = typename vm_t::stack_type;
    using ext_function_t = typename vm_t::ext_function_t;
    //using ext1_function_t = typename vm_t::ext1_function_t;
    //using call_stack_type = typename vm_t::call_stack_type;
    using var_t = typename vm_t::var_t;
    using op = vm_t::op;

    std::ostream& ss;
    explicit printer(std::ostream & s) : ss{s} {}

    std::ostream& generic_print(size_t address, string_view cmd) const
    {
        return ss << "0x" << std::hex << std::setfill('0') << std::setw(4) << address << " " << cmd;
    }

    inline void operator()(identity_type<op::noop>, ContextT&, size_t address) const
    {
        generic_print(address, "noop"sv) << '\n';
    }

    inline optional<size_t> operator()(identity_type<op::ret>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "ret"sv);
        auto optaddr = ctx.call_stack_back();
        if (!optaddr) {
            ss << " (no address)\n"sv;
        } else {
            ss << " ("sv << std::hex << "0x"sv << *optaddr << ")\n"sv;
        }
        return nullopt;
    }

    inline size_t operator()(identity_type<op::call>, ContextT& ctx, size_t address, size_t call_address, size_t ret_address) const
    {
        generic_print(address, "call"sv) << " 0x" << std::hex << call_address;
        if constexpr (requires{ ctx.describe_address(call_address); }) {
            ss << ' ' << ctx.describe_address(call_address) << '\n';
        } else {
            ss << '\n';
        }
        return ret_address;
    }

    inline size_t operator()(identity_type<op::callp>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "callp"sv);
        if constexpr (requires{ ctx.callp_describe(); }) {
            ss << ' ' << ctx.callp_describe();
        } else {
            ss << ' ' << ctx.stack_back();
        }
        ss << '\n';
        return address + 1;
    }

    inline void operator()(identity_type<op::ecall>, ContextT& ctx, size_t address, size_t fn_index) const
    {
        generic_print(address, "ecall"sv);
        ss << " #"sv << std::dec << fn_index;
        if constexpr (requires{ ctx.ecall_describe(fn_index); }) {
            ss << ' ' << ctx.ecall_describe(fn_index) << '\n';
        } else {
            ss << '\n';
        }
    }

    //void common_ecall1(ContextT& ctx, size_t arg_index, size_t fn_index) const
    //{
    //    ss << " ("sv << ctx.stack_at(arg_index) << ") "sv;
    //    if constexpr (requires{ ctx.ecall1_describe(fn_index); }) {
    //        ss << ctx.ecall1_describe(fn_index);
    //    } else {
    //        ss << '#' << std::dec << fn_index;
    //    }
    //    ss << '\n';
    //}

    //inline void operator()(identity_type<op::ecall1>, ContextT& ctx, size_t address, size_t arg_index, size_t fn_index, ext1_function_t) const
    //{
    //    generic_print(address, "ecall1"sv);
    //    common_ecall1(ctx, arg_index, fn_index);
    //}

    //inline void operator()(identity_type<op::fpecall1>, ContextT& ctx, size_t address, size_t arg_offset, size_t fn_index, ext1_function_t) const
    //{
    //    generic_print(address, "fpecall1"sv);
    //    common_ecall1(ctx, ctx.frame_stack_back() + arg_offset, fn_index);
    //}

    //inline void operator()(identity_type<op::fnecall1>, ContextT& ctx, size_t address, size_t arg_offset, size_t fn_index, ext1_function_t) const
    //{
    //    generic_print(address, "fnecall1"sv);
    //    common_ecall1(ctx, ctx.frame_stack_back() - arg_offset, fn_index);
    //}

    inline size_t operator()(identity_type<op::jmp>, ContextT&, size_t address, size_t jmp_address, size_t next_address) const
    {
        generic_print(address, "jmp"sv) << " 0x"sv << std::hex << jmp_address << "\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jmpp>, ContextT&, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jmpp"sv) << " 0x"sv << std::hex << (next_address + jmp_offset) << "\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jmpn>, ContextT&, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jmpn"sv) << " 0x"sv << std::hex << (next_address - jmp_offset) << "\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jt>, ContextT& ctx, size_t address, size_t jmp_address, size_t next_address) const
    {
        generic_print(address, "jt"sv) << " 0x"sv << std::hex << jmp_address << " ("sv << std::boolalpha << ctx.is_true(ctx.stack_back()) << ")\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jtp>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jtp"sv) << " 0x"sv << std::hex << (next_address + jmp_offset) << " ("sv << std::boolalpha << ctx.is_true(ctx.stack_back()) << ")\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jtn>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jtn"sv) << " 0x"sv << std::hex << (next_address - jmp_offset) << " ("sv << std::boolalpha << ctx.is_true(ctx.stack_back()) << ")\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jf>, ContextT& ctx, size_t address, size_t jmp_address, size_t next_address) const
    {
        generic_print(address, "jf"sv) << " 0x"sv << std::hex << jmp_address << " ("sv << std::boolalpha << ctx.is_true(ctx.stack_back()) << ")\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jfp>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jfp"sv) << " 0x"sv << std::hex << (next_address + jmp_offset) << " ("sv << std::boolalpha << ctx.is_true(ctx.stack_back()) << ")\n";
        return next_address;
    }

    inline size_t operator()(identity_type<op::jfn>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jfn"sv) << " 0x"sv << std::hex << (next_address - jmp_offset) << " ("sv << std::boolalpha << ctx.is_true(ctx.stack_back()) << ")\n";
        return next_address;
    }

    inline void operator()(identity_type<op::pop>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "pop"sv) << " ["sv << std::dec << (ctx.stack_size() - 1) << "] ("sv << ctx.stack_back() << ")\n"sv;
    }

    inline void operator()(identity_type<op::popn>, ContextT& ctx, size_t address, size_t n) const
    {
        generic_print(address, "popn"sv) << " ["sv << std::dec  << (ctx.stack_size() - n) << " - "sv << (ctx.stack_size() - 1) << "] ("sv;
        auto sp = ctx.stack_span(0, n);
        if (!sp.empty()) {
            ss << sp.front();
            sp = sp.subspan(1);
        }
        while (!sp.empty()) {
            ss << ", "sv;
            ss << sp.front();
            sp = sp.subspan(1);
        }
        ss << ")\n"sv;
    }

    inline void operator()(identity_type<op::collapse>, ContextT& ctx, size_t address, size_t n) const
    {
        generic_print(address, "collapse"sv) << ' ' << n << " ("sv;
        auto sp = ctx.stack_span(1, n);
        if (!sp.empty()) {
            ss << sp.front();
            sp = sp.subspan(1);
        }
        while (!sp.empty()) {
            ss << ", "sv;
            ss << sp.front();
            sp = sp.subspan(1);
        }
        ss << ")\n"sv;
    }

    inline void operator()(identity_type<op::push>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "push"sv) << " ["sv << std::dec << index << "]->["sv << ctx.stack_size() << "] ("sv << ctx.stack_at(index) << ")\n"sv;
    }

    inline void operator()(identity_type<op::pushr>, ContextT& ctx, size_t address, size_t offset) const
    {
        generic_print(address, "pushr"sv) << " ["sv << std::dec << (ctx.stack_size() - offset - 1) << "]->["sv << ctx.stack_size() << "] ("sv << ctx.stack_back(offset) << ")\n"sv;
    }

    inline void operator()(identity_type<op::fppush>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() + offset;
        generic_print(address, "fppush"sv) << " ["sv << std::dec << index << "]->["sv << ctx.stack_size() << "] ("sv << ctx.stack_at(index) << ")\n"sv;
    }

    inline void operator()(identity_type<op::fnpush>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() - offset;
        generic_print(address, "fnpush"sv) << " ["sv << std::dec << index << "]->["sv << ctx.stack_size() << "] ("sv << ctx.stack_at(index) << ")\n"sv;
    }


    inline void operator()(identity_type<op::pushi>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "pushi"sv) << ' ' << std::dec << index << "->[" << ctx.stack_size() << "] \n"sv;
    }

    inline void operator()(identity_type<op::fppushi>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() + offset;
        generic_print(address, "fppushi"sv) << ' ' << std::dec << index << "->[" << ctx.stack_size() << "] \n"sv;
    }

    inline void operator()(identity_type<op::fnpushi>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() - offset;
        generic_print(address, "fnpushi"sv) << ' ' << std::dec << index << "->[" << ctx.stack_size() << "] \n"sv;
    }

    inline void operator()(identity_type<op::pushc>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "pushc"sv) << " C["sv << std::dec << index << "]->["sv << ctx.stack_size() << "] ("sv << ctx.const_at(index) << ")\n"sv;
    }

    inline void operator()(identity_type<op::set>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "set"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << index << "] (" << ctx.stack_back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::setr>, ContextT& ctx, size_t address, size_t offset) const
    {
        generic_print(address, "setr"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << (ctx.stack_size() - offset - 1) << "] (" << ctx.stack_back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::fpset>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() + offset;
        generic_print(address, "fpset"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << index << "] (" << ctx.stack_back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::fnset>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() - offset;
        generic_print(address, "fnset"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << index << "] (" << ctx.stack_back() << ")" << "\n";
    }

    inline void operator()(identity_type<op::pushfp>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "pushfp"sv) << ", fp: "sv << std::dec << static_cast<uint32_t>(ctx.stack_size()) << "\n";
    }

    inline void operator()(identity_type<op::popfp>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "popfp"sv) << "\n";
    }

    inline void operator()(identity_type<op::truncatefpp>, ContextT& ctx, size_t address, size_t cnt) const
    {
        generic_print(address, "truncatefpp"sv) << " " << std::dec << (ctx.frame_stack_back() + cnt) << '\n';
    }
    inline void operator()(identity_type<op::truncatefpn>, ContextT& ctx, size_t address, size_t cnt) const
    {
        size_t min_idx = ctx.frame_stack_back() - cnt;
        //generic_print(address, "truncatefpn"sv) << " " << std::dec << min_idx << '\n';
        generic_print(address, "truncatefpn"sv) << " ["sv << std::dec << min_idx << " - "sv << (ctx.stack_size() - 1) << "] ("sv;
        auto sp = ctx.stack_span(0, ctx.stack_size() - min_idx);
        if (!sp.empty()) {
            ss << sp.front();
            sp = sp.subspan(1);
        }
        while (!sp.empty()) {
            ss << ", "sv;
            ss << sp.front();
            sp = sp.subspan(1);
        }
        ss << ")\n"sv;
    }
};

template <typename ContextT>
struct runner
{
    using vm_t = virtual_stack_machine<ContextT>;
    using stack_type = typename vm_t::stack_type;
    using ext_function_t = typename vm_t::ext_function_t;
    //using ext1_function_t = typename vm_t::ext1_function_t;
    using var_t = typename vm_t::var_t;
    using op = vm_t::op;
    
    inline void operator()(identity_type<op::noop>, ContextT &, size_t) const {}

    inline size_t operator()(identity_type<op::jmp>, ContextT&, size_t address, size_t jmp_address, size_t) const { return jmp_address; }
    inline size_t operator()(identity_type<op::jmpp>, ContextT&, size_t address, size_t jmp_offset, size_t next) const { return next + jmp_offset; }
    inline size_t operator()(identity_type<op::jmpn>, ContextT&, size_t address, size_t jmp_offset, size_t next) const { return next - jmp_offset; }

    inline size_t operator()(identity_type<op::jt>, ContextT& ctx, size_t address, size_t jmp_address, size_t next_address) const
    {
        return ctx.is_true(ctx.stack_back()) ? jmp_address : next_address;
    }

    inline size_t operator()(identity_type<op::jtp>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        return ctx.is_true(ctx.stack_back()) ? (next_address + jmp_offset) : next_address;
    }

    inline size_t operator()(identity_type<op::jtn>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        return ctx.is_true(ctx.stack_back()) ? (next_address - jmp_offset) : next_address;
    }

    inline size_t operator()(identity_type<op::jf>, ContextT& ctx, size_t address, size_t jmp_address, size_t next_address) const
    {
        return ctx.is_true(ctx.stack_back()) ? next_address : jmp_address;
    }

    inline size_t operator()(identity_type<op::jfp>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        return ctx.is_true(ctx.stack_back()) ? next_address : (next_address + jmp_offset);
    }

    inline size_t operator()(identity_type<op::jfn>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        return ctx.is_true(ctx.stack_back()) ? next_address : (next_address - jmp_offset);
    }

    inline optional<size_t> operator()(identity_type<op::ret>, ContextT& ctx, size_t) const
    {
        return ctx.call_stack_pop();
    }

    inline size_t operator()(identity_type<op::call>, ContextT& ctx, size_t address, size_t call_address, size_t ret_address) const
    {
        ctx.call_stack_push(ret_address);
        return call_address;
    }

    inline void operator()(identity_type<op::ecall>, ContextT& ctx, size_t address, size_t fn_index) const { ctx.efn(fn_index); }

    //inline void operator()(identity_type<op::ecall1>, ContextT& ctx, size_t address, size_t arg_index, size_t fn_index, ext1_function_t fn) const { fn(ctx, ctx.stack_at(arg_index)); }
    //
    //inline void operator()(identity_type<op::fpecall1>, ContextT& ctx, size_t address, size_t arg_offset, size_t fn_index, ext1_function_t fn) const { fn(ctx, ctx.stack_at(ctx.frame_stack_back() + arg_offset)); }
    //
    //inline void operator()(identity_type<op::fnecall1>, ContextT& ctx, size_t address, size_t arg_offset, size_t fn_index, ext1_function_t fn) const { fn(ctx, ctx.stack_at(ctx.frame_stack_back() - arg_offset)); }
    
    inline void operator()(identity_type<op::pop>, ContextT& ctx, size_t address) const { ctx.stack_pop(); }

    inline void operator()(identity_type<op::popn>, ContextT& ctx, size_t address, size_t n) const { ctx.stack_pop(n); }
    
    inline void operator()(identity_type<op::collapse>, ContextT& ctx, size_t address, size_t n) const { ctx.stack_collapse(n); }

    inline void operator()(identity_type<op::push>, ContextT& ctx, size_t address, size_t index) const
    {
        var_t val = ctx.stack_at(index);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::pushc>, ContextT& ctx, size_t address, size_t index) const
    {
        var_t val = ctx.const_at(index);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::pushr>, ContextT& ctx, size_t address, size_t offset) const
    {
        var_t val = ctx.stack_back(offset);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::fppush>, ContextT& ctx, size_t address, size_t offset) const
    {
        var_t val = ctx.stack_at(ctx.frame_stack_back() + offset);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::fnpush>, ContextT& ctx, size_t address, size_t offset) const
    {
        var_t val = ctx.stack_at(ctx.frame_stack_back() - offset);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::pushi>, ContextT& ctx, size_t address, size_t index) const
    {
        ctx.stack_push(ctx.value_of(index));
    }

    inline void operator()(identity_type<op::fppushi>, ContextT& ctx, size_t address, size_t offset) const
    {
        ctx.stack_push(ctx.value_of(ctx.frame_stack_back() + offset));
    }

    inline void operator()(identity_type<op::fnpushi>, ContextT& ctx, size_t address, size_t offset) const
    {
        ctx.stack_push(ctx.value_of(ctx.frame_stack_back() - offset));
    }

    inline void operator()(identity_type<op::set>, ContextT& ctx, size_t address, size_t index) const
    {
        ctx.stack_at(index) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::setr>, ContextT& ctx, size_t address, size_t offset) const
    {
        ctx.stack_back(offset) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::fpset>, ContextT& ctx, size_t address, size_t offset) const
    {
        ctx.stack_at(ctx.frame_stack_back() + offset) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::fnset>, ContextT& ctx, size_t address, size_t offset) const
    {
        ctx.stack_at(ctx.frame_stack_back() - offset) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::pushfp>, ContextT& ctx, size_t address) const { ctx.frame_stack_push(); }
    inline void operator()(identity_type<op::popfp>, ContextT& ctx, size_t address) const { ctx.frame_stack_pop(); }
    
    inline void operator()(identity_type<op::truncatefpp>, ContextT& ctx, size_t address, size_t cnt) const
    {
        ctx.stack_truncate(ctx.frame_stack_back() + cnt);
    }
    inline void operator()(identity_type<op::truncatefpn>, ContextT& ctx, size_t address, size_t cnt) const
    {
        ctx.stack_truncate(ctx.frame_stack_back() - cnt);
    }

    inline size_t operator()(identity_type<op::callp>, ContextT& ctx, size_t address) const
    {
        return ctx.callp(address + 1);
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
        if (op_byte >= 128) {
            auto cmdoffset = op_byte - 128;
            if (cmdoffset >= efns_.size()) [[unlikely]] {
                throw internal_error("wrong command");
            }
            ftor(identity<op::ecall>, ctx, address, cmdoffset);
            ++address;
            continue;
        }
        switch (static_cast<op>(op_byte)) {
        case op::noop:
            ftor(identity<op::noop>, ctx, address);
            continue;
        case op::jmp:
            {
                size_t start_address = address++;
                size_t jump_address = read_uint(address);
                address = ftor(identity<op::jmp>, ctx, start_address, jump_address, address);
                continue;
            }
        case op::jt:
            {
                size_t start_address = address++;
                size_t jump_address = read_uint(address);
                address = ftor(identity<op::jt>, ctx, start_address, jump_address, address);
                continue;
            }
        case op::jf:
            {
                size_t start_address = address++;
                size_t jump_address = read_uint(address);
                address = ftor(identity<op::jf>, ctx, start_address, jump_address, address);
                continue;
            }
        case op::jmpp:
            {
                size_t start_address = address++;
                size_t jump_offset = read_uint(address);
                address = ftor(identity<op::jmpp>, ctx, start_address, jump_offset, address);
                continue;
            }
        case op::jmpn:
            {
                size_t start_address = address++;
                size_t jump_offset = read_uint(address);
                address = ftor(identity<op::jmpn>, ctx, start_address, jump_offset, address);
                continue;
            }
        case op::jtp:
            {
                size_t start_address = address++;
                size_t jump_offset = read_uint(address);
                address = ftor(identity<op::jtp>, ctx, start_address, jump_offset, address);
                continue;
            }
        case op::jtn:
            {
                size_t start_address = address++;
                size_t jump_offset = read_uint(address);
                address = ftor(identity<op::jtn>, ctx, start_address, jump_offset, address);
                continue;
            }
        case op::jfp:
            {
                size_t start_address = address++;
                size_t jump_offset = read_uint(address);
                address = ftor(identity<op::jfp>, ctx, start_address, jump_offset, address);
                continue;
            }
        case op::jfn:
            {
                size_t start_address = address++;
                size_t jump_offset = read_uint(address);
                address = ftor(identity<op::jfn>, ctx, start_address, jump_offset, address);
                continue;
            }
        case op::call:
            {
                size_t start_address = address++;
                size_t call_address = read_uint(address);
                address = ftor(identity<op::call>, ctx, start_address, call_address, address);
                continue;
            }
        case op::callp:
            address = ftor(identity<op::callp>, ctx, address);
            continue;
        case op::ecall:
            {
                size_t start_address = address++;
                size_t fn_index = read_uint(address);
                ftor(identity<op::ecall>, ctx, address, fn_index);
                continue;
            }
        //case op::ecall1:
        //    {
        //        size_t start_address = address++;
        //        size_t arg_index = read_uint(address);
        //        size_t fn_index = read_uint(address);
        //        ftor(identity<op::ecall1>, ctx, start_address, arg_index, fn_index, e1fns_.at(fn_index));
        //        continue;
        //    }
        //case op::fnecall1:
        //    {
        //        size_t start_address = address++;
        //        size_t arg_offset = read_uint(address);
        //        size_t fn_index = read_uint(address);
        //        ftor(identity<op::fnecall1>, ctx, start_address, arg_offset, fn_index, e1fns_.at(fn_index));
        //        continue;
        //    }
        //case op::fpecall1:
        //    {
        //        size_t start_address = address++;
        //        size_t arg_offset = read_uint(address);
        //        size_t fn_index = read_uint(address);
        //        ftor(identity<op::fpecall1>, ctx, start_address, arg_offset, fn_index, e1fns_.at(fn_index));
        //        continue;
        //    }
        case op::ret:
            if (auto optaddress = ftor(identity<op::ret>, ctx, address); optaddress) {
                address = *optaddress;
                continue;
            }
            return;
        
        case op::pop:
            ftor(identity<op::pop>, ctx, address);
            ++address;
            continue;
        case op::popn:
            {
                size_t start_address = address++;
                size_t num = read_uint(address);
                ftor(identity<op::popn>, ctx, start_address, num);
                continue;
            }
        case op::collapse:
            {
                size_t start_address = address++;
                size_t num = read_uint(address);
                ftor(identity<op::collapse>, ctx, start_address, num);
                continue;
            }
        case op::push:
            {
                size_t start_address = address++;
                size_t index = read_uint(address);
                ftor(identity<op::push>, ctx, start_address, index);
                continue;
            }
        case op::pushr:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::pushr>, ctx, start_address, offset);
                continue;
            }
        case op::fppush:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::fppush>, ctx, start_address, offset);
                continue;
            }
        case op::fnpush:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::fnpush>, ctx, start_address, offset);
                continue;
            }
        
        case op::pushi:
            {
                size_t start_address = address++;
                size_t index = read_uint(address);
                ftor(identity<op::pushi>, ctx, start_address, index);
                continue;
            }
        case op::fppushi:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::fppushi>, ctx, start_address, offset);
                continue;
            }
        case op::fnpushi:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::fnpushi>, ctx, start_address, offset);
                continue;
            }
        case op::pushc:
            {
                size_t start_address = address++;
                size_t index = read_uint(address);
                ftor(identity<op::pushc>, ctx, start_address, index);
                continue;
            }
        case op::set:
            {
                size_t start_address = address++;
                size_t index = read_uint(address);
                ftor(identity<op::set>, ctx, start_address, index);
                continue;
            }
        case op::setr:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::setr>, ctx, start_address, offset);
                continue;
            }
        case op::fpset:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::fpset>, ctx, start_address, offset);
                continue;
            }
        case op::fnset:
            {
                size_t start_address = address++;
                size_t offset = read_uint(address);
                ftor(identity<op::fnset>, ctx, start_address, offset);
                continue;
            }
        
        case op::pushfp:
            ftor(identity<op::pushfp>, ctx, address);
            ++address;
            continue;
        case op::popfp:
            ftor(identity<op::popfp>, ctx, address);
            ++address;
            continue;
        case op::truncatefpp:
            {
                size_t start_address = address++;
                size_t count = read_uint(address);
                ftor(identity<op::truncatefpp>, ctx, start_address, count);
                continue;
            }
        case op::truncatefpn:
            {
                size_t start_address = address++;
                size_t count = read_uint(address);
                ftor(identity<op::truncatefpn>, ctx, start_address, count);
                continue;
            }
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

}
