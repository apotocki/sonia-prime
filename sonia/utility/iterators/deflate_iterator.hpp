//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/span.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

#include "zlib.h"

namespace sonia {

template <class IteratorT>
class deflate_iterator
{
	template <class, class> friend class ptr_proxy_wrapper;

public:
	using value_type = void;
	using pointer = void;
	using reference = wrapper_iterator_proxy<ptr_proxy_wrapper<deflate_iterator<IteratorT> const*, void>>;
	using difference_type = ptrdiff_t;
	using iterator_category = std::output_iterator_tag;

	reference operator*() const
	{
		return iterators::make_value_proxy(this);
	}

	deflate_iterator& operator++()
	{
		data_->deflate();
		return *this;
	}

private:
	struct strm_data
	{
		IteratorT base_;
		z_stream strm_;
		Bytef * out_begin = nullptr;
		int ret_;

		strm_data(IteratorT, bool);
		~strm_data();

		void set(std::span<const char>);
		std::span<const char> get() const;

		void deflate(int flag = Z_NO_FLUSH);
		void close();
	};

	void set_dereference(std::span<const char> rng)
	{
		data_->set(rng);
	}

public:
	deflate_iterator(IteratorT base, bool gzip)
	{
		data_ = make_shared<strm_data>(std::move(base), gzip);
	}

	void close()
	{
		data_->close();
	}

    bool empty() const { return !data_->strm_.next_out; }

private:
	shared_ptr<strm_data> data_;
};

template <class IteratorT>
deflate_iterator<IteratorT>::strm_data::strm_data(IteratorT it, bool gzip)
    : base_{std::move(it)}
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
	deflateEnd(&strm_);
}

template <class IteratorT>
void deflate_iterator<IteratorT>::strm_data::set(std::span<const char> d)
{
	strm_.avail_in = static_cast<uInt>(d.size());
	strm_.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(d.data()));
}

template <class IteratorT>
std::span<const char> deflate_iterator<IteratorT>::strm_data::get() const
{
	return {reinterpret_cast<char*>(out_begin), static_cast<size_t>(strm_.next_out - out_begin)};
}

template <class IteratorT>
void deflate_iterator<IteratorT>::strm_data::deflate(int flag)
{
	if (!strm_.next_out) { // first call
		if (sonia::empty(base_)) return;
		std::span<char> outrng = *base_;
		strm_.avail_out = static_cast<uInt>(outrng.size());
		strm_.next_out = reinterpret_cast<Bytef*>(outrng.data());
	}

	while (strm_.avail_in || (flag == Z_FINISH && ret_ != Z_STREAM_END)) {
		while (!strm_.avail_out) {
			++base_;
			if (sonia::empty(base_)) {
				strm_.next_out = nullptr;
				return;
			}
			std::span<char> outrng = *base_;
			strm_.avail_out = static_cast<uInt>(outrng.size());
			strm_.next_out = reinterpret_cast<Bytef*>(outrng.data());
		}

		ret_ = ::deflate(&strm_, flag);
		if (ret_ < 0) {
			throw exception("deflate compressor error #%1%"_fmt % ret_);
		}

		BOOST_ASSERT(!strm_.avail_in || !strm_.avail_out);
	}
}

template <class IteratorT>
void deflate_iterator<IteratorT>::strm_data::close()
{
    if (strm_.next_out) {
        deflate(Z_FINISH);
		std::span<char> outrng = *base_;
        if (reinterpret_cast<char*>(strm_.next_out) != data_end(outrng)) {
            *base_ = std::span(reinterpret_cast<char*>(strm_.next_out), data_end(outrng));
        } else {
            ++base_;            
        }
        strm_.next_out = nullptr;
        strm_.avail_out = 0;
        
        if constexpr (iterators::has_method_close_v<IteratorT, void()>) {
            base_.close();
        }
    }
}

}
