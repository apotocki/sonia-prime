//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/logger/logger.hpp"

#include <boost/preprocessor/stringize.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/from_stream.hpp>

namespace sonia { namespace logger {

#define SONIA_PRINT_CASE(r, data, i, elem) case severity_level::BOOST_PP_TUPLE_ELEM(2, 0, elem): return BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, elem));

const char* to_string(severity_level lvl) {
    switch (lvl) {
        BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_CASE, _, SONIA_LOG_SEVERITY_LEVEL_SEQ)
    default:
        return "UNKNOWN";
    }
}

#define SONIA_PRINT_STR_COND(r, data, i, elem) if (str == BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, elem))) return severity_level::BOOST_PP_TUPLE_ELEM(2, 0, elem);

severity_level from_string(std::string const& str) {
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_STR_COND, _, SONIA_LOG_SEVERITY_LEVEL_SEQ)
    return severity_level::UNKNOWN;
}

void initialize(std::istream & s) {
    using namespace boost::log;

    add_common_attributes(); // (LineID, TimeStamp, ProcessID, ThreadID)
    register_simple_filter_factory<severity_level>("Severity");
    register_simple_formatter_factory<severity_level, char>("Severity");

    init_from_stream(s);
}

void deinitialize() {
    auto core = boost::log::core::get();
    if (!core) return;
    core->flush();
    core->remove_all_sinks();
}

}}
