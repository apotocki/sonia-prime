/*=============================================================================
    Copyright (c) 2017 Avicomp
    Author: Alexander A Potocki
==============================================================================*/
#ifndef MARGOT_COMPONENTS_NET_TCP_CONNECTION_HPP
#define MARGOT_COMPONENTS_NET_TCP_CONNECTION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <deque>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/thread/condition_variable.hpp>

#include "margot/optional.hpp"
#include "margot/shared_ptr.hpp"
#include "margot/mutex.hpp"
#include "margot/components/buffer.hpp"

#include "margot/components/async/socket.hpp"

namespace margot { namespace net {

class tcp_connection;

class tcpsocket_read_input_iterator
    : public boost::iterator_facade
        < tcpsocket_read_input_iterator
        , array_ref<uint8_t>
        , std::forward_iterator_tag
        , array_ref<uint8_t>>
{
    friend class boost::iterator_core_access;

    bool equal(tcpsocket_read_input_iterator const& rhs) const { return buffer_ == rhs.buffer_; }

    array_ref<uint8_t> dereference() const { return buffer_.get_array_ref(); }

    void increment();

public:
    tcpsocket_read_input_iterator(shared_ptr<tcp_connection>, shared_buffer_ptr<uint8_t>);

    bool empty() const;

private:
    void read_block_done(boost::system::error_code const& err, size_t sz) const;

private:
    mutable buffer<uint8_t> buffer_;
    weak_ptr<tcp_connection> conn_;
};

class tcpsocket_write_output_iterator
    : public boost::iterator_facade
        < tcpsocket_write_output_iterator
        , buffer<uint8_t>
        , std::output_iterator_tag>
{
    friend class boost::iterator_core_access;

    bool equal(tcpsocket_write_output_iterator const&) const { return false; }
    buffer<uint8_t> & dereference() const { return buffer_; }

    void increment();

public:
    explicit tcpsocket_write_output_iterator(shared_ptr<tcp_connection>);

    bool empty() const;

private:
    mutable buffer<uint8_t> buffer_;
    weak_ptr<tcp_connection> conn_;
};

class tcp_read_buffer
    : public shared_buffer<uint8_t>
{
public:
    template <typename ... ArgsT>
    explicit tcp_read_buffer(ArgsT&& ... args)
        : shared_buffer<uint8_t>(std::forward<ArgsT>(args) ...)
        , used_size_(0), generation_(0)
    {

    }

    mutex & mtx() const { return mtx_; }

    uint32_t used_size() const { return used_size_; }
    uint32_t generation() const { return generation_; }
    uint32_t& generation() { return generation_; }

    void append_used_size(uint32_t sz) {
        used_size_ += sz;
    }

private:
    mutable mutex mtx_;
    uint32_t used_size_;
    uint32_t generation_;
};

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
    friend class tcpsocket_read_input_iterator;
    friend class tcpsocket_write_output_iterator;

public:
    explicit tcp_connection(margot::async::tcp_socket sock);
    virtual ~tcp_connection() {}

    tcpsocket_read_input_iterator & read_iterator();
    tcpsocket_write_output_iterator write_iterator();

    virtual void close();
   
protected:
    // read ops
    void next_buffer(buffer<uint8_t> &);

    void do_read_unsafe();
    void read_block_done(boost::system::error_code const& err, size_t sz);

    // write ops
    void push_buffer(buffer<uint8_t>);
    void write_block_done(boost::system::error_code const& err, size_t sz);

private:
    mutex read_mtx_, write_mtx_;
    margot::async::tcp_socket socket_;
    optional<tcpsocket_read_input_iterator> read_it_;
    shared_buffer_ptr<uint8_t> read_buffer_;
    uint32_t current_generation_;
    uint32_t commited_generation_;
    uint32_t read_generation_ahead_;

    mutable boost::condition_variable read_condition_;
    optional<boost::system::error_code> read_error_, write_error_;
    bool reading_;

    std::deque<buffer<uint8_t>> write_buffers_;
    boost::condition_variable push_condition_;
    size_t buffered_size_;
    size_t write_buffer_size_threshold_;
};

}}

#endif // MARGOT_COMPONENTS_NET_TCP_CONNECTION_HPP
