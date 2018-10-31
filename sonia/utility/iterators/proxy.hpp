//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ITERATORS_PROXY_HPP
#define SONIA_UTILITY_ITERATORS_PROXY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

namespace sonia {

#if 0
template <class ImplT, class ReferenceT>
class wrapper_iterator_proxy
{
public:
    explicit wrapper_iterator_proxy(ImplT & impl) : impl_(impl) {}
    wrapper_iterator_proxy(wrapper_iterator_proxy &&) = default;

    wrapper_iterator_proxy(wrapper_iterator_proxy const&) = delete;
    wrapper_iterator_proxy & operator = (wrapper_iterator_proxy const&) = delete;
    wrapper_iterator_proxy & operator = (wrapper_iterator_proxy &&) = delete;

    operator ReferenceT() const { return impl_.dereference(); }
    wrapper_iterator_proxy & operator = (ReferenceT ref) { impl_.set(ref); return *this; }

private:
    ImplT & impl_;
};
#endif

template <class WrapperT>
class wrapper_iterator_proxy
{
public:
    typedef typename WrapperT::reference reference;

    template <typename ArgT>
    explicit wrapper_iterator_proxy(ArgT && arg) : wr_(std::forward<ArgT>(arg)) {}
    wrapper_iterator_proxy(wrapper_iterator_proxy &&) = default;

    wrapper_iterator_proxy(wrapper_iterator_proxy const&) = delete;
    wrapper_iterator_proxy & operator = (wrapper_iterator_proxy const&) = delete;
    wrapper_iterator_proxy & operator = (wrapper_iterator_proxy &&) = delete;

    operator reference() const { return wr_.get(); }
    wrapper_iterator_proxy & operator = (reference ref) { wr_.set(ref); return *this; }

private:
    WrapperT wr_;
};

template <class WrappedT, typename ReferenceT>
class basic_dereference_wrapper {
public:
    typedef ReferenceT reference;
    basic_dereference_wrapper(WrappedT & wr) : wr_(wr) {}
    reference get() const { return wr_.get_dereference(); }
    template <typename ArgT> void set(ArgT && arg) { wr_.set_dereference(std::forward<ArgT>(arg)); }

private:
    WrappedT & wr_;
};


template <class MemberT, typename ReferenceT>
class member_dereference_wrapper {
public:
    typedef ReferenceT reference;
    member_dereference_wrapper(MemberT & m) : m_(m) {}
    ReferenceT get() const { return m_.dereference(); }
    void set(ReferenceT arg) { m_.set(arg); }

private:
    MemberT & m_;
};

}

#endif // SONIA_UTILITY_ITERATORS_PROXY_HPP
