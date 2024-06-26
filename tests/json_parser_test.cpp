//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#include "sonia/config.hpp"

#include <fstream>
#include <iterator>

#include "sonia/filesystem.hpp"

#include "sonia/utility/parsers/json/lexertl_lexer.hpp"
#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"

using namespace sonia;

#include <boost/any.hpp>
#include <boost/unordered_map.hpp>

#include "applied/sonia_test.hpp"

#if 0
void json_parse_test()
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

//#include <fstream>
//
//#include "sonia/utility/iterators/file_region_iterator.hpp"
//#include "sonia/utility/iterators/archive_extract_iterator.hpp"
//
//namespace {
//
//void restore_assets(const char* archivepath)
//{
//    using namespace sonia;
//    try {
//        fs::path filepath = archivepath;
//        fs::path filename = filepath.filename();
//        archive_iterator ait = make_archive_extract_iterator(filename.string().c_str(), file_region_iterator<const char>(filepath, 0, 65536));
//        ait.impl->set_extraction_depth(2);
//        while (ait.next()) {
//            fs::path itempath = ait.name();
//            std::vector<fs::path> parts;
//            while (!itempath.empty()) {
//                parts.emplace_back(itempath.filename());
//                itempath = itempath.parent_path();
//            }
//            if (parts.empty()) continue;
//            //parts.pop_back();
//            while (!parts.empty()) {
//                itempath /= parts.back();
//                parts.pop_back();
//            }
//            
//            if (auto parent = itempath.parent_path(); !parent.empty()) {
//                fs::create_directories(parent);
//            }
//
//            { std::ofstream f{ itempath.string().c_str() }; }
//            file_region_iterator<char> fit(itempath, 0, 65536);
//            std::cout << "restored " << itempath << "\n";
//            size_t fsz = 0;
//            do {
//                span<const char> rng = *ait;
//                fit.write(rng);
//                ++ait;
//            } while (!ait.empty());
//            fit.close();
//        }
//    }
//    catch (...) {
//        std::cout << boost::current_exception_diagnostic_information();
//    }
//}
//
//}

