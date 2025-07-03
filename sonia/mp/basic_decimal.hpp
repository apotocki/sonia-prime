/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <cstdint>
#include <limits>
#include <concepts>
#include <memory>
#include <algorithm>
#include <functional>
#include <span>
#include <tuple>
#include <expected>
#include <bit>
#include <sstream>

#include "sonia/mp/arithmetic.hpp"
#include "sonia/mp/limbs_from_integral.hpp"
#include "sonia/mp/limbs_from_string.hpp"
#include "sonia/mp/limbs_from_decimal_string.hpp"
#include "sonia/mp/integer_view.hpp"

#include "sonia/mp/basic_integer.hpp"
#include "sonia/mp/decimal_view.hpp"

namespace sonia::mp::detail {

// first Limbs forexponent
struct decimal_data
{
    uint32_t allocated_size : 30; // total exp + sig
    uint32_t sign : 1;
    uint32_t allocated_exponent : 1;
    uint32_t size;

    int64_t exponent;
};

template <std::unsigned_integral LimbT, size_t N, intptr_t ExponentBitCount, typename DataT, typename AllocatorT>
requires (N * sizeof(LimbT) >= sizeof(DataT*))
struct decimal_holder : AllocatorT
{
    static constexpr bool overwritten_limb_flag = N * sizeof(LimbT) == sizeof(void*);

    // to store an exponent value that can be stored inplace;
    using exp_inplace_t = std::make_signed_t<LimbT>;

    // Last (ctl) limb layout (from most significant to least significant bits): 
    // 1 bit: inplace flag, 1 bit: value sign (1 if negative), 0-? bits: used limb count, 0-1: exponent sign bit, 0-?: exponent value bits, 1-up to end: value bits;
    static constexpr size_t limb_bits = std::numeric_limits<LimbT>::digits;
    static constexpr LimbT in_place_mask = LimbT(1) << (limb_bits - 1);
    static constexpr LimbT sign_mask = LimbT(1) << (limb_bits - 2);
    static constexpr size_t sig_limb_count_bits = sonia::arithmetic::consteval_log2(N) + ((N & (N - 1)) ? 1u : 0);
    static constexpr LimbT sig_limb_count_mask = ((LimbT(1) << sig_limb_count_bits) - 1) << (limb_bits - 2 - sig_limb_count_bits);

    // space for significand is needed
    static_assert(ExponentBitCount + intptr_t(sig_limb_count_bits) + 2 < intptr_t(limb_bits));
    
    static constexpr bool has_exp_sign_bit = ExponentBitCount > 1;
    
    // part of the exponent (without exponent sign bit): 0 for 0, 1 for 1, x - 1 for x
    static constexpr size_t exp_value_bits = (ExponentBitCount < 0 ? 0 : ExponentBitCount) - (has_exp_sign_bit ? 1 : 0);

    static constexpr LimbT max_inplace_exp_value = (LimbT(1) << exp_value_bits) - 1;
    static constexpr LimbT exp_sign_mask = has_exp_sign_bit ? LimbT(1) << (limb_bits - 3 - sig_limb_count_bits) : 0;
    static constexpr size_t exp_value_bit_offset = limb_bits - 2 - exp_value_bits - (sig_limb_count_bits + (has_exp_sign_bit ? 1 : 0));
    static constexpr LimbT exp_bits_mask = exp_sign_mask | (max_inplace_exp_value << exp_value_bit_offset);

    static constexpr LimbT last_limb_mask_bits = (has_exp_sign_bit ? 1 : 0) + exp_value_bits + sig_limb_count_bits + 2; // takes into account in_place_mask, sign_mask, limb_count_mask
    static constexpr LimbT last_significand_limb_mask = ~(((LimbT(1) << last_limb_mask_bits) - 1) << (limb_bits - last_limb_mask_bits));
    static constexpr size_t data_sizeof_in_limbs = (sizeof(decimal_data) + sizeof(LimbT) - 1) / sizeof(LimbT);

    static constexpr size_t max_inplace_significand_size = N;

    using allocator_type = AllocatorT;
    using alloc_traits_t = std::allocator_traits<allocator_type>;

    alignas(decimal_data*) alignas(LimbT) LimbT inplace_limbs_[N];

    inline allocator_type& allocator() noexcept { return static_cast<allocator_type&>(*this); }

    inline LimbT* allocate(size_t sz)
    {
        return alloc_traits_t::allocate(static_cast<allocator_type&>(*this), sz);
    }

    inline void deallocate(LimbT* ptr, size_t sz) noexcept
    {
        alloc_traits_t::deallocate(static_cast<allocator_type&>(*this), ptr, sz);
    }

    // small size optimized allocator
    struct sso_allocator_type
    {
        using value_type = LimbT;
        
        decimal_holder& holder_;
        bool sso_allocation_ = false;
        inline explicit sso_allocator_type(decimal_holder& h) noexcept : holder_{ h } {}

        LimbT* allocate(size_t cnt)
        {
            if (cnt <= N && !sso_allocation_) {
                return holder_.inplace_limbs_;
                sso_allocation_ = true;
            } else {
                return holder_.allocate(cnt + data_sizeof_in_limbs) + data_sizeof_in_limbs;
            }
        }

        void deallocate(LimbT* ptr, size_t sz)
        {
            LimbT * ibuff = holder_.inplace_limbs_;
            if (!std::equal_to<LimbT*>{}(ptr, ibuff)) {
                holder_.deallocate(ptr - data_sizeof_in_limbs, sz + data_sizeof_in_limbs);
            } else {
                sso_allocation_ = false;
            }
        }
    };

