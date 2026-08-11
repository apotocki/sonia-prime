//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/config.hpp"

#include <vector>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include "sonia/span.hpp"
#include "sonia/function.hpp"
#include "sonia/logger/logger.hpp"
#include "vmop_proto.hpp"

#include <iostream>

namespace sonia::vm {

/*
class context
{
public:
    using variable_type = unspecified
    bool is_zero(variable_type const&) const; // for conditional jumps
    bool is_positive(variable_type const&) const; // for conditional jumps
    bool is_negative(variable_type const&) const; // for conditional jumps
    void cmp();
    variable_type value_of(size_t val) const; // for pushi

    variable_type const& static_at(size_t index) const;

    optional<size_t> call_stack_pop();
    void call_stack_push(size_t addr);
    void stack_pop();
}
*/

template <auto v> struct identity_type {};
template <auto v> inline constexpr identity_type<v> identity{};

template <typename VarT>
class basic_context
{
public:
    using variable_type = VarT;
    using stack_type = std::vector<variable_type>;

    inline variable_type const& stack_back(size_t i = 0) const
    {
        size_t ssz = stack_size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return stack()[ssz - 1 - i];
    }

    inline variable_type& stack_back(size_t i = 0)
    {
        size_t ssz = stack_size();
        if (ssz <= i) [[unlikely]] {
            THROW_INTERNAL_ERROR("wrong stack index");
        }
        return stack()[ssz - 1 - i];
    }

    inline void set_at_stack(size_t pos, variable_type value)
    {
        if (stack_.size() <= pos) {
            stack_.resize(pos + 1);
        }
        stack_[pos] = std::move(value);
    }

    template <typename T>
    requires (std::is_convertible_v<std::remove_cvref_t<T>, variable_type>)
    inline size_t push_on_stack(T&& value)
    {
        size_t pos = stack_.size();
        stack_.emplace_back(std::forward<T>(value));
        return pos;
    }

    inline void stack_pop(size_t n = 1)
    {
        size_t ssz = stack_size();
        stack().resize(ssz - n);
    }

    inline void stack_truncate(size_t sz)
    {
        stack().resize(sz);
    }

