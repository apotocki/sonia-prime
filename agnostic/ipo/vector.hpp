// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_SAME
#   include "agnostic/std/type_traits/is_same.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_CV
#   include "agnostic/std/type_traits/remove_cv.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#   include "agnostic/std/type_traits/remove_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ENABLE_IF
#   include "agnostic/std/type_traits/enable_if.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF
#   include "agnostic/std/type_traits/alignment_of.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#   include "agnostic/std/memory/allocator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR
#   include "agnostic/std/iterator/reverse_iterator.hpp"
#endif

#include "agnostic/adjacent_buffer.hpp"
#include "agnostic/utility/static_log2.hpp"
#include "agnostic/utility/ceiling.hpp"

#include "agnostic/allocator/BareAllocator.hpp"
#include "agnostic/intrusive/find_option.hpp"
//#include "agnostic/intrusive/

#include "in_place_capacity.hpp"
#include "holder.hpp"

namespace agnostic {

namespace vector_detail {

template <typename T>
struct vector_buffer_base
{
    size_t current_size;

    void destroy() noexcept
    {
        std::destroy(derived().begin(), derived().begin() + current_size);
    }

    inline T* data_begin() noexcept { return derived().begin(); }
    inline T* data_end() noexcept { return derived().begin() + current_size; }

private:
    using derived_t = adjacent_buffer<T, vector_buffer_base>;
    inline derived_t& derived() noexcept { return static_cast<derived_t&>(*this); }
};

template <typename T, intrusive::Option ... OptionsT>
struct helper
{
    using ipo_descriptor_t = intrusive::find_option_t<in_place_capacity_tag, in_place_capacity<0>, OptionsT...>;

    static constexpr size_t ipo_capacity_sz = ipo_descriptor_t::value;
    static constexpr size_t szbits = ipo_capacity_sz ? 1 + static_log2_v<ipo_capacity_sz ? ipo_capacity_sz : 1> : 0;
    static constexpr size_t begin_offs = ((szbits + 1) + (CHAR_BIT - 1)) / CHAR_BIT;
    static constexpr size_t alv = ipo_capacity_sz ? std::alignment_of_v<T> : 1;
    static constexpr size_t aligned_offs = ceiling_v<begin_offs, alv>;
    static constexpr size_t estimated_holder_sz = aligned_offs + sizeof(T) * ipo_capacity_sz;
    static constexpr size_t holder_sz = estimated_holder_sz < sizeof(uintptr_t) ? sizeof(uintptr_t) : estimated_holder_sz;

    using holder_t = ipo_holder<holder_sz, szbits>;

