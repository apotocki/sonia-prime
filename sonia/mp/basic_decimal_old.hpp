//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <boost/lexical_cast.hpp>
#include "sonia/string.hpp"
#include "sonia/optional.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/functional/hash.hpp"

namespace sonia {

template <typename SignificandT, typename ExponentT>
class basic_decimal
{
public:
    basic_decimal() noexcept
        : value_(0), exponent_10_(0)
    {}

    template <typename SomeSignificandT, typename SomeExponentT>
    basic_decimal(SomeSignificandT s, SomeExponentT e)
        : value_(std::move(s)), exponent_10_(std::move(e))
    {}

    template <typename SomeSignificandT, typename SomeExponentT>
    explicit basic_decimal(basic_decimal<SomeSignificandT, SomeExponentT> const& rhs)
        : value_(rhs.value_), exponent_10_(rhs.exponent_10_)
    {}

    template <typename T>
    explicit basic_decimal(T val)
    {
        this->operator= (std::move(val));
    }

    basic_decimal(basic_decimal const&) = default;
    basic_decimal(basic_decimal &&) = default;
    basic_decimal& operator=(basic_decimal const&) = default;
    basic_decimal& operator=(basic_decimal&&) = default;

    template <typename SomeSignificandT, typename SomeExponentT>
    basic_decimal & operator= (basic_decimal<SomeSignificandT, SomeExponentT> const& rhs)
    {
        value_ = rhs.value_;
        exponent_10_ = rhs.exponent_10_;
        return *this;
    }

    template <typename T>
    basic_decimal & operator= (T val)
    {
        if constexpr (is_integral_v<T>) {
            *this = basic_decimal(val, 0);
        } else {
            *this = parse(boost::lexical_cast<std::string>(val));
        }

        return *this;
    }

    SignificandT const& raw_value() const { return value_; }
    ExponentT const& raw_exp() const { return exponent_10_; }

    template <typename T>
    T get() const;

    template <typename T>
    requires(!is_same_v<T, basic_decimal>)
    explicit operator T() const { return get<T>(); }

    static basic_decimal parse(string_view);
    static optional<basic_decimal> parse_no_throw(string_view);

    void operator += (basic_decimal const& rhs);
    void operator -= (basic_decimal const& rhs);
    void operator *= (basic_decimal const& rhs);
    void operator /= (basic_decimal const& rhs);

private:
    SignificandT value_;
    ExponentT exponent_10_;
};

template <class T> struct is_decimal : false_type {};
template <typename SignificandT, typename ExponentT> struct is_decimal<basic_decimal<SignificandT, ExponentT>> : true_type {};
template <class T> constexpr bool is_decimal_v = is_decimal<T>::value;

} // sonia

#include "decimal.ipp"

