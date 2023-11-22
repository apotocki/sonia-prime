//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "value_decoder.hpp"

#include "boost/conversion/detail/utility/data_ops.hpp"

namespace sonia::invokation {

namespace cvt = boost::conversion;

blob_result value_decoder::decode(span<const uint8_t> sp, string_view type, string_view endianness) const
{
    if (auto it = value_handlers_.find(type, hasher{}, string_equal_to{}); it != value_handlers_.end()) {
        return it->second(sp, endianness);
    }
    throw exception("unknown type: %1%"_fmt % type);
}

template <std::integral T>
static T decode_integral(const uint8_t* data, string_view es)
{
    T buff;
    return *[es, &buff](const uint8_t* data) {
        if (es == "le"sv) {
            return cvt::le_to_T<8, sizeof(T)* CHAR_BIT, T>(&buff, data, sizeof(T));
        } else if (es == "be"sv) {
            return cvt::be_to_T<8, sizeof(T)* CHAR_BIT, T>(&buff, data, sizeof(T));
        } else {
            throw exception("unknown endianness: %1%"_fmt % es);
        }
    }(data);
}

template <std::integral T>
static void decode_integral_array(const uint8_t* data, string_view es, span<T> result)
{
    for (T& elem : result) {
        elem = decode_integral<int32_t>(data, es);
        data += sizeof(T);
    }
}

value_decoder::value_decoder()
{
    value_handlers_["ui8"] = [](span<const uint8_t> sp, string_view) -> blob_result {
        if (sp.empty()) return nil_blob_result();
        return ui8_blob_result(sp.front());
    };
    value_handlers_["i8"] = [](span<const uint8_t> sp, string_view) -> blob_result {
        if (sp.empty()) return nil_blob_result();
        return i8_blob_result(static_cast<int8_t>(sp.front()));
    };
    value_handlers_["ui16"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 2) return nil_blob_result();
        return ui16_blob_result(decode_integral<uint16_t>(sp.data(), es));
    };
    value_handlers_["i16"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 2) return nil_blob_result();
        return i16_blob_result(std::bit_cast<int16_t>(decode_integral<uint16_t>(sp.data(), es)));
    };
    value_handlers_["ui32"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 4) return nil_blob_result();
        return ui32_blob_result(decode_integral<uint32_t>(sp.data(), es));
    };
    value_handlers_["i32"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 4) return nil_blob_result();
        return i32_blob_result(std::bit_cast<int32_t>(decode_integral<uint32_t>(sp.data(), es)));
    };
    value_handlers_["ui64"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 8) return nil_blob_result();
        return ui64_blob_result(decode_integral<uint64_t>(sp.data(), es));
    };
    value_handlers_["i64"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 8) return nil_blob_result();
        return i64_blob_result(std::bit_cast<int64_t>(decode_integral<uint64_t>(sp.data(), es)));
    };
    value_handlers_["f16"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 2) return nil_blob_result();
        uint16_t ui16val = decode_integral<uint16_t>(sp.data(), es);
        return f16_blob_result(float16{ui16val});
    };
    value_handlers_["f32"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 4) return nil_blob_result();
        uint32_t ui32val = decode_integral<uint32_t>(sp.data(), es);
        return f32_blob_result(std::bit_cast<float_t>(ui32val));
    };
    value_handlers_["f64"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 8) return nil_blob_result();
        uint64_t ui64val = decode_integral<uint64_t>(sp.data(), es);
        return f64_blob_result(std::bit_cast<double_t>(ui64val));
    };

    // for now just custom case
    value_handlers_["ui32[2]"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 8) return nil_blob_result();
        uint32_t arr[2];
        decode_integral_array<uint32_t>(sp.data(), es, span{ arr });
        blob_result br = array_blob_result(arr);
        blob_result_allocate(&br);
        return br;
    };

    value_handlers_["i64[2]"] = [](span<const uint8_t> sp, string_view es) -> blob_result {
        if (sp.size() < 16) return nil_blob_result();
        int64_t arr[2];
        decode_integral_array<int64_t>(sp.data(), es, span{ arr });
        blob_result br = array_blob_result(arr);
        blob_result_allocate(&br);
        return br;
    };
}

}
