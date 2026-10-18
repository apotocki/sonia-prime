//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <bit>
#include <cstring>

#include "sonia/utility/invocation/invocation.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "integral.hpp"
#include "span.hpp"

namespace sonia::serialization {

// Binary form of a blob_result: its fields one by one rather than as raw memory (the bit-field
// layout is compiler specific) - type, inplace_size, the flag bits (reserved0, bp_reserved_used,
// reserved1) and the 14 data bytes with the payload pointer zeroed - followed by its out-of-line
// payload, if any: the raw bytes, or the element blobs of a tuple. Scalars stay in the native byte
// order, so the format is little-endian only. Values that can't outlive the process (objects,
// functions, references, errors) are refused.
//
// Decoding gives an owning blob: the payload storage is allocated and decoded into directly. Sizes
// read from the input are not trusted, see check_decoded_count.

// type, inplace_size, flags, data bytes
template <typename TagT>
struct min_encoded_size<TagT, blob_result> : integral_constant<size_t, 3 + sizeof(blob_result::data)> {};

template <typename TagT>
class coder<TagT, blob_result>
{
    static_assert(std::endian::native == std::endian::little);

public:
    static bool is_storable(blob_type t) noexcept
    {
        return t != blob_type::object && t != blob_type::function && t != blob_type::blob_reference && t != blob_type::error;
    }

    static bool has_payload(blob_type t) noexcept
    {
        return (((uint8_t)t) & 0x80) || t == blob_type::bigint || t == blob_type::decimal || t == blob_type::blob;
    }

    template <typename OutputIteratorT>
    OutputIteratorT encode(blob_result const& b, OutputIteratorT oi) const
    {
        if (!is_storable(b.type)) {
            throw exception("blob_result %1% is not serializable"_fmt % b);
        }
        bool out_of_line = !b.inplace_size && has_payload(b.type);
        blob_result fields = b;
        if (out_of_line) fields.data.bp.payload_ptr = nullptr;

        oi = sonia::encode<TagT>(b.type, std::move(oi));
        oi = sonia::encode<TagT>(static_cast<uint8_t>(b.inplace_size), std::move(oi));
        oi = sonia::encode<TagT>(static_cast<uint8_t>(b.reserved0 | (b.bp_reserved_used << 1) | (b.reserved1 << 2)), std::move(oi));
        oi = sonia::encode<TagT>(std::span<const uint8_t, sizeof(blob_result::data)>{ fields.data.ui8array }, std::move(oi));

        if (!out_of_line || !b.data.bp.size) return std::move(oi);
        if (b.type == blob_type::tuple) {
            for (blob_result const& e : std::span{ data_of<blob_result>(b), array_size_of<blob_result>(b) }) {
                oi = encode(e, std::move(oi));
            }
            return std::move(oi);
        }
        uint8_t const* payload = data_of<uint8_t>(b);
        return sonia::copy(payload, payload + b.data.bp.size, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, blob_result & b) const
    {
        // `b` is assigned only once its fields are read and checked: if the input fails earlier, it
        // keeps its value (a nil tuple element, see below, has to stay releasable)
        blob_result fields;
        uint8_t inplace_size, flags;
        ii = sonia::decode<TagT>(std::move(ii), fields.type);
        ii = sonia::decode<TagT>(std::move(ii), inplace_size);
        ii = sonia::decode<TagT>(std::move(ii), flags);
        ii = sonia::decode<TagT>(std::move(ii), std::span<uint8_t, sizeof(blob_result::data)>{ fields.data.ui8array });
        if (inplace_size > sizeof(blob_result::data) || !is_storable(fields.type)) {
            throw exception("malformed blob_result data");
        }
        fields.inplace_size = inplace_size;
        fields.reserved0 = flags & 1;
        fields.bp_reserved_used = (flags >> 1) & 1;
        fields.reserved1 = (flags >> 2) & 1;
        fields.need_unpin = 0;
        b = fields;

        if (b.inplace_size || !has_payload(b.type) || !b.data.bp.size) return std::move(ii);

        uint32_t sz = b.data.bp.size;
        bool is_tuple = b.type == blob_type::tuple;
        if (is_tuple && sz % sizeof(blob_result)) {
            throw exception("malformed blob_result data");
        }
        if (is_tuple) {
            check_decoded_count<TagT, blob_result>(ii, sz / sizeof(blob_result));
        } else {
            check_decoded_count(ii, sz, 1);
        }

        // the payload is decoded straight into the allocated storage; it was out-of-line when
        // encoded, keep it so (the out-of-line decimal layout depends on it)
        b.data.bp.payload_ptr = nullptr;
        blob_result_allocate(&b, true);
        SCOPE_EXCEPTIONAL_EXIT([&b]() { blob_result_unpin(&b); });
        uint8_t* payload = mutable_data_of<uint8_t>(b);
        if (!is_tuple) {
            return sonia::pull(std::move(ii), payload, payload + sz);
        }
        // nil elements until decoded, so a failure releases the tuple with the decoded ones only
        std::memset(payload, 0, sz);
        for (blob_result* e = reinterpret_cast<blob_result*>(payload), *ee = e + sz / sizeof(blob_result); e != ee; ++e) {
            ii = decode(std::move(ii), *e);
        }
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, blob_result * b) const
    {
        return decode(std::move(ii), *b);
    }
};

}
