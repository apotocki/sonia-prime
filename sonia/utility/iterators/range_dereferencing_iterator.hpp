//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <utility>

#ifndef __ANDROID__
#   include <ranges>
#endif

#include <boost/assert.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/hazardous.hpp"

namespace sonia {

template <typename IteratorT>
struct range_dereferencing_iterator_equal
{
    bool operator()(IteratorT const& l, IteratorT const& r) const
    {
        if (l.empty()) return r.empty();
        if (r.empty()) return false;
        return l.unsafe_begin() == r.unsafe_begin();
    }
};


template <class IteratorT, class CategoryOrTraversalT = iterator_category_t<IteratorT>>
class range_dereferencing_iterator_state
{
public:
    //using categoty_t = iterator_category_to_traversal_t<CategoryOrTraversalT>;
    static constexpr bool is_bidirectional_v = is_base_of_v<std::bidirectional_iterator_tag, CategoryOrTraversalT>;

#ifdef __ANDROID__
    using range_t = iterator_dereferenced_range_t<IteratorT>;
    using subrange_iterator = iterator_dereferenced_range_iterator_t<IteratorT>;
#else
    using range_t = typename std::iterator_traits<IteratorT>::value_type;
    using subrange_iterator = std::ranges::iterator_t<range_t>;
#endif

    using state_t = conditional_t<
        is_bidirectional_v,
        std::tuple<subrange_iterator, subrange_iterator, subrange_iterator>,
        std::tuple<subrange_iterator, subrange_iterator>
    >;

    range_dereferencing_iterator_state()
        : state_initialized_{0}
    {}

    explicit range_dereferencing_iterator_state(IteratorT it)
        : base{std::move(it)}, state_initialized_{0}
    {}

    template <typename ... IteratorArgsT>
    explicit range_dereferencing_iterator_state(in_place_t, IteratorArgsT && ... args)
        : base{std::forward<IteratorArgsT>(args) ...}, state_initialized_{0}
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

    void advance_base(std::ptrdiff_t d)
    {
        base.advance_offset(d);
        hazardous_destroy_as_optional(state_, !!state_initialized_);
        state_initialized_ = 0;
    }

    void decrement_one() requires(is_bidirectional_v)
    {
        if (!state_initialized_ || std::get<0>(*state_) == std::get<2>(*state_)) {
            decrement();
        }
        BOOST_ASSERT(std::get<0>(*state_) != std::get<2>(*state_)); // retrieved range must not be empty
        --std::get<0>(*state_);
    }

    void decrement() requires(is_bidirectional_v)
    {
        using std::begin;
        using std::end;
        --base;
        hazardous_destroy_as_optional(state_, !!state_initialized_);
        state_initialized_ = 0;
        iterator_dereferenced_range_t<IteratorT> rng = *base;
        state_.emplace(end(rng), end(rng), begin(rng));
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
            *base = range_t{std::get<0>(*state_), std::get<1>(*state_)};
            if constexpr (iterators::has_method_flush_v<IteratorT, void()>) {
                base.flush();
            }
        }
    }

    void fix()
    {
        if (state_initialized_) {
            *base = range_t{std::get<0>(*state_), std::get<1>(*state_)};
        }
    }

    std::ptrdiff_t offset() const
    {
        using std::begin;
        if (state_initialized_) {
            iterator_dereferenced_range_t<IteratorT> rng = *base;
            return std::get<0>(*state_) - begin(rng);
        }
        return 0;
    }

    std::ptrdiff_t span_size()
    {
        if (state_initialized_) {
            return std::get<1>(*state_) - std::get<0>(*state_);
        }
        return 0;
    }

    mutable IteratorT base;

private:
    void init_state() const
    {
        using std::begin;
        using std::end;
        do {
            iterator_dereferenced_range_t<IteratorT> rng = *base;
            if (!std::empty(rng)) {
                if constexpr (is_bidirectional_v) {
                    state_.emplace(begin(rng), end(rng), begin(rng));
                } else {
                    state_.emplace(begin(rng), end(rng));
                }
                state_initialized_ = 1;
                return;
            }
            ++base;
        } while (!sonia::empty(base));
    }

    mutable hazardous<state_t> state_;
    mutable uint8_t state_initialized_ : 1;
};

