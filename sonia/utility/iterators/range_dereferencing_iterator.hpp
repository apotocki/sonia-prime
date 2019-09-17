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

#include "sonia/iterator_traits.hpp"
#include "sonia/utility/hazardous.hpp"

namespace sonia {

#if 1
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
    mutable hazardous<state_t> state_;
    mutable uint8_t state_initialized_ : 1;

public:
    range_dereferencing_iterator_state()
        : state_initialized_{0}
    {}

    explicit range_dereferencing_iterator_state(IteratorT it)
        : base{std::move(it)}, state_initialized_{0}
    {}

    range_dereferencing_iterator_state(range_dereferencing_iterator_state const& rhs)
        : base{rhs.base}, state_initialized_{rhs.state_initialized_}
    {
        hazardous_copy_as_optional(rhs.state_, !!rhs.state_initialized_, state_, false);
    }

    range_dereferencing_iterator_state(range_dereferencing_iterator_state && rhs)
        : base{std::move(rhs.base)}, state_initialized_{rhs.state_initialized_}
    {
        hazardous_move_as_optional(rhs.state_, !!rhs.state_initialized_, state_, false);
    }

    range_dereferencing_iterator_state& operator= (range_dereferencing_iterator_state const& rhs)
    {
        base = rhs.base;
        hazardous_copy_as_optional(rhs.state_, !!rhs.state_initialized_, state_, state_initialized_);
        state_initialized_ = rhs.state_initialized_;
        return *this;
    }

    range_dereferencing_iterator_state& operator= (range_dereferencing_iterator_state && rhs)
    {
        base = std::move(rhs.base);
        hazardous_move_as_optional(rhs.state_, !!rhs.state_initialized_, state_, state_initialized_);
        state_initialized_ = rhs.state_initialized_;
        return *this;
    }

    ~range_dereferencing_iterator_state()
    {
        hazardous_destroy_as_optional(state_, !!state_initialized_);
    }

    bool empty() const
    {
        if (BOOST_LIKELY(!!state_initialized_)) return false;
        if (sonia::empty(base)) return true;
        init_state();
        return !state_initialized_;
    }

    void increment()
    {
        ++base;
        hazardous_destroy_as_optional(state_, !!state_initialized_);
        state_initialized_ = 0;
    }

    void decrement_one()
    {
        if (!state_initialized_ || std::get<0>(*state_) == std::get<2>(*state_)) {
            decrement();
        }
        BOOST_ASSERT(std::get<0>(*state_) != std::get<2>(*state_)); // retrieved range must not be empty
        --std::get<0>(*state_);
    }

    void decrement()
    {
        --base;
        hazardous_destroy_as_optional(state_, !!state_initialized_);
        state_initialized_ = 0;
        iterator_dereferenced_range_t<IteratorT> rng = *base;
        state_.emplace(boost::end(rng), boost::end(rng), boost::begin(rng));
        state_initialized_ = 1;
    }

    /*
    state[0] - cursor, state[1] - end of rng, (for bidirectional:) state[2] - begin of range
    */
    state_t & get() const
    {
        if (!state_initialized_) {
            init_state();
        }
        return *state_;
    }

    subrange_iterator unsafe_begin() const
    {
        return std::get<0>(*state_);
    }

    void reset()
    {
        hazardous_destroy_as_optional(state_, !!state_initialized_);
        state_initialized_ = 0;
    }

    void flush()
    {
        if (state_initialized_) {
            *base = array_view(std::get<0>(*state_), std::get<1>(*state_));
            if constexpr (iterators::has_method_flush_v<IteratorT, void()>) {
                base.flush();
            }
        }
    }

    void fix()
    {
        if (state_initialized_) {
            *base = array_view(std::get<0>(*state_), std::get<1>(*state_));
        }
    }

    mutable IteratorT base;

private:
    void init_state() const
    {
        do {
            iterator_dereferenced_range_t<IteratorT> rng = *base;
            if (!boost::empty(rng)) {
                if constexpr (is_bidirectional_v) {
                    state_.emplace(boost::begin(rng), boost::end(rng), boost::begin(rng));
                } else {
                    state_.emplace(boost::begin(rng), boost::end(rng));
                }
                state_initialized_ = 1;
                return;
            }
            ++base;
        } while (!sonia::empty(base));
    }
};

#else


#endif

template <class IteratorT, class CategoryOrTraversal = iterator_category_t<IteratorT>>
class range_dereferencing_iterator 
    : public boost::iterator_facade<
        range_dereferencing_iterator<IteratorT, CategoryOrTraversal>,
        iterator_dereferenced_range_iterator_value_t<IteratorT>,
        CategoryOrTraversal,
        iterator_dereferenced_range_iterator_reference_t<IteratorT>>
    , public range_dereferencing_iterator_state<IteratorT>
{
    friend class boost::iterator_core_access;

    using range_iterator_type = iterator_dereferenced_range_iterator_t<IteratorT>;
    using reference_type = iterator_dereferenced_range_iterator_reference_t<IteratorT>;
    using state_t = range_dereferencing_iterator_state<IteratorT>;

    bool equal(range_dereferencing_iterator const& rhs) const
    {
        if (state_t::empty()) return rhs.empty();
        if (rhs.empty()) return false;
        return state_t::unsafe_begin() == rhs.unsafe_begin();
    }

    reference_type dereference() const
    {
        return *std::get<0>(state_t::get());
    }

    void increment()
    {
        auto & st = state_t::get();
        if (BOOST_LIKELY(++std::get<0>(st) != std::get<1>(st))) return;
        state_t::increment();
    }

    void decrement()
    {
        state_t::decrement_one();
    }

public:
    range_dereferencing_iterator() = default;

    explicit range_dereferencing_iterator(IteratorT it)
        : state_t{std::move(it)}
    {}

    range_dereferencing_iterator(range_dereferencing_iterator const&) = default;
    range_dereferencing_iterator(range_dereferencing_iterator &&) = default;
    range_dereferencing_iterator& operator= (range_dereferencing_iterator const&) = default;
    range_dereferencing_iterator& operator= (range_dereferencing_iterator &&) = default;

    ~range_dereferencing_iterator() noexcept = default; // no flush call on exit!

    template <typename T = IteratorT>
    enable_if_t<iterators::has_method_close_v<T, void()>> close()
    {
        state_t::flush();
        state_t::reset();
        this->base.close();
    }
};

}

#endif // SONIA_UTILITY_RANGE_DEREFERENCING_ITERATOR_HPP