    inline stack_type const& stack() const noexcept { return stack_; }
    inline stack_type& stack() noexcept { return stack_; }
    inline size_t stack_size() const noexcept { return stack_.size(); }

protected:
    stack_type stack_;
};

// NOTE: hoisted out of virtual_stack_machine<ContextT> (it never depended on ContextT) so it
// is a plain namespace-scope enum that an external tool (sonia-pygen) can annotate and walk
// directly. Each enumerator below carries a VMOP_* tag (see vmop_proto.hpp) describing its
// bytecode-decode shape; once the sonia-pygen vm-dispatch generator exists, those tags drive
// generation of virtual_stack_machine::traverse()'s dispatch body (switch form for MSVC,
// computed-goto form for GCC/Clang), replacing the hand-written switch below. Until then the
// tags are inert metadata and traverse() is written by hand, same as sonia/utility/lang/vm.hpp.
DEFINE_VMOP_PROTO(op, uint8_t,
    noop VMOP_SIMPLE = 0,
    jmp  VMOP_ARG VMOP_JUMP = 1, jmpp VMOP_ARG VMOP_JUMP = 2, jmpn VMOP_ARG VMOP_JUMP = 3,
    //jt = 4, jtp = 5, jtn = 6,
    //jf = 7, jfp = 8, jfn = 9,
    jg  VMOP_ARG VMOP_JUMP = 10, jgp  VMOP_ARG VMOP_JUMP = 11, jgn  VMOP_ARG VMOP_JUMP = 12, // jump if greater (positive value)
    jge VMOP_ARG VMOP_JUMP = 13, jgep VMOP_ARG VMOP_JUMP = 14, jgen VMOP_ARG VMOP_JUMP = 15, // jump if greater or equal (non-negative value)
    jl  VMOP_ARG VMOP_JUMP = 16, jlp  VMOP_ARG VMOP_JUMP = 17, jln  VMOP_ARG VMOP_JUMP = 18, // jump if less (negative value)
    jle VMOP_ARG VMOP_JUMP = 19, jlep VMOP_ARG VMOP_JUMP = 20, jlen VMOP_ARG VMOP_JUMP = 21, // jump if less or equal (non-positive value)
    je  VMOP_ARG VMOP_JUMP = 22, jep  VMOP_ARG VMOP_JUMP = 23, jen  VMOP_ARG VMOP_JUMP = 24, // jump if equal (zero)
    jne VMOP_ARG VMOP_JUMP = 25, jnep VMOP_ARG VMOP_JUMP = 26, jnen VMOP_ARG VMOP_JUMP = 27, // jump if not equal (non-zero)
    cmp VMOP_SIMPLE = 28, // compare stack_top and stack_top-1, replaces stack[top] with the sgn(stack[top-1] - stack[top])

    // function call/return
    call  VMOP_CUSTOM = 29,  // call(address) -> call address
    callp VMOP_JUMP = 30, // call() -> call stack[stack_back_pos()]
    ecall VMOP_CUSTOM = 31, // ecall(index) -> call efn[index]
    //ecall1 = 7, // call(fn_index), call(param index, fn_index)
    //fpecall1 = 8, fnecall1 = 9, //call(param +-offset, fn_index)
    ret VMOP_CUSTOM = 32,

    // data move
    push  VMOP_ARG = 33, // push on stack stack[uint]
    pushr VMOP_ARG = 34, // push on stack stack[stack_back_pos() - uint] // r aka relative
    fppush VMOP_ARG = 35, fnpush VMOP_ARG = 36, // push on stack stack[fp +- uint]
    pushi VMOP_ARG = 37, fppushi VMOP_ARG = 38, fnpushi VMOP_ARG = 39, // push index on stack // positive/negative index
    pushc VMOP_ARG = 40, // push const on stack: consts_[uint] -> on stack
    push_stsz VMOP_SIMPLE = 41, // push stack size
    dup VMOP_SIMPLE = 42, // duplicate stack top value
    set   VMOP_ARG = 43, // set stack[uint] = stack_top_value
    setr  VMOP_ARG = 44, // set stack[stack_back_pos() - uint] = stack_top_value // r aka relative
    fpset VMOP_ARG = 45, fnset VMOP_ARG = 46, // set stack[fp +- uint] = stack_top_value
    pindexs VMOP_ARG = 47, nindexs VMOP_ARG = 48, // replace stack top with stack[stack_top + shift]
    indexc VMOP_SIMPLE = 49, // replace stack top with consts[stack_top]
    pop VMOP_SIMPLE, popn VMOP_ARG, // pop COUNT:uint, pop0 === pop 1
    collapse VMOP_ARG, // pop COUNT before the back
    // frame data pointer
    pushfp VMOP_SIMPLE, popfp VMOP_SIMPLE, truncatefpp VMOP_ARG, truncatefpn VMOP_ARG
);

template <typename ContextT>
class virtual_stack_machine
{
public:
    using var_t = typename ContextT::variable_type;

    using ext_function_t = void(*)(ContextT&);

    using op = sonia::vm::op;

protected:
    std::vector<var_t> consts_;
    //stack_type stack_;

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

    // append_jt/append_jtx/append_jf/append_jfx reference op::jt/jtp/jtn/jf/jfp/jfn, which are
    // commented out of the `op` enum above (never had a traverse() dispatch case either, in
    // this file or vm.hpp). Their only real call sites (compiler_visitor.hpp, conditional_t /
    // not_empty_condition_t handling) are themselves inside `#if 0` blocks behind a
    // THROW_NOT_IMPLEMENTED_ERROR - so these four were always dead, not just currently unused.
    // MSVC doesn't eagerly check non-dependent names in an uninstantiated class template's
    // member bodies, so it silently tolerated the dangling op::jt* / op::jf* references; Clang
    // (via sonia-pygen's AST parse of this file) does check them and fails outright. Commented
    // out rather than left for MSVC's leniency to keep masking it - see BUGFIXES.md.
    //void append_jt(size_t address)
    //{
    //    code_.push_back(static_cast<uint8_t>(op::jt));
    //    append_uint(address);
    //}
    //
    //void append_jtx(intptr_t offset)
    //{
    //    if (offset >= 0) {
    //        code_.push_back(static_cast<uint8_t>(op::jtp));
    //        append_uint(static_cast<size_t>(offset));
    //    } else {
    //        code_.push_back(static_cast<uint8_t>(op::jtn));
    //        append_uint(static_cast<size_t>(-offset));
    //    }
    //}
    //
    //void append_jf(size_t address)
    //{
    //    code_.push_back(static_cast<uint8_t>(op::jf));
    //    append_uint(address);
    //}
    //
    //void append_jfx(intptr_t offset)
    //{
    //    if (offset >= 0) {
    //        code_.push_back(static_cast<uint8_t>(op::jfp));
    //        append_uint(static_cast<size_t>(offset));
    //    } else {
    //        code_.push_back(static_cast<uint8_t>(op::jfn));
    //        append_uint(static_cast<size_t>(-offset));
    //    }
    //}

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

