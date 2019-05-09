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
#include <vector>

#include <boost/assert.hpp>
#include <boost/throw_exception.hpp>
#include <boost/thread/future.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"
#include "sonia/services.hpp"
#include "sonia/sal.hpp"

#include "sonia/services/thread_descriptor.hpp"

#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"
#include "sonia/utility/parsers/json/lexertl_lexer.hpp"

#include "sonia/utility/file_persister.hpp"
#include "local_service_registry.hpp"
#include "local_type_registry.hpp"

#ifndef BOOST_WINDOWS
#include "sonia/utility/posix/signals.hpp"
#endif

namespace sonia::services {

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace sp = sonia::parameters;

struct host_equal_to
{
    bool operator()(std::string const& n, shared_ptr<host_impl> const& h) const { return n == h->get_name(); }
    bool operator()(string_view n, shared_ptr<host_impl> const& h) const { return n == h->get_name(); }
};

environment::environment() : log_initialized_(false)
{
    options_.add_options()
        ("log", po::value<std::string>()->default_value("log.conf"), "the logging subsystem configuration file")
        ("cfg,c", po::value<std::string>()->default_value("config.json"), "configuration (json) file paths")
        ("service-registry-file,r", po::value<std::string>()->default_value(".services"), "services registry file")
        ("type-registry-file,t", po::value<std::string>()->default_value(".types"), "types registry file")
        ("version,v", "display version and exit")
        ("verbose,V", po::value<bool>()->default_value(true), "verbose")
        ("help,h", "display this help and exit")
        ;
    
    // required | optional | default | default from string
    config_parameters_.bind()
        .array("hosts", &environment_configuration::hosts, "hosts description")
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

    auto const * vmsg = get_version_message();
    if (!vmsg) {
        std::ostringstream version_ss;
        version_ss << "[Version " SONIA_ONE_VERSION " (" << BUILD_NAME << " " << BUILD_DATETIME ")]" HELLO_MESSAGE;
        version_msg_ = version_ss.str();
        set_version_message(version_msg_);
    } else {
        version_msg_ = *vmsg;
    }

#ifndef BOOST_WINDOWS
    sonia::posix::run_watchers(1);
#endif
}

environment::~environment() noexcept
{
#if 0
    std::vector<fiber> fibers;
    for (auto const& h : hosts_) {
        fibers.emplace_back([h]() { h->close(); });
    }
    for (fiber & f : fibers) {
        f.join();
    }
    fibers.clear();
#else
    for (auto const& h : hosts_) {
        h->close();
    }
    slocator_.shutdown();
#endif
    hosts_.clear();
    if (log_initialized_) {
        GLOBAL_LOG_INFO() << "environment terminated";
        logger::deinitialize();
    }
#ifndef BOOST_WINDOWS
    sonia::posix::stop_watchers();
#endif
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
        throw closed_exception("%1%\n%2%"_fmt % version_msg_ % options_);
    }

