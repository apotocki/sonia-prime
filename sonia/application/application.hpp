//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_APPLICATION_APPLICATION_HPP
#define SONIA_APPLICATION_APPLICATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>

#include <boost/program_options.hpp>

#include "sonia/string.hpp"
#include "application_host.hpp"

namespace sonia {

class application {
public:
    application();
    ~application();

    int open(int argc, char const* argv[], std::istream * cfgstream = nullptr);
    void load_configuration(string_view);

    application_host & host(string_view);

private:
    boost::program_options::options_description application_options_;
    std::string version_msg_;
    bool verbose_;
    bool log_initialized_;
};

}

#endif // APPLICATION_
