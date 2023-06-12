//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <string>
#include <boost/format.hpp>
#include "array_view.hpp"
#include "sonia/utility/comparison_operators.hpp"

namespace sonia {

template <typename CharT, class TraitsT = std::char_traits<CharT>>
class basic_string_view : public array_view<std::add_const_t<CharT>>
{
    using char_ct = std::add_const_t<CharT>;
    using base_t = array_view<char_ct>;

public:
    using traits_type = TraitsT;
    using value_type = CharT;
    using pointer = CharT * ;
    using const_pointer = CharT const*;
    using reference = CharT & ;
    using const_reference = CharT const&;
    using size_type = typename base_t::size_type;

    constexpr basic_string_view() noexcept = default;
    constexpr basic_string_view(char_ct * str, size_type sz) noexcept : base_t(str, sz) {}
    constexpr basic_string_view(char_ct * bstr, char_ct * estr) noexcept : base_t(bstr, estr) {}

    template <size_t N>
    constexpr basic_string_view(char_ct(&arr)[N]) noexcept : base_t(arr, N) {}

    template <typename SomeCharT>
    constexpr basic_string_view(array_view<SomeCharT> arr) noexcept : base_t(arr) {}

    basic_string_view(char_ct * str) noexcept : base_t(str, str ? TraitsT::length(str) : 0) {}

    template <class AllocT>
    basic_string_view(std::basic_string<CharT, TraitsT, AllocT> const& str) noexcept : base_t(str.c_str(), str.size()) {}

    template <typename SrcCharT, class SrcTraitsT, class AllocT>
    requires (!is_same_v<CharT, SrcCharT>)
    explicit basic_string_view(std::basic_string<SrcCharT, SrcTraitsT, AllocT> const& str) noexcept
        : base_t(reinterpret_cast<const CharT*>(str.data()), str.size())
    {}

    constexpr bool is_equal(basic_string_view const& rhs) const noexcept
    {
        return this->size_ == rhs.size_ && TraitsT::compare(this->data_, rhs.data_, rhs.size_) == 0;
    }

    constexpr int compare(basic_string_view const& rhs) const noexcept
    {
        return compare(rhs.data_, rhs.size_);
    }

    constexpr int compare(CharT const* data, size_t sz) const noexcept
    {
        const int r = TraitsT::compare(this->data_, data, (std::min)(this->size_, sz));
        if (r != 0) return r;
        if (this->size_ < sz) return -1;
        if (this->size_ > sz) return 1;
        return 0;
    }

    bool starts_with(basic_string_view prefix) const
    {
        return base_t::starts_with(prefix);
    }

    bool ends_with(basic_string_view prefix) const
    {
        return base_t::ends_with(prefix);
    }
};

template <typename CharT, class TraitsT = std::char_traits<CharT>>
class basic_cstring_view : public basic_string_view<CharT, TraitsT>
{
    using base_type = basic_string_view<CharT, TraitsT>;
    using char_ct = std::add_const_t<CharT>;

public:
    using size_type = typename base_type::size_type;

    basic_cstring_view() noexcept : base_type{zerostr} {}
    constexpr basic_cstring_view(char_ct * str) noexcept : base_type(str) {}
    constexpr basic_cstring_view(char_ct * str, size_type sz) : base_type(str, sz) { BOOST_ASSERT(str[sz] == 0); }
    constexpr basic_cstring_view(char_ct * bstr, char_ct * estr) : base_type(bstr, estr) { BOOST_ASSERT(*estr == 0); }

    template <class AllocT>
    basic_cstring_view(std::basic_string<CharT, TraitsT, AllocT> const& str) noexcept : base_type(str) {}

    template <typename SomeCharT>
    constexpr basic_cstring_view(array_view<SomeCharT> arr) : base_type(arr) { BOOST_ASSERT(!arr.empty() && arr[arr.size() - 1] == 0); this->advance_back(-1); }

    const char* c_str() const noexcept { return this->data(); }

