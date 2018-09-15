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

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "host.hpp"
#include "service_factory.hpp"
#include "sonia/utility/parameters/parameters_description.hpp"

namespace sonia { namespace services {

class host_configuration {
public:
    std::string name;
    std::vector<std::string> services;
};

class factory_configuration {
public:
    std::string name;
    std::string implementation;
    int layer;
    json_object parameters;
};

class bundle_configuration {
public:

};


class environment_configuration {
public:
    std::vector<host_configuration> hosts;
    std::vector<factory_configuration> factories;
    std::vector<bundle_configuration> bundles;
};

class environment {
public:
    environment();
    ~environment();

    void open(int argc, char const* argv[], std::istream * cfgstream = nullptr);
    void load_configuration(boost::filesystem::path const &);
    void load_configuration(std::istream &);

    std::list<host> const& hosts() const noexcept { return hosts_; }
    // host & host(string_view);

    void register_service_factory(string_view, function<shared_ptr<service>()> const&);

private:
    std::list<host> hosts_;
    shared_ptr<service_registry> registry_;
    shared_ptr<basic_service_factory> factory_;

    boost::program_options::options_description options_;
    sonia::parameters::parameters_description<environment_configuration> config_parameters_;

    std::string version_msg_;
    bool verbose_;
    bool log_initialized_;
};

}}

#endif // SONIA_SERVICES_ENVIRONMENT_HPP
