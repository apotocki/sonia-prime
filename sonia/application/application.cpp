//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "application.hpp"

#include <mutex>
#include <iostream>
#include <sstream>

#include <boost/throw_exception.hpp>
#include <boost/filesystem.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia {

namespace po = boost::program_options;
namespace fs = boost::filesystem;

application::application() : log_initialized_(false)
{
    application_options_.add_options()
        ("log", po::value<std::string>()->default_value("log.conf"), "the logging subsystem configuration file")
        ("bundle", po::value<std::vector<std::string>>()->composing(), "service bundles map file paths")
        ("conf", po::value<std::vector<std::string>>()->composing(), "configuration (json) file paths")

        ("version,v", "display version and exit")
        ("verbose,V", po::value<bool>()->default_value(true), "verbose")
        ("help,h", "display this help and exit")
        ;
    
    std::ostringstream version_ss;
    version_ss << "[Version " SONIA_ONE_VERSION " (" << BUILD_NAME << " " << BUILD_DATETIME ")]\n";
    version_msg_ = std::move(version_ss.str());
}

application::~application() {
    if (log_initialized_) {
        logger::deinitialize();
    }
}

int application::open(int argc, char const* argv[], std::istream * cfgstream)
{
    po::variables_map vm;

    if (argv) {
        po::store(po::command_line_parser(argc, argv).options(application_options_).run(), vm);
    }

    if (cfgstream) {
        po::store(parse_config_file(*cfgstream, application_options_), vm);
    }

    try
    {
        notify(vm);
    } catch (std::exception const& e) {
        std::cerr << version_msg_ << HELLO_MESSAGE << "\n";
        std::cerr << "The syntax is incorrect : " << e.what() << "\n" << application_options_;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << version_msg_ << HELLO_MESSAGE << "\n";
        std::cout << application_options_ << "\n";
        return 0;
    }

    if (vm.count("version")) {
        std::cout << version_msg_;
        return 0;
    }

    verbose_ = vm["verbose"].as<bool>();

    std::string const& logcfg = vm["log"].as<std::string>();

    if (!fs::is_regular_file(logcfg)) {
        if (verbose_) std::cout << version_msg_ << HELLO_MESSAGE << "\n";
        std::cerr << "Can not find a log configuration file: " << logcfg << "\n";
        return 1;
    }

    std::ifstream logcdfgis(logcfg.c_str());
    logger::initialize(logcdfgis);
    log_initialized_ = true;

    if (verbose_) {
        GLOBAL_LOG_INFO() << version_msg_ << HELLO_MESSAGE << "\n";
    }

    return 0;
    //server_configuration.verbose() = vm["verbose"].as<bool>();
    //server_configuration.logger_conf_file_name() = vm["log"].as<std::string>();
    //server_configuration.handling_system_failure() = vm["handling-system-failure"].as<bool>();
}

void application::load_configuration(string_view cfgstr)
{

}

}
