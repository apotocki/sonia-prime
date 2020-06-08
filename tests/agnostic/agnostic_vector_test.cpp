// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

//#include "sonia/config.hpp"
#include <limits>

#include "agnostic/allocator/BareAllocator.hpp"
//#include "agnostic/allocator/default_bare_allocator.hpp"
//#include "agnostic/ipo/vector.hpp"
//
//#include "agnostic/std/compare/partial_ordering.hpp"

namespace std {

class partial_ordering {

};

}

namespace {

class Value
{
public:
    Value() : value_{ 'Z' } { ++count_; }
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

#define AGNOSTIC_ASSERT(x) assert(x)

void agnostic_vector_test()
{

    //{
    //    agnostic::vector<int> vec;
    //    vec.push_back(1);
    //    AGNOSTIC_ASSERT(vec.size() == 1);
    //    AGNOSTIC_ASSERT(vec.front() == 1);
    //    AGNOSTIC_ASSERT(vec.back() == 1);

    //    size_t cnt = vec.capacity() - vec.size() + 1;
    //    for (int i = 0; i < cnt; ++i) {
    //        vec.push_back(i + 2);
    //        AGNOSTIC_ASSERT(vec.size() == size_t(2) + i);
    //        AGNOSTIC_ASSERT(vec.front() == 1);
    //        AGNOSTIC_ASSERT(vec.back() == i + 2);
    //    }
    //}
}