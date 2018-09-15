//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "environment.hpp"

#include <mutex>
#include <iostream>
//#include <sstream>
#include <fstream>

#include <boost/throw_exception.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"

//#include "sonia/utility/parsers/json/boost_any_builder.hpp"
#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"
#include "sonia/utility/parsers/json/lexertl_lexer.hpp"

#include "sonia/utility/parameters/json_store.hpp"

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
        ("conf", po::value<std::vector<std::string>>()->composing(), "configuration (json) file paths")
        ("registry-file,r", po::value<std::string>()->default_value(".services"), "services registry file")
        ("version,v", "display version and exit")
        ("verbose,V", po::value<bool>()->default_value(true), "verbose")
        ("help,h", "display this help and exit")
        ;
    
    // required | optional | default | default from string
    config_parameters_.bind()
        .array("hosts", &environment_configuration::hosts, "hosts description")
            .required().default_value(std::vector<host_configuration>()).default_json_value("[]")
            .binder(sp::parameters_description<host_configuration>().bind()
                .variable("name", &host_configuration::name, "optional name of host").required(false)
                .array("services", &host_configuration::services, "list of startup services to run")
            )
        .array("factories", &environment_configuration::factories);
    /*
    config_parameters_.add_parameters()
        ("hosts", sp::array_value<shared_ptr<json_value>>())
        ("factories", sp::array_value<shared_ptr<json_value>>())
        ("bundles", sp::array_value<shared_ptr<json_value>>())
        ;
        */
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

    factory_ = make_shared<basic_service_factory>();
    if (vm.count("conf")) {
        for (std::string const& f : vm["conf"].as<std::vector<std::string>>()) {
            load_configuration(boost::filesystem::path(f));
        }
    }

    std::string const& rfile = vm["registry-file"].as<std::string>();

    shared_ptr<persister> pstr = make_shared<file_persister>(rfile);
    registry_ = make_shared<local_service_registry>(pstr);

    hosts_.emplace_back(registry_, factory_);

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

    //sp::variables pv;
    //sp::store(config_parameters_, model.detach_result(), pv);
}

void environment::register_service_factory(string_view nm, function<shared_ptr<service>()> const& fm)
{
    factory_->register_service_factory(nm, fm);
}

}}
