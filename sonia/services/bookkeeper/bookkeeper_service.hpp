//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BOOKKEEPER_SERVICE_HPP
#define SONIA_BOOKKEEPER_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/unordered_map.hpp>
#include <boost/serialization/boost_unordered_map.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"
#include "sonia/services/service.hpp"
#include "sonia/utility/file_statable.ipp"
#include "sonia/utility/serialization/boost/json_value.hpp"
#include "bookkeeper.hpp"
#include "bookkeeper_service_configuration.hpp"

namespace sonia::services {

class bookkeeper_service
    : public bookkeeper
    , public service
    , public utility::file_statable<bookkeeper_service>
{
    friend class boost::serialization::access;

    using statable_t = utility::file_statable<bookkeeper_service>;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int = 0)
    {
        ar & boost::serialization::make_nvp("registry", map_);
    }

public:
    explicit bookkeeper_service(bookkeeper_service_configuration const&);
    void open() override;
    
    path_ref_t get_state_path() const { if (cfg_.path) return *cfg_.path; else return statable_t::get_state_path(); }
    path_ref_t get_state_dir() const { if (cfg_.dir) return *cfg_.dir; else return statable_t::get_state_dir(); }

    void set(string_view key, json_value const&) override;
    optional<json_value> get(string_view key) const override;
    void erase(string_view key) override;
    bool compare_and_swap(string_view key, json_value const* expected, json_value const* newval) override;

    using statable_t::backup;
    using statable_t::restore;

private:
    bookkeeper_service_configuration cfg_;
    boost::unordered_map<std::string, json_value, hasher> map_;
    mutable fibers::rw_mutex mtx_;
};

}

#endif // SONIA_BOOKKEEPER_SERVICE_HPP
