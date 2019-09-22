//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ITERATORS_CHAIN_LINKABLE_ITERATOR_HPP
#define SONIA_UTILITY_ITERATORS_CHAIN_LINKABLE_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include <boost/intrusive_ptr.hpp>

#include "sonia/utility/iterators/wrapper_iterator.hpp"

#include "proxy.hpp"

namespace sonia {

template <typename ValueT, typename DifferenceT>
class chain_linkable_iterator_polymorphic
    : public iterator_polymorphic<polymorphic_iterator_proxy_t<ValueT>, DifferenceT>
    , public polymorphic_proxy_backend<ValueT>
{
public:
    chain_linkable_iterator_polymorphic() = default;
    chain_linkable_iterator_polymorphic(chain_linkable_iterator_polymorphic const&) = default;

    friend void intrusive_ptr_add_ref(chain_linkable_iterator_polymorphic * p)
    {
        ++p->refs;
    }

    friend void intrusive_ptr_release(chain_linkable_iterator_polymorphic * p)
    {
        if (0 == --p->refs) {
            std::destroy_at(p);
            delete[] reinterpret_cast<char*>(p);
        }
    }

    unsigned int refs{1};
};

template <typename ValueT, typename DifferenceT>
class chain_linkable_iterator_impl
{
public:
    using polymorphic_t = chain_linkable_iterator_polymorphic<ValueT, DifferenceT>;

    chain_linkable_iterator_impl() = default;

    template <class T, typename ... ArgsT>
    chain_linkable_iterator_impl(in_place_type_t<T>, ArgsT && ... args)
    {
        static_assert(is_base_of_v<polymorphic_t, T>);

        char * p = new char[sizeof(T)];
        try {
            new (p) T(std::forward<ArgsT>(args)...);
        } catch (...) {
            delete[] p;
            throw;
        }
        ptr.reset(reinterpret_cast<T*>(p), false);
    }

    explicit operator bool() const { return !!ptr; }

    polymorphic_t * operator-> () const { return ptr.get(); }
    boost::intrusive_ptr<polymorphic_t> ptr;
};

template <typename ValueT, class CategoryOrTraversal, typename DifferenceT = std::ptrdiff_t>
using chain_linkable_iterator = wrapper_iterator<
    chain_linkable_iterator_impl<ValueT, DifferenceT>,
    ValueT,
    CategoryOrTraversal,
    DifferenceT
>;

template <class IteratorT, class CategoryOrTraversal, typename DifferenceT>
class chain_linkable_iterator_polymorpic_adapter
    : public iterator_polymorpic_adapter_base<
        IteratorT,
        CategoryOrTraversal,
        chain_linkable_iterator_polymorphic<iterator_value_t<IteratorT>, DifferenceT>>
{
    using base_type = typename chain_linkable_iterator_polymorpic_adapter::adapter_base_t;
    using value_t = iterator_value_t<IteratorT>;
    using reference_t = polymorphic_iterator_proxy_t<value_t>;
    using proxy_backend_t = polymorphic_proxy_backend<value_t>;

public:
    using base_type::base_type;

    reference_t dereference() const override
    {
        return iterators::make_value_proxy<value_t>(static_cast<proxy_backend_t const*>(this));
    }

    value_t get_dereference() const override
    {
        return *this->base;
    }

    void set_dereference(value_t val) override
    {
        *this->base = std::move(val);
    }
};

template <typename CategoryOrTraversalT = use_default_t, typename DifferenceT = use_default_t, class IteratorT>
auto make_chain_linkable_iterator(IteratorT it)
{
    using categ_t = conditional_t<is_same_v<CategoryOrTraversalT, use_default_t>, iterator_category_t<IteratorT>, CategoryOrTraversalT>;
    using diff_t = conditional_t<is_same_v<DifferenceT, use_default_t>, iterator_difference_t<IteratorT>, DifferenceT>;
    using chain_linkable_iterator_t = chain_linkable_iterator<iterator_value_t<IteratorT>, categ_t, diff_t>;
    using adapter_t = chain_linkable_iterator_polymorpic_adapter<IteratorT, categ_t, diff_t>;
    return chain_linkable_iterator_t{in_place_type<adapter_t>, std::move(it)};
}

}

#endif // SONIA_UTILITY_ITERATORS_CHAIN_LINKABLE_ITERATOR_HPP