    static CharT zerostr[1];
};

template <typename CharT, class TraitsT>
CharT basic_cstring_view<CharT, TraitsT>::zerostr[1] = {0};

template <typename CharT, class TraitsT>
basic_string_view<CharT, TraitsT> to_string_view(basic_string_view<CharT, TraitsT> sv) { return sv; }

template <typename CharT, class TraitsT>
basic_cstring_view<CharT, TraitsT> to_string_view(basic_cstring_view<CharT, TraitsT> sv) { return sv; }

template <typename CharT, class TraitsT, class AllocT>
basic_cstring_view<CharT, TraitsT> to_string_view(std::basic_string<CharT, TraitsT, AllocT> const& s)
{
    return basic_cstring_view<CharT, TraitsT>(s);
}

template <typename CharT, class AllocatorT>
basic_string_view<CharT> to_string_view(std::vector<CharT, AllocatorT> const& v)
{
    return basic_string_view<CharT>(v.empty() ? nullptr : &v.front(), v.size());
}

template <typename CharT, class TraitsT>
basic_cstring_view<CharT, TraitsT> to_cstring_view(basic_cstring_view<CharT, TraitsT> sv) { return sv; }

template <typename CharT, class TraitsT, class AllocT>
basic_cstring_view<CharT, TraitsT> to_cstring_view(std::basic_string<CharT, TraitsT, AllocT> const& s)
{
    return basic_cstring_view<CharT, TraitsT>(s);
}

template <typename CharT>
basic_string_view<CharT> to_string_view(CharT * arr) noexcept
{
    return basic_string_view<CharT>(arr);
}

template <class T> struct is_string_view : false_type {};
template <typename CharT, class TraitsT> struct is_string_view<basic_string_view<CharT, TraitsT>> : true_type {};
template <typename CharT, class TraitsT> struct is_string_view<basic_cstring_view<CharT, TraitsT>> : true_type {};
template <class T> constexpr bool is_string_view_v = is_string_view<T>::value;

template <typename CharT, class TraitsT>
constexpr bool operator== (basic_string_view<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return lhs.is_equal(rhs);
}

template <typename CharT, class TraitsT>
constexpr bool operator== (basic_string_view<CharT, TraitsT> const& lhs, std::basic_string<CharT, TraitsT> const& rhs) noexcept
{
    return lhs.is_equal(basic_string_view<CharT, TraitsT>(rhs));
}

template <typename CharT, class TraitsT>
constexpr bool operator== (std::basic_string<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return rhs.is_equal(basic_string_view<CharT, TraitsT>(lhs));
}

template <typename CharT, class TraitsT>
bool operator== (basic_string_view<CharT, TraitsT> const& lhs, CharT const* rhs) noexcept
{
    return lhs.is_equal(basic_string_view<CharT, TraitsT>(rhs));
}

template <typename CharT, class TraitsT>
bool operator== (CharT const* lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return rhs.is_equal(basic_string_view<CharT, TraitsT>(lhs));
}

template <typename CharT, class TraitsT>
constexpr bool operator< (basic_string_view<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template <typename CharT, class TraitsT>
constexpr bool operator< (basic_string_view<CharT, TraitsT> const& lhs, std::basic_string<CharT, TraitsT> const& rhs) noexcept
{
    return lhs.compare(rhs.c_str(), rhs.size()) < 0;
}

template <typename CharT, class TraitsT>
constexpr bool operator< (std::basic_string<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return rhs.compare(lhs.c_str(), lhs.size()) > 0;
}

MAKE_FREE_COMPARISON_OPERATORS(is_string_view_v)

using string_view = basic_string_view<char>;
using u8string_view = basic_string_view<char8_t>;
using wstring_view = basic_string_view<wchar_t>;
using cstring_view = basic_cstring_view<char>;
using u8cstring_view = basic_cstring_view<char8_t>;

struct string_equal_to
{
    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, basic_string_view<CharT, TraitsT> r) const noexcept
    {
        return l == r;
    }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    {
        return l == r;
    }

    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    {
        return l == r;
    }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string_view<CharT, TraitsT> l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    {
        return l == r;
    }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, basic_string_view<CharT, TraitsT> r) const noexcept
    {
        return r == l;
    }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, std::basic_string_view<CharT, TraitsT> r) const noexcept
    {
        return r == l;
    }
};

