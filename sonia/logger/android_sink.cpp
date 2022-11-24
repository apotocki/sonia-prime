//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "android_sink.hpp"

#include "logger.hpp"

#include <boost/log/utility/setup.hpp>
//#include <boost/log/utility/setup/from_settings.hpp>
//#include <boost/log/utility/setup/from_stream.hpp>

#include <android/log.h>

namespace sonia::logger {

using namespace boost::log;

android_sink::android_sink()
{
    
}

void android_sink::consume(boost::log::record_view const& rec, string_type const& message)
{
    int prio = ANDROID_LOG_UNKNOWN;
    if (value_ref<severity_level> severity =
        rec["Severity"].extract<severity_level >())
    {
        switch (severity.get()) {
        case severity_level::trace:
            prio = ANDROID_LOG_VERBOSE; break;
        case severity_level::debug:
            prio = ANDROID_LOG_DEBUG; break;
        case severity_level::info:
            prio = ANDROID_LOG_INFO; break;
        case severity_level::warning:
            prio = ANDROID_LOG_WARN; break;
        case severity_level::error:
            prio = ANDROID_LOG_ERROR; break;
        case severity_level::fatal:
            prio = ANDROID_LOG_FATAL; break;
        default:
            prio = ANDROID_LOG_UNKNOWN;
        }
    }
    __android_log_write(prio, "sonia", message.c_str());
}

void android_sink::flush()
{

}

boost::shared_ptr<sinks::sink > android_sink_factory::create_sink(settings_section const& settings)
{
    // Create the sink
    boost::shared_ptr<android_sink> backend = boost::make_shared<android_sink>();
    boost::shared_ptr< sinks::synchronous_sink<android_sink> > sink = boost::make_shared< sinks::synchronous_sink<android_sink> >(backend);

    if (boost::optional< std::string > param = settings["Filter"])
    {
        sink->set_filter(parse_filter(param.get()));
    }

    if (boost::optional< std::string > param = settings["Format"])
    {
        sink->set_formatter(parse_formatter(param.get()));
    }

    return sink;
}

}