    if (vm.count("version")) {
        throw closed_exception("%1%"_fmt % version_msg_);
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

    std::string const& srfile = vm["service-registry-file"].as<std::string>();
    shared_ptr<persister> psrp = make_shared<file_persister>(srfile);
    registry_ = make_shared<local_service_registry>(psrp);

    std::string const& trfile = vm["type-registry-file"].as<std::string>();
    shared_ptr<persister> ptrp = make_shared<file_persister>(trfile);
    set_type_registry(make_shared<local_type_registry>(ptrp));

    factory_ = make_shared<basic_service_factory>();

    if (vm.count("cfg")) {
        start_conf_ = vm["cfg"].as<std::string>();
    }
}

void environment::start()
{
    if (start_conf_) {
        load_configuration(boost::filesystem::path(*start_conf_));
        //for (std::string const& f : vm["cfg"].as<std::vector<std::string>>()) {
        //    load_configuration(boost::filesystem::path(f));
        //}
    }

    //vm["verbose"].as<bool>();
    //vm["handling-system-failure"].as<bool>();
}

void environment::load_configuration(boost::filesystem::path const & fpath)
{
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

    std::string text{std::istreambuf_iterator<char>(cfg), std::istreambuf_iterator<char>()};

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
    std::vector<boost::promise<void>> bootstrap_promises(ecfg.hosts.size());

    for (size_t hidx = 0; hidx < ecfg.hosts.size(); ++hidx) {
        boost::thread host_creation_thread([this, &ecfg, hidx, &bootstrap_promises] {
            auto & hcfg = ecfg.hosts[hidx];
            try {
                sonia::sal::set_thread_name(sonia::this_thread::get_id(), "bootstrap thread for host '" + hcfg.name + "'");

                unique_lock lk(host_mtx_);
                auto hit = hosts_.find(hcfg.name, hash<std::string>(), host_equal_to());
                if (hit == hosts_.end()) {
                    lk.unlock();
                    auto hptr = make_shared<host_impl>(hcfg.name);
                    hptr->open(registry_, factory_);
                    lk.lock();
                    hit = hosts_.insert(hit, std::move(hptr));
                }
                lk.unlock();
                (*hit)->run(hcfg.services);
                bootstrap_promises[hidx].set_value();
            } catch (...) {
                bootstrap_promises[hidx].set_exception(std::runtime_error(to_string("Error occurred during '%1%' host initialization\n%2%"_fmt % (hcfg.name.empty() ? "default" : hcfg.name.c_str()) % boost::current_exception_diagnostic_information())));
            }
        });
        host_creation_thread.detach();
    }

    for (auto it = bootstrap_promises.begin(), eit = bootstrap_promises.end(); it != eit; ++it) {
        try {
            it->get_future().get();
        } catch (...) {
            // wait for other hosts
            for (++it; it != eit; ++it) {
                try { it->get_future().get(); } catch (...) { /* ignore */ }
            }
            throw;
        }
    }
}

singleton & environment::locate_singleton(std::type_index const& ti, function<shared_ptr<singleton>()> const& f)
{
    const uint32_t tid = get_type_id(ti);
    return *slocator_.get(tid, f);
}

shared_ptr<host_impl> environment::default_host()
{
    unique_lock lk(host_mtx_);
    if (hosts_.empty()) {
        lk.unlock();
        auto h = make_shared<host_impl>("");
        h->open(registry_, factory_);
        lk.lock();
        return *hosts_.insert(h).first;
    } else if (hosts_.size() == 1) {
        return *hosts_.begin();
    }

    auto it = hosts_.find(string_view(""), hasher(), host_equal_to());
    if (it != hosts_.end()) return *hosts_.begin();
    return {};
}

shared_ptr<host_impl> environment::get_host(string_view hnm)
{
    lock_guard guard(host_mtx_);
    auto it = hosts_.find(hnm, hasher(), host_equal_to());
    if (it != hosts_.end()) {
        return *it;
    }
    throw exception("host %1% is not found"_fmt % hnm);
}

void environment::register_service_factory(string_view nm, function<shared_ptr<service>()> const& fm)
{
    factory_->register_service_factory(nm, fm);
}

shared_ptr<service> environment::create_service(service_configuration const& cfg)
{
    shared_ptr<builder> bld = locate<builder>(cfg.factory);
    return bld->build(cfg);
}

shared_ptr<service> environment::create_bundle_service(bundle_configuration const& cfg)
{
    return sonia::sal::load_bundle(cfg);
}

uint32_t environment::get_type_id(std::type_index const& ti)
{
    lock_guard guard(type_id_mtx_);
    auto it = type_id_map_.left.find(ti);
    if (it == type_id_map_.left.end()) {
        uint32_t idx = ++type_id_counter_;
        BOOST_VERIFY(type_id_map_.insert(type_id_map_type::value_type(ti, idx)).second);
        return idx;
    }
    return it->second;
}

}
