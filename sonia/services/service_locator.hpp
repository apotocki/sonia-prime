//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_LOCATOR_HPP
#define SONIA_SERVICE_LOCATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "singleton_locator.hpp"
#include "service.hpp"

namespace sonia {

class service_locator 
    : public singleton_locator
{
public:
    service_locator(shared_ptr<service_registry> sr, shared_ptr<service_factory> sf);
    ~service_locator() noexcept;

    using singleton_locator::get;
    shared_ptr<service> get(string_view name);
    shared_ptr<service> get(service::id id);

private:
    shared_ptr<service> get(service::id id, string_view name);

    shared_ptr<service_registry> sr_;
    shared_ptr<service_factory> sf_;
};

}

#endif // SONIA_SERVICE_LOCATOR_HPP
