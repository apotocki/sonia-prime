//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_AUTOMATIC_HPP
#define SONIA_AUTOMATIC_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T>
class automatic {
public:
    automatic() {
        new (get_pointer()) T;
    }

    template <typename FactoryT>
    explicit automatic(FactoryT const& f) {
        f.template apply<T>(&buffer_);
    }

    automatic(automatic const& rhs) {
        new (get_pointer()) T(*rhs);
    }

    automatic(automatic && rhs) {
        new (get_pointer()) T(std::move(*rhs));
    }

    template <typename FactoryT>
    automatic & operator= (FactoryT const& rhs) {
        automatic temp(rhs);
        get() = std::move(temp.get());
        return *this;
    }

    automatic & operator= (automatic const& rhs) {
        get() = rhs.get();
        return *this;
    }

    automatic & operator= (automatic && rhs) {
        get() = std::move(rhs.get());
        return *this;
    }

    ~automatic() {
        get_pointer()->~T();
    }

    T const& get() const { return *get_pointer(); }
    T & get() { return *get_pointer(); }

    T const* get_pointer() const { return reinterpret_cast<T const*>(&buffer_); }
    T * get_pointer() { return reinterpret_cast<T*>(&buffer_); }

    T const& operator*() const { return get(); }
    T & operator*() { return get(); }

    T const* operator->() const { return get_pointer(); }
    T * operator->() { return get_pointer(); }

private:
    aligned_storage_t<sizeof(T), std::alignment_of_v<T>> buffer_;
};

}

#endif // SONIA_AUTOMATIC_HPP
