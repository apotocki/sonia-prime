/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <limits>
#include <concepts>
#include <new>
#include <tuple>
#include <bit>
#include <memory>
#include <functional>
#include <algorithm>
#include <string_view>
#include <span>
#include <compare>
#include <iosfwd>
#include <sstream>

#include "sonia/utility/scope_exit.hpp"

#include "integer_view.hpp"
#include "integer_view_arithmetic.hpp"

namespace sonia::mp::detail {

struct limbs_data
{
    uint32_t allocated_size : 31;
    uint32_t sign : 1;
    uint32_t size;
};

template <std::unsigned_integral LimbT, size_t N, typename AllocatorT>
requires (N * sizeof(LimbT) >= sizeof(limbs_data*))
struct integer_holder : AllocatorT
{
    // important note:
    // any function started with 'inplaced_' must be called only if is_inplaced = true
    // any function started with 'allocated_' must be called only if is_inplaced = false
    
    static constexpr bool overwritten_limb_flag = N * sizeof(LimbT) == sizeof(void*);

    static constexpr size_t limb_bits = std::numeric_limits<LimbT>::digits;
    static constexpr LimbT no_mask = (std::numeric_limits<LimbT>::max)();
    static constexpr LimbT in_place_mask = LimbT(1) << (limb_bits - 1);
    static constexpr LimbT sign_mask = LimbT(1) << (limb_bits - 2);
    static constexpr size_t limb_count_bits = sonia::arithmetic::consteval_log2(N) + ((N & (N - 1)) ? 1u : 0);
    static constexpr LimbT limb_count_mask = ((LimbT(1) << limb_count_bits) - 1) << (limb_bits - 2 - limb_count_bits);
    static constexpr LimbT last_limb_mask_bits = limb_count_bits + 2; // takes into account in_place_mask, sign_mask, limb_count_mask
    static constexpr LimbT last_limb_mask = ~(((LimbT(1) << last_limb_mask_bits) - 1) << (limb_bits - last_limb_mask_bits));
    static constexpr size_t limbs_data_sizeof_in_limbs = (sizeof(limbs_data) + sizeof(LimbT) - 1) / sizeof(LimbT);

    using allocator_type = AllocatorT;
    using alloc_traits_t = std::allocator_traits<allocator_type>;

    alignas(limbs_data*) alignas(LimbT) LimbT inplace_limbs_[N];

    LimbT * allocate(size_t sz)
    {
        return alloc_traits_t::allocate(static_cast<allocator_type&>(*this), sz);
    }

    void deallocate(LimbT * ptr, size_t sz)
    {
        alloc_traits_t::deallocate(static_cast<allocator_type&>(*this), ptr, sz);
    }

    // small size optimized allocator
    struct sso_allocator_type
    {
        using value_type = LimbT;
        
        integer_holder& holder_;
        bool sso_allocation_ = false;
        inline explicit sso_allocator_type(integer_holder& h) noexcept : holder_{ h } {}

        LimbT* allocate(size_t cnt)
        {
            if (cnt <= N && !sso_allocation_) {
                return holder_.inplace_limbs_;
                sso_allocation_ = true;
            } else {
                return holder_.allocate(cnt + limbs_data_sizeof_in_limbs) + limbs_data_sizeof_in_limbs;
            }
        }

        void deallocate(LimbT* ptr, size_t sz)
        {
            LimbT * ibuff = holder_.inplace_limbs_;
            if (!std::equal_to<LimbT*>{}(ptr, ibuff)) {
                holder_.deallocate(ptr - limbs_data_sizeof_in_limbs, sz + limbs_data_sizeof_in_limbs);
            } else {
                sso_allocation_ = false;
            }
        }
    };

    inline sso_allocator_type sso_allocator() noexcept { return sso_allocator_type{ *this }; }

    explicit integer_holder()
    {
        init_zero();
    }

    template <typename AllocT>
    explicit integer_holder(AllocT&& alloc) : allocator_type{ std::forward<AllocT>(alloc) }
    {
        init_zero();
    }

    template <typename AllocT>
    integer_holder(basic_integer_view<LimbT> const& rhs, AllocT&& alloc)
        : allocator_type{ std::forward<AllocT>(alloc) }
    {
        init_copy<false>(rhs.data(), rhs.size(), rhs.sgn(), rhs.last_limb_mask());
    }

    template <std::integral T, typename AllocT>
    integer_holder(T value, AllocT && alloc)
        : allocator_type{ std::forward<AllocT>(alloc) }
    {
        ctl_limb(inplace_limbs_) = 0;
        init(to_limbs<LimbT>(value, sso_allocator()));
    }

    //template <typename AllocT>
    //integer_holder(std::string_view str, int base, AllocT && alloc)
    //    : allocator_type{ std::forward<AllocT>(alloc) }
    //{
    //    ctl_limb(inplace_limbs_) = 0;
    //    init(to_limbs<LimbT>(str, base, sso_allocator()));
    //}

    template <typename BuilderT, typename AllocT>
    requires(requires{ std::declval<BuilderT const&>()(std::declval<integer_holder&>()); })
    inline integer_holder(BuilderT const& iftor, AllocT&& alloc) : allocator_type{ std::forward<AllocT>(alloc) }
    {
        iftor(*this);
    }

