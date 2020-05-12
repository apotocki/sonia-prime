#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#if 0
#if 0
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

#define _WIN32_WINNT 0x0501
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
#include <iostream>
#include <boost/asio.hpp>
#include "sonia/thread.hpp"

BOOST_AUTO_TEST_CASE (test)
{
    using namespace sonia;

    boost::asio::io_service serv;
    boost::asio::io_service::work w(serv);

    thread t([&serv]() {
        serv.run();
    });

    boost::asio::deadline_timer tm(serv);
    auto dt = boost::posix_time::seconds(5);
    tm.expires_from_now(dt);
    tm.async_wait([](const boost::system::error_code& error) {
        std::cout << "tick!\n";
    });
    std::string v;
    std::cin >> v;

    //typedef boost::multiprecision::number<boost::multiprecision::cpp_int_backend<64, 0>> integer_type;
    //std::cout << sizeof(integer_type) << "\n";
    //std::cout << 	__LITTLE_ENDIAN__ << "\n";
    //d0 d = {1};
    //std::cout << "sizeof(d0) = " << sizeof(d0) << "\n";
    //std::cout << "sizeof(d1) = " << sizeof(d1) << "\n";
    //std::cout << "sizeof(d2) = " << sizeof(d2) << "\n";
    //std::cout << "sizeof(d3) = " << sizeof(d3) << "\n";
}

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>

std::string get_fiber(std::string const& line) {
    std::string res;
    for (char c : line) {
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {
            res.push_back(c);
            if (res.size() == 16) return res;
        } else {
            res.clear();
        }
    }
    return "";
}

BOOST_AUTO_TEST_CASE (log_processor)
{
    std::string in_file_name = "in_log.txt";
    std::string out_file_name = "out_log.txt";

    std::ifstream infile(in_file_name.c_str());
    std::ofstream outfile(out_file_name.c_str());

    std::map<std::string, std::vector<std::string>> fiber_lines;

    std::string line;
    while (std::getline(infile, line)) {
        std::string fb = get_fiber(line);
        fiber_lines[fb].push_back(line);
    }

    for (auto& pair : fiber_lines) {
        outfile << "{{{{{{{{ " << pair.first << "\n";
        for (std::string const& l : pair.second) {
            outfile << l << "\n";
        }
        outfile << "}}}}}}}\n\n\n\n";
    }
}

template <char ... chars> struct constexpr_string {};

template <size_t SizeV> using static_string = std::array<const char, SizeV>;
template <size_t SizeV, size_t ... I>
constexpr static_string<sizeof ... (I) + 1> make_static_string(const char (&str)[SizeV], std::index_sequence<I ...>) {
    return {str[I] ..., '\0'};
}
template <size_t SizeV>
constexpr static_string<SizeV> make_static_string(const char (&str)[SizeV]) {
    return make_static_string(str, std::make_index_sequence<SizeV - 1>());
}

template <size_t SizeV, size_t ... I>
constexpr auto make_constexpr_string(static_string<SizeV> const ss, std::index_sequence<I ...>) {
    return constexpr_string<ss[I] ...>();
}

template <size_t SizeV>
constexpr auto make_constexpr_string(static_string<SizeV> const ss) {
    return make_constexpr_string(ss, std::make_index_sequence<SizeV>());
}

#define declare_tag3(lit) \
using lit##_tag = decltype(make_constexpr_string(make_static_string(#lit)));

template <int N>
constexpr auto tag(const char(&arr)[1])
{
    return compose_tag<arr[0]>();
    //return compose_tag<nth_char(arr, 0)>();
}

template <class ParticularStringT, int sz, char ... chars>
struct builder {
    using result = typename builder < ParticularStringT, sz - 1, ParticularStringT{}.str[sz - 1], chars ... > ::result;
};

template <class ParticularStringT, char ... chars>
struct builder<ParticularStringT, 0, chars ...> {
    using result = constexpr_string <chars ... > ;
};

