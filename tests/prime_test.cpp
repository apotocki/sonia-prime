//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <tuple>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "sonia/automatic.hpp"
#include "sonia/serialization/serialization.hpp"
#include "sonia/serialization/integral.hpp"

#include "sonia/logger/logger.hpp"
#include <mutex>

using namespace sonia;

#if 0
BOOST_AUTO_TEST_CASE (prime_test)
{
    std::vector<char> vec;
    encode<serialization::default_t, int>(10, std::back_inserter(vec));

    int val1;
    decode<serialization::default_t, int>(vec.begin(), val1);
    BOOST_CHECK_EQUAL(10, val1);

    automatic<int> val(in_place_decode<serialization::default_t>(vec.begin()));

    BOOST_CHECK_EQUAL(10, *val);

    std::tuple<char, int, int> t1(1, 2, 3); // explicit deduction guide is used in this case

    std::cout << sizeof(t1) << "\n";

    GLOBAL_LOG_FATAL() << "HELLO";

    std::mutex mx_;
    if (std::lock_guard<std::mutex> lk(mx_); vec.empty()) {
        vec.push_back('a');
    }
}

#endif

#include "sonia/reference_wrapper.hpp"
#include "sonia/utility/variadic.hpp"

//template <typename Op, typename Indexes, typename ... Args> struct tuple_builder_helper {};
//
//template <typename Op, std::size_t... I, typename ... ArgsT>
//struct tuple_builder_helper<Op, std::index_sequence<I...>, ArgsT...> {
//    typedef std::tuple<typename Op::template apply<variadic::type_at_t<I, ArgsT...>>::type ...> type;
//};
//
//template <typename Op, typename ... Args>
//struct tuple_builder : tuple_builder_helper<Op, std::make_index_sequence<sizeof...(Args)>, Args ...> {};
//
//template <typename Op, typename ... Args>
//using tuple_builder_t = typename tuple_builder<Op, Args...>::type;
//
//struct foo_op {
//    template <typename T> struct apply {
//        typedef int type;
//    };
//};
//
//template <typename T> struct foo_op1 {
//    typedef int type;
//};
//template <template <class> class Op, typename ... ArgsT>
//struct transform : 

#include "sonia/utility/bind.hpp"

void test_foo(std::string& a0, std::string const& a1, int a2, std::string& a3, std::string const& a4, std::string a5, std::string && a6, std::string * a7, std::string const* a8, std::vector<char> & a9, std::vector<char> a10)
{
    BOOST_CHECK_EQUAL(a0, "123");
    BOOST_CHECK_EQUAL(a1, "123");
    BOOST_CHECK(&a0 == &a1);
    BOOST_CHECK_EQUAL(a2, 1);
    BOOST_CHECK_EQUAL(a3, "123");
    BOOST_CHECK(&a3 == &a0);
    BOOST_CHECK_EQUAL(a4, "234");
    BOOST_CHECK_EQUAL(a5, "345");
    BOOST_CHECK_EQUAL(a6, "456");
    BOOST_CHECK(a7 == &a0);
    BOOST_CHECK(a8 == &a4);
    BOOST_CHECK(a9 == std::vector<char>({1, 2, 3}));
}

template <typename T>
void test_tuple(T && bind_tuple) {
    typedef T tuple_t;
    BOOST_STATIC_ASSERT(is_same_v<reference_wrapper<std::string>, std::tuple_element_t<0, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<reference_wrapper<const std::string>, std::tuple_element_t<1, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<rvalue_reference_wrapper<int>, std::tuple_element_t<2, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<reference_wrapper<std::string>, std::tuple_element_t<3, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<reference_wrapper<const std::string>, std::tuple_element_t<4, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<rvalue_reference_wrapper<std::string>, std::tuple_element_t<5, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<rvalue_reference_wrapper<std::string>, std::tuple_element_t<6, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<std::string*, std::tuple_element_t<7, tuple_t>>);
    BOOST_STATIC_ASSERT(is_same_v<std::string const*, std::tuple_element_t<8, tuple_t>>);
    BOOST_STATIC_ASSERT(std::is_placeholder_v<std::tuple_element_t<9, tuple_t>>);

    std::vector<char> vec({1, 2, 3});
    
    apply_placeholders(&test_foo, bind_tuple, vec, std::vector<char>({2, 3, 4}));
}

BOOST_AUTO_TEST_CASE (bind_test)
{
    std::string s0 = "123";
    const std::string s1 = "234";
    std::string s2 = "345";
    const reference_wrapper<std::string> refs0(s0);

    //foo(std::placeholders::_2);
    //make_bind_tuple(std::placeholders::_2);
    // warn! temporary value check:  std::string("456")
    test_tuple(make_bind_tuple(refs0, cref(s0), 1, s0, s1, std::move(s2), std::string("456"), &s0, &s1, std::placeholders::_1, std::placeholders::_2));

    //std::cout << typeid(bind_tuple).name() << "\n";
    //std::cout << typeid(std::tuple_element<0, decltype(bind_tuple)>::type).name() << "\n";
}

#include "sonia/utility/placement_new.hpp"
#include "sonia/utility/in_place_factory.hpp"

struct test_component {
    test_component(std::string& a0, std::string const& a1, int a2, std::string& a3, std::string const& a4, std::string a5, std::string && a6, std::string * a7, std::string const* a8)
    {
        BOOST_CHECK_EQUAL(a0, "123");
        BOOST_CHECK_EQUAL(a1, "123");
        BOOST_CHECK(&a0 == &a1);
        BOOST_CHECK_EQUAL(a2, 1);
        BOOST_CHECK_EQUAL(a3, "123");
        BOOST_CHECK(&a3 == &a0);
        BOOST_CHECK_EQUAL(a4, "234");
        BOOST_CHECK_EQUAL(a5, "345");
        BOOST_CHECK_EQUAL(a6, "456");
        BOOST_CHECK(a7 == &a0);
        BOOST_CHECK(a8 == &a4);
    }
};

BOOST_AUTO_TEST_CASE (in_place_factory_test)
{
    std::string s0 = "123";
    const std::string s1 = "234";
    std::string s2 = "345";
    const reference_wrapper<std::string> refs0(s0);

    test_component * place = (test_component*)new char[sizeof(test_component)];
    placement_new<test_component>(place, in_place_factory(refs0, cref(s0), 1, s0, s1, std::move(s2), std::string("456"), &s0, &s1));
    place->~test_component();
    delete[] place;
}

/*
static get_exec_context()
exec_hierarchy_context
    open() // called by uninitialized thread
    close()
    on_close
hierarchy context for on_close and etc.

*/