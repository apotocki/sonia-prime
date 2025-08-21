//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <concepts>

#include "numetron/basic_decimal.hpp"
#include "numetron/decimal_view.hpp"
#include "holder.hpp"

namespace sonia::detail {

template <std::unsigned_integral LimbT, typename RefCountT, class AllocatorT = std::allocator<LimbT>>
class optimized_decimal
    : public optimized_base<RefCountT>
    , AllocatorT
    , numetron::detail::decimal_data
{
    using alloc_traits_t = std::allocator_traits<AllocatorT>;

    static constexpr size_t self_size_in_limbs() noexcept
    {
        return (sizeof(optimized_decimal) + sizeof(LimbT) - 1) / sizeof(LimbT);
    }

    AllocatorT& allocator() { return static_cast<AllocatorT&>(*this); }

    explicit optimized_decimal(AllocatorT const& alloc = AllocatorT{})
        : AllocatorT{alloc}
    {}

public:
    numetron::basic_decimal_view<uint64_t> value() const noexcept
    {
        LimbT const* limbs = reinterpret_cast<LimbT const*>(this) + self_size_in_limbs();

        numetron::basic_integer_view<LimbT> sig{std::span{ limbs, this->size }, this->sign ? -1 : 1 };
        auto exp = !this->allocated_exponent
            ? numetron::basic_integer_view<LimbT>{ this->exponent }
            : numetron::basic_integer_view<LimbT>{
                std::span{ limbs + this->size, static_cast<size_t>(std::abs(this->exponent)) },
                this->exponent > 0 ? 1 : -1
              };
        return numetron::basic_decimal_view<uint64_t>{std::move(sig), std::move(exp)};
    }

    static optimized_decimal* create(numetron::basic_decimal_view<LimbT> const& dv, AllocatorT && alloc = AllocatorT{})
    {
        uint32_t limb_sz_to_allocate = static_cast<uint32_t>(dv.significand().size());
        bool inplace_exp = dv.exponent().template is_fit<int64_t>();
        if (!inplace_exp) {
            limb_sz_to_allocate += static_cast<uint32_t>(dv.exponent().size());
        }
        LimbT* ptr = alloc_traits_t::allocate(alloc, self_size_in_limbs() + limb_sz_to_allocate);
        optimized_decimal * self = new (ptr) optimized_decimal{ alloc };
        LimbT* limbs = ptr + self_size_in_limbs();
        self->allocated_size = limb_sz_to_allocate;
        self->sign = dv.is_negative() ? 1 : 0;
        if (inplace_exp) {
            self->allocated_exponent = 0;
            self->size = limb_sz_to_allocate;
            self->exponent = (int64_t)dv.exponent();
        } else {
            self->allocated_exponent = 1;
            self->size = static_cast<uint32_t>(dv.significand().size());
            self->exponent = dv.exponent().size();
            std::copy(dv.exponent().data(), dv.exponent().data() + 1, limbs + self->size);
            if (dv.exponent().most_significant_skipping_bits()) {
                *(limbs + limb_sz_to_allocate - 1) &= dv.exponent().last_significand_limb_mask();
            }
        }
        std::copy(dv.significand().data(), dv.significand().data() + 1, limbs);
        if (dv.significand().most_significant_skipping_bits()) {
            *(limbs + self->size - 1) &= dv.significand().last_significand_limb_mask();
        }
        return self;
    }

    optimized_base<RefCountT>* clone() const override
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    void dispose() noexcept override
    {
        AllocatorT alloc = allocator();
        size_t sz = this->size;
        this->~optimized_decimal();
        LimbT * limbs = reinterpret_cast<LimbT*>(this);
        
        alloc_traits_t::deallocate(alloc, limbs, self_size_in_limbs() + sz);
    }

    optimized_decimal& operator=(numetron::basic_decimal_view<LimbT>)
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }


};

}

namespace sonia {

template <typename SignedT, typename UnsignedT>
SignedT unsigned_to_signed(UnsignedT val, UnsignedT sign_mask)
{
    return (val & sign_mask) ? -((SignedT)(val & (~sign_mask))) : (SignedT)val;
}

template <typename UnsignedT, typename SignedT>
UnsignedT signed_to_unsigned(SignedT val, UnsignedT sign_mask)
{
    return val < 0 ? ((UnsignedT)-val) + sign_mask : (UnsignedT)val;
}

template <class HolderT, typename RefCountT = uint32_t>
struct optimized_decimal_impl
{
    using uint_t = typename HolderT::uint_t;
    using int_t = typename boost::int_t<HolderT::value_bits>::fast;
    using limb_t = typename numetron::decimal_view::limb_type;
    using optimized_decimal = sonia::detail::optimized_decimal<limb_t, RefCountT>;

    static const size_t mnts_bits = 4 + 3 * HolderT::value_bits / 4;
    static const size_t exp_bits = HolderT::value_bits - mnts_bits;

    static const uint_t mnts_mask = (((uint_t)1) << mnts_bits) - 1;
    static const uint_t mnts_sign_mask = ((uint_t)1) << (mnts_bits - 1);
    static const uint_t exp_mask = (((uint_t)1) << exp_bits) - 1;
    static const uint_t exp_sign_mask = ((uint_t)1) << (exp_bits - 1);

    static const int_t mnts_max = (int_t)(mnts_mask >> 1);
    static const int_t mnts_min = -mnts_max - 1;
    static const int_t exp_max = (int_t)(exp_mask >> 1);
    static const int_t exp_min = -exp_max - 1;

    
    static void init(HolderT * self, numetron::decimal_view dv)
    {
        self->init_not_ptr();
        set(self, dv);
    }

    template <typename T>
    static T get(HolderT const* self)
    {
        if (self->is_ptr()) {
            return (T)ptr(self)->value();
        } else {
            uint_t rval = self->get_uint();
            numetron::basic_decimal_view<uint64_t> d(
                unsigned_to_signed<int_t>(rval & mnts_mask, mnts_sign_mask),
                unsigned_to_signed<int_t>(rval >> mnts_bits, exp_sign_mask)
            );
            return (T)d;
        }
    }

    static numetron::basic_decimal_view<uint64_t> get(HolderT const* self)
    {
        if (self->is_ptr()) {
            return ptr(self)->value();
        } else {
            uint_t rval = self->get_uint();
            return numetron::basic_decimal_view<uint64_t>(
                unsigned_to_signed<int_t>(rval & mnts_mask, mnts_sign_mask),
                (int32_t)unsigned_to_signed<int_t>(rval >> mnts_bits, exp_sign_mask)
            );
        }
    }

    static void set(HolderT * self, numetron::decimal_view val)
    {
        if (self->is_ptr()) {
            *ptr(self) = val;
        } else {
            if (val.significand() >= mnts_min && val.significand() <= mnts_max &&
                val.exponent() >= exp_min && val.exponent() <= exp_max)
            {
                self->set_uint(
                    signed_to_unsigned((int_t)val.significand(), mnts_sign_mask) |
                    (signed_to_unsigned(val.exponent(), exp_sign_mask) << mnts_bits)
                );
            } else {
                self->set_pointer(optimized_decimal::create(val));
            }
        }
    }

private:
    static optimized_decimal * ptr(HolderT * self)
    {
        return static_cast<optimized_decimal *>(self->get_pointer());
    }

    static optimized_decimal const* ptr(HolderT const* self)
    {
        return static_cast<optimized_decimal const*>(self->get_pointer());
    }
};

}
