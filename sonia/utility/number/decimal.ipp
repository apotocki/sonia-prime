//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

//template <typename ExponentT>
//ExponentT exponent_sub(ExponentT minuend, ExponentT subtrahend)
//{
//    if (minuend < 0 && subtrahend <= 0 || minuend > 0 && subtrahend >= 0) return minuend - subtrahend;
//    if (minuend <= 0 && subtrahend >= 0) { // -5 - 6
//    
//    }
//}

template <typename SignificandT, typename ExponentT>
void decimal_normilize(SignificandT & v, ExponentT & e)
{
    if (e != (std::numeric_limits<ExponentT>::max)()) {
        if (v) {
            for (;;) {
                if (0 != v % 10) break;
                v = v / 10;
                ++e;
            }
        }
    } else if (v > 0) { // INF
        v = 1;
    } else if (v < 0) { // -INF
        v = -1;
    } // else v = 0, NAN
}

template <typename SignificandT, typename ExponentT>
std::pair<SignificandT, ExponentT> decimal_parse(string_view str)
{
    // to do: parser INF, -INF, NAN
    std::pair<SignificandT, ExponentT> result{ 0, 0 };
    SignificandT & value = result.first;
    ExponentT & exp = result.second;
    while (!str.empty())
    {
        bool has_minus = false;
        auto p = str.begin();
        if (*p == '+') {
            ++p;
        } else if (*p == '-') {
            has_minus = true;
            ++p;
        }
        if (p == str.end()) break;

        bool has_point = false;
        do {
            char c = *p;
            if (c >= '0' && c <= '9') {
                value = value * 10 + int(c - '0');
                if (has_point) --exp;
            } else if (c == '.' && !has_point) {
                has_point = true;
            } else {
                break;
            }
            ++p;
        } while (p != str.end());

        if (p != str.end()) {
            if (*p != 'e' && *p != 'E') break;
            ++p;
            if (p == str.end()) break;
            bool exp_has_minus = false;
            if (*p == '+') {
                ++p;
            } else if (*p == '-') {
                exp_has_minus = true;
                ++p;
            }
            ExponentT expval = 0;
            for (; p != str.end(); ++p)
            {
                char c = *p;
                if (c < '0' || c > '9') break;
                expval = expval * 10 + int(c - '0');
            }
            if (p == str.end()) {
                exp += expval * (exp_has_minus ? -1 : 1);
            }
        }

        if (p != str.end()) break;

        decimal_normilize(value, exp);

        if (has_minus) {
            value = -value;
        }
        return result;
    }

    throw exception("wrong decimal string: '%1%'"_fmt % str);
}

template <typename SignificandT, typename ExponentT>
std::string decimal_string(SignificandT const& v, ExponentT const& e)
{
    if (e != (std::numeric_limits<ExponentT>::max)()) {
        std::string result = boost::lexical_cast<std::string>(v);
        if (e >= 0) {
            result.resize(result.size() + e, '0');
        } else {
            int pos = v < 0 ? 1 : 0;
            int zpadcount = -e - (int)result.size() + pos + 1;
            if (zpadcount > 0) {
                result.insert(result.begin() + pos, zpadcount, '0');
            }
            result.insert(result.begin() + result.size() + e, '.');
        }
        return std::move(result);
    } else if (v > 0) {
        return "INF";
    } else if (v < 0) {
        return "-INF";
    } else {
        return "NAN";
    }
}

template <typename SignificandT, typename ExponentT>
std::string decimal_scientific_string(SignificandT const& v, ExponentT const& e)
{
    if (v == 0) return "0.0E0";
    std::string result = boost::lexical_cast<std::string>(v);
    ExponentT sc_e = e;
    for (; result.back() == '0'; result.pop_back(), ++sc_e);

    int pos = v < 0 ? 1 : 0;

    sc_e += (ExponentT)(result.size() - 1);
    sc_e -= pos;
    
    result.insert(result.begin() + 1 + pos, '.');
    if (result.back() == '.') result.pop_back();
    result.push_back('E');
    result.append(std::to_string(sc_e));
    return result;
}

