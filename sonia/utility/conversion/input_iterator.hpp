/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility> // std::pair
#include <boost/utility/addressof.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/optional.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/utility/hazardous.hpp"

namespace sonia::conversion {

template <
      class ConverterT
    , class InputIteratorT
>
class convert_input_iterator
    : public boost::iterator_facade<
          convert_input_iterator<ConverterT, InputIteratorT>
        //, const typename detail::clear_type<typename ConverterT::template apply<void>::type::destination_type>::type
        , const typename ConverterT::template apply<void>::type::destination_type
        , boost::single_pass_traversal_tag
    >
{
    typedef convert_input_iterator this_type;
    typedef typename ConverterT::template apply<void>::type converter_t;
    typedef typename converter_t::destination_type converter_value_type;
    
public:
    typedef typename converter_t::gstate_t::type state_type;
    typedef typename convert_input_iterator::value_type value_type;
    //typedef typename detail::clear_type<converter_value_type>::type value_type;

    convert_input_iterator() : state_(nullptr)
    { }

    template <typename CVTExprOrObjectT>
    convert_input_iterator(CVTExprOrObjectT const& expr, state_type & st, InputIteratorT const& first, InputIteratorT const& last)
        : state_{std::addressof(st)}, range_{in_place, first, last}
    {
        converter_t::gclear(expr.get_init_tag(), *state_);
        increment();
    }

    convert_input_iterator(convert_input_iterator const& rhs)
        : state_(rhs.state_), value_(rhs.value_)
    {
        if (state_) {
            range_.emplace(*rhs.range_);
        }
    }

    convert_input_iterator(convert_input_iterator && rhs)
        : state_(rhs.state_), value_(std::move(rhs.value_))
    {
        if (state_) {
            range_.emplace(std::move(*rhs.range_));
            rhs.range_.destroy();
            rhs.state_ = nullptr;
        }
    }

    ~convert_input_iterator() {
        if (state_) {
            range_.destroy();
        }
    }

    void operator= (convert_input_iterator const& rhs) {
        if (state_) {
            if (rhs.state_) {
                range_ = *rhs.range_;
            } else {
                range_.destroy();
            }
        } else if (rhs.state_) {
            range_.emplace(*rhs.range_);
        }
        state_ = rhs.state_;
        value_ = rhs.value_;
    }

    void operator= (convert_input_iterator && rhs) {
        value_ = std::move(rhs.value_);
        if (state_) {
            if (rhs.state_) {
                range_ = std::move(*rhs.range_);
                rhs.range_.destroy();
            } else {
                range_.destroy();
            }
        } else if (rhs.state_) {
            range_.emplace(*rhs.range_);
            rhs.range_.destroy();
        }
        state_ = rhs.state_;
        rhs.state_ = nullptr;
    }

    //template <typename ForwardWritableIterator>
    //inline ForwardWritableIterator copy_range(ForwardWritableIterator beginDest, ForwardWritableIterator endDest) {
    //    BOOST_ASSERT(value_);
    //    if (beginDest != endDest) {
    //        *beginDest = value_.get();
    //        ++beginDest;
    //        ForwardWritableIterator result = converter_t::get_range(*range_, state_, beginDest, endDest);
    //        increment();
    //        return result;
    //    }
    //    return beginDest;
    //}

private:
    friend class boost::iterator_core_access;

    inline void increment()
    {
        BOOST_ASSERT(state_);
        hazardous<value_type> temp;
        if (converter_t::get(*range_, *state_, temp.get_pointer())) {
            value_ = *temp;
        } else {
            state_ = nullptr;
        }
    }

    inline bool equal(this_type const& other) const {
        return !state_ && !other.state_;
    }

    inline value_type const& dereference() const {
        BOOST_ASSERT(value_);
        return *value_;
    }

    state_type * state_;
    hazardous<std::pair<InputIteratorT, InputIteratorT>> range_;
    optional<value_type> value_;
};

}