template <class F>
auto runner(F const& f) { return f(); }

#define declare_tag(lit) \
auto lit##_tag_instance = [](){ \
    struct constexpr_str { const char * str = #lit; }; \
    return builder<constexpr_str, sizeof(#lit) - 1>::result (); \
}(); \
typedef decltype(lit##_tag_instance) lit##_tag;

#define declare_tag2(lit) \
struct lit##_tag_helper{ const char * str = #lit; }; \
using lit##_tag = builder<lit##_tag_helper, sizeof(#lit) - 1>::result ;

#define STRLIT(v) union { static constexpr char * value() { return #v; } }
#define declare_tag4(lit) using lit##_tag = decltype(builder<STRLIT(lit), sizeof(#lit) - 1>::result ())
//template <char ... chars>
//struct tag {
//
//};
//
//struct my_str_provider
//{
//    constexpr static char const* str() { return "hello world"; }
//};

template <class T>
struct D : T {

};

template <class T>
D<T> foo(T);

template <class T>
struct strbuilder {};

#define STRFUNC(v) []{ return #v;}
template <class T>
constexpr auto make_strbuilder(T) { return strbuilder<T>(); }

#include "sonia/sal.hpp"

#include <iostream>
BOOST_AUTO_TEST_CASE (test)
{
    uint64_t v = 255;
    auto f = [](uint64_t v) {
        int lv = sonia::sal::log2(v);
        return (sonia::sal::log2(v) + CHAR_BIT) / CHAR_BIT;
    };
    std::cout << f(1) << ", " << f(255) << ", " << f(256) << ", " << f(65535) << ", " << f(65536) << "\n";
    //typedef f<"asd"> tag_type;
    //declare_tag2(main);
    //declare_tag2(secondary);
    //std::cout << typeid(get_string(5)).name() << "\n";

    //using m = decltype(make_strbuilder(STRFUNC(forth)));
    //declare_tag4(third);
    //typedef builder<decltype(foo(union { static constexpr auto value() { return "third"; })), 5> ttt_t;
    //using third_tag = decltype(builder<struct third { const char * str = "third"; }, 5 > ::result());
    //constexpr auto arr0 = make_static_string("lll");
    //foo<4, arr0>();
    //make_constexpr_string(arr0);
    //declare_tag3(third);
    /*
    //typedef decltype(sonia_tag("main")) tag_type;
    std::cout << typeid(main_tag).name() << "\n";
    std::cout << typeid(secondary_tag).name() << "\n";
    //std::cout << typeid(third_tag).name() << "\n";
    //typedef tag<my_str_provider::str()> tag_main;

    constexpr auto arr0 = make_static_string("lll");
    make_constexpr_string(arr0, std::make_index_sequence<4>());
    typedef ppp<make_static_string("lll")[0]> ppp_t;
    */
}

#include "applied/scoped_services.hpp"
#include "sonia/utility/multimethod.hpp"
#include "sonia/utility/command.hpp"
#include "sonia/utility/bind_command.hpp"
#include "sonia/utility/serialization/command.hpp"

#include <boost/mpl/size.hpp>

using namespace sonia;


class plus_cmd : public sonia::command<int, int, int>{
public:
    int operator()(int x, int y) const override {
        return x + y;
    }

    std::type_index ti() const override { return typeid(plus_cmd); }
};

class foo_class {
public:
    int method0(int v, int v1, int v2) { return v + v1 - v2; }
};

namespace sonia { namespace serialization {

template <typename SerializationTagT>
class coder<SerializationTagT, plus_cmd>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(plus_cmd const& did, OutputIteratorT oi) const {
        return std::move(oi);
    }
};

}}


template <class ArgT>
auto foo(ArgT && f) {
    typedef typename boost::function_types::function_type<remove_reference_t<ArgT>>::type f_type;
    return function<f_type>(f);
}

BOOST_AUTO_TEST_CASE (multimethod_test)
{
    scoped_services ss;
    
    ///*
    struct my_method_name {};
    //register_multimethod(function<int&&(int, int*, int&, int&&)>());
    struct minus {};
    struct plus {};
    register_multimethod<plus>(function<int(int, int)>([](int a, int b) { return a + b; }));
    register_multimethod<minus>(function<int(int, int)>([](int a, int b) { return a - b; }));
    BOOST_CHECK_EQUAL(42, (get_multimethod<plus, int(int, int)>()->operator()(40, 2)));
    BOOST_CHECK_EQUAL(42, (get_multimethod<minus, int(int, int)>()->operator()(44, 2)));

    register_multimethod<plus, int>(function<int(int, int)>([](int a, int b) { return a + b + 1; }));
    BOOST_CHECK_EQUAL(43, (get_multimethod<plus, int(int, int), int>()->operator()(40, 2)));
    BOOST_CHECK_EQUAL(43, (get_multimethod<plus, int(int, int)>({typeidx(int)})->operator()(40, 2)));

#if 0
    plus_cmd cmd0;
    std::vector<char> result;
    serialization::command_proxy_coder<serialization::default_t>()
        .encode(cmd0, std::back_inserter(result));
    //*/
    foo_class c0;
    //using std::placeholders::_1;
    //using std::placeholders::_2;
    auto cmd = make_bind_command(&foo_class::method0, c0, 1, _2, _1);
    std::cout << cmd(1, 42) << "\n";
    //foo(&foo_class::method0);
#endif
}

#endif

#if 0
#include <iostream>
#include <fstream>
#include "sonia/exceptions.hpp"

using namespace sonia;

void parse_line(std::string const& line, std::array<std::string, 5> & res)
{
    size_t pos = 0;
    auto bit = line.begin();
    for (auto it = bit, eit = line.end(); it != eit; ++it) {
        if (*it == '\"') {
            for (++it; it != eit && *it != '\"'; ++it);
            if (it == eit) throw exception("unexpected eol %1%"_fmt % line);
            continue;
        }
        if (*it != ',') continue;
        res[pos++] =  std::string(bit, it);
        bit = it;
        ++bit;
        BOOST_ASSERT (pos < res.size());
    }
    res[pos++] = std::string(bit, line.end());
    BOOST_ASSERT (pos == res.size());
}

template <class VectorT>
void load_file(std::string const& name, VectorT & result)
{
    std::ifstream s{name.c_str()};
    if (s.fail()) throw exception("%1% is not found"_fmt % name);

    std::string line;
    std::getline(s, line);

    using row_type = typename VectorT::value_type;

    row_type row;
    while (std::getline(s, line)) {
        parse_line(line, row);
        result.push_back(std::move(row));
    }
}

std::string get_enum_name(std::string const& v)
{
    std::string result;
    for (char c : v) {
        if (c == '-') c = '_';
        c = (char)std::toupper(c);
        result.push_back(c);
    }
    return result;
}


#endif

#include "sonia/utility/iterators/file_region_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/utility/iterators/archive_extract_iterator.hpp"

BOOST_AUTO_TEST_CASE (tmp_test)
{
    using namespace sonia;

    std::string filename = "k:\\projects\\ai4bd\\data\\test0.ttl";

    int cnt = 0;
    archive_iterator ar_it =
        make_archive_extract_iterator(filename, file_region_iterator<const char>(filename, 0, 65536), 65536);

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

BOOST_AUTO_TEST_CASE(test)
{
    std::cout << "hello\n";
    assert(!(std::is_same_v<t1, int>));
    //assert((std::is_same<t5, f5<char,short,int,long,float>>::value));

    //assert((std::is_same<t1_my,  int>::value));
    //assert((std::is_same<t5_my,  f5 <char,short,int,long,float>>::value));
    //assert((std::is_same<t10_my, f10<char,short,int,long,float,char,short,int,long,float>>::value));
    //assert(!(std::is_same<t5_my, f10<char,short,int,long,float,char,short,int,long,float>>::value));
}