    inline sso_allocator_type sso_allocator() noexcept { return sso_allocator_type{ *this }; }

    inline LimbT const* laundered_data() const noexcept { return std::launder(inplace_limbs_); }
    inline LimbT* laundered_data() noexcept { return std::launder(inplace_limbs_); }

    inline static LimbT& ctl_limb(LimbT* data) noexcept { return data[N - 1]; }
    inline static LimbT ctl_limb(LimbT const* data) noexcept { return data[N - 1]; }

    // ctl limb is the last limb
    // 'no launder' optimization for the case if the last limb is not overwritten by a pointer to limbs_data
    inline LimbT& ctl_limb() noexcept
    {
        if constexpr (overwritten_limb_flag) {
            LimbT& r = ctl_limb(laundered_data());
            return is_inplaced(r) ? ctl_limb(inplace_limbs_) : r;
        } else return ctl_limb(inplace_limbs_);
    }
    inline LimbT ctl_limb() const noexcept
    {
        if constexpr (overwritten_limb_flag) return ctl_limb(laundered_data());
        else return ctl_limb(inplace_limbs_);
    }

    inline static bool is_inplaced(LimbT ctl) noexcept { return !!(ctl & in_place_mask); }
    inline bool is_inplaced() const noexcept { return is_inplaced(ctl_limb()); }

    inline static bool inplaced_is_negative(LimbT ctl) noexcept { return !!(ctl & sign_mask); }
    inline bool inplaced_is_negative() const noexcept { return inplaced_is_negative(ctl_limb()); }

    inline static size_t inplaced_size(LimbT ctl) noexcept
    {
        if constexpr (sig_limb_count_bits) {
            return 1 + ((ctl & sig_limb_count_mask) >> (limb_bits - sig_limb_count_bits - 2)); // 2 bits are sso and sign bits
        } else {
            return 1;
        }
    }
    inline size_t inplaced_size() const noexcept { return inplaced_size(ctl_limb()); }

    inline static exp_inplace_t inplace_exponent(LimbT ctl) noexcept
    {
        exp_inplace_t result = (ctl >> exp_value_bit_offset) & max_inplace_exp_value;
        if (ctl & exp_sign_mask) result = -result;
        return result;
    }

    inline std::span<const LimbT> inplaced_copy_significand_limbs(std::span<LimbT> buff) const
    {
        size_t cnt = (std::min)(buff.size(), inplaced_size());
        std::copy(inplace_limbs_, inplace_limbs_ + cnt, buff.data());
        if (max_inplace_significand_size == cnt) {
            buff[max_inplace_significand_size - 1] &= last_significand_limb_mask;
        }
        return { buff.data(), cnt };
    }

    inline void inplaced_set_masks(size_t sz, int sign, int64_t exponent)
    {
        // limbs[N - 1] is 0 or the last limb
        LimbT mask = in_place_mask | (sign < 0 ? sign_mask : 0);
        if constexpr (sig_limb_count_bits) {
            mask |= static_cast<LimbT>(sz - 1) << (limb_bits - 2 - sig_limb_count_bits);
        }
        if constexpr (has_exp_sign_bit) {
            if (exponent < 0) mask |= exp_sign_mask;
        }
        if constexpr (exp_value_bits) {
            uint64_t absexp = static_cast<uint64_t>(exponent);
            if (exponent < 0) {
                absexp = ~absexp + 1;
            }
            assert(absexp <= max_inplace_exp_value);
            mask |= static_cast<LimbT>(absexp) << exp_value_bit_offset;
        }
        LimbT& ctl = ctl_limb(inplace_limbs_);
        ctl = (ctl & last_significand_limb_mask) | mask;
    }

    void set_allocated(DataT* data)
    {
        static_assert(std::endian::native == std::endian::little, "unsupported endianness");

        if constexpr (!overwritten_limb_flag) {
            *reinterpret_cast<DataT**>(inplace_limbs_) = data;
            inplace_limbs_[N - 1] &= ~in_place_mask; // reset inplace bit
        } else {
            // litle endian case
            *reinterpret_cast<uintptr_t*>(inplace_limbs_) = std::rotr(reinterpret_cast<uintptr_t>(data), 1);
        }
    }

    inline DataT * allocated_data() const noexcept
    {
        if constexpr (!overwritten_limb_flag) {
            return *std::launder(reinterpret_cast<DataT* const*>(inplace_limbs_));
        } else {
            return reinterpret_cast<DataT*>(std::rotl(*std::launder(reinterpret_cast<uintptr_t const*>(inplace_limbs_)), 1));
        }
    }

    inline std::tuple<DataT const*, LimbT const*> allocated_data_and_limbs() const noexcept
    {
        if constexpr (!overwritten_limb_flag) {
            LimbT const* limbs = *std::launder(reinterpret_cast<LimbT const* const*>(inplace_limbs_)) + data_sizeof_in_limbs;
            DataT const* ldata = *std::launder(reinterpret_cast<DataT const* const*>(inplace_limbs_));
            return std::tuple{ ldata, limbs };
        } else {
            DataT const* ldata = reinterpret_cast<DataT const*>(std::rotl(*reinterpret_cast<uintptr_t const*>(inplace_limbs_), 1));
            LimbT const* limbs = std::launder((reinterpret_cast<LimbT const*>(ldata) + data_sizeof_in_limbs));
            return std::tuple{ ldata, limbs };
        }
    }

