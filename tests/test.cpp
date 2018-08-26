#if 1
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

#if 1
#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>
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

BOOST_AUTO_TEST_CASE (test)
{
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