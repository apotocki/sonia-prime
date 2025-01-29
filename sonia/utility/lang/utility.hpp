//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/cstdint.hpp"
#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/small_vector.hpp"

namespace sonia::lang {

// ========================================================================
// a numeric identifier of a lexical identifier
// some ranges can be reserved by lang needs
template <std::integral ValueT, typename TagT = void>
struct identifier
{
#ifdef SONIA_LANG_DEBUG
    string_view debug_name;
#endif

    using value_type = ValueT;
    value_type value;

    inline bool empty() const noexcept { return !value; }
    inline explicit operator bool() const noexcept { return !empty(); }

    inline identifier() noexcept : value{ 0 } {}
    inline explicit identifier(value_type val) noexcept : value{ val } {}

    template <std::integral IT>
    inline explicit identifier(IT val) : value{ static_cast<value_type>(val) }
    {
        BOOST_ASSERT((std::numeric_limits<value_type>::max)() >= val);
        BOOST_ASSERT((std::numeric_limits<value_type>::min)() <= val);
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
inline size_t hash_value(identifier<ValueT, TagT> const& v) noexcept
{
    return hash<typename identifier<ValueT, TagT>::value_type>{}(v.value);
}

template <typename T, typename Traits, std::integral ValueT, typename TagT>
inline std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits> & os, identifier<ValueT, TagT> idval)
{
    return os << idval.value;
}

namespace lex {

class code_resource
{
public:
    virtual ~code_resource() = default;
    virtual small_string description() const = 0;
};

template <typename T, typename Traits>
inline std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& os, code_resource const& res)
{
    return os << res.description();
}

template <typename T, typename Traits>
inline std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& os, shared_ptr<code_resource> const& res)
{
    if (res) return os << *res;
    return os << "<undefined resource>"sv;
}

struct resource_location
{
    int line;
    int column;
    shared_ptr<code_resource> resource;

    explicit operator bool() const noexcept { return !!resource; }
};

struct resource_span
{
    int line_begin, line_end;
    int column_being, column_end;
    small_string resource;
};

struct scanner_data
{
    const char* str_buff_begin;
    small_vector<resource_location, 8> loc_stack;
};

inline void undefined_lexem(const char* ltext, size_t sz)
{
    throw exception("undefined lexem: '%1%'"_fmt % string_view{ ltext, sz });
}

}

}
