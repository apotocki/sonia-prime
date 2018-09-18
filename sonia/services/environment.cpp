//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "environment.hpp"

#include <mutex>
#include <iostream>
//#include <sstream>
#include <fstream>
#include <tuple>

#include <boost/throw_exception.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"
#include "sonia/services.hpp"
#include "sonia/sal.hpp"
#include "sonia/services/builder.hpp"

#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"
#include "sonia/utility/parsers/json/lexertl_lexer.hpp"

#include "sonia/utility/file_persister.hpp"
#include "local_service_registry.hpp"

namespace sonia { namespace services {

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace sp = sonia::parameters;

environment::environment() : log_initialized_(false)
{
    options_.add_options()
        ("log", po::value<std::string>()->default_value("log.conf"), "the logging subsystem configuration file")
        ("cfg,c", po::value<std::vector<std::string>>()->composing(), "configuration (json) file paths")
        ("registry-file,r", po::value<std::string>()->default_value(".services"), "services registry file")
        ("version,v", "display version and exit")
        ("verbose,V", po::value<bool>()->default_value(true), "verbose")
        ("help,h", "display this help and exit")
        ;
    
    // required | optional | default | default from string
    config_parameters_.bind()
        .array("hosts", &environment_configuration::hosts, "hosts description").required()
            .binder(sp::parameters_description<host_configuration>().bind()
                .variable("name", &host_configuration::name, "optional name of host")
                .array("services", &host_configuration::services, "list of startup services to run")
            )
        .map("services", &environment_configuration::services, "services description")
            .binder(sp::parameters_description<service_configuration>().bind()
                .variable("factory", &service_configuration::factory, "builder name").required()
                .variable("layer", &service_configuration::layer, "service layer").default_value(16)
                .variable("parameters", &service_configuration::parameters, "service parameters")
            )
        .map("bundles", &environment_configuration::bundles, "bundles description")
            .binder(sp::parameters_description<bundle_configuration>().bind()
                .variable("lib", &bundle_configuration::lib, "library name").required()
                .variable("layer", &bundle_configuration::layer, "service layer").default_value(0)
            )
        ;

    std::ostringstream version_ss;
    version_ss << "[Version " SONIA_ONE_VERSION " (" << BUILD_NAME << " " << BUILD_DATETIME ")]" HELLO_MESSAGE;
    version_msg_ = std::move(version_ss.str());
}

environment::~environment() {
    hosts_.clear();
    if (log_initialized_) {
        GLOBAL_LOG_INFO() << "terminated";
        logger::deinitialize();
    }
}

void environment::open(int argc, char const* argv[], std::istream * cfgstream)
{
    po::variables_map vm;

    if (argv) {
        po::store(po::command_line_parser(argc, argv).options(options_).run(), vm);
    }

    if (cfgstream) {
        po::store(parse_config_file(*cfgstream, options_), vm);
    }

    if (!argv && !cfgstream) {
        std::istringstream dummy;
        po::store(parse_config_file(dummy, options_), vm);
    }

    try
    {
        notify(vm);
    } catch (std::exception const& e) {
        throw exception("%1%\nThe syntax is incorrect : %2%\n%3%"_fmt % version_msg_ % e.what() % options_);
    }

    if (vm.count("help")) {
        throw shutdown_exception("%1%\n%2%"_fmt % version_msg_ % options_);
    }

    if (vm.count("version")) {
        throw shutdown_exception("%1%"_fmt % version_msg_);
    }

    verbose_ = vm["verbose"].as<bool>();

    std::string const& logcfg = vm["log"].as<std::string>();

    if (!fs::is_regular_file(logcfg)) {
        throw exception("Can not find log configuration file: %1%"_fmt % logcfg);
    }

    std::ifstream logcdfgis(logcfg.c_str());
    logger::initialize(logcdfgis);
    log_initialized_ = true;

    if (verbose_) {
        GLOBAL_LOG_INFO() << version_msg_;
    }

    std::string const& rfile = vm["registry-file"].as<std::string>();
    shared_ptr<persister> pstr = make_shared<file_persister>(rfile);
    registry_ = make_shared<local_service_registry>(pstr);

    factory_ = make_shared<basic_service_factory>();

    if (vm.count("cfg")) {
        for (std::string const& f : vm["cfg"].as<std::vector<std::string>>()) {
            load_configuration(boost::filesystem::path(f));
        }
    }

    //server_configuration.verbose() = vm["verbose"].as<bool>();
    //server_configuration.logger_conf_file_name() = vm["log"].as<std::string>();
    //server_configuration.handling_system_failure() = vm["handling-system-failure"].as<bool>();
 }

void environment::load_configuration(boost::filesystem::path const & fpath) {
    if (!fs::is_regular_file(fpath)) {
        throw exception("Can not find the configuration file: %1%"_fmt % fs::absolute(fpath));
    }
    std::ifstream file(fpath.string().c_str());
    try {
        load_configuration(file);
    } catch (std::exception const& e) {
        throw exception("Error occurred during loading file: %1%,\n%2%"_fmt % fs::absolute(fpath) % e.what());
    }
}

void environment::load_configuration(std::istream & cfg)
{
    using namespace sonia::parsers;

    std::string text;
    std::copy(std::istreambuf_iterator<char>(cfg), std::istreambuf_iterator<char>(), std::back_inserter(text));

    json::model model;

    parse<
        json::light_lexertl_lexer,
        json::parser
    >(model, text.c_str(), text.c_str() + text.size());

    json_value res = model.detach_result();

    environment_configuration ecfg;
    config_parameters_.apply(res.get_object(), &ecfg);

    for (auto const& pair : ecfg.services) {
        factory_->register_service_factory(pair.first, [cfg = std::move(pair.second)]() {
            return environment::create_service(cfg);
        });
    }

    for (auto const& pair : ecfg.bundles) {
        factory_->register_service_factory(pair.first, [cfg = std::move(pair.second)]() {
            return environment::create_bundle_service(cfg);
        });
    }

    // load hosts
    std::vector<std::tuple<shared_ptr<host>, std::vector<std::string>>> host_servs;
    host_servs.reserve(ecfg.hosts.size());
    {
        lock_guard<mutex> guard(cfg_mutex_);
        for (host_configuration const& h : ecfg.hosts) {
            auto hit = hosts_.find(h.name);
            if (hit == hosts_.end()) {
                auto hptr = make_shared<host>(registry_, factory_);
                hosts_.insert(std::make_pair(h.name, hptr));
                host_servs.push_back(std::make_tuple(std::move(hptr), std::move(h.services)));
            }
        }
    }

    for (auto & htpl : host_servs) {
        std::get<0>(htpl)->run(std::get<1>(htpl));
    }
}

shared_ptr<host> environment::default_host() {
    lock_guard<mutex> guard(cfg_mutex_);
    if (hosts_.empty()) {
        return hosts_.insert(std::make_pair("", make_shared<host>(registry_, factory_))).first->second;
    } else if (hosts_.size() == 1) {
        return hosts_.begin()->second;
    }

    auto it = hosts_.find("");
    if (it != hosts_.end()) return hosts_.begin()->second;
    return shared_ptr<host>();
}

void environment::register_service_factory(string_view nm, function<service_descriptor()> const& fm) {
    factory_->register_service_factory(nm, fm);
}

service_descriptor environment::create_service(service_configuration const& cfg) {
    shared_ptr<builder> bld = locate<builder>(cfg.factory);
    return {bld->build(cfg.parameters), cfg.layer};
}

service_descriptor environment::create_bundle_service(bundle_configuration const& cfg) {
    return {sonia::sal::load_bundle(cfg.lib), cfg.layer};
}

}}
