//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "sonia/utility/parsers/json/lexertl_lexer.hpp"
#include "sonia/utility/parsers/json/basic_builder.hpp"
#include "sonia/utility/parsers/json/model.hpp"
#include "sonia/utility/parsers/json/parser.hpp"

using namespace sonia;

#include <fstream>
#include <iterator>

#include <boost/any.hpp>
#include <boost/unordered_map.hpp>

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

#if 0
BOOST_AUTO_TEST_CASE(json_parser_test)
{
    namespace fs = boost::filesystem;

    fs::path suitedir("data/json-test-suite");

    std::for_each(fs::directory_iterator(suitedir), fs::directory_iterator(), [](auto const& p) {
        if (!fs::is_regular_file(p)) return;
        
        std::string fname = p.path().filename().string();

        size_t pos = fname.find(std::string(".json"));
        if (pos == std::string::npos || pos + 5 != fname.size()) return;
        
        std::string text;
        std::ifstream file(p.path().string().c_str());
        std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(text));

        parsers::json::basic_builder b;
        parsers::json::model model(b);

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
            shared_ptr<json_value> res = model.detach_result();
        }
        // std::cout << p.path().filename() << "\n";
    });

}
#endif