    inline std::span<const LimbT> allocated_limbs() const
    {
        auto [ldata, limbs] = allocated_data_and_limbs();
        return { limbs, ldata->size };
    }

    inline bool allocated_is_negative() const
    {
        return !!allocated_data()->sign;
    }

    inline bool is_negative() const noexcept
    {
        LimbT ctl = ctl_limb();
        return is_inplaced(ctl) ? inplaced_is_negative(ctl) : allocated_is_negative();
    }

    inline void negate()
    {
        LimbT& ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            ctl ^= sign_mask;
        } else {
            allocated_data()->sign ^= 1;
        }
    }

    explicit decimal_holder()
    {
        init_zero();
    }

    template <typename AllocT>
    explicit decimal_holder(AllocT&& alloc) : allocator_type{ std::forward<AllocT>(alloc) }
    {
        init_zero();
    }

    template <typename BuilderT, typename AllocT>
    requires(requires{ std::declval<BuilderT const&>()(std::declval<decimal_holder&>()); })
    inline decimal_holder(BuilderT const& iftor, AllocT&& alloc) : allocator_type{ std::forward<AllocT>(alloc) }
    {
        iftor(*this);
    }

    decimal_holder(decimal_holder const& rhs)
        : allocator_type{ static_cast<allocator_type const&>(rhs) }
    {
        LimbT rlimb = rhs.ctl_limb();
        if (decimal_holder::is_inplaced(rlimb)) {
            if constexpr (N > 1) {
                std::copy(rhs.inplace_limbs_, rhs.inplace_limbs_ + decimal_holder::inplaced_size(rlimb), inplace_limbs_);
            }
            decimal_holder::ctl_limb(inplace_limbs_) = rlimb;
        } else {
            auto [rhs_ldata, rhs_limbs] = rhs.allocated_data_and_limbs();
            
            auto can_inplace = [](DataT const* ldata, LimbT const* limbs) noexcept {
                if (ldata->size > N) return false;
                if (ldata->size == N && (limbs[ldata->size - 1] & ~last_significand_limb_mask)) return false;
                if constexpr (ExponentBitCount >= 0) {
                    if (!ldata->allocated_exponent) {
                        if (!has_exp_sign_bit && ldata->exponent < 0) return false;
                        if (ldata->exponent > (int64_t)max_inplace_exp_value || ldata->exponent < -(int64_t)max_inplace_exp_value) return false;
                    } else {
                        basic_integer_view<LimbT> exp {
                            std::span{ limbs + ldata->size, static_cast<size_t>(std::abs(ldata->exponent)) },
                            ldata->exponent > 0 ? 1 : -1
                        };
                        if (!has_exp_sign_bit && ldata->exponent < 0) return false;
                        if (exp.abs() > (int64_t)max_inplace_exp_value) return false;
                    }
                }
                return true;
            };
            if (can_inplace(rhs_ldata, rhs_limbs)) {
                std::copy(rhs_limbs, rhs_limbs + rhs_ldata->size, inplace_limbs_);
                inplaced_set_masks(rhs_ldata->size, rhs_ldata->sign ? -1 : 1, rhs.integral_exponent<int64_t>());
            } else {
                size_t exp_sz = rhs_ldata->allocated_exponent ? (size_t)std::abs(rhs_ldata->exponent) : 0;
                size_t allocsz = rhs_ldata->size + exp_sz;
                LimbT* limbsdata = allocate(allocsz + data_sizeof_in_limbs);
                std::copy(rhs_limbs, rhs_limbs + rhs_ldata->size + exp_sz, limbsdata + data_sizeof_in_limbs);
                DataT* ldata = new (limbsdata) DataT{ *rhs_ldata };
                ldata->allocated_size = (uint32_t)(allocsz);
                set_allocated(ldata);
            }
        }
    }

    ~decimal_holder()
    {
        free();
    }

    // initilize by inplace value = 0
    inline void init_zero() noexcept
    {
        inplace_limbs_[N - 1] = in_place_mask;
        if constexpr (N > 1) {
            inplace_limbs_[0] = 0;
        }
    }