    void append_push_stsz()
    {
        code_.push_back(static_cast<uint8_t>(op::push_stsz));
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
    size_t push_const(T&& value)
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
    using stack_type = typename ContextT::stack_type;
    using ext_function_t = typename vm_t::ext_function_t;
    //using ext1_function_t = typename vm_t::ext1_function_t;
    //using call_stack_type = typename vm_t::call_stack_type;
    using var_t = typename vm_t::var_t;

    std::ostream& ss;
    function<void()> line_commiter_;

    inline explicit printer(std::ostream & s, function<void()> const& lc = {}) noexcept : ss{ s }, line_commiter_{ lc } {}

    void commit_line() const
    {
        if (line_commiter_) {
            line_commiter_();
        }
    }

    std::ostream& generic_print(size_t address, string_view cmd) const
    {
        return ss << "0x"sv << std::hex << std::noshowbase << std::setfill('0') << std::setw(4) << address << ' ' << cmd;
    }

    inline void operator()(identity_type<op::noop>, ContextT&, size_t address) const
    {
        generic_print(address, "noop"sv) << '\n';
        commit_line();
    }

    inline optional<size_t> operator()(identity_type<op::ret>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "ret"sv);
        auto optaddr = ctx.call_stack_back();
        if (!optaddr) {
            ss << "\t; no address\n"sv;
        } else {
            ss << "\t; 0x"sv << std::hex << std::uppercase << *optaddr << '\n';
        }
        commit_line();
        return nullopt;
    }

    inline size_t operator()(identity_type<op::call>, ContextT& ctx, size_t address, size_t call_address, size_t ret_address) const
    {
        generic_print(address, "call 0x"sv) << std::hex << std::uppercase << call_address;
        if constexpr (requires{ ctx.describe_address(call_address); }) {
            ss << "\t; "sv << ctx.describe_address(call_address) << '\n';
        } else {
            ss << '\n';
        }
        commit_line();
        return ret_address;
    }

    inline size_t operator()(identity_type<op::callp>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "callp "sv);
        if constexpr (requires{ ctx.callp_describe(); }) {
            ss << ctx.callp_describe();
        } else {
            ss << "0x"sv << std::hex << std::uppercase << ctx.stack_back().template as<size_t>();
        }
        ss << '\n';
        commit_line();
        return address + 1;
    }

    inline void operator()(identity_type<op::ecall>, ContextT& ctx, size_t address, size_t fn_index) const
    {
        generic_print(address, "ecall #"sv) << std::dec << fn_index;
        if constexpr (requires{ ctx.ecall_describe(fn_index); }) {
            ss << "\t; "sv << ctx.ecall_describe(fn_index) << '\n';
        } else {
            ss << '\n';
        }
        commit_line();
    }

    inline size_t operator()(identity_type<op::jmp>, ContextT&, size_t address, size_t jmp_address, size_t next_address) const
    {
        generic_print(address, "jmp 0x"sv) << std::hex << std::uppercase << jmp_address << '\n';
        commit_line();
        return next_address;
    }

    inline size_t operator()(identity_type<op::jmpp>, ContextT&, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jmpp 0x"sv) << std::hex << std::uppercase << (address + jmp_offset) << '\n';
        commit_line();
        return next_address;
    }