    integer_holder(integer_holder const& rhs)
        : allocator_type{ static_cast<allocator_type const&>(rhs) }
    {
        LimbT rlimb = rhs.ctl_limb();
        if (integer_holder::is_inplaced(rlimb)) {
            if constexpr (N > 1) {
                std::copy(rhs.inplace_limbs_, rhs.inplace_limbs_ + integer_holder::inplaced_size(rlimb), inplace_limbs_);
            }
            integer_holder::ctl_limb(inplace_limbs_) = rlimb;
        } else {
            auto [rhs_ldata, rhs_limbs] = rhs.allocated_data_and_limbs();
            
            if (rhs_ldata->size < N || (rhs_ldata->size == N && !(rhs_limbs[rhs_ldata->size - 1] & ~last_limb_mask))) {
                // use inplace allocation
                std::copy(rhs_limbs, rhs_limbs + rhs_ldata->size, inplace_limbs_);
                inplaced_set_masks(rhs_ldata->size, rhs_ldata->sign ? -1 : 1);
            } else {
                LimbT* limbsdata = allocate(rhs_ldata->size + limbs_data_sizeof_in_limbs);
                std::copy(rhs_limbs, rhs_limbs + rhs_ldata->size, limbsdata + limbs_data_sizeof_in_limbs);
                new (limbsdata) detail::limbs_data{ *rhs_ldata };
                set_allocated(reinterpret_cast<detail::limbs_data*>(limbsdata));
            }
        }
    }

    template <size_t N2, typename AllocT>
    integer_holder(integer_holder<LimbT, N2, AllocT> const& rhs)
        : allocator_type{ static_cast<AllocT const&>(rhs) }
    {
        using rhs_t = integer_holder<LimbT, N2, AllocT>;
        LimbT ctl = rhs.ctl_limb();
        if (rhs_t::is_inplaced(ctl)) {
            LimbT const* rhs_limbs = rhs.inplace_limbs_;
            if constexpr (N2 == 1) {
                static_assert(N != 1);
                inplace_limbs_[0] = rhs_limbs[0] & rhs_t::last_limb_mask;
                integer_holder::ctl_limb(inplace_limbs_) = in_place_mask | (rhs_t::inplaced_is_negative(ctl) ? sign_mask : 0);
            } else {
                size_t sz = rhs_t::inplaced_size(ctl);
                init_copy<N >= N2>(rhs_limbs, sz, rhs_t::inplaced_is_negative(ctl) ? -1 : 1, sz == N2 ? rhs_t::last_limb_mask : no_mask);
            }
        } else {
            auto [rhs_ldata, rhs_limbs] = rhs.allocated_data_and_limbs();
            size_t sz = rhs_ldata->size;
            init_copy<false>(rhs_limbs, sz, rhs_ldata->sign ? -1 : 1);
        }
    }

    ~integer_holder()
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

    // max inplaced value
    void init_abs_max(int sign) noexcept
    {
        LimbT maxlimb = (std::numeric_limits<LimbT>::max)();
        if constexpr (N > 1) {
            std::fill(inplace_limbs_, inplace_limbs_ + N - 1, maxlimb);
        }
        maxlimb &= last_limb_mask | in_place_mask;
        if constexpr (limb_count_bits) {
            maxlimb |= limb_count_mask;
        }
        if (sign < 0) {
            maxlimb |= sign_mask;
        }
        inplace_limbs_[N - 1] = maxlimb;
    }

    // (limbs, size, allocated size, sign)
    inline void init(std::tuple<LimbT*, size_t, size_t, int> tpl)
    {
        LimbT* limbsdata;
        auto [limbs, sz, asz, sign] = tpl;
        if (!sz) {
            init_zero();
            return;
        }
        assert(limbs);
        if (std::equal_to<LimbT*>()(limbs, inplace_limbs_)) {
            // assert (sz <= N);
            if (sz < N || !(limbs[N - 1] & ~last_limb_mask)) { // sso case
                return inplaced_set_masks(sz, sign);
            } else { // need allocate
                limbsdata = allocate(sz + limbs_data_sizeof_in_limbs);
                std::copy(limbs, limbs + sz, limbsdata + limbs_data_sizeof_in_limbs);
            }
        } else { // libsdata is already allocated, but not initialized
            limbsdata = limbs - limbs_data_sizeof_in_limbs;
        }
        new (limbsdata) limbs_data{ .allocated_size = static_cast<uint32_t>(asz), .sign = (sign < 0) ? 1u : 0, .size = static_cast<uint32_t>(sz) };
        set_allocated(reinterpret_cast<limbs_data*>(limbsdata));
    }

    template <bool ForceInplaceV>
    void init_copy(LimbT const* rhs_limbs, size_t sz, int sign, LimbT most_significant_limb_mask = no_mask)
    {
        if (ForceInplaceV || sz < N || (N == sz && (!(rhs_limbs[sz - 1] & most_significant_limb_mask & ~last_limb_mask)))) {
            std::copy(rhs_limbs, rhs_limbs + sz, inplace_limbs_);
            inplace_limbs_[sz - 1] &= most_significant_limb_mask;
            if (N > sz) ctl_limb(inplace_limbs_) = 0;
            inplaced_set_masks(sz, sign);
        } else {
            LimbT* limbsdata = allocate(sz + limbs_data_sizeof_in_limbs);
            LimbT* limbs = limbsdata + limbs_data_sizeof_in_limbs;
            std::copy(rhs_limbs, rhs_limbs + sz, limbs);
            limbs[sz - 1] &= most_significant_limb_mask;
            new (limbsdata) limbs_data{ .allocated_size = static_cast<uint32_t>(sz), .sign = (sign < 0) ? 1u : 0, .size = static_cast<uint32_t>(sz) };
            set_allocated(reinterpret_cast<limbs_data*>(limbsdata));
        }
    }

    inline void negate()
    {
        LimbT & ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            ctl ^= sign_mask;
        } else {
            allocated_data()->sign ^= 1;
        }
    }
    
    inline LimbT const* laundered_data() const noexcept { return std::launder(inplace_limbs_); }
    inline LimbT* laundered_data() noexcept { return std::launder(inplace_limbs_); }

    inline static LimbT& ctl_limb(LimbT * data) noexcept { return data[N - 1]; }
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

