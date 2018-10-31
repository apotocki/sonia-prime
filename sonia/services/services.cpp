//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <fstream>

#include <boost/throw_exception.hpp>
#include <boost/filesystem.hpp>

#include "sonia/services.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services/host_impl.hpp"
#include "sonia/services/environment.hpp"
#include "sonia/services/thread_descriptor.hpp"

#include "sonia/utility/on_close.hpp"
#include "sonia/utility/type_id.hpp"
#include "sonia/utility/type_durable_id.hpp"
#include "sonia/utility/multimethod.hpp"

namespace sonia { namespace services {

thread_local thread_descriptor * tdesc_ = nullptr;

environment * env_ = nullptr;

void thread_descriptor::reset() {
    delete tdesc_;
    tdesc_ = nullptr;
}

void thread_descriptor::set() {
    if (tdesc_) {
        delete tdesc_;
    } else {
        sonia::this_thread::at_thread_exit([] { thread_descriptor::reset(); });
    }
    tdesc_ = this;
}

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
    sonia::this_thread::detach_host();
    GLOBAL_LOG_INFO() << "terminating...";
    delete env_;
    env_ = nullptr;
}

shared_ptr<host> get_host() {
    thread_descriptor * td = tdesc_;
    if (td) return td->host;
    BOOST_THROW_EXCEPTION(internal_error("inappropriate thread to operate, attach a host to the current thread first."));
}

char const* bundles_path() {
    return "/data/app-lib/com.ontos.rdf-1/"; // android case
}

shared_ptr<host_impl> get_host_impl() {
    BOOST_ASSERT(env_);
    thread_descriptor * td = tdesc_;
    if (td) {
        return td->host;
    }
    shared_ptr<host_impl> h = env_->default_host();
    if (!h) {
        BOOST_THROW_EXCEPTION(internal_error("inappropriate thread to operate"));
    }
    h->attach_to_current_thread();
    return h;
}

shared_ptr<service> locate(string_view nm) {
    return get_host_impl()->locate(nm);
}

shared_ptr<service> locate(service::id id) {
    return get_host_impl()->locate(id);
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

void on_close(function<void()> const& func) {
    get_host_impl()->register_on_close(func);
}

uint32_t get_type_id(std::type_info const& ti) {
    BOOST_ASSERT(env_);
    return env_->get_type_id(ti);
}

uint32_t register_durable_id(string_view nm, string_view servnm, std::type_info const& ti) {
    BOOST_ASSERT(env_);
    return env_->register_durable_id(nm, servnm, ti);
}

uint32_t get_durable_id(std::type_info const& ti) {
    BOOST_ASSERT(env_);
    return env_->get_durable_id(ti);
}

void register_multimethod(multimethod && mm, type_info const& id, array_view<const type_info> mmid) {
    get_host_impl()->register_multimethod(std::move(mm), id, mmid);
}

multimethod const* get_multimethod(type_info const& id) {
    return get_host_impl()->get_multimethod(id);
}

}}

namespace sonia { namespace this_thread {

void attach_host(string_view nm) {
    BOOST_ASSERT(sonia::services::env_);
    sonia::services::env_->get_host(nm)->attach_to_current_thread();
}

void detach_host() {
    sonia::services::thread_descriptor::reset();
}

}}
