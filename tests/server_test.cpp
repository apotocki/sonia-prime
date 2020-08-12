//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#define TEST_FOLDER "server_test"

#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/services.hpp"

using namespace sonia;

namespace {

std::string get_configuration()
{
    std::stringstream is;
    is <<
        "{"
        "   'hosts': ["
        "       {"
        "           name: 'h0',"
        "           services: ['async.serv', 'async2.serv']"
        "       }"
        "   ],"
        "   'factories': ["
        "       {"
        "           name: 'async.serv',"
        "           implementation: 'async',"
        "           layer: 16,"
        "           parameters: {"
        "               threads: 40"
        "           }"
        "       },"
        "       {"
        "           name: 'async2.serv',"
        "           implementation: 'async',"
        "           layer: 8,"
        "           parameters: {"
        "               threads: 40"
        "           }"
        "       }"
        "   ]"
        "}";
    return std::move(is.str());
}

class my_service
{
public:
    virtual ~my_service() = default;
    virtual std::string do_job() = 0;
};

class test_service0 
    : public service
    , public my_service
{
public:
    std::string do_job() override { return "job0"; }
};

}

#include "applied/scoped_services.hpp"

BOOST_AUTO_TEST_CASE (server_test)
{
    fs::remove_all(TEST_FOLDER);

    try {
        scoped_services ss{"base-path=" TEST_FOLDER "/"};

        services::register_service_factory("asd", []() -> shared_ptr<service> {
            return make_shared<test_service0>();
        });

        auto p = services::locate<my_service>("asd");
        BOOST_CHECK_EQUAL("job0", p->do_job());

        try {
            services::register_service_factory("asd", []() -> shared_ptr<service> { throw 1; });
            BOOST_CHECK(false);
        } catch (internal_error const&) {}

        services::load_configuration("host.json");

        auto s0 = services::locate("scheduler.serv");
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));

    } catch (closed_exception const& e) {
        std::cout << e.what() << "\n";
    } catch (std::exception const& e) {
        std::cerr << e.what() << "\n";
        BOOST_REQUIRE(false);
    }
}
