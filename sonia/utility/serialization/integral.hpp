//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_INTEGRAL_HPP
#define SONIA_SERIALIZATION_INTEGRAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <algorithm>

#include <boost/integer_traits.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/utility/algorithm/pull.hpp"

#include "serialization.hpp"

namespace sonia { namespace serialization {

template <typename TagT>
class coder<TagT, bool> {
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(bool value, OutputIteratorT oi) const {
        *oi = value ? 1 : 0;
        ++oi;
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, bool & value) const {
        value = *ii != 0;
        ++ii;
        return std::move(ii);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, bool * val) const {
        return decode(std::move(ii), *val);
    }
};

template <typename TagT, typename T>
class coder<TagT, T, enable_if_t<is_integral_v<T> && sizeof(T) == 1 && !is_same_v<bool, T>>> {
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T value, OutputIteratorT oi) const {
        *oi = value;
        ++oi;
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T & value) const {
        value = (T)*ii;
        ++ii;
        return std::move(ii);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T * val) const {
        return decode(std::move(ii), *val);
    }
};

template <typename TagT, typename T>
class coder<TagT, T, enable_if_t<is_enum_v<T>>> {
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T value, OutputIteratorT oi) const {
        return coder<TagT, int>().encode((int)value, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T * value) const {
        return coder<TagT, int>().decode(std::move(ii), (int*)value);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T & value) const {
        return coder<TagT, int>().decode(std::move(ii), (int*)&value);
    }
};

namespace integral_detail {

template <typename T> constexpr bool is_special = sizeof(T) == 1 || is_same_v<bool, T> || is_enum_v<T>;

}

template <typename T>
class coder<default_t, T, enable_if_t<is_integral_v<T> && !integral_detail::is_special<T>>>
{
public:
    template <typename ArgT, typename OutputIteratorT>
    OutputIteratorT encode(ArgT val, OutputIteratorT oi) const {
        auto it = reinterpret_cast<char const*>(&val), eit = it + sizeof(ArgT);
        return std::copy(it, eit, oi);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT * val) const {
        auto it = reinterpret_cast<char*>(val), eit = it + sizeof(ArgT);
        return pull(std::move(ii), it, eit);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT & val) const {
        return decode(std::move(ii), &val);
    }
};

template <typename T>
class coder<compressed_t, T, enable_if_t<is_integral_v<T> && is_signed_v<T> && !integral_detail::is_special<T>>>
{
    typedef remove_cv_t<T> type;
    typedef boost::integer_traits<type> traits;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type val, OutputIteratorT oi) const {
        int fields  = (traits::digits + 6) / 7u;

        int sign = val < 0 ? 1 : 0;

        if (sign) {
            val *= -1;
        }

        for (int i = 0; i < fields; ++i, ++oi) {
            if (i == 0) {
                uint8_t block = val & 0x3F;
                val >>= 6;

                if (sign) block |= 0x80;
                if (val > 0) {
                    block |= 0x40;
                    *oi = static_cast<char>(block);
                } else {
                    *oi = static_cast<char>(block);
                    ++oi;
                    break;
                }
            } else {
                uint8_t block = val & 0x7F;
                val >>= 7;

                if (val > 0) {
                    block |= 0x80;
                    *oi = static_cast<char>(block);
                } else {
                    *oi = static_cast<char>(block);
                    ++oi;
                    break;
                }              
            }
        }

        return std::move(oi);
    }
};

template <typename T>
class coder<compressed_t, T, enable_if_t<is_integral_v<T> && !is_signed_v<T> && !integral_detail::is_special<T>>>
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

template <typename T>
class coder<ordered_t, T, enable_if_t<is_integral_v<T> && !is_signed_v<T> && !integral_detail::is_special<T>>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T val, OutputIteratorT oi) const {
        for (int i = sizeof(T) - 1; i >= 0; --i) {
            *oi = (val >> CHAR_BIT * i) & ((1 << CHAR_BIT) - 1);
            ++oi;
        }
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T & val) const {
        val = T{0};
        for (int i = sizeof(T) - 1; i >= 0; --i) {
            val |= static_cast<uint8_t>(*ii) * (1 << CHAR_BIT * i);
            ++ii;
        }
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T * val) const {
        return decode(std::move(ii), *val);
    }
};

}}

#endif // SONIA_SERIALIZATION_INTEGRAL_HPP