struct string_less
{
    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, basic_string_view<CharT, TraitsT> r) const noexcept
    {
        return l < r;
    }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    {
        return l < r;
    }

    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    {
        return l < r;
    }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, basic_string_view<CharT, TraitsT> r) const noexcept
    {
        return l < r;
    }
};

template <typename CharT>
std::basic_string<CharT> to_string(const CharT * arr) { return std::basic_string<CharT>(arr); }

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(std::basic_string<CharT, TraitsT> const& s) { return s; }

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(std::basic_string<CharT, TraitsT> && s) { return std::move(s); }

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(basic_string_view<CharT, TraitsT> sv)
{
    return std::basic_string<CharT, TraitsT>(sv.cbegin(), sv.cend());
}

template <typename CharT, typename SrcCharT, class SrcTraitsT>
requires(!is_same_v<CharT, SrcCharT> && sizeof(CharT) == sizeof(SrcCharT))
std::basic_string<CharT> to_string(basic_string_view<SrcCharT, SrcTraitsT> sv)
{
    return std::basic_string<CharT>(reinterpret_cast<const CharT*>(sv.data()), sv.size());
}

template <typename CharT, class AllocatorT>
std::basic_string<CharT> to_string(std::vector<CharT, AllocatorT> const& v)
{
    return std::basic_string<CharT>(v.begin(), v.end());
}

template<class Ch, class Tr, class Alloc>
std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> const& fmt) { return fmt.str(); }

//template<class Ch, class Tr, class Alloc>
//std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> & fmt) { return fmt.str(); }

template<class Ch, class Tr, class Alloc>
std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> && fmt) { return fmt.str(); }

template<class Ch, class Tr, class Alloc>
std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> const && fmt) { return fmt.str(); }

template <typename CharT, template<class> class TraitsT, typename SrcCharT>
std::basic_ostream<CharT, TraitsT<CharT>>& operator<< (std::basic_ostream<CharT, TraitsT<CharT>>& os, basic_string_view<SrcCharT, TraitsT<SrcCharT>> s)
{
    return os.write(reinterpret_cast<CharT const*>(s.data()), s.size());
}

inline boost::basic_format<char> operator "" _fmt(const char* str, std::size_t)
{
    return boost::format(str);
}

/*
inline boost::basic_format<char8_t> operator "" _fmt(const char8_t* str, std::size_t)
{
    return boost::basic_format<char8_t>(str);
}
*/

template <typename CharT, size_t N>
inline boost::basic_format<CharT> fmt(const CharT(&str)[N])
{
    return boost::format(str);
}

// template specialization for foreign namespace std
template <class CharT, class TraitsT, class AllocatorT>
struct hash<std::basic_string<CharT, TraitsT, AllocatorT>>
{
    size_t operator()(std::basic_string<CharT, TraitsT, AllocatorT> const& str) const noexcept
    {
        return hash<array_view<const CharT>>()(to_string_view(str));
    }
};

template <class CharT, class TraitsT>
struct hash<std::basic_string_view<CharT, TraitsT>>
{
    size_t operator()(std::basic_string_view<CharT, TraitsT> str) const noexcept
    {
        return hash<array_view<const CharT>>()({str.data(), str.size()});
    }
};

// for own namespace just add function definitions to use ADL
template <class CharT, class TraitsT>
size_t hash_value(basic_string_view<CharT, TraitsT> sv)
{
    return hash<array_view<const CharT>>()(sv);
}

template <class CharT, class TraitsT>
size_t hash_value(basic_cstring_view<CharT, TraitsT> csv)
{
    return hash<array_view<const CharT>>()(csv);
}

template <size_t BuffSzV, class CharT, class TraitsT, typename FtorT>
auto as_cstring(basic_string_view<CharT, TraitsT> sv, FtorT const& ftor)
{
    if (BuffSzV > sv.size()) {
        CharT buff[BuffSzV];
        *std::copy(sv.begin(), sv.end(), buff) = 0;
        return ftor(basic_cstring_view<CharT, TraitsT>(buff, sv.size()));
    } else {
        auto tmp = to_string(sv);
        return ftor(basic_cstring_view<CharT, TraitsT>(tmp.c_str(), sv.size()));
    }
}

}

