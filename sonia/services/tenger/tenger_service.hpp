//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/services/service.hpp"
#include "tenger_service_configuration.hpp"

namespace sonia::services {

class tenger_service
    : public service
{
public:
    explicit tenger_service(tenger_service_configuration const&);
    void open() override;
    
private:
    tenger_service_configuration cfg_;
};

}