#if 1
void json_test()
{
    //restore_assets("w:\\projects\\android\\TesCppApplication\\test_assets.tar.xz");

    char const* path = std::getenv("TESTS_HOME");
    fs::path data_path{ (path ? fs::path(path) / "testdata" : fs::path("testdata")) };

    std::string text;
    std::ifstream file((data_path / "json_test.json").string().c_str());
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(text));

    parsers::json::model model;

    parsers::parse<
        parsers::json::light_lexertl_lexer,
        parsers::json::parser
    >(model, text.c_str(), text.c_str() + text.size());
    
    json_value res = model.detach_result();
    BOOST_REQUIRE(res.type() == json_value_type::object);
    json_object jobj = res.get_object();
    BOOST_CHECK_EQUAL(jobj.size(), 12);
    jobj["key0"];
    BOOST_REQUIRE(jobj["key0"]);
    BOOST_CHECK(jobj["key0"]->type() == json_value_type::null);
    BOOST_CHECK(jobj["key1"]->type() == json_value_type::boolean);
    BOOST_CHECK(jobj["key2"]->type() == json_value_type::boolean);
    BOOST_CHECK(jobj["key3"]->type() == json_value_type::number);
    BOOST_CHECK(jobj["key4"]->type() == json_value_type::string);
    BOOST_CHECK(jobj["key5"]->type() == json_value_type::string);
    BOOST_CHECK(jobj["key6"]->type() == json_value_type::number);

    BOOST_CHECK(!jobj["key1"]->get_bool());
    BOOST_CHECK(jobj["key2"]->get_bool());
    BOOST_CHECK_EQUAL(jobj["key3"]->get_int(), 123);
    BOOST_CHECK_EQUAL(jobj["key3"]->get_int64(), 123);
    BOOST_CHECK_EQUAL(jobj["key3"]->get_number(), 123);
    BOOST_CHECK_EQUAL(jobj["key4"]->get_string(), "string value");
    BOOST_CHECK_EQUAL(jobj["key5"]->get_string(), "str");
    BOOST_CHECK(jobj["key4 - just a long key"]->type() == json_value_type::object);
    BOOST_CHECK_EQUAL(jobj["key4 - just a long key"]->get_object().size(), 0);
    BOOST_CHECK(jobj["key4 - just a long long key"]->type() == json_value_type::array);
    BOOST_CHECK_EQUAL(jobj["key4 - just a long long key"]->get_array().size(), 0);
    BOOST_CHECK(jobj["$k0"]->type() == json_value_type::object);
    BOOST_CHECK_EQUAL(jobj["$k0"]->get_object().size(), 2);
    BOOST_CHECK_EQUAL(jobj["$k0"]->get_object()["%0"]->get_int(), 1);
    BOOST_CHECK_EQUAL(jobj["$k0"]->get_object()["%1"]->get_int(), 2);
    BOOST_CHECK(jobj["$k1"]->type() == json_value_type::array);
    BOOST_CHECK_EQUAL(jobj["$k1"]->get_array().size(), 3);
    BOOST_CHECK_EQUAL(jobj["$k1"]->get_array()[0].get_int(), 1);
    BOOST_CHECK_EQUAL(jobj["$k1"]->get_array()[1].get_int(), 2);
    BOOST_CHECK(jobj["$k1"]->get_array()[2].type() == json_value_type::null);
    BOOST_CHECK(!jobj["key1_"]);

    BOOST_REQUIRE(jobj["test"]);
    json_object test = jobj["test"]->get_object();
    BOOST_REQUIRE(test["k0l"]);
    BOOST_REQUIRE(test["k0r"]);
    BOOST_CHECK_EQUAL(*test["k0l"], *test["k0r"]);
    BOOST_CHECK(!(*test["k0l"] < *test["k0r"]));
    BOOST_CHECK(!(*test["k0r"] < *test["k0l"]));

    std::string etalon = "{"
        "\"$k0\": {\"%0\": 1, \"%1\": 2}, "
        "\"$k1\": [1, 2, null], "
        "\"key0\": null, "
        "\"key1\": false, "
        "\"key2\": true, "
        "\"key3\": 123, "
        "\"key4\": \"string value\", "
        "\"key4 - just a long key\": {}, "
        "\"key4 - just a long long key\": [], "
        "\"key5\": \"str\", "
        "\"key6\": -5.3, "
        "\"test\": {\"k0l\": [1, 2, 3], \"k0r\": [1, 2, 3]}"
        "}";
    BOOST_CHECK_EQUAL(to_string(res), etalon);
}
#endif

#if 1

void json_suite_test()
{
    char const* path = std::getenv("TESTS_HOME");
    fs::path suitedir{ (path ? fs::path(path) / "testdata" : fs::path("testdata")) / "json-test-suite" };

    std::for_each(fs::directory_iterator(suitedir), fs::directory_iterator(), [](auto const& p) {
        if (!fs::is_regular_file(p)) return;
        
        std::string fname = p.path().filename().string();

        size_t pos = fname.find(std::string(".json"));
        if (pos == std::string::npos || pos + 5 != fname.size()) return;
        
        std::string text;
        std::ifstream file(p.path().string().c_str());
        std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(text));

        parsers::json::model model;

        optional<std::string> error;
        try {
            parsers::parse<
                parsers::json::lexertl_lexer,
                parsers::json::parser
            >(model, text.c_str(), text.c_str() + text.size());
        } catch (exception const& e) {
            error = e.what();
        }

        if (fname[0] == 'y' && error) {
            BOOST_ERROR(fname + ", error: " + *error);
        } else if (fname[0] == 'n' && !error) {
            BOOST_ERROR(fname + " parsed, but must be rejected");
        } else if (fname[0] == 'i' && error) {
            BOOST_ERROR(fname + " not parsed, but may be accepted, error: "+ *error);
        }
        
        if (!error) {
            json_value res = model.detach_result();
            BOOST_CHECK(true);
        }
        // std::cout << p.path().filename() << "\n";
    });

}
#endif

void json_parser_test_registrar()
{
    register_test(BOOST_TEST_CASE(&json_test));
    register_test(BOOST_TEST_CASE(&json_suite_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(json_parser_test_registrar)
#endif
