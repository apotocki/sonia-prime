//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

#include "sonia/span.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"
#include "sonia/utility/serialization/iostream.hpp"

namespace sonia {

class persister
{
public:
    virtual ~persister() = default;

    static const size_t iterator_size = 6 * sizeof(void*);

    using input_iterator = automatic_polymorphic_output_iterator<iterator_size, std::span<const char>, forward_traversal_tag, std::span<const char>>;
    using output_iterator = automatic_polymorphic_output_iterator<iterator_size, std::span<char>, forward_traversal_tag, std::span<char>>;
    
    virtual bool read(function<void(input_iterator)> const& ftor) const = 0; // returns false if persister was unable to read
    virtual void write(function<void(output_iterator)> const& ftor) = 0;

    template <typename FtorT>
    bool read_stream(FtorT const& ftor)
    {
        return read([&ftor](input_iterator ii) {
            serialization::istream<input_iterator, char> is(std::move(ii));
            ftor(is);
        });
    }

    template <typename FtorT>
    void write_stream(FtorT const& ftor)
    {
        write([&ftor](output_iterator oi) {
            serialization::ostream<output_iterator, char> os(std::move(oi));
            ftor(os);
        });
    }
};

}
