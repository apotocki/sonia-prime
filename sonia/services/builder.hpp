//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_BUILDER_HPP
#define SONIA_SERVICE_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>

#include "service.hpp"
#include "sonia/utility/json/value.hpp"
#include "sonia/utility/parameters/parameters.hpp"

namespace sonia::services {

struct service_configuration
{
    std::string factory;
    int layer{0};
    json_object parameters;
};

class builder
{
public:
    virtual ~builder() = default;
    virtual shared_ptr<service> build(service_configuration const&) = 0;
};

template <class ServT, class ConfT>
class basic_builder 
    : public service
    , public builder
{
public:
    shared_ptr<service> build(service_configuration const& scfg) override;

protected:
    sonia::parameters::parameters_description<ConfT> parameters_;
};

}

#define DECLARE_PARTICULAR_BUILDER(name)                                         \
class name;                                                                      \
struct name##_configuration;                                                     \
class name##_builder                                                             \
    : public sonia::services::basic_builder<name, name##_configuration>          \
{                                                                                \
public:                                                                          \
    name##_builder();                                                            \
};

#endif // SONIA_SERVICE_BUILDER_HPP
