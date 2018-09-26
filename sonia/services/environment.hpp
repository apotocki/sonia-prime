//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_ENVIRONMENT_HPP
#define SONIA_SERVICES_ENVIRONMENT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>
#include <list>
#include <atomic>
#include <typeinfo>
#include <typeindex>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

#include "sonia/thread.hpp"

#include "host.hpp"
#include "service_factory.hpp"
#include "type_registry.hpp"
#include "sonia/utility/parameters/parameters.hpp"

namespace sonia { namespace services {

class host_configuration {
public:
    std::string name;
    std::vector<std::string> services;
};

class service_configuration {
public:
    std::string factory;
    int layer;
    json_object parameters;
};

class bundle_configuration {
public:
    std::string lib;
    int layer;
};

class environment_configuration {
public:
    std::vector<host_configuration> hosts;
    std::map<std::string, service_configuration> services;
    std::map<std::string, bundle_configuration> bundles;
};

class environment {
public:
    environment();
    ~environment();

    void open(int argc, char const* argv[], std::istream * cfgstream = nullptr);
    void load_configuration(boost::filesystem::path const &);
    void load_configuration(std::istream &);

    //std::list<host> const& hosts() const noexcept { return hosts_; }
    // host & host(string_view);
    shared_ptr<host> default_host();

    void register_service_factory(string_view, function<service_descriptor()> const&);

    // type_id api
    uint32_t get_type_id(std::type_info const&);

    // durable id api
    uint32_t register_durable_id(string_view nm, std::type_info const& ti);
    uint32_t get_durable_id(std::type_info const&);

private:
    static service_descriptor create_service(service_configuration const& cfg);
    static service_descriptor create_bundle_service(bundle_configuration const& cfg);

    boost::unordered_map<std::string, shared_ptr<host>> hosts_;
    shared_ptr<type_registry> type_registry_;
    shared_ptr<service_registry> registry_;
    shared_ptr<basic_service_factory> factory_;

    boost::program_options::options_description options_;
    sonia::parameters::parameters_description<environment_configuration> config_parameters_;

    std::string version_msg_;
    bool verbose_;
    bool log_initialized_;

    mutable mutex cfg_mutex_;

    // type_id support
    mutable spinlock type_id_mtx_;
    typedef boost::bimap<
        boost::bimaps::unordered_set_of<std::type_index>,
        boost::bimaps::unordered_set_of<uint32_t>
    > type_id_map_type;
    type_id_map_type type_id_map_;
    std::atomic<uint32_t> type_id_counter_;

    mutable spinlock type_durable_id_mtx_;
    type_id_map_type type_durable_id_map_;
};

}}

#endif // SONIA_SERVICES_ENVIRONMENT_HPP