    inline LimbT const* data() const noexcept
    {
        LimbT const* laundered_limbs = laundered_data();
        LimbT ctl = ctl_limb(laundered_limbs);
        // do not launder limbs if they store inplace data
        return is_inplaced(ctl) ? inplace_limbs_ : laundered_limbs;
    }

    inline LimbT * data() noexcept
    {
        LimbT * laundered_limbs = laundered_data();
        LimbT ctl = ctl_limb(laundered_limbs);
        // do not launder limbs if they store inplace data
        return is_inplaced(ctl) ? inplace_limbs_ : laundered_limbs;
    }

    inline static bool inplaced_is_negative(LimbT ctl) noexcept { return !!(ctl & sign_mask); }
    inline bool inplaced_is_negative() const noexcept { return inplaced_is_negative(ctl_limb()); }
        
    inline static size_t inplaced_size(LimbT ctl) noexcept
    {
        if constexpr (limb_count_bits) {
            return 1 + ((ctl & limb_count_mask) >> (limb_bits - limb_count_bits - 2)); // 2 bits are sso and sign bits
        } else {
            return 1;
        }
    }
    inline size_t inplaced_size() const noexcept { return inplaced_size(ctl_limb()); }

    // returns count of first used inplace_limbs_. ctl limb isn't taken into account if it doesn't contain value bits
    inline static size_t raw_significant_size(LimbT ctl)
    {
        if (is_inplaced(ctl)) return inplaced_size(ctl);
        return sizeof(limbs_data*) / sizeof(LimbT); // limbs used to store the pointer to limbs_data
    }

    inline std::span<const LimbT> inplaced_copy_significand_limbs(std::span<LimbT> buff) const
    {
        size_t cnt = (std::min)(buff.size(), inplaced_size());
        std::copy(inplace_limbs_, inplace_limbs_ + cnt, buff.data());
        if (N == cnt) {
            buff[N - 1] &= last_limb_mask;
        }
        return { buff.data(), cnt };
    }

    inline void inplaced_set_masks(size_t sz, int sign)
    {
        // limbs[N - 1] is 0 or the last limb
        LimbT mask = in_place_mask | (sign < 0 ? sign_mask : 0);
        if constexpr (limb_count_bits) {
            mask |= static_cast<LimbT>(sz - 1) << (limb_bits - 2 - limb_count_bits);
        }
        LimbT& ctl = integer_holder::ctl_limb(inplace_limbs_);
        ctl = (ctl & last_limb_mask) | mask;
    }

    void set_allocated(limbs_data * limbsdata)
    {
        static_assert(std::endian::native == std::endian::little, "unsupported endianness");

        if constexpr (!overwritten_limb_flag) {
            *reinterpret_cast<limbs_data**>(inplace_limbs_) = limbsdata;
            inplace_limbs_[N - 1] &= ~in_place_mask; // reset inplace bit
        } else {
            // litle endian case
            *reinterpret_cast<uintptr_t*>(inplace_limbs_) = std::rotr(reinterpret_cast<uintptr_t>(limbsdata), 1);
        }
    }

    inline limbs_data* allocated_data() noexcept
    {
        if constexpr (!overwritten_limb_flag) {
            return *std::launder(reinterpret_cast<limbs_data**>(inplace_limbs_));
        } else {
            return reinterpret_cast<limbs_data*>(std::rotl(*std::launder(reinterpret_cast<uintptr_t*>(inplace_limbs_)), 1));
        }
    }

    inline std::tuple<limbs_data const*, LimbT const*> allocated_data_and_limbs() const
    {
        if constexpr (!overwritten_limb_flag) {
            LimbT const* limbs = *std::launder(reinterpret_cast<LimbT const* const*>(inplace_limbs_)) + limbs_data_sizeof_in_limbs;
            limbs_data const* ldata = *std::launder(reinterpret_cast<limbs_data const* const*>(inplace_limbs_));
            return std::tuple{ ldata, limbs };
        } else {
            limbs_data const* ldata = reinterpret_cast<limbs_data const*>(std::rotl(*reinterpret_cast<uintptr_t const*>(inplace_limbs_), 1));
            LimbT const* limbs = std::launder((reinterpret_cast<LimbT const*>(ldata) + limbs_data_sizeof_in_limbs));
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
        auto [ldata, _] = allocated_data_and_limbs();
        return !!ldata->sign;
    }

    inline bool is_negative() const noexcept
    {
        LimbT ctl = ctl_limb();
        return is_inplaced(ctl) ? inplaced_is_negative(ctl) : allocated_is_negative();
    }

    bool is_zero() const noexcept
    {
        LimbT ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            return inplaced_size(ctl) == 1 && ((N == 1 && !(inplace_limbs_[0] & last_limb_mask)) || (N > 1 && !inplace_limbs_[0]));
        } else {
            auto limbs = allocated_limbs();
            return std::find_if(limbs.rbegin(), limbs.rend(), [](LimbT l) { return !!l; }) == limbs.rend();
        }
    }

    void swap(integer_holder & rhs) noexcept
    {
        LimbT& llimb = ctl_limb();
        LimbT& rlimb = rhs.ctl_limb();
        if (integer_holder::is_inplaced(llimb)) {
            if (!integer_holder::is_inplaced(rlimb)) return inplaced_swap_with_limbs_data(llimb, rhs);
        } else if (integer_holder::is_inplaced(rlimb)) {
            return rhs.inplaced_swap_with_limbs_data(rlimb, *this);
        } else {
            return swap_limbs_data(rhs);
        }
        // both inplace
        // trying valgrind friendly
        
        size_t lsz = integer_holder::inplaced_size(llimb);
        size_t rsz = integer_holder::inplaced_size(rlimb);
        size_t common_sz = (std::min)(lsz, rsz);
        
        LimbT* llimbs = inplace_limbs_;
        LimbT* rlimbs = rhs.inplace_limbs_;
        LimbT* llimbs_end = llimbs + common_sz;
        while (llimbs != llimbs_end) {
            LimbT tmp = *llimbs;
            *llimbs++ = *rlimbs;
            *rlimbs++ = tmp;
        }

        if (lsz > rsz) {
            std::copy(llimbs, inplace_limbs_ + (std::min)(lsz, N - 1), rlimbs);
        } else if (rsz > lsz) {
            std::copy(rlimbs, rhs.inplace_limbs_ + (std::min)(rsz, N - 1), llimbs);
        }
        if (common_sz < N) {
            std::swap(llimb, rlimb);
        }
    }

