//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services.hpp"
#include "auth_service.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/base_xx_util.hpp"

#include <boost/uuid/detail/md5.hpp>

namespace sonia::services {

using boost::uuids::detail::md5;

auth_service::auth_service(auth_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "auth_service");
}

void auth_service::open()
{
    statable_t::open();
    restore();

    auto authenticator = locate<authentication::authenticator>(cfg_.authenticator_app);
    for (auto const& u : cfg_.bootstrap_users) {
        auto it = map_.find(u.first, hasher{}, user_data_equal_to{});
        if (it != map_.end()) {
            it->digest = authenticator->get_digest_for(u.first, u.second);
        } else {
            map_.insert(it, user_data{u.first, authenticator->get_digest_for(u.first, u.second)});
        }
    }
    backup();
}

optional<std::string> auth_service::get_digest_for(std::string_view uid) const
{
    lock_guard guard(mtx_);
    auto it = map_.find(uid, hasher{}, user_data_equal_to{});
    return (it != map_.end()) ? optional<std::string>{it->digest} : nullopt;
}



}
