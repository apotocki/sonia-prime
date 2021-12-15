//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/assert.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/polymorphic_traits.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/exceptions.hpp"

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
        : impl{std::forward<ArgsT>(args) ...}
    {}

    template <typename ArgT>
    explicit wrapper_iterator(ArgT && arg, enable_if_t<is_base_of_v<wrapper_iterator, remove_cvref_t<ArgT>>> * enabler = nullptr) 
        : impl{std::forward<ArgT>(arg).impl}
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

    void close()
    {
        if constexpr (iterators::has_method_close_v<ImplT, void()>)
        {
            impl.close();
        } else {
            impl->close();
        }
    }

    ImplT impl;
};

template <typename ValueT, typename SetValueT = ValueT>
class polymorphic_proxy_backend
{
public:
    virtual ~polymorphic_proxy_backend() = default;

    [[noreturn]] virtual ValueT get_dereference() const
    {
        THROW_NOT_SUPPORTED_ERROR("polymorphic_proxy_backend::get_dereference");
    }

    [[noreturn]] virtual void set_dereference(SetValueT)
    {
        THROW_NOT_SUPPORTED_ERROR("polymorphic_proxy_backend::set_dereference");
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
    virtual ~iterator_polymorphic() = default;

    virtual bool equal(iterator_polymorphic const& rhs) const = 0;
    virtual ReferenceT dereference() const = 0;
    virtual void increment() = 0;

    [[noreturn]] virtual void decrement()
    {
        THROW_NOT_SUPPORTED_ERROR("iterator_polymorphic::decrement");
    }

    [[noreturn]] virtual void advance(DifferenceT dif)
    {
        THROW_NOT_SUPPORTED_ERROR("iterator_polymorphic::advance");
    }

    [[noreturn]] virtual bool empty() const
    {
        THROW_NOT_SUPPORTED_ERROR("iterator_polymorphic::empty");
    }

    [[noreturn]] virtual void flush()
    {
        THROW_NOT_SUPPORTED_ERROR("iterator_polymorphic::flush");
    }

    [[noreturn]] virtual void close()
    {
        THROW_NOT_SUPPORTED_ERROR("iterator_polymorphic::close");
    }

    [[noreturn]] virtual size_t get_sizeof() const
    {
        THROW_NOT_SUPPORTED_ERROR("iterator_polymorphic::get_sizeof");
    }
};

template <typename ValueT, typename SetValueT = ValueT>
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
        return iterators::make_value_proxy<ValueT>(static_cast<proxy_backend_t const*>(this));
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
        : base(std::forward<ArgsT>(args)...)
    {}

    bool empty() const override
    {
        if constexpr (iterators::has_method_empty_v<IteratorT, bool()>) {
            return base.empty();
        } else {
            THROW_NOT_SUPPORTED_ERROR("%1% has no member: empty()"_fmt % typeid(IteratorT).name());
        }
    }

    bool equal(typename BaseT::iterator_polymorphic_t const& rhs) const override
    {
        return base == dynamic_cast<iterator_polymorpic_adapter_base const&>(rhs).base;
    }

    void increment() override { ++base; }

    void decrement() override
    {
        if constexpr (is_base_of_v<bidirectional_traversal_tag, CategoryT>) {
            --base;
        } else {
            THROW_NOT_SUPPORTED_ERROR("%1% is not a bidirectional iterator"_fmt % typeid(IteratorT).name());
        }
    }

    void advance(difference_type dif) override
    {
        if constexpr (is_base_of_v<random_access_traversal_tag, CategoryT>) {
            std::advance(base, dif);
        } else {
            THROW_NOT_SUPPORTED_ERROR("%1% is not a random access iterator"_fmt % typeid(IteratorT).name());
        }
    }

    void flush() override
    {
        if constexpr (iterators::has_method_flush_v<IteratorT, void()>) {
            base.flush();
        } else {
            THROW_NOT_SUPPORTED_ERROR("%1% has no member: flush()"_fmt % typeid(IteratorT).name());
        }
    }

    void close() override
    {
        if constexpr (iterators::has_method_close_v<IteratorT, void()>) {
            base.close();
        } else {
            THROW_NOT_SUPPORTED_ERROR("%1% has no member: close()"_fmt % typeid(IteratorT).name());
        }
    }

    IteratorT base;

protected:
    template <class DerivedT>
    static polymorphic_clonable * do_clone(DerivedT const * obj, void * address, size_t sz)
    {
        if constexpr (is_copy_constructible_v<IteratorT>) {
            BOOST_ASSERT(sz >= obj->get_sizeof());
            new (address) DerivedT(*obj);
            return reinterpret_cast<DerivedT*>(address);
        } else {
            THROW_NOT_SUPPORTED_ERROR("%1% can not be copied"_fmt % typeid(IteratorT).name());
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
            THROW_NOT_SUPPORTED_ERROR("%1% can not be moved"_fmt % typeid(IteratorT).name());
        }
    }
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
        return *this->base;
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
        return *this->base;
    }

    void set_dereference(SetValueT val) override final
    {
        *this->base = std::move(val);
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

template <size_t SizeV, typename ValueT, class CategoryOrTraversal, typename ReferenceT = ValueT const&, typename DifferenceT = std::ptrdiff_t, size_t AlignmentV = std::alignment_of_v<iterator_polymorphic<ReferenceT, DifferenceT>>, typename OffsetT = void>
using automatic_polymorphic_iterator =
    wrapper_iterator<
        automatic_polymorphic<iterator_polymorphic<ReferenceT, DifferenceT>, SizeV, AlignmentV, OffsetT>,
        ValueT, CategoryOrTraversal, DifferenceT
    >;

template <size_t SizeV, typename ValueT, class CategoryOrTraversal, typename SetValueT, typename DifferenceT = std::ptrdiff_t, size_t AlignmentV = std::alignment_of_v<proxying_iterator_polymorphic<ValueT, SetValueT, DifferenceT>>, typename OffsetT = void>
using automatic_polymorphic_output_iterator =
    wrapper_iterator<
        automatic_polymorphic<proxying_iterator_polymorphic<ValueT, SetValueT, DifferenceT>, SizeV, AlignmentV, OffsetT>,
        ValueT, CategoryOrTraversal, DifferenceT
    >;
}
