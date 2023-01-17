//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "mime_mapping.hpp"

#include <fstream>
#include <boost/algorithm/string/trim.hpp>

#include "sonia/exceptions.hpp"

namespace sonia::services {

mime_mapping::mime_mapping(fs::path const& p)
{
    std::ifstream mmfile(p.string());
    if (!mmfile.is_open()) {
        throw exception("can't open the mime-mapping file '%1%'"_fmt % p);
    }

    while (!mmfile.eof()) {
        char line[256];
        mmfile.getline(line, sizeof(line));
        std::string rule(line);
        if (!rule.empty() && rule[0] == '#') continue;
        auto delit = std::find(rule.begin(), rule.end(), ',');
        if (delit == rule.end()) {
            continue;
        }
        std::string ext = boost::trim_copy(std::string{ rule.begin(), delit });
        std::string mt = boost::trim_copy(std::string{ delit + 1, rule.end() });
        mime_map_[ext] = mt;
    }
}

std::string const* mime_mapping::find(string_view ext)
{
    auto extit = mime_map_.find(ext, hasher(), string_equal_to());
    if (extit == mime_map_.end()) return nullptr;
    return &extit->second;
}

}
