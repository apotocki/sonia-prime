//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <fstream>

#include <boost/throw_exception.hpp>
#include <boost/filesystem.hpp>

#include "sonia/singleton.hpp"
#include "sonia/services.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services/host_impl.hpp"
#include "sonia/services/environment.hpp"
#include "sonia/services/thread_descriptor.hpp"
#include "sonia/services/on_close.hpp"
#include "sonia/services/timer.hpp"
#include "sonia/utility/type_id.hpp"
#include "sonia/utility/type_durable_id.hpp"
#include "sonia/utility/multimethod.hpp"

namespace sonia::services {

thread_local thread_descriptor * tdesc_ = nullptr;

post_initialize_fn * post_initialize_fn_ = nullptr;

std::string * default_base_path_ = nullptr;
std::string * version_message_ = nullptr;
environment * env_ = nullptr;

void thread_descriptor::reset()
{
    delete tdesc_;
    tdesc_ = nullptr;
}

void thread_descriptor::set()
{
    if (tdesc_) {
        delete tdesc_;
    } else {
        sonia::this_thread::at_thread_exit([] { thread_descriptor::reset(); });
    }
    tdesc_ = this;
}

void set_default_base_path(std::string val)
{
    if (!default_base_path_) {
        default_base_path_ = new std::string(std::move(val));
    } else {
        *default_base_path_ = std::move(val);
    }
}

std::string const* get_default_base_path()
{
    return default_base_path_;
}

void set_version_message(std::string msg)
{
    if (!version_message_) {
        version_message_ = new std::string(std::move(msg));
    } else {
        *version_message_ = std::move(msg);
    }
}

std::string const* get_version_message()
{
    return version_message_;
}

void set_post_initialize(post_initialize_fn* fn)
{
    post_initialize_fn_ = fn;
}

void initialize(int argc, char const* argv[], std::istream * cfgstream)
{
    if (env_) {
        THROW_INTERNAL_ERROR("an attempt to initialize not empty environment");
    }

    env_ = new environment;
    try {
        env_->open(argc, argv, cfgstream);
        if (post_initialize_fn_) {
            post_initialize_fn_(env_);
        }
        env_->start();
    } catch (...) {
        delete env_;
        env_ = nullptr;
        throw;
    }
}

void shutdown()
{
    sonia::this_thread::detach_host();
    GLOBAL_LOG_INFO() << "terminating...";
    delete env_;
    env_ = nullptr;
    if (version_message_) {
        delete version_message_;
        version_message_ = nullptr;
    }
    if (default_base_path_) {
        delete default_base_path_;
        default_base_path_ = nullptr;
    }
}

shared_ptr<host> get_host()
{
    thread_descriptor * td = tdesc_;
    if (td) return td->host;
    THROW_INTERNAL_ERROR("inappropriate thread to operate, attach a host to the current thread first.");
}

char const* bundles_path()
{
    return "/data/app-lib/com.ontos.rdf-1/"; // android case
}

shared_ptr<host_impl> get_host_impl()
{
    BOOST_ASSERT(env_);
    thread_descriptor * td = tdesc_;
    if (td) {
        return td->host;
    }
    shared_ptr<host_impl> h = env_->default_host();
    if (!h) {
        THROW_INTERNAL_ERROR("inappropriate thread to operate");
    }
    h->attach_to_current_thread();
    return h;
}

shared_ptr<service> locate(string_view nm)
{
    return get_host_impl()->locate(nm);
}

shared_ptr<service> locate(service::id id)
{
    return get_host_impl()->locate(id);
}

void register_service_factory(string_view nm, function<shared_ptr<service>()> const& fm)
{
    BOOST_ASSERT(env_);
    env_->register_service_factory(nm, fm);
}

singleton & locate_singleton(std::type_index const& ti, function<shared_ptr<singleton>(singleton::id)> const& f)
{
    return env_->locate_singleton(ti, f);
}

void load_configuration(boost::filesystem::path const & fnm)
{
    BOOST_ASSERT(env_);
    env_->load_configuration(fnm);
}

void load_configuration(std::istream & is)
{
    BOOST_ASSERT(env_);
    env_->load_configuration(is);
}

void on_close(function<void()> const& func)
{
    get_host_impl()->register_on_close(func);
}

uint32_t get_type_id(std::type_index ti)
{
    BOOST_ASSERT(env_);
    return env_->get_type_id(ti);
}

uint32_t register_durable_id(string_view nm, string_view servnm, std::type_index ti)
{
    BOOST_ASSERT(env_);
    return env_->register_durable_id(nm, servnm, ti);
}

uint32_t get_durable_id(std::type_index ti)
{
    BOOST_ASSERT(env_);
    return env_->get_durable_id(ti);
}

std::type_index get_durable_type_index(uint32_t did)
{
    BOOST_ASSERT(env_);
    return env_->get_durable_type_index(did);
}

void load_durable_id(string_view name, string_view meta)
{
    BOOST_ASSERT(env_);
    locate(meta);
}

void register_multimethod(multimethod && mm, array_view<const std::type_index> mmid)
{
    get_host_impl()->register_multimethod(std::move(mm), mmid);
}

multimethod const* get_multimethod(array_view<const std::type_index> mmid)
{
    return get_host_impl()->get_multimethod(mmid);
}

// timer
#ifdef BOOST_WINDOWS
timer::timer(function<void()> const& f)
{
    BOOST_ASSERT(env_);
    impl_ = env_->get_threadpool().create_timer(f);
}
#endif
}

namespace sonia::this_thread {

void attach_host(string_view nm)
{
    BOOST_ASSERT(sonia::services::env_);
    sonia::services::env_->get_host(nm)->attach_to_current_thread();
}

void detach_host()
{
    sonia::services::thread_descriptor::reset();
}

}
