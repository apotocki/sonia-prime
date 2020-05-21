#include "maat/config.hpp"
#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include "maat/parsers/maat/lexertl_lexer.hpp"
#include "maat/parsers/maat/model.hpp"
#include "maat/parsers/maat/parser.hpp"

#include "llvm/ADT/APFloat.h"

using namespace maat;
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE (tmp_test)
{
    char const* path = std::getenv("MAAT_HOME");
    BOOST_REQUIRE_MESSAGE(path, "MAAT_HOME must be set");
    fs::path maat_home{ path };

    std::ifstream file((maat_home / "tests" / "data" / "test.maat").string().c_str(), std::ios::binary);
    std::string text(std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{});

    parsers::maat::model model;

    parsers::parse<
        parsers::xml::lexertl_lexer,
        parsers::xml::parser
    >(model, text.c_str(), text.c_str() + text.size());
}
