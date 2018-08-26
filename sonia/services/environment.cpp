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
    
    config_parameters_.add_parameters()
        ("hosts", sp::array_value<shared_ptr<json_value>>())
        ("factories", sp::array_value<shared_ptr<json_value>>())
        ("bundles", sp::array_value<shared_ptr<json_value>>())
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

int environment::open(int argc, char const* argv[], std::istream * cfgstream)
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
        std::cerr << version_msg_ << "\n";
        std::cerr << "The syntax is incorrect : " << e.what() << "\n" << options_;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << version_msg_ << "\n" << options_ << "\n";
        return 0;
    }

    if (vm.count("version")) {
        std::cout << version_msg_;
        return 0;
    }

    verbose_ = vm["verbose"].as<bool>();

    std::string const& logcfg = vm["log"].as<std::string>();

    if (!fs::is_regular_file(logcfg)) {
        if (verbose_) std::cout << version_msg_ << "\n";
        std::cerr << "Can not find the log configuration file: " << logcfg << "\n";
        return 1;
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
    return 0;
    //server_configuration.verbose() = vm["verbose"].as<bool>();
    //server_configuration.logger_conf_file_name() = vm["log"].as<std::string>();
    //server_configuration.handling_system_failure() = vm["handling-system-failure"].as<bool>();
}

void environment::load_configuration(boost::filesystem::path const & fpath) {
    if (!fs::is_regular_file(fpath)) {
        throw exception(fmt("Can not find the configuration file: '%1%'") % fpath);
    }
    std::ifstream file(fpath.string().c_str());
    load_configuration(file);
}

void environment::load_configuration(std::istream & cfg)
{
    /*
    using namespace sonia::parsers;

    std::string text;
    std::copy(std::istreambuf_iterator<char>(cfg), std::istreambuf_iterator<char>(), std::back_inserter(text));

    json::boost_any_builder b;
    json::model model(b);

    parse<
        json::light_lexertl_lexer,
        json::parser
    >(model, text.c_str(), text.c_str() + text.size());

    sp::variables pv;
    sp::store(config_parameters_, model.detach_result(), pv);
    */
}

void environment::register_service_factory(string_view nm, function<shared_ptr<service>()> const& fm)
{
    factory_->register_service_factory(nm, fm);
}

}}
