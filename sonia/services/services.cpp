//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <fstream>

#include <boost/throw_exception.hpp>
#include <boost/filesystem.hpp>

#include "sonia/services.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services/host.hpp"
#include "sonia/services/environment.hpp"

namespace sonia { namespace services {

class thread_descriptor {
public:
    host * get_host() { return host_; }

private:
    sonia::services::host * host_;
};

environment * env_ = nullptr;
thread_local thread_descriptor * tdesc_ = nullptr;

void initialize(int argc, char const* argv[], std::istream * cfgstream) {
    if (env_) {
        BOOST_THROW_EXCEPTION(internal_error("an attempt to initialize not empty environment"));
    }
    env_ = new environment;
    try {
        env_->open(argc, argv, cfgstream);
    } catch (...) {
        delete env_;
        env_ = nullptr;
        throw;
    }
}

void shutdown() {
    GLOBAL_LOG_INFO() << "terminating...";
    delete env_;
    env_ = nullptr;
}

char const* bundles_path() {
    return "/data/app-lib/com.ontos.rdf-1/"; // adnroid case
}

template <typename ArgT>
shared_ptr<service> locate_tmpl(ArgT larg) {
    BOOST_ASSERT(env_);
    thread_descriptor * td = tdesc_;
    if (td) {
        return td->get_host()->locate(larg);
    }
    shared_ptr<host> h = env_->default_host();
    if (h) {
        return h->locate(larg);
    }
    
    BOOST_THROW_EXCEPTION (internal_error(fmt("inappropriate thread to locate '%1%'") % larg));
}

shared_ptr<service> locate(string_view nm) {
    return locate_tmpl(nm);
}

shared_ptr<service> locate(service::id id) {
    return locate_tmpl(id);
}

void register_service_factory(string_view nm, function<service_descriptor()> const& fm) {
    BOOST_ASSERT(env_);
    env_->register_service_factory(nm, fm);
}

void load_configuration(boost::filesystem::path const & fnm) {
    BOOST_ASSERT(env_);
    env_->load_configuration(fnm);
}

void load_configuration(std::istream & is) {
    BOOST_ASSERT(env_);
    env_->load_configuration(is);
}

}}
