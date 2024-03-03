//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <string>
#include <string_view>
#include <atomic>
#include <boost/format.hpp>
#include "array_view.hpp"
#include "sonia/span.hpp"
#include "sonia/utility/comparison_operators.hpp"
#include "sonia/utility/optimized/array.hpp"

namespace sonia::experimental {

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

    const CharT* c_str() const noexcept { return this->data(); }

    operator std::basic_string_view<CharT, TraitsT>() const { return std::basic_string_view<CharT, TraitsT>{ c_str(), this->size() }; }

    static CharT zerostr[1];
};

template <typename CharT, class TraitsT>
CharT basic_cstring_view<CharT, TraitsT>::zerostr[1] = {0};

template <typename CharT, class TraitsT>
std::basic_string_view<CharT, TraitsT> to_string_view(std::basic_string_view<CharT, TraitsT> sv) { return sv; }

template <typename CharT, class TraitsT>
std::basic_string_view<CharT, TraitsT> to_string_view(basic_cstring_view<CharT, TraitsT> sv) { return std::basic_string_view<CharT, TraitsT>{sv.data(), sv.size()}; }

/*
template <typename CharT, class TraitsT, class AllocT>
basic_cstring_view<CharT, TraitsT> to_string_view(std::basic_string<CharT, TraitsT, AllocT> const& s)
{
    return basic_cstring_view<CharT, TraitsT>(s);
}
*/

template <typename CharT, class TraitsT>
basic_cstring_view<CharT, TraitsT> to_cstring_view(basic_cstring_view<CharT, TraitsT> sv) { return sv; }

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
bool operator== (basic_string_view<CharT, TraitsT> const& lhs, CharT const* rhs) noexcept
{
    return lhs.is_equal(basic_string_view<CharT, TraitsT>(rhs));
}

template <typename CharT, class TraitsT>
constexpr bool operator== (std::basic_string<CharT, TraitsT> const& lhs, basic_string_view<CharT, TraitsT> const& rhs) noexcept
{
    return rhs.is_equal(basic_string_view<CharT, TraitsT>(lhs));
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

template <typename CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& os, basic_string_view<CharT, TraitsT> s)
{
    return os.write(s.data(), s.size());
}

template <typename CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& os, basic_cstring_view<CharT, TraitsT> s)
{
    return os.write(s.data(), s.size());
}

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(basic_string_view<CharT, TraitsT> sv)
{
    return std::basic_string<CharT>(sv.data(), sv.size());
}

template <typename CharT, class TraitsT>
std::basic_string<CharT, TraitsT> to_string(basic_cstring_view<CharT, TraitsT> sv)
{
    return std::basic_string<CharT>(sv.data(), sv.size());
}

} // experimental

namespace sonia {

using namespace std::string_literals;
using namespace std::string_view_literals;
using std::basic_string_view;
using experimental::basic_cstring_view;
using string_view = std::string_view;
using u8string_view = std::u8string_view;
using wstring_view = std::wstring_view;
using cstring_view = experimental::basic_cstring_view<char>;
using u8cstring_view = experimental::basic_cstring_view<char8_t>;

// immutable string
template <typename CharT, size_t SzV = (std::max)(8 * sizeof(CharT), sizeof(void*)), class TraitsT = std::char_traits<CharT>>
class basic_small_string
{
    using shared_array_t = shared_optimized_array<const CharT, SzV, std::atomic<uint32_t>>;

public:
    using value_type = CharT;
    using pointer = CharT const*;
    using const_pointer = CharT const*;
    using reference = CharT const&;
    using const_reference = CharT const&;
    using size_type = size_t;

    using iterator = pointer;
    using const_iterator = pointer;

    basic_small_string() = default;

    basic_small_string(string_view sv)
        : array_{sv}
    {}

    template <size_t N>
    basic_small_string(const CharT(&arr)[N]) : array_{arr, N} {}

    basic_small_string(const CharT* str, size_t size) : array_{ basic_string_view{str, size} } {}

    basic_small_string(basic_small_string const&) = default;
    basic_small_string(basic_small_string &&) = default;
    basic_small_string& operator=(basic_small_string const&) = default;
    basic_small_string& operator=(basic_small_string &&) = default;

    basic_small_string & operator= (std::string_view sv)
    {
        array_ = shared_array_t{ sv };
        return *this;
    }

    template <size_t EV>
    bool is_equal(span<const CharT, EV> const& rhs) const noexcept
    {
        return size() == rhs.size() && TraitsT::compare(data(), rhs.data(), rhs.size()) == 0;
    }

