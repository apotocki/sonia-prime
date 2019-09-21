//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
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

    for (auto const& u : cfg_.bootstrap_users) {
        auto it = map_.find(u.first, hasher{}, user_data_equal_to{});
        if (it != map_.end()) {
            it->digest = get_digest_for(u.first, u.second);
        } else {
            map_.insert(it, user_data{u.first, get_digest_for(u.first, u.second)});
        }
    }
    backup();
}

optional<std::string> auth_service::get_digest_for(string_view uid) const
{
    lock_guard guard(mtx_);
    auto it = map_.find(uid, hasher{}, user_data_equal_to{});
    return (it != map_.end()) ? optional<std::string>{it->digest} : nullopt;
}

std::string auth_service::get_digest_for(string_view user, string_view password) const
{
    md5 hash;
    md5::digest_type digest;

    std::ostringstream digest_steam_src;
    digest_steam_src << user << ':' << cfg_.digest_realm << ':' << password;
    std::string digest_src = digest_steam_src.str();
    hash.process_bytes(digest_src.data(), digest_src.size());
    hash.get_digest(digest);
    const auto digest_bytes = reinterpret_cast<const char *>(&digest);
    
    static_assert(sizeof(md5::digest_type) == 16);

    std::string result;
    result.reserve(32);
    base16_encode(digest_bytes, digest_bytes + sizeof(md5::digest_type), std::back_inserter(result));
    return result;
}

string_view auth_service::get_realm() const
{
    return cfg_.digest_realm;
}

}
