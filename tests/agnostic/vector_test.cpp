// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>

#define DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#define DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF
#define DO_NOT_USE_AGNOSTIC_ENABLE_IF
#define DO_NOT_USE_AGNOSTIC_PLACEMENT_NEW
#define DO_NOT_USE_AGNOSTIC_DESTROY_AT
#define DO_NOT_USE_AGNOSTIC_FORWARD
#define DO_NOT_USE_AGNOSTIC_CONDITIONAL
#define DO_NOT_USE_AGNOSTIC_TYPE_IDENTITY
#define DO_NOT_USE_AGNOSTIC_IS_ARRAY
#define DO_NOT_USE_AGNOSTIC_IS_POINTER
#define DO_NOT_USE_AGNOSTIC_IS_SAME
#define DO_NOT_USE_AGNOSTIC_ADDRESSOF
#define DO_NOT_USE_AGNOSTIC_POINTER_TRAITS
#define DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_REMOVE_CV
#define DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TAGS
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR
#define DO_NOT_USE_AGNOSTIC_ALLOCATOR
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIAL
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_COPYABLE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_DESTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_DEFAULT_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_ASSIGNABLE
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_COPY
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_COPY
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_DEFAULT_CONSTRUCT
#define DO_NOT_USE_AGNOSTIC_ITERATOR_BEGIN
#define DO_NOT_USE_AGNOSTIC_ITERATOR_END
#define DO_NOT_USE_AGNOSTIC_ADD_RVALUE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_CONVERTIBLE_TO
#define DO_NOT_USE_AGNOSTIC_SAME_AS
#define DO_NOT_USE_AGNOSTIC_NOTHROW
#define DO_NOT_USE_AGNOSTIC_IN_PLACE
#define DO_NOT_USE_AGNOSTIC_FLOAT_ROUND_STYLE
#define DO_NOT_USE_AGNOSTIC_FLOAT_DENORM_STYLE
#define DO_NOT_USE_AGNOSTIC_NUMERIC_LIMITS

#include <compare>
#include <functional>

#include "agnostic/allocator/BareAllocator.hpp"
#include "agnostic/allocator/default_bare_allocator.hpp"
#include "agnostic/ipo/vector.hpp"


#include <set>
#include <vector>
#include <memory>

//#include <span>
namespace {

struct test_element0
{
    alignas(16) char dummy[16];
};

class Value
{
public:
    Value() : value_{'Z'} { ++count_; }
    Value(char v) : value_(v) { ++count_; }
    Value(Value const& rhs) : value_(rhs.value_) { ++count_; }
    Value(Value&& rhs) noexcept : value_(rhs.value_) { ++count_; }

    ~Value() { --count_; }

    void operator= (Value const& rhs) { value_ = rhs.value_; }
    void operator= (Value&& rhs) noexcept { value_ = rhs.value_; }

    char operator*() const { return value_; }

    static int count_;

    //std::strong_ordering operator<=>(const Value& rhs) const = default;

    auto operator<=>(const Value& rhs) const
    {
        return value_ <=> rhs.value_;
    }

    bool operator ==(const Value& rhs) const { return value_ == rhs.value_; }

private:
    char value_;
    char filler[63];
};

int Value::count_ = 0;

}

