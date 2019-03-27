//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "transceiver_service.hpp"

#include <vector>

#include <boost/crc.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "sonia/services.hpp"

#include "sonia/services/io/socket_address.hpp"
#include "sonia/exceptions/internal_errors.hpp"
#include "sonia/utility/serialization/string.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::services {

using namespace sonia::io;

struct chunk_header
{
    uint8_t crc[4];
    uint16_t size;
};

class chunk_write_iterator
    : public boost::iterator_facade<
          chunk_write_iterator
        , array_view<char>
        , std::output_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<chunk_write_iterator const*, array_view<char>>>
    >
{
    using proxy_type = wrapper_iterator_proxy<ptr_proxy_wrapper<chunk_write_iterator const*, array_view<char>>>;

    friend class boost::iterator_core_access;
    template <class, class> friend class sonia::ptr_proxy_wrapper;

    bool equal(chunk_write_iterator const& rhs) const { return false; }

    proxy_type dereference() const { return proxy_type(this); }
    
    array_view<char> get_dereference() const
    {
        return array_view(buff_.begin() + sizeof(chunk_header), datasz_);
    }

    void set_dereference(array_view<char> span)
    {
        BOOST_ASSERT(span.begin() == buff_.begin() + sizeof(chunk_header));
        datasz_ = span.size();
    }

    void increment()
    {
        flush();
    }

public:
    chunk_write_iterator(chunk_write_iterator const&) = delete;
    chunk_write_iterator(chunk_write_iterator &&) = default;
    chunk_write_iterator& operator=(chunk_write_iterator const&) = delete;
    chunk_write_iterator& operator=(chunk_write_iterator &&) = default;

    chunk_write_iterator(tcp_socket & soc, array_view<char> buff) : soc_(soc), buff_(buff)
    {
        BOOST_ASSERT(buff.size() > sizeof(chunk_header));
        BOOST_ASSERT(buff.size() <= static_cast<size_t>((std::numeric_limits<uint16_t>::max)()) + sizeof(chunk_header));
        datasz_ = buff_.size() - sizeof(chunk_header);
    }

    void flush()
    {
        if (datasz_) {
            uint32_t crc = boost::crc<32, 0xBEEF, 0xC001F00D, 0, true, true>(buff_.begin() + sizeof(chunk_header), datasz_);
            make_encoder<sonia::serialization::ordered_t>(buff_.begin()) & crc & (uint16_t)datasz_;
            
            size_t sz2wr = sizeof(chunk_header) + datasz_;
            char const* bf = buff_.begin();
            for (;;) {
                auto expsz = soc_.write_some(bf, sz2wr);
                if (!expsz.has_value() || !expsz.value()) throw eof_exception();
                sz2wr -= expsz.value();
                if (!sz2wr) break;
                bf += expsz.value();
            }
        }
        datasz_ = buff_.size() - sizeof(chunk_header);
    }

private:
    tcp_socket & soc_;
    array_view<char> buff_;
    size_t datasz_;
};

class chunk_read_iterator
    : public boost::iterator_facade<
          chunk_read_iterator
        , array_view<const char>
        , boost::forward_traversal_tag
        , const array_view<const char>
    >
{
    friend class boost::iterator_core_access;

    bool equal(chunk_read_iterator const& rhs) const { return false; }

    void read(size_t limit) const
    {
        while (rdsz_ < limit) {
            size_t sz2rd = buff_.size() - rdsz_;
            if (!sz2rd) {
                THROW_INTERNAL_ERROR("the buffer is too small");
            }
            char * bf = buff_.begin() + rdsz_;
            auto expsz = soc_.read_some(bf, sz2rd);
            if (!expsz.has_value() || !expsz.value()) {
                throw eof_exception();
            }
            rdsz_ += expsz.value();
        }
    }

    array_view<const char> dereference() const
    {
        if (data_.empty()) {
            read(sizeof(chunk_header));

            uint32_t crc;
            uint16_t datasz;
            make_decoder<sonia::serialization::ordered_t>(buff_.cbegin()) & crc & datasz;

            read(datasz + sizeof(chunk_header));
            uint32_t rcrc = boost::crc<32, 0xBEEF, 0xC001F00D, 0, true, true>(buff_.begin() + sizeof(chunk_header), datasz);
            if (crc != rcrc) {
                throw exception("crc inconsistancy : %1% instead of %2%"_fmt % crc % rcrc);
            }
            data_ = buff_.subview(sizeof(chunk_header), datasz);
        }
        return data_;
    }

    void increment()
    {
        size_t hsz = static_cast<size_t>(data_.end() - buff_.begin()); // handled size
        if (hsz < rdsz_) {
            rdsz_ -= data_.size() + sizeof(chunk_header);
            std::memmove(buff_.begin(), data_.end(), rdsz_);
        } else {
            rdsz_ = 0;
        }
        data_.reset();
    }

public:
    chunk_read_iterator(chunk_read_iterator const&) = delete;
    chunk_read_iterator(chunk_read_iterator &&) = default;
    chunk_read_iterator& operator=(chunk_read_iterator const&) = delete;
    chunk_read_iterator& operator=(chunk_read_iterator &&) = default;

    chunk_read_iterator(tcp_socket & soc, array_view<char> buff, size_t rdsz = 0)
        : soc_(soc), buff_(buff), rdsz_(rdsz)
    { }

private:
    tcp_socket & soc_;
    array_view<char> buff_;
    mutable size_t rdsz_;
    mutable array_view<const char> data_;
};

