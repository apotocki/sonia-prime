#include "sonia/config.hpp"
#include <filesystem>

#include <boost/test/unit_test.hpp>

#if 0
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

#include "maat/parsers/maat/lexertl_lexer.hpp"
#include "maat/parsers/maat/model.hpp"
#include "maat/parsers/maat/parser.hpp"

#include "llvm/ADT/APFloat.h"

using namespace maat;
namespace fs = std::filesystem;

BOOST_AUTO_TEST_CASE (tmp_test)
{
    char const* path = std::getenv("MAAT_HOME");
    BOOST_REQUIRE_MESSAGE(path, "MAAT_HOME must be set");
    fs::path maat_home{ path };

    std::ifstream file((maat_home / "tests" / "data" / "test.maat").string().c_str(), std::ios::binary);
    std::string text(std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{});

    parsers::maat::model model;

    parsers::parse<
        parsers::xml::lexertl_lexer,
        parsers::xml::parser
    >(model, text.c_str(), text.c_str() + text.size());
}
    auto it = range_dereferencing_iterator(std::move(ar_it)), eit = range_dereferencing_iterator<archive_iterator>();
    auto it2 = it;
    for (; it != eit; ++it) {
        //std::cout << *it;
        std::cout << cnt++ << "\n";
        it2 = it;
    }
}
#endif

#include <type_traits>
#include <cassert>
#include <iostream>
#include <boost/mpl/quote.hpp>

namespace boost { namespace mpl {
template<template<typename... Args2>class F>
struct my_quoteN
{
    template<typename... Args>
    struct apply:quote_impl<F<Args...>,aux::has_type<F<Args...>>::value>{};
};
}}

template<typename T>
struct f1
{
    typedef T type;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
struct f5
{
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
struct f10
{
};

typedef boost::mpl::quote1<f1>::apply<int>::type t1;
typedef boost::mpl::quote5<f5>::apply<char,short,int,long,float>::type t5;

typedef boost::mpl::my_quoteN<f1>::apply<int>::type t1_my;
typedef boost::mpl::my_quoteN<f5>::apply<char,short,int,long,float>::type t5_my;
typedef boost::mpl::my_quoteN<f10>::apply<char,short,int,long,float,char,short,int,long,float>::type t10_my;

static_assert(std::is_same<t1, int>::value);
static_assert(std::is_same<t5, f5<char,short,int,long,float>>::value);

template <size_t N> struct get_type;
template <> struct get_type<0> { using type = int; };
template <> struct get_type<1> { using type = float; };
template <size_t N> using get_type_t = typename get_type<N>::type;

template <typename T, size_t N> struct xxx;
template <size_t N> struct xxx<get_type_t<N>, N> {};

//template <typename T1, size_t N1, typename T2, size_t N2> struct yyy;
//template <size_t N> struct yyy<get_type_t<N>, N> {};

template <size_t N>
void foo(get_type_t<N> r)
{

}

BOOST_AUTO_TEST_CASE(test)
{
    foo<1>(1.2f);
    xxx<int, 0> x0;
    xxx<float, 1> x1;
    std::cout << "hello\n";
    assert(!(std::is_same_v<t1, int>));
    //assert((std::is_same<t5, f5<char,short,int,long,float>>::value));

    //assert((std::is_same<t1_my,  int>::value));
    //assert((std::is_same<t5_my,  f5 <char,short,int,long,float>>::value));
    //assert((std::is_same<t10_my, f10<char,short,int,long,float,char,short,int,long,float>>::value));
    //assert(!(std::is_same<t5_my, f10<char,short,int,long,float,char,short,int,long,float>>::value));
}