namespace sonia {

template <typename SignificandT, typename ExponentT>
inline optional<basic_decimal<SignificandT, ExponentT>> basic_decimal<SignificandT, ExponentT>::parse_no_throw(string_view sval)
{
    if (auto optpair = decimal_parse<SignificandT, ExponentT>(sval); optpair) {
        return basic_decimal(std::move(optpair->first), std::move(optpair->second));
    }
    return nullopt;
}

template <typename SignificandT, typename ExponentT>
inline basic_decimal<SignificandT, ExponentT> basic_decimal<SignificandT, ExponentT>::parse(string_view sval)
{
    if (auto optres = parse_no_throw(sval); optres) return *optres;
    throw exception("can't convert to decimal: '%1%'"_fmt % sval);
}

template <typename SignificandT, typename ExponentT>
template <typename T>
T basic_decimal<SignificandT, ExponentT>::get() const
{
    if constexpr (is_floating_point_v<T>) {
        return (T)((T)value_ * std::pow(10.0, exponent_10_));
    } else {
        if (exponent_10_ == 0) {
            return (T)value_; // .convert_to<T>();
        } else if (exponent_10_ > 0) {
            return (T)(value_ * pow(SignificandT(10), exponent_10_)); // .convert_to<T>();
        } else {
            return (T)(value_ / pow(SignificandT(10), -exponent_10_)); // .convert_to<T>();
        }
    }
}

template <typename SignificandT, typename ExponentT>
std::string to_string(basic_decimal<SignificandT, ExponentT> const& val)
{
    return decimal_string(val.raw_value(), val.raw_exp());
}

template <typename SignificandT, typename ExponentT>
std::string to_scientific_string(basic_decimal<SignificandT, ExponentT> const& val)
{
    return decimal_scientific_string(val.raw_value(), val.raw_exp());
}

template <typename LSignificandT, typename LExponentT, typename RSignificandT, typename RExponentT>
bool operator== (basic_decimal<LSignificandT, LExponentT> const& lhs, basic_decimal<RSignificandT, RExponentT> const& rhs) noexcept
{
    return decimal_equal(lhs.raw_value(), lhs.raw_exp(), rhs.raw_value(), rhs.raw_exp());
}

template <typename LSignificandT, typename LExponentT, typename T>
bool operator== (basic_decimal<LSignificandT, LExponentT> const& lhs, T const& rhs)
{
    return lhs == basic_decimal<LSignificandT, LExponentT>(rhs);
}

template <typename LSignificandT, typename LExponentT, typename T>
bool operator== (T const& lhs, basic_decimal<LSignificandT, LExponentT> const& rhs) { return rhs == lhs; }


template <typename LSignificandT, typename LExponentT, typename RSignificandT, typename RExponentT>
auto operator<=> (basic_decimal<LSignificandT, LExponentT> const& lhs, basic_decimal<RSignificandT, RExponentT> const& rhs) noexcept
{
    return decimal_compare_three_way(lhs.raw_value(), lhs.raw_exp(), rhs.raw_value(), rhs.raw_exp());
}

template <typename LSignificandT, typename LExponentT, typename T>
auto operator<=> (basic_decimal<LSignificandT, LExponentT> const& lhs, T const& rhs)
{
    return lhs <=> basic_decimal<LSignificandT, LExponentT>(rhs);
}

template <typename LSignificandT, typename LExponentT, typename T>
auto operator<=> (T const& lhs, basic_decimal<LSignificandT, LExponentT> const& rhs)
{
    return basic_decimal<LSignificandT, LExponentT>(lhs) <=> rhs;
}

template <class OStreamT, typename SignificandT, typename ExponentT>
auto & operator<< (OStreamT & os, basic_decimal<SignificandT, ExponentT> const& val)
{
    return os << to_string(val);
}

template <typename SignificandT, typename ExponentT>
struct hash<basic_decimal<SignificandT, ExponentT>>
{
    size_t operator()(basic_decimal<SignificandT, ExponentT> const& s) const
    {
        return hasher()(s.raw_value(), s.raw_exp());
    }
};

template <typename SignificandT, typename ExponentT>
inline size_t hash_value(basic_decimal<SignificandT, ExponentT> const& v)
{
    return hash<basic_decimal<SignificandT, ExponentT>>()(v);
}

template <typename SignificandT, typename ExponentT>
void basic_decimal<SignificandT, ExponentT>::operator += (basic_decimal<SignificandT, ExponentT> const& rhs)
{
    decimal_add(value_, exponent_10_, rhs.value_, rhs.exponent_10_);
}

template <typename SignificandT, typename ExponentT>
void basic_decimal<SignificandT, ExponentT>::operator -= (basic_decimal<SignificandT, ExponentT> const& rhs)
{
    decimal_minus(value_, exponent_10_, rhs.value_, rhs.exponent_10_);
}

template <typename SignificandT, typename ExponentT>
void basic_decimal<SignificandT, ExponentT>::operator *= (basic_decimal<SignificandT, ExponentT> const& rhs)
{
    decimal_mul(value_, exponent_10_, rhs.value_, rhs.exponent_10_);
}

template <typename SignificandT, typename ExponentT>
void basic_decimal<SignificandT, ExponentT>::operator /= (basic_decimal<SignificandT, ExponentT> const& rhs)
{
    decimal_divide(value_, exponent_10_, rhs.value_, rhs.exponent_10_);
}

template <typename SignificandT, typename ExponentT>
basic_decimal<SignificandT, ExponentT> operator+ (basic_decimal<SignificandT, ExponentT> const& lhs, basic_decimal<SignificandT, ExponentT> const& rhs)
{
    basic_decimal<SignificandT, ExponentT> result{lhs};
    result += rhs;
    return result;
}

template <typename SignificandT, typename ExponentT>
basic_decimal<SignificandT, ExponentT> operator- (basic_decimal<SignificandT, ExponentT> const& lhs, basic_decimal<SignificandT, ExponentT> const& rhs)
{
    basic_decimal<SignificandT, ExponentT> result{lhs};
    result -= rhs;
    return result;
}

template <typename SignificandT, typename ExponentT>
basic_decimal<SignificandT, ExponentT> operator* (basic_decimal<SignificandT, ExponentT> const& lhs, basic_decimal<SignificandT, ExponentT> const& rhs)
{
    basic_decimal<SignificandT, ExponentT> result{lhs};
    result *= rhs;
    return result;
}

template <typename SignificandT, typename ExponentT>
basic_decimal<SignificandT, ExponentT> operator/ (basic_decimal<SignificandT, ExponentT> const& lhs, basic_decimal<SignificandT, ExponentT> const& rhs)
{
    basic_decimal<SignificandT, ExponentT> result{lhs};
    result /= rhs;
    return result;
}

}
