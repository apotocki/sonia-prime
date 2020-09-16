//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "sonia/concurrency.hpp"
#include "sonia/services/service.hpp"
#include "sonia/services/registry.hpp"
#include "sonia/utility/file_statable.ipp"
#include "sonia/utility/basic_local_registry.hpp"
#include "registry_service_configuration.hpp"

namespace sonia::services {

class registry_service
    : public registry
    , public service
    , public utility::file_statable<registry_service>
    , public utility::basic_local_registry<registry_service, uint32_t>
{
    using statable_t = utility::file_statable<registry_service>;
    using impl_t = utility::basic_local_registry<registry_service, uint32_t>;

public:
    explicit registry_service(registry_service_configuration const&);
    void open() override;
    
    path_ref_t get_state_path() const { if (cfg_.path) return *cfg_.path; else return statable_t::get_state_path(); }
    path_ref_t get_state_dir() const { if (cfg_.dir) return *cfg_.dir; else return statable_t::get_state_dir(); }

    uint32_t lookup(string_view s) override { return impl_t::get_id(s, {}); }
    std::string const* lookup(uint32_t id) const override { return impl_t::lookup_name(id); }

    persister & get_state_persister() const { return *statable_t::state_persister_; }

    using statable_t::backup;
    using statable_t::restore;
    using impl_t::bootstrap;
    using service::get_name;

private:
    registry_service_configuration cfg_;
};

}