    void set_exponent(basic_integer_view<LimbT> exponent)
    {
        LimbT& ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            auto can_be_inplaced = [](basic_integer_view<LimbT> exponent) noexcept {
                if constexpr (exp_value_bits > 0) {
                    // can fit exponent in available bits?
                    if constexpr (!has_exp_sign_bit) {
                        if (exponent.is_negative()) return false;
                    }
                    if (exponent.abs() > max_inplace_exp_value) return false;
                } else {
                    if (exponent) return false;
                }
                return true;
            };

            if (can_be_inplaced(exponent)) {
                LimbT mask = static_cast<LimbT>(exponent.abs()) << exp_value_bit_offset;
                if constexpr (has_exp_sign_bit) {
                    if (exponent.is_negative()) mask |= exp_sign_mask;
                }
                ctl = (ctl & ~exp_bits_mask) | mask;
            } else {
                uint32_t sz = static_cast<uint32_t>(inplaced_size(ctl));
                uint32_t esz = static_cast<uint32_t>(exponent.template is_fit<int64_t>() ? 0 : exponent.size());
                LimbT* newlimbdata = allocate(sz + esz + data_sizeof_in_limbs);
                LimbT* lastlimb = std::copy(inplace_limbs_, inplace_limbs_ + sz, newlimbdata + data_sizeof_in_limbs);
                if (inplaced_size(ctl) == N) *(lastlimb - 1) &= sig_limb_count_mask;
                if (!esz) {
                    new (newlimbdata) DataT{
                        .allocated_size = sz,
                        .sign = inplaced_is_negative() ? 1u : 0,
                        .allocated_exponent = 0,
                        .size = sz,
                        .exponent = (int64_t)exponent};
                } else {
                    new (newlimbdata) DataT{
                        .allocated_size = sz + esz,
                        .sign = inplaced_is_negative() ? 1u : 0,
                        .allocated_exponent = 1,
                        .size = sz,
                        .exponent = exponent.is_negative() ? -(int64_t)exponent.size() : (int64_t)exponent.size()};
                    exponent.copy_to(newlimbdata + data_sizeof_in_limbs + sz);
                }
                set_allocated(reinterpret_cast<DataT*>(newlimbdata));
            }
        } else {
            DataT* ldata = allocated_data();
            if (exponent.template is_fit<int64_t>()) {
                ldata->exponent = (int64_t)exponent;
                ldata->allocated_exponent = 0;
            } else if (exponent.size() + ldata->size <= ldata->allocated_size) {
                exponent.copy_to(reinterpret_cast<LimbT*>(ldata) + data_sizeof_in_limbs + ldata->size);
                ldata->exponent = exponent.is_negative() ? -(int64_t)exponent.size() : (int64_t)exponent.size();
                ldata->allocated_exponent = 1;
            } else { // reallocate
                size_t asize = exponent.size() + ldata->size;
                LimbT* newlimbdata = allocate(asize + data_sizeof_in_limbs);
                DataT* newldata = new (newlimbdata) DataT{
                    .allocated_size = static_cast<uint32_t>(asize),
                    .sign = ldata->sign,
                    .allocated_exponent = 1,
                    .size = ldata->size,
                    .exponent = exponent.is_negative() ? -(int64_t)exponent.size() : (int64_t)exponent.size()};
                LimbT const* lsrc = std::launder(reinterpret_cast<LimbT*>(ldata) + data_sizeof_in_limbs);
                std::copy(lsrc, lsrc + ldata->size, newlimbdata + data_sizeof_in_limbs);
                exponent.copy_to(newlimbdata + data_sizeof_in_limbs + ldata->size);
                deallocate(reinterpret_cast<LimbT*>(ldata), ldata->allocated_size + data_sizeof_in_limbs);
                set_allocated(newldata);
            }
        }
    }

    static bool can_be_inplaced(LimbT const* limbs, size_t sz, LimbT last_limb_mask, basic_integer_view<LimbT> exponent) noexcept
    {
        if (sz > N) return false;
        if (sz == N && (limbs[N - 1] & last_limb_mask)) return false;
        if constexpr (exp_value_bits > 0) {
            // can fit exponent in available bits?
            if constexpr (!has_exp_sign_bit) {
                if (exponent.is_negative()) return false;
            }
            if (exponent.abs() > max_inplace_exp_value) return false;
        } else {
            if (exponent) return false;
        }
        return true;
    };

    inline void init(basic_integer_view<LimbT> s, basic_integer_view<LimbT> exponent)
    {
        if (!s) {
            init_zero();
            return;
        }
        LimbT const* limbs = s.data();
        size_t sz = s.size();
        LimbT last_limb_mask = s.last_limb_mask();
        if (can_be_inplaced(limbs, sz, last_limb_mask & ~last_significand_limb_mask, exponent)) {
            memcpy(inplace_limbs_, limbs, sz * sizeof(LimbT));
            inplace_limbs_[sz - 1] &= s.last_limb_mask();
            return inplaced_set_masks(sz, s.sgn(), (int64_t)exponent);
        } else { // need allocate
            size_t exp_alloc_sz = exponent.template is_fit<int64_t>() ? 0 : exponent.size();
            size_t asz = sz + exp_alloc_sz;
            LimbT* limbsdata = allocate(asz + data_sizeof_in_limbs);
            std::copy(limbs, limbs + sz, limbsdata + data_sizeof_in_limbs);
            if (exp_alloc_sz) {
                exponent.copy_to(limbsdata + data_sizeof_in_limbs + sz);
            }

            new (limbsdata) DataT{ .allocated_size = static_cast<uint32_t>(asz), .sign = (s.sgn() < 0) ? 1u : 0, .size = static_cast<uint32_t>(sz) };
            if constexpr (ExponentBitCount >= 0) {
                if (exp_alloc_sz) {
                    reinterpret_cast<DataT*>(limbsdata)->allocated_exponent = 1;
                    reinterpret_cast<DataT*>(limbsdata)->exponent = (exponent.is_negative() ? -1 : 1) * static_cast<int64_t>(exp_alloc_sz);
                } else {
                    reinterpret_cast<DataT*>(limbsdata)->allocated_exponent = 0;
                    reinterpret_cast<DataT*>(limbsdata)->exponent = (int64_t)exponent;
                }
            }
            set_allocated(reinterpret_cast<DataT*>(limbsdata));
        }
    }

    // (limbs, size, allocated size, sign)
    template <typename InitAllocatorT>
    inline void init(std::tuple<LimbT*, size_t, size_t, int> tpl, InitAllocatorT& alloc, basic_integer_view<LimbT> exponent)
    {
        using alloc_traits_t = std::allocator_traits<InitAllocatorT>;

        LimbT* limbsdata;
        size_t exp_alloc_sz;
        auto [limbs, sz, asz, sign] = tpl;
        if (!limbs) {
            init_zero();
            return;
        }

        if (std::equal_to<const LimbT*>()(limbs, inplace_limbs_)) {
            //auto can_be_inplaced = [](LimbT const* limbs, size_t sz, basic_integer_view<LimbT> exponent) noexcept {
            //    if (sz > N) return false;
            //    if (sz == N && (limbs[N - 1] & ~last_significand_limb_mask)) return false;
            //    if constexpr (exp_value_bits > 0) {
            //        // can fit exponent in available bits?
            //        if constexpr (!has_exp_sign_bit) {
            //            if (exponent.is_negative()) return false;
            //        }
            //        if (exponent.abs() > max_inplace_exp_value) return false;
            //    } else {
            //        if (exponent) return false;
            //    }
            //    return true;
            //};
            if (can_be_inplaced(limbs, sz, ~last_significand_limb_mask, exponent)) { // sso case
                return inplaced_set_masks(sz, sign, (int64_t)exponent);
            } else { // need allocate
                exp_alloc_sz = exponent.template is_fit<int64_t>() ? 0 : exponent.size();
                asz = sz + exp_alloc_sz;
                limbsdata = allocate(asz + data_sizeof_in_limbs);
                std::copy(limbs, limbs + sz, limbsdata + data_sizeof_in_limbs);
                if (exp_alloc_sz) {
                    exponent.copy_to(limbsdata + data_sizeof_in_limbs + sz);
                }
            }
        } else if (!exponent.template is_fit<int64_t>()) { // libsdata is allocated, but needs to be reallocated
            exp_alloc_sz = exponent.size();
            size_t new_asz = sz + exp_alloc_sz;
            try {
                limbsdata = allocate(new_asz + data_sizeof_in_limbs);
            } catch (...) {
                alloc_traits_t::deallocate(alloc, limbs, asz);
                throw;
            }
            std::copy(limbs, limbs + sz, limbsdata + data_sizeof_in_limbs);
            exponent.copy_to(limbsdata + data_sizeof_in_limbs + sz);
            alloc_traits_t::deallocate(alloc, limbs, asz);
            asz = new_asz;
        } else { // libsdata is already allocated, but not initialized
            limbsdata = limbs - data_sizeof_in_limbs;
            exp_alloc_sz = 0;
        }
        new (limbsdata) DataT{ .allocated_size = static_cast<uint32_t>(asz), .sign = (sign < 0) ? 1u : 0, .size = static_cast<uint32_t>(sz) };
        if constexpr (ExponentBitCount >= 0) {
            if (exp_alloc_sz) {
                reinterpret_cast<DataT*>(limbsdata)->allocated_exponent = 1;
                reinterpret_cast<DataT*>(limbsdata)->exponent = (exponent.is_negative() ? -1 : 1) * static_cast<int64_t>(exp_alloc_sz);
            } else {
                reinterpret_cast<DataT*>(limbsdata)->allocated_exponent = 0;
                reinterpret_cast<DataT*>(limbsdata)->exponent = (int64_t)exponent;
            }
        }
        set_allocated(reinterpret_cast<DataT*>(limbsdata));
        //DataT* p = allocated_data();
        //assert(p == reinterpret_cast<DataT*>(limbsdata));
    }

    bool is_zero() const noexcept
    {
        LimbT ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            return inplaced_size(ctl) == 1 && ((N == 1 && !(inplace_limbs_[0] & last_significand_limb_mask)) || (N > 1 && !inplace_limbs_[0]));
        } else {
            auto limbs = allocated_limbs();
            return std::find_if(limbs.rbegin(), limbs.rend(), [](LimbT l) { return !!l; }) == limbs.rend();
        }
    }

    [[nodiscard]] inline basic_integer_view<LimbT> significand() const noexcept
    {
        LimbT ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            int sign = inplaced_is_negative(ctl) ? -1 : 1;
            size_t sz = inplaced_size(ctl);
            if (sz < N) { // no overlapping with ctl bits
                return basic_integer_view<LimbT>{std::span{inplace_limbs_, sz}, sign};
            }
            return basic_integer_view<LimbT>{std::span{ inplace_limbs_, sz }, sign, last_limb_mask_bits};
        } else {
            auto [ldata, limbs] = allocated_data_and_limbs();
            return basic_integer_view<LimbT>{std::span{ limbs, ldata->size }, ldata->sign ? -1 : 1 };
        }
    }

    [[nodiscard]] inline basic_integer_view<LimbT> exponent() const noexcept
    {
        LimbT ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            return inplace_exponent(ctl);
        } else {
            auto [ldata, limbs] = allocated_data_and_limbs();
            if (!ldata->allocated_exponent) [[likely]] {
                return ldata->exponent;
            } else {
                return basic_integer_view<LimbT> {
                    std::span{ limbs + ldata->size, static_cast<size_t>(std::abs(ldata->exponent)) },
                    ldata->exponent > 0 ? 1 : -1
                };
            }
        }
    }

    template <std::signed_integral T>
    requires(ExponentBitCount >= 0)
    [[nodiscard]] inline T integral_exponent() const
    {
        LimbT ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            return static_cast<T>(inplace_exponent(ctl));
        } else {
            auto [ldata, limbs] = allocated_data_and_limbs();
            if (!ldata->allocated_exponent) [[likely]] {
                return static_cast<T>(ldata->exponent);
            } else {
                return (T)basic_integer_view<LimbT>{
                    std::span{ limbs + ldata->size, static_cast<size_t>(std::abs(ldata->exponent)) },
                    ldata->exponent > 0 ? 1 : -1
                };
            }
        }
    }

    template <std::integral T>
    [[nodiscard]] bool is_fit_significand() const noexcept
    {
        return significand().template is_fit<T>();
    }

    inline void free() noexcept
    {
        LimbT ctl = ctl_limb();
        if (!is_inplaced(ctl)) {
            DataT* ldata = allocated_data();
            deallocate(reinterpret_cast<LimbT*>(ldata), ldata->allocated_size + data_sizeof_in_limbs);
        }
    }
};

