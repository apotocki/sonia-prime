//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

namespace sonia::services {

struct scheduler_service_configuration
{
    uint32_t threads;
    uint32_t fibers;
};

}
