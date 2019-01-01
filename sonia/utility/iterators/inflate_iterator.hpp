//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_INFLATE_ITERATOR_HPP
#define SONIA_UTILITY_INFLATE_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/throw_exception.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/array_view.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include "zlib.h"

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

        bool inflate(); // returns false in case of eof
    };

    proxy_t dereference() const
    {
        return make_proxy<array_view<const char>>(this);
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
        if (!data_->inflate()) {
            data_.reset();
        }
    }

public:
    inflate_iterator(IteratorT base, bool gzip)
    {
        data_ = make_shared<strm_data>(std::move(base), gzip);
    }

    bool empty() const { return !data_; }

private:
    shared_ptr<strm_data> data_;
};

template <class IteratorT>
inflate_iterator<IteratorT>::strm_data::strm_data(IteratorT it, bool gzip)
    : base_(std::move(it))
{
    strm_.zalloc = Z_NULL;
    strm_.zfree = Z_NULL;
    strm_.opaque = Z_NULL;
    strm_.next_in = strm_.next_out = nullptr;
    strm_.avail_in = strm_.avail_out = 0;

    ret_ = inflateInit2(&strm_, gzip ? 16 + MAX_WBITS : -15);

    if (ret_ != Z_OK) {
        BOOST_THROW_EXCEPTION(internal_error("inflate decompressor initialization error #%1%"_fmt % ret_));
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
bool inflate_iterator<IteratorT>::strm_data::inflate()
{
    if (Z_STREAM_END == ret_) return false;

    if (!strm_.next_in) { // first call
        if (base_.empty()) return false;
        array_view<const char> crng = *base_;
        strm_.avail_in = static_cast<uInt>(crng.size());
        strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(crng.begin()));
    }

    do {
        while (!strm_.avail_in) {
            ++base_;
            if (base_.empty()) break;
            array_view<const char> crng = *base_;
            strm_.avail_in = static_cast<uInt>(crng.size());
            strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(crng.begin()));
        }

        int flag = strm_.avail_in ? Z_SYNC_FLUSH : Z_FINISH;
        ret_ = ::inflate(&strm_, flag);
        if (ret_ < 0) {
            throw exception("inflate decompressor error #"_fmt % ret_);
        }
    } while (ret_ != Z_STREAM_END && strm_.avail_out);

    return true;
}

}

#endif // SONIA_UTILITY_INFLATE_ITERATOR_HPP
