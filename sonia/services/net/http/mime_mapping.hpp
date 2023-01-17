//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "sonia/string.hpp"
#include "sonia/filesystem.hpp"

namespace sonia::services {

class mime_mapping
{
    using mime_mapping_type = boost::unordered_map<std::string, std::string, hasher>;

public:
    mime_mapping(fs::path const&);
    std::string const* find(string_view ext);

private:
    mime_mapping_type mime_map_;
};

}
