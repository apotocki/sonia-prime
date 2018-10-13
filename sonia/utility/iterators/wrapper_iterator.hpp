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
#include <sstream>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/throw_exception.hpp>
#include <boost/assert.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/polymorphic_traits.hpp"
#include "sonia/utility/optional_inheritor.hpp"

namespace sonia { namespace detail {

template <class ImplT, class ReferenceT>
class wrapper_iterator_proxy
{
public:
    explicit wrapper_iterator_proxy(ImplT & impl) : impl_(impl) {}
    wrapper_iterator_proxy(wrapper_iterator_proxy &&) = default;

    wrapper_iterator_proxy(wrapper_iterator_proxy const&) = delete;
    wrapper_iterator_proxy & operator = (wrapper_iterator_proxy const&) = delete;
    wrapper_iterator_proxy & operator = (wrapper_iterator_proxy &&) = delete;

    operator ReferenceT() const { return impl_.dereference(); }
    wrapper_iterator_proxy & operator = (ReferenceT ref) { impl_.set(ref); return *this; }

private:
    ImplT & impl_;
};

} // namespace sonia::detail

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
class wrapper_input_iterator
    : public boost::iterator_facade<
          wrapper_input_iterator<ImplT, Value, CategoryOrTraversal, ReferenceT, DifferenceT>
        , Value, CategoryOrTraversal, ReferenceT, DifferenceT
    >
{
    friend class boost::iterator_core_access;

    bool equal(wrapper_input_iterator const& rhs) const {
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
    template <class T, typename ... ArgsT>
    explicit wrapper_input_iterator(in_place_type_t<T>, ArgsT && ... args) 
        : impl_(in_place_type_t<T>(), std::forward<ArgsT>(args) ...)
    { }

    template <typename ... ArgsT>
    explicit wrapper_input_iterator(in_place_t, ArgsT && ... args) 
        : impl_(std::forward<ArgsT>(args) ...)
    { }

    bool empty() const { return impl_.empty(); }

private:
    ImplT impl_;
};

template <
      class ImplT
    , class Value
    , class CategoryOrTraversal
    , class ReferenceT = Value&
    , class DifferenceT = std::ptrdiff_t
>
class wrapper_output_iterator 
    : public boost::iterator_facade<
          wrapper_output_iterator<ImplT, Value, CategoryOrTraversal, ReferenceT, DifferenceT>
        , Value, CategoryOrTraversal, detail::wrapper_iterator_proxy<ImplT, ReferenceT>, DifferenceT
    >
{
    friend class boost::iterator_core_access;
    typedef detail::wrapper_iterator_proxy<ImplT, ReferenceT> proxy_type;

    bool equal(wrapper_output_iterator const& rhs) const {
        return impl_.equal(rhs.impl_);
    }

    void increment() {
        impl_.increment();
    }

    void decrement() {
        impl_.decrement();
    }

    proxy_type dereference() const {
        return proxy_type(impl_);
    }

    void advance(DifferenceT dif) {
        impl_.advance(dif);
    }

public:
    template <class T, typename ... ArgsT>
    explicit wrapper_output_iterator(in_place_type_t<T>, ArgsT && ... args) 
        : impl_(in_place_type_t<T>(), std::forward<ArgsT>(args) ...)
    { }

    template <typename ... ArgsT>
    explicit wrapper_output_iterator(std::in_place_t, ArgsT && ... args) 
        : impl_(std::forward<ArgsT>(args) ...)
    { }

    bool empty() const { return impl_.empty(); }

private:
    mutable ImplT impl_;
};

template <
      class ReferenceT
    , class DifferenceT = std::ptrdiff_t
>
class iterator_polymorphic_impl 
    : public polymorphic_clonable
    , public polymorphic_movable
{
public:
    virtual ~iterator_polymorphic_impl() noexcept {}

    virtual bool empty() const {
        throw not_supported_operation_error("iterator_polymorphic_impl::empty");
    }

    virtual bool equal(iterator_polymorphic_impl const& rhs) const {
        throw not_supported_operation_error("iterator_polymorphic_impl::equal");
    }

    virtual void increment() {
        throw not_supported_operation_error("iterator_polymorphic_impl::increment");
    }

    virtual void decrement() {
        throw not_supported_operation_error("iterator_polymorphic_impl::decrement");
    }

    virtual void set(ReferenceT ref) {
        throw not_supported_operation_error("iterator_polymorphic_impl::set");
    }

    virtual ReferenceT dereference() const {
        throw not_supported_operation_error("iterator_polymorphic_impl::dereference");
    }

    virtual void advance(DifferenceT dif) {
        throw not_supported_operation_error("iterator_polymorphic_impl::advance");
    }

    virtual size_t get_sizeof() const {
        throw not_supported_operation_error("iterator_polymorphic_impl::get_sizeof");
    }
};

template <typename PtrT, typename CategoryT>
class iterator_ptr_wrappee_adapter
{
public:
    template <typename ... ArgsT>
    explicit iterator_ptr_wrappee_adapter(in_place_t, ArgsT && ... args) 
        : impl_(std::forward<ArgsT>(args) ...)
    { }

    iterator_ptr_wrappee_adapter(iterator_ptr_wrappee_adapter const& rhs)
        : impl_(iterator_copy<CategoryT>::copy(rhs.impl_))
    {

    }

    iterator_ptr_wrappee_adapter & operator=(iterator_ptr_wrappee_adapter const& rhs) {
        if (impl_ != rhs.impl_) {
            //impl_.reset();
            impl_ = iterator_copy<CategoryT>::copy(rhs.impl_);
        }
        return *this;
    }

    iterator_ptr_wrappee_adapter(iterator_ptr_wrappee_adapter &&) = default;
    iterator_ptr_wrappee_adapter & operator=(iterator_ptr_wrappee_adapter &&) = default;

    bool empty() const { return impl_->empty(); }

    bool equal(iterator_ptr_wrappee_adapter const& rhs) const {
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

    decltype(auto) dereference() const {
        return impl_->dereference();
    }

    template <typename DifferenceT>
    void advance(DifferenceT dif) {
        return impl_->advance(dif);
    }

    template <typename T>
    void set(T && arg) {
        impl_->set(std::forward<T>(arg));
    }

private:
    PtrT impl_;
};

template <
      size_t SizeV
    , typename ReferenceT
    , typename DifferenceT = std::ptrdiff_t
    , typename OffsetT = void
>
class iterator_polymorphic_wrappee_adapter
{
    typedef iterator_polymorphic_impl<ReferenceT, DifferenceT> polymorphic_impl_t;

public:
    iterator_polymorphic_wrappee_adapter() {}

    template <typename T, class ... ArgsT>
    explicit iterator_polymorphic_wrappee_adapter(in_place_type_t<T>, ArgsT&& ... args) 
        : impl_(in_place_type_t<T>(), std::forward<ArgsT>(args) ...) 
    { }

    bool equal(iterator_polymorphic_wrappee_adapter const& rhs) const {
        if (empty()) return rhs.empty();
        if (rhs.empty()) return false;
        return impl_->equal(*rhs.impl_);
    }

    void increment() {
        impl_->increment();
    }

    void decrement() {
        impl_->decrement();
    }

    decltype(auto) dereference() const {
        return impl_->dereference();
    }

    template <typename DiffT>
    void advance(DiffT dif) {
        return impl_->advance(dif);
    }

    template <typename T>
    void set(T && arg) {
        impl_->set(std::forward<T>(arg));
    }

    bool empty() const { return impl_.empty() || impl_->empty(); }

private:
    optional_inheritor<polymorphic_impl_t, SizeV, OffsetT> impl_;
};

template <
    typename IteratorT,
    typename CategoryT = iterator_traversal_t<IteratorT>,
    typename ReferenceT = iterator_reference_t<IteratorT>
>
class iterator_polymorpic_impl_adapter_impl
    : public iterator_polymorphic_impl<
        ReferenceT,
        iterator_difference_t<IteratorT>
    >
{
public:
    typedef iterator_difference_t<IteratorT> difference_type;
    typedef iterator_polymorphic_impl<ReferenceT, difference_type> impl_t;

    template <typename ... ArgsT>
    explicit iterator_polymorpic_impl_adapter_impl(ArgsT&& ... args) : it_(std::forward<ArgsT>(args)...) {}

    bool empty() const override {
        return it_.empty();
    }

    bool equal(impl_t const& rhs) const override {
        return it_ == static_cast<iterator_polymorpic_impl_adapter_impl const&>(rhs).it_;
    }

    void increment() override {
        ++it_;
    }

    ReferenceT dereference() const override {
        return *it_;
    }

    void set(ReferenceT val) override {
        if constexpr (is_output_iterator_v<IteratorT, ReferenceT>) {
            *it_ = val;
        } else {
            std::ostringstream errss;
            errss << "An attempt to write into the not writable iterator " << typeid(IteratorT).name();
            BOOST_THROW_EXCEPTION(not_supported_operation_error(std::move(errss.str())));
        }
    }

protected:
    IteratorT it_;
};

template <class IteratorT, typename ReferenceT>
class iterator_polymorpic_impl_adapter_impl<IteratorT, forward_traversal_tag, ReferenceT>
    : public iterator_polymorpic_impl_adapter_impl<IteratorT, single_pass_traversal_tag, ReferenceT>
{
    typedef iterator_polymorpic_impl_adapter_impl<IteratorT, single_pass_traversal_tag, ReferenceT> base_t;

public:
    using base_t::base_t;

};

template <class IteratorT, typename ReferenceT>
class iterator_polymorpic_impl_adapter_impl<IteratorT, bidirectional_traversal_tag, ReferenceT>
    : public iterator_polymorpic_impl_adapter_impl<IteratorT, forward_traversal_tag, ReferenceT>
{
    typedef iterator_polymorpic_impl_adapter_impl<IteratorT, forward_traversal_tag, ReferenceT> base_t;

public:
    using base_t::base_t;

    void decrement() override {
        --base_t::it_;
    }
};

template <class IteratorT, typename ReferenceT>
class iterator_polymorpic_impl_adapter_impl<IteratorT, random_access_traversal_tag, ReferenceT>
    : public iterator_polymorpic_impl_adapter_impl<IteratorT, bidirectional_traversal_tag, ReferenceT>
{
    typedef iterator_polymorpic_impl_adapter_impl<IteratorT, bidirectional_traversal_tag, ReferenceT> base_t;

public:
    using base_t::base_t;

    typedef iterator_difference_t<IteratorT> difference_type;

    void advance(difference_type dif) override {
        std::advance(base_t::it_, dif);
    }
};

template <
    typename IteratorT,
    typename CategoryT = iterator_traversal_t<IteratorT>,
    typename ReferenceT = iterator_reference_t<IteratorT>
>
class iterator_polymorpic_impl_adapter
    : public iterator_polymorpic_impl_adapter_impl<IteratorT, CategoryT, ReferenceT>
{
    typedef iterator_polymorpic_impl_adapter_impl<IteratorT, CategoryT, ReferenceT> base_t;

public:
    using base_t::base_t;

    size_t get_sizeof() const override final {
        return sizeof(iterator_polymorpic_impl_adapter);
    }

    polymorphic_clonable * clone(void * address, size_t sz) const override final {
        BOOST_ASSERT(sz >= this->get_sizeof());
        new (address) iterator_polymorpic_impl_adapter(*this);
        return reinterpret_cast<iterator_polymorpic_impl_adapter*>(address);
    }

    polymorphic_movable * move(void * address, size_t sz) override final {
        BOOST_ASSERT(sz >= this->get_sizeof());
        new (address) iterator_polymorpic_impl_adapter(std::move(*this));
        return reinterpret_cast<iterator_polymorpic_impl_adapter*>(address);
    }
};

template <size_t SizeV, class ValueT, class CategoryOrTraversal, class ReferenceT = ValueT&, class DifferenceT = std::ptrdiff_t, typename OffsetT = void>
using polymorphic_input_iterator = wrapper_input_iterator<
    iterator_polymorphic_wrappee_adapter<SizeV, ReferenceT, DifferenceT, OffsetT>,
    ValueT, CategoryOrTraversal, ReferenceT, DifferenceT
>;

template <size_t SizeV, class ValueT, class CategoryOrTraversal, class ReferenceT = ValueT&, class DifferenceT = std::ptrdiff_t, typename OffsetT = void>
using polymorphic_output_iterator = wrapper_output_iterator<
    iterator_polymorphic_wrappee_adapter<SizeV, ReferenceT, DifferenceT, OffsetT>,
    ValueT, CategoryOrTraversal, ReferenceT, DifferenceT
>;

}

#endif // SONIA_UTILITY_POLYMORPHIC_ITERATOR_HPP
