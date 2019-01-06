//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_DEFLATE_ITERATOR_HPP
#define SONIA_UTILITY_DEFLATE_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "sonia/array_view.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include "zlib.h"

namespace sonia {

template <class IteratorT>
class deflate_iterator
	: public boost::iterator_facade<
		  deflate_iterator<IteratorT>
		, void
		, std::output_iterator_tag
		, wrapper_iterator_proxy<ptr_proxy_wrapper<deflate_iterator<IteratorT> const*, void>>
	>
{
	friend class boost::iterator_core_access;
	template <class, class> friend class ptr_proxy_wrapper;

	struct strm_data
	{
		IteratorT base_;
		z_stream strm_;
		Bytef * out_begin = nullptr;
		int ret_;

		strm_data(IteratorT, bool);
		~strm_data();

		void set(array_view<const char>);
		array_view<const char> get() const;

		void deflate(int flag = Z_SYNC_FLUSH);
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
	deflate_iterator(IteratorT base, bool gzip)
	{
		data_ = make_shared<strm_data>(std::move(base), gzip);
	}

	void flush()
	{
		data_->flush();
	}

private:
	shared_ptr<strm_data> data_;
};

template <class IteratorT>
deflate_iterator<IteratorT>::strm_data::strm_data(IteratorT it, bool gzip)
	: base_(std::move(it))
{
	strm_.zalloc = Z_NULL;
	strm_.zfree = Z_NULL;
	strm_.opaque = Z_NULL;
	strm_.avail_in = strm_.avail_out = 0;
	strm_.next_out = nullptr;

	ret_ = deflateInit2(&strm_, 9, Z_DEFLATED, gzip ? 16 + MAX_WBITS : -15, 9, Z_DEFAULT_STRATEGY);

	if (ret_ != Z_OK) {
		BOOST_THROW_EXCEPTION(internal_error("deflate compressor initialization error #%1%"_fmt % ret_));
	}
}

template <class IteratorT>
deflate_iterator<IteratorT>::strm_data::~strm_data()
{
    flush();
	deflateEnd(&strm_);
}

template <class IteratorT>
void deflate_iterator<IteratorT>::strm_data::set(array_view<const char> d)
{
	strm_.avail_in = static_cast<uInt>(d.size());
	strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(d.empty() ? nullptr : &d.front()));
}

template <class IteratorT>
array_view<const char> deflate_iterator<IteratorT>::strm_data::get() const
{
	return {reinterpret_cast<char*>(out_begin), static_cast<size_t>(strm_.next_out - out_begin)};
}

template <class IteratorT>
void deflate_iterator<IteratorT>::strm_data::deflate(int flag)
{
	if (!strm_.next_out) { // first call
		array_view<char> outrng = *base_;
		strm_.avail_out = static_cast<uInt>(outrng.size());
		strm_.next_out = reinterpret_cast<Bytef*>(outrng.begin());
	}

	while (strm_.avail_in || (flag == Z_FINISH && ret_ != Z_STREAM_END)) {
		while (!strm_.avail_out) {
			++base_;
			array_view<char> outrng = *base_;
			strm_.avail_out = static_cast<uInt>(outrng.size());
			strm_.next_out = reinterpret_cast<Bytef*>(outrng.begin());
		}

		ret_ = ::deflate(&strm_, flag);
		if (ret_ < 0) {
			throw exception("deflate compressor error #%1%"_fmt % ret_);
		}

		BOOST_ASSERT(!strm_.avail_in || !strm_.avail_out);
	}
}

template <class IteratorT>
void deflate_iterator<IteratorT>::strm_data::flush()
{
    if (strm_.next_out) {
        deflate(Z_FINISH);
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

#endif // SONIA_UTILITY_DEFLATE_ITERATOR_HPP
