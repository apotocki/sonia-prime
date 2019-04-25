#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <fstream>
#include "sonia/exceptions.hpp"

using namespace sonia;

void parse_line(std::string const& line, std::array<std::string, 5> & res)
{
    size_t pos = 0;
    auto bit = line.begin();
    for (auto it = bit, eit = line.end(); it != eit; ++it) {
        if (*it == '\"') {
            for (++it; it != eit && *it != '\"'; ++it);
            if (it == eit) throw exception("unexpected eol %1%"_fmt % line);
            continue;
        }
        if (*it != ',') continue;
        res[pos++] =  std::string(bit, it);
        bit = it;
        ++bit;
        BOOST_ASSERT (pos < res.size());
    }
    res[pos++] = std::string(bit, line.end());
    BOOST_ASSERT (pos == res.size());
}

template <class VectorT>
void load_file(std::string const& name, VectorT & result)
{
    std::ifstream s{name.c_str()};
    if (s.fail()) throw exception("%1% is not found"_fmt % name);

    std::string line;
    std::getline(s, line);

    using row_type = typename VectorT::value_type;

    row_type row;
    while (std::getline(s, line)) {
        parse_line(line, row);
        result.push_back(std::move(row));
    }
}

std::string get_enum_name(std::string const& v)
{
    std::string result;
    for (char c : v) {
        if (c == '-') c = '_';
        c = (char)std::toupper(c);
        result.push_back(c);
    }
    return result;
}

BOOST_AUTO_TEST_CASE (http_header_gather_util)
{
    using row_type = std::array<std::string, 5>;
    std::vector<row_type> rows;

    load_file("k:\\temp\\perm-headers.csv", rows);
    load_file("k:\\temp\\prov-headers.csv", rows);

    std::sort(rows.begin(), rows.end(), [](auto const& l, auto const& r) { return l[0] < r[0];});

    std::vector<std::string> rowstrs;
    rowstrs.reserve(rows.size());

    size_t maxsz = 0;
    for (auto const& r : rows) {
        if (r[2] != "http") continue;
        std::ostringstream ress;
        ress << "((" << get_enum_name(r[0]) << ", \"" << r[0] << "\"))";
        maxsz = (std::max)(maxsz, ress.str().size());
        rowstrs.push_back(ress.str());
    }

    maxsz += 2;
    std::ofstream ress("k:\\temp\\enum.hpp", std::ios::binary);
    for (auto const& l : rowstrs) {
        const size_t addsz = maxsz - l.size();
        ress << l << std::string(addsz, ' ') << "\\\n";
    }
    std::cout << "COUNT: " << rowstrs.size() << "\n";
}
