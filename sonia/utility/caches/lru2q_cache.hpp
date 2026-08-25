//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <atomic>

#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/parent_from_member.hpp>

#include <boost/call_traits.hpp>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"

#include "bucket_set.hpp"
#include "keyed_refcount_pool.hpp"

namespace sonia::lru2q_detail {

enum class queue_state : uint8_t
{
    not_in_queue = 0,
    in_queue,
    out_queue,
    lru_queue,
    removing
};

enum class lru2q_provenance
{
    existing = 1,
    created
};

template <typename ElementT, typename TraitsT>
struct cache_elem_hasher
{
    using key_type = typename TraitsT::key_type;
    using key_param_type = typename boost::call_traits<key_type>::param_type;
    using hash_type = typename TraitsT::hash_type;

    inline size_t operator()(key_param_type s) const noexcept { return hash_type{}(s); }
    inline size_t operator()(ElementT const& d) const noexcept { return hash_type{}(key(d)); }
};

template <typename ElementT, typename TraitsT>
struct cache_elem_comparer
{
    using key_type = typename TraitsT::key_type;
    using key_param_type = typename boost::call_traits<key_type>::param_type;

    inline bool operator()(key_param_type k, ElementT const& s) const noexcept { return k == key(s); }
    inline bool operator()(ElementT const& s, key_param_type k) const noexcept { return key(s) == k; }
    inline bool operator()(ElementT const& l, ElementT const& r) const noexcept { return key(l) == key(r); }
};

template <typename CacheT>
class cached_value
{
    using KeyT = typename CacheT::key_type;
    using ValueT = typename CacheT::value_type;
    using element_type = typename CacheT::element_type;
    using traits_type = typename CacheT::traits_type;

public:
    inline cached_value(CacheT* cache, element_type* element) noexcept
        : cache_(cache), element_(element)
    {}

    inline cached_value(const cached_value& rhs)
        : cache_(rhs.cache_), element_(rhs.element_)
    {
        if (element_) {
            add_ref_fetch(element_);
        }
    }

    inline cached_value(cached_value&& rhs) noexcept
        : cache_(rhs.cache_), element_(rhs.element_)
    {
        rhs.element_ = nullptr;
    }

    inline cached_value& operator=(const cached_value& rhs) noexcept
    {
        if (element_ != rhs.element_) {
            if (element_) {
                if (1 == fetch_release(element_)) {
                    cache_->release(element_);
                }
            }
            cache_ = rhs.cache_;
            element_ = rhs.element_;
            if (element_) {
                add_ref_fetch(element_);
            }
        }
        return *this;
    }

    inline cached_value& operator=(cached_value&& rhs) noexcept
    {
        if (element_ != rhs.element_) {
            if (element_) {
                if (1 == fetch_release(element_)) {
                    cache_->release(element_);
                }
            }
            cache_ = rhs.cache_;
            element_ = rhs.element_;
            rhs.element_ = nullptr;
        }
        return *this;
    }

    inline ~cached_value()
    {
        if (element_) {
            if (1 == fetch_release(element_)) {
                cache_->release(element_);
            }
        }
    }

    inline ValueT& value() const noexcept
    {
        return element_->value;
    }

    inline ValueT& operator*() const noexcept
    {
        return element_->value;
    }

    inline ValueT* operator->() const noexcept
    {
        return &element_->value;
    }

    inline KeyT const& key() const noexcept
    {
        return element_->key;
    }

private:
    CacheT* cache_;
    element_type* element_;
};

template <typename TraitsT>
class hash_partition
{
    hash_partition(hash_partition const&) = delete;
    hash_partition& operator=(hash_partition const&) = delete;

    using key_type = typename TraitsT::key_type;
    using value_type = typename TraitsT::value_type;
    using key_param_type = boost::call_traits<key_type>::param_type;

public:
    using set_hook_type = boost::intrusive::unordered_set_member_hook<>;
    using list_hook_type = boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>>;

    struct element
    {
        set_hook_type set_hook_;
        list_hook_type queue_hook_;
        key_type key;
        value_type value;
        std::atomic<size_t> refs = 1;
        std::atomic<size_t> pin_ = 0;
        queue_state state = queue_state::not_in_queue;
    };

