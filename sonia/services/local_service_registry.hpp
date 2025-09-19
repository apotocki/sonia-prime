//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/services/service.hpp"
#include "sonia/utility/basic_local_registry.hpp"

namespace sonia {

class local_service_registry 
    : public service_registry
    , utility::basic_local_registry<local_service_registry, service::id>
{
    using base_t = utility::basic_local_registry<local_service_registry, service::id>;
    friend base_t;

public:
    explicit local_service_registry(shared_ptr<persister> sp);

    service::id get_id(std::string_view) override;
    std::string_view get_name(service::id) const override;

    persister & get_state_persister() const { return *state_persister_; }

private:
    shared_ptr<persister> state_persister_;
};

}
