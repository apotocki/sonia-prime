//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "type_registry.hpp"
#include "sonia/utility/basic_local_registry.hpp"

namespace sonia {

class local_type_registry 
    : public type_registry
    , utility::basic_local_registry<local_type_registry, uint32_t>
{
    typedef utility::basic_local_registry<local_type_registry, uint32_t> base_t;
    friend base_t;

public:
    explicit local_type_registry(shared_ptr<persister> sp);

    uint32_t get_type_id(std::string_view, std::string_view) override;
    std::pair<std::string_view, std::string_view> get_type_description(uint32_t) const override;

    persister & get_state_persister() const { return *state_persister_; }

private:
    shared_ptr<persister> state_persister_;
};

}
