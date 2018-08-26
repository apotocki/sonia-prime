//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIMIZED_HOLDER_HPP
#define SONIA_UTILITY_OPTIMIZED_HOLDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/assert.hpp>
#include <boost/integer/static_log2.hpp> 
#include <boost/integer.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/functional/reference_wrapper.hpp"

#define SONIA_OPTIMIZED_TYPE_ALIGNMENT sizeof(void*) // to be able to store ponters directly in first holder bytes

namespace sonia {

class optimized_base {
public:
    optimized_base() : refs_(1) {}
    optimized_base(optimized_base const& rhs) : service_cookie_(rhs.service_cookie_), refs_(1) {}
    optimized_base(optimized_base && rhs) = delete;

    virtual ~optimized_base() {}

    optimized_base & operator= (optimized_base const&) = delete;
    optimized_base & operator= (optimized_base &&) = delete;

    size_t service_cookie() const { return service_cookie_; }
    size_t& service_cookie() { return service_cookie_; }

    void add_ref() {
        ++refs_;
    }

    void release() {
        if (--refs_ == 0) {
            dispose();
        }
    }

    bool unique() const noexcept { return 1 == refs_; }

    virtual optimized_base * clone() const = 0;
    virtual void dispose() noexcept = 0;

private:
    size_t service_cookie_;
    long refs_;
};

template <class WrappedT>
class optimized_wrapper : public optimized_base
{
public:
    template <typename ArgT>
    explicit optimized_wrapper(ArgT && arg)
        : val_(std::forward<ArgT>(arg))
    { }

    WrappedT const& get() const {
        return val_;
    }

    template <typename T>
    void set(T && arg) {
        val_ = std::forward<T>(arg);
    }

    void dispose() noexcept override {
        delete this;
    }

    optimized_base * clone() const override {
        return new optimized_wrapper(*this);
    }

private:
    WrappedT val_;
};

template <size_t HolderBytesV, size_t ServiceCookieBitsV>
struct optimized_holder_base {
    static const size_t used_bits = ServiceCookieBitsV + 1;

    static_assert (HolderBytesV >= sizeof(intptr_t));
    static_assert (sizeof(intptr_t) * CHAR_BIT - 1 >= ServiceCookieBitsV);

    static const size_t begin_offs = (ServiceCookieBitsV + CHAR_BIT) / CHAR_BIT;

    static const size_t max_cookie_val = (((size_t)1) << ServiceCookieBitsV) - 1;

    static const int first_byte_bits = ServiceCookieBitsV < (CHAR_BIT - 1) ? ServiceCookieBitsV : (CHAR_BIT - 1);
    static const uint8_t first_byte_mask = (((uint8_t)1) << first_byte_bits) - 1;

    aligned_storage_t<HolderBytesV, SONIA_OPTIMIZED_TYPE_ALIGNMENT> holder_;

    void init_not_ptr() noexcept { *data() = 1; }
    void set_not_ptr() noexcept { *data() |= 1; }

    uint8_t const* data() const noexcept { return reinterpret_cast<uint8_t const*>(&holder_); }
    uint8_t * data() noexcept { return reinterpret_cast<uint8_t*>(&holder_); }
    bool is_ptr() const noexcept { return !(1 & *data()); }
    
    uint8_t const* begin() const { return data() + begin_offs; }
    uint8_t * begin() { return data() + begin_offs; }
    uint8_t const* end() const { return data() + HolderBytesV; }
    uint8_t * end() { return data() + HolderBytesV; }

    size_t get_service_cookie() const {
        if constexpr (ServiceCookieBitsV <= first_byte_bits) {
            return first_byte_mask & ((*data()) >> 1);
        } else {
            return get_service_cookie_adv();
        }
    }

    size_t get_service_cookie_adv() const {
        uint8_t const* src = data();
        size_t res = first_byte_mask & ((*src) >> 1);
        size_t sbits = ServiceCookieBitsV - first_byte_bits;
        do {
            size_t next_byte_bits = sbits < CHAR_BIT ? sbits : CHAR_BIT;
            uint8_t next_byte_mask = (((uint8_t)1) << next_byte_bits) - 1;
            ++src;
            res <<= next_byte_bits;
            res |= next_byte_mask & *src;
            sbits -= next_byte_bits;
        } while (sbits > 0);
        return res;
    }

