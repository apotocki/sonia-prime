/*=============================================================================
    Copyright (c) 2017 Avicomp
    Author: Alexander A Potocki
==============================================================================*/
#include "margot/config.hpp"
#include "tcp_connection.hpp"

#include <boost/bind.hpp>

#include "margot/log/logger.hpp"
#include "margot/components/buffer_chain.hpp"

#ifndef MARGOT_DEFAULT_TCP_OUTGOING_BUFFERED_THRESHOLD_SIZE
#   define MARGOT_DEFAULT_TCP_OUTGOING_BUFFERED_THRESHOLD_SIZE 1048576
#endif

namespace margot { namespace net {

tcp_connection::tcp_connection(margot::async::tcp_socket sock)
    : current_generation_(0)
    , commited_generation_(0)
    , read_generation_ahead_(1)
    , reading_(false)
    , buffered_size_(0)
    , write_buffer_size_threshold_(MARGOT_DEFAULT_TCP_OUTGOING_BUFFERED_THRESHOLD_SIZE)
{
    socket_ = std::move(sock);
}


void tcp_connection::do_read_unsafe()
{
    if (reading_ || (commited_generation_ + read_generation_ahead_ <= current_generation_)) {
        return;
    }

    tcp_read_buffer * buff = static_cast<tcp_read_buffer*>(read_buffer_.get());

    uint32_t us = buff->used_size();
    if (us == buff->size()) {
        chain_buffer_next<tcp_read_buffer>(read_buffer_);
        buff = static_cast<tcp_read_buffer*>(read_buffer_.get());

        if (!buff->generation()) {
            ++current_generation_;
            buff->generation() = current_generation_;
        }

        us = 0;
    }

    socket_.async_read_some(read_buffer_->begin() + us, read_buffer_->size() - us,
        boost::bind(&tcp_connection::read_block_done, shared_from_this(), _1, _2)
    );

    reading_ = true;
}

void tcp_connection::read_block_done(boost::system::error_code const& err, size_t sz)
{
    if (err) {
        read_error_ = err;
        MARGOT_GLOBAL_LOG_TRACE() << "socket error: " << read_error_->message();
    } else {
        boost::lock_guard<mutex> read_guard(read_mtx_);
        static_cast<tcp_read_buffer&>(*read_buffer_).append_used_size((uint32_t)sz);
        reading_ = false;
        do_read_unsafe();
        read_condition_.notify_all();
    }
}

tcpsocket_read_input_iterator & tcp_connection::read_iterator()
{
    if (!read_it_) {
        read_buffer_ = make_chain_buffer<tcp_read_buffer, 65536>(std::allocator<void>());
        read_it_.emplace(shared_from_this(), read_buffer_);
        
        boost::lock_guard<mutex> read_guard(read_mtx_);
        do_read_unsafe();
    }
    return *read_it_;
}

tcpsocket_write_output_iterator tcp_connection::write_iterator()
{
    return tcpsocket_write_output_iterator(shared_from_this());
}

void tcp_connection::next_buffer(buffer<uint8_t> & buff)
{
    tcp_read_buffer* rb = static_cast<tcp_read_buffer*>(buff.raw().get());
    boost::unique_lock<mutex> read_lock(read_mtx_);
    while (rb->generation() < current_generation_) {
        uint32_t next_offset = (uint32_t)(buff.end() - rb->begin());
        if (next_offset < rb->size()) {
            buff.raw_offset() = next_offset;
            buff.raw_size() = rb->size() - next_offset;
            return;
        }
        // retrieve next buffer
        chain_buffer_next<tcp_read_buffer>(buff.raw());
        rb = static_cast<tcp_read_buffer*>(buff.raw().get());
        if (!rb->generation()) { // async read op hasn't started yet
            ++current_generation_;
            rb->generation() = current_generation_;
        }

        if (commited_generation_ < rb->generation()) {
            commited_generation_ = rb->generation();
            do_read_unsafe();
        }
        buff.raw_offset() = buff.raw_size() = 0;
    }
    uint32_t next_offset = (uint32_t)(buff.end() - rb->begin());
    for (;;)
    {
        if (next_offset < rb->used_size()) {
            buff.raw_offset() = next_offset;
            buff.raw_size() = rb->used_size() - next_offset;
            return;
        }
        if (read_error_) {
            buff.raw_size() = 0;
            return;
        }
    
        // wait for data readiness
        read_condition_.wait(read_lock);
    }
}

void tcp_connection::push_buffer(buffer<uint8_t> buff)
{
    boost::unique_lock<boost::mutex> lock(write_mtx_);

    while (!write_error_ && buffered_size_ > write_buffer_size_threshold_) {
        push_condition_.wait(lock);
    }

    if (write_error_) return;

    write_buffers_.push_back(std::move(buff));
    buffered_size_ += write_buffers_.back().size();

    if (1 == write_buffers_.size()) {
        //MARGOT_GLOBAL_LOG_TRACE() << "write into socket: " << std::string(buffers_.front()->begin(), buffers_.front()->end()) << "<--";
        socket_.async_write(write_buffers_.front().begin(), write_buffers_.front().size(),
            boost::bind(&tcp_connection::write_block_done, this, _1, _2)
        );
    }
}

void tcp_connection::write_block_done(boost::system::error_code const& err, size_t sz) {
    boost::unique_lock<boost::mutex> lock(write_mtx_);
    buffered_size_ -= write_buffers_.front().size();
    //MARGOT_GLOBAL_LOG_TRACE() << "written into socket: " << std::string(buffers_.front()->begin(), buffers_.front()->begin() + sz) << "<--";
    if (err) {
        write_buffers_.clear();
        if (!write_error_) {
            write_error_ = err;
        }
    } else {
        write_buffers_.pop_front();
        if (!write_buffers_.empty()) {
            socket_.async_write(write_buffers_.front().begin(), write_buffers_.front().size(),
                boost::bind(&tcp_connection::write_block_done, this, _1, _2)
            );
        }
    }
    push_condition_.notify_all();
}

void tcp_connection::close()
{
    socket_.close();
}

tcpsocket_read_input_iterator::tcpsocket_read_input_iterator(shared_ptr<tcp_connection> conn, shared_buffer_ptr<uint8_t> buff)
    : buffer_(buff, 0u, 0u), conn_(conn)
{}

bool tcpsocket_read_input_iterator::empty() const
{
    if (!buffer_.empty()) return false;
    auto conn = conn_.lock();
    if (!conn) return true;
    conn->next_buffer(buffer_);
    return buffer_.empty();
}

void tcpsocket_read_input_iterator::increment() {
    BOOST_VERIFY(!empty());
    auto conn = conn_.lock();
    if (conn) {
        conn->next_buffer(buffer_);
    }
}

tcpsocket_write_output_iterator::tcpsocket_write_output_iterator(shared_ptr<tcp_connection> conn)
    : conn_(std::move(conn))
{

}

bool tcpsocket_write_output_iterator::empty() const
{
    auto conn = conn_.lock();
    if (!conn) return false;
    return !!conn->write_error_;
}

void tcpsocket_write_output_iterator::increment()
{
    auto conn = conn_.lock();
    if (conn) {
        conn->push_buffer(std::move(buffer_));
    }
}

#if 0
void tcpsocket_read_input_iterator::increment() {
    if (buffer_->size() != buffer_->max_size()) {
        offset_ = buffer_->size();
        soc_->async_read_some(buffer_->begin() + offset_, buffer_->max_size() - offset_,
            boost::bind(&tcpsocket_read_input_iterator::read_block_done, this, _1, _2)
        );
    } else {
        boost::lock_guard<mutex> guard(buffer_->mtx());
        cow_ptr<tcp_read_buffer> next = buffer_->next();
        if (!next) {
            buffer_.cow();
        }

        if (next) {
            buffer_ = next;
        }

    }

    //soc_->async_read_some(value_->begin(), value_->max_size(),
    //    boost::bind(&tcpsocket_read_output_iterator::read_block_done, this, _1, _2)
    //);
}

void tcpsocket_read_input_iterator::read_block_done(boost::system::error_code const& err, size_t sz) const {
    boost::unique_lock<boost::mutex> lock(read_mutex_);
    if (err) {
        error_ = err;
        MARGOT_GLOBAL_LOG_TRACE() << "socket error: " << error_->message();
        buffer_->set_size(offset_);
    } else {
        buffer_->set_size(sz);
    }
    unknown_state_ = false;
    read_condition_.notify_all();
}

#endif

}}
