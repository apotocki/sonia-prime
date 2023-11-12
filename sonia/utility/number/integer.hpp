//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>

#include <boost/multiprecision/cpp_int.hpp>

#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"
#include "sonia/utility/comparison_operators.hpp"

namespace sonia {

class integer
{
public:
    using integer_type = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>;

    integer() : value_(0) {}

    template <typename T>
    explicit integer(T && val) : value_(do_assign(std::forward<T>(val))) {}

    explicit integer(string_view str);

    integer(integer const& rhs) : value_(rhs.value_) {}
    integer(integer && rhs) : value_(std::move(rhs.value_)) {}

    template <typename T>
    void operator= (T && val) { value_ = do_assign(std::forward<T>(val)); }

    void operator= (integer const& rhs) { value_ = rhs.value_; }
    void operator= (integer && rhs) { value_ = std::move(rhs.value_); }

    template <typename T>
    T get() const
    {
        return value_.convert_to<T>();
    }

    std::string string() const;
    size_t hash() const;
    integer_type const& raw() const { return value_; }
    integer_type & raw() { return value_; }

    bool operator! () const
    {
        return !value_;
    }

    bool operator== (integer const& rhs) const
    {
        return value_ == rhs.value_;
    }

    bool operator< (integer const& rhs) const
    {
        return value_ < rhs.value_;
    }

    bool operator> (integer const& rhs) const
    {
        return value_ > rhs.value_;
    }

    // float    
    bool operator== (float_t const& rhs) const
    {
        return value_.convert_to<float_t>() == rhs;
    }

    bool operator< (float_t const& rhs) const
    {
        return value_.convert_to<float_t>() < rhs;
    }

    bool operator> (float_t const& rhs) const
    {
        return value_.convert_to<float_t>() > rhs;
    }

    // double_t
    bool operator== (double_t const& rhs) const
    {
        return value_.convert_to<double_t>() == rhs;
    }

    bool operator< (double_t const& rhs) const
    {
        return value_.convert_to<double_t>() < rhs;
    }

    bool operator> (double_t const& rhs) const
    {
        return value_.convert_to<double_t>() > rhs;
    }

    template <typename T>
    bool operator== (T && rhs) const
    {
        return value_ == std::forward<T>(rhs);
    }

    template <typename T>
    bool operator< (T && rhs) const
    {
        return value_ < std::forward<T>(rhs);
    }

    template <typename T>
    bool operator> (T && rhs) const
    {
        return value_ > std::forward<T>(rhs);
    }

    MAKE_COMPARISON_OPERATORS(integer)

    // operations
    integer operator- () const
    {
        return integer(-value_);
    }

    BOOST_FORCEINLINE integer & operator+= (integer const& rhs)
    {
        value_ += rhs.value_;
        return *this;
    }

    BOOST_FORCEINLINE integer & operator-= (integer const& rhs)
    {
        value_ -= rhs.value_;
        return *this;
    }

    BOOST_FORCEINLINE integer & operator*= (integer const& rhs)
    {
        value_ *= rhs.value_;
        return *this;
    }

    BOOST_FORCEINLINE integer & operator/= (integer const& rhs)
    {
        value_ /= rhs.value_;
        return *this;
    }

private:
    static integer_type const& do_assign(integer & rhs) { return rhs.value_; }
    static integer_type const& do_assign(integer const && rhs) { return rhs.value_; }

    template <typename T>
    static T && do_assign(T && rhs) { return std::forward<T>(rhs); }

    //static integer_type do_assign(decimal & rhs) { return rhs.get<integer_type>(); }
    //static integer_type do_assign(decimal const& rhs) { return rhs.get<integer_type>(); }
    //static integer_type do_assign(decimal && rhs) { return rhs.get<integer_type>(); }
    //static integer_type do_assign(decimal const&& rhs) { return rhs.get<integer_type>(); }

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
    return std::move(result);
}

inline integer operator- (integer const& lhs, integer const& rhs)
{
    integer result(lhs);
    result -= rhs;
    return std::move(result);
}

inline integer operator* (integer const& lhs, integer const& rhs)
{
    integer result(lhs);
    result *= rhs;
    return std::move(result);
}

inline integer operator/ (integer const& lhs, integer const& rhs)
{
    integer result(lhs);
    result /= rhs;
    return std::move(result);
}

}
