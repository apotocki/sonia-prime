//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_LOGGER_HPP
#define SONIA_LOGGER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <istream>
#include <ostream>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <boost/log/keywords/severity.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"

// implementation is based on boost/log/trivial.hpp example
namespace sonia { namespace logger {

#define SONIA_LOG_SEVERITY_LEVEL_SEQ  \
    ((trace, TRACE))     \
    ((debug, DEBUG))     \
    ((info, INFO))       \
    ((warning, WARN))    \
    ((error, ERROR))     \
    ((fatal, FATAL))

#define SONIA_PRINT_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem),

enum class severity_level
{
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_TYPE_ENUM, _, SONIA_LOG_SEVERITY_LEVEL_SEQ)
    UNKNOWN
};

#undef SONIA_PRINT_TYPE_ENUM

string_view to_string_view(severity_level lvl);
severity_level from_string(std::string const&);

void initialize(std::istream &);
void deinitialize();

//! Outputs stringized representation of the severity level to the stream
template <typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT>& strm, severity_level lvl)
{
    return strm << to_string_view(lvl);
}

//! Reads stringized representation of the severity level from the stream
template <typename CharT, typename TraitsT>
std::basic_istream<CharT, TraitsT> & operator>> (std::basic_istream<CharT, TraitsT> & strm, severity_level& lvl)
{
    std::string str;
    strm >> str;
    lvl = from_string(str);
    return strm;
}

using logger_type = boost::log::sources::severity_logger_mt<severity_level>;
using logger_ptr = shared_ptr<logger_type>;

BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(global_logger, boost::log::sources::severity_logger_mt<severity_level>, (severity_level::trace))

}

#define LOG_SEV(l, sev) BOOST_LOG_SEV(*l, sev)
#define LOG_TRACE(l) BOOST_LOG_SEV(*l, sonia::logger::severity_level::trace)
#define LOG_DEBUG(l) BOOST_LOG_SEV(*l, sonia::logger::severity_level::debug)
#define LOG_INFO(l)  BOOST_LOG_SEV(*l, sonia::logger::severity_level::info)
#define LOG_WARN(l)  BOOST_LOG_SEV(*l, sonia::logger::severity_level::warning)
#define LOG_ERROR(l) BOOST_LOG_SEV(*l, sonia::logger::severity_level::error)
#define LOG_FATAL(l) BOOST_LOG_SEV(*l, sonia::logger::severity_level::fatal)

#define GLOBAL_LOG_TRACE() BOOST_LOG_SEV(sonia::logger::global_logger::get(), sonia::logger::severity_level::trace)
#define GLOBAL_LOG_DEBUG() BOOST_LOG_SEV(sonia::logger::global_logger::get(), sonia::logger::severity_level::debug)
#define GLOBAL_LOG_INFO()  BOOST_LOG_SEV(sonia::logger::global_logger::get(), sonia::logger::severity_level::info)
#define GLOBAL_LOG_WARN()  BOOST_LOG_SEV(sonia::logger::global_logger::get(), sonia::logger::severity_level::warning)
#define GLOBAL_LOG_ERROR() BOOST_LOG_SEV(sonia::logger::global_logger::get(), sonia::logger::severity_level::error)
#define GLOBAL_LOG_FATAL() BOOST_LOG_SEV(sonia::logger::global_logger::get(), sonia::logger::severity_level::fatal)

}

#endif // SONIA_LOGGER_HPP