BOOST_AUTO_TEST_CASE(agnostic_vector_test)
{
    using vec0_helper_t = agnostic::vector_detail::helper<int, agnostic::in_place_capacity<0>>;
    static_assert(vec0_helper_t::begin_offs == 1);
    static_assert(vec0_helper_t::aligned_offs == 1);
    static_assert(vec0_helper_t::holder_sz == sizeof(uintptr_t));

    using vec01_helper_t = agnostic::vector_detail::helper<int, agnostic::in_place_capacity<1>>;
    static_assert(vec01_helper_t::begin_offs == 1);
    static_assert(vec01_helper_t::aligned_offs == sizeof(int));
    static_assert(vec01_helper_t::holder_sz == sizeof(uintptr_t));

    using vec02_helper_t = agnostic::vector_detail::helper<int, agnostic::in_place_capacity<2>>;
    static_assert(vec02_helper_t::begin_offs == 1);
    static_assert(vec02_helper_t::aligned_offs == sizeof(int));
    static_assert(vec02_helper_t::holder_sz == sizeof(uintptr_t) + sizeof(int));

    using vec1_helper_t = agnostic::vector_detail::helper<test_element0, agnostic::in_place_capacity<0>>;
    static_assert(vec1_helper_t::begin_offs == 1);
    static_assert(vec1_helper_t::aligned_offs == 1);
    static_assert(vec1_helper_t::holder_sz == sizeof(uintptr_t));

    using vec11_helper_t = agnostic::vector_detail::helper<test_element0, agnostic::in_place_capacity<1>>;
    static_assert(vec11_helper_t::begin_offs == 1);
    static_assert(vec11_helper_t::aligned_offs == 16);
    static_assert(vec11_helper_t::holder_sz == 32);

    using vec2_helper_t = agnostic::vector_detail::helper<char, agnostic::in_place_capacity<127>>;
    static_assert(vec2_helper_t::begin_offs == 1);
    static_assert(vec2_helper_t::aligned_offs == 1);
    static_assert(vec2_helper_t::holder_sz == 128);

    using vec21_helper_t = agnostic::vector_detail::helper<char, agnostic::in_place_capacity<128>>;
    static_assert(vec21_helper_t::begin_offs == 2);
    static_assert(vec21_helper_t::aligned_offs == 2);
    static_assert(vec21_helper_t::holder_sz == 130);

    //std::vector<int, agnostic::stl_allocator_adapter<int, agnostic::default_bare_allocator>> vec(ta);
    //std::vector<char, agnostic::default_bare_allocator> vec(ta);
    {
        agnostic::vector<int> vec;
        vec.push_back(1);
        BOOST_ASSERT(vec.size() == 1);
        BOOST_ASSERT(vec.front() == 1);
        BOOST_ASSERT(vec.back() == 1);

        size_t cnt = vec.capacity() - vec.size() + 1;
        for (int i = 0; i < cnt; ++i) {
            vec.push_back(i + 2);
            BOOST_ASSERT(vec.size() == size_t(2) + i);
            BOOST_ASSERT(vec.front() == 1);
            BOOST_ASSERT(vec.back() == i + 2);
        }
    }
    {
        // reallocation_test
        using namespace agnostic;
        agnostic::vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec1;
        BOOST_CHECK_EQUAL(vec0.capacity(), 3);

        for (size_t i = 0; i < 4; ++i) {
            vec0.push_back((char)('a' + i));
        }
        size_t newcap = vec0.capacity();
        BOOST_CHECK(newcap > 3);
        for (size_t i = vec0.size(); i < newcap + 1; ++i) {
            vec0.push_back('x');
        }
        size_t newcap2 = vec0.capacity();
        BOOST_CHECK(newcap2 > newcap);
        vec0 = std::move(vec1);
        BOOST_CHECK_EQUAL(vec0.capacity(), 3);
        BOOST_CHECK_EQUAL(Value::count_, 0);
    }
    {
        // reserve test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec1, vec2;
        vec0.reserve(10);

        for (size_t i = 0; i < 11; ++i) {
            vec0.push_back((char)('a' + i));
        }
        for (size_t i = 0; i < 11; ++i) {
            BOOST_CHECK_EQUAL(*vec0[i], (char)('a' + i));
        }

        vec1.push_back('a');
        vec1.push_back('b');
        vec1.reserve(10);
        for (size_t i = 2; i < 11; ++i) {
            vec1.push_back((char)('a' + i));
        }
        for (size_t i = 0; i < 11; ++i) {
            BOOST_CHECK_EQUAL(*vec1[i], (char)('a' + i));
        }

        vec2.push_back('a');
        vec2.push_back('b');
        vec2.push_back('c');
        vec2.push_back('d');
        vec2.reserve(10);
        for (size_t i = 4; i < 11; ++i) {
            vec2.push_back((char)('a' + i));
        }
        for (size_t i = 0; i < 11; ++i) {
            BOOST_CHECK_EQUAL(*vec2[i], (char)('a' + i));
        }
    }
    BOOST_CHECK_EQUAL(Value::count_, 0);
    {
        // copy test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec2, vec3, vec4, vec5, vec6, vec7;
        vec0.push_back('a');
        vec0.push_back('b');
        BOOST_CHECK(vec0.is_inplace());

        vector<Value, default_bare_allocator, in_place_capacity<3>> vec1(vec0);
        BOOST_CHECK(vec1.is_inplace());
        BOOST_CHECK(vec1 == vec0);
        
        BOOST_CHECK_EQUAL(vec0.size(), 2);
        BOOST_CHECK_EQUAL(vec1.size(), 2);
        BOOST_CHECK_EQUAL(*vec1[0], 'a');
        BOOST_CHECK_EQUAL(*vec1[1], 'b');

        vec1 = vec1;

        vec2.push_back('A');
        BOOST_CHECK(vec2 != vec0);
        BOOST_CHECK(vec2 < vec0); // 'A' < 'a'
        vec2 = vec1;
        BOOST_CHECK(vec2.is_inplace());
        BOOST_CHECK_EQUAL(vec2.size(), 2);
        BOOST_CHECK_EQUAL(*vec2[0], 'a');
        BOOST_CHECK_EQUAL(*vec2[1], 'b');

        vec3.push_back('A');
        vec3.push_back('B');
        vec3 = vec1;
        BOOST_CHECK(vec3.is_inplace());
        BOOST_CHECK_EQUAL(vec3.size(), 2);
        BOOST_CHECK_EQUAL(*vec3[0], 'a');
        BOOST_CHECK_EQUAL(*vec3[1], 'b');

        vec4.push_back('A');
        vec4.push_back('B');
        vec4.push_back('C');
        vec4 = vec1;
        BOOST_CHECK(vec4.is_inplace());
        BOOST_CHECK_EQUAL(vec4.size(), 2);
        BOOST_CHECK_EQUAL(*vec4[0], 'a');
        BOOST_CHECK_EQUAL(*vec4[1], 'b');

        vec5.push_back('A');
        vec5.push_back('B');
        vec5.push_back('C');
        vec5.push_back('D');

        vec2 = vec5;
        BOOST_CHECK(!vec2.is_inplace());
        BOOST_CHECK_EQUAL(vec2.size(), 4);
        BOOST_CHECK_EQUAL(*vec2[0], 'A');
        BOOST_CHECK_EQUAL(*vec2[1], 'B');
        BOOST_CHECK_EQUAL(*vec2[2], 'C');
        BOOST_CHECK_EQUAL(*vec2[3], 'D');
        vec2 = vec1;

        vec5 = vec1;

        //BOOST_CHECK(!vec5.is_inplace());
        BOOST_CHECK_EQUAL(vec5.size(), 2);
        BOOST_CHECK_EQUAL(*vec5[0], 'a');
        BOOST_CHECK_EQUAL(*vec5[1], 'b');

        vec6.push_back('A');
        vec6.push_back('B');
        vec6.push_back('C');
        vec6.push_back('D');
        vec6.pop_back();
        vec6.pop_back();
        vec6.pop_back();
        vec6.pop_back();
        vec6 = vec1;
        //BOOST_CHECK(!vec6.is_inplace());
        BOOST_CHECK_EQUAL(vec6.size(), 2);
        BOOST_CHECK_EQUAL(*vec6[0], 'a');
        BOOST_CHECK_EQUAL(*vec6[1], 'b');

        vec7.push_back('A');
        vec7.push_back('B');
        vec7.push_back('C');
        vec7.push_back('D');
        vec7.pop_back();
        vec7.pop_back();
        vec7.pop_back();
        vec7 = vec1;
        //BOOST_CHECK(!vec7.is_inplace());
        BOOST_CHECK_EQUAL(vec6.size(), 2);
        BOOST_CHECK_EQUAL(*vec7[0], 'a');
        BOOST_CHECK_EQUAL(*vec7[1], 'b');
    }
    BOOST_CHECK_EQUAL(Value::count_, 0);
    {
        // additional copy test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec2, vec3;
        vec0.push_back('a');
        vec0.push_back('b');
        vec0.push_back('c');
        vec0.push_back('d');
        BOOST_CHECK(!vec0.is_inplace());

        vector<Value, default_bare_allocator, in_place_capacity<3>> vec1(vec0);
        BOOST_CHECK_EQUAL(vec0.size(), 4);
        BOOST_CHECK_EQUAL(vec1.size(), 4);
        BOOST_CHECK_EQUAL(*vec1[0], 'a');
        BOOST_CHECK_EQUAL(*vec1[1], 'b');
        BOOST_CHECK_EQUAL(*vec1[2], 'c');
        BOOST_CHECK_EQUAL(*vec1[3], 'd');

        vec2.push_back('A');
        vec2 = vec1;
        BOOST_CHECK_EQUAL(vec2.size(), 4);
        BOOST_CHECK_EQUAL(*vec2[0], 'a');
        BOOST_CHECK_EQUAL(*vec2[1], 'b');
        BOOST_CHECK_EQUAL(*vec2[2], 'c');
        BOOST_CHECK_EQUAL(*vec2[3], 'd');

        vec3.resize(16, 'Z');
        vec3 = vec1;
        BOOST_CHECK_EQUAL(vec3.size(), 4);
        BOOST_CHECK_EQUAL(*vec3[0], 'a');
        BOOST_CHECK_EQUAL(*vec3[1], 'b');
        BOOST_CHECK_EQUAL(*vec3[2], 'c');
        BOOST_CHECK_EQUAL(*vec3[3], 'd');
    }
    BOOST_CHECK_EQUAL(Value::count_, 0);
    {
        // additional copy test
        using namespace agnostic;
        vector<char, default_bare_allocator, in_place_capacity<3>> vec0, vec1;
        vec0.push_back('a');
        vec0.push_back('b');
        vec0.push_back('c');
        vec0.push_back('d');

        vec1.push_back('A');
        vec1.push_back('B');
        BOOST_CHECK(vec1.is_inplace());
        vec1 = vec0;
        BOOST_CHECK(!vec1.is_inplace());
        BOOST_CHECK_EQUAL(vec1.size(), 4);
        BOOST_CHECK_EQUAL(vec1[0], 'a');
        BOOST_CHECK_EQUAL(vec1[1], 'b');
        BOOST_CHECK_EQUAL(vec1[2], 'c');
        BOOST_CHECK_EQUAL(vec1[3], 'd');
    }
    {
        // move test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec1, vec2, vec3, vec4, vec5, vec6, vec7;
        vec0.push_back('a');
        vec0.push_back('b');
        BOOST_CHECK_EQUAL(vec0.size(), 2);

        vec1.push_back('A');
        vec1 = std::move(vec0);

        BOOST_CHECK_EQUAL(vec1.size(), 2);
        BOOST_CHECK_EQUAL(*vec1[0], 'a');
        BOOST_CHECK_EQUAL(*vec1[1], 'b');
        vec1.pop_back();

        vec2.push_back('A');
        vec2.push_back('B');
        vec2 = std::move(vec1);

        BOOST_CHECK_EQUAL(vec2.size(), 1);
        BOOST_CHECK_EQUAL(*vec2[0], 'a');

        vec2.push_back('b');
        BOOST_CHECK_EQUAL(*vec2[1], 'b');

        vec3.push_back('A');
        vec3.push_back('B');
        vec3.push_back('C');
        vec3 = std::move(vec2);

        BOOST_CHECK_EQUAL(vec3.size(), 2);
        BOOST_CHECK_EQUAL(*vec3[0], 'a');
        BOOST_CHECK_EQUAL(*vec3[1], 'b');

        vec4.push_back('A');
        vec4.push_back('B');
        vec4.push_back('C');
        vec4.push_back('D');
        vec4 = std::move(vec3);

        BOOST_CHECK_EQUAL(vec4.size(), 2);
        BOOST_CHECK_EQUAL(*vec4[0], 'a');
        BOOST_CHECK_EQUAL(*vec4[1], 'b');

        vec5.push_back('A');
        vec5.push_back('B');
        vec5.push_back('C');
        vec5.push_back('D');
        vec5.pop_back();
        vec5.pop_back();
        vec5.pop_back();
        vec5.pop_back();
        BOOST_CHECK(vec5.empty());
        vec5 = std::move(vec4);

        BOOST_CHECK_EQUAL(vec5.size(), 2);
        BOOST_CHECK_EQUAL(*vec5[0], 'a');
        BOOST_CHECK_EQUAL(*vec5[1], 'b');

        vec6.push_back('A');
        vec6.push_back('B');
        vec6.push_back('C');
        vec6.push_back('D');
        vec6.pop_back();
        vec6.pop_back();
        vec6.pop_back();
        BOOST_CHECK(vec6.size() == 1);
        vec6 = std::move(vec5);

        BOOST_CHECK_EQUAL(vec6.size(), 2);
        BOOST_CHECK_EQUAL(*vec6[0], 'a');
        BOOST_CHECK_EQUAL(*vec6[1], 'b');

        vec7 = std::move(vec6);

        BOOST_CHECK_EQUAL(vec7.size(), 2);
        BOOST_CHECK_EQUAL(*vec7[0], 'a');
        BOOST_CHECK_EQUAL(*vec7[1], 'b');

        vector<Value, default_bare_allocator, in_place_capacity<3>> vec8(std::move(vec7));

        BOOST_CHECK_EQUAL(vec8.size(), 2);
        BOOST_CHECK_EQUAL(*vec8[0], 'a');
        BOOST_CHECK_EQUAL(*vec8[1], 'b');
    }

    BOOST_CHECK_EQUAL(Value::count_, 0);
    {
        // additional move test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0, vec1, vec2;

        vec0.push_back('a');
        vec0.push_back('b');
        vec0.push_back('c');
        BOOST_CHECK_EQUAL(vec0.size(), 3);

        vec1.push_back('A');
        vec1.push_back('B');
        vec1 = std::move(vec0);
        BOOST_CHECK(vec0.empty());
        BOOST_CHECK_EQUAL(vec1.size(), 3);
        BOOST_CHECK_EQUAL(*vec1[0], 'a');
        BOOST_CHECK_EQUAL(*vec1[1], 'b');
        BOOST_CHECK_EQUAL(*vec1[2], 'c');

        vec2.push_back('A');
        vec2.push_back('B');
        vec2.push_back('C');
        vec2 = std::move(vec1);
        BOOST_CHECK(vec1.empty());
        BOOST_CHECK_EQUAL(vec2.size(), 3);
        BOOST_CHECK_EQUAL(*vec2[0], 'a');
        BOOST_CHECK_EQUAL(*vec2[1], 'b');
        BOOST_CHECK_EQUAL(*vec2[2], 'c');

        vector<Value, default_bare_allocator, in_place_capacity<2>> vec3(std::move(vec2));
        BOOST_CHECK(vec2.empty());
        BOOST_CHECK_EQUAL(vec3.size(), 3);
        BOOST_CHECK_EQUAL(*vec3[0], 'a');
        BOOST_CHECK_EQUAL(*vec3[1], 'b');
        BOOST_CHECK_EQUAL(*vec3[2], 'c');
    }
    BOOST_CHECK_EQUAL(Value::count_, 0);
    {
        // reverse test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0;
        BOOST_CHECK(vec0.rbegin() == vec0.crend());

        vec0.push_back(1);
        BOOST_CHECK(vec0.crbegin() != vec0.rend());
        BOOST_CHECK(*vec0.crbegin() == vec0[0]);
        auto it = vec0.crbegin(); ++it;
        BOOST_CHECK(it == vec0.crend());
        it = vec0.crend(); --it;
        BOOST_CHECK(vec0.crbegin() == it);

        vec0.push_back(2);
        it = vec0.crbegin();
        BOOST_CHECK(*it++ == vec0[1]);
        BOOST_CHECK(*it++ == vec0[0]);
        BOOST_CHECK(it == vec0.crend());

        it = vec0.rend(); it--;
        BOOST_CHECK(*it-- == vec0[0]);
        BOOST_CHECK(*it == vec0[1]);
        BOOST_CHECK(vec0.rbegin() == it);

        vec0.push_back(3);
        it = vec0.crbegin();
        BOOST_CHECK(*it++ == 3);
        BOOST_CHECK(*it++ == 2);
        BOOST_CHECK(*it++ == 1);
        BOOST_CHECK(it == vec0.crend());

        it = vec0.rend(); --it;
        BOOST_CHECK(*it-- == 1);
        BOOST_CHECK(*it-- == 2);
        BOOST_CHECK(*it == 3);
        BOOST_CHECK(vec0.rbegin() == it);
    }
    BOOST_CHECK_EQUAL(Value::count_, 0);
    {
        // aux test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0{'a', 'B'};
        BOOST_CHECK(vec0.size() == 2);
        BOOST_CHECK(vec0[0] == 'a');
        BOOST_CHECK(vec0[1] == 'B');
        vec0.clear();
        BOOST_CHECK(vec0.empty());
        BOOST_CHECK_EQUAL(Value::count_, 0);

        vector<Value, default_bare_allocator, in_place_capacity<2>> vec1{ 'a', 'B', 'c' };
        BOOST_CHECK(vec1.size() == 3);
        BOOST_CHECK(vec1[0] == 'a');
        BOOST_CHECK(vec1[1] == 'B');
        BOOST_CHECK(vec1[2] == 'c');
        vec1.clear();
        BOOST_CHECK(vec1.empty());
        BOOST_CHECK_EQUAL(Value::count_, 0);
    }
    {
        std::vector<Value> v{ 'a', 'B', 'c' };
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0(v.begin(), v.end());
        BOOST_CHECK(!vec0.is_inplace());
        BOOST_CHECK(vec0.size() == 3);
        BOOST_CHECK(vec0[0] == 'a');
        BOOST_CHECK(vec0[1] == 'B');
        BOOST_CHECK(vec0[2] == 'c');

        std::list<Value> l{ 'a', 'B', 'c', 'D' };
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec1(l.begin(), l.end());
        BOOST_CHECK(!vec1.is_inplace());
        BOOST_CHECK(vec1.size() == 4);
        BOOST_CHECK(vec1[0] == 'a');
        BOOST_CHECK(vec1[1] == 'B');
        BOOST_CHECK(vec1[2] == 'c');
        BOOST_CHECK(vec1[3] == 'D');

        auto errb = vec1.cbegin() + 1;
        auto erre = vec1.cend() - 1;
        auto it = vec1.erase(errb, erre);
        BOOST_CHECK(*it == 'D');
        BOOST_CHECK(vec1.size() == 2);
        BOOST_CHECK(vec1[0] == 'a');
        BOOST_CHECK(vec1[1] == 'D');
        it = vec1.erase(vec1.cbegin());
        BOOST_CHECK(*it == 'D');
        BOOST_CHECK(vec1.size() == 1);
        BOOST_CHECK(vec1[0] == 'D');

        it = vec1.insert(vec1.begin(), v.begin(), v.end());
        BOOST_CHECK(*it == 'a');
        BOOST_CHECK(vec1.size() == 4);
        BOOST_CHECK(vec1[0] == 'a');
        BOOST_CHECK(vec1[1] == 'B');
        BOOST_CHECK(vec1[2] == 'c');
        BOOST_CHECK(vec1[3] == 'D');
        it = vec1.insert(vec1.begin() + 2, v.begin(), v.end());
        BOOST_CHECK(*it == 'a');
        BOOST_CHECK(vec1.size() == 7);
        BOOST_CHECK(vec1[0] == 'a');
        BOOST_CHECK(vec1[1] == 'B');
        BOOST_CHECK(vec1[2] == 'a');
        BOOST_CHECK(vec1[3] == 'B');
        BOOST_CHECK(vec1[4] == 'c');
        BOOST_CHECK(vec1[5] == 'c');
        BOOST_CHECK(vec1[6] == 'D');
        it = vec1.insert(vec1.end(), v.begin(), v.end());
        BOOST_CHECK(*it == 'a');
        BOOST_CHECK(vec1.size() == 10);
        BOOST_CHECK(vec1[0] == 'a');
        BOOST_CHECK(vec1[1] == 'B');
        BOOST_CHECK(vec1[2] == 'a');
        BOOST_CHECK(vec1[3] == 'B');
        BOOST_CHECK(vec1[4] == 'c');
        BOOST_CHECK(vec1[5] == 'c');
        BOOST_CHECK(vec1[6] == 'D');
        BOOST_CHECK(vec1[7] == 'a');
        BOOST_CHECK(vec1[8] == 'B');
        BOOST_CHECK(vec1[9] == 'c');

        vec1.resize(5);
        BOOST_CHECK(vec1.size() == 5);
        
        it = vec1.insert(vec1.end() - 2, l.begin(), l.end());
        BOOST_CHECK(*it == 'a');
        BOOST_CHECK(vec1.size() == 9);
        BOOST_CHECK(vec1[0] == 'a');
        BOOST_CHECK(vec1[1] == 'B');
        BOOST_CHECK(vec1[2] == 'a');

        BOOST_CHECK(vec1[3] == 'a');
        BOOST_CHECK(vec1[4] == 'B');
        BOOST_CHECK(vec1[5] == 'c');
        BOOST_CHECK(vec1[6] == 'D');

        BOOST_CHECK(vec1[7] == 'B');
        BOOST_CHECK(vec1[8] == 'c');
    }
    BOOST_CHECK_EQUAL(Value::count_, 0);
    {
        // resize test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec1;
        vec1.resize(128);
        BOOST_CHECK(vec1.size() == 128);
        for (auto const& v : vec1) {
            BOOST_CHECK(v == 'Z');
        }
    }
    BOOST_CHECK_EQUAL(Value::count_, 0);

    //foo(agnostic::get_default_fixed_size_allocator<16>());
    //vv c0;
    //auto e = --c0.end();
    //std::set<int> v;
    //auto v1 = --v.end();

    //using vec0_t = agnostic::vector<int>;
    //std::cout << sizeof(vec0_t) << "\n";

    //vec0_t vec0;
    //BOOST_CHECK(vec0.size() == 0);
    //vec0.push_back(1);
#if 0
    //std::to_address
    //

    vec0.push_back(1);
    BOOST_CHECK(vec0.size() == 1);
    BOOST_CHECK_EQUAL(vec0[0], 1);
#endif
}
