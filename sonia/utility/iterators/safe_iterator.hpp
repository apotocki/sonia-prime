//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/iterator_traits.hpp"

namespace sonia {

template <class IteratorT>
class safe_iterator
{
public:
    using value_type = iterator_value_t<IteratorT>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type;

public:
    safe_iterator(IteratorT b, IteratorT e)
        : b_{std::move(b)}, e_{std::move(e)}
    {}

    bool empty() const { return b_ == e_; }

    bool operator==(safe_iterator const& rhs) const
    {
        return rhs.b_ == b_;
    }

    auto && operator* () const
    {
        return *b_;
    }

    safe_iterator& operator++()
    {
        if (b_ == e_) {
            THROW_EOF_ERROR();
        }
        ++b_;
        return *this;
    }

    template <typename T = IteratorT>
    requires(iterators::has_method_flush_v<T, void()>)
    void flush()
    {
        b_.flush();
    }

    template <typename T = IteratorT>
    requires(iterators::has_method_close_v<T, void()>)
    void close()
    {
        b_.close();
    }

    IteratorT b_, e_;
};

template <class RangeT>
auto make_safe_iterator(RangeT && rng)
{
    return safe_iterator{rng.begin(), rng.end()};
}

}
