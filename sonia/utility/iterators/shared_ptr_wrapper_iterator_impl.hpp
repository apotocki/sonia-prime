//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_SHARED_PTR_WRAPPER_ITERATOR_IMPL_HPP
#define SONIA_UTILITY_SHARED_PTR_WRAPPER_ITERATOR_IMPL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include <boost/throw_exception.hpp>

#include "sonia/exceptions.hpp"

namespace sonia {

template <typename CategoryT> struct iterator_copy {
    template <typename T>
    static T copy(T const& val) { return make_clone(val); }
};

template <> struct iterator_copy<std::input_iterator_tag> {
    template <typename T>
    static T copy(T val) { return std::move(val); }
};

template <typename PtrT, typename CategoryT>
class iterator_wrapped_ptr_adapter {
public:
    typedef iterator_wrapped_ptr_adapter self_type;

    template <typename ... ArgsT>
    explicit iterator_wrapped_ptr_adapter(ArgsT && ... args) : impl_(std::forward<ArgsT>(args) ...) {}

    iterator_wrapped_ptr_adapter(iterator_wrapped_ptr_adapter const& rhs)
        : impl_(iterator_copy<CategoryT>(rhs.impl_))
    {
        
    }

    iterator_wrapped_ptr_adapter & operator=(iterator_wrapped_ptr_adapter const& rhs) {
        if (impl_.get() != rhs.impl_.get()) {
            impl_.reset();
            impl_ = iterator_copy<CategoryT>(rhs.impl_);
        }
    }

    iterator_wrapped_ptr_adapter(iterator_wrapped_ptr_adapter &&) = default;
    iterator_wrapped_ptr_adapter & operator=(iterator_wrapped_ptr_adapter &&) = default;

    bool equal(self_type const& rhs) const {
        if (impl_) return !!rhs.impl_;
        if (rhs.impl_) return false;
        return impl_->equal(*rhs.impl_);
    }

    void increment() {
        impl_->increment();
    }

    void decrement() {
        impl_->decrement();
    }

    auto dereference() const {
        return impl_->dereference();
    }

    template <typename DifferenceT>
    void advance(DifferenceT dif) {
        return impl_->advance(dif);
    }

private:
    PtrT impl_;
};

template <
      class ReferenceT
    , class DifferenceT = std::ptrdiff_t
>
class iterator_polymorphic_impl
{
public:
    virtual ~iterator_polymorphic_impl() noexcept {}

    virtual bool equal(iterator_polymorphic_impl const& rhs) const {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::equal"));
    }

    virtual void increment() {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::increment"));
    }

    virtual void decrement() {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::decrement"));
    }

    virtual ReferenceT dereference() const {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::dereference"));
    }

    virtual void advance(DifferenceT dif) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::advance"));
    }

    virtual void clone(void * address, size_t sz) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::clone"));
    }

    virtual void move(void * address, size_t sz) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::move"));
    }
};

}

#endif // SONIA_UTILITY_SHARED_PTR_WRAPPER_ITERATOR_IMPL_HPP
