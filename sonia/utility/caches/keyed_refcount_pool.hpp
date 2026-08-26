//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/intrusive/unordered_set.hpp>
#include "sonia/concurrency.hpp"
#include "sonia/utility/object_pool.hpp"

#include "bucket_set.hpp"

namespace sonia::keyed_refcount_pool_detail {

template <typename KeyT, typename ValueT, typename HashT, typename EqualT>
struct element
{
    boost::intrusive::unordered_set_member_hook<> set_hook;
    KeyT key;
    ValueT value;
    uint32_t refs;

    template <typename KeyInitializerT, typename ... InitializersT>
    explicit element(KeyInitializerT&& key_init, InitializersT&& ... inits) 
        : key(std::forward<KeyInitializerT>(key_init))
        , value(std::forward<InitializersT>(inits)...)
        , refs(1)
    {}

    element(element const&) = delete;
    element& operator= (element const&) = delete;

    inline friend bool operator== (element const& a, element const& b) noexcept
    {
        return EqualT{}(a.key, b.key);
    }

    inline friend std::size_t hash_value(element const& e) noexcept
    {
        return HashT{}(e.key);
    }
};

struct element_comparer
{
    template <typename T, typename KeyT, typename ValueT, typename HashT, typename EqualT>
    inline bool operator()(T const& key, element<KeyT, ValueT, HashT, EqualT> const& e) const noexcept
    {
        return EqualT{}(key, e.key);
    }

    template <typename T, typename KeyT, typename ValueT, typename HashT, typename EqualT>
    inline bool operator()(element<KeyT, ValueT, HashT, EqualT> const& e, T const& key) const noexcept
    {
        return EqualT{}(e.key, key);
    }
};

}

namespace sonia {

template <typename KeyT, typename ValueT, typename HashT, typename EqualT>
class keyed_refcount_pool  : private bucket_set<
    boost::intrusive::unordered_set<
          keyed_refcount_pool_detail::element<KeyT, ValueT, HashT, EqualT>
        , boost::intrusive::member_hook<
              keyed_refcount_pool_detail::element<KeyT, ValueT, HashT, EqualT>
            , boost::intrusive::unordered_set_member_hook<>
            , &keyed_refcount_pool_detail::element<KeyT, ValueT, HashT, EqualT>::set_hook
        >
    >
>
{
    using bucket_set_type = typename keyed_refcount_pool::bucket_set_type;
    using set_t = typename keyed_refcount_pool::set_t;

public:
    using element_type = keyed_refcount_pool_detail::element<KeyT, ValueT, HashT, EqualT>;
    using key_param_type = typename boost::call_traits<KeyT>::param_type;

    explicit keyed_refcount_pool(size_t buckets = 17, size_t reserve_size = 32)
        : bucket_set_type{ buckets }
        , pool_{ reserve_size }
        , set_{ typename set_t::bucket_traits(this->buckets(), buckets) }
    {

    }

    template <typename ... InitializersT>
    element_type* acquire(key_param_type k, InitializersT&& ... initializers)
    {
        element_type * result;
        lock_guard guard(mtx_);
        typename set_t::insert_commit_data data;

        auto itpair = set_.insert_check(k, HashT{}, keyed_refcount_pool_detail::element_comparer{}, data);

        if (!itpair.second) {
            result = &*itpair.first;
            ++result->refs;
        } else {
            result = pool_.new_object(k, std::forward<InitializersT>(initializers)...);
            try {
                set_.insert_commit(*result, data);
            } catch (...) {
                pool_.delete_object(result);
                throw;
            }
        }
        return result;
    }

    void release(element_type *e)
    {
        lock_guard guard(mtx_);
        if (--e->refs == 0) {
            set_.erase(*e);
            pool_.delete_object(e);
        }
    }

private:
    object_pool<element_type> pool_;
    set_t set_;
    fibers::mutex mtx_;
};

}
