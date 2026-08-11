//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

// Auto-generated file. Do not edit manually.
#pragma once

// Generated from the `sonia::vm::op` enum (VMOP_* tags).
//
// Custom-tagged opcodes (VMOP_CUSTOM) are not generated here - the including file must
// #define one hook macro per custom opcode, per dispatch form, before this #include:
//   SONIA_VM_GEN_CUSTOM_<NAME>_SWITCH  (used in the switch-based form)
//   SONIA_VM_GEN_CUSTOM_<NAME>_GOTO    (used in the computed-goto form)
// <NAME> is the opcode's identifier, upper-cased.
//
// This file assumes an opcode literally named `ecall` exists and is VMOP_CUSTOM - its packed
// short form (op_byte >= 128 encodes the callee index directly in the opcode byte) is baked
// into both dispatch forms below, not generated from per-value data.
//
// The computed-goto form below relies on every reachable instruction sequence ending in a
// control-flow op that redirects `address` somewhere valid (op::ret included) - it does not
// bounds-check `address` against code_.size() at each dispatch site, unlike the switch form's
// `while (address < code_.size())`. See BUGFIXES.md's `environment::compile` entry for why that
// invariant needed a compiler-side fix before this form could rely on it safely.

#if defined(__GNUC__) || defined(__clang__)

// Label-as-value (&&label) and computed goto (goto *expr) are GNU extensions, not standard
// C++ - -Wpedantic flags every use. Intentional here (that's the whole point of this dispatch
// form); scoped to just this function so -Wpedantic stays fully active everywhere else.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

