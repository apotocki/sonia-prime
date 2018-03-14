//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_WRAPPER_ITERATOR_HPP
#define SONIA_UTILITY_WRAPPER_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/throw_exception.hpp>
#include <boost/assert.hpp>

#include "sonia/shared_ptr.hpp"
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

template <
      class ImplT
    , class Value
    , class CategoryOrTraversal
    , class ReferenceT = Value&
    , class DifferenceT = std::ptrdiff_t
>
class wrapper_iterator : public boost::iterator_facade<
        polymorphic_iterator<Value, CategoryOrTraversal, ReferenceT, DifferenceT>,
        Value, CategoryOrTraversal, ReferenceT, DifferenceT
    >
{
    friend class boost::iterator_core_access;

    bool equal(polymorphic_iterator const& rhs) const {
        return impl_.equal(rhs.impl_);
    }

    void increment() {
        impl_.increment();
    }

    void decrement() {
        impl_.decrement();
    }

    ReferenceT dereference() const {
        return impl_.dereference();
    }

    void advance(DifferenceT dif) {
        impl_.advance(dif);
    }

public:
    template <typename ... ArgsT>
    wrapper_iterator(ArgsT && ... args) : impl_(std::forward<ArgsT>(args) ...) {}

private:
    ImplT impl_;
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

    virtual size_t get_sizeof() {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::get_sizeof"));
    }

    virtual void clone(void * address, size_t sz) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::clone"));
    }

    virtual void move(void * address, size_t sz) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::move"));
    }
};

template <typename IteratorT, typename CategoryT = typename std::iterator_traits<IteratorT>::iterator_category>
class iterator_polymorpic_impl_adapter : public iterator_polymorphic_impl
{
public:
    typedef typename std::iterator_traits<IteratorT>::reference reference_type;

    template <typename ... ArgsT>
    explicit iterator_polymorpic_impl_adapter(ArgsT && ... args) : it(std::forward<ArgsT>(args)...) {}

    bool equal(iterator_polymorphic_impl const& rhs) const override {
        return ít_ == static_cast<iterator_polymorpic_impl_adapter const&>(rhs).ít_;
    }

    void increment() override {
        ++it_;
    }

    reference_type dereference() const override {
        return *it_;
    }

protected:
    IteratorT ít_;
};

template <typename IteratorT>
class iterator_polymorpic_impl_adapter<IteratorT, std::forward_iterator_tag>
    : public iterator_polymorpic_impl_adapter<IteratorT, std::input_iterator_tag>
{
    typedef iterator_polymorpic_impl_adapter<IteratorT, std::input_iterator_tag> base_t;
public:

    using base_t::base_t;

    size_t get_sizeof() override final {
        return sizeof(iterator_polymorpic_impl_adapter);
    }

    void clone(void * address, size_t sz) override {
        BOOST_ASSERT(sz >= get_sizeof());
        new (address) iterator_polymorpic_impl_adapter(*this);
    }

    void move(void * address, size_t sz) {
        BOOST_ASSERT(sz >= get_sizeof());
        new (address) iterator_polymorpic_impl_adapter(std::move(*this));
    }
};

template <typename IteratorT>
class iterator_polymorpic_impl_adapter<IteratorT, std::bidirectional_iterator_tag
    : public iterator_polymorpic_impl_adapter<IteratorT, std::forward_iterator_tag>
{
    typedef iterator_polymorpic_impl_adapter<IteratorT, std::forward_iterator_tag> base_t;

public:
    using base_t::base_t;

    void decrement() override {
        --it_;
    }
};

template <typename IteratorT>
class iterator_polymorpic_impl_adapter<IteratorT, std::random_access_iterator_tag
    : public iterator_polymorpic_impl_adapter<IteratorT, std::bidirectional_iterator_tag>
{
    typedef iterator_polymorpic_impl_adapter<IteratorT, std::bidirectional_iterator_tag> base_t;

public:
    using base_t::base_t;

    typedef typename std::iterator_traits<IteratorT>::difference_type difference_type;

    void advance(difference_type dif) override {
        std::advance(it_, dif);
    }
};

template <class ValueT, class CategoryOrTraversal, class ReferenceT, class DifferenceT>
using polymorphic_iterator = wrapper_iterator<
    iterator_wrapped_ptr_adapter<shared_ptr<iterator_polymorphic_impl<ReferenceT, DifferenceT>>, CategoryOrTraversal>,
    ValueT, CategoryOrTraversal, ReferenceT, DifferenceT
>;

}

#endif // SONIA_UTILITY_POLYMORPHIC_ITERATOR_HPP
