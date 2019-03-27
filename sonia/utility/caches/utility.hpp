//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CACHES_UTILITY_HPP
#define SONIA_CACHES_UTILITY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>

#include <boost/call_traits.hpp>
#include <boost/intrusive/unordered_set.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/utility/functional/hash.hpp"
#include "sonia/exceptions/internal_errors.hpp"

namespace sonia::cache {

template <class CacheT>
class item_handle
{
    CacheT * cache_;
    typename CacheT::item_type * item_{nullptr};
    bool cache_miss_{false};

public:
    item_handle() = default;

    item_handle(CacheT & c, typename CacheT::item_type & i, bool cache_miss = false) noexcept
        : cache_(&c), item_(&i), cache_miss_(cache_miss)
    {
        ++item_->refs;
    }

    item_handle(item_handle const& rhs) noexcept
        : cache_(rhs.cache_), item_(rhs.item_)
    {
        if (item_) {
            ++item_->refs;
        }
    }

    item_handle(item_handle && rhs) noexcept
        : cache_(rhs.cache_), item_(rhs.item_)
    {
        rhs.item_ = nullptr;
    }

    item_handle& operator=(item_handle const& rhs) noexcept
    {
        if (item_ != rhs.item_) {
            if (item_) {
                cache_->release(*item_);
            }
            cache_ = rhs.cache_;
            item_ = rhs.item_;
            if (item_) {
                ++item_->refs;
            }
        }
        return *this;
    }

    item_handle& operator=(item_handle && rhs) noexcept
    {
        if (item_ != rhs.item_) {
            if (item_) {
                cache_->release(*item_);
            }
            cache_ = rhs.cache_;
            item_ = rhs.item_;
            rhs.item_ = nullptr;
        }
        return *this;
    }

    ~item_handle() noexcept
    {
        if (item_) {
            cache_->release(*item_);
        }
    }

    explicit operator bool() const noexcept { return !!item_; }

    auto & value() noexcept { return item_->value; }
    bool is_miss() const noexcept { return cache_miss_; }
};

template <typename BucketT>
class buckets
{
public:
    buckets() = default;

    buckets(buckets const&) = delete;
    buckets(buckets &&) = default;
    buckets& operator=(buckets const&) = delete;
    buckets& operator=(buckets &&) = default;

    explicit buckets(size_t sz) 
        : buckets_(sz)
    {}

    ~buckets() = default;

    BucketT * get_buckets() { return &buckets_.front(); }

    void resize_buckets(size_t sz) { buckets_.resize(sz); }

    size_t size() const { return buckets_.size(); }

private:
    std::vector<BucketT> buckets_;
};

template <typename CacheT>
class hash_partition
{
    using item_type = typename CacheT::item_type ;
    using key_type = typename CacheT::key_type;
    using key_hook_type = typename item_type::key_hook_type;
    using set_t = boost::intrusive::unordered_set<
          item_type
        , boost::intrusive::member_hook<item_type, key_hook_type, &item_type::key_hook>
    >;
    using buckets_t = buckets<typename set_t::bucket_type>;
    using key_param_type = typename boost::call_traits<key_type>::param_type;

public:
    hash_partition(hash_partition const&) = delete;
    hash_partition(hash_partition &&) = delete;
    hash_partition& operator= (hash_partition const&) = delete;
    hash_partition& operator= (hash_partition &&) = delete;

    explicit hash_partition(size_t bucket_count)
        : buckets_(bucket_count)
        , set_(typename set_t::bucket_traits(buckets_.get_buckets(), buckets_.size()))
    {}

    item_handle<CacheT> try_get(CacheT & c, key_param_type key)
    {
        lock_guard guard(mtx_);
        auto it = set_.find(key);
        if (it != set_.end()) {
            return item_handle<CacheT>(c, *it);
        }
        return {};
    }

private:
    buckets_t buckets_;
    set_t set_;
    mutable spin_mutex mtx_;
};

template <class DerivedT, class AlgoT>
class cache_facade : AlgoT
{
    using item_type = typename AlgoT::item;
    using partition_type = hash_partition<item_type>;
    using key_type = typename AlgoT::key_type;
    using key_param_type = typename boost::call_traits<typename AlgoT::key_type>::param_type;
    using handle_type = item_handle<item_type>;

    // prototype properties
    size_t get_partion_count() const { return 1; }
    size_t get_buckets_per_partition() const { return 97; }

public:
    void open();

    template <typename ... ArgsT>
    handle_type lookup(key_param_type key, ArgsT&& ... args);

private:
    partition_type const& get_partition(key_param_type key) const noexcept
    {
        size_t pidx = hash<key_type>()(key) % partitions_.size();
        return partitions_[pidx];
    }

    DerivedT & derived() noexcept { return static_cast<DerivedT&>(*this); }
    DerivedT const& derived() const noexcept { return static_cast<DerivedT const&>(*this); }

    std::vector<partition_type> partitions_;
};

template <class DerivedT, class AlgoT>
void cache_facade<DerivedT, AlgoT>::open()
{
    auto pcount = derived().get_partion_count();
    auto buckets_per_partition = derived().get_buckets_per_partition();
    partitions_.reserve(pcount);
    while (partitions_.size() < pcount) {
        partitions_.emplace_back(buckets_per_partition);
    }
}

template <class DerivedT, class AlgoT>
template <typename ... ArgsT>
typename cache_facade<DerivedT, AlgoT>::handle_type cache_facade<DerivedT, AlgoT>::lookup(key_param_type key, ArgsT&& ... args)
{
    partition_type & partition = get_partition(key);
    handle_type h = partition.try_get(*this, key);
    if (h) return h;
    THROW_NOT_IMPLEMENTED_ERROR();
}

}

#endif // SONIA_CACHES_UTILITY_HPP
