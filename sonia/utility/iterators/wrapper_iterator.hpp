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

#include "sonia/type_traits.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/polymorphic_traits.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"

#include "proxy.hpp"

namespace sonia {

template <
      class ImplT
    , class ValueT
    , class CategoryOrTraversal
    , class DifferenceT = std::ptrdiff_t
>
class wrapper_iterator
    : public boost::iterator_facade<
          wrapper_iterator<ImplT, ValueT, CategoryOrTraversal, DifferenceT>
        , ValueT
        , CategoryOrTraversal
        , decltype(std::declval<const ImplT>()->dereference())
        , DifferenceT
    >
{
    friend class boost::iterator_core_access;

    bool equal(wrapper_iterator const& rhs) const
    {
        if (impl) return !!rhs.impl;
        if (rhs.impl) return false;
        return impl->equal(*rhs.impl);
    }

    void increment()
    {
        if constexpr (iterators::has_method_increment_v<ImplT, void()>) {
            impl.increment();
        } else {
            impl->increment();
        }
    }

    void decrement()
    {
        if constexpr (iterators::has_method_decrement_v<ImplT, void()>) {
            impl.decrement();
        } else {
            impl->decrement();
        }
    }

    decltype(auto) dereference() const
    {
        return impl->dereference();
    }

    void advance(DifferenceT dif) { impl->advance(dif); }

public:
    using wrapper_iterator_t = wrapper_iterator;

    template <typename ... ArgsT>
    explicit wrapper_iterator(ArgsT && ... args)
        : impl(std::forward<ArgsT>(args) ...)
    {}

    template <typename ArgT>
    explicit wrapper_iterator(ArgT && arg, enable_if_t<is_same_v<remove_cvref_t<ArgT>, wrapper_iterator>> * enabler = nullptr) 
        : impl(std::forward<ArgT>(arg).impl)
    {}

    wrapper_iterator(wrapper_iterator const& rhs) = default;
    wrapper_iterator(wrapper_iterator && rhs) = default;
    wrapper_iterator & operator=(wrapper_iterator const& rhs) = default;
    wrapper_iterator & operator=(wrapper_iterator && rhs) = default;

    bool empty() const
    {
        if (!impl) return true;
        if constexpr(iterators::has_method_empty_v<ImplT, bool()>) {
            return impl.empty();
        } else {
            return impl->empty();
        }
    }

    void flush()
    {
        if constexpr (iterators::has_method_flush_v<ImplT, void()>)
        {
            impl.flush();
        } else {
            impl->flush();
        }
    }

    ImplT impl;
};

template <typename ValueT, typename SetValueT>
class polymorphic_proxy_backend
{
public:
    virtual ~polymorphic_proxy_backend() noexcept {}

    virtual ValueT get_dereference() const
    {
        throw not_supported_operation_error("polymorphic_proxy_backend::get_dereference");
    }

