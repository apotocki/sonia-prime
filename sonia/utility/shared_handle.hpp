//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <atomic>
#include <numeric>

#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia {

/**
see HandleTraitsT prototype:
struct handle_traits
{
    using base_type = unspecified_type; // used to enable polymorphic destruction

    template <typename ... ServiceT>
    void close(ServiceT && ..., shared_handle *);
    
    template <typename ... ServiceT>
    void free(ServiceT && ..., shared_handle *);
};
*/

template <class HandleTraitsT>
struct shared_handle : HandleTraitsT::base_type
{
    std::atomic<uintptr_t> refs_{0};
    static constexpr uintptr_t weakunit = ((uintptr_t)1) << (sizeof(uintptr_t) * 4);
    static constexpr uintptr_t refs_mask = weakunit - 1;

    shared_handle() = default;
    virtual ~shared_handle() = default;

    shared_handle(shared_handle const&) = delete;
    shared_handle(shared_handle &&) = delete;
    shared_handle& operator= (shared_handle const&) = delete;
    shared_handle& operator= (shared_handle &&) = delete;

    uintptr_t add_ref() { return ++refs_; }
    void add_weakref() { refs_.fetch_add(weakunit); }

    template <typename ... ServiceT>
    void release(ServiceT && ... s)
    {
        const uintptr_t rval = refs_.fetch_sub(1) - 1;
        if (0 == (rval & refs_mask)) {
            HandleTraitsT::close(std::forward<ServiceT>(s) ..., this);
        }
        if (!rval) {
            HandleTraitsT::free(std::forward<ServiceT>(s) ..., this);
        }
    }

    template <typename ... ServiceT>
    void release_weak(ServiceT && ... s)
    {
        if (weakunit == refs_.fetch_sub(weakunit)) {
            HandleTraitsT::free(std::forward<ServiceT>(s) ..., this);
        }
    }
};

/*
template <class ServiceT, class SharedHandleT>
class scoped_handle
{
public:
    explicit scoped_handle(ServiceT s, SharedHandleT * sh)
        : s_(s), sh_(static_cast<SharedHandleT*>(sh->lock()))
    {
        if (!sh_) throw eof_exception("socket is closed");
    }

    template <class HandleTraitsT>
    explicit scoped_handle(ServiceT s, shared_handle<HandleTraitsT> * sh)
        : scoped_handle(s, static_cast<SharedHandleT*>(sh))
    {}

    scoped_handle(scoped_handle const&) = delete;
    scoped_handle& operator=(scoped_handle const&) = delete;

    ~scoped_handle()
    {
        if (sh_) sh_->release(s_);
    }

    SharedHandleT * operator-> () const { return sh_; }
    SharedHandleT * get() const { return sh_; }

private:
    ServiceT s_;
    SharedHandleT * sh_;
};
*/

}
