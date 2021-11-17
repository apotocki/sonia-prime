//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/functional/has.hpp"
#include "integer.hpp"

namespace sonia {

using namespace boost::multiprecision;

class hash_combilne_iterator {
public:
    hash_combilne_iterator(size_t & val) : value_(&val) {}

    hash_combilne_iterator& operator*() { return *this; }
    hash_combilne_iterator& operator++() { return *this; }

    template <typename T>
    void operator= (T const& val)
    {
        sonia::hash_combine(*value_, val);
    };

    size_t get() const { return *value_; }

private:
    size_t * value_;
};

integer::integer(string_view str) 
    : value_(to_string(str))
{

}

std::string integer::string() const {
    return value_.str();
}

size_t integer::hash() const {
    size_t hash_value = 0;
    export_bits(value_, hash_combilne_iterator(hash_value), sizeof(size_t));
    return hash_value;
}

}
