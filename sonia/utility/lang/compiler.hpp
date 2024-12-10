//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/concurrency.hpp"
#include "sonia/variant.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/functional/hash.hpp"
#include "sonia/utility/object_pool.hpp"

#include <vector>

#include <boost/intrusive/unordered_set.hpp>

namespace sonia::lang::detail {

template <typename SetT>
class bucket_set
{
    bucket_set(bucket_set const&) = delete;
    bucket_set& operator= (bucket_set const&) = delete;

public:
    using bucket_set_type = bucket_set;
    using set_t = SetT ;

    bucket_set() = default;

    inline explicit bucket_set(size_t sz) : buckets_{ sz } {}

    inline bucket_set(bucket_set&& rhs) noexcept : buckets_{ std::move(rhs.buckets_) } {}
    
    inline void operator= (bucket_set&& rhs) noexcept { buckets_ = std::move(rhs.buckets_); }

    inline typename SetT::bucket_type* buckets() noexcept { return &buckets_.front(); }

    inline void resize_buckets(size_t sz) { buckets_.resize(sz); }

    size_t size() const { return buckets_.size(); }

private:
    std::vector<typename SetT::bucket_type> buckets_;
};

}

namespace sonia::lang {

class compiler_worker_id
{
    fibers::fiber::id id_;

public:
    inline compiler_worker_id() : id_{ this_fiber::get_id() } {}

    friend inline bool operator== (compiler_worker_id const& l, compiler_worker_id const& r) noexcept
    {
        return l.id_ == r.id_;
    }

    friend inline auto operator<=>(compiler_worker_id const& l, compiler_worker_id const& r) noexcept
    {
        return l.id_ <=> r.id_;
    }

    friend inline size_t hash_value(compiler_worker_id const& v) noexcept
    {
        return hash_value(v.id_);
    }
};

class compiler_task_id : public polymorphic_clonable_and_movable
{
public:
    virtual ~compiler_task_id() = default;
    virtual bool equal(compiler_task_id const&) const noexcept = 0;
    virtual size_t hash() const noexcept = 0;

    friend inline bool operator== (compiler_task_id const& l, compiler_task_id const& r) noexcept
    {
        return l.equal(r);
    }

    friend inline size_t hash_value(compiler_task_id const& v) noexcept
    {
        return v.hash();
    }
};

class compiler_task_tracer
{
    using set_hook_type = boost::intrusive::unordered_set_member_hook<>;

    struct compiler_task
    {
        set_hook_type member_hook_;

        automatic_polymorphic<compiler_task_id, 2 * sizeof(void*)> id_;
        compiler_worker_id worker_id;
        mutable fibers::mutex mtx_;
        std::atomic_long refs{ 1 };

        inline compiler_task(compiler_task_id const& tid, compiler_worker_id wid) noexcept
            : id_{ tid }
            , worker_id{ std::move(wid) }
        {}

        compiler_task(compiler_task const&) = delete;
        compiler_task& operator=(compiler_task const&) = delete;

        inline void lock() { mtx_.lock(); }
        inline void unlock() { mtx_.unlock(); }

        inline void add_ref() { ++refs; }
        inline bool release_ref() { return refs.fetch_sub(1) == 1; }
        inline bool no_refs() const noexcept { return refs.load() == 0; }

        friend inline bool operator== (compiler_task const& l, compiler_task const& r) noexcept
        {
            return *l.id_ == *r.id_;
        }

        friend inline size_t hash_value(compiler_task const& v) noexcept
        {
            return hash_value(*v.id_);
        }
    };

    struct compiler_task_comparer
    {
        inline bool operator()(compiler_task const& l, compiler_task_id const& r) const noexcept { return l.id_ == r; }
        inline bool operator()(compiler_task_id const& l, compiler_task const& r) const noexcept { return l == r.id_; }
    };

    using task_set_t = boost::intrusive::unordered_set<
        compiler_task,
        boost::intrusive::member_hook<compiler_task, set_hook_type, &compiler_task::member_hook_>
    >;
    detail::bucket_set<task_set_t> buckets_;
    task_set_t tasks_;
    
    object_pool<compiler_task> task_pool_;
    fibers::mutex tasks_mtx_;

public:
    class task_guard
    {
        compiler_task_tracer* ptracer_;
        compiler_task* ptask_;

    public:
        inline task_guard(compiler_task_tracer& tr, compiler_task* pt) noexcept : ptracer_{ &tr }, ptask_{ pt } {}

        task_guard(task_guard const&) = delete;

        inline task_guard(task_guard&& r) noexcept : ptracer_{ r.ptracer_ }, ptask_{ r.ptask_ } { r.ptask_ = nullptr; }

        task_guard& operator=(task_guard const&) = delete;

        inline task_guard& operator=(task_guard&& r) noexcept
        {
            if (ptask_) ptracer_->release_task(*ptask_);
            ptracer_ = r.ptracer_;
            ptask_ = r.ptask_;
            r.ptask_ = nullptr;
        }

        inline ~task_guard()
        {
            if (ptask_) ptracer_->release_task(*ptask_);
        }

        inline explicit operator bool() const noexcept { return !!ptask_; }
    };

    inline compiler_task_tracer(size_t bucket_count = 64)
        : buckets_{ bucket_count }
        , tasks_{ typename task_set_t::bucket_traits(buckets_.buckets(), buckets_.size()) }
        , task_pool_{ bucket_count, bucket_count }
    {}

    //variant<primary_locked_t, secondary_locked_t, circular_t>
    task_guard try_lock_task(compiler_task_id const& tid, compiler_worker_id wid)
    {
        compiler_task* task;
        {
            lock_guard lck(tasks_mtx_);
            task_set_t::insert_commit_data insert_data;
            auto pair = tasks_.insert_check(tid, hasher{}, compiler_task_comparer{}, insert_data);
            if (pair.second) {
                task = task_pool_.new_object(tid, wid);
                tasks_.insert_commit(*task, insert_data);
                task->lock();
                return { *this, task };
            }
            /*
            auto it = tasks_.find(tid, hasher{}, compiler_task_comparer{});
            if (it == tasks_.end()) {
                task = task_pool_.new_object(tid, wid);
                tasks_.insert((it, *task);
                task->lock();
                return { *this, task };
            }
            */
            if (pair.first->worker_id == wid) { // circular run
                return { *this, nullptr };
            }
            task = &*pair.first;
            task->add_ref();
        }
        task->lock();
        task->worker_id = wid;
        return { *this, task };
    }

    void release_task(compiler_task& t)
    {
        t.unlock();
        if (t.release_ref()) {
            lock_guard lck(tasks_mtx_);
            if (t.no_refs()) {
                tasks_.erase(tasks_.iterator_to(t));
                task_pool_.delete_object(&t);
            }
        }
    }
};

}
