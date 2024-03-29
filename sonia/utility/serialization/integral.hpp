//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include <algorithm>
#include <iterator> // std::advance

#include <boost/integer_traits.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/utility/algorithm/pull.hpp"
#include "sonia/utility/algorithm/copy.hpp"

#include "serialization.hpp"

namespace sonia::serialization {

template <typename TagT>
class coder<TagT, bool>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(bool value, OutputIteratorT oi) const
    {
        *oi = value ? 1 : 0;
        ++oi;
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, bool & value) const
    {
        value = *ii != 0;
        ++ii;
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, bool * val) const
    {
        return decode(std::move(ii), *val);
    }
};

template <typename TagT, std::integral T>
requires (sizeof(T) == 1 && !is_same_v<bool, T>)
class coder<TagT, T>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T value, OutputIteratorT oi) const
    {
        *oi = value;
        ++oi;
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T & value) const
    {
        value = (T)*ii;
        ++ii;
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T * val) const
    {
        return decode(std::move(ii), *val);
    }
};

template <typename TagT, typename T>
requires(is_enum_v<T>)
class coder<TagT, T>
{
    using utype_t = underlying_type_t<T>;
    static inline coder<TagT, utype_t> coder_{};

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T value, OutputIteratorT oi) const
    {
        return coder_.encode((utype_t)value, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T * value) const
    {
        return coder_.decode(std::move(ii), (utype_t*)value);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T & value) const
    {
        return coder_.decode(std::move(ii), (utype_t&)value);
    }
};

namespace integral_detail {

template <typename T> constexpr bool is_special = sizeof(T) == 1 || is_same_v<bool, T> || is_enum_v<T>;

}

template <std::integral T>
requires(!integral_detail::is_special<T>)
class coder<default_t, T>
{
public:
    template <typename ArgT, typename OutputIteratorT>
    OutputIteratorT encode(ArgT val, OutputIteratorT oi) const
    {
        auto it = reinterpret_cast<char const*>(&val), eit = it + sizeof(ArgT);
        return sonia::copy(it, eit, std::move(oi));
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT * val) const
    {
        auto it = reinterpret_cast<char*>(val), eit = it + sizeof(ArgT);
        return sonia::pull(std::move(ii), it, eit);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT & val) const
    {
        return decode(std::move(ii), &val);
    }
};

//template <typename T>
//class coder<compressed_t, T, enable_if_t<is_integral_v<T> && is_signed_v<T> && !integral_detail::is_special<T>>>
//{
//    typedef remove_cv_t<T> type;
//    typedef boost::integer_traits<type> traits;
//
//public:
//    template <typename OutputIteratorT>
//    OutputIteratorT encode(type val, OutputIteratorT oi) const
//    {
//        int fields  = (traits::digits + 6) / 7u;
//
//        int sign = val < 0 ? 1 : 0;
//
//        if (sign) {
//            val *= -1;
//        }
//
//        for (int i = 0; i < fields; ++i, ++oi) {
//            if (i == 0) {
//                uint8_t block = val & 0x3F;
//                val >>= 6;
//
//                if (sign) block |= 0x80;
//                if (val > 0) {
//                    block |= 0x40;
//                    *oi = static_cast<char>(block);
//                } else {
//                    *oi = static_cast<char>(block);
//                    ++oi;
//                    break;
//                }
//            } else {
//                uint8_t block = val & 0x7F;
//                val >>= 7;
//
//                if (val > 0) {
//                    block |= 0x80;
//                    *oi = static_cast<char>(block);
//                } else {
//                    *oi = static_cast<char>(block);
//                    ++oi;
//                    break;
//                }              
//            }
//        }
//
//        return std::move(oi);
//    }
//};

template <std::integral T>
requires(!is_signed_v<T> && !integral_detail::is_special<T>)
class coder<compressed_t, T>
{
    using type = remove_cv_t<T>;
    using traits = boost::integer_traits<type>;
    static constexpr unsigned int parts_ = (traits::digits + 6) / 7u;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type val, OutputIteratorT oi) const
    {
        for (unsigned int fs = parts_; fs; --fs) {
            uint8_t part = val & 0x7F;
            val >>= 7;
            if (val) {
                *oi = (part | 0x80);
                ++oi;
            } else {
                *oi = part;
                ++oi;
                break;
            }
        }

        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type & val) const
    {
        val = 0;
        for (unsigned int i = 0; i < parts_ * 7; i += 7) {
            type part = *ii;
            ++ii;

            const auto hasnext = part & 0x80;
            val |= ((part & 0x7f) << i);
            if (!hasnext) break;
        }
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type * val) const
    {
        return decode(std::move(ii), *val);
    }
};

