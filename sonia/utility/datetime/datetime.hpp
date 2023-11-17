//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <limits>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/utility/functional/hash.hpp"

namespace sonia {

template <typename StorageT = int64_t>
class datetime
{
    friend class boost::serialization::access;

    template <class ArchiveT>
    ArchiveT & serialize(ArchiveT & ar, const unsigned int v/* file_version */)
    {
        return ar & boost::serialization::make_nvp(0, value_);
    }

public:
    datetime() noexcept : value_{0} {}
    explicit datetime(StorageT val) noexcept : value_{val} {}

    static datetime (min)() noexcept { return datetime((std::numeric_limits<StorageT>::min)()); }
    static datetime (max)() noexcept { return datetime((std::numeric_limits<StorageT>::max)()); }

    StorageT ticks() const noexcept { return value_; }
    
    void swap(datetime & rhs) noexcept { std::swap(value_, rhs.value_); }

    friend inline bool operator==(const datetime& lhs, const datetime& rhs) = default;
    friend inline auto operator<=>(const datetime& lhs, const datetime& rhs) = default;

private:
    StorageT value_;
};

template <typename StorageT>
size_t hash_value(datetime<StorageT> const& v) noexcept
{
    return hasher{}(v.ticks());
}

template<typename ElemT, typename CharTraitsT, typename StorageT>
inline std::basic_ostream<ElemT, CharTraitsT> & operator << (std::basic_ostream<ElemT, CharTraitsT> & os, datetime<StorageT> const& sobj)
{
    return os << sobj.ticks();
}

}
