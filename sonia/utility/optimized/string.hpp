//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIMIZED_STRING_HPP
#define SONIA_UTILITY_OPTIMIZED_STRING_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <memory>

#include "sonia/string.hpp"
#include "sonia/utility/optimized/holder.hpp"
#include "sonia/utility/buffer.hpp"

namespace sonia {

// inheritance sequence: adjacent_buffer -> DerivedBaseT -> optimized_array_base -> optimized_base

template <typename T, class DerivedBaseT = void, class AllocatorT = std::allocator<T>>
class optimized_array_base : public optimized_base
{
public:
    optimized_base * clone() const override;
    void dispose() noexcept override;
};

template <typename T, class DerivedBaseT, class AllocatorT>
optimized_base * optimized_array_base<T, DerivedBaseT, AllocatorT>::clone() const {
    typedef conditional_t<is_same_v<void, DerivedBaseT>, optimized_array_base, DerivedBaseT> base_derived_t;
    typedef adjacent_buffer<T, base_derived_t> derived_t;
    derived_t const& self = static_cast<derived_t const&>(*this);
    return allocate_adjacent_buffer<T, base_derived_t, AllocatorT>(self.size(), self.to_array_view(), self);
}

template <typename T, class DerivedBaseT, class AllocatorT>
void optimized_array_base<T, DerivedBaseT, AllocatorT>::dispose() noexcept {
    typedef conditional_t<is_same_v<void, DerivedBaseT>, optimized_array_base, DerivedBaseT> base_derived_t;
    typedef adjacent_buffer<T, base_derived_t> derived_t;
    derived_t & self = static_cast<derived_t&>(*this);
    self.~derived_t();
    deallocate_adjacent_buffer<T, base_derived_t, AllocatorT>(&self);
}

template <class HolderT>
struct optimized_string_impl
{
    typedef std::allocator<char> allocator_t;
    typedef optimized_array_base<char, void, allocator_t> optimized_array_base_t;
    typedef adjacent_buffer<char, optimized_array_base_t> optimized_string_t;

    static const size_t gmaxsz = sizeof(HolderT) - HolderT::begin_offs; // guessed
    static const size_t szbitscnt = boost::static_log2<gmaxsz>::value + 1;
    static const size_t sz_mask = (1 << (szbitscnt - 1)) - 1;
    static const size_t charoffs = (HolderT::used_bits + szbitscnt + CHAR_BIT - 1) / CHAR_BIT;
    static const size_t maxsz = sizeof(HolderT) - charoffs;

    static char const* begin(HolderT const* h) noexcept { return (char const*)(h->data() + charoffs); }
    static char * begin(HolderT * h) noexcept { return (char*)(h->data() + charoffs); }
    static char const* end(HolderT const* h) { return (char const*)h->end(); }
    static char * end(HolderT * h) { return (char*)h->end(); }

    static void init(HolderT * self, string_view s) {
        if (s.size() <= maxsz) {
            self->set_uint(1 | (s.size() << HolderT::used_bits));
            std::memcpy(begin(self), s.begin(), s.size());
        } else {
            optimized_string_t * ptr = allocate_adjacent_buffer<char, optimized_array_base_t, allocator_t>(s.size(), s);
            self->set_pointer(ptr);
        }
    }

    static string_view get(HolderT const* self) {
        return self->is_ptr() ? 
              (string_view)ptr(self)->to_array_view()
            : string_view(begin(self), (self->get_uint() >> HolderT::used_bits) & sz_mask);
    }

    /*
    static void set(HolderT * self, string_view val) {
        if (self->is_ptr()) {
            ptr(self)->set(std::forward<T>(val));
        } else if () {

        }
    }
    */
private:
    static optimized_string_t * ptr(HolderT * self) {
        return static_cast<optimized_string_t *>(self->get_pointer());
    }

    static optimized_string_t const* ptr(HolderT const* self) {
        return static_cast<optimized_string_t const*>(self->get_pointer());
    }
};

}

#endif // SONIA_UTILITY_OPTIMIZED_STRING_HPP