    inline void swap_limbs_data(integer_holder & rhs) noexcept
    {
        limbs_data* self_ld = allocated_data();
        set_allocated(rhs.allocated_data());
        rhs.set_allocated(self_ld);
    }

    void inplaced_swap_with_limbs_data(LimbT ctl, integer_holder & rhs) noexcept
    {
        limbs_data* rhs_ld = rhs.allocated_data();
        size_t sz = inplaced_size(ctl);
        std::copy(inplace_limbs_, inplace_limbs_ + sz, rhs.inplace_limbs_);
        if (sz < N) {
            integer_holder::ctl_limb(rhs.inplace_limbs_) = ctl;
        }
        set_allocated(rhs_ld);
    }

    std::pair<std::span<const LimbT>, LimbT> limbs() const noexcept
    {
        LimbT ctl = ctl_limb();
        if (is_inplaced(ctl)) {
            if constexpr (N == 1) {
                return { {}, ctl & last_limb_mask };
            } else {
                if (auto sz = inplaced_size(ctl); sz != N) {
                    return { std::span{ inplace_limbs_, sz }, 0 };
                } else {
                    return { std::span{ inplace_limbs_, N - 1 }, ctl & last_limb_mask };
                }
            }
        } else {
            auto [ldata, limbs] = allocated_data_and_limbs();
            return { std::span{limbs, ldata->size}, 0 };
        }
    }

    inline basic_integer_view<LimbT> significand() const noexcept
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

    //template <typename FunctorT>
    //inline decltype(auto) with_view(FunctorT const& ftor) const
    //{
    //    LimbT ctl = ctl_limb();
    //    if (is_inplaced(ctl)) {
    //        int sign = inplaced_is_negative(ctl) ? -1 : 1;
    //        size_t sz = inplaced_size(ctl);
    //        if (sz < N) { // no overlapping with ctl bits
    //            return ftor(basic_integer_view{ std::span{inplace_limbs_, sz}, sign });
    //        }
    //        LimbT tmp_buff[N];
    //        return ftor(basic_integer_view<LimbT>{ inplaced_copy_significand_limbs(tmp_buff), sign });
    //    } else {
    //        auto [ldata, limbs] = allocated_data_and_limbs();
    //        return ftor(basic_integer_view{ std::span{limbs, ldata->size}, ldata->sign ? -1 : 1 });
    //    }
    //}

    // no reenterable code (hacking a bit)
    //template <typename FunctorT>
    //inline decltype(auto) with_view2(FunctorT const& ftor)
    //{
    //    LimbT ctl = ctl_limb();
    //    if (is_inplaced(ctl)) {
    //        int sign = inplaced_is_negative(ctl) ? -1 : 1;
    //        size_t sz = inplaced_size(ctl);
    //        if (sz < N) { // no overlapping with ctl bits
    //            return ftor(basic_integer_view{ std::span{inplace_limbs_, sz}, sign });
    //        }
    //        inplace_limbs_[N - 1] = ctl & last_limb_mask;
    //        SCOPE_EXIT([this, ctl] { inplace_limbs_[N - 1] = ctl; });
    //        return ftor(basic_integer_view<LimbT>{ std::span{ inplace_limbs_, sz }, sign });
    //    } else {
    //        auto [ldata, limbs] = allocated_data_and_limbs();
    //        return ftor(basic_integer_view{ std::span{limbs, ldata->size}, ldata->sign ? -1 : 1 });
    //    }
    //}

    void do_move(integer_holder & rhs) noexcept
    {
        LimbT* rlimbs = rhs.data();
        LimbT& rctllimb = integer_holder::ctl_limb(rlimbs);
        size_t rsz = rhs.raw_significant_size(rctllimb);

        std::copy(rlimbs, rlimbs + rsz, inplace_limbs_);
        if (rsz < N) {
            integer_holder::ctl_limb(inplace_limbs_) = rctllimb;
        }
        rctllimb = in_place_mask; // rhs can contain a trash value after this
    }

    inline void free() noexcept
    {
        LimbT ctl = ctl_limb();
        if (!is_inplaced(ctl)) {
            detail::limbs_data* ldata = allocated_data();
            deallocate(reinterpret_cast<LimbT*>(ldata), ldata->allocated_size + limbs_data_sizeof_in_limbs);
        }
    }
};

template <std::unsigned_integral LimbT, size_t N, typename AllocatorT>
std::exception_ptr from_integer_string(integer_holder<LimbT, N, AllocatorT>& dh, std::string_view & str, int base = 0)
{
    using storage_type = integer_holder<LimbT, N, AllocatorT>;
    std::string_view orig_str = str;

    auto opt_tpl = to_limbs<LimbT>(str, base, dh.sso_allocator());
    if (!opt_tpl.has_value()) {
        std::string error;
        try { std::rethrow_exception(opt_tpl.error()); } catch (std::exception const& e) { error = e.what(); }
        return std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "string '"sv << orig_str << "' cannot be parsed as an integer, " << error).str()));
    }

    try {
        dh.init(*opt_tpl);
    } catch (std::bad_alloc const& e) {
        return std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "can't allocate an integer storage for '" << orig_str << "', error: " << e.what()).str()));
    } catch (...) {
        return std::current_exception();
    }

    return nullptr;
}
}

