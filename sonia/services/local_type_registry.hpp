//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_LOCAL_TYPE_REGISTRY_HPP
#define SONIA_SERVICE_LOCAL_TYPE_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

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

    uint32_t get_type_id(string_view, string_view) override;
    std::pair<string_view, string_view> get_type_description(uint32_t) const override;

    persister & get_state_persister() const { return *state_persister_; }

private:
    shared_ptr<persister> state_persister_;
};

}

#endif // SONIA_SERVICE_LOCAL_TYPE_REGISTRY_HPP
