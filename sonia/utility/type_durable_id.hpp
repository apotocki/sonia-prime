//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_TYPE_DURABLE_ID_HPP
#define SONIA_UTILITY_TYPE_DURABLE_ID_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <mutex>
#include <iosfwd>

#include "sonia/prime_config.hpp"
#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/explicit_operator_bool.hpp"
#include "sonia/utility/serialization/serialization_fwd.hpp"

namespace sonia {

// !!! type::durable_id.val_ != type::id.val_ !!!

namespace services {

SONIA_PRIME_API uint32_t register_durable_id(string_view, std::type_info const&);
SONIA_PRIME_API uint32_t get_durable_id(std::type_info const&);

}

namespace type {

class durable_id {
    SONIA_DECLARE_SERIALIZATION_FRIENDLY;
    
    explicit durable_id(uint32_t idval) noexcept : val_(idval) {}

    template <typename T>
    struct cache {
        static uint32_t cached_val_;
        static std::once_flag once_flag_;
        cache() {
            std::call_once(once_flag_, []() {
                cache<T>::cached_val_ = sonia::services::get_durable_id(typeid(T));
            });
        }
        explicit cache(string_view nm) {
            std::call_once(once_flag_, [nm]() {
                cache<T>::cached_val_ = sonia::services::register_durable_id(nm, typeid(T));
            });
        }
        uint32_t get() const noexcept { return cached_val_; }
    };

public:
    durable_id(std::type_info const& ti) : val_(sonia::services::get_durable_id(ti)) {}

    //durable_id() noexcept : val_(0) {}

    //BOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL()

    //constexpr bool operator!() const noexcept { return !val_; }

    template <class T, typename ... ArgsT>
    static durable_id get(ArgsT && ... args) {
        return durable_id(cache<T>(std::forward<ArgsT>(args) ...).get());
    }

    template <typename CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, durable_id const& v){
        return os << v.val_;
    }

private:
    uint32_t val_;
};

template <typename T>
std::once_flag durable_id::cache<T>::once_flag_{};

template <typename T>
uint32_t durable_id::cache<T>::cached_val_{0};

}}

#endif // SONIA_UTILITY_TYPE_DURABLE_ID_HPP
