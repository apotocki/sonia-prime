//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_BUFFER_HPP
#define SONIA_UTILITY_BUFFER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include "sonia/type_traits.hpp"
#include "sonia/array_view.hpp"

namespace sonia {

template <class ABT>
constexpr size_t adjacent_buffer_offset = (sizeof(ABT) + ABT::type_alignment - 1) / ABT::type_alignment;

template <typename T, typename BaseT>
class adjacent_buffer : public BaseT {
public:
    typedef T value_type;
    typedef size_t size_type;
    static const size_t type_alignment = alignment_of_v<T>;

    template <typename RngT, typename ... ArgsT>
    explicit adjacent_buffer(size_t sz, RngT rng, ArgsT&& ... args)
        : BaseT(std::forward<ArgsT>(args) ...), sz_(sz)
    {
        if constexpr (is_pod_v<T> && is_same_v<remove_cv_t<range_value_t<RngT>>, T> && is_pointer_v<range_iterator_t<RngT>>) {
            std::memcpy(begin(), rng.begin(), sizeof(T) * sz);
        } else if constexpr (is_pod_v<T> && is_same_v<remove_cv_t<range_value_t<RngT>>, T>) {
            std::copy_n(rng.begin(), sz, begin());
        } else {
            auto s = rng.begin();
            T * b = begin();
            for (; sz != 0; ++b, ++s, --sz) {
                new (b) T(*s);
            }
        }
    }

    adjacent_buffer(adjacent_buffer const& rhs) = delete;
    adjacent_buffer(adjacent_buffer && rhs) = delete;
    adjacent_buffer & operator= (adjacent_buffer const&) = delete;
    adjacent_buffer & operator= (adjacent_buffer &&) = delete;
    
    ~adjacent_buffer() {
        if constexpr (!is_trivially_destructible_v<T>) {
            std::for_each(begin(), end(), [](T & obj) { obj.~T(); });
        }
    }

    T * begin() noexcept { return reinterpret_cast<T*>(this) + adjacent_buffer_offset<adjacent_buffer>; }
    T const* begin() const noexcept { return reinterpret_cast<T const*>(this) + adjacent_buffer_offset<adjacent_buffer>; }
    T const* cbegin() const noexcept { return begin(); }

    T* end() noexcept { return begin() + sz_; }
    T const* end() const noexcept { return begin() + sz_; }
    T const* cend() const noexcept { return end(); }
    
    T& front() noexcept { return *begin(); }
    T const& front() const noexcept { return *begin(); }
    
    T& back() noexcept { return *(end() - 1); }
    T const& back() const noexcept { return *(end() - 1); }
    
    T& operator[](size_t ind) noexcept { return begin()[ind]; }
    T const& operator[](size_t ind) const noexcept { return begin()[ind]; }
    
    array_view<T> to_array_view() noexcept { return array_view<T>(begin(), sz_); }
    array_view<const T> to_array_view() const noexcept { return array_view<const T>(begin(), sz_); }

    size_type size() const noexcept { return sz_; }
    bool empty() const noexcept { return 0 == sz_; }
    bool operator!() const noexcept { return empty(); }

private:
    size_type sz_;
};

template <typename T, typename BaseT, typename AllocatorT, typename ... ArgsT>
adjacent_buffer<T, BaseT> * allocate_adjacent_buffer(size_t sz, ArgsT&& ... args) {
    typedef typename AllocatorT::template rebind<T>::other allocator_type;
    typedef adjacent_buffer<T, BaseT> buffer_t;

    allocator_type a;
    T * raw_ptr = a.allocate(adjacent_buffer_offset<buffer_t> + sz);
    buffer_t * ptr = reinterpret_cast<buffer_t*>(raw_ptr);
    try {
        return new (ptr) buffer_t(sz, std::forward<ArgsT>(args) ...);
    } catch (...) {
        a.deallocate(raw_ptr, adjacent_buffer_offset<buffer_t> + sz);
        throw;
    }
}

template <typename T, typename BaseT, typename AllocatorT>
void deallocate_adjacent_buffer(adjacent_buffer<T, BaseT> * ptr) noexcept {
    typedef typename AllocatorT::template rebind<T>::other allocator_type;
    typedef adjacent_buffer<T, BaseT> buffer_t;
    allocator_type a;
    a.deallocate(reinterpret_cast<T*>(ptr), adjacent_buffer_offset<buffer_t> + ptr->size());
}

}

#endif // SONIA_UTILITY_BUFFER_HPP