transceiver_service::transceiver_service(transceiver_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "transceiver");
}

void transceiver_service::close() noexcept
{
    lock_guard guard(mtx);
    closed_ = true;
    for (auto & soc : using_set_) {
        soc.close();
    }
}

void transceiver_service::transmit_and_receive(string_view dest, serializable_proxy_object & obj)
{
    if (!soc_factory_) {
        sonia::services::locate(cfg_.socket_factory, soc_factory_);
    }

    auto [protocol, address, port] = sonia::io::parse_address(dest);

    auto soc = soc_factory_->create_connected_tcp_socket(address, port);
    
    char tmp_buff[65536];

    output_iterator_polymorpic_adapter<
        chunk_write_iterator,
        boost::forward_traversal_tag,
        array_view<char>,
        array_view<char>
    > writimpl(soc, to_array_view(tmp_buff));

    obj.serialize(serializable::range_write_iterator{&writimpl});
        
    iterator_polymorpic_adapter<
        chunk_read_iterator,
        boost::forward_traversal_tag,
        array_view<const char>
    > rditimpl(soc, to_array_view(tmp_buff));

    obj.deserialize(serializable::range_read_iterator{&rditimpl});
}

void transceiver_service::connect(array_view<char> buff, size_t sz, io::tcp_socket soc)
{
    std::list<io::tcp_socket>::iterator soc_it;
    {
        lock_guard guard(mtx);
        if (closed_) throw closed_exception();
        using_set_.push_back(soc);
        soc_it = using_set_.end();
        --soc_it;
    }
    SCOPE_EXIT([this, soc_it]() {
        lock_guard guard(mtx);
        using_set_.erase(soc_it);
    });

    iterator_polymorpic_adapter<
        chunk_read_iterator,
        boost::forward_traversal_tag,
        array_view<const char>
    > rditimpl(soc, buff, sz);

    output_iterator_polymorpic_adapter<
        chunk_write_iterator,
        boost::forward_traversal_tag,
        array_view<char>
    > writimpl(soc, buff);

    serializable::range_write_iterator wit(&writimpl);
    
    range_dereferencing_iterator rdit(serializable::range_read_iterator {&rditimpl});
    range_dereferencing_iterator rwdit(serializable::range_write_iterator{&writimpl});

    for (;;)
    {
        automatic<sonia::type::durable_id> btid(in_place_decode<sonia::serialization::compressed_t>(rdit));

        try {
            auto * pfunc = sonia::get_multimethod<process_taged_object, void(stub_read_iterator&, stub_write_iterator&)>({btid->ti()});
            if (!pfunc) {
                throw exception("transceiver exceptor error: object invoker is not defined for: %1%"_fmt % btid);
            }

            (*pfunc)(rdit, rwdit);
            
        } catch (std::exception const& e) {
            string_view err{e.what()};
            (make_encoder<sonia::serialization::compressed_t>(std::move(rwdit)) & err.size() & err).iterator().flush();
            break; // current socket can not be used any more
        }

        rwdit.flush();
    }
}

void transceiver_service::connect(array_view<char> buff, size_t sz, sonia::sal::socket_address const&, sonia::io::udp_socket&)
{
    if (sz < sizeof(chunk_header)) {
        throw exception("wrong message size: %1%"_fmt % sz);
    }
    
    uint32_t crc;
    uint16_t datasz;
    make_decoder<sonia::serialization::ordered_t>(buff.cbegin()) & crc & datasz;
    
    if (sz != sizeof(chunk_header) + datasz) {
        throw exception("wrong message size: %1%, expected:"_fmt % sz % (sizeof(chunk_header) + datasz));
    }

    uint32_t rcrc = boost::crc<32, 0xBEEF, 0xC001F00D, 0, true, true>(buff.begin() + sizeof(chunk_header), datasz);
    if (crc != rcrc) {
        throw exception("wrong message crc: %1% instead of %2%"_fmt % crc % rcrc);
    }

    THROW_NOT_IMPLEMENTED_ERROR("transceiver_service::connect (udp)");
}

}
