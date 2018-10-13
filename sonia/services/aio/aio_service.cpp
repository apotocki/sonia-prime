//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "aio_service.hpp"

namespace sonia { namespace services {

aio_service::aio_service(aio_configuration const& cfg)
    : cfg_(cfg)
{
    set_attribute("Type", "aio");
}

void aio_service::open() {
    sonia::io::factory::open(cfg_.threads);
}

void aio_service::close() noexcept {
    sonia::io::factory::close();
}

}}
