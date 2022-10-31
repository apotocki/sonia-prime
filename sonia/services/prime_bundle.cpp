//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "sonia/prime_config.hpp"

#include "prime_bundle.hpp"

#include "bundle.ipp"
#include "sonia/services/scheduler/scheduler_service_builder.hpp"
#include "sonia/services/auth/auth_service_builder.hpp"
#include "sonia/services/registry/registry_service_builder.hpp"
#include "sonia/services/bookkeeper/bookkeeper_service_builder.hpp"

#ifndef SONIA_NO_NET
#include "sonia/services/net/net_service_builder.hpp"
#include "sonia/services/net/echo/echo_connector_builder.hpp"
#include "sonia/services/net/http/http_connector_builder.hpp"
#include "sonia/services/net/http/http_default_application_builder.hpp"
#include "sonia/services/net/http/http_static_application_builder.hpp"
#include "sonia/services/net/http/http_digest_authentication_application_builder.hpp"
#include "sonia/services/transceiver/transceiver_builder.hpp"
#include "sonia/services/io/io_service_builder.hpp"
#include "sonia/services/io/io_cache_service_builder.hpp"

#ifndef SONIA_NO_SSL
#   include "sonia/services/io/io_ssl_service_builder.hpp"
#endif

#endif
namespace sonia::services {

string_view prime_bundle::build_id() const noexcept { return BUILD_ID; }

void prime_bundle::init()
{
    install<scheduler_service_builder>("scheduler");
    install<auth_service_builder>("auth");
    install<registry_service_builder>("registry");
    install<bookkeeper_service_builder>("bookkeeper");
#ifndef SONIA_NO_NET
    install<net_service_builder>("net-server");
    install<http_connector_builder>("http-server");
    install<http_default_application_builder>("http-default");
    install<http_static_application_builder>("http-static");
    install<http_digest_authentication_application_builder>("http-auth");
    
    install<io_service_builder>("io");

    install<io_cache_service_builder>("io-cache");
    install<echo_connector_builder>("echo");
    install<transceiver_service_builder>("transceiver");

#   ifndef SONIA_NO_SSL
    install<io_ssl_service_builder>("io-ssl");
#   endif
#endif
}

}

#ifndef SONIA_PRIME_STATIC
extern "C" {

BOOST_SYMBOL_EXPORT sonia::services::bundle* get_bundle() {
    return new sonia::services::prime_bundle();
}

}

#endif
