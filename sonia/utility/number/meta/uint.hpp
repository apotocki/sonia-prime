//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/cstdint.hpp"

#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/list_c.hpp>

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

#include <boost/preprocessor/tuple/elem.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/div.hpp> 
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/less.hpp>

#ifndef SONIA_UINT_MAXSIZE
#   define SONIA_UINT_MAXSIZE 20
#endif

namespace sonia::mpl {

struct uint_tag : boost::mpl::int_<10> {};

template <typename vec_t>
struct big_uint_
{
    using type = big_uint_;
    using sequence = vec_t;
    using tag = uint_tag;
};

/*
#define SONIA_UINT_REVERS_PARAMS(z, n, data) \
    BOOST_PP_CAT( \
        BOOST_PP_TUPLE_ELEM(2, 1, data), \
        BOOST_PP_DEC(BOOST_PP_SUB(BOOST_PP_TUPLE_ELEM(2, 0, data), n)) \
    )
*/

#define SONIA_UINT_REVERS_PARAMS(z, n, data)                                               \
BOOST_PP_IF(                                                                                \
    BOOST_PP_LESS(BOOST_PP_INC(BOOST_PP_ADD(n, n)), data),                                  \
    BOOST_PP_CAT(v, BOOST_PP_DEC(BOOST_PP_SUB(data, BOOST_PP_ADD(n, n))))                   \
        + (BOOST_PP_CAT(v, BOOST_PP_SUB(BOOST_PP_SUB(data, BOOST_PP_ADD(n, n)), 2)) << 16), \
    BOOST_PP_CAT(v, BOOST_PP_DEC(BOOST_PP_SUB(data, BOOST_PP_ADD(n, n))))                   \
)

#define SONIA_UINT_VERIFY_PARAMS(z, n, data) (data##n < 0x10000 && data##n >= 0) &&

#define SONIA_UINT_N(z, n, unused)                                                                                      \
template <BOOST_PP_ENUM_PARAMS(n, uint_fast32_t v)>                                                               \
struct big_uint_hex##n##_ : big_uint_<BOOST_PP_CAT(BOOST_PP_CAT(boost::mpl::list, BOOST_PP_DIV(BOOST_PP_INC(n), 2)), _c) \
    <uint_fast32_t, BOOST_PP_ENUM(BOOST_PP_DIV(BOOST_PP_INC(n), 2), SONIA_UINT_REVERS_PARAMS, n)> >              \
{                                                                                                                        \
    BOOST_MPL_ASSERT_MSG( (BOOST_PP_REPEAT(n, SONIA_UINT_VERIFY_PARAMS, v) true),                                       \
        THERE_IS_OUT_OF_RANGE_TEMPLATE_ARGUMENT, (big_uint_hex##n##_));                                                  \
};

template <BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(SONIA_UINT_MAXSIZE, uint_fast32_t v, 0xFFFFFFFF)>
struct big_uint_hex_;

#define SONIA_UINT_(z, n, unused)                                                                      \
template <BOOST_PP_ENUM_PARAMS(n, uint_fast32_t v)>                                              \
struct big_uint_hex_<BOOST_PP_ENUM_PARAMS(n, v)> : big_uint_hex##n##_<BOOST_PP_ENUM_PARAMS(n, v)> {};

BOOST_PP_REPEAT_FROM_TO(1, SONIA_UINT_MAXSIZE, SONIA_UINT_N, ~)
BOOST_PP_REPEAT_FROM_TO(1, SONIA_UINT_MAXSIZE, SONIA_UINT_, ~)

}
