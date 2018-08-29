//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "sonia/utility/parsers/json/lexertl_lexer.hpp"
#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"

using namespace sonia;

#include <fstream>
#include <iterator>

#include <boost/any.hpp>
#include <boost/unordered_map.hpp>

#if 0
class config {
public:
    template <typename T>
    T const& operator[](string_view key) const {
        auto it = values_.find(key, string_hasher(), string_equal_to());
        if (it != values_.end()) {
            return boost::any_cast<T const&>(it->second);
        }
        //throw exception(boost::format("key %1% is not found") % key);
        //throw exception(fmt("key %1% is not found %x") % key % 'a');
        throw exception(fmt("%1$#x") % (int)'a');
    }

private:
    boost::unordered_map<std::string, boost::any> values_;
};

#include <iostream>

BOOST_AUTO_TEST_CASE(just_test)
{
    config cfg;
    std::cout << cfg.operator[]<std::string>(string_view("key0")) << "\n";
}

#endif

#if 0
BOOST_AUTO_TEST_CASE(json_temp_test)
{
    namespace fs = boost::filesystem;
    std::string text;
    std::ifstream file("data/json_temp_test.json");
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(text));

    parsers::json::model model;

    parsers::parse<
        parsers::json::light_lexertl_lexer,
        parsers::json::parser
    >(model, text.c_str(), text.c_str() + text.size());

    json_value res = model.detach_result();
    auto v = to_string(res);
    std::cout << v << "\n";
}
#endif

#if 1
BOOST_AUTO_TEST_CASE(json_test)
{
    namespace fs = boost::filesystem;
    std::string text;
    std::ifstream file("data/json_test.json");
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(text));

    parsers::json::model model;

    parsers::parse<
        parsers::json::light_lexertl_lexer,
        parsers::json::parser
    >(model, text.c_str(), text.c_str() + text.size());
    
    json_value res = model.detach_result();
    BOOST_CHECK(res.type() == json_value_type::object);
    json_object jobj = res.get_object();
    BOOST_CHECK_EQUAL(jobj.size(), 11);
    jobj["key0"];
    BOOST_REQUIRE(jobj["key0"]);
    BOOST_CHECK(jobj["key0"]->type() == json_value_type::null);
    BOOST_CHECK(jobj["key1"]->type() == json_value_type::boolean);
    BOOST_CHECK(jobj["key2"]->type() == json_value_type::boolean);
    BOOST_CHECK(jobj["key3"]->type() == json_value_type::number);
    BOOST_CHECK(jobj["key4"]->type() == json_value_type::string);
    BOOST_CHECK(jobj["key5"]->type() == json_value_type::string);
    
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
        "\"test\": {\"k0l\": [1, 2, 3], \"k0r\": [1, 2, 3]}"
        "}";
    BOOST_CHECK_EQUAL(to_string(res), etalon);
}
#endif

#if 1

BOOST_AUTO_TEST_CASE(json_suite_test)
{
    namespace fs = boost::filesystem;

    fs::path suitedir("data/json-test-suite");
    //fs::path suitedir("data/temp");

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

