// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_UNIQUE_LOCK_HPP
#define AGNOSTIC_STD_UNIQUE_LOCK_HPP

#pragma once

namespace std {

template <class MutexT>
class unique_lock
{
public:
    using mutex_type = MutexT;

    unique_lock() noexcept : pmtx_{ nullptr }, acquired_{ false } {}

    unique_lock(unique_lock&& other) noexcept : pmtx_{ other.pmtx_ } { other.pmtx_ = nullptr; }

    explicit unique_lock(mutex_type& m) : pmtx_{ std::addressof(m) }, acquired_{ true } { m.lock(); }

    unique_lock(mutex_type& m, defer_lock_t) noexcept : pmtx_{ &m }, acquired_{ false } {}

    unique_lock(mutex_type& m, try_to_lock_t t) : pmtx_{ &m } { acquired_ = pmtx_->try_lock(); }

    unique_lock(mutex_type& m, adopt_lock_t t) : pmtx_{ &m }, acquired_{ true } {}

    //template <class Rep, class Period>
    //unique_lock(mutex_type& m, const chrono::duration<Rep, Period>& timeout_duration);

    //template <class Clock, class Duration>
    //unique_lock(mutex_type& m, const std::chrono::time_point<Clock, Duration>& timeout_time);

    ~unique_lock()
    {
        if (acquired_) {
            pmtx_->unlock();
        }
    }

    void lock()
    {
        assert(pmtx_);
        assert(!acquired_);
        pmtx_->lock();
        acquired_ = true;
    }

    bool try_lock()
    {
        assert(pmtx_);
        assert(!acquired_);
        acquired_ = pmtx_->try_lock();
        return acquired_;
    }

    void unlock()
    {
        assert(pmtx_);
        assert(acquired_);
        pmtx_->ulock();
        acquired_ = false;
    }

    bool owns_lock() const noexcept { return acquired_; }

    explicit operator bool() const noexcept { return acquired_; }

private:
    mutex_type* pmtx_;
    bool acquired_;
};

}

#endif // AGNOSTIC_STD_UNIQUE_LOCK_HPP
