//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_TYPE_DURABLE_ID_HPP
#define SONIA_UTILITY_TYPE_DURABLE_ID_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <mutex> // call_once
#include <iosfwd>
#include <typeindex>

#include "sonia/prime_config.hpp"
#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/explicit_operator_bool.hpp"
#include "sonia/utility/serialization/serialization_fwd.hpp"

namespace sonia {

// !!! type::durable_id.val_ != type::id.val_ !!!

namespace services {

SONIA_PRIME_API uint32_t register_durable_id(string_view, string_view, std::type_index);
SONIA_PRIME_API uint32_t get_durable_id(std::type_index);
SONIA_PRIME_API std::type_index get_durable_type_index(uint32_t);

}

namespace type {

class durable_id {
    SONIA_DECLARE_SERIALIZATION_FRIENDLY;
    
    durable_id(uint32_t idval, std::type_index ti)
        : val_(idval), ti_(ti)
    {}

    template <typename T> friend class cache;
    
    template <typename T>
    class cache {
        static uint32_t cached_val_;
        static std::once_flag once_flag_;
        friend class durable_id;

        cache() {
            std::call_once(once_flag_, []() {
                cache<T>::cached_val_ = sonia::services::get_durable_id(typeid(T));
            });
        }

        cache(string_view nm, string_view servnm) {
            std::call_once(once_flag_, [nm, servnm]() {
                cache<T>::cached_val_ = sonia::services::register_durable_id(nm, servnm, typeid(T));
            });
        }

        durable_id get() const noexcept { return durable_id{cached_val_, typeid(T)}; }
    };

public:
    explicit durable_id(uint32_t idval)
        : val_(idval)
        , ti_(sonia::services::get_durable_type_index(idval))
    {}

    explicit durable_id(std::type_index ti)
        : val_(sonia::services::get_durable_id(ti))
        , ti_(ti)
    {}

    std::type_index const& ti() const { return ti_; }

    template <class T>
    static durable_id get() {
        return cache<T>().get();
    }

    template <class T>
    static durable_id get(string_view nm, string_view servnm) {
        return cache<T>(nm, servnm).get();
    }

    template <typename CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, durable_id const& v) {
        return os << v.val_ << " (" << v.ti_.name() << ")";
    }

private:
    uint32_t val_;
    std::type_index ti_;
};

template <typename T>
std::once_flag durable_id::cache<T>::once_flag_{};

template <typename T>
uint32_t durable_id::cache<T>::cached_val_{0};

}}

#endif // SONIA_UTILITY_TYPE_DURABLE_ID_HPP