template <typename ContextT>
template <typename FunctorT>
void virtual_stack_machine<ContextT>::traverse(ContextT& ctx, size_t address, FunctorT const& ftor)
{
    // Plain positional initializer, one entry per byte value 0-255 (gaps -> L_bad_op, the
    // packed ecall short-form range 128-255 -> L_ecall_packed) - GCC's C++ frontend doesn't
    // accept GNU C's designated array initializers ([N] = value), unlike Clang, which takes them
    // as a portability extension; a positional list needs nothing beyond label-as-value itself,
    // which both compilers support identically.
    static void* const dispatch_table[256] = {
        &&L_noop,
        &&L_jmp,
        &&L_jmpp,
        &&L_jmpn,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_jg,
        &&L_jgp,
        &&L_jgn,
        &&L_jge,
        &&L_jgep,
        &&L_jgen,
        &&L_jl,
        &&L_jlp,
        &&L_jln,
        &&L_jle,
        &&L_jlep,
        &&L_jlen,
        &&L_je,
        &&L_jep,
        &&L_jen,
        &&L_jne,
        &&L_jnep,
        &&L_jnen,
        &&L_cmp,
        &&L_call,
        &&L_callp,
        &&L_ecall,
        &&L_ret,
        &&L_push,
        &&L_pushr,
        &&L_fppush,
        &&L_fnpush,
        &&L_pushi,
        &&L_fppushi,
        &&L_fnpushi,
        &&L_pushc,
        &&L_push_stsz,
        &&L_dup,
        &&L_set,
        &&L_setr,
        &&L_fpset,
        &&L_fnset,
        &&L_pindexs,
        &&L_nindexs,
        &&L_indexc,
        &&L_pop,
        &&L_popn,
        &&L_collapse,
        &&L_pushfp,
        &&L_popfp,
        &&L_truncatefpp,
        &&L_truncatefpn,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_bad_op,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
        &&L_ecall_packed,
    };

    goto *dispatch_table[code_[address]];

L_ecall_packed:
    {
        uint8_t op_byte = code_[address];
        auto cmdoffset = op_byte - 128u;
        if (cmdoffset >= efns_.size()) [[unlikely]] {
            throw internal_error("wrong command");
        }
        ftor(identity<op::ecall>, ctx, address, cmdoffset);
        ++address;
        goto *dispatch_table[code_[address]];
    }

L_bad_op:
    THROW_INTERNAL_ERROR();

L_call:
    SONIA_VM_GEN_CUSTOM_CALL_GOTO

L_ecall:
    SONIA_VM_GEN_CUSTOM_ECALL_GOTO

L_ret:
    SONIA_VM_GEN_CUSTOM_RET_GOTO

L_noop:
    ftor(identity<op::noop>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_jmp:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jmp>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jmpp:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jmpp>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jmpn:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jmpn>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jg:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jg>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jgp:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jgp>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jgn:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jgn>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jge:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jge>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jgep:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jgep>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jgen:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jgen>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jl:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jl>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jlp:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jlp>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jln:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jln>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jle:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jle>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jlep:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jlep>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jlen:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jlen>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_je:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::je>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jep:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jep>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jen:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jen>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jne:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jne>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jnep:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jnep>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_jnen:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        address = ftor(identity<op::jnen>, ctx, start_address, operand, address);
        goto *dispatch_table[code_[address]];
    }

L_cmp:
    ftor(identity<op::cmp>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_callp:
    address = ftor(identity<op::callp>, ctx, address);
    goto *dispatch_table[code_[address]];

L_push:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::push>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_pushr:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::pushr>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_fppush:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::fppush>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_fnpush:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::fnpush>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_pushi:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::pushi>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_fppushi:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::fppushi>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_fnpushi:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::fnpushi>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_pushc:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::pushc>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_push_stsz:
    ftor(identity<op::push_stsz>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_dup:
    ftor(identity<op::dup>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_set:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::set>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_setr:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::setr>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_fpset:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::fpset>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_fnset:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::fnset>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_pindexs:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::pindexs>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_nindexs:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::nindexs>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_indexc:
    ftor(identity<op::indexc>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_pop:
    ftor(identity<op::pop>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_popn:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::popn>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_collapse:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::collapse>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_pushfp:
    ftor(identity<op::pushfp>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_popfp:
    ftor(identity<op::popfp>, ctx, address);
    ++address;
    goto *dispatch_table[code_[address]];

L_truncatefpp:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::truncatefpp>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }

L_truncatefpn:
    {
        size_t start_address = address++;
        size_t operand = read_uint(address);
        ftor(identity<op::truncatefpn>, ctx, start_address, operand);
        goto *dispatch_table[code_[address]];
    }
}

#pragma GCC diagnostic pop

#else

template <typename ContextT>
template <typename FunctorT>
void virtual_stack_machine<ContextT>::traverse(ContextT& ctx, size_t address, FunctorT const& ftor)
{
    while (address < code_.size()) {
        uint8_t op_byte = code_[address];
        if (op_byte >= 128) {
            auto cmdoffset = op_byte - 128u;
            if (cmdoffset >= efns_.size()) [[unlikely]] {
                throw internal_error("wrong command");
            }
            ftor(identity<op::ecall>, ctx, address, cmdoffset);
            ++address;
            continue;
        }
        switch (static_cast<op>(op_byte)) {
        case op::call:
            SONIA_VM_GEN_CUSTOM_CALL_SWITCH
        case op::ecall:
            SONIA_VM_GEN_CUSTOM_ECALL_SWITCH
        case op::ret:
            SONIA_VM_GEN_CUSTOM_RET_SWITCH
        case op::noop:
            ftor(identity<op::noop>, ctx, address);
            ++address;
            continue;
        case op::jmp:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jmp>, ctx, start_address, operand, address);
                continue;
            }
        case op::jmpp:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jmpp>, ctx, start_address, operand, address);
                continue;
            }
        case op::jmpn:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jmpn>, ctx, start_address, operand, address);
                continue;
            }
        case op::jg:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jg>, ctx, start_address, operand, address);
                continue;
            }
        case op::jgp:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jgp>, ctx, start_address, operand, address);
                continue;
            }
        case op::jgn:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jgn>, ctx, start_address, operand, address);
                continue;
            }
        case op::jge:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jge>, ctx, start_address, operand, address);
                continue;
            }
        case op::jgep:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jgep>, ctx, start_address, operand, address);
                continue;
            }
        case op::jgen:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jgen>, ctx, start_address, operand, address);
                continue;
            }
        case op::jl:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jl>, ctx, start_address, operand, address);
                continue;
            }
        case op::jlp:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jlp>, ctx, start_address, operand, address);
                continue;
            }
        case op::jln:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jln>, ctx, start_address, operand, address);
                continue;
            }
        case op::jle:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jle>, ctx, start_address, operand, address);
                continue;
            }
        case op::jlep:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jlep>, ctx, start_address, operand, address);
                continue;
            }
        case op::jlen:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jlen>, ctx, start_address, operand, address);
                continue;
            }
        case op::je:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::je>, ctx, start_address, operand, address);
                continue;
            }
        case op::jep:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jep>, ctx, start_address, operand, address);
                continue;
            }
        case op::jen:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jen>, ctx, start_address, operand, address);
                continue;
            }
        case op::jne:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jne>, ctx, start_address, operand, address);
                continue;
            }
        case op::jnep:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jnep>, ctx, start_address, operand, address);
                continue;
            }
        case op::jnen:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                address = ftor(identity<op::jnen>, ctx, start_address, operand, address);
                continue;
            }
        case op::cmp:
            ftor(identity<op::cmp>, ctx, address);
            ++address;
            continue;
        case op::callp:
            address = ftor(identity<op::callp>, ctx, address);
            continue;
        case op::push:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::push>, ctx, start_address, operand);
                continue;
            }
        case op::pushr:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::pushr>, ctx, start_address, operand);
                continue;
            }
        case op::fppush:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::fppush>, ctx, start_address, operand);
                continue;
            }
        case op::fnpush:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::fnpush>, ctx, start_address, operand);
                continue;
            }
        case op::pushi:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::pushi>, ctx, start_address, operand);
                continue;
            }
        case op::fppushi:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::fppushi>, ctx, start_address, operand);
                continue;
            }
        case op::fnpushi:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::fnpushi>, ctx, start_address, operand);
                continue;
            }
        case op::pushc:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::pushc>, ctx, start_address, operand);
                continue;
            }
        case op::push_stsz:
            ftor(identity<op::push_stsz>, ctx, address);
            ++address;
            continue;
        case op::dup:
            ftor(identity<op::dup>, ctx, address);
            ++address;
            continue;
        case op::set:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::set>, ctx, start_address, operand);
                continue;
            }
        case op::setr:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::setr>, ctx, start_address, operand);
                continue;
            }
        case op::fpset:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::fpset>, ctx, start_address, operand);
                continue;
            }
        case op::fnset:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::fnset>, ctx, start_address, operand);
                continue;
            }
        case op::pindexs:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::pindexs>, ctx, start_address, operand);
                continue;
            }
        case op::nindexs:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::nindexs>, ctx, start_address, operand);
                continue;
            }
        case op::indexc:
            ftor(identity<op::indexc>, ctx, address);
            ++address;
            continue;
        case op::pop:
            ftor(identity<op::pop>, ctx, address);
            ++address;
            continue;
        case op::popn:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::popn>, ctx, start_address, operand);
                continue;
            }
        case op::collapse:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::collapse>, ctx, start_address, operand);
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
                size_t operand = read_uint(address);
                ftor(identity<op::truncatefpp>, ctx, start_address, operand);
                continue;
            }
        case op::truncatefpn:
            {
                size_t start_address = address++;
                size_t operand = read_uint(address);
                ftor(identity<op::truncatefpn>, ctx, start_address, operand);
                continue;
            }
        default:
            THROW_INTERNAL_ERROR();
        }
    }
}

#endif