    inline size_t operator()(identity_type<op::jmpn>, ContextT&, size_t address, size_t jmp_offset, size_t next_address) const
    {
        generic_print(address, "jmpn 0x"sv) << std::hex << std::uppercase << (address - jmp_offset) << '\n';
        commit_line();
        return next_address;
    }

#define SONIA_VM_PRINTER_JUMP_OPS(opname, cond) \
    inline size_t operator()(identity_type<op::opname>, ContextT& ctx, size_t address, size_t jmp_address, size_t next_address) const \
    {\
        generic_print(address, #opname##sv) << " 0x"sv << std::hex << std::uppercase << jmp_address << "\t; "sv << std::boolalpha << cond(ctx.stack_back()) << '\n'; \
        commit_line(); \
        return next_address; \
    }\
    inline size_t operator()(identity_type<op::opname##p>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const \
    {\
        generic_print(address, BOOST_STRINGIZE(BOOST_PP_CAT(opname, p))) << " 0x"sv << std::hex << std::uppercase << (address + jmp_offset) << "\t; "sv << std::boolalpha << cond(ctx.stack_back()) << '\n'; \
        commit_line(); \
        return next_address; \
    }\
    inline size_t operator()(identity_type<op::opname##n>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const \
    {\
        generic_print(address, BOOST_STRINGIZE(BOOST_PP_CAT(opname, n))) << " 0x"sv << std::hex << std::uppercase << (address - jmp_offset) << "\t; "sv << std::boolalpha << cond(ctx.stack_back()) << '\n'; \
        commit_line(); \
        return next_address; \
    }

    SONIA_VM_PRINTER_JUMP_OPS(jne, !ctx.is_zero)
    SONIA_VM_PRINTER_JUMP_OPS(je, ctx.is_zero)
    SONIA_VM_PRINTER_JUMP_OPS(jg, ctx.is_positive)
    SONIA_VM_PRINTER_JUMP_OPS(jge, !ctx.is_negative)
    SONIA_VM_PRINTER_JUMP_OPS(jl, ctx.is_negative)
    SONIA_VM_PRINTER_JUMP_OPS(jle, !ctx.is_positive)
    #undef SONIA_VM_PRINTER_JUMP_OPS

    inline void operator()(identity_type<op::cmp>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "cmp"sv) << " ("sv << ctx.stack_back(1) << " - " << ctx.stack_back() << ")\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::pop>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "pop"sv) << " ["sv << std::dec << (ctx.stack_size() - 1) << "]\t; "sv << ctx.stack_back() << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::popn>, ContextT& ctx, size_t address, size_t n) const
    {
        generic_print(address, "popn"sv) << " ["sv << std::dec  << (ctx.stack_size() - n) << " - "sv << (ctx.stack_size() - 1) << "]\t; ["sv;
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
        ss << "]\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::collapse>, ContextT& ctx, size_t address, size_t n) const
    {
        generic_print(address, "collapse"sv) << ' ' << n << "\t; ["sv;
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
        ss << "]\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::push>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "push"sv) << " ["sv << std::dec << index << "]->["sv << ctx.stack_size() << "]\t; "sv << ctx.stack_at(index) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::pushr>, ContextT& ctx, size_t address, size_t offset) const
    {
        generic_print(address, "pushr"sv) << " ["sv << std::dec << (ctx.stack_size() - offset - 1) << "]->["sv << ctx.stack_size() << "]\t; "sv << ctx.stack_back(offset) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::fppush>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() + offset;
        generic_print(address, "fppush"sv) << " ["sv << std::dec << index << "]->["sv << ctx.stack_size() << "]\t; "sv << ctx.stack_at(index) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::fnpush>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() - offset;
        generic_print(address, "fnpush"sv) << " ["sv << std::dec << index << "]->["sv << ctx.stack_size() << "]\t; "sv << ctx.stack_at(index) << '\n';
        commit_line();
    }


    inline void operator()(identity_type<op::pushi>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "pushi"sv) << ' ' << std::dec << index << "->[" << ctx.stack_size() << "]\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::fppushi>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() + offset;
        generic_print(address, "fppushi"sv) << ' ' << std::dec << index << "->[" << ctx.stack_size() << "]\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::fnpushi>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() - offset;
        generic_print(address, "fnpushi"sv) << ' ' << std::dec << index << "->[" << ctx.stack_size() << "]\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::pushc>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "pushc"sv) << " C["sv << std::dec << index << "]->["sv << ctx.stack_size() << "]\t; "sv << ctx.const_at(index) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::push_stsz>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "push_stsz"sv) << "\t; "sv << std::dec << ctx.stack_size() << " -> ["sv << ctx.stack_size() << "]\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::dup>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "dup"sv) << "\t; "sv << ctx.stack_back() << " -> ["sv << std::dec << ctx.stack_size() << "]\n"sv;
        commit_line();
    }

    inline void operator()(identity_type<op::set>, ContextT& ctx, size_t address, size_t index) const
    {
        generic_print(address, "set"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << index << "]\t; " << ctx.stack_back() << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::setr>, ContextT& ctx, size_t address, size_t offset) const
    {
        generic_print(address, "setr"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << (ctx.stack_size() - offset - 1) << "]\t; " << ctx.stack_back() << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::fpset>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() + offset;
        generic_print(address, "fpset"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << index << "]\t; " << ctx.stack_back() << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::fnset>, ContextT& ctx, size_t address, size_t offset) const
    {
        size_t index = ctx.frame_stack_back() - offset;
        generic_print(address, "fnset"sv) << " [" << std::dec << (ctx.stack_size() - 1) << "]->[" << index << "]\t; " << ctx.stack_back() << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::pindexs>, ContextT& ctx, size_t address, size_t shift) const
    {
        uint64_t index = ctx.stack_back().template as<size_t>() + shift;
        generic_print(address, "indexs "sv) << shift << "\t; stack["sv << std::dec << index << "] = 0x"sv << std::hex << std::uppercase << ctx.stack_at(index) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::nindexs>, ContextT& ctx, size_t address, size_t shift) const
    {
        uint64_t index = ctx.stack_back().template as<size_t>() - shift;
        generic_print(address, "indexs -"sv) << shift << "\t; stack["sv << std::dec << index << "] = 0x"sv << std::hex << std::uppercase << ctx.stack_at(index) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::indexc>, ContextT& ctx, size_t address) const
    {
        uint64_t index = ctx.stack_back().template as<size_t>();
        generic_print(address, "indexc"sv) << "\t; consts["sv << std::dec << index << "] = 0x"sv << std::hex << std::uppercase << ctx.const_at(index) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::pushfp>, ContextT& ctx, size_t address) const
    {
        generic_print(address, "pushfp"sv) << "\t; fp: "sv << std::dec << static_cast<uint32_t>(ctx.stack_size()) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::popfp>, ContextT& /*ctx*/, size_t address) const
    {
        generic_print(address, "popfp"sv) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::truncatefpp>, ContextT& ctx, size_t address, size_t cnt) const
    {
        generic_print(address, "truncatefpp "sv) << std::dec << (ctx.frame_stack_back() + cnt) << '\n';
        commit_line();
    }

    inline void operator()(identity_type<op::truncatefpn>, ContextT& ctx, size_t address, size_t cnt) const
    {
        size_t min_idx = ctx.frame_stack_back() - cnt;
        generic_print(address, "truncatefpn"sv) << " ["sv << std::dec << min_idx;
        if (min_idx < ctx.stack_size() - 1) {
            ss << " - "sv << (ctx.stack_size() - 1);
        }
        ss << "]\t; ["sv;

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
        ss << "]\n"sv;
        commit_line();
    }
};

template <typename ContextT>
struct runner
{
    using vm_t = virtual_stack_machine<ContextT>;
    using stack_type = typename ContextT::stack_type;
    using ext_function_t = typename vm_t::ext_function_t;
    //using ext1_function_t = typename vm_t::ext1_function_t;
    using var_t = typename vm_t::var_t;

    inline void operator()(identity_type<op::noop>, ContextT &, size_t) const {}

    inline size_t operator()(identity_type<op::jmp>, ContextT&, size_t /*address*/, size_t jmp_address, size_t) const { return jmp_address; }
    inline size_t operator()(identity_type<op::jmpp>, ContextT&, size_t address, size_t jmp_offset, size_t /*next*/) const { return address + jmp_offset; }
    inline size_t operator()(identity_type<op::jmpn>, ContextT&, size_t address, size_t jmp_offset, size_t /*next*/) const { return address - jmp_offset; }

    inline size_t operator()(identity_type<op::jg>, ContextT& ctx, size_t /*address*/, size_t jmp_address, size_t next_address) const
    {
        bool condition = ctx.is_positive(ctx.stack_back());
        ctx.stack_pop();
        return condition ? jmp_address : next_address;
    }

    inline size_t operator()(identity_type<op::jgp>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_positive(ctx.stack_back());
        ctx.stack_pop();
        return condition ? (address + jmp_offset) : next_address;
    }

    inline size_t operator()(identity_type<op::jgn>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_positive(ctx.stack_back());
        ctx.stack_pop();
        return condition ? (address - jmp_offset) : next_address;
    }

    inline size_t operator()(identity_type<op::jge>, ContextT& ctx, size_t /*address*/, size_t jmp_address, size_t next_address) const
    {
        bool condition = ctx.is_negative(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : jmp_address;
    }

    inline size_t operator()(identity_type<op::jgep>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_negative(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : (address + jmp_offset);
    }

    inline size_t operator()(identity_type<op::jgen>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_negative(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : (address - jmp_offset);
    }

    inline size_t operator()(identity_type<op::jl>, ContextT& ctx, size_t /*address*/, size_t jmp_address, size_t next_address) const
    {
        bool condition = ctx.is_negative(ctx.stack_back());
        ctx.stack_pop();
        return condition ? jmp_address : next_address;
    }

    inline size_t operator()(identity_type<op::jlp>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_negative(ctx.stack_back());
        ctx.stack_pop();
        return condition ? (address + jmp_offset) : next_address;
    }

    inline size_t operator()(identity_type<op::jln>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_negative(ctx.stack_back());
        ctx.stack_pop();
        return condition ? (address - jmp_offset) : next_address;
    }

    inline size_t operator()(identity_type<op::jle>, ContextT& ctx, size_t /*address*/, size_t jmp_address, size_t next_address) const
    {
        bool condition = ctx.is_positive(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : jmp_address;
    }

    inline size_t operator()(identity_type<op::jlep>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_positive(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : (address + jmp_offset);
    }

    inline size_t operator()(identity_type<op::jlen>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_positive(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : (address - jmp_offset);
    }

    inline size_t operator()(identity_type<op::jne>, ContextT& ctx, size_t /*address*/, size_t jmp_address, size_t next_address) const
    {
        bool condition = ctx.is_zero(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : jmp_address;
    }

    inline size_t operator()(identity_type<op::jnep>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_zero(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : (address + jmp_offset);
    }

    inline size_t operator()(identity_type<op::jnen>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_zero(ctx.stack_back());
        ctx.stack_pop();
        return condition ? next_address : (address - jmp_offset);
    }

    inline size_t operator()(identity_type<op::je>, ContextT& ctx, size_t /*address*/, size_t jmp_address, size_t next_address) const
    {
        bool condition = ctx.is_zero(ctx.stack_back());
        ctx.stack_pop();
        return condition ? jmp_address : next_address;
    }

    inline size_t operator()(identity_type<op::jep>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_zero(ctx.stack_back());
        ctx.stack_pop();
        return condition ? (address + jmp_offset) : next_address;
    }

    inline size_t operator()(identity_type<op::jen>, ContextT& ctx, size_t address, size_t jmp_offset, size_t next_address) const
    {
        bool condition = ctx.is_zero(ctx.stack_back());
        ctx.stack_pop();
        return condition ? (address - jmp_offset) : next_address;
    }

    inline void operator()(identity_type<op::cmp>, ContextT& ctx, size_t) const
    {
        ctx.cmp();
    }

    inline optional<size_t> operator()(identity_type<op::ret>, ContextT& ctx, size_t) const
    {
        return ctx.call_stack_pop();
    }

    inline size_t operator()(identity_type<op::call>, ContextT& ctx, size_t /*address*/, size_t call_address, size_t ret_address) const
    {
        ctx.call_stack_push(ret_address);
        return call_address;
    }

    inline void operator()(identity_type<op::ecall>, ContextT& ctx, size_t /*address*/, size_t fn_index) const { ctx.efn(fn_index); }

    //inline void operator()(identity_type<op::ecall1>, ContextT& ctx, size_t address, size_t arg_index, size_t fn_index, ext1_function_t fn) const { fn(ctx, ctx.stack_at(arg_index)); }
    //
    //inline void operator()(identity_type<op::fpecall1>, ContextT& ctx, size_t address, size_t arg_offset, size_t fn_index, ext1_function_t fn) const { fn(ctx, ctx.stack_at(ctx.frame_stack_back() + arg_offset)); }
    //
    //inline void operator()(identity_type<op::fnecall1>, ContextT& ctx, size_t address, size_t arg_offset, size_t fn_index, ext1_function_t fn) const { fn(ctx, ctx.stack_at(ctx.frame_stack_back() - arg_offset)); }

    inline void operator()(identity_type<op::pop>, ContextT& ctx, size_t /*address*/) const { ctx.stack_pop(); }

    inline void operator()(identity_type<op::popn>, ContextT& ctx, size_t /*address*/, size_t n) const { ctx.stack_pop(n); }

    inline void operator()(identity_type<op::collapse>, ContextT& ctx, size_t /*address*/, size_t n) const { ctx.stack_collapse(n); }

    inline void operator()(identity_type<op::push>, ContextT& ctx, size_t /*address*/, size_t index) const
    {
        var_t val = ctx.stack_at(index);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::pushc>, ContextT& ctx, size_t /*address*/, size_t index) const
    {
        var_t val = ctx.const_at(index);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::push_stsz>, ContextT& ctx, size_t /*address*/) const
    {
        var_t val = ctx.value_of(ctx.stack_size());
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::pushr>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        var_t val = ctx.stack_back(offset);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::fppush>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        var_t val = ctx.stack_at(ctx.frame_stack_back() + offset);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::fnpush>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        var_t val = ctx.stack_at(ctx.frame_stack_back() - offset);
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::pushi>, ContextT& ctx, size_t /*address*/, size_t index) const
    {
        ctx.stack_push(ctx.value_of(index));
    }

    inline void operator()(identity_type<op::fppushi>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        ctx.stack_push(ctx.value_of(ctx.frame_stack_back() + offset));
    }

    inline void operator()(identity_type<op::fnpushi>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        ctx.stack_push(ctx.value_of(ctx.frame_stack_back() - offset));
    }

    inline void operator()(identity_type<op::dup>, ContextT& ctx, size_t /*address*/) const
    {
        var_t val = ctx.stack_back();
        ctx.stack_push(std::move(val));
    }

    inline void operator()(identity_type<op::set>, ContextT& ctx, size_t /*address*/, size_t index) const
    {
        ctx.stack_at(index) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::setr>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        ctx.stack_back(offset) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::fpset>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        ctx.stack_at(ctx.frame_stack_back() + offset) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::fnset>, ContextT& ctx, size_t /*address*/, size_t offset) const
    {
        ctx.stack_at(ctx.frame_stack_back() - offset) = ctx.stack_back();
    }

    inline void operator()(identity_type<op::pindexs>, ContextT& ctx, size_t /*address*/, size_t shift) const
    {
        uint64_t index = ctx.stack_back().template as<size_t>() + shift;
        ctx.stack_back().replace(var_t{ ctx.stack_at(index) });
    }

    inline void operator()(identity_type<op::nindexs>, ContextT& ctx, size_t /*address*/, size_t shift) const
    {
        uint64_t index = ctx.stack_back().template as<size_t>() - shift;
        ctx.stack_back().replace(var_t{ ctx.stack_at(index) });
    }

    inline void operator()(identity_type<op::indexc>, ContextT& ctx, size_t /*address*/) const
    {
        uint64_t index = ctx.stack_back().template as<size_t>();
        ctx.stack_back().replace(var_t{ ctx.const_at(index) });
    }

    inline void operator()(identity_type<op::pushfp>, ContextT& ctx, size_t /*address*/) const { ctx.frame_stack_push(); }
    inline void operator()(identity_type<op::popfp>, ContextT& ctx, size_t /*address*/) const { ctx.frame_stack_pop(); }

    inline void operator()(identity_type<op::truncatefpp>, ContextT& ctx, size_t /*address*/, size_t cnt) const
    {
        ctx.stack_truncate(ctx.frame_stack_back() + cnt);
    }
    inline void operator()(identity_type<op::truncatefpn>, ContextT& ctx, size_t /*address*/, size_t cnt) const
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
    FirstRunnerT first;
    SecondRunnerT second;

    sequence_runner(FirstRunnerT const& f, SecondRunnerT const& s)
        : first{ f }, second{ s }
    {}

    template <typename ... ArgsT>
    inline auto operator()(ArgsT&& ... args) const
    {
        first(std::forward<ArgsT>(args)...);
        return second(std::forward<ArgsT>(args)...);
    }
};

// traverse() is generated by sonia-pygen's vm-dispatch generator from the VMOP_* tags on `op`
// above (see vmop_proto.hpp) - switch form for MSVC, computed-goto form for GCC/Clang. call,
// ecall and ret are VMOP_CUSTOM (irregular arg count / early-return / packed short form) and
// stay hand-written here as hook macros the generated file splices in by name; every other
// opcode's dispatch case is generated. vm2.gen.hpp is checked in (like sonia-prime's other
// *.gen.hpp files), so a build without sonia-pygen available just compiles the pre-generated
// file as-is - SONIA_DO_NOT_INCLUDE_GEN skips the #include instead for the pygen tool's own
// parse pass, which only needs the `op` declaration above, not a (possibly not yet generated)
// vm2.gen.hpp.
#define SONIA_VM_GEN_CUSTOM_CALL_SWITCH \
    { \
        size_t start_address = address++; \
        size_t call_address = read_uint(address); \
        address = ftor(identity<op::call>, ctx, start_address, call_address, address); \
        continue; \
    }
#define SONIA_VM_GEN_CUSTOM_ECALL_SWITCH \
    { \
        size_t start_address = address++; \
        size_t fn_index = read_uint(address); \
        ftor(identity<op::ecall>, ctx, start_address, fn_index); \
        continue; \
    }
#define SONIA_VM_GEN_CUSTOM_RET_SWITCH \
    if (auto optaddress = ftor(identity<op::ret>, ctx, address); optaddress) { \
        address = *optaddress; \
        continue; \
    } \
    return;

#define SONIA_VM_GEN_CUSTOM_CALL_GOTO \
    { \
        size_t start_address = address++; \
        size_t call_address = read_uint(address); \
        address = ftor(identity<op::call>, ctx, start_address, call_address, address); \
        goto *dispatch_table[code_[address]]; \
    }
#define SONIA_VM_GEN_CUSTOM_ECALL_GOTO \
    { \
        size_t start_address = address++; \
        size_t fn_index = read_uint(address); \
        ftor(identity<op::ecall>, ctx, start_address, fn_index); \
        goto *dispatch_table[code_[address]]; \
    }
#define SONIA_VM_GEN_CUSTOM_RET_GOTO \
    { \
        auto optaddress = ftor(identity<op::ret>, ctx, address); \
        if (!optaddress) return; \
        address = *optaddress; \
        goto *dispatch_table[code_[address]]; \
    }

#ifndef SONIA_DO_NOT_INCLUDE_GEN
#   include "vm2.gen.hpp"
#endif

#undef SONIA_VM_GEN_CUSTOM_CALL_SWITCH
#undef SONIA_VM_GEN_CUSTOM_ECALL_SWITCH
#undef SONIA_VM_GEN_CUSTOM_RET_SWITCH
#undef SONIA_VM_GEN_CUSTOM_CALL_GOTO
#undef SONIA_VM_GEN_CUSTOM_ECALL_GOTO
#undef SONIA_VM_GEN_CUSTOM_RET_GOTO

template <typename ContextT>
void virtual_stack_machine<ContextT>::run(ContextT& ctx, size_t address)
{
#if 0
    sequence_runner<printer<ContextT>, runner<ContextT>> rn{ printer<ContextT>{ std::cout }, {}};
#elif 0
    std::ostringstream line;
    sequence_runner<printer<ContextT>, runner<ContextT>> rn{ printer<ContextT>{ line, [&line](){
        std::string linestr = line.str();
        if (!linestr.empty() && linestr.back() == '\n') linestr.pop_back();
        GLOBAL_LOG_INFO() << linestr;
        line.str("");
    }}, {} };
    //printer<ContextT> rn{ std::cout };
#else
    runner<ContextT> rn;
#endif
    traverse(ctx, address, rn);
}

}
