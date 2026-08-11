//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

// Annotation macros describing the bytecode-decode shape of each virtual_stack_machine::op
// enumerator, consumed by the sonia-pygen vm-dispatch generator to emit
// virtual_stack_machine::traverse()'s dispatch body (switch form for MSVC, computed-goto form
// for GCC/Clang) from the `op` enum declaration in vm2.hpp instead of hand-writing it.
//
// Every enumerator in a DEFINE_VMOP_PROTO enum must carry exactly one of these three tags —
// there is no default/untagged shape. The generator treats a missing or unrecognized tag as a
// hard error rather than guessing, since guessing wrong here means silently misdecoding the
// bytecode stream (see BUGFIXES.md's op::ecall entry for what that looks like in practice).
//
//   VMOP_ARG    - the opcode reads one varint operand from the bytecode before dispatch.
//   VMOP_JUMP   - the handler's return value replaces the interpreter's `address` (control
//                 flow), instead of `address` simply advancing past the opcode/operand.
//   VMOP_SIMPLE - explicitly neither of the above: no operand, no control-flow change. Exists
//                 so "no operand, doesn't jump" is a deliberate, checkable statement rather than
//                 indistinguishable from "nobody tagged this enumerator yet" - an untagged
//                 enumerator has zero annotate attributes in the AST either way, so the strict
//                 "every value must carry a recognized tag" rule has nothing to check without it.
//   VMOP_CUSTOM - excluded from generation entirely; traverse() still hand-writes this case in
//                 both dispatch forms (irregular arg count, early-return, or non-uniform
//                 decoding that doesn't fit the ARG/JUMP/SIMPLE shapes below).
//
// Every enumerator must carry exactly one of: VMOP_CUSTOM, VMOP_SIMPLE, or one-or-both of
// {VMOP_ARG, VMOP_JUMP}. VMOP_SIMPLE and VMOP_CUSTOM are each mutually exclusive with every
// other tag (including each other) - the generator rejects any other combination, including no
// tag at all.

#ifndef __has_cpp_attribute
#   define __has_cpp_attribute(x) 0
#endif

#if __has_cpp_attribute(clang::annotate)
#   define VMOP_ARG      [[clang::annotate("vmop_arg")]]
#   define VMOP_JUMP     [[clang::annotate("vmop_jump")]]
#   define VMOP_SIMPLE   [[clang::annotate("vmop_simple")]]
#   define VMOP_CUSTOM   [[clang::annotate("vmop_custom")]]
#   define DEFINE_VMOP_PROTO(TypeName, UnderlyingType, ...) \
        enum class [[clang::annotate("vmop")]] \
        TypeName : UnderlyingType { __VA_ARGS__ };
#else
#   define VMOP_ARG
#   define VMOP_JUMP
#   define VMOP_SIMPLE
#   define VMOP_CUSTOM
#   define DEFINE_VMOP_PROTO(TypeName, UnderlyingType, ...) \
        enum class TypeName : UnderlyingType { __VA_ARGS__ };
#endif
