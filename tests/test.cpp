#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#if 0
size_t operator "" _a0(const char* str, size_t v) {
    return v;
}

void do_nothing(size_t) {}

void foo_ok(int arg) {
    size_t r = "string"_a0; // OK
    do_nothing("string"_a0); // OK
}

template <typename T>
void foo_fail() {
    size_t r = "string"_a0; // OK
    do_nothing("string"_a0); // error
}

int main() { return 0; }
#endif

#if 0
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

//template <typename HolderT, size_t RightServBits>
//class integer

template <class DT> struct b0 {
    b0(int v = 0) : ival(v) { std::cout << "init b0\n"; }
    b0(b0 const& v) { ival = v.ival + 1; }
    int ival;
};

template <class DT> struct b1 {
    b1(unsigned int v = 0) : uival(v) { std::cout << "init b1\n"; }
    unsigned int uival;
};

template <class DT> struct b2 {
    b2() { std::cout << "init b2\n"; }
    float fval;
};

template <class DT> struct b3 {
    b3() { std::cout << "init b3\n"; }
    signed int sival;
};

/*
struct d0 : public b0<d0>
{
    union {
        void * ptrl;
        int ival;
    };
};

struct d1 
    : private b0<d1>
    , private b1<d1>
{
    union {
        void * ptrl;
        int ival;
    };
};

struct d2
    : public b0<d2>
    , public b1<d2>
    , public b2<d2>
{
    union {
        void * ptrl;
        int ival;
    };
};

struct d3
    : public b0<d3>
    , public b1<d3>
    , public b2<d3>
    , public b3<d3>
{
    union {
        void * ptrl;
        int ival;
    };
};
*/

union d0 {
    //std::string str;
    b0<d0> b0val_;
    b1<d0> b1val_;
    b2<d0> b2val_;
    b3<d0> b3val_;

    struct {
        intptr_t filler : sizeof(intptr_t) - 4;
        intptr_t type : 3;
        intptr_t isptr : 1;
    };
    //U() {}
};

#endif

#if 0

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

#endif

#if 0
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
#endif

//template <typename T> struct tag;

//template <int N>
//using char(&ArrT)[N];

template <char ... chars>
struct compose_tag {
    template <char c> using append = compose_tag<chars..., c>;
};

template< unsigned N > constexpr
char nth_char( const char (&arr) [N], unsigned i )
{
    return arr[i];
}

//constexpr auto tag(const char (&arr) [1]) {
//    return compose_tag<nth_char(arr, 0)>();
//}

//template <char(*F)(int)>
//struct resolver {
//
//};

#if 0
template <int N, int RN > constexpr
auto tag(const char (&arr) [RN])
{
    auto f = [arr](int n) { return arr[n]; };
    //auto f = [](int n) { return 'a'; };
    constexpr char(*inc)(int) = f;
    return resolver <inc> ();
    /*
    if constexpr (N == 1) {
        return compose_tag<nth_char(arr, 0)>();
    } else {
        typedef decltype(tag<RN - 1>(arr)) prev_t;
        typedef prev_t::template append<nth_char(arr, N)> next_t;
        return next_t();
    }
    */
}
#endif

#if 0
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

#endif

#include <iostream>
//#include <typeindex>
//
//namespace std {
//template <typename CharT, class TraitsT>
//std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, std::type_index const& val) {
//    return os << ((std::type_info)val).name();
//}
//}

#if 0

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