template <std::unsigned_integral LimbT, size_t N, intptr_t EBC, typename DataT, typename AllocatorT>
std::exception_ptr from_integer_string(decimal_holder<LimbT, N, EBC, DataT, AllocatorT>& dh, std::string_view & str, int base = 0) noexcept
{
    using storage_type = decimal_holder<LimbT, N, EBC, DataT, AllocatorT>;
    std::string_view orig_str = str;

    auto opt_tpl = to_limbs<LimbT>(str, base, dh.sso_allocator());
    if (!opt_tpl.has_value()) {
        std::string error;
        try { std::rethrow_exception(opt_tpl.error()); }
        catch (std::exception const& e) { error = e.what(); }
        return std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "string '"sv << orig_str << "' cannot be parsed as an integer, "sv << error).str()));
    }

    try {
        dh.init(*opt_tpl, 0);
    } catch (std::bad_alloc const& e) {
        return std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "can't allocate an integer storage for '" << orig_str << "', error: " << e.what()).str()));
    } catch (...) {
        return std::current_exception();
    }

    return nullptr;
}

template <std::unsigned_integral LimbT, size_t N, intptr_t EBC, typename DataT, typename AllocatorT>
std::exception_ptr from_decimal_string(decimal_holder<LimbT, N, EBC, DataT, AllocatorT>& dh, std::string_view & str) noexcept
{
    using storage_type = decimal_holder<LimbT, N, EBC, DataT, AllocatorT>;
    std::string_view orig_str = str;
    int64_t exp;
    auto alloc = dh.sso_allocator();
    auto opt_sig_tpl = to_significand_limbs<LimbT>(str, alloc, exp);
    
    if (!opt_sig_tpl.has_value()) {
        try { std::rethrow_exception(opt_sig_tpl.error()); } catch (std::exception const& e) {
            return std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "string '"sv << orig_str << "' cannot be parsed as a decimal, "sv << e.what()).str()));
        } catch (...) { return std::current_exception(); }
    }
    std::string_view sig_str = str;

    try {
        dh.init(*opt_sig_tpl, alloc, exp);
        if (!str.empty() && (str.front() == 'e' || str.front() == 'E')) {
            str = str.substr(1);
            auto opt_e = basic_integer<LimbT>::from_string(str, 10, dh.allocator()); // noexcept
            if (!opt_e.has_value()) [[unlikely]] { // can't parse exponent, just roll back
                str = sig_str;
            } else {
                (*opt_e) += exp; // can throw
                dh.set_exponent(*opt_e); // can throw
            }
        }

    } catch (std::bad_alloc const& e) {
        return std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "can't allocate a decimal storage for '" << str << "', error: " << e.what()).str()));
    } catch (...) {
        return std::current_exception();
    }

    return nullptr;
}

}

