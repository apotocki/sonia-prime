//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#define TEST_FOLDER "http_test"

#include <sstream>

#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/services.hpp"
#include "sonia/net/http/message.ipp"

using namespace sonia;

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
        "       io.serv: {"
        "           factory: 'io-factory',"
        "           layer: 16,"
        "           parameters: { threads: 4 }"
        "       },"
        "       io-cache.serv: {"
        "           factory: 'io-cache-factory',"
        "           layer: 16,"
        "           parameters: { tcp-socket-factory: 'io.serv', max-connection-count: 128, per-route-max-connection-count: 128 }"
        "       },"
        "       scheduler.serv: {"
        "           factory: 'scheduler-factory',"
        "           layer: 16,"
        "           parameters: { threads: 8, fibers: 10 }"
        "       },"
        "       net.serv: {"
        "           factory: 'net-server-factory',"
        "           layer : 16,"
        "           parameters : {"
        "               tcp-server-socket-factory: 'io.serv',"
        "               scheduler: 'scheduler.serv',"
        "               listeners: ["
        "                   { connector: 'http.serv', workers: 4, buffer-size: 65536, address: '127.0.0.1', port: 2222, type: 'tcp'}"
        "               ]"
        "           }"
        "       },"
        "       http.serv: {"
        "           factory: 'http-connector-factory',"
        "           layer : 16,"
        "           parameters: { keep-alive-max-count: 16, one-shot-max-count:16,"
        "               routes: [{application: 'web0', pathre: '/web0'}, {application: 'web1', pathre: '/web1'}]"
        "           }"
        "       },"
        "       http-connector-factory: { factory: 'prime', layer: 8, parameters: {name: 'http-server'} },"
        "       transceiver-factory: { factory: 'prime', layer: 8, parameters: {name: 'transceiver'} },"
        "       net-server-factory: { factory: 'prime', layer: 0, parameters: { name: 'net-server' } },"
        "       io-factory: { factory: 'prime', layer: 0, parameters: { name: 'io' } },"
        "       io-cache-factory: { factory: 'prime', layer: 0, parameters: { name: 'io-cache' } },"
        "       scheduler-factory: { factory: 'prime', layer: 0, parameters: { name: 'scheduler' } }"
        "   },"  
        "   bundles: {"
        "       prime: {lib: 'sonia-prime'}"
        "   }"
        "}";
}

}

#include <vector>
#include "applied/scoped_services.hpp"
#include "sonia/services/io/tcp_socket.hpp"
#include "sonia/services/io/sockets.hpp"
#include "sonia/net/http/message.hpp"

#include "sonia/net/http/application.hpp"

#include "sonia/net/uri.hpp"
#include "sonia/utility/serialization/serialization.hpp"
#include "sonia/utility/serialization/http_request.hpp"
#include "sonia/utility/serialization/http_response.hpp"
#include "sonia/utility/iterators/socket_write_input_iterator.hpp"
#include "sonia/utility/iterators/socket_read_input_iterator.hpp"
#include "sonia/utility/iterators/reference_wrapper_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

#if 1
class web0_application 
    : public sonia::http::application
    , public sonia::service
{
public:
    void handle(http::request & req, http::response & resp)
    {
        resp.content_writer = [](http::message::range_write_input_iterator it) {
            copy_range(string_view("web0"), it);
        };
    }
};

class web1_application 
    : public sonia::http::application
    , public sonia::service
{
public:
    void handle(http::request & req, http::response & resp)
    {
        resp.content_writer = [](http::message::range_write_input_iterator it) {
            copy_range(string_view("web1"), it);
        };
    }
};

BOOST_AUTO_TEST_CASE (http_service_test)
{
    scoped_services ss;

    services::register_service_factory("web0", []() -> shared_ptr<service> {
        shared_ptr<service> serv = make_shared<web0_application>();
        return serv;
    });

    services::register_service_factory("web1", []() -> shared_ptr<service> {
        shared_ptr<service> serv = make_shared<web1_application>();
        return serv;
    });

    std::stringstream cfgss;
    get_configuration(cfgss);
    services::load_configuration(cfgss);

    // start net service
    services::locate("net.serv");

    // make request
    char buff[128];
    auto sf = services::locate<io::tcp_socket_factory_type>("io-cache.serv");
    http::request req{"http://localhost:2222/web0"};
    io::tcp_socket sock = sf->create_connected_tcp_socket(req.host, req.port, sal::net_family_type::INET);
    socket_write_input_iterator<io::tcp_socket> oi{sock, array_view<char>(buff)};
    encode<serialization::default_t>(req, range_dereferencing_iterator(reference_wrapper_iterator(oi))).flush();
    oi.flush();

    // get response
    http::response resp;
    socket_read_input_iterator<io::tcp_socket> ii{sock, buff};
    decode<serialization::default_t>(range_dereferencing_iterator{reference_wrapper_iterator(ii)}, resp).flush();
    BOOST_CHECK_EQUAL(resp.status_code, http::status::OK);
    resp.build_input_iterator(ii);
    std::string r = resp.get_body_as_string();
    BOOST_CHECK_EQUAL(r, "web0"); 
}
#endif

#if 0
BOOST_AUTO_TEST_CASE (http_test)
{
    fs::remove_all(TEST_FOLDER);

    try {
        scoped_services ss;
        std::stringstream cfgss;
        get_configuration(cfgss);
        services::load_configuration(cfgss);
        auto sf = services::locate<io::tcp_socket_factory_type>("io-cache.serv");

        http::request req{"http://example.com/"};
        
        io::tcp_socket sock = sf->create_connected_tcp_socket(req.host, req.port, sal::net_family_type::UNSPEC);

        char buff[3];
        socket_write_buffered_iterator<io::tcp_socket> oi{sock, array_view<char>(buff)};

        encode<serialization::default_t>(req, range_dereferencing_iterator(reference_wrapper_iterator(oi))).flush();

        http::response resp;

        char buff2[10];

        socket_read_input_iterator<io::tcp_socket> ii{sock, buff2};

        decode<serialization::default_t>(range_dereferencing_iterator{reference_wrapper_iterator(ii)}, resp);

        for (auto const& hp : resp.headers) {
            for (auto const& hv : hp.second) {
                std::cout << http::message::header_name(hp.first) << " = " << hv << "\n";
            }
        }
        //range_dereferencing_iterator rit{reference_wrapper_iterator(ii)}, eit{reference_wrapper_iterator(eii)};
        //std::copy(rit, eit, std::ostreambuf_iterator<char>(std::cout));
        //std::fill(buff2, buff2 + sizeof(buff2), 0);
        //sock.read_some(buff2, sizeof(buff2));
        //std::cout << buff2;

    } catch (closed_exception const& e) {
        std::cout << e.what() << "\n";
    } catch (std::exception const& e) {
        std::cerr << e.what() << "\n";
        BOOST_CHECK(false);
    }
}
#endif
