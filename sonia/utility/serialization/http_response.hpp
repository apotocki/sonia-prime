//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/string.hpp"
#include "sonia/type_traits.hpp"

#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/parsers/utility.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/utility/iterators/http_chunking_write_input_iterator.hpp"
#include "sonia/utility/iterators/deflate_iterator.hpp"
#include "sonia/utility/iterators/buffering_write_input_iterator.hpp"

#include "serialization.hpp"
#include "http_message.hpp"

namespace sonia::serialization {

template <typename TagT>
class coder<TagT, http::response> : coder<TagT, http::message>
{
    using base_type = coder<TagT, http::message>;

public:
    template <typename WriteIteratorT>
    WriteIteratorT encode(http::response const& r, WriteIteratorT oi) const
    {
        auto msg = r.status_string.value_or(to_string(r.status_code));
        std::string start_line = to_string("HTTP/%1%.%2% %3% %4%\r\n"_fmt % (r.version / 10) % (r.version % 10) % (int)r.status_code % msg);

        range_dereferencing_iterator writ{std::move(oi)};
        writ = std::copy(start_line.begin(), start_line.end(), std::move(writ));

        if (!r.content_writer) {
            writ = base_type::encode(r, std::move(writ));
            writ.flush();
            return std::move(writ.base);
        }

        auto hvals = r.get_header(http::header::CONTENT_LENGTH);
        if (!hvals.empty() || !r.content_writer) { // content legth is a flag that signals of raw write op
            writ = base_type::encode(r, std::move(writ));
            writ.flush();

            oi = std::move(writ.base);
            
            if (r.content_writer) {
                output_iterator_polymorpic_adapter<WriteIteratorT> roimpl{oi};
                r.content_writer(http::message::range_write_input_iterator{&roimpl});
                roimpl.flush();
            }

        } else {
            hvals = r.get_header(http::header::TRANSFER_ENCODING);
            if (hvals.size() != 1 || hvals[0] != "chunked") {
                const_cast<http::response&>(r).set_header(http::header::TRANSFER_ENCODING, string_view("chunked"));
            }

            writ = base_type::encode(r, std::move(writ));
            writ.flush();
            oi = std::move(writ.base);

            automatic_polymorphic<http::message::range_write_impl_type, 128> roimpl;

            using chunking_output_iterator = output_iterator_polymorpic_adapter<http_chunking_write_input_iterator<WriteIteratorT>>;
            using deflate_output_iterator_t = output_iterator_polymorpic_adapter<buffering_write_input_iterator<deflate_iterator<http_chunking_write_input_iterator<WriteIteratorT>>>>;

            std::vector<char> buff;
            bool is_gzip = true;
            hvals = r.get_header(http::header::CONTENT_ENCODING);
            if (hvals.empty()) {
                roimpl.emplace<chunking_output_iterator>(http_chunking_write_input_iterator{std::move(oi)});
            } else if (hvals[0] == "gzip") {
                is_gzip = true;
            } else if (hvals[0] == "deflate") {
                is_gzip = false;
            } else {
                THROW_INTERNAL_ERROR("unknown content encoding: %1%"_fmt % hvals[0]);
            }
            
            if (!roimpl) {
                buff.resize(4096);
                roimpl.emplace<deflate_output_iterator_t>(
                    buffering_write_input_iterator{
                        deflate_iterator{
                            http_chunking_write_input_iterator{std::move(oi)}
                            , is_gzip
                        }, buff
                    }
                );
            }
            
            r.content_writer(http::message::range_write_input_iterator{roimpl.get_pointer()});

            roimpl->close();
        }
        
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, http::response & resp) const
    {
        ii = base_type::decode_protocol(std::move(ii), resp);
        
        if (*ii != ' ') {
            throw exception("wrong protocol version");
        }
        ++ii;

        // parse status code
        unsigned int code;
        if (!sonia::parsers::integer(ii, InputIteratorT(), 1, 3, code)) {
            throw exception("wrong status code");
        }
        resp.status_code = (sonia::http::status)code;

        char c = *ii; ++ii;
        if (c != ' ') {
            throw exception("wrong status format");
        }

        // parse status description
        std::string msg;
        for (;;) {
            char c = *ii; ++ii;
            if (c == '\r') {
                c = *ii; ++ii;
                if (c == '\n') break;
                msg.push_back('\r');
            }
            msg.push_back(c);
        }

        resp.status_string = msg;
        ii = base_type::decode(std::move(ii), resp);
        return std::move(ii);
    }
};

}
