//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <utility>
#include <memory>
#include <span>

#include "sonia/type_traits.hpp"
#include "sonia/array_view.hpp"
#include "sonia/utility/algorithm/construct.hpp"

namespace sonia {

template <class ABT>
constexpr size_t adjacent_buffer_offset_v = ceiling_v<sizeof(ABT), ABT::type_alignment>;

template <typename T, typename BaseT>
class adjacent_buffer : public BaseT
{
public:
    using value_type = T ;
    using size_type = size_t;
    static constexpr size_t type_alignment = alignment_of_v<T>;

    template <typename RngT, typename ... ArgsT>
    explicit adjacent_buffer(size_t sz, RngT && rng, ArgsT&& ... args)
        : BaseT(std::forward<ArgsT>(args) ...), sz_(sz)
    {
        sonia::construct(begin(), std::forward<RngT>(rng), sz);
    }

    template <typename ... ArgsT>
    adjacent_buffer(size_t sz, in_place_t, ArgsT&& ... args)
        : BaseT(std::forward<ArgsT>(args) ...), sz_(sz)
    {
        sonia::construct(begin(), sz);
    }

    adjacent_buffer(adjacent_buffer const& rhs) = delete;
    adjacent_buffer(adjacent_buffer && rhs) = delete;
    adjacent_buffer & operator= (adjacent_buffer const&) = delete;
    adjacent_buffer & operator= (adjacent_buffer &&) = delete;
    
    ~adjacent_buffer()
    {
        if constexpr (!is_trivially_destructible_v<T>) {
            std::for_each(begin(), end(), [](T & obj) { obj.~T(); });
        }
    }

    T * begin() noexcept { return reinterpret_cast<T*>(((char*)this) + adjacent_buffer_offset_v<adjacent_buffer>); }
    T const* begin() const noexcept { return reinterpret_cast<T const*>(((char const*)this) + adjacent_buffer_offset_v<adjacent_buffer>); }
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
    
    operator std::span<T> () noexcept { return std::span<T>(begin(), sz_); }
    operator std::span<const T> () const noexcept { return std::span<const T>(begin(), sz_); }

    std::span<T> to_span() noexcept { return std::span<T>(begin(), sz_); }
    std::span<const T> to_span() const noexcept { return std::span<const T>(begin(), sz_); }

    template <typename OffsT>
    array_view<T> subview(OffsT offset) { return to_subview(*this, offset); }

    template <typename OffsT>
    array_view<const T> subview(OffsT offset) const { return to_subview(*this, offset); }

    template <typename OffsT>
    array_view<T> subview(OffsT offset, size_t sz) { return to_subview(*this, offset, sz); }

    template <typename OffsT>
    array_view<const T> subview(OffsT offset, size_t sz) const { return to_subview(*this, offset, sz); }

    size_type size() const noexcept { return sz_; }
    bool empty() const noexcept { return 0 == sz_; }
    bool operator!() const noexcept { return empty(); }

private:
    size_type sz_;
};

template <typename T, typename BaseT, typename AllocatorT, typename ... ArgsT>
adjacent_buffer<T, BaseT> * allocate_adjacent_buffer(AllocatorT const& alloc, size_t sz, ArgsT&& ... args)
{
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<char>;
    using buffer_t = adjacent_buffer<T, BaseT>;

	allocator_type a{alloc};
    char * raw_ptr = a.allocate(adjacent_buffer_offset_v<buffer_t> + sz * sizeof(T));
    buffer_t * ptr = reinterpret_cast<buffer_t*>(raw_ptr);
    try {
        return new (ptr) buffer_t(sz, std::forward<ArgsT>(args) ...);
    } catch (...) {
        a.deallocate(raw_ptr, adjacent_buffer_offset_v<buffer_t> + sz * sizeof(T));
        throw;
    }
}

template <typename AllocatorT, typename T, typename BaseT>
void deallocate_adjacent_buffer(AllocatorT const& alloc, adjacent_buffer<T, BaseT> * ptr) noexcept
{
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<char>;
    using buffer_t = adjacent_buffer<T, BaseT>;
	size_t sz = adjacent_buffer_offset_v<buffer_t> + ptr->size() * sizeof(T);
	std::destroy_at(ptr);
	allocator_type a{alloc};
    a.deallocate(reinterpret_cast<char*>(ptr), sz);
}

}