namespace sonia::mp {

template <std::unsigned_integral LimbT, size_t N = 1, typename AllocatorT = std::allocator<LimbT>>
class basic_integer
{
    static constexpr size_t expected_sizeof = (N * sizeof(LimbT) + sizeof(void*) - 1) & ~size_t(sizeof(void*) - 1); // a multiple of the pointer size
    static constexpr size_t actualN = expected_sizeof / sizeof(LimbT);
    
    using allocator_type = std::allocator_traits<AllocatorT>::template rebind_alloc<LimbT>;
    using alloc_traits_t = std::allocator_traits<allocator_type>;

    template <std::unsigned_integral LimbT2, size_t N2, typename AllocatorT2>
    friend class basic_integer;

    using alloc_holder = detail::integer_holder<LimbT, actualN, allocator_type>;

    alloc_holder aholder_;

    inline allocator_type& allocator() noexcept { return aholder_; }
    inline allocator_type const& allocator() const noexcept { return aholder_; }

public:
    basic_integer() = default;
    ~basic_integer() = default;

    using storage_type = alloc_holder; // for debug

    explicit basic_integer(AllocatorT const& alloc) : aholder_{ alloc } {}

    basic_integer(basic_integer_view<LimbT> const& rhs, AllocatorT const& alloc = AllocatorT{})
        : aholder_{ rhs, alloc }
    {}

    template <std::integral T>
    basic_integer(T value, AllocatorT const& alloc = AllocatorT{})
        : aholder_{ value, alloc }
    {}
    
    explicit basic_integer(std::string_view str, int base = 0, AllocatorT const& alloc = AllocatorT{}) // base=0 means autodetection
        : aholder_{ alloc }
    {
        std::string_view orig_str = str;
        if (auto eptr = from_integer_string(aholder_, str, base); eptr) {
            std::rethrow_exception(eptr);
        } else if (!str.empty()) {
            throw std::invalid_argument((std::ostringstream() << "can't parse string '"sv << orig_str << "' as an integer, stopped at: '"sv << str << '\'').str());
        }
    }

    static std::expected<basic_integer, std::exception_ptr> from_string(std::string_view & str, int base = 0, AllocatorT const& alloc = AllocatorT{}) noexcept
    {
        basic_integer result(alloc);
        if (auto eptr = from_integer_string(result.aholder_, str, base); eptr) {
            return std::unexpected(eptr);
        }
        return std::move(result);
    }

    template <typename BuilderT>
    requires(requires{ std::declval<BuilderT const&>()(std::declval<alloc_holder&>()); })
    explicit inline basic_integer(BuilderT const& iftor, AllocatorT const& alloc = AllocatorT{})
        : aholder_{ iftor, alloc }
    {}

    basic_integer(basic_integer const& rhs)
        : aholder_{ rhs.aholder_ }
    {}

    template <size_t N2, typename AllocatorT2>
    basic_integer(basic_integer<LimbT, N2, AllocatorT2> const& rhs)
        : aholder_{ rhs.aholder_ }
    {}

    basic_integer(basic_integer && rhs) noexcept
        : aholder_{ std::move(rhs.allocator()) }
    {
        aholder_.do_move(rhs.aholder_);
    }

    // BuilderT: (alloc_holder)->void
    template <typename BuilderT>
    requires(requires{ std::declval<BuilderT const&>()(std::declval<alloc_holder&>()); })
    inline basic_integer build_new(BuilderT const& builder) const
    {
        return basic_integer{ builder, allocator() };
    }

    inline static basic_integer inplace_max()
    {
        return basic_integer{[](alloc_holder& h) { h.init_abs_max(1); }};
    }

    inline static basic_integer inplace_min()
    {
        return basic_integer{[](alloc_holder& h) { h.init_abs_max(-1); }};
    }

    inline basic_integer& operator=(basic_integer_view<LimbT> const& rhs)
    {
        return this->operator=(basic_integer{ rhs });
    }

    template <std::integral T>
    inline basic_integer& operator=(T value)
    {
        return this->operator=(basic_integer{ value });
    }

    inline basic_integer& operator=(basic_integer const& rhs)
    {
        return this->operator=(basic_integer{ rhs });
    }

    basic_integer& operator=(basic_integer && rhs) noexcept
    {
        aholder_.free();
        allocator() = std::move(rhs.allocator());
        aholder_.do_move(rhs.aholder_);
        return *this;
    }

    explicit operator bool() const
    {
        return !aholder_.is_zero();
    }

    inline operator basic_integer_view<LimbT>() const
    {
        return aholder_.significand();
    }

    template <std::integral T>
    inline explicit operator T() const
    {
        return (T)aholder_.significand();
    }

    template <std::floating_point T>
    inline explicit operator T() const
    {
        return (T)aholder_.significand();
    }

    template <std::integral T>
    bool is_fit() const noexcept
    {
        return aholder_.significand().template is_fit<T>();
    }

    basic_integer operator- () const
    {
        basic_integer result{ *this };
        result.negate();
        return result;
    }
    
    template <std::integral TermT>
    inline basic_integer& operator+= (TermT r) { *this = *this + r; return *this; }
    inline basic_integer& operator+= (basic_integer_view<LimbT> r) { *this = *this + r; return *this; }
    inline basic_integer& operator+= (basic_integer const& r) { *this = *this + r; return *this; }

    template <std::integral TermT>
    inline basic_integer& operator-= (TermT r) { *this = *this - r; return *this; }
    inline basic_integer& operator-= (basic_integer_view<LimbT> r) { *this = *this - r; return *this; }
    inline basic_integer& operator-= (basic_integer const& r) { *this = *this - r; return *this; }

