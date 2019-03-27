//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_STRING_HPP
#define SONIA_SERIALIZATION_STRING_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <algorithm>

#include "sonia/string.hpp"
#include "array.hpp"
#include "integral.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename T>
class coder<TagT, T, enable_if_t<
    is_template_instance_v<basic_string_view, T> ||
    is_template_instance_v<basic_cstring_view, T>>>
{
    using element_type = add_const_t<typename T::value_type>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T const& value, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(static_cast<array_view<element_type> const&>(value), std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T & value) const
    {
        return sonia::decode<TagT>(std::move(ii), array_view(const_cast<typename T::value_type*>(value.begin(), value.size())));
    }
};

template <typename T>
class coder<ordered_t, T, enable_if_t<
    is_template_instance_v<basic_string_view, T> ||
    is_template_instance_v<basic_cstring_view, T>>>
{
    using type = T;
    using elem_t = typename T::value_type;

public:
    template <typename OutputIteratorT>
    static void encode_elem(elem_t e, OutputIteratorT & oi)
    {
        if constexpr (is_trivial_v<elem_t> && sizeof(elem_t) == 1) {
            *oi = e; ++oi;
        } else {
            oi = sonia::encode<ordered_t>(e, std::move(oi));
        }
    }

    template <typename OutputIteratorT>
    OutputIteratorT encode(T const& value, OutputIteratorT oi) const
    {
        for (auto c : value) {
            if (BOOST_UNLIKELY(c < 2)) {
                encode_elem(1, oi);
            }
            encode_elem(c, oi);
        }
        encode_elem(0, oi);
        return std::move(oi);
    }

    template <typename InputIteratorT>
    static elem_t decode_elem(InputIteratorT & ii)
    {
        if constexpr (is_trivial_v<elem_t> && sizeof(elem_t) == 1)
        {
            elem_t c = *ii; ++ii;
            return c;
        } else {
            elem_t c;
            ii = sonia::decode<ordered_t>(std::move(ii), c);
            return c;
        }
    }

    template <typename InputIteratorT, typename OutputIteratorT>
    InputIteratorT decode(InputIteratorT ii, OutputIteratorT oi) const
    {
        for (;;) {
            elem_t c = decode_elem(ii);
            if (BOOST_LIKELY(c > 1)) {
                *oi = c; ++oi;
            } else if (BOOST_LIKELY(!c)) {
                break;
            } else {
                c = decode_elem(ii);
                *oi = c; ++oi;
            }
        }
        return std::move(ii);
    }
};

template <typename TagT, typename CharT, class TraitsT>
class coder<TagT, std::basic_string<CharT, TraitsT>>
{
    using string_t = std::basic_string<CharT, TraitsT>;
    using size_type = typename string_t::size_type;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(string_t const& value, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(to_array_view(value),
            sonia::encode<TagT, size_type>(value.size(), std::move(oi))
        );
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, string_t & value) const
    {
        size_type sz;
        ii = sonia::decode<TagT>(std::move(ii), sz);
        value.resize(sz);
        return sonia::decode<TagT>(std::move(ii), array_view(value.data(), value.size()));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, string_t * value) const
    {
        return default_decode_ptr<TagT>(std::move(ii), value);
    }
};

template <typename CharT, class TraitsT>
class coder<ordered_t, std::basic_string<CharT, TraitsT>>
{
    using string_t = std::basic_string<CharT, TraitsT>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(string_t const& value, OutputIteratorT oi) const
    {
        return sonia::encode<ordered_t>(to_string_view(value), std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, string_t & value) const
    {
        value.clear();
        return sonia::decode<ordered_t, basic_string_view<CharT, TraitsT>>(std::move(ii), std::back_inserter(value));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, string_t * value) const
    {
        return default_decode_ptr<ordered_t>(std::move(ii), value);
    }
};

}}

#endif // SONIA_SERIALIZATION_STRING_HPP
