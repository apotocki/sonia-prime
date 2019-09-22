//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_INFLATE_ITERATOR_HPP
#define SONIA_UTILITY_INFLATE_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/array_view.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include "sonia/utility/zlib_util.hpp"

namespace sonia {

template <class IteratorT>
class inflate_iterator
    : public boost::iterator_facade<
          inflate_iterator<IteratorT>
        , array_view<const char>
        , std::output_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<inflate_iterator<IteratorT> const*, array_view<const char>>>
    >
{
    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<inflate_iterator const*, array_view<const char>>>;

    struct strm_data
    {
        IteratorT base_;
        z_stream strm_;
        Bytef * out_begin = nullptr;
        int ret_;

        strm_data(IteratorT, bool);
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
void inflate_iterator<IteratorT>::strm_data::set(array_view<char> d)
{
    strm_.avail_out = static_cast<uInt>(d.size());
    out_begin = strm_.next_out = reinterpret_cast<Bytef*>(d.empty() ? nullptr : &d.front());
}

template <class IteratorT>
array_view<const char> inflate_iterator<IteratorT>::strm_data::get() const
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
            array_view<const char> crng = *base_;
            const char* pos = reinterpret_cast<char*>(strm_.next_in);
            if (crng.end() == pos) {
                ++base_;
            } else {
                *base_ = array_view{pos, crng.end()};
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
        array_view<const char> crng = *base_;
        strm_.avail_in = static_cast<uInt>(crng.size());
        strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(crng.begin()));
    }

    do {
        while (!strm_.avail_in && !base_.empty()) {
            ++base_;
            if (base_.empty()) break;
            array_view<const char> crng = *base_;
            strm_.avail_in = static_cast<uInt>(crng.size());
            strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(crng.begin()));
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

#endif // SONIA_UTILITY_INFLATE_ITERATOR_HPP