    template <std::integral MultiplierT>
    inline basic_integer& operator*= (MultiplierT r) { *this = *this * r; return *this; }
    inline basic_integer& operator*= (basic_integer_view<LimbT> r) { *this = *this * r; return *this; }
    inline basic_integer& operator*= (basic_integer const& r) { *this = *this * r; return *this; }

    template <std::integral DividerT>
    inline basic_integer& operator/= (DividerT r) { *this = *this / r; return *this; }
    inline basic_integer& operator/= (basic_integer_view<LimbT> r) { *this = *this / r; return *this; }
    inline basic_integer& operator/= (basic_integer const& r) { *this = *this / r; return *this; }

    template <std::integral DividerT>
    inline basic_integer& operator%= (DividerT r) { *this = *this % r; return *this; }
    inline basic_integer& operator%= (basic_integer_view<LimbT> r) { *this = *this % r; return *this; }
    inline basic_integer& operator%= (basic_integer const& r) { *this = *this % r; return *this; }

    inline int sgn() const noexcept { return aholder_.is_zero() ? 0 : (aholder_.is_negative() ? -1 : 1); }

    inline void negate() noexcept
    {
        aholder_.negate();
    }

    inline bool is_negative() const noexcept { return aholder_.is_negative(); }

    inline bool is_inplaced() const noexcept { return aholder_.is_inplaced(); }

    inline LimbT const* raw_data() const noexcept { return aholder_.laundered_data(); }
    inline LimbT * raw_data() noexcept { return aholder_.laundered_data(); }

    inline std::pair<std::span<const LimbT>, LimbT> limbs() const noexcept
    {
        return aholder_.limbs();
    }

    friend inline void swap(basic_integer & lhs, basic_integer& rhs) noexcept
    {
        lhs.aholder_.swap(rhs.aholder_);
    }

    friend inline std::string to_string(basic_integer const& val, int base = 10, bool show_base = true)
    {
        std::string result;

        LimbT buff[actualN];
        bool reversed;
        std::span<const LimbT> limbs = val.is_inplaced() ?
            val.aholder_.inplaced_copy_significand_limbs(std::span{buff}) : val.aholder_.allocated_limbs();
        
        if (val.is_negative()) result.push_back('-');
        if (show_base) {
            switch (base) {
            case 8: result.push_back('0'); break;
            case 16: result.push_back('0'); result.push_back('x'); break;
            }
        }
        size_t offset = result.size();
        to_string(limbs, std::back_inserter(result), reversed, base);
        
        if (reversed) {
            std::reverse(result.begin() + offset, result.end());
        }
        return result;
    }

    //template <typename FunctorT>
    //decltype(auto) with_view(FunctorT const& ftor) const
    //{
    //    return aholder_.with_view(ftor);
    //}

