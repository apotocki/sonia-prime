// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#include "agnostic/allocator/BareAllocator.hpp"
#include "agnostic/ipo/vector.hpp"

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

void agnostic_vector_test()
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

    {
        agnostic::vector<int> vec;
        vec.push_back(1);
        AGNOSTIC_CHECK(vec.size() == 1);
        AGNOSTIC_CHECK(vec.front() == 1);
        AGNOSTIC_CHECK(vec.back() == 1);

        size_t cnt = vec.capacity() - vec.size() + 1;
        for (int i = 0; i < cnt; ++i) {
            vec.push_back(i + 2);
            AGNOSTIC_CHECK(vec.size() == size_t(2) + i);
            AGNOSTIC_CHECK(vec.front() == 1);
            AGNOSTIC_CHECK(vec.back() == i + 2);
        }
    }
    {
        // reallocation_test
        using namespace agnostic;
        agnostic::vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec1;
        AGNOSTIC_CHECK_EQUAL(vec0.capacity(), 3);

        for (size_t i = 0; i < 4; ++i) {
            vec0.push_back((char)('a' + i));
        }
        size_t newcap = vec0.capacity();
        AGNOSTIC_CHECK(newcap > 3);
        for (size_t i = vec0.size(); i < newcap + 1; ++i) {
            vec0.push_back('x');
        }
        size_t newcap2 = vec0.capacity();
        AGNOSTIC_CHECK(newcap2 > newcap);
        vec0 = std::move(vec1);
        AGNOSTIC_CHECK_EQUAL(vec0.capacity(), 3);
        AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
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
            AGNOSTIC_CHECK_EQUAL(*vec0[i], (char)('a' + i));
        }

        vec1.push_back('a');
        vec1.push_back('b');
        vec1.reserve(10);
        for (size_t i = 2; i < 11; ++i) {
            vec1.push_back((char)('a' + i));
        }
        for (size_t i = 0; i < 11; ++i) {
            AGNOSTIC_CHECK_EQUAL(*vec1[i], (char)('a' + i));
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
            AGNOSTIC_CHECK_EQUAL(*vec2[i], (char)('a' + i));
        }
    }
    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
    {
        // copy test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec2, vec3, vec4, vec5, vec6, vec7;
        vec0.push_back('a');
        vec0.push_back('b');
        AGNOSTIC_CHECK(vec0.is_inplace());

        vector<Value, default_bare_allocator, in_place_capacity<3>> vec1(vec0);
        AGNOSTIC_CHECK(vec1.is_inplace());
        AGNOSTIC_CHECK(vec1 == vec0);
        
        AGNOSTIC_CHECK_EQUAL(vec0.size(), 2);
        AGNOSTIC_CHECK_EQUAL(vec1.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec1[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec1[1], 'b');

        vec1 = vec1;

        vec2.push_back('A');
        AGNOSTIC_CHECK(vec2 != vec0);
        AGNOSTIC_CHECK(vec2 < vec0); // 'A' < 'a'
        vec2 = vec1;
        AGNOSTIC_CHECK(vec2.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec2.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec2[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec2[1], 'b');

        vec3.push_back('A');
        vec3.push_back('B');
        vec3 = vec1;
        AGNOSTIC_CHECK(vec3.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec3.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec3[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec3[1], 'b');

        vec4.push_back('A');
        vec4.push_back('B');
        vec4.push_back('C');
        vec4 = vec1;
        AGNOSTIC_CHECK(vec4.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec4.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec4[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec4[1], 'b');

        vec5.push_back('A');
        vec5.push_back('B');
        vec5.push_back('C');
        vec5.push_back('D');

        vec2 = vec5;
        AGNOSTIC_CHECK(!vec2.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec2.size(), 4);
        AGNOSTIC_CHECK_EQUAL(*vec2[0], 'A');
        AGNOSTIC_CHECK_EQUAL(*vec2[1], 'B');
        AGNOSTIC_CHECK_EQUAL(*vec2[2], 'C');
        AGNOSTIC_CHECK_EQUAL(*vec2[3], 'D');
        vec2 = vec1;

        vec5 = vec1;

        //AGNOSTIC_CHECK(!vec5.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec5.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec5[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec5[1], 'b');

        vec6.push_back('A');
        vec6.push_back('B');
        vec6.push_back('C');
        vec6.push_back('D');
        vec6.pop_back();
        vec6.pop_back();
        vec6.pop_back();
        vec6.pop_back();
        vec6 = vec1;
        //AGNOSTIC_CHECK(!vec6.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec6.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec6[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec6[1], 'b');

        vec7.push_back('A');
        vec7.push_back('B');
        vec7.push_back('C');
        vec7.push_back('D');
        vec7.pop_back();
        vec7.pop_back();
        vec7.pop_back();
        vec7 = vec1;
        //AGNOSTIC_CHECK(!vec7.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec6.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec7[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec7[1], 'b');
    }
    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
    {
        // additional copy test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec2, vec3;
        vec0.push_back('a');
        vec0.push_back('b');
        vec0.push_back('c');
        vec0.push_back('d');
        AGNOSTIC_CHECK(!vec0.is_inplace());

        vector<Value, default_bare_allocator, in_place_capacity<3>> vec1(vec0);
        AGNOSTIC_CHECK_EQUAL(vec0.size(), 4);
        AGNOSTIC_CHECK_EQUAL(vec1.size(), 4);
        AGNOSTIC_CHECK_EQUAL(*vec1[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec1[1], 'b');
        AGNOSTIC_CHECK_EQUAL(*vec1[2], 'c');
        AGNOSTIC_CHECK_EQUAL(*vec1[3], 'd');

        vec2.push_back('A');
        vec2 = vec1;
        AGNOSTIC_CHECK_EQUAL(vec2.size(), 4);
        AGNOSTIC_CHECK_EQUAL(*vec2[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec2[1], 'b');
        AGNOSTIC_CHECK_EQUAL(*vec2[2], 'c');
        AGNOSTIC_CHECK_EQUAL(*vec2[3], 'd');

        vec3.resize(16, 'Z');
        vec3 = vec1;
        AGNOSTIC_CHECK_EQUAL(vec3.size(), 4);
        AGNOSTIC_CHECK_EQUAL(*vec3[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec3[1], 'b');
        AGNOSTIC_CHECK_EQUAL(*vec3[2], 'c');
        AGNOSTIC_CHECK_EQUAL(*vec3[3], 'd');
    }
    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
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
        AGNOSTIC_CHECK(vec1.is_inplace());
        vec1 = vec0;
        AGNOSTIC_CHECK(!vec1.is_inplace());
        AGNOSTIC_CHECK_EQUAL(vec1.size(), 4);
        AGNOSTIC_CHECK_EQUAL(vec1[0], 'a');
        AGNOSTIC_CHECK_EQUAL(vec1[1], 'b');
        AGNOSTIC_CHECK_EQUAL(vec1[2], 'c');
        AGNOSTIC_CHECK_EQUAL(vec1[3], 'd');
    }
    {
        // move test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec0, vec1, vec2, vec3, vec4, vec5, vec6, vec7;
        vec0.push_back('a');
        vec0.push_back('b');
        AGNOSTIC_CHECK_EQUAL(vec0.size(), 2);

        vec1.push_back('A');
        vec1 = std::move(vec0);

        AGNOSTIC_CHECK_EQUAL(vec1.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec1[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec1[1], 'b');
        vec1.pop_back();

        vec2.push_back('A');
        vec2.push_back('B');
        vec2 = std::move(vec1);

        AGNOSTIC_CHECK_EQUAL(vec2.size(), 1);
        AGNOSTIC_CHECK_EQUAL(*vec2[0], 'a');

        vec2.push_back('b');
        AGNOSTIC_CHECK_EQUAL(*vec2[1], 'b');

        vec3.push_back('A');
        vec3.push_back('B');
        vec3.push_back('C');
        vec3 = std::move(vec2);

        AGNOSTIC_CHECK_EQUAL(vec3.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec3[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec3[1], 'b');

        vec4.push_back('A');
        vec4.push_back('B');
        vec4.push_back('C');
        vec4.push_back('D');
        vec4 = std::move(vec3);

        AGNOSTIC_CHECK_EQUAL(vec4.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec4[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec4[1], 'b');

        vec5.push_back('A');
        vec5.push_back('B');
        vec5.push_back('C');
        vec5.push_back('D');
        vec5.pop_back();
        vec5.pop_back();
        vec5.pop_back();
        vec5.pop_back();
        AGNOSTIC_CHECK(vec5.empty());
        vec5 = std::move(vec4);

        AGNOSTIC_CHECK_EQUAL(vec5.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec5[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec5[1], 'b');

        vec6.push_back('A');
        vec6.push_back('B');
        vec6.push_back('C');
        vec6.push_back('D');
        vec6.pop_back();
        vec6.pop_back();
        vec6.pop_back();
        AGNOSTIC_CHECK(vec6.size() == 1);
        vec6 = std::move(vec5);

        AGNOSTIC_CHECK_EQUAL(vec6.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec6[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec6[1], 'b');

        vec7 = std::move(vec6);

        AGNOSTIC_CHECK_EQUAL(vec7.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec7[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec7[1], 'b');

        vector<Value, default_bare_allocator, in_place_capacity<3>> vec8(std::move(vec7));

        AGNOSTIC_CHECK_EQUAL(vec8.size(), 2);
        AGNOSTIC_CHECK_EQUAL(*vec8[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec8[1], 'b');
    }

    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
    {
        // additional move test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0, vec1, vec2;

        vec0.push_back('a');
        vec0.push_back('b');
        vec0.push_back('c');
        AGNOSTIC_CHECK_EQUAL(vec0.size(), 3);

        vec1.push_back('A');
        vec1.push_back('B');
        vec1 = std::move(vec0);
        AGNOSTIC_CHECK(vec0.empty());
        AGNOSTIC_CHECK_EQUAL(vec1.size(), 3);
        AGNOSTIC_CHECK_EQUAL(*vec1[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec1[1], 'b');
        AGNOSTIC_CHECK_EQUAL(*vec1[2], 'c');

        vec2.push_back('A');
        vec2.push_back('B');
        vec2.push_back('C');
        vec2 = std::move(vec1);
        AGNOSTIC_CHECK(vec1.empty());
        AGNOSTIC_CHECK_EQUAL(vec2.size(), 3);
        AGNOSTIC_CHECK_EQUAL(*vec2[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec2[1], 'b');
        AGNOSTIC_CHECK_EQUAL(*vec2[2], 'c');

        vector<Value, default_bare_allocator, in_place_capacity<2>> vec3(std::move(vec2));
        AGNOSTIC_CHECK(vec2.empty());
        AGNOSTIC_CHECK_EQUAL(vec3.size(), 3);
        AGNOSTIC_CHECK_EQUAL(*vec3[0], 'a');
        AGNOSTIC_CHECK_EQUAL(*vec3[1], 'b');
        AGNOSTIC_CHECK_EQUAL(*vec3[2], 'c');
    }
    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
    {
        // reverse test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0;
        AGNOSTIC_CHECK(vec0.rbegin() == vec0.crend());

        vec0.push_back(1);
        AGNOSTIC_CHECK(vec0.crbegin() != vec0.rend());
        AGNOSTIC_CHECK(*vec0.crbegin() == vec0[0]);
        auto it = vec0.crbegin(); ++it;
        AGNOSTIC_CHECK(it == vec0.crend());
        it = vec0.crend(); --it;
        AGNOSTIC_CHECK(vec0.crbegin() == it);

        vec0.push_back(2);
        it = vec0.crbegin();
        AGNOSTIC_CHECK(*it++ == vec0[1]);
        AGNOSTIC_CHECK(*it++ == vec0[0]);
        AGNOSTIC_CHECK(it == vec0.crend());

        it = vec0.rend(); it--;
        AGNOSTIC_CHECK(*it-- == vec0[0]);
        AGNOSTIC_CHECK(*it == vec0[1]);
        AGNOSTIC_CHECK(vec0.rbegin() == it);

        vec0.push_back(3);
        it = vec0.crbegin();
        AGNOSTIC_CHECK(*it++ == 3);
        AGNOSTIC_CHECK(*it++ == 2);
        AGNOSTIC_CHECK(*it++ == 1);
        AGNOSTIC_CHECK(it == vec0.crend());

        it = vec0.rend(); --it;
        AGNOSTIC_CHECK(*it-- == 1);
        AGNOSTIC_CHECK(*it-- == 2);
        AGNOSTIC_CHECK(*it == 3);
        AGNOSTIC_CHECK(vec0.rbegin() == it);
    }
    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
    {
        // aux test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0{'a', 'B'};
        AGNOSTIC_CHECK(vec0.size() == 2);
        AGNOSTIC_CHECK(vec0[0] == 'a');
        AGNOSTIC_CHECK(vec0[1] == 'B');
        vec0.clear();
        AGNOSTIC_CHECK(vec0.empty());
        AGNOSTIC_CHECK_EQUAL(Value::count_, 0);

        vector<Value, default_bare_allocator, in_place_capacity<2>> vec1{ 'a', 'B', 'c' };
        AGNOSTIC_CHECK(vec1.size() == 3);
        AGNOSTIC_CHECK(vec1[0] == 'a');
        AGNOSTIC_CHECK(vec1[1] == 'B');
        AGNOSTIC_CHECK(vec1[2] == 'c');
        vec1.clear();
        AGNOSTIC_CHECK(vec1.empty());
        AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
    }
    {
        std::array<Value, 3> v = { 'a', 'B', 'c' };
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec0(v.begin(), v.end());
        AGNOSTIC_CHECK(!vec0.is_inplace());
        AGNOSTIC_CHECK(vec0.size() == 3);
        AGNOSTIC_CHECK(vec0[0] == 'a');
        AGNOSTIC_CHECK(vec0[1] == 'B');
        AGNOSTIC_CHECK(vec0[2] == 'c');

        std::list<Value> l{ 'a', 'B', 'c', 'D' };
        vector<Value, default_bare_allocator, in_place_capacity<3>> vec1(l.begin(), l.end());
        AGNOSTIC_CHECK(!vec1.is_inplace());
        AGNOSTIC_CHECK(vec1.size() == 4);
        AGNOSTIC_CHECK(vec1[0] == 'a');
        AGNOSTIC_CHECK(vec1[1] == 'B');
        AGNOSTIC_CHECK(vec1[2] == 'c');
        AGNOSTIC_CHECK(vec1[3] == 'D');

        auto errb = vec1.cbegin() + 1;
        auto erre = vec1.cend() - 1;
        auto it = vec1.erase(errb, erre);
        AGNOSTIC_CHECK(*it == 'D');
        AGNOSTIC_CHECK(vec1.size() == 2);
        AGNOSTIC_CHECK(vec1[0] == 'a');
        AGNOSTIC_CHECK(vec1[1] == 'D');
        it = vec1.erase(vec1.cbegin());
        AGNOSTIC_CHECK(*it == 'D');
        AGNOSTIC_CHECK(vec1.size() == 1);
        AGNOSTIC_CHECK(vec1[0] == 'D');

        it = vec1.insert(vec1.begin(), v.begin(), v.end());
        AGNOSTIC_CHECK(*it == 'a');
        AGNOSTIC_CHECK(vec1.size() == 4);
        AGNOSTIC_CHECK(vec1[0] == 'a');
        AGNOSTIC_CHECK(vec1[1] == 'B');
        AGNOSTIC_CHECK(vec1[2] == 'c');
        AGNOSTIC_CHECK(vec1[3] == 'D');
        it = vec1.insert(vec1.begin() + 2, v.begin(), v.end());
        AGNOSTIC_CHECK(*it == 'a');
        AGNOSTIC_CHECK(vec1.size() == 7);
        AGNOSTIC_CHECK(vec1[0] == 'a');
        AGNOSTIC_CHECK(vec1[1] == 'B');
        AGNOSTIC_CHECK(vec1[2] == 'a');
        AGNOSTIC_CHECK(vec1[3] == 'B');
        AGNOSTIC_CHECK(vec1[4] == 'c');
        AGNOSTIC_CHECK(vec1[5] == 'c');
        AGNOSTIC_CHECK(vec1[6] == 'D');
        it = vec1.insert(vec1.end(), v.begin(), v.end());
        AGNOSTIC_CHECK(*it == 'a');
        AGNOSTIC_CHECK(vec1.size() == 10);
        AGNOSTIC_CHECK(vec1[0] == 'a');
        AGNOSTIC_CHECK(vec1[1] == 'B');
        AGNOSTIC_CHECK(vec1[2] == 'a');
        AGNOSTIC_CHECK(vec1[3] == 'B');
        AGNOSTIC_CHECK(vec1[4] == 'c');
        AGNOSTIC_CHECK(vec1[5] == 'c');
        AGNOSTIC_CHECK(vec1[6] == 'D');
        AGNOSTIC_CHECK(vec1[7] == 'a');
        AGNOSTIC_CHECK(vec1[8] == 'B');
        AGNOSTIC_CHECK(vec1[9] == 'c');

        vec1.resize(5);
        AGNOSTIC_CHECK(vec1.size() == 5);
        
        it = vec1.insert(vec1.end() - 2, l.begin(), l.end());
        AGNOSTIC_CHECK(*it == 'a');
        AGNOSTIC_CHECK(vec1.size() == 9);
        AGNOSTIC_CHECK(vec1[0] == 'a');
        AGNOSTIC_CHECK(vec1[1] == 'B');
        AGNOSTIC_CHECK(vec1[2] == 'a');

        AGNOSTIC_CHECK(vec1[3] == 'a');
        AGNOSTIC_CHECK(vec1[4] == 'B');
        AGNOSTIC_CHECK(vec1[5] == 'c');
        AGNOSTIC_CHECK(vec1[6] == 'D');

        AGNOSTIC_CHECK(vec1[7] == 'B');
        AGNOSTIC_CHECK(vec1[8] == 'c');
    }
    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
    {
        // resize test
        using namespace agnostic;
        vector<Value, default_bare_allocator, in_place_capacity<2>> vec1;
        vec1.resize(128);
        AGNOSTIC_CHECK(vec1.size() == 128);
        for (auto const& v : vec1) {
            AGNOSTIC_CHECK(v == 'Z');
        }
    }
    AGNOSTIC_CHECK_EQUAL(Value::count_, 0);
}