    inline friend key_type const& key(element const& element) noexcept
    {
        return element.key;
    }

    inline friend auto add_ref_fetch(element* element) noexcept
    {
        return element->refs.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    inline friend auto fetch_release(element* element) noexcept
    {
        return element->refs.fetch_sub(1, std::memory_order_relaxed);
    }

    inline friend bool try_acquire_pin(element* elem) noexcept
    {
        size_t exp = 0;
        return elem->pin_.compare_exchange_weak(exp, 1, std::memory_order_relaxed);
    }

    inline friend void release_pin(element* elem) noexcept
    {
        BOOST_ASSERT(elem->pin_.load() == 1);
        elem->pin_.store(0);
    }

    struct set_hook_functor_type
    {
        using hook_type = set_hook_type;
        using hook_ptr = hook_type*;
        using const_hook_ptr = const hook_type*;
        using value_type = element;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        inline static hook_ptr to_hook_ptr(value_type& value) noexcept { return &value.set_hook_; }
        inline static const_hook_ptr to_hook_ptr(const value_type& value) noexcept { return &value.set_hook_; }
        inline static pointer to_value_ptr(hook_ptr n) noexcept { return boost::intrusive::get_parent_from_member<element>(n, &element::set_hook_); }
        inline static const_pointer to_value_ptr(const_hook_ptr n) noexcept { return boost::intrusive::get_parent_from_member<element>(n, &element::set_hook_); }
    };

    struct list_hook_functor_type
    {
        using hook_type = list_hook_type;
        using hook_ptr = hook_type*;
        using const_hook_ptr = const hook_type*;
        using value_type = element;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        static hook_ptr to_hook_ptr(value_type& value) { return &value.queue_hook_; }
        static const_hook_ptr to_hook_ptr(const value_type& value) { return &value.queue_hook_; }
        static pointer to_value_ptr(hook_ptr n) { return boost::intrusive::get_parent_from_member<element>(n, &element::queue_hook_); }
        static const_pointer to_value_ptr(const_hook_ptr n) { return boost::intrusive::get_parent_from_member<element>(n, &element::queue_hook_); }
    };

    using cache_elem_hasher_type = cache_elem_hasher<element, TraitsT>;
    using cache_elem_comparer_type = cache_elem_comparer<element, TraitsT>;

    using set_t = boost::intrusive::unordered_set<
          element
        , boost::intrusive::function_hook<set_hook_functor_type>
        , boost::intrusive::hash<cache_elem_hasher_type>
        , boost::intrusive::equal<cache_elem_comparer_type>
    >;

    using bucket_set_type = bucket_set<set_t>;

    using queue_t = boost::intrusive::list<
          element
        , boost::intrusive::function_hook<list_hook_functor_type>
        , boost::intrusive::constant_time_size<false>
    >;

    explicit hash_partition(size_t bucket_count)
        : buckets_(bucket_count)
        , set_(typename set_t::bucket_traits(buckets_.buckets(), buckets_.size()))
    {}

    hash_partition(hash_partition&& rhs) noexcept
        : buckets_(std::move(rhs.buckets_))
        , set_(std::move(rhs.set_))
    {}

    bool try_get(key_param_type key, element** result)
    {
        shared_lock_guard guard(set_mutex_);
        auto it = set_.find(key, cache_elem_hasher_type{}, cache_elem_comparer_type{});
        if (it != set_.end()) {
            *result = &*it;
            add_ref_fetch(*result);
            return true;
        }
        return false;
    }

    void insert(element& e) noexcept
    {
        lock_guard guard(set_mutex_);
        auto rpair = set_.insert(e);
        BOOST_ASSERT(rpair.second);
    }

    template <typename EraseFtorT>
    void erase(EraseFtorT const& ef)
    {
        while (!set_.empty()) {
            element* elem = &*set_.begin();
            set_.erase(*elem);
            try {
                ef(elem);
            } catch (...) {
                set_.insert(*elem);
                throw;
            }
        }
    }

    template <typename CheckFtorT>
    bool lock_check_erase(element& e, CheckFtorT const& ftor)
    {
        lock_guard guard(set_mutex_);
        if (ftor(e)) {
            BOOST_VERIFY(1 == set_.erase(e));
            return true;
        }
        return false;
    }

private:
    fibers::rw_mutex set_mutex_;
    bucket_set_type buckets_;
    set_t set_;
};

}

namespace sonia {

template <typename DerivedT, typename TraitsT>
class lru2q_cache
{
    friend class lru2q_detail::cached_value<lru2q_cache>;
    DerivedT& derived() { return static_cast<DerivedT&>(*this); }
    
protected:
    using traits_type = TraitsT;
    using partition_type = lru2q_detail::hash_partition<traits_type>;
    using queue_t = typename partition_type::queue_t;

