//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <utility>

namespace sonia {

template <typename SetT>
class bucket_set
{
    bucket_set(bucket_set const&) = delete;
    bucket_set& operator= (bucket_set const&) = delete;

public:
    using bucket_set_type = bucket_set;
    using set_t = SetT;
    using bucket_type = typename set_t::bucket_type;

    inline bucket_set() = default;

    inline explicit bucket_set(size_t sz) noexcept
        : buckets_(sz)
    {}

    inline bucket_set(bucket_set&& rhs) noexcept
        : buckets_(std::move(rhs.buckets_))
    {}

    inline void operator= (bucket_set&& rhs) noexcept
    {
        buckets_ = std::move(rhs.buckets_);
    }

    inline bucket_type* buckets() noexcept { return buckets_.data(); }

    inline void resize_buckets(size_t sz) noexcept { buckets_.resize(sz); }

    inline size_t size() const noexcept { return buckets_.size(); }

private:
    std::vector<bucket_type> buckets_;
};

}
