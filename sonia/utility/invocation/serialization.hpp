//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <sstream>

#include "sonia/utility/invocation/invocation.hpp"

namespace boost::serialization {

struct blob_result_serialization_helper
{
    static constexpr void* id = 0;
};

template<class ArchiveT>
void save(ArchiveT& ar, blob_result& b, const unsigned int version)
{
    using namespace ::sonia;

    std::ostringstream valuess;
    switch (b.type)
    {
    case blob_type::nil:
        valuess << "nil";
        //if (!b.data) ar << "nil";
        // to do: save byte array
        break;
    case blob_type::boolean:
        valuess << (b.bp.i8value ? "true" : "false");
        break;
    case blob_type::string:
        valuess << '"';
        for (char c : sonia::string_view(data_of<const char>(b), array_size_of<char>(b))) {
            if (c == '"' || c == '\\') {
                valuess << '\\';
            }
            valuess << c;
        }
        valuess << '"';
        break;
    default:
        throw sonia::exception("unimplemented blob serialization for: %1%"_fmt % b);
    }
    std::string value = valuess.str();
    ar << make_nvp("value", value);
}

template<class ArchiveT>
void parse(ArchiveT& ar, sonia::string_view str, blob_result& b)
{
    using namespace sonia;
    if (str == "nil") {
        b = nil_blob_result();
    } else if (str == "true") {
        b = bool_blob_result(true);
    } else if (str == "false") {
        b = bool_blob_result(false);
    } else if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        std::string result;
        for (auto it = str.begin() + 1, eit = str.end() - 1; it != eit; ++it) {
            char c = *it;
            if (c == '\\') {
                ++it;
                if (it == eit) break;
            }
            result.push_back(*it);
        }
        //blob_result_serialization_helper& hlp =
        //    ar.template get_helper<blob_result_serialization_helper>(blob_result_serialization_helper::id);
        b = string_blob_result(result);
        blob_result_allocate(&b);
    } else {
        throw sonia::exception("can't decode blob result: %1%"_fmt % str);
    }
}

template<class ArchiveT>
void load(ArchiveT& ar, blob_result& b, const unsigned int version)
{
    std::string value;
    ar >> make_nvp("value", value);
    parse(ar, value, b);
}

template<class ArchiveT>
void serialize(ArchiveT& ar, blob_result& b, const unsigned int version)
{
    if constexpr (ArchiveT::is_saving::value) {
        save(ar, b, version);
    } else {
        load(ar, b, version);
    }
}

}
