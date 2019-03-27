//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OBJECT_POOL_HPP
#define SONIA_UTILITY_OBJECT_POOL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include <boost/pool/pool.hpp>
#include <boost/unordered_set.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia {

template <
    typename T,
    typename MutexT = dummy_mutex_t,
    typename AllocatorT = boost::default_user_allocator_new_delete
>
class object_pool
    : private boost::pool<AllocatorT>
    , private MutexT
{
    object_pool(object_pool const&) = delete;
    object_pool& operator= (object_pool const&) = delete;

    typedef boost::pool<AllocatorT> pool_t;

public:
    explicit object_pool(size_t start_reserve_size, size_t max_reserve_size = 0)
        : pool_t(sizeof(T), start_reserve_size, max_reserve_size)
    {

    }

    template <typename ... ArgsT>
    T * new_object(ArgsT&& ... args)
    {
        T * result;
        {
            lock_guard<MutexT> guard(*this);
            result = reinterpret_cast<T*>(do_malloc());
            if (!result) {
                throw internal_error(fmt("out of memory, can't allocate a space for '%1%' (%2% bytes)") % typeid(T).name() % sizeof(T));
            }
        }
        try {
            return new (result) T(std::forward<ArgsT>(args) ...);
        } catch (...) {
            lock_guard<MutexT> guard(*this);
            do_free(result);
            throw;
        }
    }

#ifndef NDEBUG
    ~object_pool()
    {
        if (used_.size()) {
            GLOBAL_LOG_FATAL() << "An attempt to delete not empty pool<" << typeid(T).name() << ">, size: " << used_.size();
        }
    }

    void delete_object(T * obj) noexcept
    {
        lock_guard<MutexT> guard(*this);
        if (!used_.erase(obj)) {
            GLOBAL_LOG_FATAL() << "an attempt to free a not initialized object '" << typeid(T).name() << "' (" << sizeof(T) << " bytes)";
            return;
        }
        obj->~T();
        pool_t::free(obj);
    }

private:
    boost::unordered_set<T*> used_;

    void * do_malloc()
    {
        T * result = std::launder(reinterpret_cast<T*>(pool_t::malloc()));
        BOOST_VERIFY (result && used_.insert(result).second);
        return result;
    }

    void do_free(T * p)
    {
        BOOST_ASSERT(used_.erase(p));
        pool_t::free(p);
    }
#else
    void delete_object(T * obj) noexcept
    {
        lock_guard<MutexT> guard(*this);
        obj->~T();
        pool_t::free(obj);
    }

private:
    void * do_malloc() { return pool_t::malloc(); }
    void do_free(void * p) { pool_t::free(p); }
#endif
};

}

#endif // SONIA_UTILITY_OBJECT_POOL_HPP
