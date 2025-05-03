//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <memory>

namespace sonia {

using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;

using std::make_shared;
using std::allocate_shared;
using std::dynamic_pointer_cast;
using std::static_pointer_cast;
using std::const_pointer_cast;

template <class T>
shared_ptr<T> make_clone(shared_ptr<T> const& sptr)
{
    if (!sptr) return sptr;
    size_t sz = sptr->get_sizeof();
    std::byte* ptr = new std::byte[sz];
    try {
        sptr->clone(ptr, sz);
        return shared_ptr<T>(std::launder(reinterpret_cast<T*>(ptr)), [](T * p) { p->~T();  delete[] reinterpret_cast<std::byte*>(p); });
    } catch (...) {
        delete[] ptr;
        throw;
    }
}

template <class T, class FactoryT>
shared_ptr<T> construct_shared(FactoryT const& f)
{
    std::byte* ptr = new std::byte[sizeof(T)];
    try {
        f.template apply<T>(ptr);
        return shared_ptr<T>(std::launder(reinterpret_cast<T*>(ptr)), [](T * p) { p->~T();  delete[] reinterpret_cast<std::byte*>(p); });
    } catch (...) {
        delete[] ptr;
        throw;
    }
}

}

namespace std {

template <typename T> T * get_pointer(shared_ptr<T> const& p) { return p.get(); }

}
