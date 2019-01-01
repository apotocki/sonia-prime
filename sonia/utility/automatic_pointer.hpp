//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_AUTOMATIC_POINTER_HPP
#define SONIA_UTILITY_AUTOMATIC_POINTER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/shared_ptr.hpp"
#include "sonia/utility/polymorphic_traits.hpp"
#include "sonia/utility/explicit_operator_bool.hpp"

namespace sonia {

template <typename T>
class automatic_pointer;


template <typename T>
class automatic_pointer<shared_ptr<T>>
{
public:
    explicit automatic_pointer(T * rawptr = nullptr) : ptr_(rawptr) {}
    
    template <typename ... ArgsT>
    automatic_pointer(in_place_t, ArgsT && ... args)
        : ptr_(make_shared<T>(std::forward<ArgsT>(args) ...))
    {}

    template <class DT, typename ... ArgsT>
    automatic_pointer(in_place_type_t<DT>, ArgsT && ... args)
        : ptr_(make_shared<DT>(std::forward<ArgsT>(args) ...))
    {}

    automatic_pointer(automatic_pointer const& rhs)
    {
        if constexpr (is_polymorphic_clonable_v<T>) {
            ptr_ = make_clone(rhs.ptr_);
        } else {
            ptr_ = make_shared<T>(*rhs);
        }
    }

    automatic_pointer(automatic_pointer &&) = default;

    automatic_pointer & operator= (automatic_pointer const& rhs)
    {
        if (BOOST_LIKELY(ptr_.get() != rhs.ptr_.get())) {
            automatic_pointer tmp(rhs);
            ptr_ = std::move(tmp.ptr_);
        }
        return *this;
    }

    automatic_pointer & operator= (automatic_pointer && rhs) = default;

    bool operator!() const noexcept { return !ptr_; }

    BOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL();

    T * get() const { return ptr_.get(); }
    T * operator-> () const { return ptr_.get(); }
    T & operator* () const { return *ptr_; }

private:
    shared_ptr<T> ptr_;
};

template <typename T> T * get_pointer(automatic_pointer<T> const& p) { return p.get(); }

}

#endif // SONIA_UTILITY_AUTOMATIC_POINTER_HPP
