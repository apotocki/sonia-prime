//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "json.hpp"

#include "sonia/utility/parsers/json/lexertl_lexer.hpp"
#include "sonia/utility/parsers/json/basic_builder.hpp"
#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"

namespace sonia { namespace options {

using namespace sonia::parsers;
/*
json_loader::json_loader(array_view<char> data)
    : base_t()
{
    json::basic_builder b;
    json::model model(b);
    parse<
        parsers::json::lexertl_lexer,
        parsers::json::parser
    >(model, data.begin(), data.end());
}

void json_loader::load(options_description const& opts, variables_map & vm) {

}

*/
}}
