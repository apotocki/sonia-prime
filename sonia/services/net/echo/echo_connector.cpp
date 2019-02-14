//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "echo_connector.hpp"

#include "sonia/exceptions.hpp"

#include "sonia/utility/iterators/socket_write_iterator.hpp"
#include "sonia/utility/iterators/socket_read_iterator.hpp"

namespace sonia { namespace services {

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

void echo_connector::connect(buff_ptr buff, size_t sz, sonia::io::tcp_socket soc)
{
    socket_write_iterator wit{soc};
    socket_read_iterator rit{soc};
    //tcp_socket_read_iterator r(buff, sz, soc);

    try {
        array_view<const char> readybuff{buff->cbegin(), sz};
        while (!wit.empty() && !rit.empty()) {
            *wit = readybuff; ++wit;
            *rit = buff->to_array_view(); ++rit;
            readybuff = *rit;
        }
    } catch (...) {}
}

}}
