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
shared_ptr<T> make_clone(shared_ptr<T> const& ptr)
{
    if (!ptr) return ptr;
    size_t sz = ptr->get_sizeof();
    char * charptr = new char[sz];
    try {
        ptr->clone(charptr, sz);
        return shared_ptr<T>(std::launder(reinterpret_cast<T*>(charptr)), [](T * p) { p->~T();  delete[] reinterpret_cast<char*>(p); });
    } catch (...) {
        delete[] charptr;
        throw;
    }
}

template <class T, class FactoryT>
shared_ptr<T> construct_shared(FactoryT const& f)
{
    char * charptr = new char[sizeof(T)];
    try {
        f.template apply<T>(charptr);
        return shared_ptr<T>(std::launder(reinterpret_cast<T*>(charptr)), [](T * p) { p->~T();  delete[] reinterpret_cast<char*>(p); });
    } catch (...) {
        delete[] charptr;
        throw;
    }
}

}

namespace std {

template <typename T> T * get_pointer(shared_ptr<T> const& p) { return p.get(); }

}
