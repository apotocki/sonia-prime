//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SHARED_PTR_HPP
#define SONIA_SHARED_PTR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <memory>

namespace sonia {

using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;

using std::make_shared;
using std::dynamic_pointer_cast;

template <class T>
shared_ptr<T> make_clone(shared_ptr<T> const& ptr) {
    if (!ptr) return ptr;
    size_t sz = ptr->get_sizeof();
    void * ptr = new char[sz];
    try {
        ptr->clone(ptr, sz);
        return shared_ptr<T>(reinterpret_cast<T*>(ptr), [](T * ptr) { ptr->~T();  delete[] ptr; });
    } catch (...) {
        throw;
    }
}

}

#endif // SONIA_SHARED_PTR_HPP
