//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_JSON_OPTIONS_HPP
#define SONIA_JSON_OPTIONS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/options.hpp"
#include "sonia/array_view.hpp"

namespace sonia { namespace options {
/*
class json_loader 
    : public options_loader
    , basic_parsed_options<char>
{
    typedef basic_parsed_options<char> base_t;

public:
    ~json_loader() override {}

    json_loader(array_view<char>);

    void load(options_description const& opts, variables_map & vm) override;
       
};
*/
}}

#endif // SONIA_JSON_OPTIONS_HPP
