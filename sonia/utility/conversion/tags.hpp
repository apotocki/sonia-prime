/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"
#include "error_handling.hpp"

namespace sonia::conversion { 

struct cvt_tag {};

template <typename T>
struct type_tag : cvt_tag
{
    typedef type_tag customer_tag;
    typedef type_tag provider_tag;
};

typedef type_tag<char> int8_tag;
typedef type_tag<uint16_t> int16_tag;
typedef type_tag<uint32_t> int32_tag;
typedef type_tag<uint64_t> int64_tag;


template <typename T>
struct array_tag : cvt_tag
{
    typedef array_tag customer_tag;
    typedef array_tag provider_tag;
};

template <typename left_hand_tag, typename right_hand_tag>
class combine_tag : cvt_tag
{
public:
    typedef left_hand_tag customer_tag;
    typedef right_hand_tag provider_tag;

    combine_tag(left_hand_tag const& lv, right_hand_tag const& rv)
        : value_(lv, rv)
    {}

    left_hand_tag const& cvalue() const { return value_.first; }
    right_hand_tag const& pvalue() const { return value_.second; }

private:
    std::pair<left_hand_tag, right_hand_tag> value_;
};

template <typename tagT>
inline tagT const& pvalue(tagT const& tagval) { return tagval; }

template <typename tagT>
inline tagT const& cvalue(tagT const& tagval) { return tagval; }

template <typename left_hand_tag, typename right_hand_tag>
inline right_hand_tag const& pvalue(combine_tag<left_hand_tag, right_hand_tag> const& ctv)
{ return ctv.pvalue(); }

template <typename left_hand_tag, typename right_hand_tag>
inline left_hand_tag const& cvalue(combine_tag<left_hand_tag, right_hand_tag> const& ctv)
{ return ctv.cvalue(); }

template <typename left_tag, typename right_tag> 
combine_tag<left_tag, right_tag> operator / (left_tag const& l, right_tag const& r) {
    return combine_tag<left_tag, right_tag>(l, r);
}

inline int8_tag int8() { return int8_tag(); }
inline int16_tag int16() { return int16_tag(); }
inline int32_tag int32() { return int32_tag(); }

}
