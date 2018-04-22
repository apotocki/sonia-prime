//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "application.hpp"

#include <mutex>
#include <iostream>
#include <sstream>

#include <boost/program_options.hpp>
#include <boost/throw_exception.hpp>

#include "sonia/exceptions.hpp"

namespace sonia {

namespace po = boost::program_options;
//namespace fs = boost::filesystem;

std::once_flag application_options_init_flag;
po::options_description application_options;
std::string version_msg;

application::application()
{
    std::call_once(application_options_init_flag, []()->void {
        application_options.add_options()
            ("log", po::value<std::string>()->default_value("log.conf"), "applications")
            ("bundle", po::value<std::vector<std::string>>(), "service bundles map file paths")
            ("host", po::value<std::vector<std::string>>()->composing(), "host configuration file paths")

            ("version,v", "display version and exit")
            ("verbose,V", po::value<bool>()->default_value(true), "verbose")
            ("help,h", "display this help and exit")
            ;

        std::ostringstream version_ss;
        version_ss << "[Version " SONIA_ONE_VERSION " (" << BUILD_NAME << " " << BUILD_DATETIME ")]\n";
        version_msg = std::move(version_ss.str());
    });

}

int open(int argc, char const* argv[], std::istream * cfgstream)
{
    po::variables_map vm;

    if (argv) {
        po::store(po::command_line_parser(argc, argv).options(application_options).run(), vm);
    }

    if (cfgstream) {
        po::store(parse_config_file(*cfgstream, application_options), vm);
    }

    try
    {
        notify(vm);
    } catch (std::exception const& e) {
        std::cout << version_msg << HELLO_MESSAGE << "\n";
        std::ostringstream errss;
        errss << "The syntax is incorrect : " << e.what() << "\n" << application_options;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << version_msg << HELLO_MESSAGE << "\n";
        std::cout << application_options << "\n";
        return 1;
    }

    if (vm.count("version")) {
        std::cout << version_msg;
        return 1;
    }

    return 0;
    //server_configuration.verbose() = vm["verbose"].as<bool>();
    //server_configuration.logger_conf_file_name() = vm["log"].as<std::string>();
    //server_configuration.handling_system_failure() = vm["handling-system-failure"].as<bool>();
}

}
