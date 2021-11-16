//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/services/service.hpp"
#include "sonia/services/io/factory.hpp"
#include "sonia/services/io/ssl_factory.hpp"
#include "sonia/services/io/io_service_configuration.hpp"

namespace sonia::services {

class io_service 
    : public service
    , public sonia::io::factory
{
public:
    explicit io_service(io_service_configuration cfg);

    void open() override;
    void close() noexcept override;

private:
    io_service_configuration cfg_;
};

class io_ssl_service 
    : public service
    , public sonia::io::ssl_factory
{
public:
    explicit io_ssl_service(io_ssl_service_configuration cfg);

    void open() override;
    void close() noexcept override;

private:
    io_ssl_service_configuration cfg_;
};

}
