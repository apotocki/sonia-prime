/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <boost/iterator/iterator_facade.hpp>

namespace sonia::conversion::auxilary {

template <class Value>
class input_iterator_reference
{
public:
    input_iterator_reference() = default;
    input_iterator_reference(input_iterator_reference const&) = delete;

    virtual Value dereference() const = 0;
    virtual Value dereference_and_increment() = 0;
    virtual size_t load_chunk(Value * dest, size_t sz) = 0;
    virtual void increment() = 0;
    virtual bool equal(input_iterator_reference<Value> const& other) const = 0;
    virtual ~input_iterator_reference() = default;
};

template <class Value, class T>
class concrete_input_iterator_reference
    : public input_iterator_reference<Value>
{
public:
    inline explicit concrete_input_iterator_reference(T & ref) : ref_(ref) {}
    inline Value dereference() const { return *ref_; }
    inline Value dereference_and_increment()
    { 
        //return *ref_++;
        Value result = *ref_;
        ++ref_;
        return result;
    }
    inline void increment() { ++ref_; }
    inline bool equal(input_iterator_reference<Value> const& other) const
    { return ref_ == static_cast<concrete_input_iterator_reference const&>(other).ref_; }

    inline size_t load_chunk(Value * dest, size_t sz)
    {
        return copy_range(ref_, T(), dest, dest + sz) - dest;
    }
private:
    T & ref_;
};

template <class Value, class T>
class concrete_input_iterator_reference<Value, const T>
    : public input_iterator_reference<Value>
{
public:
    inline explicit concrete_input_iterator_reference(T const& ref) : ref_(ref) {}
    inline Value dereference() const { return *ref_; }
    inline Value dereference_and_increment() { return *ref_; }
    inline void increment() { }
    inline bool equal(input_iterator_reference<Value> const& other) const
    { return ref_ == static_cast<concrete_input_iterator_reference const&>(other).ref_; }

    inline size_t load_chunk(Value *, size_t) { return 0; }
private:
    T const& ref_;
};

template <typename Value>
class dyn_ref_fwd_input_iterator
    : public boost::iterator_facade<
          dyn_ref_fwd_input_iterator<Value>
        , const Value
        , boost::single_pass_traversal_tag
        , Value
      >
{
    typedef dyn_ref_fwd_input_iterator self;
public:
    inline explicit dyn_ref_fwd_input_iterator(input_iterator_reference<Value> & ref)
        : store_(ref)
    {}

private:
    friend class boost::iterator_core_access;

    inline void increment() { store_.increment(); }
    inline bool equal(self const& other) const {
        return store_.equal(other.store_);
    }
    inline Value dereference() const { return store_.dereference(); }
// members
    input_iterator_reference<Value> & store_;
};

} // namespace sonia::conversion::auxilary
