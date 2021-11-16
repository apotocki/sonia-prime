/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <boost/iterator/iterator_facade.hpp>

namespace sonia::conversion::auxilary {

namespace detail {

    template <class Value>
    class proxy_impl
    {
    public:
        template <class T>
        proxy_impl& operator=(const T& value)
        {
            do_put(value);
            return *this; 
        }
        virtual void increment() = 0;
        virtual bool equal(proxy_impl const& other) const = 0;

    protected:
        virtual void do_put(Value const& v) = 0;
    };

    template <class Value, class T>
    class concrete_proxy_impl
        : public proxy_impl<Value>
    {
    public:
        inline explicit concrete_proxy_impl(T & ref) : ref_(ref) {}
        inline void increment() { ++ref_; }
        inline bool equal(proxy_impl<Value> const& other) const {
            return ref_ == static_cast<concrete_proxy_impl const&>(other).ref_;
        }

    protected:
        void do_put(Value const& v) { *ref_ = v; }
    private:
        T & ref_;
    };

    template <class Value, class T>
    class concrete_nc_proxy_impl
        : public proxy_impl<Value>
    {
    public:
        inline explicit concrete_nc_proxy_impl(T & ref) : ref_(ref) {}
        inline void increment() { ++ref_; }
        inline bool equal(proxy_impl<Value> const& other) const {
            return false;
        }

    protected:
        void do_put(Value const& v) { *ref_ = v; }
    private:
        T & ref_;
    };

} // namespace sonia::conversion::auxilary::detail

template <class Value, class T>
class concrete_output_iterator_reference
{
public:
    inline explicit concrete_output_iterator_reference(T & ref)
        : proxy_(ref) {}

    inline detail::concrete_proxy_impl<Value, T> & get_proxy()
    { return proxy_; }
private:
    detail::concrete_proxy_impl<Value, T> proxy_;
};

// not comparable version
template <class Value, class T>
class concrete_nc_output_iterator_reference
{
public:
    inline explicit concrete_nc_output_iterator_reference(T & ref)
        : proxy_(ref) {}

    inline detail::concrete_nc_proxy_impl<Value, T> & get_proxy()
    { return proxy_; }
private:
    detail::concrete_nc_proxy_impl<Value, T> proxy_;
};

template <class Value>
class dyn_ref_fwd_output_iterator
{
    typedef dyn_ref_fwd_output_iterator this_type;
public:
    typedef std::output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    template <typename ForwardWritableIterator>
    explicit dyn_ref_fwd_output_iterator(concrete_output_iterator_reference<Value, ForwardWritableIterator> & ref)
        : proxy_(ref.get_proxy())
    {}

    template <typename ForwardWritableIterator>
    explicit dyn_ref_fwd_output_iterator(concrete_nc_output_iterator_reference<Value, ForwardWritableIterator> & ref)
        : proxy_(ref.get_proxy())
    {}

    detail::proxy_impl<Value>& operator*() { return proxy_; }
    this_type& operator++() { proxy_.increment(); return *this; } 
    this_type& operator++(int) { proxy_.increment(); return *this; } // !!!

    inline bool operator == (this_type const& other) const {
        return proxy_.equal(other.proxy_);
    }

    inline bool operator != (this_type const& other) const {
        return !this->operator==(other);
    }
private:
    detail::proxy_impl<Value> & proxy_;
};

} // namespace sonia::conversion::auxilary
