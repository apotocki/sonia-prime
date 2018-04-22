//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ARRAY_VALUE_FORWARD_ITERATOR_ADAPTER_HPP
#define SONIA_UTILITY_ARRAY_VALUE_FORWARD_ITERATOR_ADAPTER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/array_view.hpp"

namespace sonia {

template <typename DerivedT, typename ArrayElementT, typename SharedBufferManagerT>
class array_value_forward_iterator_adapter
    : public boost::iterator_facade
        < DerivedT
        , array_view<ArrayElementT>
        , std::forward_iterator_tag
        , array_view<ArrayElementT>>
{
    friend class boost::iterator_core_access;

    typedef typename SharedBufferManagerT::buffer_type buffer_type;

    bool equal(tcpsocket_input_iterator const& rhs) const noexcept {
        if (empty()) return rhs.empty();
        if (rhs.empty()) return false;
        return value_ == rhs.value_;
    }

    array_view<ArrayElementT> dereference() const
    {
        BOOST_VERIFY(!empty());
        return value_;
    }

    void increment()
    {
        BOOST_VERIFY(!empty());
        BOOST_ASSERT(value_);
        size_t offset = value_.end() - buff_.cbegin();
        value_ = SharedBufferManagerT::get_available(offset, buff_);
        if (value_.empty()) {
            derived().load_value();
        }
    }

public:
    array_value_forward_iterator_adapter()
    {
        buff_ = SharedBufferManagerT::create(buff_);
    }

    array_value_forward_iterator_adapter(array_value_forward_iterator_adapter const& rhs) noexcept
    {
        rhs.wait_for_value();
        buff_ = rhs.buff_;
        value_ = rhs.value_;
    }

    array_value_forward_iterator_adapter(array_value_forward_iterator_adapter && rhs) noexcept
    {
        rhs.wait_for_value();
        buff_ = std::move(rhs.buff_);
        value_ = rhs.value_;
    }

    ~array_value_forward_iterator_adapter() noexcept
    {
        derived().wait_for_value();
    }

    void operator= (array_value_forward_iterator_adapter const& rhs) noexcept
    {
        if (this != &rhs) {
            rhs.wait_for_value();
            buff_ = rhs.buff_;
            value_ = rhs.value_;
        }
    }

    void operator= (array_value_forward_iterator_adapter && rhs) noexcept
    {
        if (this != &rhs) {
            rhs.wait_for_value();
            buff_ = std::move(rhs.buff_);
            value_ = rhs.value_;
        }
    }

    bool empty() const
    {
        derived().wait_for_value();
        if (!buff_) return true;
        if (!value_.empty()) return false;
        derived().load_value();
        derived().wait_for_value();
        return !buff_;
    }

    // sync implementation
    void wait_for_value() {}

protected:
    DerivedT const& derived() const { return static_cast<DerivedT const&>(*this); }
    DerivedT & derived() { return static_cast<DerivedT&>(*this); }

    buffer_type buff_;
    array_view<ArrayElementT> value_;
};

#endif // SONIA_UTILITY_ARRAY_VALUE_FORWARD_ITERATOR_ADAPTER_HPP
