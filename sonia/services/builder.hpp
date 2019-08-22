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
    explicit basic_builder(json_value const& props) : props_{props}
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
    json_value props_;
    sonia::parameters::parameters_description<ConfT> parameters_;

    using basic_builder_t = basic_builder;
};

}

#define DECLARE_PARTICULAR_BUILDER(name)                                         \
class name##_builder                                                             \
    : public sonia::services::basic_builder<name, name##_configuration>          \
{                                                                                \
public:                                                                          \
    explicit name##_builder(json_value const& props) : basic_builder_t{props} {} \
    void open() override final;                                                  \
};

#endif // SONIA_SERVICE_BUILDER_HPP
