//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <cstdint>
#include <jni.h>

#include "sonia/shared_ptr.hpp"

namespace sonia::invocation {

// A native object exposed to Java (com.sonia.invocation.NativeObject) is referenced by a handle:
// the address of a heap-allocated shared_ptr copy owned by that Java object. Every crossing into
// Java gets its own copy, so releasing a handle never affects any other Java wrapper.
template <typename T>
inline jlong make_native_handle(shared_ptr<T> obj)
{
    return static_cast<jlong>(reinterpret_cast<intptr_t>(new shared_ptr<T>(std::move(obj))));
}

template <typename T>
inline shared_ptr<T> const& native_handle_ref(jlong handle) noexcept
{
    return *reinterpret_cast<shared_ptr<T> const*>(static_cast<intptr_t>(handle));
}

template <typename T>
inline void free_native_handle(jlong handle) noexcept
{
    delete reinterpret_cast<shared_ptr<T>*>(static_cast<intptr_t>(handle));
}

}
