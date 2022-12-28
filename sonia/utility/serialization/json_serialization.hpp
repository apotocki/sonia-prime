//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <cstdio>
#include <span>
#include <array>

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/karma_uint.hpp>
#include <boost/spirit/include/karma_generate.hpp>
#include <boost/variant.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "serialization.hpp"
#include "sonia/utility/serialization/variant.hpp"

namespace sonia::serialization {

struct json_t {};

class json_string_coder
{
public:
    template <typename OutputIteratorT>
    static void encode_elem(char c, OutputIteratorT& oi)
    {
        switch (c) {
        case 0: break;
        case '"':
            *oi = '\\'; ++oi;
            *oi = '"'; ++oi;
            break;
        case '\\':
            *oi = '\\'; ++oi;
            *oi = '\\'; ++oi;
            break;

        default:
            if (c >= 0 && c < 0x1f) {
                *oi = '\\'; ++oi;
                *oi = 'u'; ++oi;
                *oi = '0'; ++oi;
                *oi = '0'; ++oi;
                if (c < 16) {
                    *oi = '0'; ++oi;
                }
                    
                boost::spirit::karma::generate(oi, boost::spirit::hex, static_cast<unsigned int>(c));
            }
            else {
                *oi = c; ++oi;
            }
        }
    }

    template <typename StringT, typename OutputIteratorT>
    static OutputIteratorT encode(StringT const& value, OutputIteratorT oi)
    {
        for (auto c : value) {
            encode_elem(c, oi);
        }
        return std::move(oi);
    }
};

template <typename T>
requires (
    is_template_instance_v<basic_string_view, T> ||
    is_template_instance_v<basic_cstring_view, T> ||
    is_template_instance_v<std::basic_string, T>
)
class coder<json_t, T>
{
    using type = T;
    using elem_t = typename T::value_type;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T const& value, OutputIteratorT oi) const
    {
        if constexpr (is_trivial_v<elem_t> && sizeof(elem_t) == 1) {
            *oi = '"'; ++oi;
            oi = json_string_coder::encode(value, std::move(oi));
            *oi = '"'; ++oi;
            return std::move(oi);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    }
};

template <typename TargetT>
requires (is_integral_v<TargetT>)
class coder<json_t, TargetT>
{
public:
    template <typename ArgT, typename OutputIteratorT>
    OutputIteratorT encode(ArgT val, OutputIteratorT oi) const
    {
        char buff[64];
        int res = snprintf(buff, sizeof(buff), get_format<ArgT>(), val);
        if (res > 0) {
            return std::copy(buff, buff + res, std::move(oi));
        }
        THROW_INTERNAL_ERROR("can't print integer");
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT * val) const
    {
        auto it = reinterpret_cast<char*>(val), eit = it + sizeof(ArgT);
        return pull(std::move(ii), it, eit);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT & val) const
    {
        return decode(std::move(ii), &val);
    }

private:
    template <typename T>
    static const char * get_format()
    {
        if constexpr (is_same_v<T, int>) return "%d";
        else if constexpr (is_same_v<T, unsigned int>) return "%u";
        else if constexpr (is_same_v<T, int16_t>) return "%hd";
        else if constexpr (is_same_v<T, uint16_t>) return "%hu";
        else if constexpr (is_same_v<T, int64_t>) return "%lld";
        else if constexpr (is_same_v<T, uint64_t>) return "%llu";
        else {
            error<T>();
        }
    }

    template <typename T> static void error()
    {
        // unexpected integer type
        static_assert(is_integral_v<T>);
    }
};

template <typename ElemT, typename TraitsT, typename AllocT>
class coder<json_t, std::basic_string<ElemT, TraitsT, AllocT>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(std::basic_string<ElemT, TraitsT, AllocT> const& value, OutputIteratorT oi) const
    {
        return coder<json_t, basic_string_view<ElemT, TraitsT>>().encode(value, std::move(oi));
    }
};

template <typename ElemT, size_t SE>
class coder<json_t, std::span<ElemT, SE>>
{
    using span_t = std::span<ElemT, SE>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(std::span<const ElemT, SE> value, OutputIteratorT oi) const
    {
        *oi = '['; ++oi;
        if (auto it = value.begin(), eit = value.end(); it != eit) {
            oi = sonia::encode<json_t>(*it, std::move(oi));
            for (++it; it != eit; ++it) {
                *oi = ','; ++oi;
                oi = sonia::encode<json_t>(*it, std::move(oi));
            }
        }
        *oi = ']'; ++oi;
        return std::move(oi);
    }
};

template <typename ElemT, typename AllocatorT>
class coder<json_t, std::vector<ElemT, AllocatorT>>
    : public coder<json_t, std::span<ElemT>>
{

};

template <typename MapContainerT> class map_view;

template <typename KeyT, typename ValueT, size_t EV>
class map_view<std::span<const std::pair<KeyT, ValueT>, EV>>
{
    using pair_t = std::pair<KeyT, ValueT>;
    using map_t = std::span<const pair_t, EV>;

public:
    using key_type = KeyT;
    using value_type = ValueT;

    template <typename PairT>
    explicit map_view(std::span<PairT, EV> m) noexcept : map_{m} {}

    auto begin() const { return map_.begin(); }
    auto end() const { return map_.end(); }

private:
    map_t map_;
};

template <typename KeyT, typename ValueT, size_t EV>
class map_view<std::array<std::pair<KeyT, ValueT>, EV>>
{
    using pair_t = std::pair<KeyT, ValueT>;
    using map_t = std::array<pair_t, EV>;

public:
    using key_type = KeyT;
    using value_type = ValueT;

    auto begin() const { return map_.begin(); }
    auto end() const { return map_.end(); }

    map_t& container() { return map_; }
    map_t const& container() const { return map_; }

private:
    map_t map_;
};

template <typename PairT, size_t EV>
map_view(std::span<PairT, EV>) -> map_view<std::span<const PairT, EV>>;

template <typename ContainerT>
//requires(std::is_convertible_v<typename ContainerT::key_type, string_view>)
class coder<json_t, map_view<ContainerT>>
{
    using map_t = map_view<ContainerT>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(map_t value, OutputIteratorT oi) const
    {
        *oi = '{'; ++oi;
        if (auto it = value.begin(), eit = value.end(); it != eit) {
            oi = sonia::encode<json_t>(string_view{it->first}, std::move(oi));
            *oi = ':'; ++oi;
            oi = sonia::encode<json_t>(it->second, std::move(oi));
            for (++it; it != eit; ++it) {
                *oi = ','; ++oi;
                oi = sonia::encode<json_t>(string_view{it->first}, std::move(oi));
                *oi = ':'; ++oi;
                oi = sonia::encode<json_t>(it->second, std::move(oi));
            }
        }
        *oi = '}'; ++oi;
        return std::move(oi);
    }
};

}
