//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "scoped_services.hpp"

#ifdef SONIA_PRIME_STATIC
#	include "sonia/services/prime_bundle.hpp"
#	include "sonia/services/environment.hpp"
#endif

void scoped_services::pre_initialize(int& argc, char const**& argv)
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
}