template <std::integral T>
requires(is_signed_v<T> && !integral_detail::is_special<T>)
class coder<compressed_t, T>
{
    using type = remove_cv_t<T>;
    using unsigned_t = make_unsigned_t<type>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type val, OutputIteratorT oi) const
    {
        bool is_neg = val < 0 ? 1 : 0;
        unsigned_t surrogate = static_cast<unsigned_t>(is_neg ? -1 - val : val);
        surrogate = (surrogate << 1) + (is_neg ? 1 : 0);
        return coder<compressed_t, unsigned_t>().encode(surrogate, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type & val) const
    {
        unsigned_t surrogate;
        ii = coder<compressed_t, unsigned_t>().decode(std::move(ii), surrogate);
        bool is_neg = !!(surrogate & 1);
        val = static_cast<type>(surrogate >> 1);
        if (is_neg) {
            val = -val - 1;
        }
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type * val) const
    {
        return decode(std::move(ii), *val);
    }
};

// SzV is the size of an external storage in increments
template <size_t SzV, std::integral T>
requires(!is_signed_v<T> && !integral_detail::is_special<T>)
class coder<sized_t<SzV, ordered_t>, T>
{
    static constexpr size_t tsz_in_bytes = sizeof(T) * CHAR_BIT / 8;
    static constexpr bool is_too_big = tsz_in_bytes < SzV;
    static constexpr size_t realsz = is_too_big ? tsz_in_bytes : SzV;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T value, OutputIteratorT oi) const
    {
        if constexpr (is_too_big) { // put leading 0s if required
            for (auto i = SzV; i > sizeof(T); --i) {
                *oi = 0;
                ++oi;
            }
        } else if constexpr (sizeof(T) > SzV) {
            BOOST_ASSERT(value < ((T)1) << (realsz * 8));
        }

        for (int i = realsz - 1; i >= 0; --i) {
            *oi = (value >> (CHAR_BIT * i)) & ((1 << 8) - 1);
            ++oi;
        }

        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T & value) const
    {
        value = 0;
        if constexpr (sizeof(T) < SzV) { // skip trailing 0s if required
            std::advance(ii, sizeof(T) - SzV);
        }
        for (int i = realsz - 1; i >= 0; --i) {
            value |= static_cast<uint8_t>(*ii) * (1 << CHAR_BIT * i);
            ++ii;
        }
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T * value) const
    {
        return decode(std::move(ii), *value);
    }
};

template <size_t SzV, std::integral T>
requires(!is_signed_v<T> && !is_enum_v<T>)
class coder<sized_t<SzV, le_t>, T>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T value, OutputIteratorT oi) const
    {
        //static constexpr T bytemask = (((std::uintmax_t)1) << CHAR_BIT) - 1;
        for (int i = SzV; i > 0; --i) {
            *oi = static_cast<uint8_t>(value);
            value >>= CHAR_BIT;
            ++oi;
        }
        return std::move(oi);
    }
    
    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T& value) const
    {
        value = 0;
        for (size_t i = 0; i < SzV; ++i) {
            T tmp = static_cast<uint8_t>(*ii);
            tmp <<= CHAR_BIT * i;
            value |= tmp;
            ++ii;
        }
        return std::move(ii);
    }

    template <typename InputIteratorT>
    inline InputIteratorT decode(InputIteratorT ii, T* value) const
    {
        return decode(std::move(ii), *value);
    }
};

template <typename TagT, std::integral T>
requires(!is_signed_v<T> && !integral_detail::is_special<T>)
class coder<TagT, T>
    : public coder<sized_t<sizeof(T), TagT>, T>
{};

template <typename TagT, std::integral T, size_t SzV>
//requires(!integral_detail::is_special<T>)
class coder<TagT, sized_integral_t<T, SzV>>
{
    using value_t = sized_integral_t<T, SzV>;

public:
    template <typename OutputIteratorT>
    inline OutputIteratorT encode(value_t value, OutputIteratorT oi) const
    {
        return coder<sized_t<SzV, TagT>, T>{}.encode(value.value, std::move(oi));
    }

    template <typename InputIteratorT>
    inline InputIteratorT decode(InputIteratorT ii, value_t const& value) const
    {
        return coder<sized_t<SzV, TagT>, T>{}.decode(std::move(ii), value.value);
    }

    template <typename InputIteratorT>
    inline InputIteratorT decode(InputIteratorT && ii, value_t const* value) const
    {
        return coder<sized_t<SzV, TagT>, T>{}.decode(std::move(ii), value->value);
    }
};

}
