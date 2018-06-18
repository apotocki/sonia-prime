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

namespace sonia {

/*
class configuration_locator {
public:
    shared_ptr<std::istream> operator()(string_view cfgname)
};
*/


}

#include <iostream>
#include "sonia/application/application.hpp"

using namespace sonia;

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

std::string get_configuration() {
    std::stringstream is;
    is << 
        "{"
        "   'hosts': ["
        "       {"
        "           'name': 'h0',"
        "           'services': ['async.serv']"
        "       }"
        "   ]"
        "}";
    return std::move(is.str());
}

BOOST_AUTO_TEST_CASE (server_test)
{
    using namespace sonia;

    fs::remove_all(TEST_FOLDER);

    //std::cout << foo("chars str") << "\n";
    //std::cout << foo(std::string("std::str")) << "\n";
    //std::cout << foo(123) << "\n";
    application app;
    BOOST_CHECK_EQUAL(0, app.open(0, nullptr, &std::istringstream()));
    app.load_configuration(get_configuration());

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
}
