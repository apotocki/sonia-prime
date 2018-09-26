//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_TYPE_INFO_HPP
#define SONIA_UTILITY_TYPE_INFO_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <typeinfo>
#include <iosfwd>

#include "explicit_operator_bool.hpp"
#include "sonia/functional/hash.hpp"

namespace sonia {

class type_info {
    class empty_flag {};

public:
    type_info(std::type_info const& arg = typeid(empty_flag)) : pinfo_(&arg) {}

    std::type_info const& get() const { return *pinfo_; }
    const char* name() const { return pinfo_->name(); }

    bool operator!() const noexcept { return empty(); }
    bool empty() const noexcept { return *pinfo_ == typeid(empty_flag); }

    BOOST_EXPLICIT_OPERATOR_BOOL();

private:
    std::type_info const* pinfo_;
};

inline bool operator== (type_info const& lhs, type_info const& rhs) {
    return lhs.get() == rhs.get();
}

inline bool operator== (type_info const& lhs, std::type_info const& rhs) {
    return lhs.get() == rhs;
}

inline bool operator== (std::type_info const& lhs, type_info const& rhs) {
    return lhs == rhs.get();
}

inline bool operator!= (type_info const& lhs, type_info const& rhs) {
    return !(lhs == rhs);
}

inline bool operator!= (type_info const& lhs, std::type_info const& rhs) {
    return lhs.get() != rhs;
}

inline bool operator!= (std::type_info const& lhs, type_info const& rhs) {
    return lhs != rhs.get();
}

inline bool operator< (type_info const& lhs, type_info const& rhs) {
    return lhs.get().before(rhs.get());
}

inline bool operator< (type_info const& lhs, std::type_info const& rhs) {
    return lhs.get().before(rhs);
}

inline bool operator< (std::type_info const& lhs, type_info const& rhs) {
    return lhs.before(rhs.get());
}

inline bool operator> (type_info const& lhs, type_info const& rhs) {
    return rhs < lhs;
}

inline bool operator<= (type_info const& lhs, type_info const& rhs) {
    return !(lhs > rhs);
}

inline bool operator>= (type_info const& lhs, type_info const& rhs) {
    return !(lhs < rhs);
}

template <typename ElemT, typename TraitsT>
std::basic_ostream<ElemT, TraitsT> & operator<< (std::basic_ostream<ElemT, TraitsT> & os, type_info const& val) {
    return os << "typeinfo(" << val.name() << ")";
}

template<>
struct hash<type_info> {
    size_t operator()(type_info const& s) const {
        return s.get().hash_code();
    }
};

}

#endif // SONIA_UTILITY_TYPE_INFO_HPP
