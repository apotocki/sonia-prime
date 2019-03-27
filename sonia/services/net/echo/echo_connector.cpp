//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "echo_connector.hpp"

#include "sonia/exceptions.hpp"

#include "sonia/utility/iterators/socket_write_iterator.hpp"
#include "sonia/utility/iterators/socket_read_iterator.hpp"

namespace sonia::services {

using sonia::io::tcp_socket;

echo_connector::echo_connector(echo_connector_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "echo-connector");
}

void echo_connector::open()
{}

void echo_connector::close() noexcept
{}

void echo_connector::connect(array_view<char> buff, size_t rsz, sonia::io::tcp_socket soc)
{
    socket_write_iterator wit{soc};
    socket_read_iterator rit{soc};

    try {
        array_view<const char> readybuff = buff.subview(0, rsz);
        while (!wit.empty() && !rit.empty()) {
            *wit = readybuff; ++wit;
            *rit = buff; ++rit;
            readybuff = *rit;
        }
    } catch (...) {}
}

void echo_connector::connect(array_view<char> buff, size_t rsz, sonia::sal::socket_address const& addr, sonia::io::udp_socket & soc)
{
    LOG_TRACE(logger()) << "udp connection from: " << addr.str();
    BOOST_VERIFY(rsz == soc.write_some(addr, buff.subview(0, rsz)));
}

#if 0
void echo_connector::connect(buff_ptr buff, size_t sz, sonia::io::tcp_socket soc)
{
    socket_write_iterator wit{soc};
    socket_read_iterator rit{soc};
    //tcp_socket_read_iterator r(buff, sz, soc);

    try {
        auto readybuff = std::as_const(*buff).subview(0, sz);
        while (!wit.empty() && !rit.empty()) {
            *wit = readybuff; ++wit;
            *rit = buff->to_array_view(); ++rit;
            readybuff = *rit;
        }
    } catch (...) {}
}

void echo_connector::connect(buff_ptr buff, size_t sz, sonia::io::socket_address const& addr, sonia::io::udp_weak_socket soc)
{
    LOG_TRACE(logger()) << "udp connection from: " << addr.str();
    BOOST_VERIFY(sz == soc.write_some(addr, buff->subview(0, sz)));
}
#endif

}
