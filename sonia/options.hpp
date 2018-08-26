//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_OPTIONS_HPP
#define SONIA_OPTIONS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/program_options.hpp>

/*
namespace sonia { namespace options {

using namespace boost::program_options;
//using boost::program_options::options_description;
//using boost::program_options::variables_map;

class loader {
public:
    virtual ~loader() {}
    virtual void load(options_description const&, variables_map &) = 0;
};

template <typename CharT>
class command_line_loader 
    : public options_loader
    , basic_command_line_parser<CharT>
{
public:
    ~command_line_loader() override {}

    command_line_loader(int argc, const CharT* const argv[])
        : basic_command_line_parser<CharT>(argc, argv)
    {}

    void load(options_description const& opts, variables_map & vm) override {
        store(this->options(opts).run(), vm);
    };
};

}

typedef options::loader options_loader;

}

*/

#endif // SONIA_OPTIONS_HPP
