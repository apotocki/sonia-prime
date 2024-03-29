//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/span.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia { namespace http_form_data_read_iterator_detail {

template <typename SrcIteratorT, typename SampleIteratorT>
SrcIteratorT search_sample(SrcIteratorT bit,  SrcIteratorT eit, SampleIteratorT sample_bit, SampleIteratorT sample_eit)
{
    BOOST_ASSERT (!(sample_bit == sample_eit));

    while (bit != eit) {
        if (*bit == *sample_bit) {
            SrcIteratorT it1 = bit;
            SampleIteratorT it2 = sample_bit;
            ++it1, ++it2;
            for (;;) {
                if (it2 == sample_eit || it1 == eit) return bit;
                if (*it1 != *it2) break;
                ++it1, ++it2;
            }
        }
        ++bit;
    }
    return eit;
}

}

template <class IteratorT>
class http_form_data_read_iterator
{
public:
    using value_type = std::span<const char>;
    using iterator_category = not_more_traversal_t<forward_traversal_tag, iterator_traversal_t<IteratorT>>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<http_form_data_read_iterator const*, value_type>>;

private:
    enum class state : uint8_t
    {
        NEED_CHUNK,
        NEED_ANALIZE,
        READY_CHUNK,
        CHUNK_WITH_FULL_BOUNDARY,
        CHUNK_WITH_PARTIAL_BOUNDARY,
        CHUNK_STARTS_WITH_FULL_BOUNDARY,
        CHUNK_STARTS_WITH_PARTIAL_BOUNDARY,
        CHUNK_STARTS_WITH_PART_OF_FULL_BOUNDARY,
        CHUNK_IS_PART_OF_PARTIAL_BOUNDARY,
        PARTIAL_BOUNDARY_AS_CHUNK,
        EMPTY
    };

    void process()
    {
        for(;;) switch (state_)
        {
            case state::NEED_CHUNK:
                for(;;) {
                    if (base.empty()) {
                        throw exception("unexpected eof");
                    }
                    value_ = *base;
                    if (!value_.empty()) break;
                    ++base;
                }
            case state::NEED_ANALIZE:
                state_ = analize_chunk();
                break;
            case state::READY_CHUNK:
            case state::CHUNK_WITH_FULL_BOUNDARY:
            case state::CHUNK_WITH_PARTIAL_BOUNDARY:
            case state::PARTIAL_BOUNDARY_AS_CHUNK:
                return;
            case state::CHUNK_STARTS_WITH_FULL_BOUNDARY:
            case state::CHUNK_STARTS_WITH_PART_OF_FULL_BOUNDARY:
            {
                state_ = state::EMPTY;
                std::span<const char> tmp = *base;
                if (value_.begin() + (extboundary_.size() - partial_boundary_size_) != tmp.end()) {
                    *base = std::span(value_.begin() + (extboundary_.size() - partial_boundary_size_), tmp.end());
                } else {
                    ++base;
                }
                return;
            }
            case state::CHUNK_STARTS_WITH_PARTIAL_BOUNDARY:
            case state::CHUNK_IS_PART_OF_PARTIAL_BOUNDARY:
                ++base;
                state_ = state::NEED_CHUNK;
                break;
            default:
                THROW_INTERNAL_ERROR("unexpected state: "_fmt % (int)state_);
        }
    }

