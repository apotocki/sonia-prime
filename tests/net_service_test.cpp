//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//============================================================================
#include "sonia/config.hpp"
//#include "sonia/utility/windows.hpp"
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <boost/scope_exit.hpp>
#include <boost/thread/condition_variable.hpp>

#include "applied/scoped_services.hpp"
#include "sonia/services/io/tcp_socket.hpp"
#include "sonia/services/io/udp_socket.hpp"
#include "sonia/services/io/sockets.hpp"

using namespace sonia;

namespace {

void get_configuration(std::ostream & os)
{
    os <<
        "{"
        "   hosts: ["
        "       {"
        "           name: 'server',"
        "           services: ['net.serv']"
        "       }"
        "   ],"
        "   services: {"
        "       io.serv: {"
        "           factory: 'io-factory',"
        "           layer : 16,"
        "           parameters : {"
        "               threads: 8"
        "           }"
        "       },"
        "       scheduler.serv: {"
        "           factory: 'scheduler-factory',"
        "           layer : 16,"
        "           parameters : {"
        "               threads: 8,"
        "               fibers: 16"
        "           }"
        "       },"
        "       net.serv: {"
        "           factory: 'net-server-factory',"
        "           layer : 16,"
        "           parameters : {"
        "               tcp-factory: 'io.serv',"
        "               udp-factory: 'io.serv',"
//        "               scheduler: 'scheduler.serv',"
        "               listeners: ["
//        "                   { connector: 'echo.serv', address: '0.0.0.0', port: 2223, type: 'ssl'},"
 //       "                   { connector: 'echo.serv', address: '0.0.0.0', port: 2224, type: 'udp'},"
        "                   { connector: 'echo.serv', address: '0.0.0.0', port: 2222, type: 'tcp'}"
        "               ]"
        "           }"
        "       },"
        "       echo.serv: {"
        "           factory: 'echo-connector-factory',"
        "           layer : 16,"
        "           parameters : {"
        "           }"
        "       },"
        // factories
        "       scheduler-factory: {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'scheduler' }"
        "       },"
        "       io-factory: {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'io' }"
        "       },"
        "       net-server-factory: {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'net-server' }"
        "       },"
        "       echo-connector-factory: {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'echo' }"
        "       }"
        "   },"
        "   bundles: {"
        "       prime: {lib: 'sonia-prime'}"
        "   }"
        "}";
}

#if 0
struct test_conn
{
    mutex mtx;
    boost::condition_variable cvar;
    bool rflag;
    std::vector<char> buffer;

    test_conn() : rflag(false), buffer(1024)
    {
        std::fill_n(buffer.begin(), buffer.size(), 0);
    }

    void notify(size_t sz) 
    {
        boost::unique_lock lock(mtx);
        rflag = true;
        buffer.resize(sz);
        cvar.notify_one();
    }

