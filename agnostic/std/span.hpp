// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_SPAN_HPP
#define AGNOSTIC_STD_SPAN_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR
#   include "iterator/reverse_iterator.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_CV
#   include "type_traits/remove_cv.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#   include "type_traits/remove_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_TYPE_IDENTITY
#   include "type_traits/type_identity.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_TO_ADDRESS
#   include "memory/to_address.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ARRAY
#   include "array.hpp"
#endif

namespace std {

inline constexpr size_t dynamic_extent = -1;

namespace span_detail {

template <size_t EV>
struct extent_holder
{
    constexpr extent_holder() noexcept = default;
    constexpr explicit extent_holder(size_t) noexcept {};
    [[nodiscard]] constexpr size_t size() const noexcept { return EV; }
};

template <>
struct extent_holder<dynamic_extent>
{
    constexpr extent_holder() noexcept = default;
    constexpr explicit extent_holder(size_t) noexcept : size_{0} {};
    [[nodiscard]] constexpr size_t size() const noexcept { return size_; }

private:
    size_t size_{0};
};

}

template <class ElementT, size_t Extent = dynamic_extent>
class span : span_detail::extent_holder<Extent>
{
public:
    // constants and types
    using element_type = ElementT;
    using value_type = remove_cv_t<ElementT>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = element_type*;
    using const_pointer = const element_type*;
    using reference = element_type&;
    using const_reference = const element_type&;
    using iterator = element_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    static constexpr size_type extent = Extent;

    // constructors, copy, and assignment
    constexpr span() noexcept : data_{nullptr}, size_{0} {}

    template <class It>
    constexpr span(It first, size_type count) : data_{to_address(first)}, size_{count} {}

    template <class It, class End>
    constexpr span(It first, End last) : data_{to_address(first)}, size_{last - first} {}

    template <size_t N>
    constexpr span(element_type(&arr)[N]) noexcept : data_{arr}, size_{N} {}

    template <class T, size_t N>
    constexpr span(array<T, N>& arr) noexcept : data_{arr.data()}, size_{N} {}

    template <class T, size_t N>
    constexpr span(const array<T, N>& arr) noexcept : data_{ arr.data() }, size_{ N } {}

    template<class R>
    constexpr span(R&& r) : data_ {::std::data(r), }

    constexpr span(const span& other) noexcept = default;
    template<class OtherElementT, size_t OtherExtent>
    constexpr explicit(/* see description */)
        span(const span<OtherElementT, OtherExtent>& s) noexcept;

    ~span() noexcept = default;

    constexpr span& operator=(const span& other) noexcept = default;

    // subviews
    template<size_t Count>
    constexpr span<element_type, Count> first() const;
    template<size_t Count>
    constexpr span<element_type, Count> last() const;
    template<size_t Offset, size_t Count = dynamic_extent>
    constexpr span<element_type, /* see description */> subspan() const;

    constexpr span<element_type, dynamic_extent> first(size_type count) const;
    constexpr span<element_type, dynamic_extent> last(size_type count) const;
    constexpr span<element_type, dynamic_extent> subspan(
        size_type offset, size_type count = dynamic_extent) const;

    // observers
    constexpr size_type size() const noexcept;
    constexpr size_type size_bytes() const noexcept;
    [[nodiscard]] constexpr bool empty() const noexcept;

    // element access
    constexpr reference operator[](size_type idx) const;
    constexpr reference front() const;
    constexpr reference back() const;
    constexpr pointer data() const noexcept;

    // iterator support
    constexpr iterator begin() const noexcept;
    constexpr iterator end() const noexcept;
    constexpr reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rend() const noexcept;

private:
    pointer data_;              // exposition only
    size_type size_;            // exposition only
};

template<class It, class EndOrSize>
span(It, EndOrSize)->span<remove_reference_t<iter_reference_t<It>>>;
template<class T, size_t N>
span(T(&)[N])->span<T, N>;
template<class T, size_t N>
span(array<T, N>&)->span<T, N>;
template<class T, size_t N>
span(const array<T, N>&)->span<const T, N>;
template<class R>
span(R&&)->span<remove_reference_t<ranges::range_reference_t<R>>>;

}

#endif // AGNOSTIC_STD_SPAN_HPP