namespace sonia::mp {

// ExponentBitCount includes the sign bit
// inplaced exponent must fit in LimbT
template <std::unsigned_integral LimbT, size_t N, size_t ExponentBitCount, typename AllocatorT = std::allocator<LimbT>>
// requires(std::numeric_limits<LimbT2>::digits > ExponentBitCount2) // clang issue
class basic_decimal
{
    static_assert(std::numeric_limits<LimbT>::digits > ExponentBitCount);

    static constexpr size_t expected_sizeof = (N * sizeof(LimbT) + sizeof(void*) - 1) & ~size_t(sizeof(void*) - 1); // a multiple of the pointer size
    static constexpr size_t actualN = expected_sizeof / sizeof(LimbT);

    using allocator_type = std::allocator_traits<AllocatorT>::template rebind_alloc<LimbT>;
    using alloc_traits_t = std::allocator_traits<allocator_type>;

    //template <std::unsigned_integral LimbT2, size_t N2, size_t ExponentBitCount2, typename AllocatorT2>
    //requires(std::numeric_limits<LimbT2>::digits > ExponentBitCount2)
    template <std::unsigned_integral LimbT2, size_t, size_t, typename> friend class basic_decimal;

    using alloc_holder = detail::decimal_holder<LimbT, actualN, ExponentBitCount, detail::decimal_data, allocator_type>;
    alloc_holder aholder_;

public:
    inline allocator_type& allocator() noexcept { return aholder_; }
    inline allocator_type const& allocator() const noexcept { return aholder_; }

    basic_decimal() = default;
    ~basic_decimal() = default;

    using storage_type = alloc_holder; // for debug

    explicit basic_decimal(AllocatorT const& alloc) noexcept : aholder_{ alloc } {}

