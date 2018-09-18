//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#define TEST_FOLDER "server_test"

#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"

#include "sonia/type_traits.hpp"

#include "sonia/services.hpp"

namespace sonia {

/*
class configuration_locator {
public:
    shared_ptr<std::istream> operator()(string_view cfgname)
};
*/


}

#include <iostream>

using namespace sonia;

#if 0
template <typename ... ArgT>
void list(ArgT && ... args) {
    ((std::cout << args << ", "), ...);
    std::cout << "\n";
}

template <typename ArgT>
std::string foo(ArgT && arg) {
    using namespace sonia;

    if constexpr (is_same_v<remove_cvref_t<ArgT>, std::string>) {
        return std::string(std::forward<ArgT>(arg));
    } else {
        std::ostringstream ss;
        ss << "streamed: " << std::forward<ArgT>(arg);
        return ss.str();
    }
}

#endif

std::string get_configuration() {
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

#include "applied/scoped_services.hpp"

class my_service {
public:
    virtual ~my_service() {}

    virtual std::string do_job() = 0;
};

class test_service0 
    : public service
    , public my_service
{
public:
    std::string do_job() override { return "job0"; }
};

BOOST_AUTO_TEST_CASE (server_test)
{
    fs::remove_all(TEST_FOLDER);

    try {
        scoped_services ss;

        services::register_service_factory("asd", []() -> service_descriptor {
            return {make_shared<test_service0>(), 0};
        });

        auto p = services::locate<my_service>("asd");
        BOOST_CHECK_EQUAL("job0", p->do_job());

        try {
            services::register_service_factory("asd", []() -> service_descriptor { throw 1; });
            BOOST_CHECK(false);
        } catch (internal_error const&) {}

        services::load_configuration("host.json");

        auto s0 = services::locate("scheduler.serv");

    } catch (shutdown_exception const& e) {
        std::cout << e.what() << "\n";
    } catch (std::exception const& e) {
        std::cerr << e.what() << "\n";
        BOOST_REQUIRE(false);
    }
    
#if 0
    //std::cout << foo("chars str") << "\n";
    //std::cout << foo(std::string("std::str")) << "\n";
    //std::cout << foo(123) << "\n";

    //BOOST_CHECK_EQUAL(0, app.open(0, nullptr, &std::istringstream()));
    //app.load_configuration(get_configuration());





    //app.load_host();
    //application_host host1;

    //list(1, 2, 3);

    /*
    {
        auto f = make_shared<server_default>(test_configuration_provider());
        BOOST_REQUIRE(!f->load(0, nullptr, "main", margot::applications::get_host_configuration_for_modification()));
        BOOST_REQUIRE(margot::applications::start_host());
        BOOST_SCOPE_EXIT(void) { margot::applications::finish_host(); } BOOST_SCOPE_EXIT_END
    }
    */
#endif
}
