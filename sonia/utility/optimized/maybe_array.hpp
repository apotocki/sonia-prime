//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIMIZED_MAYBE_ARRAY_HPP
#define SONIA_UTILITY_OPTIMIZED_MAYBE_ARRAY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/throw_exception.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/utility/optimized/array.hpp"

namespace sonia {

template <typename ElementT, size_t ByteSzV, typename RefCountT>
class maybe_array : optimized_holder<
    (std::max)(ByteSzV, sizeof(ElementT*) + sizeof(size_t)),
    1, RefCountT>
{
    using holder_t = typename maybe_array::optimized_holder_t;
    using array_t = optimized_array_impl<remove_cv_t<ElementT>, holder_t>;
    
public:
    // range resembling
    using value_type = remove_cv_t<ElementT>;
    using const_value_type = add_const_t<ElementT>;
    using pointer = add_pointer_t<ElementT>;
    using reference = add_lvalue_reference_t<ElementT>;
    using const_reference = add_lvalue_reference_t<const_value_type>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = add_pointer_t<ElementT>;
    using const_iterator = add_pointer_t<const_value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_t max_size = (std::numeric_limits<size_t>::max)() >> 2;

    maybe_array() noexcept // as view
        : maybe_array(nullptr, 0)
    {}

    template <size_t N>
    maybe_array(ElementT(&arr)[N]) noexcept // as view
        : maybe_array(arr, N)
    {}

    maybe_array(array_view<ElementT> av) noexcept // as view
        : maybe_array(av.begin(), av.size())
    {}

    maybe_array(ElementT * d, size_type sz) noexcept // as view
    {
        BOOST_ASSERT(sz <= max_size);
        holder_t::set_uint(sz);
        *view_data() = d;
        holder_t::set_service_cookie(0);
    }

    explicit maybe_array(size_type sz) // as array
    {
        array_t::init(this, sz);
        holder_t::set_service_cookie(1);
    }

    iterator begin() const noexcept
    {
        if (!holder_t::get_service_cookie()) {
            return *view_data();
        } else {
            return array_t::begin(this);
        }
    }

    iterator end() const noexcept
    {
        if (!holder_t::get_service_cookie()) {
            return *view_data() + (holder_t::get_uint() & max_size);
        } else {
            return array_t::end(this);
        }
    }

    size_type size() const noexcept
    {
        if (!holder_t::get_service_cookie()) {
            return (holder_t::get_uint() & max_size);
        } else {
            return array_t::size(this);
        }
    }

    array_view<value_type> to_array_view()
    {
        if (!holder_t::get_service_cookie()) {
            if constexpr (is_const_v<ElementT>) {
                BOOST_THROW_EXCEPTION(internal_error("maybe_array: non-const access to const array_view"));
            } else {
                return array_view<value_type>{*view_data(), holder_t::get_uint() & max_size};
            }
        }
        return array_t::get(this);
    }

    array_view<const ElementT> to_array_view() const noexcept
    {
        if (!holder_t::get_service_cookie()) {
            return array_view<const ElementT>{*view_data(), holder_t::get_uint() & max_size};
        }
        return array_t::get(this);
    }

private:
    ElementT ** view_data() noexcept
    {
        return std::launder(reinterpret_cast<ElementT**>(holder_t::data() + sizeof(size_t)));
    }

    ElementT * const* view_data() const noexcept
    {
        return std::launder(reinterpret_cast<ElementT* const*>(holder_t::data() + sizeof(size_t)));
    }
};

template <typename T, size_t ByteSzV0, typename RefCountT0, size_t ByteSzV1, typename RefCountT1>
bool operator== (maybe_array<T, ByteSzV0, RefCountT0> const& lhs, maybe_array<T, ByteSzV1, RefCountT1> const& rhs)
{
    return range_equal()(lhs, rhs);
}

template <typename T, size_t ByteSzV0, typename RefCountT0, size_t ByteSzV1, typename RefCountT1>
bool operator!= (maybe_array<T, ByteSzV0, RefCountT0> const& lhs, maybe_array<T, ByteSzV1, RefCountT1> const& rhs)
{
    return !range_equal()(lhs, rhs);
}

template <typename T, size_t ByteSzV0, typename RefCountT0, size_t ByteSzV1, typename RefCountT1>
bool operator< (maybe_array<T, ByteSzV0, RefCountT0> const& lhs, maybe_array<T, ByteSzV1, RefCountT1> const& rhs)
{
    return range_less()(lhs, rhs);
}

template <typename T, size_t ByteSzV, typename RefCountT>
array_view<const T> to_array_view(maybe_array<T, ByteSzV, RefCountT> const& sa) { return sa.to_array_view(); }

template <typename T, size_t ByteSzV, typename RefCountT>
auto to_array_view(maybe_array<T, ByteSzV, RefCountT> & sa) { return sa.to_array_view(); }

}

#endif // SONIA_UTILITY_OPTIMIZED_MAYBE_ARRAY_HPP
