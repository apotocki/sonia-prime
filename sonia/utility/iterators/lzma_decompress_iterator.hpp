//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/exceptions.hpp"
#include "sonia/span.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include <lzma.h>

namespace sonia {

template <class IteratorT>
class lzma_decompress_iterator
{
    template <class, class> friend class ptr_proxy_wrapper;

public:
    using value_type = std::span<const char>;
    using pointer = value_type*;
    using reference = wrapper_iterator_proxy<ptr_proxy_wrapper<lzma_decompress_iterator const*, std::span<const char>>>;
    using difference_type = ptrdiff_t;
    using iterator_category = std::output_iterator_tag;

    struct strm_data
    {
        IteratorT base_;
        lzma_stream strm_ = LZMA_STREAM_INIT;
        uint8_t* out_begin = nullptr;
        lzma_ret ret_ = LZMA_OK;

        explicit strm_data(IteratorT);
        ~strm_data();

        void set(std::span<char>);
        std::span<const char> get() const;

        bool empty() const;

        void inflate();
    };

    reference operator*() const
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

    void operator++()
    {
        if (empty()) {
            THROW_INTERNAL_ERROR("increment an empty iterator");
        }
        data_->inflate();
    }

public:
    explicit lzma_decompress_iterator(IteratorT base)
    {
        data_ = std::make_unique<strm_data>(std::move(base));
    }

    lzma_decompress_iterator(lzma_decompress_iterator const&) = delete;
    lzma_decompress_iterator(lzma_decompress_iterator &&) = default;
    lzma_decompress_iterator& operator=(lzma_decompress_iterator const&) = delete;
    lzma_decompress_iterator& operator=(lzma_decompress_iterator &&) = default;

    bool empty() const { return data_->empty(); }

    IteratorT & base() const { return data_->base_; }

private:
    std::unique_ptr<strm_data> data_;
};

template <class IteratorT>
lzma_decompress_iterator<IteratorT>::strm_data::strm_data(IteratorT it)
    : base_(std::move(it))
{
    lzma_ret ret = lzma_auto_decoder(&strm_, UINT64_MAX, LZMA_CONCATENATED);
    strm_.avail_in = 0;

    if (ret == LZMA_OK) return;
    switch (ret) {
    case LZMA_MEM_ERROR:
        throw std::runtime_error("Memory allocation failed");
    case LZMA_OPTIONS_ERROR:
        throw std::runtime_error("Unsupported decompressor flags");
    default:
        throw std::runtime_error("Unknown error, possibly a bug");
    }
}

template <class IteratorT>
lzma_decompress_iterator<IteratorT>::strm_data::~strm_data()
{
    lzma_end(&strm_);
}

template <class IteratorT>
void lzma_decompress_iterator<IteratorT>::strm_data::set(std::span<char> d)
{
    strm_.avail_out = d.size();
    out_begin = strm_.next_out = reinterpret_cast<uint8_t*>(d.data());
}

template <class IteratorT>
std::span<const char> lzma_decompress_iterator<IteratorT>::strm_data::get() const
{
    return {reinterpret_cast<char*>(out_begin), static_cast<size_t>(strm_.next_out - out_begin)};
}

template <class IteratorT>
bool lzma_decompress_iterator<IteratorT>::strm_data::empty() const
{
    return ret_ != LZMA_OK && ret_ != LZMA_STREAM_END;
}

template <class IteratorT>
void lzma_decompress_iterator<IteratorT>::strm_data::inflate()
{
    if (BOOST_UNLIKELY(LZMA_STREAM_END == ret_)) {
        if (!base_.empty()) {
            std::span<const char> crng = *base_;
            if (data_end(crng) == (const char*)strm_.next_in) {
                ++base_;
            } else {
                *base_ = std::span{(const char*)strm_.next_in, data_end(crng)};
            }
        }
        ret_ = LZMA_DATA_ERROR;
        return;
    }

    if (!strm_.next_in) { // first call
        if (BOOST_UNLIKELY(base_.empty())) { // no input data => no output data
            ret_ = LZMA_FORMAT_ERROR;
            return;
        }

        std::span<const char> crng = *base_;
        strm_.avail_in = crng.size();
        strm_.next_in = reinterpret_cast<const uint8_t*>(crng.data());
    }

    do {
        while (!strm_.avail_in && !base_.empty()) {
            ++base_;
            if (base_.empty()) break;
            std::span<const char> crng = *base_;
            strm_.avail_in = crng.size();
            strm_.next_in = reinterpret_cast<const uint8_t*>(crng.data());
        }

        bool no_input = !strm_.avail_in;
        ret_ = lzma_code(&strm_, no_input ? LZMA_FINISH : LZMA_RUN);
        if (ret_ != LZMA_OK && ret_ != LZMA_STREAM_END) {
            throw exception("bz2 decompressor error #%1% (%2%)"_fmt % ret_ % bz2_detail::err_to_str(ret_));
        } else if (no_input && strm_.avail_out && LZMA_STREAM_END != ret_) {
            throw exception("insufficient input data to decompress");
        }
    } while (ret_ != LZMA_STREAM_END && strm_.avail_out);
}

}
