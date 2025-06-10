//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "../vm/bang_vm.hpp"

namespace sonia::lang::bang {

void bang_error(vm::context&);
void bang_assert(vm::context&);
void bang_any_equal(vm::context&);
void bang_decimal_equal(vm::context&);
void bang_tostring(vm::context&);
void bang_print_string(vm::context&);
void bang_concat(vm::context&);
void bang_arrayify(vm::context&);
void bang_array_tail(vm::context&);
void bang_array_at(vm::context&);

void bang_create_extern_object(vm::context&);
void bang_set_object_property(vm::context&);

void bang_negate(vm::context&);
void bang_concat_string(vm::context&);

void bang_operator_plus_integer(vm::context&);
void bang_operator_plus_decimal(vm::context&);


void bang_str2dec(vm::context&);
void bang_int2dec(vm::context&);
void bang_int2flt(vm::context&);

}
