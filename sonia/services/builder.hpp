//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

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
    explicit basic_builder(json_object const& props) : props_{props}
    {
        set_log_attribute("Type", "builder");
    }

    shared_ptr<service> build(service_configuration const& scfg) override
    {
        ConfT cfg;
        parameters_.apply(scfg.parameters, &cfg);
        auto r = sonia::make_shared<ServT>(cfg);
        service_access::set_layer(*r, scfg.layer);
        return r;
    }

protected:
    json_object props_;
    parameters::parameters_description<ConfT> parameters_;

    using basic_builder_t = basic_builder;
};

}

#define DECLARE_PARTICULAR_CUSTOM_BUILDER(builder_name, service_name, cfg_name)   \
class builder_name                                                                \
    : public sonia::services::basic_builder<service_name, cfg_name>               \
{                                                                                 \
public:                                                                           \
    explicit builder_name(json_object const& props) : basic_builder_t{props} {}   \
    void open() override final;                                                   \
};

#define DECLARE_PARTICULAR_BUILDER(name)                                          \
DECLARE_PARTICULAR_CUSTOM_BUILDER(name##_builder, name, name##_configuration)
