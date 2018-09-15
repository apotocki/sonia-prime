//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/fiber/fiber.hpp>

#include "sonia/io/factory.hpp"

#include <iostream>

using namespace sonia;
namespace sio = sonia::io;

BOOST_AUTO_TEST_CASE (server_socket_test)
{
    shared_ptr<sonia::io::factory> f = make_shared<sio::factory>();
    f->open(8);

    io::tcp_acceptor soc = f->create_tcp_acceptor("localhost", 80);

    //sio::tcp_socket client_soc = soc.accept_and_read_some(to_array_view(buffer));

    std::vector<char> buffer(1024);
    soc.async_accept_and_read_some(to_array_view(buffer),
        [&buffer](std::error_code const& err, size_t rsz, sio::tcp_socket soc, io::tcp_acceptor::renew_functor const& rf) {
        size_t tsz = buffer.size();
            if (err) {
                GLOBAL_LOG_ERROR() << err.message();
            } else {
                buffer.resize(rsz);
                buffer.push_back(0);
                std::cout << string_view(&buffer.front()) << "\n";
                buffer.resize(tsz);
                rf(&buffer.front(), tsz);
            }
        }
    );

    std::string v;
    std::cin >> v;

    boost::fibers::fiber fib([&soc]() {
        soc.close();
    });
    fib.detach();
    //soc.close();
    f->close();

}

#if 0
BOOST_AUTO_TEST_CASE (socket_test)
{
    shared_ptr<sonia::io::factory> f = make_shared<sonia::io::factory>();
    f->open(8);

    io::tcp_socket soc = f->create_tcp_socket("rbc.ru", 80);

    std::string reqstr = "GET / HTTP/1.1\r\nHost: rbc.ru\r\n\r\n";
    size_t sz = soc.write_some(array_view<const char>(reqstr.c_str(), reqstr.size()));
    BOOST_CHECK_EQUAL(sz, reqstr.size());

    size_t rsz = 0;
    std::vector<char> buffer(1024);
    soc.async_read_some(array_view<char>(&buffer.front(), buffer.size()), [&buffer](std::error_code const& e, size_t rsz) {
        buffer.resize(rsz);
        buffer.push_back(0);
        std::cout << string_view(&buffer.front()) << "\n";
    });
    std::string v;
    std::cin >> v;
    /*
        size_t rsz = soc.read_some(array_view<char>(&buffer.front(), buffer.size()));
        buffer.resize(rsz);
        buffer.push_back(0);
        std::cout << string_view(&buffer.front()) << "\n";
    */
}
#endif