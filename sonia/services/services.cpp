//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <fstream>

#include <boost/throw_exception.hpp>

#include "sonia/filesystem.hpp"
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
std::string const* version_message_ = nullptr;
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

void set_version_message(std::string const* msg)
{
    version_message_ = msg;
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

void assert_environment()
{
    if (env_) return;
    initialize(0, nullptr, nullptr);
}

void shutdown()
{
    if (!env_) return;
    GLOBAL_LOG_INFO() << "terminating...";
    delete env_;
    env_ = nullptr;
    sonia::this_thread::detach_host();
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
    assert_environment();
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

shared_ptr<service> locate(std::string_view nm)
{
    return get_host_impl()->locate(nm);
}

shared_ptr<service> locate(service::id id)
{
    return get_host_impl()->locate(id);
}

void shutdown(int to_level)
{
    get_host_impl()->shutdown(to_level);
}

void register_service_factory(std::string_view nm, function<shared_ptr<service>()> const& fm)
{
    assert_environment();
    env_->register_service_factory(nm, fm);
}

singleton & locate_singleton(std::type_index const& ti, function<shared_ptr<singleton>(singleton::id)> const& f)
{
    assert_environment();
    return env_->locate_singleton(ti, f);
}

void load_configuration(fs::path const & fnm)
{
    assert_environment();
    env_->load_configuration(fnm);
}

void load_configuration(std::istream & is)
{
    assert_environment();
    env_->load_configuration(is);
}

void on_close(function<void()> const& func)
{
    get_host_impl()->register_on_close(func);
}

uint32_t get_type_id(std::type_index ti)
{
    assert_environment();
    return env_->get_type_id(ti);
}

uint32_t register_durable_id(std::string_view nm, std::string_view servnm, std::type_index ti)
{
    assert_environment();
    return env_->register_durable_id(nm, servnm, ti);
}

uint32_t get_durable_id(std::type_index ti)
{
    assert_environment();
    return env_->get_durable_id(ti);
}

std::type_index get_durable_type_index(uint32_t did)
{
    assert_environment();
    return env_->get_durable_type_index(did);
}

void load_durable_id(std::string_view name, std::string_view meta)
{
    assert_environment();
    locate(meta);
}

bool autorun()
{
    assert_environment();
    return env_->autorun();
}

void register_multimethod(multimethod && mm, std::span<const mm_id_elem_t> mmid)
{
    get_host_impl()->register_multimethod(std::move(mm), mmid);
}

multimethod const* get_multimethod(std::span<const mm_id_elem_t> mmid)
{
    return get_host_impl()->get_multimethod(mmid);
}

void copy_multimethods(span<const mm_id_elem_t> from, span<const mm_id_elem_t> to)
{
    return get_host_impl()->copy_multimethods(from, to);
}

// timer
#ifdef BOOST_WINDOWS
timer::timer(function<void()> const& f)
{
    assert_environment();
    impl_ = env_->get_threadpool().create_timer(f);
}
#endif
}

namespace sonia::this_thread {

void attach_host(string_view nm)
{
    sonia::services::assert_environment();
    sonia::services::env_->get_host(nm)->attach_to_current_thread();
}

void detach_host()
{
    sonia::services::thread_descriptor::reset();
}

}