    using key_type = typename traits_type::key_type;
    using value_type = typename traits_type::value_type;
    using element_type = typename partition_type::element;
    using key_param_type = typename boost::call_traits<key_type>::param_type;

    using product = lru2q_detail::cached_value<lru2q_cache>;

public:
    lru2q_cache(
        size_t partion_count = 1,
        size_t buckets_per_partition = 17,
        size_t dcl_buckets = 17,
        size_t dcl_pool_reserve_size = 32);

    ~lru2q_cache()
    {
        dispose();
    }

    void dispose();


    template <typename ... AuxParamsT>
    lru2q_detail::lru2q_provenance acquire(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params);

    product get_product(key_type const& key)
    {
        element_type* element = nullptr;
        acquire(key, &element);
        return product(this, element);
    }

    void release(element_type* elem);

    // prototype for derived class to implement
    size_t get_lru_quota() const;
    size_t get_in_quota() const;
    size_t get_out_quota() const;

protected:
    template <typename ... AuxParamsT>
    lru2q_detail::lru2q_provenance acquire0(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params);
    
    inline partition_type& get_partition(key_param_type key) noexcept
    {
        size_t pidx = typename traits_type::hash_type{}(key) % partitions_.size();
        return partitions_[pidx];
    }

    //inline partition_type const& get_partition(const key_type& key) const noexcept
    //{
    //    size_t pidx = typename traits_type::hash_type{}(key) % partitions_.size();
    //    return partitions_[pidx];
    //}

    element_type* put_in_queue(element_type*);
    element_type* remove_from_cache(element_type*);
    element_type* case_out_queue(element_type*);
    element_type* case_lru_queue(element_type*);
    void case_out_queue_no_lru(element_type*);

    
    // 
    std::vector<partition_type> partitions_;
    keyed_refcount_pool<key_type, fibers::mutex, typename traits_type::hash_type, typename traits_type::equal_type> dcl_mutexes_;
    size_t in_size_, out_size_;
    std::atomic<size_t> lru_size_;
    fibers::mutex fallback_queue_mtx_;
    std::atomic<size_t> fallback_queue_size_;
    queue_t fallback_queue_;
    //
    // perhaps somewhere here will be a new cache line for frequently changing variables
    fibers::mutex queue_mtx_;
    queue_t queue_;
    typename queue_t::iterator next_out_it_;

    fibers::mutex lru_queue_mtx_;
    queue_t lru_queue_;

