//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_DECIMAL_IPP
#define SONIA_UTILITY_DECIMAL_IPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

template <typename SignificandT, typename ExponentT>
void decimal_normilize(SignificandT & v, ExponentT & e)
{
    if (v) {
        for (;;) {
            if (0 != v % 10) break;
            v /= 10;
            ++e;
        }
    }
}

template <typename SignificandT, typename ExponentT>
void decimal_parse(string_view str, SignificandT & v, ExponentT & e)
{
    SignificandT value = 0;
    ExponentT exp = 0;
    while (!str.empty())
    {
        bool has_minus = false;
        char const* p = str.begin();
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
        v = value;
        e = exp;
        return;
    }

    throw exception("wrong decimal string: '%1%'"_fmt % str);
}

template <typename SignificandT, typename ExponentT>
std::string decimal_string(SignificandT const& v, ExponentT const& e)
{
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
}

template <typename SignificandT, typename ExponentT>
std::string decimal_scientific_string(SignificandT const& v, ExponentT const& e)
{
    if (v == 0) return "0";
    std::string result = boost::lexical_cast<std::string>(v);
    ExponentT sc_e = e;
    for (; result.back() == '0'; result.pop_back(), ++sc_e);

    int pos = v < 0 ? 1 : 0;

    sc_e += (ExponentT)(result.size() - 1);
    sc_e -= pos;
    
    result.insert(result.begin() + 1 + pos, '.');
    result.push_back('E');
    result.append(std::to_string(sc_e));
    return result;
}

template <typename LSignificandT, typename LExponentT, typename RSignificandT, typename RExponentT>
bool decimal_less(LSignificandT const& lv, LExponentT const& le, RSignificandT const& rv, RExponentT const& re)
{
    if ((lv < 0 && rv >= 0) || (lv == 0 && rv > 0)) return true;
    if (rv <= 0 && lv >= 0) return false;
    if (le <= re) {
        return lv < rv * pow(RSignificandT(10), re - le);
    } else {
        return lv * pow(LSignificandT(10), le - re) < rv;
    }
}

template <typename LSignificandT, typename LExponentT, typename RSignificandT, typename RExponentT>
bool decimal_equal(LSignificandT const& lv, LExponentT const& le, RSignificandT const& rv, RExponentT const& re)
{
    if ((lv < 0 && rv >= 0) || (lv == 0 && rv != 0) || (lv > 0 && rv <= 0)) return false;
    if (lv == 0) return true;
    if (le <= re) {
        return lv == rv * pow(RSignificandT(10), re - le);
    } else {
        return lv * pow(LSignificandT(10), le - re) == rv;
    }
}

}

#endif // SONIA_UTILITY_DECIMAL_IPP
