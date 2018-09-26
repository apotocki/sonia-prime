//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_TYPE_ID_HPP
#define SONIA_UTILITY_TYPE_ID_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <mutex>
#include <iosfwd>

#include "sonia/prime_config.hpp"
#include "sonia/cstdint.hpp"
#include "sonia/utility/explicit_operator_bool.hpp"
#include "sonia/utility/on_close.hpp"

namespace sonia {

namespace services {
    SONIA_PRIME_API uint32_t get_type_id(std::type_info const&);
}

namespace type {

class id {
    explicit id(uint32_t idval) noexcept : val_(idval) {}

    template <typename T>
    struct cache {
        static uint32_t cached_val_;
        static std::once_flag once_flag_;
        cache() {
            std::call_once(once_flag_, []() {
                cache<T>::cached_val_ = sonia::services::get_type_id(typeid(T));
                sonia::services::on_close([]() { cache<T>::once_flag_ = std::once_flag{}; });
            });
        }
        uint32_t get() const noexcept { return cached_val_; }
    };

public:
    //id() noexcept : val_(0) {}

    //BOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL()

    //constexpr bool operator!() const noexcept { return !val_; }

    template <class T>
    static id get() noexcept {
        return id(cache<T>().get());
    }

    template <typename CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, id const& v){
        return os << v.val_;
    }

private:
    uint32_t val_;
};

template <typename T>
std::once_flag id::cache<T>::once_flag_{};

template <typename T>
uint32_t id::cache<T>::cached_val_{0};

}}

#endif // SONIA_UTILITY_TYPE_ID_HPP
