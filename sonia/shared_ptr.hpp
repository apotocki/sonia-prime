//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SHARED_PTR_HPP
#define SONIA_SHARED_PTR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <memory>
#include "sonia/utility/allocator_facade.hpp"

namespace sonia {

using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;

using std::make_shared;
using std::allocate_shared;
using std::dynamic_pointer_cast;
using std::static_pointer_cast;
using std::const_pointer_cast;

template <typename T, typename ... ArgsT>
shared_ptr<T> make_shared_adv(ArgsT && ... args)
{
    if constexpr (sizeof ...(ArgsT) == 1) {
        sonia::allocator_facade<std::allocator<T>> alloc;
        return sonia::allocate_shared<T>(alloc, std::forward<ArgsT>(args) ...);
    } else {
        return sonia::make_shared(std::forward<ArgsT>(args) ...);
    }
}

template <class T>
shared_ptr<T> make_clone(shared_ptr<T> const& ptr)
{
    if (!ptr) return ptr;
    size_t sz = ptr->get_sizeof();
    void * charptr = new char[sz];
    try {
        ptr->clone(charptr, sz);
        return shared_ptr<T>(reinterpret_cast<T*>(charptr), [](T * p) { p->~T();  delete[] (char*)p; });
    } catch (...) {
        throw;
    }
}

}

#endif // SONIA_SHARED_PTR_HPP