    void wait()
    {
        boost::unique_lock lock(mtx);
        while (!rflag) {
            cvar.wait(lock);
        }
    }
};

void echo_test(tcp_socket soc)
{
    // sync test
    std::string test_str("This is a test meassage.");
    soc.write(to_array_ref(test_str));

    std::vector<char> result(1024);
    size_t sz = soc.read_some(to_array_ref(result));
    BOOST_REQUIRE(sz);
    BOOST_CHECK_EQUAL(test_str, std::string(&result[0], sz));

    // async test
    test_conn conn;
    soc.async_write(to_array_ref(test_str), [&conn, &soc](boost::system::error_code const& err, size_t sz) {
        if (!sz) {
            conn.notify(sz);
        } else {
            soc.async_read_some(to_array_ref(conn.buffer), [&conn, &soc](boost::system::error_code const& err, size_t sz) {
                conn.notify(sz);
            });
        }
    });

    conn.wait();
    BOOST_CHECK_EQUAL(test_str, std::string(&conn.buffer[0], conn.buffer.size()));
}

#endif

void udp_echo_test(io::udp_socket soc)
{
    // sync test
    std::string test_str("This is a test meassage.");

    soc.write_some("127.0.0.1", 2224, to_array_view(test_str));
    std::vector<char> result(1024);
    size_t sz = soc.read_some(to_array_view(result));
    BOOST_REQUIRE(sz);
    BOOST_CHECK_EQUAL(test_str, std::string(&result[0], sz));

    // async test
    //test_conn conn;
    //soc.async_write(to_array_ref(test_str), ep, [&conn, &soc, &ep](boost::system::error_code const& err, size_t sz) {
    //    if (!sz) {
    //        conn.notify(sz);
    //    } else {
    //        soc.async_read_some(to_array_ref(conn.buffer), [&conn, &soc, &ep](boost::system::error_code const& err, udp_endpoint rep, size_t sz) {
    //            BOOST_ASSERT(ep == rep);
    //            conn.notify(sz);
    //        });
    //    }
    //});

    //conn.wait();
    //BOOST_CHECK_EQUAL(test_str, std::string(&conn.buffer[0], conn.buffer.size()));
}

void tcp_echo_test(io::tcp_socket soc)
{
    uint8_t buff[1024];
    for (size_t idx = 0; idx < 1024; ++idx) buff[idx] = (uint8_t)(0xff & idx);
    size_t sz = soc.write_some(buff, sizeof(buff));
    BOOST_CHECK_EQUAL(sz, sizeof(buff));

    size_t test_sz = 1024;
    size_t tested_sz = 0;

    uint8_t rbuff[961];
    while (tested_sz != test_sz) {
        size_t rsz = soc.read_some(rbuff, sizeof(rbuff));
        if (!rsz) throw exception("echo mismatch, no enough data");
        for (uint8_t c : array_view(rbuff, rsz)) {
            BOOST_ASSERT(c == buff[tested_sz % 1024]);
            ++tested_sz;
        }
    }

    /*
    auto conn = make_shared<io::tcp_connection>(std::move(soc));
    auto wit = conn->write_iterator();
    for (size_t idx = 0; idx < 1024; ++idx) {
        *wit = buff;
        ++wit;
    }
    size_t test_sz = 1024 * 1024;
    auto it = conn->read_iterator();
    size_t tested_sz = 0;
    while (tested_sz != test_sz && !it.empty()) {
        array_view<uint8_t> arr = *it;
        for (uint8_t c : arr) {
            BOOST_ASSERT(c == buff[tested_sz%1024]);
            ++tested_sz;
        }
        if (tested_sz == test_sz) {
            break;
        }
        ++it;
    }
    
    conn->close();
    */
}

}

BOOST_AUTO_TEST_CASE( net_service_test )
{
    scoped_services ss;

    std::stringstream cfgss;
    get_configuration(cfgss);
    services::load_configuration(cfgss);

    std::string test_str("This is a test meassage.");
    auto tcpfactory = services::locate<io::tcp_socket_factory_type>("io.serv");

#if 0
    // test tcp connection
    auto soc = tcpfactory->create_tcp_socket("127.0.0.1", 2222);
    try {
        tcp_echo_test(std::move(soc));
    } catch (std::exception const& e) {
        BOOST_CHECK_MESSAGE(false, e.what());
    }
#endif
#if 0
    soc = tcpfactory->create_tcp_socket("127.0.0.1", 2222);
    try {
        tcp_echo_test(std::move(soc));
    } catch (std::exception const& e) {
        BOOST_CHECK_MESSAGE(false, e.what());
    }
#endif
#if 0
    auto udpfactory = services::locate<io::udp_socket_factory_type>("io.serv");
    auto udpsoc = udpfactory->create_udp_socket();
    udp_echo_test(std::move(udpsoc));

    //// test ssl connection
    //soc = socfactory->create_tcp_socket("127.0.0.1", 2223, tcp_socket_type::SSL);
    //tcp_conn_echo_test(std::move(soc));

    // test tcp
    //auto soc = socfactory->create_tcp_socket("127.0.0.1", 2222);
    //echo_test(std::move(soc));

    //// test ssl
    //soc = socfactory->create_tcp_socket("127.0.0.1", 2223, tcp_socket_type::SSL);
    //echo_test(std::move(soc));
#endif

}