    std::atomic<bool> filled_;
};


template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::lru2q_cache(size_t partion_count, size_t buckets_per_partition, size_t dcl_buckets, size_t dcl_pool_reserve_size)
    : dcl_mutexes_(dcl_buckets, dcl_pool_reserve_size)
{
    partitions_.reserve(partion_count);
    while (partitions_.size() < partion_count) {
        partitions_.emplace_back(buckets_per_partition);
    }
}

template <typename DerivedT, typename TraitsT>
template <typename ... AuxParamsT>
lru2q_detail::lru2q_provenance lru2q_cache<DerivedT, TraitsT>::acquire(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params)
{
    partition_type& partition = get_partition(key);
    if (partition.try_get(key, ppelement)) return lru2q_detail::lru2q_provenance::existing;
    return acquire0(key, ppelement, std::forward<AuxParamsT>(aux_params)...);
}

template <typename DerivedT, typename TraitsT>
template <typename ... AuxParamsT>
lru2q_detail::lru2q_provenance lru2q_cache<DerivedT, TraitsT>::acquire0(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params)
{
    partition_type& partition = get_partition(key);
    auto * dcl_mutex = dcl_mutexes_.acquire(key);

    SCOPE_EXIT([dcl_mutex, this]{
        dcl_mutexes_.release(dcl_mutex);
    });
    {
        lock_guard dcl_guard(dcl_mutex->value); // it's expected that locked location can not be removed from cache (if allocated)
        if (partition.try_get(key, ppelement)) return lru2q_detail::lru2q_provenance::existing;

        // a cache miss takes place
        for (;;) {
            if (0 == fallback_queue_size_.load()) {
                derived().do_create_value(key, ppelement, std::forward<AuxParamsT>(aux_params) ...);
                BOOST_ASSERT(*ppelement);
                partition.insert(**ppelement);
                return lru2q_detail::lru2q_provenance::created;
            }
            lock_guard guard(fallback_queue_mtx_);
            while (!fallback_queue_.empty()) {
                element_type * elem = &fallback_queue_.front();
                fallback_queue_.pop_front();
                try {
                    derived().do_dispose_value(elem); // can throw
                } catch (...) {
                    fallback_queue_.push_back(*elem);
                    throw;
                }
                --fallback_queue_size_;
            }
        }
    }
}

template <typename DerivedT, typename TraitsT>
void lru2q_cache<DerivedT, TraitsT>::release(element_type* elem)
{
    using namespace lru2q_detail;

    while (elem)
    {
        size_t r = fetch_release(elem);
        
        if (r > 1) {
            return; // there are other instancies
        }

        if (!r) {
            add_ref_fetch(elem);
            return;
        }

        // element doesn't change its queue position if requested during the time when a thread processing free_element for it
        if (!try_acquire_pin(elem)) {
            // processing at the moment, just skip
            return;
        }

        switch (elem->state)
        {
        case queue_state::not_in_queue: // just created, not in any queue, is going to be placed in "in" queue
            elem = put_in_queue(elem);
            break;
       
        case queue_state::removing:
            elem = remove_from_cache(elem);
            break;

        case queue_state::in_queue:
            release_pin(elem); // element has been handled, unpin
            return;

        case queue_state::out_queue:
            if (0 == derived().get_lru_quota()) {
                case_out_queue_no_lru(elem);
                elem = nullptr;
            } else {
                elem = case_out_queue(elem);
            }
            break;

        case queue_state::lru_queue:
            elem = case_lru_queue(elem);
            break;

        default:
            THROW_INTERNAL_ERROR("unknown lru2q state");
        }
    }
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::put_in_queue(element_type* elem)
{
    using namespace lru2q_detail;

    size_t in_quota = derived().get_in_quota();
    size_t out_quota = derived().get_out_quota();
    size_t lru_quota = derived().get_lru_quota();

    {
        unique_lock lock(queue_mtx_);
        queue_.push_back(*elem); // put in "in" queue
        
        if (in_size_ < in_quota) { // "in" queue has a free slot for the new element
            elem->state = queue_state::in_queue; // mark as placed in "in" queue
            release_pin(elem);
            ++in_size_;
            if (in_size_ == in_quota) { // it was the final slot
                next_out_it_ = queue_.begin(); // update reference on the front queue element
            }
            return nullptr;
        }

        if (next_out_it_ == queue_.end()) { // case 0 == in_quota
            BOOST_ASSERT(0 == in_quota);
            --next_out_it_; // set back position
        } else {
            elem->state = queue_state::in_queue; // mark as placed in "in" queue
        }
        release_pin(elem);

        // "in" queue is full
        //auto dlen = std::distance(next_out_it_, queue_.end());
        //BOOST_ASSERT((size_t)dlen == in_quota + 1);

        auto & e = *next_out_it_;
        while (!try_acquire_pin(&e)); // *next_out_it_ is in "in" queue => no blocking allows
        e.state = queue_state::out_queue;  // mark the front "in" queue element as an "out" queue element
        release_pin(&e);

        ++next_out_it_; // update reference on the front queue element of "in" queue
        ++out_size_;
        
        BOOST_ASSERT(next_out_it_ != queue_.end());
        
        for (;;)
        {
            if (out_size_ <= out_quota || (out_size_ + lru_size_.load()) < (out_quota + lru_quota)) return nullptr; // use lru quata if lru_size allows

            elem = &queue_.front();

            if (!try_acquire_pin(elem)) {
                // processing at the moment, just skip
                return nullptr;
            }
            queue_.pop_front();
            --out_size_;

            lock.unlock();
            elem->state = queue_state::removing;
            elem = remove_from_cache(elem);
            if (elem) return elem;
            lock.lock();
        }
    }
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::remove_from_cache(element_type* elem)
{
    filled_.store(true); // set flag

    partition_type & partition = get_partition(key(*elem));

    auto * dcl_mutex = dcl_mutexes_.acquire(key(*elem));

    SCOPE_EXIT([dcl_mutex, this]{
        dcl_mutexes_.release(dcl_mutex);
    });

    lock_guard dcl_guard(dcl_mutex->value);
    if (partition.lock_check_erase(*elem, [](element_type &e) {
        return 1 == add_ref_fetch(&e);
    })) {

        try {
            derived().do_dispose_value(elem); // can throw
        } catch (...) {
            lock_guard guard(fallback_queue_mtx_);
            fallback_queue_.push_back(*elem);
            ++fallback_queue_size_;
        }
        return nullptr;
    }

    // add reference was called by try_erase
    // very rare case (catching up while 'out' queue releasing)
    release_pin(elem); // unpin
    return elem; // repeat disposing
}

template <typename DerivedT, typename TraitsT>
void lru2q_cache<DerivedT, TraitsT>::case_out_queue_no_lru(element_type* elem)
{
    lock_guard guard(queue_mtx_);
    queue_.erase(queue_.s_iterator_to(*elem));
    queue_.push_back(*elem);
    elem->state = lru2q_detail::queue_state::in_queue;
    release_pin(elem);
    if (next_out_it_ != queue_.end()) {
        // don't care about pin status of *next_out_it_
        (*next_out_it_).state = lru2q_detail::queue_state::out_queue;  // mark the front "in" queue element as an "out" queue element
        ++next_out_it_;
    }
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::case_out_queue(element_type* elem)
{
    using namespace lru2q_detail;

    {
        lock_guard guard(queue_mtx_);
        queue_.erase(queue_.s_iterator_to(*elem));
        --out_size_;
    }

    element_type* elem2rm = nullptr;

    {
        lock_guard guard(lru_queue_mtx_);
        if (lru_size_.load() < derived().get_lru_quota()) {
            ++lru_size_;
            lru_queue_.push_back(*elem);
            elem->state = queue_state::lru_queue;
            release_pin(elem); // unpin
            return nullptr;
        }

        // try find elem to move in "in" queue
        for (auto elem_it = lru_queue_.begin(); elem_it != lru_queue_.end(); ++elem_it)
        {
            if (!try_acquire_pin(&*elem_it)) {
                // processing at the moment, just postpone, wiil be replaced in lru_queue case.
                continue;
            }
            elem2rm = &*elem_it;
            lru_queue_.erase(elem_it);
            elem2rm->state = queue_state::not_in_queue;

            lru_queue_.push_back(*elem);
            elem->state = queue_state::lru_queue;
            release_pin(elem); // unpin
            break;
        }
    }

    return put_in_queue(elem2rm ? elem2rm : elem);
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::case_lru_queue(element_type* elem)
{
    using namespace lru2q_detail;
    {
        lock_guard guard(lru_queue_mtx_);
        queue_state st = elem->state;
        BOOST_ASSERT(st == queue_state::lru_queue);
        //if (st == queue_state::lru_queue) {
        lru_queue_.erase(lru_queue_.s_iterator_to(*elem));
        lru_queue_.push_back(*elem);
        release_pin(elem); // unpin
        return nullptr;
        //}
        //BOOST_ASSERT(st == queue_state::not_in_queue);
    }
    return put_in_queue(elem);
}

template <typename DerivedT, typename TraitsT>
void lru2q_cache<DerivedT, TraitsT>::dispose()
{
    //size_t pnum = 0;
    for (partition_type& p : partitions_) {
        //LOG_INFO(logger()) << "disposing cache partition: " << pnum;
        p.erase([this](element_type* elem) {
            derived().do_dispose_value(elem);
        });
        //++pnum;
    }
    partitions_.clear();
}

}
