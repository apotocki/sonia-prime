//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include "sonia/type_traits.hpp"

namespace sonia {

template <class WrapperT>
class wrapper_iterator_proxy
{
public:
	using reference = typename WrapperT::reference;

	template <typename ArgT>
	explicit wrapper_iterator_proxy(ArgT && arg) : wr_(std::forward<ArgT>(arg)) {}

	wrapper_iterator_proxy(wrapper_iterator_proxy &&) = default;
	wrapper_iterator_proxy(wrapper_iterator_proxy const&) = delete;
	wrapper_iterator_proxy & operator = (wrapper_iterator_proxy const&) = delete;
	wrapper_iterator_proxy & operator = (wrapper_iterator_proxy &&) = delete;

	template <typename RT = reference>
	operator enable_if_t<is_same_v<RT, reference> && !is_void_v<RT>, reference>() const { return wr_.get(); }

	template <typename ArgT>
	wrapper_iterator_proxy const& operator = (ArgT && arg)
	{
		wr_.set(std::forward<ArgT>(arg));
		return *this;
	}

private:
	WrapperT wr_;
};

template <class WrappedPtrT, typename ReferenceT>
class ptr_proxy_wrapper
{
public:
	using reference = ReferenceT;

	explicit ptr_proxy_wrapper(WrappedPtrT ptr) : ptr_(ptr) {}

	template <typename ArgT> void set(ArgT && arg)
	{
		if constexpr (is_const_v<remove_pointer_t<WrappedPtrT>>) {
			using ptr_t = remove_cv_t<remove_pointer_t<WrappedPtrT>> *;
			const_cast<remove_cv_t<ptr_t>>(ptr_)->set_dereference(std::forward<ArgT>(arg));
		} else {
			ptr_->set_dereference(std::forward<ArgT>(arg));
		}
	}

	template <typename RT = reference>
	enable_if_t<is_same_v<RT, reference> && !is_void_v<RT>, reference> get() const
	{
		return ptr_->get_dereference();
	}

private:
	WrappedPtrT ptr_;
};

namespace iterators {

template <typename ReferenceT = void, class T>
wrapper_iterator_proxy<ptr_proxy_wrapper<T*, ReferenceT>> make_value_proxy(T * ptr)
{
    return wrapper_iterator_proxy<ptr_proxy_wrapper<T*, ReferenceT>>(ptr);
}

}}
