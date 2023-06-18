//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>

#include "prime_config.hpp"
#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/filesystem.hpp"
#include "sonia/services/service.hpp"
#include "sonia/services/host.hpp"

namespace sonia::this_thread {

SONIA_PRIME_API void attach_host(std::string_view);
SONIA_PRIME_API void detach_host();

}

namespace sonia::services {

class environment;

using post_initialize_fn = void(environment*);

SONIA_PRIME_API char const* bundles_path();

SONIA_PRIME_API void set_default_base_path(std::string);
SONIA_PRIME_API std::string const* get_default_base_path();

SONIA_PRIME_API void set_version_message(std::string const*);
SONIA_PRIME_API std::string const* get_version_message();

SONIA_PRIME_API void set_post_initialize(post_initialize_fn*);

SONIA_PRIME_API void initialize(int argc = 0, char const* argv[] = nullptr, std::istream * cfgstream = nullptr);
SONIA_PRIME_API void shutdown();
SONIA_PRIME_API void shutdown(int to_level); // including level

SONIA_PRIME_API shared_ptr<host> get_host();

SONIA_PRIME_API shared_ptr<service> locate(std::string_view);
SONIA_PRIME_API shared_ptr<service> locate(service::id);

template <class ServiceT, typename IdT>
shared_ptr<ServiceT> locate(IdT && id)
{
    shared_ptr<ServiceT> rval = dynamic_pointer_cast<ServiceT>(locate(std::forward<IdT>(id)));
    if (!rval) {
        throw exception("'%1%' service is not compatible with %2%"_fmt % id % typeid(ServiceT).name());
    }
    return std::move(rval);
}

template <class ServiceT, typename IdT>
void locate(IdT && id, shared_ptr<ServiceT> & serv)
{
    serv = locate<ServiceT>(std::forward<IdT>(id));
}

SONIA_PRIME_API void register_service_factory(std::string_view, function<shared_ptr<service>()> const&);
SONIA_PRIME_API void load_configuration(fs::path const &);
SONIA_PRIME_API void load_configuration(std::istream &);

}