    template <size_t EV>
    bool less(span<const CharT, EV> const& rhs) const noexcept
    {
        const int r = TraitsT::compare(data(), rhs.data(), (std::min)(size(), rhs.size()));
        return (r < 0) || (!r && size() < rhs.size());
    }

    operator span<const CharT>() const { return array_.to_span(); }
    operator basic_string_view<CharT, TraitsT>() const { return basic_string_view<CharT, TraitsT>{ data(), size() }; }

    inline constexpr reference front() const noexcept { return array_.front(); }
    inline constexpr reference back() const { return array_.back(); }
    inline const CharT* data() const noexcept { return array_.data(); }

    inline explicit operator bool() const noexcept { return !empty(); }
    inline bool empty() const noexcept { return array_.empty(); }
    inline size_t size() const noexcept { return array_.size(); }

    inline auto begin() const noexcept { return array_.begin(); }
    inline auto end() const noexcept { return array_.end(); }
    inline auto cbegin() const noexcept { return array_.begin(); }
    inline auto cend() const noexcept { return array_.end(); }

    bool is_shared() const { return array_.is_shared(); }

    bool starts_with(basic_string_view<CharT, TraitsT> prefix) const
    {
        if (size() < prefix.size()) return false;
        return 0 == std::memcmp(data(), prefix.data(), prefix.size() * sizeof(CharT));
    }

    reference operator[](size_type ind) const noexcept { return data()[ind]; }

private:
    shared_array_t array_;
};

template <typename CharT, template<class> class TraitsT, size_t SzV>
std::basic_ostream<CharT, TraitsT<CharT>>& operator<< (std::basic_ostream<CharT, TraitsT<CharT>>& os, basic_small_string<CharT, SzV, TraitsT<CharT>> s)
{
    return os.write(reinterpret_cast<CharT const*>(s.data()), s.size());
}

template <typename CharT, size_t SzV, class TraitsT>
inline span<const CharT> to_span(basic_small_string<CharT, SzV, TraitsT> const& s)
{
    return (span<const CharT>)s;
}


using small_string = basic_small_string<char>;
using small_wstring = basic_small_string<wchar_t>;
using small_u32string = basic_small_string<char32_t>;

template <typename CharT, class AllocatorT>
std::basic_string_view<CharT> to_string_view(std::vector<CharT, AllocatorT> const& v)
{
    return std::basic_string_view<CharT>(v.empty() ? nullptr : &v.front(), v.size());
}

template <typename CharT, class TraitsT, class AllocT>
experimental::basic_cstring_view<CharT, TraitsT> to_cstring_view(std::basic_string<CharT, TraitsT, AllocT> const& s)
{
    return basic_cstring_view<CharT, TraitsT>(s);
}

template <typename CharT>
experimental::basic_string_view<CharT> to_string_view(CharT * arr) noexcept
{
    return basic_string_view<CharT>(arr);
}

template <class T> struct is_string : false_type {};
template <class T> constexpr bool is_string_v = is_string<T>::value;
template <class T> concept string_type = is_string_v<T>;

template <typename CharT, class TraitsT> struct is_string<experimental::basic_string_view<CharT, TraitsT>> : true_type {};
template <typename CharT, class TraitsT> struct is_string<experimental::basic_cstring_view<CharT, TraitsT>> : true_type {};
template <typename CharT, class TraitsT> struct is_string<std::basic_string_view<CharT, TraitsT>> : true_type {};
template <typename CharT, class TraitsT> struct is_string<std::basic_string<CharT, TraitsT>> : true_type {};
template <typename CharT, size_t SzV, class TraitsT> struct is_string<basic_small_string<CharT, SzV, TraitsT>> : true_type {};

template <class T> struct is_owning_string : false_type {};
template <class T> constexpr bool is_owning_string_v = is_owning_string<T>::value;
template <typename CharT, class TraitsT> struct is_owning_string<std::basic_string<CharT, TraitsT>> : true_type {};
template <typename CharT, size_t SzV, class TraitsT> struct is_owning_string<basic_small_string<CharT, SzV, TraitsT>> : true_type {};


template <class T> struct is_string_view : false_type {};
template <typename CharT, class TraitsT> struct is_string_view<experimental::basic_string_view<CharT, TraitsT>> : true_type {};
template <typename CharT, class TraitsT> struct is_string_view<experimental::basic_cstring_view<CharT, TraitsT>> : true_type {};
template <typename CharT, class TraitsT> struct is_string_view<std::basic_string_view<CharT, TraitsT>> : true_type {};
template <class T> constexpr bool is_string_view_v = is_string_view<T>::value;


MAKE_FREE_COMPARISON_OPERATORS(is_string_view_v)


struct string_equal_to
{
    template <typename CharT, typename TraitsT, typename OtherT>
    requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
    inline bool operator() (basic_string_view<CharT, TraitsT> const& l, OtherT const& r) const noexcept
    {
        return l == (basic_string_view<CharT, TraitsT>)r;
    }

