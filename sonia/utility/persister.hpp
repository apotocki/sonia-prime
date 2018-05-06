//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_PERSISTER_HPP
#define SONIA_UTILITY_PERSISTER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include "sonia/array_view.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"
#include "sonia/serialization/iostream.hpp"

namespace sonia {

class persister {
public:
    virtual ~persister() {}

    static const size_t iterator_size = 4 * sizeof(void*);

    typedef polymorphic_input_iterator<iterator_size, array_view<const uint8_t>, forward_traversal_tag, const array_view<const uint8_t>> input_iterator;
    typedef polymorphic_output_iterator<iterator_size, array_view<uint8_t>, forward_traversal_tag, array_view<uint8_t>> output_iterator;
    
    virtual bool read(function<void(input_iterator)> const& ftor) const = 0; // returns false if persister was unable to read
    virtual void write(function<void(output_iterator)> const& ftor) = 0;

    template <typename FtorT>
    bool read_stream(FtorT const& ftor) {
        return read([&ftor](input_iterator ii) {
            serialization::istream<input_iterator, char> is(std::move(ii));
            ftor(is);
        });
    }

    template <typename FtorT>
    void write_stream(FtorT const& ftor) {
        write([&ftor](output_iterator oi) {
            serialization::ostream<output_iterator, char> os(std::move(oi));
            ftor(os);
        });
    }
};

}

#endif // SONIA_UTILITY_PERSISTER_HPP