template <class IteratorT, class CategoryOrTraversal = iterator_category_t<IteratorT>>
class range_dereferencing_iterator 
    : public range_dereferencing_iterator_state<IteratorT>
{
public:
    using value_type = iterator_dereferenced_range_iterator_value_t<IteratorT>;
    using pointer = value_type*;
    using reference = iterator_dereferenced_range_iterator_reference_t<IteratorT>;
    using difference_type = ptrdiff_t;
    using iterator_category = CategoryOrTraversal; // iterator_traversal_t<IteratorT>;

    using range_iterator_type = iterator_dereferenced_range_iterator_t<IteratorT>;
    using state_t = range_dereferencing_iterator_state<IteratorT>;

    constexpr static bool has_random_access_v = is_base_of_v<std::random_access_iterator_tag, iterator_category>;

    bool operator==(range_dereferencing_iterator const& rhs) const
    {
        return range_dereferencing_iterator_equal<range_dereferencing_iterator>()(*this, rhs);
    }

    reference operator*() const
    {
        return *std::get<0>(state_t::get());
    }

    range_dereferencing_iterator& operator++()
    {
        auto & st = state_t::get();
        if (BOOST_UNLIKELY(++std::get<0>(st) == std::get<1>(st))) {
            state_t::increment();
        }
        return *this;
    }

    range_dereferencing_iterator operator++(int)
    {
        range_dereferencing_iterator result{ *this };
        auto& st = state_t::get();
        if (BOOST_UNLIKELY(++std::get<0>(st) == std::get<1>(st))) {
            state_t::increment();
        }
        return result;
    }

    range_dereferencing_iterator& operator--() requires(state_t::is_bidirectional_v)
    {
        state_t::decrement_one();
        return *this;
    }

    range_dereferencing_iterator operator--(int) requires(state_t::is_bidirectional_v)
    {
        range_dereferencing_iterator result{ *this };
        state_t::decrement_one();
        return result;
    }

    range_dereferencing_iterator& operator+=(difference_type d) requires(has_random_access_v)
    {
        auto& st = state_t::get();
        difference_type spansz = std::get<1>(st) - std::get<0>(st);
        if (spansz > d) {
            std::get<0>(st) += d;
        } else {
            state_t::advance_base(d - spansz);
        }
        return *this;
    }

    range_dereferencing_iterator& operator-=(difference_type) requires(has_random_access_v)
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        return *this;
    }

    friend range_dereferencing_iterator operator+(range_dereferencing_iterator it, difference_type d) requires(has_random_access_v)
    {
        it += d;
        return it;
    }

    friend range_dereferencing_iterator operator+(difference_type d, range_dereferencing_iterator it) requires(has_random_access_v)
    {
        it += d;
        return it;
    }

    range_dereferencing_iterator operator-(difference_type d) const requires(has_random_access_v)
    {
        range_dereferencing_iterator result{ *this };
        result -= d;
        return result;
    }

    friend auto operator-(range_dereferencing_iterator const& l, range_dereferencing_iterator const& r) requires(has_random_access_v)
    {
        return l.offset() - r.offset();
    }

    friend auto operator<=>(range_dereferencing_iterator const& l, range_dereferencing_iterator const& r) requires(has_random_access_v) 
    {
        return l.offset() <=> r.offset();
    }

    reference operator[](difference_type) const requires(has_random_access_v)
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

public:
    range_dereferencing_iterator() = default;

    explicit range_dereferencing_iterator(IteratorT it)
        : state_t{std::move(it)}
    {}

    template <typename ... IteratorArgsT>
    explicit range_dereferencing_iterator(in_place_t, IteratorArgsT && ... args)
        : state_t{ in_place, std::forward<IteratorArgsT>(args) ... }
    {}

    range_dereferencing_iterator(range_dereferencing_iterator const&) = default;
    range_dereferencing_iterator(range_dereferencing_iterator &&) = default;
    range_dereferencing_iterator& operator= (range_dereferencing_iterator const&) = default;
    range_dereferencing_iterator& operator= (range_dereferencing_iterator &&) = default;

    ~range_dereferencing_iterator() noexcept = default; // no flush call on exit!

    void close() requires(iterators::has_method_close_v<IteratorT, void()>)
    {
        state_t::flush();
        state_t::reset();
        this->base.close();
    }
};

}
