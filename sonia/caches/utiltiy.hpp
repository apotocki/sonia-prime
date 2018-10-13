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

#include "sonia/thread.hpp"
#include "sonia/functional/hash.hpp"
#include "sonia/utility/explicit_operator_bool.hpp"

namespace sonia { namespace cashe {

template <class CacheT>
class handle {
    CacheT * cache_;
    typename CacheT::item_type * item_;
    bool cache_miss_{false};

public:
    handle() : item_(nullptr) {}

    handle(CacheT & c, typename CacheT::item_type & i, bool cache_miss = false) noexcept
        : cache_(&c), item_(&i), cache_miss_(cache_miss)
    {
        ++item_->refs;
    }

    handle(handle const& rhs) noexcept
        : cache_(rhs.cache_), item_(rhs.item_)
    {
        if (item_) {
            ++item_->refs;
        }
    }

    handle(handle && rhs) noexcept
        : cache_(rhs.cache_), item_(rhs.item_)
    {
        rhs.item_ = nullptr;
    }

    handle& operator=(handle const& rhs) noexcept {
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

    handle& operator=(handle && rhs) noexcept {
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

    ~handle() noexcept {
        if (item_) {
            cache_->release(*item_);
        }
    }

    bool operator!() const noexcept { return !item_; }

    BOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL();

    ValueT & value() noexcept { return item_->value; }
    bool is_miss() const noexcept { return cache_miss_; }
};

template <typename BucketT>
class buckets {
public:
    buckets() = default;

    buckets(buckets const&) = delete;
    buckets(buckets &&) = default;
    buckets& operator=(buckets const&) = delete;
    buckets& operator=(buckets &&) = default;

    explicit bucket(size_t sz) 
        : buckets_(sz)
    {}

    BucketT * buckets() { return &buckets_.front(); }

    void resize_buckets(size_t sz) { buckets_.resize(sz); }

    size_t size() const { return buckets_.size(); }

private:
    std::vector<BucketT> buckets_;
};

template <typename CacheT>
class hash_partition {
    typedef typename CacheT::item_type item_type;
    typedef typename CacheT::key_type key_type;
    typedef typename item_type::key_hook_type key_hook_type;
    typedef boost::intrusive::unordered_set<
          ItemT
        , boost::intrusive::member_hook<item_type, key_hook_type, &item_type::key_hook>
    > set_t;
    typedef buckets<typename set_t::bucket_type> buckets_t;
    typedef typename boost::call_traits<typename AlgoT::key_type>::param_type key_param_type;

public:
    hash_partition(hash_partition const&) = delete;
    hash_partition(hash_partition &&) = delete;
    hash_partition& operator= (hash_partition const&) = delete;
    hash_partition& operator= (hash_partition &&) = delete;

    explicit hash_partition(size_t bucket_count)
        : buckets_(bucket_count)
        , set_(typename set_t::bucket_traits(buckets_.buckets(), buckets_.size()))
    {}

    handle<CacheT> try_get(CacheT & c, key_param_type key) {
        auto guard = make_lock_guard(mtx_);
        auto it = set_.find(key);
        if (it != set_.end()) {
            return handle<CacheT>(c, *it);
        }
        return {};
    }

private:
    buckets_t buckets_;
    set_t set_;
    mutable spinlock mtx_;
};

template <class DerivedT, class AlgoT>
class cache_facade : AlgoT {
    typedef typename AlgoT::item item_type;
    typedef hash_partition<item_type> partition_type;
    typedef typename AlgoT::key_type key_type;
    typedef typename boost::call_traits<typename AlgoT::key_type>::param_type key_param_type;
    typedef handle<item_type> handle_type;

    // prototype properties
    size_t get_partion_count() const { return 1; }
    size_t get_buckets_per_partition() const { return 97; }

public:
    void open();

    template <typename ... ArgsT>
    handle_type lookup(key_param_type key, ArgsT&& ... args);

private:
    partition_type const& get_partition(key_param_type key) const noexcept {
        size_t pidx = hash<key_type>()(key) % partitions_.size();
        return partitions_[pidx];
    }

    DerivedT & derived() noexcept { return static_cast<DerivedT&>(*this); }
    DerivedT const& derived() const noexcept { return static_cast<DerivedT const&>(*this); }

    std::vector<partition_type> partitions_;
};

template <class DerivedT, class AlgoT>
void cache_facade<DerivedT, AlgoT>::open() {
    auto pcount = derived().get_partion_count();
    auto buckets_per_partition = derived().get_buckets_per_partition();
    partitions_.reserve(pcount);
    while (partitions_.size() < pcount) {
        partitions_.emplace_back(buckets_per_partition);
    }
}

template <class DerivedT, class AlgoT>
template <typename ... ArgsT>
cache_facade<DerivedT, AlgoT>::handle_type cache_facade<DerivedT, AlgoT>::lookup(key_param_type key, ArgsT&& ... args) {
    partition_type & partition = get_partition(key);
    handle_type h = partition.try_get(*this, key);
    if (h) return h;
}

}}

#endif // SONIA_CACHES_UTILITY_HPP
