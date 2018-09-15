//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HPP
#define SONIA_SERVICES_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>
#include <boost/filesystem.hpp>

#include "prime_config.hpp"
#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services/service.hpp"

namespace sonia { namespace services {

SONIA_PRIME_API void initialize(int argc = 0, char const* argv[] = nullptr, std::istream * cfgstream = nullptr);
SONIA_PRIME_API void shutdown();

SONIA_PRIME_API shared_ptr<service> locate(string_view);
SONIA_PRIME_API shared_ptr<service> locate(service::id);

template <class ServiceT, typename IdT>
shared_ptr<ServiceT> locate(IdT id) {
    shared_ptr<ServiceT> rval = dynamic_pointer_cast<ServiceT>(locate(id));
    if (!rval) {
        throw exception("'%1%' service is not compatible with %2%"_fmt % id % typeid(ServiceT).name());
    }
    return std::move(rval);
}

SONIA_PRIME_API void register_service_factory(string_view, function<shared_ptr<service>()> const&);
SONIA_PRIME_API void load_configuration(boost::filesystem::path const &);
SONIA_PRIME_API void load_configuration(std::istream &);

}}

#endif // SONIA_SERVICES_HPP
