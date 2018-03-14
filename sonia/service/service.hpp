//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_HPP
#define SONIA_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/logger/loggable.hpp"

namespace sonia {

class service : public virtual loggable
{
public:
    typedef uint32_t id_type;
    id_type id() const noexcept { return id_; }
    string name() const noexcept { return name_; }

private:
    id_type id_;
    string name_;
};

class service_registry {
public:
    virtual ~service_registry() {}

    virtual service::id_type get_id(string_view) const = 0;
    virtual string const* get_name(service::id_type) const = 0;
};

class service_factory {
public:
    virtual ~service_factory() {}

    virtual shared_ptr<service> create(string_view) const = 0;
};

}

#endif // SONIA_SERVICE_HPP