    using vector_buffer = adjacent_buffer<T, vector_detail::vector_buffer_base<T>>;
};

}

template <
    typename T,
    BareAllocator AllocatorT = default_bare_allocator,
    intrusive::Option ... OptionsT
>
class vector
    : vector_detail::helper<T, OptionsT...>::holder_t
    , AllocatorT
{
    using helper_t = vector_detail::helper<T, OptionsT...>;
    using holder_t = typename helper_t::holder_t;
    static constexpr size_t ipo_capacity_sz = helper_t::ipo_capacity_sz;
    static constexpr size_t MIN_DYNAMIC_STORE_SZ = 15;

public:
    using value_type = T;
    using allocator_type = AllocatorT;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = T*;
    using const_iterator = T const*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() noexcept(noexcept(AllocatorT()))
    {
        holder_t::init_not_ptr(); // also set size bits to 0;
    }

    explicit vector(AllocatorT const& alloc) noexcept
        : AllocatorT{alloc}
    {
        holder_t::init_not_ptr(); // also set size bits to 0;
    }

    explicit vector(size_type count, AllocatorT const& alloc = AllocatorT{})
        : AllocatorT{ alloc }
    {
        holder_t::init_not_ptr();
        resize(count);
    }

    explicit vector(size_type count, T const& value, AllocatorT const& alloc = AllocatorT{})
        : AllocatorT{ alloc }
    {
        holder_t::init_not_ptr();
        resize(count, value);
    }

    vector(std::initializer_list<T> init, AllocatorT const& alloc = AllocatorT{})
        : AllocatorT{ alloc }
    {
        holder_t::init_not_ptr();
        value_type* data = do_expand(init.size());
        std::uninitialized_move(init.begin(), init.end(), data);
    }

    template <class InputIt>
    vector(InputIt first, InputIt last, AllocatorT const& alloc = AllocatorT{})
        : AllocatorT{ alloc }
    {
        holder_t::init_not_ptr();
        if constexpr (std::random_access_iterator<InputIt>) {
            size_type sz = last - first;
            value_type* place = do_expand(sz);
            std::uninitialized_copy(first, last, place);
        } else {
            value_type* place = do_expand(ipo_capacity_sz);
            for (size_t cnt = 0; cnt != ipo_capacity_sz; ++place) {
                new (place) value_type{*first};
                ++first;
                ++cnt;
                if (first == last) {
                    holder_t::set_service_cookie(cnt);
                    return;
                }
            }
            for (; first != last; ++first) {
                emplace_back(*first);
            }
        }
    }

    vector(vector const& rhs)
        : AllocatorT(rhs.get_allocator())
    {
        if (rhs.is_ptr()) {
            auto * rhsbuff = rhs.get_buffer();
            typename helper_t::vector_buffer* buff = allocate_adjacent_buffer<value_type, vector_detail::vector_buffer_base<T>>(get_allocator(), rhsbuff->capacity(), rhsbuff->current_size);
            std::uninitialized_copy(rhsbuff->data_begin(), rhsbuff->data_end(), buff->begin());
            holder_t::set_ptr(buff);
        } else {
            holder_t::init_not_ptr();
            auto sz = rhs.get_service_cookie();
            std::uninitialized_copy(rhs.inplace_begin(), rhs.inplace_begin() + sz, inplace_begin());
            holder_t::set_service_cookie(sz);
        }
    }

    vector(vector && rhs) noexcept
        : AllocatorT(std::move(rhs.get_allocator()))
    {
        if (rhs.is_ptr()) {
            holder_t::set_ptr(rhs.get_buffer());
            rhs.set_not_ptr();
            rhs.set_service_cookie(0);
        } else {
            holder_t::init_not_ptr();
            auto sz = rhs.get_service_cookie();
            static_assert(std::is_nothrow_move_constructible_v<value_type>);
            std::uninitialized_move(rhs.inplace_begin(), rhs.inplace_begin() + sz, inplace_begin());
            holder_t::set_service_cookie(sz);
        }
    }

    vector & operator= (vector const& rhs)
    {
        vector tmp{rhs};
        swap(tmp);
        return *this;
    }

    vector& operator= (vector && rhs) noexcept
    {
        vector tmp{ std::move(rhs) };
        swap(tmp);
        return *this;
    }

    void swap(vector & x) noexcept
    {
        static_assert(std::is_nothrow_move_constructible_v<value_type>);
        std::swap(static_cast<AllocatorT&>(*this), static_cast<AllocatorT&>(x));
        if (holder_t::is_ptr()) {
            auto* tmp = get_buffer();
            if (x.is_ptr()) {
                holder_t::set_ptr(x.get_buffer());
            } else {
                holder_t::init_not_ptr();
                auto sz = x.get_service_cookie();
                std::uninitialized_move(x.inplace_begin(), x.inplace_begin() + sz, inplace_begin());
                std::destroy(x.inplace_begin(), x.inplace_begin() + sz);
                holder_t::set_service_cookie(sz);
            }
            x.set_ptr(tmp);
        } else {
            auto sz = holder_t::get_service_cookie();
            if (x.is_ptr()) {
                auto* tmp = x.get_buffer();
                x.init_not_ptr();
                auto sz = holder_t::get_service_cookie();
                std::uninitialized_move(inplace_begin(), inplace_begin() + sz, x.inplace_begin());
                std::destroy(inplace_begin(), inplace_begin() + sz);
                x.set_service_cookie(sz);
                holder_t::set_ptr(tmp);
            } else {
                auto xsz = x.get_service_cookie();
                size_type minsz = (std::min)(sz, xsz);
                std::swap_ranges(inplace_begin(), inplace_begin() + minsz, x.inplace_begin());
                if (minsz < sz) {
                    std::uninitialized_move(inplace_begin() + minsz, inplace_begin() + sz, x.inplace_begin() + minsz);
                    std::destroy(inplace_begin() + minsz, inplace_begin() + sz);
                } else if (minsz < xsz) {
                    std::uninitialized_move(x.inplace_begin() + minsz, x.inplace_begin() + xsz, inplace_begin() + minsz);
                    std::destroy(x.inplace_begin() + minsz, x.inplace_begin() + xsz);
                }
                holder_t::set_service_cookie(xsz);
                x.set_service_cookie(sz);
            }
        }
    }

    ~vector() noexcept
    {
        if (holder_t::is_ptr()) {
            deallocate_adjacent_buffer(get_allocator(), get_buffer());
        } else {
            auto sz = holder_t::get_service_cookie();
            std::destroy(inplace_begin(), inplace_begin() + sz);
        }
    }

    inline bool is_inplace() const noexcept { return !holder_t::is_ptr(); }

    size_type capacity() const noexcept
    {
        return holder_t::is_ptr() ? get_buffer()->capacity() : ipo_capacity_sz;
    }

    size_t size() const noexcept
    {
        if (holder_t::is_ptr()) {
            return get_buffer()->current_size;
        }
        return static_cast<size_t>(holder_t::get_service_cookie());
    }

    inline bool empty() const noexcept { return 0 == size(); }

    value_type * begin() noexcept { return holder_t::is_ptr() ? get_buffer()->data_begin() : inplace_begin(); }
    const value_type * begin() const noexcept { return holder_t::is_ptr() ? get_buffer()->data_begin() : inplace_begin(); }
    const value_type * cbegin() const noexcept { return begin(); }
    value_type* end() noexcept { return holder_t::is_ptr() ? get_buffer()->data_end() : inplace_begin() + holder_t::get_service_cookie(); }
    const value_type* end() const noexcept { return holder_t::is_ptr() ? get_buffer()->data_end() : inplace_begin() + holder_t::get_service_cookie(); }
    const value_type* cend() const noexcept { return end(); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    value_type const& front() const noexcept { return *begin(); }
    value_type & front() noexcept{ return *begin(); }
    value_type const& back() const noexcept { return *(end() - 1); }
    value_type & back() noexcept { return *(end() - 1); }

    value_type const& operator[](size_type idx) const { return begin()[idx]; }
    value_type & operator[](size_type idx) { return begin()[idx]; }

    void clear()
    {
        size_type cursz = size();
        if (cursz) {
            do_truncate(cursz);
        }
    }

    void reserve(size_type at_least_sz)
    {
        static_assert(std::is_nothrow_move_constructible_v<value_type>);

        if (holder_t::is_ptr()) {
            auto * buff = get_buffer();
            if (buff->capacity() >= at_least_sz) return;
        } else if (ipo_capacity_sz < at_least_sz) {
            auto sz = holder_t::get_service_cookie();
            auto * buff = allocate_adjacent_buffer<value_type, vector_detail::vector_buffer_base<T>>(get_allocator(), at_least_sz, sz);
            std::uninitialized_move(inplace_begin(), inplace_begin() + sz, buff->begin());
            std::destroy(inplace_begin(), inplace_begin() + sz);
            holder_t::set_ptr(buff);
        }
    }

    void resize(size_type sz, const_reference c)
    {
        size_type cursz = size();
        if (cursz < sz) {
            size_type cnt = sz - cursz;
            value_type* place = do_expand(cnt);
            std::uninitialized_fill(place, place + cnt, c);
        } else if (cursz > sz) {
            do_truncate(cursz - sz);
        }
    }

    void resize(size_type sz)
    {
        size_type cursz = size();
        if (cursz < sz) {
            size_type cnt = sz - cursz;
            value_type* place = do_expand(cnt);
            std::uninitialized_value_construct(place, place + cnt);
        } else if (cursz > sz) {
            do_truncate(cursz - sz);
        }
    }

    void push_back(T const& value)
    {
        emplace_back(value);
    }

    void push_back(T&& value)
    {
        emplace_back(std::move(value));
    }

    template <typename ... ArgsT>
    void emplace_back(ArgsT&& ... args)
    {
        value_type* place = do_expand(1);
        try {
            new (place) value_type(std::forward<ArgsT>(args)...);
        } catch (...) {
            do_update_size(-1);
            throw;
        }
    }

    inline void pop_back() noexcept
    {
        do_truncate(1);
    }

    inline AllocatorT get_allocator() const noexcept
    {
        return *this;
    }

private:
    value_type* inplace_begin() noexcept { return std::launder(reinterpret_cast<value_type*>(holder_t::data() + helper_t::aligned_offs)); }
    value_type const* inplace_begin() const noexcept { return std::launder(reinterpret_cast<value_type const*>(holder_t::data() + helper_t::aligned_offs)); }

    value_type* do_expand(size_type count)
    {
        if (holder_t::is_ptr()) {
            auto * buff = get_buffer();
            size_type sz = buff->current_size;
            if (buff->capacity() >= sz + count) {
                buff->current_size += count;
                return buff->data_begin() + sz;
            } else {
                typename helper_t::vector_buffer* nextbuff = do_create_buffer(sz, count, buff->data_begin(), buff->data_end());
                deallocate_adjacent_buffer(get_allocator(), buff);
                holder_t::set_ptr(nextbuff);
                return nextbuff->data_begin() + sz;
            }
        } else {
            auto sz = holder_t::get_service_cookie();
            if (sz + count <= ipo_capacity_sz) {
                do_update_inplace_size(count);
                return inplace_begin() + sz;
            } else {
                typename helper_t::vector_buffer* buff = do_create_buffer(sz, count, inplace_begin(), inplace_begin() + sz);
                std::destroy(inplace_begin(), inplace_begin() + sz);
                holder_t::set_ptr(buff);
                return buff->begin() + sz;
            }
        }
    }

    void do_truncate(size_type count) noexcept
    {
        if (holder_t::is_ptr()) {
            auto* buff = get_buffer();
            buff->current_size -= count;
            std::destroy(buff->data_end(), buff->data_end() + count);
        } else {
            auto sz = holder_t::get_service_cookie() - count;
            std::destroy(inplace_begin() + sz, inplace_begin() + sz + count);
            holder_t::set_service_cookie(sz);
        }
    }

    typename helper_t::vector_buffer* do_create_buffer(size_type cursz, size_type appsz, value_type* srcb, value_type* srce)
    {
        size_type ressz = (std::max)(static_cast<size_type>(cursz) * 2, (static_cast<size_type>(MIN_DYNAMIC_STORE_SZ + appsz)));
        typename helper_t::vector_buffer* buff = allocate_adjacent_buffer<value_type, vector_detail::vector_buffer_base<T>>(get_allocator(), ressz, cursz + appsz);
        static_assert(std::is_nothrow_move_constructible_v<value_type>);
        std::uninitialized_move(srcb, srce, buff->begin());
        return buff;
    }

    void do_update_size(int dsz) noexcept
    {
        if (holder_t::is_ptr()) {
            get_buffer()->current_size += dsz;
        } else {
            holder_t::set_service_cookie(holder_t::get_service_cookie() + dsz);
        }
    }

    template <typename DSZT>
    inline void do_update_inplace_size(DSZT dsz) noexcept
    {
        holder_t::set_service_cookie(holder_t::get_service_cookie() + dsz);
    }

    inline typename helper_t::vector_buffer* get_buffer() const noexcept
    {
        return reinterpret_cast<typename helper_t::vector_buffer*>(holder_t::as_ptr());
    }
};

template <typename T, typename A1, typename A2, typename ... Args1, typename ... Args2>
std::strong_ordering operator<=>(const vector<T, A1, Args1...>& lhs,  const vector<T, A2, Args2...> & rhs)
{
    auto le = lhs.end();
    auto re = rhs.end();
    auto [lit, rit] = std::mismatch(lhs.begin(), le, rhs.begin(), re);
    if (lit == le) {
        return (rit == re) ? std::strong_ordering::equivalent : std::strong_ordering::less;
    } else if (rit == re) {
        return std::strong_ordering::greater;
    }
    return *lit <=> *rit;
}

template <typename T, typename A1, typename A2, typename ... Args1, typename ... Args2>
bool operator==(const vector<T, A1, Args1...>& lhs, const vector<T, A2, Args2...>& rhs)
{
    return lhs.size() == rhs.size() && operator<=>(lhs, rhs) == std::strong_ordering::equivalent;
}

}
