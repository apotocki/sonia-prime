//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/logger/loggable.hpp"

#include "sonia/services/singleton.hpp"

namespace sonia {

struct service_access;

class service 
    : public singleton
    , public virtual loggable
{
    friend struct service_access;

public:
    service() : singleton(0) {}

    ~service() noexcept override
    {
        LOG_TRACE(logger()) << "terminated";
    }

    virtual void open() {}
};

struct service_access : singleton_access
{
    static void set_name(service & s, std::string nameval) { singleton_access::set_name(s, std::move(nameval)); }
    static void set(service & s, service::id idval, std::string nameval) { singleton_access::set_id(s, idval); singleton_access::set_name(s, std::move(nameval)); }
};

class service_registry
{
public:
    virtual ~service_registry() = default;

    virtual service::id get_id(string_view) = 0;
    virtual string_view get_name(service::id) const = 0; // throws an exception if the name is undefined for the given id.
};

class service_factory
{
public:
    virtual ~service_factory() = default;

    virtual shared_ptr<service> create(string_view) const = 0;
};

}
