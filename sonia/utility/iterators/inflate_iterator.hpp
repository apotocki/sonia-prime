//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/span.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include "sonia/utility/zlib_util.hpp"

namespace sonia {

template <class IteratorT>
class inflate_iterator
    : public boost::iterator_facade<
          inflate_iterator<IteratorT>
        , std::span<const char>
        , std::output_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<inflate_iterator<IteratorT> const*, std::span<const char>>>
    >
{
    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<inflate_iterator const*, std::span<const char>>>;

    struct strm_data
    {
        IteratorT base_;
        z_stream strm_;
        Bytef * out_begin = nullptr;
        int ret_;

        strm_data(IteratorT, bool);
        ~strm_data();

        void set(std::span<char>);
        std::span<const char> get() const;

        bool empty() const;

        void inflate();
    };

    proxy_t dereference() const
    {
        return iterators::make_value_proxy<std::span<const char>>(this);
    }

    std::span<const char> get_dereference() const
    {
        return data_->get();
    }

    void set_dereference(std::span<char> rng)
    {
        data_->set(rng);
    }

    void increment()
    {
        if (empty()) {
            THROW_INTERNAL_ERROR("increment an empty iterator");
        }
        data_->inflate();
    }

public:
    inflate_iterator(IteratorT base, bool gzip)
    {
        data_ = std::make_unique<strm_data>(std::move(base), gzip);
    }

    inflate_iterator(inflate_iterator const&) = delete;
    inflate_iterator(inflate_iterator &&) = default;
    inflate_iterator& operator=(inflate_iterator const&) = delete;
    inflate_iterator& operator=(inflate_iterator &&) = default;

    bool empty() const { return data_->empty(); }

    IteratorT & base() const { return data_->base_; }

private:
    std::unique_ptr<strm_data> data_;
};

template <class IteratorT>
inflate_iterator<IteratorT>::strm_data::strm_data(IteratorT it, bool gzip)
    : base_{std::move(it)}
{
    strm_.zalloc = Z_NULL;
    strm_.zfree = Z_NULL;
    strm_.opaque = Z_NULL;
    strm_.next_in = strm_.next_out = nullptr;
    strm_.avail_in = strm_.avail_out = 0;

    ret_ = inflateInit2(&strm_, gzip ? 16 + MAX_WBITS : -15);

    if (ret_ != Z_OK) {
        THROW_INTERNAL_ERROR("inflate decompressor initialization error #%1% (%2%)"_fmt % ret_ % zlib_detail::err_to_str(ret_));
    }
}

template <class IteratorT>
inflate_iterator<IteratorT>::strm_data::~strm_data()
{
    inflateEnd(&strm_);
}

template <class IteratorT>
void inflate_iterator<IteratorT>::strm_data::set(std::span<char> d)
{
    strm_.avail_out = static_cast<uInt>(d.size());
    out_begin = strm_.next_out = reinterpret_cast<Bytef*>(d.data());
}

template <class IteratorT>
std::span<const char> inflate_iterator<IteratorT>::strm_data::get() const
{
    return {reinterpret_cast<char*>(out_begin), static_cast<size_t>(strm_.next_out - out_begin)};
}

template <class IteratorT>
bool inflate_iterator<IteratorT>::strm_data::empty() const
{
    return ret_ < 0;
}

template <class IteratorT>
void inflate_iterator<IteratorT>::strm_data::inflate()
{
    if (BOOST_UNLIKELY(Z_STREAM_END == ret_)) {
        if (!base_.empty()) {
            std::span<const char> crng = *base_;
            const char* pos = reinterpret_cast<char*>(strm_.next_in);
            if (data_end(crng) == pos) {
                ++base_;
            } else {
                *base_ = std::span{pos, data_end(crng)};
            }
        }
        ret_ = Z_STREAM_ERROR;
        return;
    }

    if (!strm_.next_in) { // first call
        if (BOOST_UNLIKELY(base_.empty())) { // no input data => no output data
            ret_ = Z_STREAM_ERROR;
            return;
        }
        std::span<const char> crng = *base_;
        strm_.avail_in = static_cast<uInt>(crng.size());
        strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(crng.data()));
    }

    do {
        while (!strm_.avail_in && !base_.empty()) {
            ++base_;
            if (base_.empty()) break;
            std::span<const char> crng = *base_;
            strm_.avail_in = static_cast<uInt>(crng.size());
            strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(crng.data()));
        }

        int flag = strm_.avail_in ? Z_SYNC_FLUSH : Z_FINISH;
        ret_ = ::inflate(&strm_, flag);
        if (ret_ < 0) {
            throw exception("inflate decompressor error #%1% (%2%)"_fmt % ret_ % zlib_detail::err_to_str(ret_));
        } else if (Z_FINISH == flag && strm_.avail_out && Z_STREAM_END != ret_) {
            throw exception("insufficient input data to inflate");
        }
    } while (ret_ != Z_STREAM_END && strm_.avail_out);
}

}
