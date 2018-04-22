//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_STRING_HPP
#define SONIA_STRING_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <string>
#include <string_view>
#include <boost/functional/hash.hpp>
#include "array_view.hpp"

namespace sonia {

template <typename CharT, class TraitsT = std::char_traits<CharT>>
class basic_string_view : public array_view<std::add_const_t<CharT>>
{
    typedef std::add_const_t<CharT> char_ct;
    typedef array_view<char_ct> base_t;

public:
    using base_t::base_t;

    basic_string_view(std::basic_string<CharT, TraitsT> const& str) : base_t(str.c_str(), str.size()) {}

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
public:

    const char* c_str() const noexcept { return data(); }
};

template <typename CharT, class TraitsT>
constexpr bool operator== (basic_string_view<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return lhs.is_equal(rhs);
}

template <typename CharT, class TraitsT>
constexpr bool operator!= (basic_string_view<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return !lhs.is_equal(rhs);
}

template <typename CharT, class TraitsT>
bool operator== (basic_string_view<CharT, TraitsT> const& lhs, std::basic_string<CharT, TraitsT> const& rhs) noexcept
{
    return lhs.size() == rhs.size() && TraitsT::compare(lhs.cbegin(), rhs.c_str(), lhs.size()) == 0;
}

template <typename CharT, class TraitsT>
bool operator== (std::basic_string<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return rhs == lhs;
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

typedef basic_string_view<char> string_view;
typedef basic_cstring_view<char> cstring_view;

struct string_hasher {
    size_t operator()(std::string const& r) const noexcept { return boost::hash_range(r.begin(), r.end()); }
    size_t operator()(string_view r) const noexcept { return boost::hash_range(r.begin(), r.end()); } 
};

struct string_equal_to {
    bool operator()(string_view l, string_view r) const noexcept { return l == r; }
    bool operator()(std::string const& l, std::string const& r) const noexcept { return l == r; }
    bool operator()(string_view l, std::string const& r) const noexcept { return l == r; }
    bool operator()(std::string const& l, string_view r) const noexcept { return r == l; }
};

struct string_less {
    bool operator()(string_view l, string_view r) const noexcept { return l < r; }
    bool operator()(std::string const& l, std::string const& r) const noexcept { return l < r; }
    bool operator()(string_view l, std::string const& r) const noexcept { return l < r; }
    bool operator()(std::string const& l, string_view r) const noexcept { return l < r; }
};

inline std::string to_string(string_view sv) {
    return std::string(sv.cbegin(), sv.cend());
}

template <typename CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, basic_string_view<CharT, TraitsT> s) {
    return os.write(s.begin(), s.size());
}

}

#endif // SONIA_STRING_HPP
