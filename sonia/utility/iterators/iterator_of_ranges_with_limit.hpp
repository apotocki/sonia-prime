//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/iterator_traits.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <class IteratorT>
class iterator_of_ranges_with_limit
{
public:
    using value_type = iterator_value_t<IteratorT>;
    using iterator_category = iterator_category_t<IteratorT>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<iterator_of_ranges_with_limit const*, value_type>>;

public:
    iterator_of_ranges_with_limit(IteratorT it, uint64_t limitsz)
        : base{std::move(it)}, limit{limitsz}
    {}

    bool empty() const { return !limit; }

    bool operator==(iterator_of_ranges_with_limit const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_t operator* () const
    {
        return iterators::make_value_proxy<value_type>(this);
    }

    value_type get_dereference() const
    {
        if (!limit) {
            THROW_EOF_ERROR();
        }
        value_type r = *base;
        size_t cursz = std::size(r);
        if (cursz > limit) {
            auto b = std::begin(r);
            auto e = b;
            std::advance(e, limit);
            r = value_type{ b, e };
            *base = r;
            //*base = value_type{e, std::end(r)};
        }
        if (!current_size) {
            current_size = std::size(r);
        }
        return r;
    }

    void set_dereference(value_type rng)
    {
        *base = rng;
    }

    iterator_of_ranges_with_limit& operator++()
    {
        if (!current_size) {
            value_type r = *base;
            current_size = std::size(r);
        }
        limit -= *current_size;
        current_size.reset();
        ++base;
        return *this;
    }

    template <typename T = IteratorT>
    requires(iterators::has_method_flush_v<T, void()>)
    void flush() { base.flush(); }

    IteratorT base;
    uint64_t limit;
    mutable optional<size_t> current_size;
};

}
