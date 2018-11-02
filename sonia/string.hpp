//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_STRING_HPP
#define SONIA_STRING_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <string>
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include "array_view.hpp"
#include "sonia/utility/comparison_operators.hpp"

namespace sonia {

template <typename CharT, class TraitsT = std::char_traits<CharT>>
class basic_string_view : public array_view<std::add_const_t<CharT>>
{
    typedef std::add_const_t<CharT> char_ct;
    typedef array_view<char_ct> base_t;
    typedef typename base_t::size_type size_type;

public:
    constexpr basic_string_view() noexcept {}
    constexpr basic_string_view(char_ct * str, size_type sz) : base_t(str, sz) {}
    
    template <typename SomeCharT>
    constexpr basic_string_view(array_view<SomeCharT> arr) noexcept : base_t(arr) {}

    basic_string_view(char_ct * str) noexcept : base_t(str, TraitsT::length(str)) {}

    template <class AllocT>
    basic_string_view(std::basic_string<CharT, TraitsT, AllocT> const& str) noexcept : base_t(str.c_str(), str.size()) {}

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
};

template <typename CharT, class TraitsT = std::char_traits<CharT>>
class basic_cstring_view : public basic_string_view<CharT, TraitsT> {
    typedef basic_string_view<CharT, TraitsT> base_t;
    typedef std::add_const_t<CharT> char_ct;

public:
    basic_cstring_view(char_ct * str) noexcept : base_t(str) {}

    template <class AllocT>
    basic_cstring_view(std::basic_string<CharT, TraitsT, AllocT> const& str) noexcept : base_t(str) {}

    const char* c_str() const noexcept { return this->data(); }
};

template <typename CharT, class TraitsT>
basic_string_view<CharT, TraitsT> to_string_view(basic_string_view<CharT, TraitsT> sv) { return sv; }

template <typename CharT, class TraitsT, class AllocT>
basic_string_view<CharT, TraitsT> to_string_view(std::basic_string<CharT, TraitsT, AllocT> const& s) {
    return basic_string_view<CharT, TraitsT>(s);
}

template <class T> struct is_string_view : false_type {};
template <typename CharT, class TraitsT> struct is_string_view<basic_string_view<CharT, TraitsT>> : true_type {};
template <typename CharT, class TraitsT> struct is_string_view<basic_cstring_view<CharT, TraitsT>> : true_type {};
template <class T> constexpr bool is_string_view_v = is_string_view<T>::value;

template <typename CharT, class TraitsT>
constexpr bool operator== (basic_string_view<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept {
    return lhs.is_equal(rhs);
}

template <typename CharT, class TraitsT>
constexpr bool operator== (basic_string_view<CharT, TraitsT> const& lhs, std::basic_string<CharT, TraitsT> const& rhs) noexcept {
    return lhs.is_equal(basic_string_view<CharT, TraitsT>(rhs));
}

template <typename CharT, class TraitsT>
constexpr bool operator== (std::basic_string<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept {
    return rhs.is_equal(basic_string_view<CharT, TraitsT>(lhs));
}

template <typename CharT, class TraitsT>
bool operator== (basic_string_view<CharT, TraitsT> const& lhs, CharT const* rhs) noexcept {
    return lhs.is_equal(basic_string_view<CharT, TraitsT>(rhs));
}

template <typename CharT, class TraitsT>
bool operator== (CharT const* lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept {
    return rhs.is_equal(basic_string_view<CharT, TraitsT>(lhs));
}

template <typename CharT, class TraitsT>
constexpr bool operator< (basic_string_view<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept {
    return lhs.compare(rhs) < 0;
}

template <typename CharT, class TraitsT>
constexpr bool operator< (basic_string_view<CharT, TraitsT> const& lhs, std::basic_string<CharT, TraitsT> const& rhs) noexcept {
    return lhs.compare(rhs.c_str(), rhs.size()) < 0;
}

template <typename CharT, class TraitsT>
constexpr bool operator< (std::basic_string<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept {
    return rhs.compare(lhs.c_str(), lhs.size()) > 0;
}

MAKE_FREE_COMPARISON_OPERATORS(is_string_view_v)

typedef basic_string_view<char> string_view;
typedef basic_string_view<wchar_t> wstring_view;
typedef basic_cstring_view<char> cstring_view;

struct string_equal_to {
    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, basic_string_view<CharT, TraitsT> r) const noexcept
    { return l == r; }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    { return l == r; }

    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    { return l == r; }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, basic_string_view<CharT, TraitsT> r) const noexcept
    { return r == l; }
};

struct string_less {
    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, basic_string_view<CharT, TraitsT> r) const noexcept
    { return l < r; }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    { return l < r; }

    template <typename CharT, class TraitsT>
    bool operator()(basic_string_view<CharT, TraitsT> l, std::basic_string<CharT, TraitsT> const& r) const noexcept
    { return l < r; }

    template <typename CharT, class TraitsT>
    bool operator()(std::basic_string<CharT, TraitsT> const& l, basic_string_view<CharT, TraitsT> r) const noexcept
    { return l < r; }
};

template <typename CharT>
std::basic_string<CharT> to_string(const CharT * arr) { return std::basic_string<CharT>(arr); }

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(std::basic_string<CharT, TraitsT> const& s) { return s; }

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(std::basic_string<CharT, TraitsT> && s) { return std::move(s); }

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(basic_string_view<CharT, TraitsT> sv) {
    return std::basic_string<CharT, TraitsT>(sv.cbegin(), sv.cend());
}

template<class Ch, class Tr, class Alloc>
std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> const& fmt) { return fmt.str(); }

//template<class Ch, class Tr, class Alloc>
//std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> & fmt) { return fmt.str(); }

template<class Ch, class Tr, class Alloc>
std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> && fmt) { return fmt.str(); }

template<class Ch, class Tr, class Alloc>
std::basic_string<Ch, Tr> to_string(boost::basic_format<Ch, Tr, Alloc> const && fmt) { return fmt.str(); }

template <typename CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, basic_string_view<CharT, TraitsT> s) {
    return os.write(s.begin(), s.size());
}

inline boost::basic_format<char> operator "" _fmt(const char* str, std::size_t) {
    return boost::format(str);
}

template <typename CharT, size_t N>
inline boost::basic_format<CharT> fmt(const CharT (&str)[N]) {
    return boost::format(str);
}

}

#endif // SONIA_STRING_HPP
