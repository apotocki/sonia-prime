//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "sonia/prime_config.hpp"

#include "bundle.ipp"
#include "sonia/services/scheduler/scheduler_service_builder.hpp"
#include "sonia/services/net/net_service_builder.hpp"
#include "sonia/services/net/echo/echo_connector_builder.hpp"
#include "sonia/services/io/io_service_builder.hpp"

namespace sonia { namespace services {

class prime_bundle : public bundle
{
public:
    ~prime_bundle() override final {}

    string_view build_id() const noexcept override { return BUILD_ID; }

    void init() override
    {
        install<scheduler_service_builder>("scheduler");
        install<net_service_builder>("net-server");
        install<io_service_builder>("io");
        install<echo_connector_builder>("echo");
    }
};

}}

#ifndef SONIA_PRIME_STATIC
extern "C" {

BOOST_SYMBOL_EXPORT sonia::services::bundle* get_bundle() {
    return new sonia::services::prime_bundle();
}

}

#endif
