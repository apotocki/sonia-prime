//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_ELEMENT_HPP
#define SONIA_ELEMENT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/throw_exception.hpp>

#include "prime_config.hpp"
#include "cstdint.hpp"
#include "string.hpp"
#include "shared_ptr.hpp"

namespace sonia {

typedef uint32_t eid_t;

class element {
public:
    virtual ~element() noexcept {};

    eid_t id() const noexcept { return id_; }
    string name() const noexcept { return name_; }

private:
    eid_t id_;
    string name_;
};

SONIA_PRIME_API shared_ptr<element> get_element(string_view);

shared_ptr<element> get_element(eid_t);

template <class ElementT>
shared_ptr<ElementT> get_element(string_view sv) {
    shared_ptr<ElementT> rval = dynamic_pointer_cast<ElementT>(get_element(sv));
    if (!rval) {
        throw exception(std::ostringstream() << errss << "'" << sv << "' element is not compatible with " << typeid(ElementT).name());
    }
}

}

#endif // SONIA_ELEMENT_HPP
