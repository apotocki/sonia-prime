//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIMIZED_INTEGER_HPP
#define SONIA_UTILITY_OPTIMIZED_INTEGER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include "sonia/type_traits.hpp"
#include "sonia/functional/is_negative.hpp"
#include "sonia/functional/abs.hpp"
#include "sonia/utility/number/integer.hpp"

#include "holder.hpp"

namespace sonia {

/*
template <class HolderT> struct optimized_integer_impl;

class optimized_integer : public optimized_base
{
public:
    template <typename ArgT>
    explicit optimized_integer(ArgT && arg)
        : val_(std::forward<ArgT>(arg))
    { }

    bool is_zero() const { return !val_; }

    template <typename T>
    T get() const { return val_.get<T>(); }

    integer const& get() const { return val_; }

    template <typename T>
    void set(T && arg) {
        val_ = std::forward<T>(arg);
    }

    void dispose() noexcept override {
        delete this;
    }

    optimized_base * clone() const override {
        return new optimized_integer(*this);
    }

    template <typename ArgT>
    static optimized_integer * create(ArgT && arg) {
        return new optimized_integer(std::forward<ArgT>(arg));
    }

private:
    integer val_;
};

*/

template <class HolderT>
struct optimized_integer_impl
{
    typedef optimized_wrapper<integer> optimized_integer;

    typedef typename HolderT::uint_t uint_t;
    static const uint_t sign_mask = (HolderT::uint_max >> 1) + 1;

    template <typename ... ArgsT>
    static void init(HolderT * self, ArgsT&& ... args) {
        self->init_not_ptr();
        if constexpr (sizeof(ArgsT) != 0) {
            set(self, std::forward<ArgsT>(args) ...);
        }
    }

    static bool is_zero(HolderT const* self) {
        return self->is_ptr() ? ptr(self)->is_zero() : !self->get_uint();
    }

    template <typename T>
    static T get(HolderT const* self) {
        if (self->is_ptr()) {
            return ptr(self)->template get<T>();
        } else {
            uint_t rval = self->get_uint();
            if constexpr (is_signed_v<T>) {
                return (rval & sign_mask) ? (-((T)(rval & (~sign_mask))) - 1) : (T)rval;
            } else {
                return (rval & sign_mask) ? (T)(~(rval & (~sign_mask))) + 1 : (T)rval;
            }
        }
    }

    static integer get_integer(HolderT const* self) {
        if (self->is_ptr()) {
            return ptr(self)->get();
        } else {
            auto v = self->get_uint();
        }
    }

    template <typename T>
    static void set(HolderT * self, T val) {
        if (self->is_ptr()) {
            ptr(self)->set(std::forward<T>(val));
        } else {
            bool is_neg = signbit(val);
            T hval = is_neg ? abs(val + 1) : val;
            if (hval <= (HolderT::uint_max >> 1)) {
                self->set_uint((uint_t)hval | (is_neg ? sign_mask : 0));
            } else {
                self->set_pointer(new optimized_integer(val));
            }
        }
    }

private:
    static optimized_integer * ptr(HolderT * self) {
        return static_cast<optimized_integer *>(self->get_pointer());
    }

    static optimized_integer const* ptr(HolderT const* self) {
        return static_cast<optimized_integer const*>(self->get_pointer());
    }
};

}

#endif // SONIA_UTILITY_OPTIMIZED_INTEGER_HPP
