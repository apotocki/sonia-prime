// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ATOMIC_MEMORY_ORDER_HPP
#define AGNOSTIC_STD_ATOMIC_MEMORY_ORDER_HPP

#pragma once

namespace std {

enum memory_order
{
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};

}

#endif // AGNOSTIC_STD_ATOMIC_MEMORY_ORDER_HPP
