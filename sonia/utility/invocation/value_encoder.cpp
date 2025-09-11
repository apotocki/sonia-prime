//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "value_encoder.hpp"
#include "dataforge/detail/utility/data_ops.hpp"

namespace sonia::invocation {

void value_encoder::encode(blob_result const& value, string_view type, string_view endianness, function<void(span<const uint8_t>)> const& writer) const
{
    if (auto it = value_handlers_.find(type, hasher{}, string_equal_to{}); it != value_handlers_.end()) {
        it->second(value, endianness, writer);
        return;
    }
    throw exception("unknown type: %1%"_fmt % type);
}

template <std::integral T>
static void encode_integral(T value, string_view es, uint8_t* data)
{
    if (es == "le"sv) {
        dataforge::le_copy<sizeof(T) * CHAR_BIT, 8>(&value, 1, data);
    } else if (es == "be"sv) {
        dataforge::be_copy<sizeof(T) * CHAR_BIT, 8>(&value, 1, data);
    } else {
        throw exception("unknown endianness: %1%"_fmt % es);
    }
}

template <std::integral T>
static void write_integral(T value, string_view es, function<void(span<const uint8_t>)> const& writer)
{
    uint8_t buff[sizeof(T)];
    encode_integral(value, es, buff);
    writer(span{ buff });
}

value_encoder::value_encoder()
{
    value_handlers_["ui8"] = [](blob_result const& br, string_view, function<void(span<const uint8_t>)> const& writer) {
        write_integral(as<uint8_t>(br), "le"sv, writer);
    };
    value_handlers_["i8"] = [](blob_result const& br, string_view, function<void(span<const uint8_t>)> const& writer) {
        write_integral(std::bit_cast<int8_t>(as<uint8_t>(br)), "le"sv, writer);
    };
        
    value_handlers_["ui16"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(as<uint16_t>(br), es, writer);
    };
    value_handlers_["i16"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(std::bit_cast<uint16_t>(as<int16_t>(br)), es, writer);
    };

    value_handlers_["ui32"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(as<uint32_t>(br), es, writer);
    };
    value_handlers_["i32"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(std::bit_cast<uint32_t>(as<int32_t>(br)), es, writer);
    };

    value_handlers_["ui64"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(as<uint64_t>(br), es, writer);
    };
    value_handlers_["i64"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(std::bit_cast<uint64_t>(as<int64_t>(br)), es, writer);
    };

    value_handlers_["f16"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(numetron::float16{ as<float_t>(br) }.to_bits(), es, writer);
    };
    value_handlers_["f32"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(std::bit_cast<uint32_t>(as<float_t>(br)), es, writer);
    };
    value_handlers_["f64"] = [](blob_result const& br, string_view es, function<void(span<const uint8_t>)> const& writer) {
        write_integral(std::bit_cast<uint64_t>(as<double_t>(br)), es, writer);
    };
}

}