    template <typename CharT, typename TraitsT, typename OtherT>
    requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
    inline bool operator() (std::basic_string<CharT, TraitsT> const& l, OtherT const& r) const noexcept
    {
        return (basic_string_view<CharT, TraitsT>)l == (basic_string_view<CharT, TraitsT>)r;
    }

    template <typename CharT, size_t SzV, typename TraitsT, typename OtherT>
    requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
    inline bool operator() (basic_small_string<CharT, SzV, TraitsT> const& l, OtherT const& r) const noexcept
    {
        return l == r;
    }

    // pairs
    template <typename T1, typename T2, typename T3, typename T4>
    inline bool operator() (std::pair<T1, T2> const& l, std::pair<T3, T4> const& r) const noexcept
    {
        return operator()(l.first, r.first) && operator()(l.second, r.second);
    }
};

struct string_less
{
    template <typename CharT, typename TraitsT, typename OtherT>
    requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
    inline bool operator() (basic_string_view<CharT, TraitsT> const& l, OtherT const& r) const noexcept
    {
        return l < (basic_string_view<CharT, TraitsT>)r;
    }

    template <typename CharT, typename TraitsT, typename OtherT>
    requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
    inline bool operator() (std::basic_string<CharT, TraitsT> const& l, OtherT const& r) const noexcept
    {
        return (basic_string_view<CharT, TraitsT>)l < (basic_string_view<CharT, TraitsT>)r;
    }

    template <typename CharT, size_t SzV, typename TraitsT, typename OtherT>
    requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
    inline bool operator() (basic_small_string<CharT, SzV, TraitsT> const& l, OtherT const& r) noexcept
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

template <typename CharT, size_t SzV, typename TraitsT, typename OtherT>
requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
inline bool operator== (basic_small_string<CharT, SzV, TraitsT> const& lhs, OtherT const& rhs) noexcept
{
    auto sv = (basic_string_view<CharT, TraitsT>)rhs;
    return lhs.is_equal(span<const CharT>{sv.data(), sv.size()});
}

template <typename CharT, size_t SzV, typename TraitsT, typename OtherT>
requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
bool operator!= (basic_small_string<CharT, SzV, TraitsT> const& lhs, OtherT const& rhs) noexcept
{
    auto sv = (basic_string_view<CharT, TraitsT>)rhs;
    return !lhs.is_equal(span<const CharT>{sv.data(), sv.size()});
}

template <typename CharT, size_t SzV, typename TraitsT, typename OtherT>
requires(requires{(basic_string_view<CharT, TraitsT>)std::declval<OtherT>(); })
inline bool operator< (basic_small_string<CharT, SzV, TraitsT> const& lhs, OtherT const& rhs) noexcept
{
    auto sv = (basic_string_view<CharT, TraitsT>)rhs;
    return lhs.less(span<const CharT>{sv.data(), sv.size()});
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
        return hash<span<const CharT>>()({str.data(), str.size()});
    }
};

template <class CharT, class TraitsT>
struct hash<std::basic_string_view<CharT, TraitsT>>
{
    size_t operator()(std::basic_string_view<CharT, TraitsT> str) const noexcept
    {
        return hash<span<const CharT>>()({str.data(), str.size()});
    }
};

// for own namespace just add function definitions to use ADL
template <class CharT, class TraitsT>
size_t hash_value(basic_string_view<CharT, TraitsT> sv)
{
    return hash<span<const CharT>>()(sv);
}

template <class CharT, class TraitsT>
size_t hash_value(basic_cstring_view<CharT, TraitsT> csv)
{
    return hash<span<const CharT>>()(csv);
}

template <typename CharT, size_t SzV, typename TraitsT>
size_t hash_value(basic_small_string<CharT, SzV, TraitsT> ssv)
{
    return hash<span<const CharT>>()(ssv);
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

