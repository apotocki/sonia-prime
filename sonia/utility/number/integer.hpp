//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>
#include <type_traits>

#include <boost/multiprecision/cpp_int.hpp>

#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

namespace sonia {

class integer
{
public:
    using integer_type = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>;

    integer() : value_{0} {}

    template <typename T>
    requires(!std::is_same_v<integer, std::remove_cvref_t<T>>)
    explicit integer(T && val) : value_{std::forward<T>(val)} {}

    explicit integer(string_view str);

    integer(integer const&) = default;
    integer(integer &&) = default;
    
    template <typename T>
    requires(!std::is_same_v<integer, std::remove_cvref_t<T>>)
    integer& operator= (T&& val) { value_ = std::forward<T>(val); return *this; }

    integer& operator= (integer const& rhs) = default;
    integer& operator= (integer && rhs) = default;

    template <typename T>
    T get() const
    {
        return value_.convert_to<T>();
    }

    std::string string() const;
    size_t hash() const;
    integer_type const& raw() const { return value_; }
    integer_type & raw() { return value_; }

    auto const* data() const { return value_.backend().limbs(); }
    auto* data() { return value_.backend().limbs(); }
    size_t data_size() const { return value_.backend().size(); }

    bool operator! () const
    {
        return !value_;
    }

    friend inline bool operator==(const integer&, const integer&) = default;
    friend inline auto operator<=>(const integer&, const integer&) = default;

    template <typename T>
    friend inline bool operator==(const integer& lhs, const T& rhs)
    {
        if constexpr (std::is_floating_point_v<T>) {
            return lhs.value_.convert_to<T>() == rhs;
        } else {
            return lhs.value_ == rhs;
        }
    }

    template <typename T>
    friend inline auto operator<=>(const integer& lhs, const T& rhs)
    {
        if constexpr (std::is_floating_point_v<T>) {
            return lhs.value_.convert_to<T>() <=> rhs;
        } else {
            return lhs.value_ <=> rhs;
        }
    }

    // operations
    inline integer operator- () const
    {
        return integer(-value_);
    }

    inline integer & operator+= (integer const& rhs)
    {
        value_ += rhs.value_;
        return *this;
    }

    inline integer & operator-= (integer const& rhs)
    {
        value_ -= rhs.value_;
        return *this;
    }

    inline integer & operator*= (integer const& rhs)
    {
        value_ *= rhs.value_;
        return *this;
    }

    inline integer & operator/= (integer const& rhs)
    {
        value_ /= rhs.value_;
        return *this;
    }

private:
    integer_type value_;
};

template <typename ElemT, typename TraitsT>
std::basic_ostream<ElemT, TraitsT> & operator<< (std::basic_ostream<ElemT, TraitsT> & os, integer const& val)
{
    return os << val.raw();
}

inline size_t hash_value(integer const& x)
{
    return x.hash();
}

inline integer operator+ (integer const& lhs, integer const& rhs)
{
    integer result(lhs);
    result += rhs;
    return result;
}

inline integer operator- (integer const& lhs, integer const& rhs)
{
    integer result(lhs);
    result -= rhs;
    return result;
}

inline integer operator* (integer const& lhs, integer const& rhs)
{
    integer result(lhs);
    result *= rhs;
    return result;
}

inline integer operator/ (integer const& lhs, integer const& rhs)
{
    integer result(lhs);
    result /= rhs;
    return result;
}

}
