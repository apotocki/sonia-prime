//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_SIMPLE_QUEUE_HPP
#define SONIA_UTILITY_SIMPLE_QUEUE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <memory>
#include <mutex>
#include "sonia/optional.hpp"
#include "sonia/array_view.hpp"
#include "sonia/type_traits.hpp"

namespace sonia {

template <typename ElementT, class MutexT, class AllocatorT = std::allocator<ElementT>>
class simple_queue : private AllocatorT {
    typedef std::lock_guard<MutexT> guard_type;
    typedef std::conditional_t<is_pointer_v<ElementT>, ElementT, optional<ElementT>> opt_t;
public:
    template <typename ... ArgsT>
    explicit simple_queue(size_t capacity, ArgsT && ... args)
        : capacity_(capacity), AllocatorT(std::forward<ArgsT>(args) ...)
    {
        slots_ = AllocatorT::allocate(capacity);
    }

    ~simple_queue() {
        if constexpr (is_pod_v<ElementT>) {
            AllocatorT::deallocate(slots_, capacity_);
        } else {
            destruct_(slots_);
        }
    }

    simple_queue(simple_queue const&) = delete;
    simple_queue & operator=(simple_queue const&) = delete;

    bool empty() const noexcept {
        guard_type lk(lk_);
        return is_empty_();
    }

    template <typename T>
    void push(T && arg) {
        guard_type lk(lk_);
        if (is_full_()) {
            resize_();
        }
        new (slots_ + pidx_) ElementT(std::forward<T>(arg));
        pidx_ = (pidx_ + 1) % capacity_;
    }

    opt_t pop() {
        opt_t r = init_opt_();
        guard_type lk(lk_);
        if (!is_empty_()) {
            r = std::move(slots_[cidx_]);
            slots_[cidx_].~ElementT();
            cidx_ = (cidx_ + 1) % capacity_;
        }
        return std::move(r);
    }

    template <typename PredicateT>
    opt_t take_if(PredicateT const& pr) {
        opt_t r = init_opt_();
        guard_type lk(lk_);
        for (size_t i = cidx_; i != pidx_; i = (i + 1) % capacity_) {
            ElementT & c = slots_[i];
            if (!pr(c)) continue;
            r = std::move(c);
            if (cidx_ != i) {
                c = std::move(slots_[cidx_]);
            }
            slots_[cidx_].~ElementT();
            cidx_ = (cidx_ + 1) % capacity_;
            break;
        }
        return std::move(r);
    }

private:
    static opt_t init_opt_() {
        if constexpr (is_pointer_v<ElementT>) {
            return nullptr;
        } else {
            return opt_t{};
        }
    }

    bool is_full_() const noexcept {
        return cidx_ == ((pidx_ + 1) % capacity_);
    }

    bool is_empty_() const noexcept {
        return cidx_ == pidx_;
    }

    void resize_() {
        ElementT * old_slots = slots_;
        slots_ = AllocatorT::allocate(2 * capacity_);
        size_t offset = capacity_ - cidx_;

        if constexpr (is_pod_v<ElementT>) {
            std::memcpy(slots_, old_slots + cidx_, offset * sizeof(ElementT));
            if ( 0 < cidx_) {
                std::memcpy(slots_ + offset, old_slots, pidx_ * sizeof( ElementT) );
            }
            AllocatorT::deallocate(old_slots, capacity_);
        } else {
            ElementT * d = slots_;
            for (ElementT * s = old_slots + cidx_, * es = old_slots + capacity_; s != es; ++s, ++d) {
                new (d) ElementT(std::move(*s));
            }
            if (0 < cidx_) {
                for (ElementT * s = old_slots, * es = old_slots + pidx_; s != es; ++s, ++d) {
                    new (d) ElementT(std::move(*s));
                }
            }
            destruct_(slots_);
        }

        cidx_ = 0;
        pidx_ = capacity_ - 1;
        capacity_ *= 2;
    }

    void destruct_(ElementT * slots) {
        for (size_t i = cidx_; i != pidx_; i = (i + 1) % capacity_) {
            slots[i].~ElementT();
        }
        AllocatorT::deallocate(slots, capacity_);
    }

private:
    mutable MutexT lk_{};
    size_t pidx_{0};
    size_t cidx_{0};
    size_t capacity_;
    ElementT * slots_;
};

}

#endif // SONIA_UTILITY_SIMPLE_QUEUE_HPP
