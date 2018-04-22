//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RANGE_DEREFERENCING_ITERATOR_HPP
#define SONIA_UTILITY_RANGE_DEREFERENCING_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/optional.hpp"

namespace sonia {

template <typename IteratorT>
struct range_dereferencing_iterator_helper
{
    typedef typename boost::iterator_traversal<IteratorT>::type traversal_type;
    typedef typename boost::iterator_value<IteratorT>::type range_type;
    typedef typename boost::range_iterator<range_type>::type range_iterator_type;
    typedef typename boost::iterator_value<range_iterator_type>::type value_type;
    typedef typename boost::iterator_reference<range_iterator_type>::type reference_type;
};

template <typename IteratorT>
class range_dereferencing_iterator 
    : public boost::iterator_facade<
        range_dereferencing_iterator<IteratorT>,
        typename range_dereferencing_iterator_helper<IteratorT>::value_type,
        typename range_dereferencing_iterator_helper<IteratorT>::traversal_type,
        typename range_dereferencing_iterator_helper<IteratorT>::reference_type
    >
{
    friend class boost::iterator_core_access;
    typedef range_dereferencing_iterator_helper<IteratorT> helper_t;
    typedef typename helper_t::range_iterator_type range_iterator_type;
    typedef typename helper_t::reference_type reference_type;

    bool equal(range_dereferencing_iterator const& rhs) const {
        if (empty()) return rhs.empty();
        if (rhs.empty()) return false;
        return current_ == other.current_;
    }

    reference_type dereference() const {
        if (!pos_) {
            do_dereference<true>();
        }
        return *pos_->first;
    }

    void increment() {
        if (!pos_) {
            do_dereference<true>();
        }
        ++pos_->first;
        if (pos_->first == pos_->second) {
            ++it_;
            pos_.reset();
        }
    }

    void decrement() {
        if (!pos_) {
            do_dereference<false>();
        }
        if (pos_->first == boost::begin(*it_)) {
            --it_;
            pos_.reset();
            do_dereference<false>();
            return;
        }
        --pos_->first;
    }

    template <bool fwd>
    void do_dereference() const {
        for (;;) {
            typename helper_t::range_type rng = *it_;
            pos_.emplace(std::make_pair(boost::begin(rng), boost::end(rng)));
            if (pos_->first != pos_->second) return;
            pos_.reset();
            if constexpr (fwd) ++it_; else --it_;
        }
    }

public:
    explicit range_dereferencing_iterator(IteratorT it)
        : it_(std::move(it))
    {
        
    }

    bool empty() const { return it_.empty(); }

    void flush() {
        if (pos_) {
            typename helper_t::range_type rng = *it_;
            *it_ = helper_t::range_type(boost::begin(rng), pos_->first);
        }
    }

private:
    mutable IteratorT it_;
    mutable optional<std::pair<range_iterator_type, range_iterator_type>> pos_;
};

}

#endif // SONIA_UTILITY_RANGE_DEREFERENCING_ITERATOR_HPP
