//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BOOST_SERIALIZATION_JSON_VALUE_HPP
#define SONIA_BOOST_SERIALIZATION_JSON_VALUE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <sstream>

#include "sonia/exceptions.hpp"
#include "sonia/utility/json/value.hpp"

#include "sonia/utility/parsers/json/lexertl_lexer.hpp"
#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"

namespace boost::serialization {

template <class ArchiveT>
void save(ArchiveT & ar, const sonia::json_value & jv, const unsigned int /*version*/)
{
    std::string val = to_string(jv);
    ar << boost::serialization::make_nvp("value", val);
}

template <class ArchiveT>
void load(ArchiveT & ar, sonia::json_value & t, const unsigned int version)
{
    using namespace sonia::parsers;

    std::string val;
    ar >> boost::serialization::make_nvp("value", val);

    json::model model;

    parse<
        json::light_lexertl_lexer,
        json::parser
    >(model, val.c_str(), val.c_str() + val.size());

    t = model.detach_result();
}

template<class ArchiveT>
void serialize(ArchiveT & ar, sonia::json_value & t, const unsigned int version)
{
    boost::serialization::split_free(ar, t, version);
}

}

#endif // SONIA_BOOST_SERIALIZATION_JSON_VALUE_HPP
