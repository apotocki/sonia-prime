//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_BZ2_COMPRESS_ITERATOR_HPP
#define SONIA_UTILITY_BZ2_COMPRESS_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "sonia/array_view.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include "bzlib.h"

namespace sonia {

template <class IteratorT>
class bz2_compress_iterator
	: public boost::iterator_facade<
		  bz2_compress_iterator<IteratorT>
		, void
		, std::output_iterator_tag
		, wrapper_iterator_proxy<ptr_proxy_wrapper<bz2_compress_iterator<IteratorT> const*, void>>
	>
{
	friend class boost::iterator_core_access;
	template <class, class> friend class ptr_proxy_wrapper;

	struct strm_data
	{
		IteratorT base_;
        bz_stream strm_;
		char * out_begin = nullptr;
		int ret_;

		explicit strm_data(IteratorT, int blockSize100k, int workFactor);
		~strm_data();

		void set(array_view<const char>);
		array_view<const char> get() const;

		void deflate(int flag = BZ_RUN);
		void flush();
	};

	decltype(auto) dereference() const
	{
		return sonia::make_proxy(this);
	}

	void set_dereference(array_view<const char> rng)
	{
		data_->set(rng);
	}

	void increment()
	{
		data_->deflate();
	}

public:
	explicit bz2_compress_iterator(IteratorT base, int blockSize100k = 9, int workFactor = 4)
	{
		data_ = make_shared<strm_data>(std::move(base), blockSize100k, workFactor);
	}

	void flush()
	{
		data_->flush();
	}

private:
	shared_ptr<strm_data> data_;
};

template <class IteratorT>
bz2_compress_iterator<IteratorT>::strm_data::strm_data(IteratorT it, int blockSize100k, int workFactor)
	: base_(std::move(it))
{
    std::memset(&strm_, 0, sizeof(bz_stream));

	ret_ = BZ2_bzCompressInit(&strm_, blockSize100k, 0, workFactor);

	if (ret_ != BZ_OK) {
		BOOST_THROW_EXCEPTION(internal_error("bz2 compressor initialization error #%1%"_fmt % ret_));
	}
}

template <class IteratorT>
bz2_compress_iterator<IteratorT>::strm_data::~strm_data()
{
    flush();
    BZ2_bzCompressEnd(&strm_);
}

template <class IteratorT>
void bz2_compress_iterator<IteratorT>::strm_data::set(array_view<const char> d)
{
	strm_.avail_in = static_cast<unsigned int>(d.size());
	strm_.next_in = const_cast<char*>(d.empty() ? nullptr : &d.front());
}

template <class IteratorT>
array_view<const char> bz2_compress_iterator<IteratorT>::strm_data::get() const
{
	return {reinterpret_cast<char*>(out_begin), static_cast<size_t>(strm_.next_out - out_begin)};
}

template <class IteratorT>
void bz2_compress_iterator<IteratorT>::strm_data::deflate(int flag)
{
	if (!strm_.next_out) { // first call
		array_view<char> outrng = *base_;
		strm_.avail_out = static_cast<unsigned int>(outrng.size());
		strm_.next_out = outrng.begin();
	}

	while (strm_.avail_in || (flag == BZ_FINISH && ret_ != BZ_STREAM_END)) {
		while (!strm_.avail_out) {
			++base_;
			array_view<char> outrng = *base_;
			strm_.avail_out = static_cast<unsigned int>(outrng.size());
			strm_.next_out = outrng.begin();
		}

		ret_ = BZ2_bzCompress(&strm_, flag);
		if (ret_ < 0) {
			throw exception("bz2 compressor error #"_fmt % ret_);
		}

		BOOST_ASSERT(!strm_.avail_in || !strm_.avail_out);
	}
}

template <class IteratorT>
void bz2_compress_iterator<IteratorT>::strm_data::flush()
{
    if (strm_.next_out) {
        deflate(BZ_FINISH);
        array_view<char> outrng = *base_;
        *base_ = array_view(outrng.begin(), reinterpret_cast<char*>(strm_.next_out));
        strm_.next_out = nullptr;
        strm_.avail_out = 0;
        if constexpr (iterators::has_method_flush_v<IteratorT, void()>) {
            base_.flush();
        }
    }
}

}

#endif // SONIA_UTILITY_BZ2_COMPRESS_ITERATOR_HPP
