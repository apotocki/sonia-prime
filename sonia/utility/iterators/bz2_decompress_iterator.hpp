//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_BZ2_DECOMPRESS_ITERATOR_HPP
#define SONIA_UTILITY_BZ2_DECOMPRESS_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/array_view.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include "sonia/utility/bz2_util.hpp"

namespace sonia {

template <class IteratorT>
class bz2_decompress_iterator
    : public boost::iterator_facade<
          bz2_decompress_iterator<IteratorT>
        , array_view<const char>
        , std::output_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<bz2_decompress_iterator<IteratorT> const*, array_view<const char>>>
    >
{
    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<bz2_decompress_iterator const*, array_view<const char>>>;

    struct strm_data
    {
        IteratorT base_;
        bz_stream strm_;
        char * out_begin = nullptr;
        int ret_;

        explicit strm_data(IteratorT);
        ~strm_data();

        void set(array_view<char>);
        array_view<const char> get() const;

        bool empty() const;

        void inflate();
    };

    proxy_t dereference() const
    {
        return iterators::make_value_proxy<array_view<const char>>(this);
    }

    array_view<const char> get_dereference() const
    {
        return data_->get();
    }

    void set_dereference(array_view<char> rng)
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
    explicit bz2_decompress_iterator(IteratorT base)
    {
        data_ = std::make_unique<strm_data>(std::move(base));
    }

    bz2_decompress_iterator(bz2_decompress_iterator const&) = delete;
    bz2_decompress_iterator(bz2_decompress_iterator &&) = default;
    bz2_decompress_iterator& operator=(bz2_decompress_iterator const&) = delete;
    bz2_decompress_iterator& operator=(bz2_decompress_iterator &&) = default;

    bool empty() const { return data_->empty(); }

    IteratorT & base() const { return data_->base_; }

private:
    std::unique_ptr<strm_data> data_;
};

template <class IteratorT>
bz2_decompress_iterator<IteratorT>::strm_data::strm_data(IteratorT it)
    : base_(std::move(it))
{
    std::memset(&strm_, 0, sizeof(bz_stream));
    
    ret_ = BZ2_bzDecompressInit(&strm_, 0, 0);

    if (ret_ != BZ_OK) {
        THROW_INTERNAL_ERROR("bz2 decompressor initialization error #%1% (%2%)"_fmt % ret_ % bz2_detail::err_to_str(ret_));
    }
}

template <class IteratorT>
bz2_decompress_iterator<IteratorT>::strm_data::~strm_data()
{
    BZ2_bzDecompressEnd(&strm_);
}

template <class IteratorT>
void bz2_decompress_iterator<IteratorT>::strm_data::set(array_view<char> d)
{
    strm_.avail_out = static_cast<uInt>(d.size());
    out_begin = strm_.next_out = d.empty() ? nullptr : &d.front();
}

template <class IteratorT>
array_view<const char> bz2_decompress_iterator<IteratorT>::strm_data::get() const
{
    return {reinterpret_cast<char*>(out_begin), static_cast<size_t>(strm_.next_out - out_begin)};
}

template <class IteratorT>
bool bz2_decompress_iterator<IteratorT>::strm_data::empty() const
{
    return ret_ < 0;
}

template <class IteratorT>
void bz2_decompress_iterator<IteratorT>::strm_data::inflate()
{
    if (BOOST_UNLIKELY(BZ_STREAM_END == ret_)) {
        if (!base_.empty()) {
            array_view<const char> crng = *base_;
            if (crng.end() == strm_.next_in) {
                ++base_;
            } else {
                *base_ = array_view<const char>{strm_.next_in, crng.end()};
            }
        }
        ret_ = BZ_DATA_ERROR;
        return;
    }

    if (!strm_.next_in) { // first call
        if (BOOST_UNLIKELY(base_.empty())) { // no input data => no output data
            ret_ = BZ_UNEXPECTED_EOF;
            return;
        }

        array_view<const char> crng = *base_;
        strm_.avail_in = static_cast<unsigned int>(crng.size());
        strm_.next_in = const_cast<char*>(crng.begin());
    }

    do {
        while (!strm_.avail_in && !base_.empty()) {
            ++base_;
            if (base_.empty()) break;
            array_view<const char> crng = *base_;
            strm_.avail_in = static_cast<unsigned int>(crng.size());
            strm_.next_in = const_cast<char*>(crng.begin());
        }

        bool no_input = !strm_.avail_in;
        ret_ = BZ2_bzDecompress(&strm_);
        if (ret_ < 0) {
            throw exception("bz2 decompressor error #%1% (%2%)"_fmt % ret_ % bz2_detail::err_to_str(ret_));
        } else if (no_input && strm_.avail_out && BZ_STREAM_END != ret_) {
            throw exception("insufficient input data to decompress");
        }
    } while (ret_ != BZ_STREAM_END && strm_.avail_out);
}

}

#endif // SONIA_UTILITY_BZ2_DECOMPRESS_ITERATOR_HPP