    explicit basic_decimal(basic_integer_view<LimbT> s, basic_integer_view<LimbT> exp = {}, AllocatorT const& alloc = AllocatorT{})
        : aholder_{ [&s, &exp](storage_type& dh) { dh.init(s, exp); }, alloc }
    {
        // to do: normilize
    }

    explicit basic_decimal(basic_decimal_view<LimbT> dv, AllocatorT const& alloc = AllocatorT{})
        : aholder_{ [&dv](storage_type& dh) { dh.init(dv.significand(), dv.exponent()); }, alloc }
    {}

    template <std::integral T>
    basic_decimal(T value, AllocatorT const& alloc = AllocatorT{})
        : aholder_{ [&value](storage_type& dh) {
            if (!value) {
                dh.init_zero();
                return;
            }
            typename storage_type::exp_inplace_t exp = 0;
            while (0 == value % 10) {
                value /= 10;
                ++exp;
            }
            storage_type::ctl_limb(dh.inplace_limbs_) = storage_type::in_place_mask;
            auto alloc = dh.sso_allocator();
            dh.init(to_limbs<LimbT>(value, alloc), alloc, exp);
        }, alloc }
    {}

    explicit basic_decimal(std::string_view str, AllocatorT const& alloc = AllocatorT{})
        : aholder_{ alloc }
    {
        std::string_view orig_str = str;
        if (auto eptr = from_decimal_string(aholder_, str); eptr) {
            std::rethrow_exception(eptr);
        } else if (!str.empty()) {
            throw std::invalid_argument((std::ostringstream() << "can't parse string '"sv << orig_str << "' as an decimal, stopped at: '"sv << str << '\'').str());
        }
    }
        
    static std::expected<basic_decimal, std::exception_ptr> from_string(std::string_view str, AllocatorT const& alloc = AllocatorT{}) noexcept
    {
        basic_decimal result(alloc);
        if (auto eptr = from_decimal_string(result.aholder_, str); eptr) {
            return std::unexpected(eptr);
        }
        return std::move(result);
    }
    
    template <std::floating_point T>
    inline explicit operator T() const
    {
        return (T)((T)aholder_.significand() * std::pow(10.0, exponent_as<int64_t>()));
    }

    // returns only for not fractional values
    template <std::integral T>
    inline explicit operator T() const
    {
        int64_t eval = exponent_as<int64_t>();
        if (eval >= 0) {
            if (std::numeric_limits<T>::digits10 >= eval && aholder_.template is_fit_significand<T>()) {
                using UT = std::make_unsigned_t<T>;

                UT value = (UT)aholder_.significand().abs();
                UT emultiplier = sonia::arithmetic::ipow<UT>(10, eval);
            
                auto [h, l] = sonia::arithmetic::umul1(value, emultiplier);
                if (!h) {
                    if constexpr (std::is_same_v<T, UT>) {
                        return l;
                    } else if (!(l >> (std::numeric_limits<UT>::digits - 1))) { // check 0 for most significant bit
                        if (aholder_.is_negative()) {
                            l = ~l + 1;
                        }
                        return static_cast<T>(l);
                    }
                }
            }
        } else {
            eval = -eval;
            basic_integer<LimbT, actualN, AllocatorT> sig{ aholder_.significand() };
            // This is a temporary solution until division with an arbitrary size divider is implemented.
            
            constexpr uint64_t step = std::numeric_limits<LimbT>::digits10;
            constexpr uint64_t stepdivider = sonia::arithmetic::ipow<LimbT>(10, step);
            
            while (step < eval) {
                sig /= stepdivider;
                if (!sig) return 0;
                eval -= step;
            }
            uint64_t lastdivider = sonia::arithmetic::ipow<LimbT>(10, eval);
            sig /= lastdivider;
            return (T)sig;
        }
        
        throw std::invalid_argument((std::ostringstream() << "the destination type can't fit value: " << *this).str());
    }

    inline void negate() noexcept
    {
        aholder_.negate();
    }

    inline bool is_negative() const noexcept { return aholder_.is_negative(); }

    inline bool is_inplaced() const noexcept { return aholder_.is_inplaced(); }

    explicit operator bool() const
    {
        return !aholder_.is_zero();
    }

    inline basic_integer_view<LimbT> significand() const noexcept
    {
        return aholder_.significand();
    }

    inline basic_integer_view<LimbT> exponent() const noexcept
    {
        return aholder_.exponent();
    }

    template <std::signed_integral T>
    inline T exponent_as() const noexcept
    {
        return aholder_.template integral_exponent<T>();
    }

    inline operator basic_decimal_view<LimbT>() const
    {
        return basic_decimal_view<LimbT>{ significand(), aholder_.exponent() };
    }

    friend inline std::string to_string(basic_decimal const& val)
    {
        std::string result;

        LimbT buff[actualN];
        bool reversed;
        std::span<const LimbT> limbs = val.is_inplaced() ?
            val.aholder_.inplaced_copy_significand_limbs(std::span{ buff }) : val.aholder_.allocated_limbs();

        if (val.is_negative()) result.push_back('-');

        size_t offset = result.size();
        to_string(limbs, std::back_inserter(result), reversed, 10);

        if (reversed) {
            std::reverse(result.begin() + offset, result.end());
        }
        int64_t e = val.exponent_as<int64_t>();
        if (e >= 0) {
            result.resize(result.size() + e, '0');
        } else {
            int pos = val.is_negative() ? 1 : 0;
            int64_t zpadcount = -e - (int64_t)result.size() + pos + 1;
            if (zpadcount > 0) {
                result.insert(result.begin() + pos, zpadcount, '0');
            }
            result.insert(result.begin() + result.size() + e, '.');
        }
        return result;
    }

