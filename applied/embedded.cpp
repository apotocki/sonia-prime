//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/prime_config.hpp"

#include "sonia/concurrency.hpp"
#include "sonia/services.hpp"

#include <sstream>

#ifdef SONIA_PRIME_STATIC
#	include "sonia/services/prime_bundle.hpp"
#	include "sonia/services/environment.hpp"
#endif

extern "C" {

SONIA_PRIME_API void start_environment(char const* proc_cfgstr, char const* app_cfg)
{
#ifdef SONIA_PRIME_STATIC
	using namespace sonia::services;
	set_post_initialize([](environment* penv) {
	penv->add_load_bundle_hook([](bundle_configuration const& cfg) {
		sonia::shared_ptr<sonia::service> result;
		if (cfg.lib == "sonia-prime") {
			result.reset(new prime_bundle());
		}
		return result;
		});
	});
#endif
	int argc = 0;
	char const** argv = nullptr;

	std::stringstream proc_cfgss;
	proc_cfgss.str(proc_cfgstr);

	sonia::services::initialize(argc, argv, &proc_cfgss);

	std::stringstream cfgss;
	cfgss.str(app_cfg);
	sonia::services::load_configuration(cfgss);

}

SONIA_PRIME_API void stop_environment()
{
	sonia::services::shutdown();
}

}