template <typename LSignificandT, typename LExponentT, typename RSignificandT, typename RExponentT>
std::strong_ordering decimal_compare_three_way(LSignificandT const& lv, LExponentT const& le, RSignificandT const& rv, RExponentT const& re)
{
    if ((lv < 0 && rv >= 0) || (lv == 0 && rv > 0)) return std::strong_ordering::less;
    if ((rv < 0 && lv >= 0) || (rv == 0 && lv > 0)) return std::strong_ordering::greater;
    if (lv == rv && le == re) return std::strong_ordering::equal;
    if (le == re) {
        return lv <=> rv;
    } else if (le < re) {
        return lv <=> (rv * pow(RSignificandT(10), re - le));
    } else {
        return (lv * pow(LSignificandT(10), le - re)) <=> rv;
    }
}

template <typename LSignificandT, typename LExponentT, typename RSignificandT, typename RExponentT>
bool decimal_less(LSignificandT const& lv, LExponentT const& le, RSignificandT const& rv, RExponentT const& re)
{
    if ((lv < 0 && rv >= 0) || (lv == 0 && rv > 0)) return true;
    if (rv <= 0 && lv >= 0) return false;
    if (le == re) {
        return lv < rv;
    } else if (le < re) {
        return lv < rv * pow(RSignificandT(10), re - le);
    } else {
        return lv * pow(LSignificandT(10), le - re) < rv;
    }
}

template <typename LSignificandT, typename LExponentT, typename RSignificandT, typename RExponentT>
bool decimal_equal(LSignificandT const& lv, LExponentT const& le, RSignificandT const& rv, RExponentT const& re)
{
    if (le == (std::numeric_limits<LExponentT>::max)() || re == (std::numeric_limits<RExponentT>::max)()) return false;
    if ((lv < 0 && rv >= 0) || (lv > 0 && rv <= 0)) return false;
    if (!lv) return !rv;
    if (le == re) {
        return lv == rv;
    } else if (le < re) {
        return lv == rv * pow(RSignificandT{10}, re - le);
    } else {
        return lv * pow(LSignificandT{10}, le - re) == rv;
    }
}

template <typename SignificandT, typename ExponentT>
void decimal_add(SignificandT & lv, ExponentT & le, SignificandT const& rv, ExponentT const& re)
{
    ExponentT minexp = (std::min)(le, re);
    if (le != minexp) {
        lv = lv * pow(SignificandT(10), le - minexp) + rv;
        le = minexp;
    } else if (re != minexp) {
        lv += rv * pow(SignificandT(10), re - minexp);
    } else {
        lv += rv;
    }
    decimal_normilize(lv, le);
}

template <typename SignificandT, typename ExponentT>
void decimal_minus(SignificandT & lv, ExponentT & le, SignificandT const& rv, ExponentT const& re)
{
    ExponentT minexp = (std::min)(le, re);
    if (le != minexp) {
        lv = lv * pow(SignificandT(10), le - minexp) - rv;
        le = minexp;
    } else if (re != minexp) {
        lv -= rv * pow(SignificandT(10), re - minexp);
    } else {
        lv -= rv;
    }
    decimal_normilize(lv, le);
}

template <typename SignificandT, typename ExponentT>
void decimal_mul(SignificandT & lv, ExponentT & le, SignificandT const& rv, ExponentT const& re)
{
    lv *= rv;
    le += re;
    decimal_normilize(lv, le);
}

template <typename SignificandT, typename ExponentT>
void decimal_divide(SignificandT & lv, ExponentT & le, SignificandT const& rv, ExponentT const& re)
{
    if (rv == 0) {
        le = (std::numeric_limits<ExponentT>::max)();
    } else if (lv) {
        auto val = msb(lv);
        if (val < 63) {
            int p = (int)log10(pow(2.0, 63 - val));
            lv = lv * pow(SignificandT(10), p);
            le -= p;
        }
        lv /= rv;
        le -= re;
    }
    decimal_normilize(lv, le);
}

}
