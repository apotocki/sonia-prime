// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR

#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/std/type_traits/is_pointer.hpp"
#include "agnostic/std/utility/move.hpp"

namespace std {

template <class IterT>
class reverse_iterator
{
public:
    using iterator_type = IterT;
    using iterator_category = typename iterator_traits<IterT>::iterator_category;
    using value_type = typename iterator_traits<IterT>::value_type;
    using difference_type = typename iterator_traits<IterT>::difference_type;
    using pointer = typename iterator_traits<IterT>::pointer;
    using reference = typename iterator_traits<IterT>::reference;

    constexpr reverse_iterator() = default;
    
    constexpr explicit reverse_iterator(IterT it)
        : current{move(it)}
    {}

    template <class OtherIterT>
    constexpr explicit reverse_iterator(reverse_iterator<OtherIterT> const& rhs)
        : current{rhs.base()}
    {}

    template <class OtherIterT>
    constexpr reverse_iterator& operator=(reverse_iterator<OtherIterT> const& rhs)
    {
        current = rhs.base();
        return *this;
    }

    constexpr IterT base() const { return current; }

    constexpr reference operator*() const
    {
        IterT tmp = current;
        return *--tmp;
    }

    constexpr pointer operator->() const
    {
        IterT tmp = current;
        --tmp;
        if constexpr is_pointer_v<IterT> {
            return tmp;
        } else {
            return tmp.operator->();
        }
    }

    constexpr reverse_iterator& operator++()
    {
        --current;
        return *this;
    }

    constexpr reverse_iterator operator++(int)
    {
        reverse_iterator tmp = *this;
        --current;
        return tmp;
    }

    constexpr reverse_iterator& operator--()
    {
        ++current;
        return *this;
    }

    constexpr reverse_iterator operator--(int)
    {
        reverse_iterator tmp = *this;
        ++current;
        return tmp;
    }

    constexpr reverse_iterator& operator+=(const difference_type val)
    {
        current -= val;
        return *this;
    }

    constexpr reverse_iterator operator+(const difference_type val) const
    {
        return reverse_iterator(current - val);
    }

    constexpr reverse_iterator& operator-=(const difference_type val)
    {
        current += val;
        return *this;
    }

    constexpr reverse_iterator operator-(const difference_type val) const
    {
        return reverse_iterator(current + val);
    }

    reference operator[](const difference_type val) const
    {
        return current[static_cast<difference_type>(-val - 1)];
    }

protected:
    IterT current{};
};

template <class T1, class T2>
constexpr bool operator== (reverse_iterator<T1> const& l, reverse_iterator<T2> const& r)
{
    return l.base() == r.base();
}

template <class T1, class T2>
constexpr bool operator!= (reverse_iterator<T1> const& l, reverse_iterator<T2> const& r)
{
    return l.base() != r.base();
}

template <class Iter>
constexpr reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it)
{
    return it + n;
}

template <class Iterator1, class Iterator2>
constexpr auto operator-(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
    -> decltype(rhs.base() - lhs.base())
{
    return rhs.base() - lhs.base();
}

template< class Iter >
constexpr std::reverse_iterator<Iter> make_reverse_iterator(Iter i)
{
    return std::reverse_iterator<Iter>(i);
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
