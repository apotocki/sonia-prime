//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_set.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"
#include "sonia/services/service.hpp"
#include "sonia/utility/file_statable.ipp"
#include "sonia/utility/serialization/boost/json_value.hpp"
#include "authentication.hpp"
#include "auth_service_configuration.hpp"

namespace sonia::services {

class auth_service
    : public authentication::digest_provider
    , public service
    , public utility::file_statable<auth_service>
{
    friend class boost::serialization::access;

    using statable_t = utility::file_statable<auth_service>;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int = 0)
    {
        ar & boost::serialization::make_nvp("registry", map_);
    }

public:
    explicit auth_service(auth_service_configuration const&);
    void open() override;
    
    path_ref_t get_state_path() const { if (cfg_.path) return *cfg_.path; else return statable_t::get_state_path(); }
    path_ref_t get_state_dir() const { if (cfg_.dir) return *cfg_.dir; else return statable_t::get_state_dir(); }

    optional<std::string> get_digest_for(std::string_view) const override final;

    using statable_t::backup;
    using statable_t::restore;

private:
    auth_service_configuration cfg_;

    struct user_data
    {
        std::string id;
        mutable std::string digest;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int = 0)
        {
            ar & boost::serialization::make_nvp("id", id)
               & boost::serialization::make_nvp("digest", digest);
        }

        bool operator==(user_data const& rhs) const { return id == rhs.id; }
    };

    struct user_data_hasher
    {
        size_t operator()(user_data const& ud) const { return hasher()(ud.id); }
    };

    struct user_data_equal_to
    {
        bool operator()(user_data const& ud, std::string_view s) const { return ud.id == s; }
        bool operator()(std::string_view s, user_data const& ud) const { return ud.id == s; }
    };

    boost::unordered_set<user_data, user_data_hasher> map_;
    mutable fibers::mutex mtx_;
};

}
