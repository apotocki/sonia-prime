//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>
#include "sonia/function.hpp"
#include "sonia/utility/functional/hash.hpp"
#include "service.hpp"
#include "builder.hpp"

namespace sonia::services {

struct bundle_configuration
{
    std::string lib;
    int layer;
};

class bundle 
    : public service
    , public services::builder
{
public:
    using service_factory_ftor_t = function<shared_ptr<service>(json_object const&)>;

    bundle();
    ~bundle() override = default;

    void open() override;

    virtual string_view build_id() const noexcept = 0;
    virtual void init() = 0;

    void set_handle(void * h) { libhandle_ = h; }

    shared_ptr<service> build(service_configuration const&) override;

protected:
    template <typename ServiceT>
    void install(string_view nm);

protected:
    void * libhandle_;
    boost::unordered_map<std::string, service_factory_ftor_t, hasher> builders_;
};

}
