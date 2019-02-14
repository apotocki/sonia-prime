//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIMIZED_DECIMAL_HPP
#define SONIA_UTILITY_OPTIMIZED_DECIMAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/number/decimal.hpp"
#include "holder.hpp"

namespace sonia {

template <typename SignedT, typename UnsignedT>
SignedT unsigned_to_signed(UnsignedT val, UnsignedT sign_mask)
{
    return (val & sign_mask) ? (-((SignedT)(val & (~sign_mask))) - 1) : (SignedT)val;
}

template <typename UnsignedT, typename SignedT>
UnsignedT signed_to_unsigned(SignedT val, UnsignedT sign_mask)
{
    return val < 0 ? ((UnsignedT)-val) + sign_mask : (UnsignedT)val;
}

template <class HolderT, typename RefCountT = uint32_t>
struct optimized_decimal_impl
{
    using optimized_decimal = optimized_wrapper<decimal, RefCountT>;
    using uint_t = typename HolderT::uint_t;
    using int_t = typename boost::int_t<HolderT::value_bits>::fast;

    static const size_t exp_bits = 4 + 3 * HolderT::value_bits / 4;
    static const size_t mnts_bits = HolderT::value_bits - exp_bits;

    static const uint_t mnts_mask = (((uint_t)1) << mnts_bits) - 1;
    static const uint_t mnts_sign_mask = ((uint_t)1) << (mnts_bits - 1);
    static const uint_t exp_mask = (((uint_t)1) << exp_bits) - 1;
    static const uint_t exp_sign_mask = ((uint_t)1) << (exp_bits - 1);

    static const int_t mnts_max = (int_t)mnts_mask;
    static const int_t mnts_min = -((int_t)mnts_mask) - 1;
    static const int_t exp_max = (int_t)exp_mask;
    static const int_t exp_min = -((int_t)exp_mask) - 1;

    template <typename ... ArgsT>
    static void init(HolderT * self, ArgsT&& ... args)
    {
        self->init_not_ptr();
        if constexpr (sizeof...(ArgsT)) {
            set(self, decimal(std::forward<ArgsT>(args) ...));
        }
    }

    template <typename T>
    static T get(HolderT const* self)
    {
        if (self->is_ptr()) {
            return (T)ptr(self)->get();
        } else {
            uint_t rval = self->get_uint();
            basic_decimal<int_t, int_t> d(
                unsigned_to_signed<int_t>(rval & mnts_mask, mnts_sign_mask),
                unsigned_to_signed<int_t>(rval >> mnts_bits, exp_sign_mask)
            );
            return (T)d;
        }
    }

    static decimal get(HolderT const* self)
    {
        if (self->is_ptr()) {
            return ptr(self)->get();
        } else {
            uint_t rval = self->get_uint();
            return decimal(
                unsigned_to_signed<int_t>(rval & mnts_mask, mnts_sign_mask),
                (int32_t)unsigned_to_signed<int_t>(rval >> mnts_bits, exp_sign_mask)
            );
        }
    }

    static void set(HolderT * self, decimal val)
    {
        if (self->is_ptr()) {
            ptr(self)->set(std::move(val));
        } else {
            if (val.raw_value() >= (int_t)mnts_min && val.raw_value() <= (int_t)mnts_max &&
                val.raw_exp() >= exp_min && val.raw_exp() <= exp_max)
            {
                self->set_uint(
                    signed_to_unsigned(val.raw_value(), mnts_sign_mask) |
                    (signed_to_unsigned(val.raw_exp(), exp_sign_mask) << mnts_bits)
                );
            } else {
                self->set_pointer(new optimized_decimal(std::move(val)));
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

#endif // SONIA_UTILITY_OPTIMIZED_DECIMAL_HPP
