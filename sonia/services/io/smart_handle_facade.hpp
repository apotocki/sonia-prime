//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "sonia/shared_ptr.hpp"

namespace sonia {

template <template <class, class> class> class smart_handle_access;

template <class TraitsT, template <class, class> class HandleApiT>
class smart_handle_facade : public HandleApiT<smart_handle_facade<TraitsT, HandleApiT>, TraitsT>
{
    template <template <class, class> class> friend class smart_handle_access;
    friend class HandleApiT<smart_handle_facade, TraitsT>;

    using base_type = HandleApiT<smart_handle_facade, TraitsT>;
    
    smart_handle_facade(shared_ptr<typename base_type::service_type> impl, typename base_type::handle_type handle) noexcept
        : impl_(std::move(impl)), handle_(handle)
    {
        TraitsT::add_ref(handle_);
    }

public:
    smart_handle_facade() : handle_(TraitsT::not_initialized_v) {}

    smart_handle_facade(smart_handle_facade const& rhs)
        : smart_handle_facade(rhs.impl_, rhs.handle_) // ref incrementing is here
    {}

    smart_handle_facade(smart_handle_facade && rhs) noexcept
        : impl_(std::move(rhs.impl_)), handle_(rhs.handle_)
    {
        rhs.handle_ = TraitsT::not_initialized_v;
    }

    ~smart_handle_facade()
    {
        TraitsT::release(impl_.get(), handle_);
    }

    smart_handle_facade& operator= (smart_handle_facade const& rhs)
    {
        if (BOOST_LIKELY(rhs.handle_ != handle_)) {
            TraitsT::release(impl_.get(), handle_);
            handle_ = rhs.handle_;
            impl_ = rhs.impl_;
            TraitsT::add_ref(handle_);
        }
        return *this;
    }

    smart_handle_facade& operator= (smart_handle_facade && rhs) noexcept
    {
        if (BOOST_LIKELY(rhs.handle_ != handle_)) {
            TraitsT::release(impl_.get(), handle_);
            handle_ = rhs.handle_;
            impl_ = std::move(rhs.impl_);
            rhs.handle_ = TraitsT::not_initialized_v;
        }
        return *this;
    }

    void reset()
    {
        TraitsT::release(impl_.get(), handle_);
        handle_ = TraitsT::not_initialized_v;
        impl_.reset();
    }

    explicit operator bool() const noexcept { return handle_ != TraitsT::not_initialized_v; }

    typename base_type::handle_type handle() const { return handle_; }

private:
    shared_ptr<typename base_type::service_type> impl_;
    typename base_type::handle_type handle_;
};

template <template <class, class> class HandleApiT>
class smart_handle_access
{
public:
    template <typename TraitsT>
    static smart_handle_facade<TraitsT, HandleApiT> create(
        shared_ptr<typename smart_handle_facade<TraitsT, HandleApiT>::service_type> impl,
        typename TraitsT::handle_type handle) noexcept
    {
        return smart_handle_facade<TraitsT, HandleApiT>(std::move(impl), handle);
    }

    template <typename TraitsT>
    static typename TraitsT::handle_type handle(smart_handle_facade<TraitsT, HandleApiT> const& h)
    {
        return h.handle_;
    }

    template <typename TraitsT>
    static shared_ptr<typename smart_handle_facade<TraitsT, HandleApiT>::base_type::service_type> const& impl(smart_handle_facade<TraitsT, HandleApiT> const& h)
    {
        return h.impl_;
    }
};

}