    state analize_chunk()
    {
        if (0 == partial_boundary_size_) {
            auto it = http_form_data_read_iterator_detail::search_sample(value_.begin(), value_.end(), extboundary_.begin(), extboundary_.end());
            if (it == value_.end()) return state::READY_CHUNK;
            size_t sz = static_cast<size_t>(value_.end() - it);
            //value_ = value_.subspan(0, value_.size() - sz);
            value_ = advance_back(value_, it - value_.end());
            if (sz >= extboundary_.size()) {
                return value_.begin() == it ? state::CHUNK_STARTS_WITH_FULL_BOUNDARY : state::CHUNK_WITH_FULL_BOUNDARY;
            } else {
                partial_boundary_size_ = sz;
                partial_offset_ = 0;
                return value_.begin() == it ? state::CHUNK_STARTS_WITH_PARTIAL_BOUNDARY : state::CHUNK_WITH_PARTIAL_BOUNDARY;
            }
        } else {
            size_t cmpsz = (std::min)(value_.size(), extboundary_.size() - partial_boundary_size_);
            if (!std::equal(value_.begin(), value_.begin() + cmpsz, extboundary_.begin() + partial_boundary_size_, extboundary_.begin() + partial_boundary_size_ + cmpsz)) {
                return state::PARTIAL_BOUNDARY_AS_CHUNK;
            }
            if (cmpsz + partial_boundary_size_ == extboundary_.size()) {
                return state::CHUNK_STARTS_WITH_PART_OF_FULL_BOUNDARY;
            }
            partial_boundary_size_ += cmpsz;
            return state::CHUNK_IS_PART_OF_PARTIAL_BOUNDARY;
        }
    }

public:
    http_form_data_read_iterator() : state_{state::EMPTY} {}

    explicit http_form_data_read_iterator(IteratorT it, std::span<const char> extboundary)
        : base{std::move(it)}, extboundary_{extboundary}, partial_boundary_size_{0}, state_{state::NEED_CHUNK}
    {
        process();
    }

    http_form_data_read_iterator(http_form_data_read_iterator const&) = delete;
    http_form_data_read_iterator(http_form_data_read_iterator &&) = default;
    http_form_data_read_iterator& operator=(http_form_data_read_iterator const&) = delete;
    http_form_data_read_iterator& operator=(http_form_data_read_iterator &&) = default;

    bool empty() const { return state_ == state::EMPTY; }

    bool operator==(http_form_data_read_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_t operator* () const
    {
        return iterators::make_value_proxy<value_type>(this);
    }

    std::span<const char> get_dereference() const
    {
        if (state_ != state::PARTIAL_BOUNDARY_AS_CHUNK) {
            return value_;
        }
        else {
            return std::span<const char>{extboundary_.begin() + partial_offset_, partial_boundary_size_ - partial_offset_};
        }
    }

    void set_dereference(std::span<const char> rng)
    {
        if (state_ != state::PARTIAL_BOUNDARY_AS_CHUNK) {
            BOOST_ASSERT(is_subset_of(rng, value_));
            if (rng.end() != value_.end()) THROW_NOT_IMPLEMENTED_ERROR();
            value_ = rng;
        }
        else {
            BOOST_ASSERT(is_subset_of(rng, extboundary_));
            if (rng.end() != extboundary_.end()) THROW_NOT_IMPLEMENTED_ERROR();
            partial_offset_ = static_cast<size_t>(rng.begin() - extboundary_.begin());
        }
    }

    http_form_data_read_iterator& operator++()
    {
        BOOST_ASSERT(!empty());
        switch (state_)
        {
        case state::CHUNK_WITH_FULL_BOUNDARY:
            value_ = value_.subspan(value_.size());
            state_ = state::CHUNK_STARTS_WITH_FULL_BOUNDARY;
            break;
        case state::READY_CHUNK:
        case state::CHUNK_WITH_PARTIAL_BOUNDARY:
            ++base;
            state_ = state::NEED_CHUNK;
            break;
        case state::PARTIAL_BOUNDARY_AS_CHUNK:
            partial_boundary_size_ = 0;
            state_ = state::NEED_ANALIZE;
            break;
        default:
            THROW_INTERNAL_ERROR("wrong state during the increment %1%"_fmt % (int)state_);
        }
        process();
        return *this;
    }

    IteratorT base;

private:
    std::span<const char> value_;
    std::span<const char> extboundary_;
    size_t partial_offset_, partial_boundary_size_;
    state state_;
};

}
