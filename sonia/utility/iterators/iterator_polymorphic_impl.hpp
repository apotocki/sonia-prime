//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ITERATOR_POLYMORPHIC_IMPL_HPP
#define SONIA_UTILITY_ITERATOR_POLYMORPHIC_IMPL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

#include <boost/throw_exception.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/array_view.hpp"

namespace sonia {

class file_chunk_input_iterator
    : public boost::iterator_facade<
        file_chunk_input_iterator,
        array_view<char>,
        std::random_access_iterator_tag,
        array_view<char>
    >
{
    friend class boost::iterator_core_access;

    bool equal(file_chunk_input_iterator const& rhs) const {
        return impl_.equal(rhs.impl_);
    }

    void increment() {
        impl_.increment();
    }

    void decrement() {
        impl_.decrement();
    }

    ReferenceT dereference() const {
        return impl_.dereference();
    }

    void advance(std::ptrdiff_t dif) {
        impl_.advance(dif);
    }
};

template <
      class ReferenceT
    , class DifferenceT = std::ptrdiff_t
>
class iterator_polymorphic_impl
{
public:
    virtual ~iterator_polymorphic_impl() noexcept {}

    virtual bool equal(iterator_polymorphic_impl const& rhs) const {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::equal"));
    }

    virtual void increment() {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::increment"));
    }

    virtual void decrement() {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::decrement"));
    }

    virtual ReferenceT dereference() const {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::dereference"));
    }

    virtual void advance(DifferenceT dif) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::advance"));
    }

    virtual size_t get_sizeof() {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::get_sizeof"));
    }

    virtual void clone(void * address, size_t sz) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::clone"));
    }

    virtual void move(void * address, size_t sz) {
        BOOST_THROW_EXCEPTION(not_implemented_error("iterator_polymorphic_impl::move"));
    }
};

}

#endif // SONIA_UTILITY_ITERATOR_POLYMORPHIC_IMPL_HPP
