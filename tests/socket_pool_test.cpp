/*=============================================================================
    Copyright (c) 2017 Avicomp
    Author: Alexander A Potocki
==============================================================================*/
#define TEST_FOLDER "sockeet-pool-test"
//============================================================================
#include "margot/config.hpp"

#include <boost/test/unit_test.hpp>

#include "margot/shared_ptr.hpp"
#include "margot/components/async/socket_pool.hpp"

#include "margot/exceptions/not_implemented_error.hpp"

namespace {

using namespace margot;

class raw_socket
{
    async::socket::type type_;

public:
    explicit raw_socket(async::socket::type t) : type_(t) {}
};

class dummy_socket_factory 
    : public margot::async::socket_factory
    , public margot::async::socket_access
    , public enable_shared_from_this<dummy_socket_factory>
{
    typedef async::socket socket;

public:
    socket create_socket(socket::type t) override final
    {
        return socket(new raw_socket(t), shared_from_this(), t);
    }

    long socket_count(socket::type) const override final { return 0; }

    // listener socket
    socket create_udpsocket(string_ref local_address, uint16_t localport) { throw not_implemented_error(); }

    // socket_access interface
    void socket_start(socket& socket) override final {}
    void socket_kill(socket& socket) override final { delete &socket.cast<raw_socket>(); }
    void socket_shutdown(socket& socket) override final {}
    void socket_close(socket& socket) override final {}
    void socket_connect(socket& socket, string_ref dest_address, int dest_port) override final {}
    size_t socket_read_some(async::socket& socket, void * buff, size_t sz, boost::system::error_code & err) override final { return 0; }
    void socket_async_read(async::socket& socket, void * buff, size_t sz, function<void(const boost::system::error_code&, std::size_t)> const& ftor) override final {}
    void socket_async_read_some(async::socket& socket, void * buff, size_t sz, function<void(const boost::system::error_code&, std::size_t)> const& ftor) override final {}
    void socket_write(async::socket& socket, void const* buff, size_t sz, boost::system::error_code & err) override final {}
    void socket_async_write(async::socket& socket, void const* buff, size_t sz, function<void(const boost::system::error_code&, std::size_t)> const& ftor) override final {}
    void socket_async_write_some(async::socket& socket, void const* buff, size_t sz, function<void(const boost::system::error_code&, std::size_t)> const& ftor) override final {}

    bool socket_is_open(socket const& soc) const override final { return !soc.empty(); }
    std::string socket_remote_address(async::socket const& socket) const override final { return ""; }
};

class socket_pool_impl : public margot::async::socket_pool
{
    shared_ptr<dummy_socket_factory> sf_;

public:
    socket_pool_impl() { sf_ = make_shared<dummy_socket_factory>(); }

   shared_ptr<margot::async::socket_access> get_socket_access() const override final { return sf_; }

    margot::async::socket_factory & get_socket_factory() const override final { return *sf_; }
    
    long max_pooled_socket_count() const override final
    {
        return 20;
    }
    
    long max_route_socket_count(string_ref, uint16_t) const override final
    {
        return 10;
    }
    
    boost::chrono::microseconds get_pooled_socket_timeout() const override final
    {
        return boost::chrono::microseconds(1000);
    }
};

}

#include <boost/thread.hpp>
#include "margot/atomic.hpp"

template <typename LockingF, typename UnlockingF>
bool check_lock_for(LockingF const& lf, UnlockingF const& ulf, boost::chrono::microseconds ival)
{
    atomic<bool>
    boost::thread ctlt([ival, &ulf]() {
        boost::thread::sleep(ival);
        ulf();
    });
    ctlt.start_thread();

    lf();
    
}

BOOST_AUTO_TEST_CASE(socket_pool_test)
{
    using margot::async::socket;

    socket_pool_impl sp;
    sp.open();

    socket s0 = sp.create_socket("address0", 1, socket::type::TCP);

}