    basic_decimal operator- () const
    {
        basic_decimal result{ *this };
        result.negate();
        return result;
    }
};

template <std::unsigned_integral LimbT, size_t NL, size_t NR, size_t ExponentBitCountL, size_t ExponentBitCountR, typename AllocatorTL, typename AllocatorTR>
inline bool operator== (basic_decimal<LimbT, NL, ExponentBitCountL, AllocatorTL> const& lhs, basic_decimal<LimbT, NR, ExponentBitCountR, AllocatorTR> const& rhs) noexcept
{
    return lhs.exponent() == rhs.exponent() && lhs.significand() == rhs.significand();
}

template <std::unsigned_integral LimbT, size_t N, size_t ExponentBitCount, typename AllocatorT, std::integral T>
bool operator ==(basic_decimal<LimbT, N, ExponentBitCount, AllocatorT> const& lhs, T rhs) noexcept
{
    if (!rhs) return !lhs;
    if (!lhs || lhs.exponent().is_negative()) return false;
    size_t exp = 0;
    for (;;) {
        auto [q, r] = sonia::arithmetic::div1(rhs, 10);
        if (r) break;
        rhs = q;
        ++exp;
    }
    return lhs.significand() == rhs && lhs.exponent() == exp;
}

template <std::unsigned_integral LimbT, size_t NL, size_t NR, size_t ExponentBitCountL, size_t ExponentBitCountR, typename AllocatorTL, typename AllocatorTR>
inline std::strong_ordering operator<=> (basic_decimal<LimbT, NL, ExponentBitCountL, AllocatorTL> const& lhs, basic_decimal<LimbT, NR, ExponentBitCountR, AllocatorTR> const& rhs)
{
    return (basic_decimal_view<LimbT>)lhs <=> (basic_decimal_view<LimbT>)rhs;
}

template <std::unsigned_integral LimbT, size_t LN, size_t ExponentBitCountL, typename AllocatorTL, std::integral RT>
inline std::strong_ordering operator <=>(basic_decimal<LimbT, LN, ExponentBitCountL, AllocatorTL> const& lhs, RT rhs)
{
    // 1 + sizeof(RT) / sizeof(LimbT) ensures no dynamic allocation for RT representation in basic_decimal
    return lhs <=> basic_decimal<LimbT, 1 + sizeof(RT) / sizeof(LimbT), ExponentBitCountL, AllocatorTL>{ rhs };
}

template <std::unsigned_integral LimbT, size_t N, size_t E, typename AllocatorT>
inline size_t hash_value(basic_decimal<LimbT, N, E, AllocatorT> const& v) noexcept
{
    return hasher()(v.significand(), v.exponent());
}

template <std::unsigned_integral LimbT, size_t N, size_t E, typename AllocatorT>
inline basic_decimal<LimbT, N, E, AllocatorT> operator+ (basic_decimal<LimbT, N, E, AllocatorT> const& l, basic_decimal_view<LimbT> rv)
{
    basic_integer<LimbT, N, AllocatorT> ediff{ rv.exponent(), l.allocator() };
    ediff -= l.exponent();
    if (!ediff.template is_fit<int>()) throw std::overflow_error("the exponent difference is too large");
    int ediffval = (int)ediff;

    basic_integer<LimbT, N, AllocatorT> rs, re; // result significand and exponent
    if (ediffval > 0) {
        rs = pow(basic_integer<LimbT, N, AllocatorT>{ 10, l.allocator() }, (unsigned int)ediffval) * rv.significand() + l.significand();
        re = l.exponent();
    } else if (ediffval < 0) {
        rs = pow(basic_integer<LimbT, N, AllocatorT>{ 10, l.allocator() }, (unsigned int)(-ediffval))* l.significand() + rv.significand();
        re = rv.exponent();
    } else {
        rs = l.significand(); rs += rv.significand();
        re = l.exponent();
    }
    while (!(rs % 10)) {
        rs /= 10;
        re += 1;
    }
    return basic_decimal<LimbT, N, E, AllocatorT>{ rs, re, l.allocator() };
}

template <typename Elem, typename Traits, std::unsigned_integral LimbT, size_t N, size_t E, typename AllocatorT>
inline std::basic_ostream<Elem, Traits>& operator <<(std::basic_ostream<Elem, Traits>& os, basic_decimal<LimbT, N, E, AllocatorT> const& dv)
{
    return os << to_string(dv);
}

template <std::unsigned_integral LimbT, size_t N, size_t E, typename AllocatorT>
std::string to_scientific_string(basic_decimal<LimbT, N, E, AllocatorT> const& val)
{
    using namespace std::string_literals;
    if (!val) return "0.0E0"s;
    std::string result = (std::ostringstream() << val.significand()).str();
    basic_integer<LimbT, N, AllocatorT> sc_e{ val.exponent() };
    for (; result.back() == '0'; result.pop_back(), sc_e += 1);

    int pos = val.is_negative() ? 1 : 0;

    sc_e += (result.size() - 1);
    sc_e -= pos;

    result.insert(result.begin() + 1 + pos, '.');
    if (result.back() == '.') result.pop_back();
    result.push_back('E');
    to_vector((basic_integer_view<LimbT>)sc_e, 10, false, result);
    return result;
}

using decimal = basic_decimal<uint64_t, 1, 8>;

}
