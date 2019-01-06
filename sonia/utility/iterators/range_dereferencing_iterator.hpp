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
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/optional.hpp"
#include "sonia/iterator_traits.hpp"

namespace sonia {

template <class IteratorT, class CategoryOrTraversalT = iterator_traversal_t<IteratorT>>
class range_dereferencing_iterator_state
{
    using traversal_t = iterator_category_to_traversal_t<CategoryOrTraversalT>;
    static constexpr bool is_bidirectional_v = is_base_of_v<boost::bidirectional_traversal_tag, traversal_t>;
    
    using subrange_iterator = iterator_dereferenced_range_iterator_t<IteratorT>;

    using state_t = conditional_t<
        is_bidirectional_v,
        std::tuple<subrange_iterator, subrange_iterator, subrange_iterator>,
        std::tuple<subrange_iterator, subrange_iterator>
    >;
    mutable optional<state_t> state_;
    IteratorT base_;

public:
    range_dereferencing_iterator_state() {}

    explicit range_dereferencing_iterator_state(IteratorT it)
        : base_(std::move(it))
    {}

    IteratorT & base() { return base_; }
    IteratorT const& base() const { return base_; }

    bool empty() const
    {
        if (BOOST_LIKELY(!!state_)) return false;
        if (sonia::empty(base_)) return true;
        init_state();
        return false;
    }

    void increment()
    {
        ++base_;
        state_.reset();
    }

    void decrement()
    {
        --base_;
        state_.reset();
        iterator_dereferenced_range_t<IteratorT> rng = *base_;
        state_.emplace(std::tuple(boost::end(rng), boost::end(rng), boost::begin(rng)));
    }

    auto & optget() { return state_; }

    /*
    state[0] - cursor, state[1] - end of rng, (for bidirectional:) state[2] - begin of range
    */
    state_t & get() const
    {
        if (!state_) {
            init_state();
        }
        return *state_;
    }

    void flush()
    {
        iterator_dereferenced_range_t<IteratorT> rng = *base_;
        *base_ = array_view(boost::begin(rng), std::get<0>(get()));
        if constexpr (iterators::has_method_flush_v<IteratorT, void()>) {
            base_.flush();
        }
    }

private:
    void init_state() const
    {
        iterator_dereferenced_range_t<IteratorT> rng = *base_;
        if constexpr (is_bidirectional_v) {
            state_.emplace(std::tuple(boost::begin(rng), boost::end(rng), boost::begin(rng)));
        } else {
            state_.emplace(std::tuple(boost::begin(rng), boost::end(rng)));
        }
    }
};

template <class IteratorT, class CategoryOrTraversal = iterator_traversal_t<IteratorT>>
class range_dereferencing_iterator 
    : public boost::iterator_facade<
        range_dereferencing_iterator<IteratorT, CategoryOrTraversal>,
        iterator_dereferenced_range_iterator_value_t<IteratorT>,
        CategoryOrTraversal,
        iterator_dereferenced_range_iterator_reference_t<IteratorT>>
    , range_dereferencing_iterator_state<IteratorT>
{
    friend class boost::iterator_core_access;

    using range_iterator_type = iterator_dereferenced_range_iterator_t<IteratorT>;
    using reference_type = iterator_dereferenced_range_iterator_reference_t<IteratorT>;
    using state_t = range_dereferencing_iterator_state<IteratorT>;

    bool equal(range_dereferencing_iterator const& rhs) const
    {
        if (empty()) return rhs.empty();
        if (rhs.empty()) return false;
        return std::get<0>(state_t::get()) == std::get<0>(rhs.get());
    }

    reference_type dereference() const
    {
        return *std::get<0>(state_t::get());
    }

    void increment()
    {
        auto & st = state_t::get();
        ++std::get<0>(st);
        if (std::get<0>(st) == std::get<1>(st)) {
            state_t::increment();
        }
    }

    void decrement()
    {
        auto & optst = state_t::optget();
        if (!optst || std::get<0>(*optst) == std::get<2>(*optst)) {
            state_t::decrement();
            // after that optst is not nullopt
        }
        BOOST_ASSERT(std::get<0>(*optst) != std::get<2>(*optst)); // retrieved range must not be empty
        --std::get<0>(*optst);
    }

public:
    range_dereferencing_iterator() {}

    explicit range_dereferencing_iterator(IteratorT it)
        : state_t(std::move(it))
    {}

    bool empty() const { return state_t::empty(); }

    void flush()
    {
        state_t::flush();
    }

    IteratorT & base() { return state_t::base(); }
    IteratorT const& base() const { return state_t::base(); }
};

}

#endif // SONIA_UTILITY_RANGE_DEREFERENCING_ITERATOR_HPP
