//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#include "sonia/string.hpp"

#include "sonia/utility/parsers/json/lexertl_lexer.hpp"
#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"

#include "applied/sonia_test.hpp"

using namespace sonia;

#include "sonia/net/uri.hpp"

void uri_test()
{
    char const* path = std::getenv("TESTS_HOME");
    fs::path data_path{ path ? fs::path(path) / "testdata" : fs::path("testdata") };

    std::string text;
    std::ifstream file((data_path / "uri_test.json").string().c_str());
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(text));

    parsers::json::model model;

    parsers::parse<
        parsers::json::light_lexertl_lexer,
        parsers::json::parser
    >(model, text.c_str(), text.c_str() + text.size());

    json_value res = model.detach_result();

    auto optstr = [](json_value const* ptr) { return ptr ? ptr->get_string() : string_view(); };
    auto optnum = [](json_value const* ptr) { return ptr ? ptr->get_int() : 0; };

    BOOST_REQUIRE(res.type() == json_value_type::array);
    for (json_value const& jtcval : res.get_array())
    {
        BOOST_REQUIRE(jtcval.type() == json_value_type::object);
        json_object jtc = jtcval.get_object();
        string_view urival = jtc["uri"]->get_string();
        json_object jtc_result = jtc["result"]->get_object();

        string_view scheme = optstr(jtc_result["scheme"]);
        string_view authority = optstr(jtc_result["authority"]);
        string_view userinfo = optstr(jtc_result["userinfo"]);
        string_view host = optstr(jtc_result["host"]);
        string_view path = optstr(jtc_result["path"]);
        string_view query = optstr(jtc_result["query"]);
        string_view fragment = optstr(jtc_result["fragment"]);
        int port = optnum(jtc_result["port"]);
        bool is_absolute = jtc_result["is_absolute"]->get_bool();

        net::uri_parts parts = net::parse_uri(urival);
        BOOST_CHECK_MESSAGE(scheme == parts.scheme, to_string("wrong scheme('%1%' != '%2%'), uri: %3%"_fmt % scheme % parts.scheme % urival));
        BOOST_CHECK_MESSAGE(authority == parts.authority, to_string("wrong authority('%1%' != '%2%'), uri: %3%"_fmt % authority % parts.authority % urival));
        BOOST_CHECK_MESSAGE(userinfo == parts.userinfo, to_string("wrong userinfo('%1%' != '%2%'), uri: %3%"_fmt % userinfo % parts.userinfo % urival));
        BOOST_CHECK_MESSAGE(host == parts.host, to_string("wrong host('%1%' != '%2%'), uri: %3%"_fmt % host % parts.host % urival));
        BOOST_CHECK_MESSAGE(parts.port.value_or(0) == port, to_string("wrong port('%1%' != '%2%'), uri: %3%"_fmt % port % parts.port.value_or(0) % urival));
        BOOST_CHECK_MESSAGE(path == parts.path, to_string("wrong path('%1%' != '%2%'), uri: %3%"_fmt % path % parts.path % urival));
        BOOST_CHECK_MESSAGE(query == parts.query, to_string("wrong query('%1%' != '%2%'), uri: %3%"_fmt % query % parts.query % urival));
        BOOST_CHECK_MESSAGE(fragment == parts.fragment, to_string("wrong fragment('%1%' != '%2%'), uri: %3%"_fmt % fragment % parts.fragment % urival));
        BOOST_CHECK_MESSAGE(is_absolute == parts.is_absolute, to_string("wrong is_absolute('%1%' != '%2%'), uri: %3%"_fmt % is_absolute % parts.is_absolute % urival));
    }
}

void uri_test_registrar()
{
    register_test(BOOST_TEST_CASE(&uri_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(uri_test_registrar)
#endif
