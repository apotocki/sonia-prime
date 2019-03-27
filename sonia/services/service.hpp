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

class service_access;

class service : public virtual loggable
{
    friend class service_access;

public:
    service() : id_(0) {}

    ~service() override
    {
        LOG_TRACE(logger()) << "terminated";
    }

    using id = uint32_t;

    id get_id() const noexcept { return id_; }
    std::string const& get_name() const noexcept { return name_; }

    virtual void open() {}
    virtual void close() noexcept {}

private:
    std::string name_;
    id id_;
};

class service_access
{
public:
    static void set_id(service & s, service::id idval) { s.id_ = idval; }
    static void set_name(service & s, std::string nameval) { s.name_ = std::move(nameval); }
    static void set(service & s, service::id idval, std::string nameval) { s.id_ = idval; s.name_ = std::move(nameval); }
};

class service_registry
{
public:
    virtual ~service_registry() {}

    virtual service::id get_id(string_view) = 0;
    virtual string_view get_name(service::id) const = 0; // throws an exception if the name is undefined for the given id.
};

struct service_descriptor
{
    shared_ptr<service> serv;
    int layer;
};

class service_factory
{
public:
    virtual ~service_factory() noexcept {}

    virtual service_descriptor create(string_view) const = 0;
};

}

#endif // SONIA_SERVICE_HPP
