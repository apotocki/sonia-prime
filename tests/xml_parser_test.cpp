//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "sonia/utility/parsers/xml/lexertl_lexer.hpp"
#include "sonia/utility/parsers/xml/model.hpp"
#include "sonia/utility/parsers/xml/parser.hpp"

using namespace sonia;
namespace fs = boost::filesystem;

#include <fstream>
#include <iterator>

#include <boost/unordered_map.hpp>

#if 0
BOOST_AUTO_TEST_CASE(xml_parse_test)
{
    std::string text;
    std::ifstream file("");
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(text));

    parsers::json::model model;

    parsers::parse<
        parsers::json::light_lexertl_lexer,
        parsers::json::parser
    >(model, text.c_str(), text.c_str() + text.size());

}
#endif

#if 1
BOOST_AUTO_TEST_CASE(xml_test)
{
    char const* path = std::getenv("SONIA_PRIME_HOME");
    BOOST_REQUIRE_MESSAGE(path, "SONIA_PRIME_HOME must be set");
    fs::path sonia_prime_home{ path };

    std::ifstream file((sonia_prime_home / "tests" / "data" / "xml-test.xml").string().c_str(), std::ios::binary);
    std::string text(std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{});

    parsers::xml::basic_model model;

    parsers::parse<
        parsers::xml::lexertl_lexer,
        parsers::xml::parser
    >(model, text.c_str(), text.c_str() + text.size());
    
}
#endif