    //template <typename FunctorT>
    //decltype(auto) with_view2(FunctorT const& ftor)
    //{
    //    return aholder_.with_view2(ftor);
    //}
};


template <typename Elem, typename Traits, std::unsigned_integral LimbT, size_t N, typename AllocatorT>
inline std::basic_ostream<Elem, Traits>& operator <<(std::basic_ostream<Elem, Traits>& os, basic_integer<LimbT, N, AllocatorT> const& iv)
{
    int base;
    int flags = os.flags();
    if (flags & std::ios_base::hex) {
        base = 16;
    } else if (flags & std::ios_base::oct) {
        base = 8;
    } else {
        base = 10;
    }

    return os << to_string(iv, base, flags & std::ios_base::showbase);
}

template <std::unsigned_integral LimbT, size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
std::strong_ordering operator <=>(basic_integer<LimbT, LN, AllocatorLT> const& lhs, basic_integer<LimbT, RN, AllocatorRT> const& rhs)
{
    if (lhs.is_negative() && !rhs.is_negative()) return std::strong_ordering::less;
    if (!lhs.is_negative() && rhs.is_negative()) return std::strong_ordering::greater;

    auto [llimbs, last_llimb] = lhs.limbs();
    auto [rlimbs, last_rlimb] = rhs.limbs();

    // getting significant limb sizes
    size_t llimbs_sz = llimbs.size(), rlimbs_sz = rlimbs.size();
    llimbs_sz += last_llimb ? 1 : -std::count(llimbs.rbegin(), llimbs.rend(), 0);
    rlimbs_sz += last_rlimb ? 1 : -std::count(rlimbs.rbegin(), rlimbs.rend(), 0);
    
    if (llimbs_sz < rlimbs_sz) {
        return lhs.is_negative() ? std::strong_ordering::greater : std::strong_ordering::less;
    } else if (llimbs_sz > rlimbs_sz) {
        return lhs.is_negative() ? std::strong_ordering::less : std::strong_ordering::greater;
    } else if (!llimbs_sz) [[unlikely]] return std::strong_ordering::equal;

    // llimbs_sz == rlimbs_sz != 0 case
    if (auto res = (last_llimb ? last_llimb : llimbs.back()) <=> (last_rlimb ? last_rlimb : rlimbs.back()); res != std::strong_ordering::equal) {
        return (lhs.is_negative() ? 0 <=> res : res);
    }
    --llimbs_sz;

    if (llimbs_sz) {
        LimbT const* le = llimbs.data() + llimbs_sz;
        LimbT const* re = rlimbs.data() + llimbs_sz;
        do {
            --le; --re;
            if (auto res = *le <=> *re; res != std::strong_ordering::equal) {
                return (lhs.is_negative() ? 0 <=> res : res);
            }
            --llimbs_sz;
        } while (llimbs_sz);
    }
    return std::strong_ordering::equal;
}

template <std::unsigned_integral LimbT, size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
bool operator ==(basic_integer<LimbT, LN, AllocatorLT> const& lhs, basic_integer<LimbT, RN, AllocatorRT> const& rhs)
{
    if ((lhs.is_negative() && !rhs.is_negative()) || (!lhs.is_negative() && rhs.is_negative())) return false;

    auto [llimbs, last_llimb] = lhs.limbs();
    auto [rlimbs, last_rlimb] = rhs.limbs();

    // getting significant limb sizes
    size_t llimbs_sz = llimbs.size(), rlimbs_sz = rlimbs.size();
    llimbs_sz += last_llimb ? 1 : -std::count(llimbs.rbegin(), llimbs.rend(), 0);
    rlimbs_sz += last_rlimb ? 1 : -std::count(rlimbs.rbegin(), rlimbs.rend(), 0);

    if (llimbs_sz != rlimbs_sz) return false;
    else if (!llimbs_sz) [[unlikely]] return true;

    // llimbs_sz == rlimbs_sz != 0 case
    if ((last_llimb ? last_llimb : llimbs.back()) != (last_rlimb ? last_rlimb : rlimbs.back())) return false;
    --llimbs_sz;

    if (llimbs_sz) {
        LimbT const* le = llimbs.data() + llimbs_sz;
        LimbT const* re = rlimbs.data() + llimbs_sz;
        do {
            --le; --re;
            if (*le != *re) return false;
            --llimbs_sz;
        } while (llimbs_sz);
    }
    return true;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral RT>
auto operator <=>(basic_integer<LimbT, LN, AllocatorLT> const& lhs, RT rhs)
{
    // 1 + sizeof(RT) / sizeof(LimbT) ensures no dynamic allocation for RT representation in basic_integer
    return lhs <=> basic_integer<LimbT, 1 + sizeof(RT) / sizeof(LimbT), AllocatorLT>{ rhs };
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral RT>
bool operator ==(basic_integer<LimbT, LN, AllocatorLT> const& lhs, RT rhs)
{
    // 1 + sizeof(RT) / sizeof(LimbT) ensures no dynamic allocation for RT representation in basic_integer
    return lhs == basic_integer<LimbT, 1 + sizeof(RT) / sizeof(LimbT), AllocatorLT>{ rhs };
}

using integer = basic_integer<uint64_t, 1>;

namespace literals {

inline integer operator""_bi(const char* str, std::size_t sz)
{
    return integer{std::string_view{ str, sz }};
}

template <char... Chars> struct has_hex_prefix : std::false_type {};
template <char c0, char c1, char... Chars> struct has_hex_prefix<c0, c1, Chars...> : std::bool_constant<c0 == '0' && c1 == 'x' && sizeof...(Chars)> {};

template <char... Chars> struct has_oct_prefix : std::false_type {};
template <char c0, char... Chars> struct has_oct_prefix<c0, Chars...> : std::bool_constant<c0 == '0' && sizeof...(Chars)> {};


template <std::unsigned_integral LimbT, char c0, char c1, char... Chars>
inline basic_integer<LimbT, 1> from_hex_chars()
{
    using limbs_t = typename sonia::mpct::literals::mul_base_plus_method<LimbT{ 16 }, sonia::mpct::limbs<LimbT>, sonia::mpct::literals::hex_char_to_digit(Chars) ... >::type;
    return basic_integer<LimbT, 1>{ basic_integer_view<LimbT>{ limbs_t{} } };
}

template <std::unsigned_integral LimbT, char c0, char... Chars>
inline basic_integer<LimbT, 1> from_oct_chars()
{
    using limbs_t = typename sonia::mpct::literals::mul_base_plus_method<LimbT{ 8 }, sonia::mpct::limbs<LimbT>, sonia::mpct::literals::oct_char_to_digit(Chars) ... >::type;
    return basic_integer<LimbT, 1>{ basic_integer_view<LimbT>{ limbs_t{} } };
}

template <std::unsigned_integral LimbT, char... Chars>
inline basic_integer<LimbT, 1> from_dec_chars()
{
    using limbs_t = typename sonia::mpct::literals::mul_base_plus_method<LimbT{ 10 }, sonia::mpct::limbs<LimbT>, sonia::mpct::literals::dec_char_to_digit(Chars) ... >::type;
    return basic_integer<LimbT, 1>{ basic_integer_view<LimbT>{ limbs_t{} } };
}

template <char... Chars>
inline integer operator""_bi()
{
    using limb_t = uint64_t;
    if constexpr (has_hex_prefix<Chars ...>::value) {
        return from_hex_chars<limb_t, Chars ...>();
    } else if constexpr (has_oct_prefix<Chars ...>::value) {
        return from_oct_chars<limb_t, Chars ...>();
    } else {
        return from_dec_chars<limb_t, Chars ...>();
    }
}

}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT>
inline basic_integer<LimbT, LN, AllocatorLT> operator+ (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer_view<LimbT> rv)
{
    return l.build_new([lv = (basic_integer_view<LimbT>)l, rv](auto& ih) { ih.init(add(lv, rv, ih.sso_allocator())); });
}

template <std::unsigned_integral LimbT, size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
inline basic_integer<LimbT, LN, AllocatorLT> operator+ (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer<LimbT, RN, AllocatorRT> const& r)
{
    return l + (basic_integer_view<LimbT>)r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral TermT>
inline basic_integer<LimbT, LN, AllocatorLT> operator+ (basic_integer<LimbT, LN, AllocatorLT> const& l, TermT r)
{
    if constexpr (sizeof(TermT) <= sizeof(LimbT)) {
        return l + basic_integer_view<LimbT>{ r };
    } else {
        return l + basic_integer<LimbT, 1 + sizeof(TermT) / sizeof(LimbT), AllocatorLT>{ r };
    }
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral TermT>
inline basic_integer<LimbT, LN, AllocatorLT> operator+ (TermT l, basic_integer<LimbT, LN, AllocatorLT> const& r)
{
    return r + basic_integer<LimbT, 1 + sizeof(TermT) / sizeof(LimbT), AllocatorLT>{ l };
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT>
inline basic_integer<LimbT, LN, AllocatorLT> operator- (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer_view<LimbT> rv)
{
    return l.build_new([lv = (basic_integer_view<LimbT>)l, rv](auto& ih) { ih.init(sub(lv, rv, ih.sso_allocator())); });
}

template <std::unsigned_integral LimbT, size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
inline basic_integer<LimbT, LN, AllocatorLT> operator- (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer<LimbT, RN, AllocatorRT> const& r)
{
    return l - (basic_integer_view<LimbT>)r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral TermT>
inline basic_integer<LimbT, LN, AllocatorLT> operator- (basic_integer<LimbT, LN, AllocatorLT> const& l, TermT r)
{
    if constexpr (sizeof(TermT) <= sizeof(LimbT)) {
        return l - basic_integer_view<LimbT>{ r };
    } else {
        return l - basic_integer<LimbT, 1 + sizeof(TermT) / sizeof(LimbT), AllocatorLT>{ r };
    }
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral TermT>
inline basic_integer<LimbT, LN, AllocatorLT> operator- (TermT l, basic_integer<LimbT, LN, AllocatorLT> const& r)
{
    return basic_integer<LimbT, 1 + sizeof(TermT) / sizeof(LimbT), AllocatorLT>{ l } - r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT>
inline basic_integer<LimbT, LN, AllocatorLT> operator* (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer_view<LimbT> rv)
{
    return l.build_new([lv = (basic_integer_view<LimbT>)l, rv](auto& ih) { ih.init(mul(lv, rv, ih.sso_allocator())); });
}

template <std::unsigned_integral LimbT, size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
inline basic_integer<LimbT, LN, AllocatorLT> operator* (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer<LimbT, RN, AllocatorRT> const& r)
{
    return l * (basic_integer_view<LimbT>)r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral MultiplierT>
inline basic_integer<LimbT, LN, AllocatorLT> operator* (basic_integer<LimbT, LN, AllocatorLT> const& l, MultiplierT r)
{
    if constexpr (sizeof(MultiplierT) <= sizeof(LimbT)) {
        return l * basic_integer_view<LimbT>{ r };
    } else {
        return l * basic_integer<LimbT, 1 + sizeof(MultiplierT) / sizeof(LimbT), AllocatorLT>{ r };
    }
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral MultiplierT>
inline basic_integer<LimbT, LN, AllocatorLT> operator* (MultiplierT l, basic_integer<LimbT, LN, AllocatorLT> const& r)
{
    if constexpr (sizeof(MultiplierT) <= sizeof(LimbT)) {
        return r * basic_integer_view<LimbT>{ l };
    } else {
        return r * basic_integer<LimbT, 1 + sizeof(MultiplierT) / sizeof(LimbT), AllocatorLT>{ l };
    }
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT>
inline basic_integer<LimbT, LN, AllocatorLT> operator/ (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer_view<LimbT> rv)
{
    return l.build_new([lv = (basic_integer_view<LimbT>)l, rv](auto& ih) { ih.init(div(lv, rv, ih.sso_allocator())); });
}

template <std::unsigned_integral LimbT, size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
inline basic_integer<LimbT, LN, AllocatorLT> operator/ (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer<LimbT, RN, AllocatorRT> const& r)
{
    return l / (basic_integer_view<LimbT>)r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral DividerT>
inline basic_integer<LimbT, LN, AllocatorLT> operator/ (basic_integer<LimbT, LN, AllocatorLT> const& l, DividerT r)
{
    if constexpr (sizeof(DividerT) <= sizeof(LimbT)) {
        return l / basic_integer_view<LimbT>{ r };
    } else {
        return l / basic_integer<LimbT, 1 + sizeof(DividerT) / sizeof(LimbT), AllocatorLT>{ r };
    }
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral DividendT>
inline basic_integer<LimbT, LN, AllocatorLT> operator/ (DividendT l, basic_integer<LimbT, LN, AllocatorLT> const& r)
{
    return basic_integer<LimbT, 1 + sizeof(DividendT) / sizeof(LimbT), AllocatorLT>{ l } / r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT>
inline basic_integer<LimbT, LN, AllocatorLT> operator% (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer_view<LimbT> rv)
{
    return l.build_new([lv = (basic_integer_view<LimbT>)l, rv](auto& ih) { ih.init(mod(lv, rv, ih.sso_allocator())); });
}

template <std::unsigned_integral LimbT, size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
inline basic_integer<LimbT, LN, AllocatorLT> operator% (basic_integer<LimbT, LN, AllocatorLT> const& l, basic_integer<LimbT, RN, AllocatorRT> const& r)
{
    return l % (basic_integer_view<LimbT>)r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral DividerT>
inline basic_integer<LimbT, LN, AllocatorLT> operator% (basic_integer<LimbT, LN, AllocatorLT> const& l, DividerT r)
{
    if constexpr (sizeof(DividerT) <= sizeof(LimbT)) {
        return l % basic_integer_view<LimbT>{ r };
    } else {
        return l % basic_integer<LimbT, 1 + sizeof(DividerT) / sizeof(LimbT), AllocatorLT>{ r };
    }
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT, std::integral DividendT>
inline basic_integer<LimbT, LN, AllocatorLT> operator% (DividendT l, basic_integer<LimbT, LN, AllocatorLT> const& r)
{
    return basic_integer<LimbT, 1 + sizeof(DividendT) / sizeof(LimbT), AllocatorLT>{ l } % r;
}

template <std::unsigned_integral LimbT, size_t LN, typename AllocatorLT>
inline basic_integer<LimbT, LN, AllocatorLT> pow(basic_integer<LimbT, LN, AllocatorLT> const& l, unsigned int n)
{
    return l.build_new([lv = (basic_integer_view<LimbT>)l, n](auto& ih) { ih.init(pow(lv, n, ih.sso_allocator())); });
}

}