    virtual void set_dereference(SetValueT)
    {
        throw not_supported_operation_error("polymorphic_proxy_backend::set_dereference");
    }
};

template <
      typename ReferenceT
    , typename DifferenceT = std::ptrdiff_t
>
class iterator_polymorphic 
    : public polymorphic_clonable
    , public polymorphic_movable
{
protected:
    using iterator_polymorphic_t = iterator_polymorphic;

public:
    virtual ~iterator_polymorphic() noexcept {}

    virtual bool empty() const
    {
        throw not_supported_operation_error("iterator_polymorphic::empty");
    }

    virtual bool equal(iterator_polymorphic const& rhs) const
    {
        throw not_supported_operation_error("iterator_polymorphic::equal");
    }

    virtual void increment()
    {
        throw not_supported_operation_error("iterator_polymorphic::increment");
    }

    virtual void decrement()
    {
        throw not_supported_operation_error("iterator_polymorphic::decrement");
    }

    virtual void flush()
    {
        throw not_supported_operation_error("iterator_polymorphic::flush");
    }

    virtual ReferenceT dereference() const
    {
        throw not_supported_operation_error("iterator_polymorphic::dereference");
    }

    virtual void advance(DifferenceT dif)
    {
        throw not_supported_operation_error("iterator_polymorphic::advance");
    }

    virtual size_t get_sizeof() const
    {
        throw not_supported_operation_error("iterator_polymorphic::get_sizeof");
    }
};

template <typename ValueT, typename SetValueT>
using polymorphic_iterator_proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<polymorphic_proxy_backend<ValueT, SetValueT> const*, ValueT>>;

template <
      typename ValueT
    , typename SetValueT = ValueT
    , typename DifferenceT = std::ptrdiff_t
>
class proxying_iterator_polymorphic
    : public iterator_polymorphic<polymorphic_iterator_proxy_t<ValueT, SetValueT>, DifferenceT>
    , public polymorphic_proxy_backend<ValueT, SetValueT>
{
    using reference_t = polymorphic_iterator_proxy_t<ValueT, SetValueT>;
    using proxy_backend_t = polymorphic_proxy_backend<ValueT, SetValueT>;

public:
    reference_t dereference() const override final
    {
        return make_proxy<ValueT>(static_cast<proxy_backend_t const*>(this));
    }
};

template <typename IteratorT, class CategoryT, class BaseT>
class iterator_polymorpic_adapter_base
    : public BaseT
{
protected:
    using adapter_base_t = iterator_polymorpic_adapter_base;

public:
    using difference_type = iterator_difference_t<IteratorT>;

    template <typename ... ArgsT>
    explicit iterator_polymorpic_adapter_base(ArgsT&& ... args) 
        : it_(std::forward<ArgsT>(args)...)
    {}

    bool empty() const override
    {
        if constexpr (iterators::has_method_empty_v<IteratorT, bool()>) {
            return it_.empty();
        } else {
            BOOST_THROW_EXCEPTION (
                not_supported_operation_error("%1% has no member: empty()"_fmt % typeid(IteratorT).name())
            );
        }
    }

    bool equal(typename BaseT::iterator_polymorphic_t const& rhs) const override
    {
        return it_ == dynamic_cast<iterator_polymorpic_adapter_base const&>(rhs).it_;
    }

    void increment() override { ++it_; }

    void decrement() override
    {
        if constexpr (is_base_of_v<bidirectional_traversal_tag, CategoryT>) {
            --it_;
        } else {
            BOOST_THROW_EXCEPTION (
                not_supported_operation_error("%1% is not a bidirectional iterator"_fmt % typeid(IteratorT).name())
            );
        }
    }

    void advance(difference_type dif) override
    {
        if constexpr (is_base_of_v<random_access_traversal_tag, CategoryT>) {
            std::advance(it_, dif);
        } else {
            BOOST_THROW_EXCEPTION (
                not_supported_operation_error("%1% is not a random access iterator"_fmt % typeid(IteratorT).name())
            );
        }
    }

    void flush() override
    {
        if constexpr (iterators::has_method_flush_v<IteratorT, void()>) {
            it_.flush();
        } else {
            BOOST_THROW_EXCEPTION (
                not_supported_operation_error("%1% has no member: flush()"_fmt % typeid(IteratorT).name())
            );
        }
    }

    IteratorT & base() { return it_; }

protected:
    template <class DerivedT>
    static polymorphic_clonable * do_clone(DerivedT const * obj, void * address, size_t sz)
    {
        if constexpr (is_copy_constructible_v<IteratorT>) {
            BOOST_ASSERT(sz >= obj->get_sizeof());
            new (address) DerivedT(*obj);
            return reinterpret_cast<DerivedT*>(address);
        } else {
            throw not_supported_operation_error("%1% can not be copied"_fmt % typeid(IteratorT).name());
        }
    }

    template <class DerivedT>
    static polymorphic_movable * do_move(DerivedT * obj, void * address, size_t sz)
    {
        if constexpr (is_move_constructible_v<IteratorT>) {
            BOOST_ASSERT(sz >= obj->get_sizeof());
            new (address) DerivedT(std::move(*obj));
            return reinterpret_cast<DerivedT*>(address);
        } else {
            throw not_supported_operation_error("%1% can not be moved"_fmt % typeid(IteratorT).name());
        }
    }

    IteratorT it_;
};

template <
    typename IteratorT,
    class CategoryT = iterator_traversal_t<IteratorT>,
    typename ReferenceT = iterator_reference_t<IteratorT>
>
class iterator_polymorpic_adapter
    : public iterator_polymorpic_adapter_base<
        IteratorT,
        CategoryT,
        iterator_polymorphic<
            ReferenceT,
            iterator_difference_t<IteratorT>
        >
    >
{
    using adapter_base_t = typename iterator_polymorpic_adapter::adapter_base_t;

public:
    using adapter_base_t::adapter_base_t;

    ReferenceT dereference() const override final
    {
        return *this->it_;
    }

    size_t get_sizeof() const override final
    {
        return sizeof(iterator_polymorpic_adapter);
    }

    polymorphic_clonable * clone(void * address, size_t sz) const override final
    {
        return adapter_base_t::do_clone(this, address, sz);
    }

    polymorphic_movable * move(void * address, size_t sz) override final
    {
        return adapter_base_t::do_move(this, address, sz);
    }
};

template <
    typename IteratorT,
    class CategoryT = iterator_traversal_t<IteratorT>,
    typename ValueT = iterator_value_t<IteratorT>,
    typename SetValueT = iterator_value_t<IteratorT>
>
class output_iterator_polymorpic_adapter
    : public iterator_polymorpic_adapter_base<
        IteratorT,
        CategoryT,
        proxying_iterator_polymorphic<
            ValueT,
            SetValueT,
            iterator_difference_t<IteratorT>
        >
    >
{
    using adapter_base_t = typename output_iterator_polymorpic_adapter::adapter_base_t;
    using proxy_backend_t = polymorphic_proxy_backend<ValueT, SetValueT>;

public:
    using adapter_base_t::adapter_base_t;

    ValueT get_dereference() const override final
    {
        return *this->it_;
    }

    void set_dereference(SetValueT val) override final
    {
        *this->it_ = std::move(val);
    }

    size_t get_sizeof() const override final
    {
        return sizeof(output_iterator_polymorpic_adapter);
    }

    polymorphic_clonable * clone(void * address, size_t sz) const override final
    {
        return adapter_base_t::do_clone(this, address, sz);
    }

    polymorphic_movable * move(void * address, size_t sz) override final
    {
        return adapter_base_t::do_move(this, address, sz);
    }
};

template <size_t SizeV, typename ValueT, class CategoryOrTraversal, typename ReferenceT = ValueT&, typename DifferenceT = std::ptrdiff_t, typename OffsetT = void>
using automatic_polymorphic_iterator =
    wrapper_iterator<
        automatic_polymorphic<iterator_polymorphic<ReferenceT, DifferenceT>, SizeV, OffsetT>,
        ValueT, CategoryOrTraversal, DifferenceT
    >;

template <size_t SizeV, typename ValueT, class CategoryOrTraversal, typename SetValueT, typename DifferenceT = std::ptrdiff_t, typename OffsetT = void>
using automatic_polymorphic_output_iterator =
    wrapper_iterator<
        automatic_polymorphic<proxying_iterator_polymorphic<ValueT, SetValueT, DifferenceT>, SizeV, OffsetT>,
        ValueT, CategoryOrTraversal, DifferenceT
    >;
}

#endif // SONIA_UTILITY_POLYMORPHIC_ITERATOR_HPP
