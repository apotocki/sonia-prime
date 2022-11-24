//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/log/sinks.hpp>
#include <boost/log/core/record_view.hpp>
#include <boost/log/utility/setup/from_settings.hpp>

namespace sonia::logger {

class android_sink: public boost::log::sinks::basic_formatted_sink_backend<
    char,
    boost::log::sinks::combine_requirements<
        boost::log::sinks::synchronized_feeding,
        boost::log::sinks::flushing
    >::type
>
{
    using base_type = boost::log::sinks::basic_formatted_sink_backend<
        char,
        boost::log::sinks::combine_requirements< boost::log::sinks::synchronized_feeding, boost::log::sinks::flushing >::type
    >;

    using string_type = typename base_type::string_type;


public:
    android_sink();

    void consume(boost::log::record_view const& rec, string_type const& formatted_message);

    void flush();

private:

};
    
class android_sink_factory :
    public boost::log::sink_factory<char>
{
public:
    // Creates the sink with the provided parameters
    boost::shared_ptr< boost::log::sinks::sink > create_sink(boost::log::settings_section const& settings);
};

}
