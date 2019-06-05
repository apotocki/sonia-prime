//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HTTP_MESSAGE_IPP
#define SONIA_HTTP_MESSAGE_IPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/algorithm/string/predicate.hpp>

#include "message.hpp"

#include "sonia/utility/parsers/utility.hpp"
#include "sonia/utility/iterators/reference_wrapper_iterator.hpp"
#include "sonia/utility/iterators/iterator_of_ranges_with_limit.hpp"
#include "sonia/utility/iterators/http_chunking_read_input_iterator.hpp"

namespace sonia::http {

template <typename ReadIteratorT>
void message::build_input_iterator(ReadIteratorT & ii)
{
    array_view<const std::string> hval = get_header(header::CONTENT_LENGTH);
    if (hval.size() > 1) {
        throw exception("multiple content lengths");
    } else if (!hval.empty()) {
        size_t len;
        auto hit = hval[0].cbegin(), heit = hval[0].cend();
        if (!parsers::integer(hit, heit, 1, 16, len)) {
            throw exception("can't parse content length content lengths");
        }
        input = make_chain_linkable_iterator(iterator_of_ranges_with_limit{reference_wrapper_iterator{ii}, len});
    } else {
        hval = get_header(header::TRANSFER_ENCODING);
        if (hval.size() > 1) {
            throw exception("multiple transfer encodings");
        } else if (!hval.empty() && boost::iequals(hval[0], "chunked")) {
            input = make_chain_linkable_iterator(http_chunking_read_input_iterator{reference_wrapper_iterator{ii}});
        } else {
            input = make_chain_linkable_iterator(reference_wrapper_iterator{ii});
        }
    }
}

}

#endif // SONIA_HTTP_MESSAGE_IPP
