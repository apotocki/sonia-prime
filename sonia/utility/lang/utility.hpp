//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/cstdint.hpp"
#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"

#include <boost/container/small_vector.hpp>

namespace sonia::lang {

// ========================================================================
// a numeric identifier of a lexical identifier
// some ranges can be reserved by lang needs
template <std::integral ValueT, typename TagT = void>
struct identifier
{
    using value_type = ValueT;
    value_type value;
    //value_type value : 31;
    //value_type is_required : 1;

    inline bool empty() const noexcept { return !value; }
    inline explicit operator bool() const noexcept { return !empty(); }

    identifier() : value{ 0 } /*, is_required{ 0 }*/ {}
    explicit identifier(value_type val/*, bool is_required_val = false*/) : value{ val }/*, is_required{ value_type(is_required_val ? 1 : 0) }*/ {}

    template <std::integral IT>
    inline explicit identifier(IT val) : value{ static_cast<value_type>(val) }
    {
        // to do: bounds checking
    }

    friend inline bool operator== (identifier const& l, identifier const& r) noexcept
    {
        return l.value == r.value;
    }

    friend inline auto operator<=>(identifier const& l, identifier const& r) noexcept
    {
        return l.value <=> r.value;
    }

    inline identifier self_or(identifier other) const noexcept { return value ? *this : other; }

    inline value_type raw() const noexcept { return value; }
};

template <std::integral ValueT, typename TagT = void>
inline size_t hash_value(identifier<ValueT, TagT> const& v)
{
    return hash<typename identifier<ValueT, TagT>::value_type>{}(v.value);
}

template <typename T, typename Traits, std::integral ValueT, typename TagT>
std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits> & os, identifier<ValueT, TagT> idval)
{
    return os << idval.value;
}

namespace lex {

struct resource_location
{
    int line;
    int column;
    small_string resource;
};

struct scanner_data
{
    const char* str_buff_begin;
    boost::container::small_vector<resource_location, 8> loc_stack;
};

inline void undefined_lexem(const char* ltext, size_t sz)
{
    throw exception("undefined lexem: '%1%'"_fmt % string_view{ ltext, sz });
}

}

}