    void set_service_cookie(size_t val) {
        BOOST_ASSERT(val <= max_cookie_val);
        if constexpr (ServiceCookieBitsV <= first_byte_bits) {
            uint8_t * dest = data();
            *dest = (*dest & ~(first_byte_mask << 1)) | (uint8_t)(val << 1);
        } else {
            set_service_cookie_adv(val);
        }
    }

    void set_service_cookie_adv(size_t val) {
        uint8_t * dest = data();
        *dest = (uint8_t)(1 | (val >> (ServiceCookieBitsV - CHAR_BIT))); // 1 is 'not a pointer' flag
        size_t sbits = ServiceCookieBitsV - first_byte_bits;
        do {
            size_t next_byte_bits = sbits < CHAR_BIT ? sbits : CHAR_BIT;
            uint8_t next_byte_mask = (((uint8_t)1) << next_byte_bits) - 1;
            sbits -= next_byte_bits;
            ++dest;
            *dest = (*dest & ~next_byte_mask) | (uint8_t)(next_byte_mask & (val >> sbits));
        } while (sbits > 0);
    }
};

template <size_t HolderBytesV, size_t ServiceCookieBitsV, endian e = endian::native> struct optimized_holder;

template <size_t HolderBytesV, size_t ServiceCookieBitsV>
struct optimized_holder<HolderBytesV, ServiceCookieBitsV, endian::little>
    : optimized_holder_base<HolderBytesV, ServiceCookieBitsV>
{
    typedef optimized_holder_base<HolderBytesV, ServiceCookieBitsV> base_t;
    static const int value_bits_act = HolderBytesV * CHAR_BIT - ServiceCookieBitsV - 1;
    static const int value_bits = value_bits_act <= sizeof(uintmax_t) * CHAR_BIT ? value_bits_act : sizeof(uintmax_t) * CHAR_BIT;
    typedef typename boost::uint_t<value_bits>::fast uint_t;
    static const uint_t uint_max = (((((uint_t)1) << (value_bits - 1)) - 1) << 1) | 1;
    static const uint_t cookie_mask = (((uint_t)1) << (ServiceCookieBitsV + 1)) - 1;

    optimized_holder() {
        base_t::init_not_ptr();
    }

    void reset() {
        if (base_t::is_ptr()) {
            get_pointer()->release();
            base_t::set_not_ptr();
        }
    }

    optimized_holder(optimized_holder const& rhs) {
        if (rhs.is_ptr()) {
            // the cookie is supposed to be the same in the cloned object
            *reinterpret_cast<optimized_base**>(&this->holder_) = rhs.get_pointer()->clone();
        } else {
            std::memcpy(&this->holder_, &rhs.holder_, HolderBytesV);
        }
    }

    optimized_holder(optimized_holder && rhs) {
        if (rhs.is_ptr()) {
            *reinterpret_cast<optimized_base**>(&this->holder_) = rhs.get_pointer();
            rhs.set_not_ptr();
        } else {
            std::memcpy(&this->holder_, &rhs.holder_, HolderBytesV);
        }
    }

    optimized_holder(reference_wrapper<const optimized_holder> rhs) {
        if (rhs.get().is_ptr()) {
            *reinterpret_cast<optimized_base**>(&this->holder_) = rhs.get().get_pointer();
            get_pointer()->add_ref();
        } else {
            std::memcpy(&this->holder_, &rhs.get().holder_, HolderBytesV);
        }
    }

    ~optimized_holder() {
        if (base_t::is_ptr()) {
            get_pointer()->release();
        }
    }

    optimized_holder & operator= (optimized_holder const& rhs) {
        if (this != &rhs) {
            if (base_t::is_ptr()) {
                get_pointer()->release();
            }
            if (rhs.is_ptr()) {
                *reinterpret_cast<optimized_base**>(&this->holder_) = rhs.get_pointer()->clone();
            } else {
                std::memcpy(&this->holder_, &rhs.holder_, HolderBytesV);
            }
        }
        return *this;
    }

    optimized_holder & operator= (optimized_holder && rhs) {
        if (this != &rhs) {
            if (base_t::is_ptr()) {
                get_pointer()->release();
            }
            if (rhs.is_ptr()) {
                *reinterpret_cast<optimized_base**>(&this->holder_) = rhs.get_pointer();
                rhs.set_not_ptr();
            } else {
                std::memcpy(&this->holder_, &rhs.holder_, HolderBytesV);
            }
        }
        return *this;
    }

    optimized_holder & operator= (reference_wrapper<const optimized_holder> rhs) {
        if (this != &rhs.get()) {
            if (base_t::is_ptr()) {
                get_pointer()->release();
            }
            if (rhs.get().is_ptr()) {
                *reinterpret_cast<optimized_base**>(&this->holder_) = rhs.get().get_pointer();
                get_pointer()->add_ref();
            } else {
                std::memcpy(&this->holder_, &rhs.holder_, HolderBytesV);
            }
        }
    }

    uint_t get_uint() const {
        static_assert(sizeof(size_t) * CHAR_BIT > ServiceCookieBitsV + 1);
        uint_t const* src = reinterpret_cast<uint_t const*>(&this->holder_);
        uint_t r = (*src) >> (ServiceCookieBitsV + 1);
        if constexpr (sizeof(uint_t) < HolderBytesV) {
            size_t sbits = sizeof(uint_t) * CHAR_BIT - ServiceCookieBitsV - 1;
            uint8_t const* bsrc = reinterpret_cast<uint8_t const*>(++src);
            uint8_t const* esrc = bsrc + HolderBytesV - sizeof(uint_t);
            do {
                uint_t tmp = *bsrc++;
                tmp <<= sbits;
                sbits += CHAR_BIT;
                r |= tmp;
            } while (bsrc != esrc);
        }
        return r;
    }

    void set_uint(uint_t val) {
        BOOST_ASSERT(val <= uint_max);
        uint_t * dest = reinterpret_cast<uint_t*>(&this->holder_);
        *dest = ((*dest) & cookie_mask) | (val << (ServiceCookieBitsV + 1));
        if constexpr (sizeof(uint_t) < HolderBytesV) {
            size_t dbits = sizeof(uint_t) * CHAR_BIT - ServiceCookieBitsV - 1;
            uint8_t * bdest = reinterpret_cast<uint8_t*>(++dest);
            uint8_t * edest = bdest + HolderBytesV - sizeof(uint_t);
            do {
                *bdest++ = 0xff & (val >> dbits);
                dbits += CHAR_BIT;
            } while (bdest != edest);
        }
    }

    optimized_base * get_pointer() const {
        BOOST_ASSERT(base_t::is_ptr());
        return *reinterpret_cast<optimized_base* const*>(&this->holder_);
    }

    void set_pointer(optimized_base * ptr) {
        BOOST_ASSERT(!base_t::is_ptr());
        BOOST_ASSERT(!(reinterpret_cast<intptr_t>(ptr) & 1));
        //size_t cookie = base_t::get_service_cookie();
        *reinterpret_cast<optimized_base**>(&this->holder_) = ptr;
        //ptr->service_cookie() = cookie;
    }

    size_t get_service_cookie() const noexcept {
        return base_t::is_ptr() ? get_pointer()->service_cookie() : base_t::get_service_cookie();
    }

    void set_service_cookie(size_t val) {
        if (base_t::is_ptr()) {
            get_pointer()->service_cookie() = val;
        } else {
            base_t::set_service_cookie(val);
        }
    }
};

template <size_t HolderBytesV, class ... Ts>
class optimized_union : optimized_holder<HolderBytesV, boost::static_log2<sizeof(Ts)>::value + 1>
{

};

}

#endif // SONIA_UTILITY_OPTIMIZED_HOLDER_HPP
