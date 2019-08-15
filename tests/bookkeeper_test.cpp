//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#define TEST_FOLDER "bookkeeper-test"

#include "sonia/config.hpp"
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "sonia/utility/scope_exit.hpp"

#include "applied/scoped_services.hpp"

#include "sonia/services/bookkeeper/bookkeeper.hpp"


namespace {

void get_configuration(std::ostream & os)
{
     os << 
        "{"
        "   hosts: ["
        "       {"
        "           name: 'default',"
        "           services: []"
        "       }"
        "   ],"
        "   services: {"
        "       bookkeeper.serv: {"
        "           factory: 'bookkeeper-factory',"
        "           layer: 4,"
        "           parameters: { dir: '" TEST_FOLDER "' }"
        "       },"
        "       bookkeeper-factory: { factory: 'prime', layer: 0, parameters: { name: 'bookkeeper' } }"
        "   },"
        "   bundles: {"
        "       prime: {lib: 'sonia-prime'}"
        "   }"
        "}";
}

}

BOOST_AUTO_TEST_CASE (bookkeeper_test)
{
    using namespace sonia;
    
    boost::filesystem::remove_all(TEST_FOLDER);

    json_value jsarr = {json_value(), json_value(true), json_value(false), json_value(42), json_value("string")};

    {
        scoped_services ss{"base-path=" TEST_FOLDER "/"};

        std::stringstream cfgss;
        get_configuration(cfgss);
        sonia::services::load_configuration(cfgss);

        auto bk = services::locate<services::bookkeeper>("bookkeeper.serv");

        BOOST_CHECK (!bk->get("bool_key0"));
        bk->set("bool_key0", json_value(true));
        bk->set("bool_key1", json_value(false));
        BOOST_CHECK_EQUAL(bk->get("bool_key0")->get_bool(), true);
        BOOST_CHECK_EQUAL(bk->get("bool_key1")->get_bool(), false);
        bk->erase("bool_key0");
        bk->set("bool_key1", json_value(true));
        BOOST_CHECK (!bk->get("bool_key0"));
        BOOST_CHECK_EQUAL(bk->get("bool_key1")->get_bool(), true);

        BOOST_CHECK(!bk->compare_and_swap("bool_key0", &json_value(false), &json_value("value0")));
        BOOST_CHECK (!bk->get("bool_key0"));

        BOOST_CHECK(bk->compare_and_swap("bool_key0", nullptr, &json_value("value1")));
        BOOST_CHECK_EQUAL(bk->get("bool_key0")->get_string(), "value1");

        BOOST_CHECK(!bk->compare_and_swap("bool_key1", nullptr, &json_value("value2")));
        BOOST_CHECK_EQUAL(bk->get("bool_key1")->get_bool(), true);

        BOOST_CHECK(!bk->compare_and_swap("bool_key1", &json_value(), &json_value("value3")));
        BOOST_CHECK_EQUAL(bk->get("bool_key1")->get_bool(), true);

        BOOST_CHECK(bk->compare_and_swap("bool_key1", &json_value(true), &json_value("value4")));
        BOOST_CHECK_EQUAL(bk->get("bool_key1")->get_string(), "value4");

        BOOST_CHECK(bk->compare_and_swap("arr_key0", nullptr, &jsarr));
        BOOST_CHECK_EQUAL(*bk->get("arr_key0"), jsarr);
    }
    {
        scoped_services ss{"base-path=" TEST_FOLDER "/"};

        std::stringstream cfgss;
        get_configuration(cfgss);
        sonia::services::load_configuration(cfgss);

        auto bk = services::locate<services::bookkeeper>("bookkeeper.serv");
        
        BOOST_CHECK_EQUAL(bk->get("bool_key0")->get_string(), "value1");
        BOOST_CHECK_EQUAL(bk->get("bool_key1")->get_string(), "value4");
        BOOST_CHECK_EQUAL(*bk->get("arr_key0"), jsarr);
    }
}
