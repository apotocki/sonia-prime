//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <map>
#include <vector>
#include <fstream>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>

namespace fs = std::filesystem;
#define TEST_FOLDER "serialization_stream_test"

#include "sonia/string.hpp"
#include "sonia/utility/serialization/iostream.hpp"
#include "sonia/utility/iterators/file_region_iterator.hpp"

using namespace sonia;

BOOST_AUTO_TEST_CASE(serialization_stream_test)
{
    fs::remove_all(TEST_FOLDER);

    using file_iterator_t = file_region_iterator<const char>;
    using file_write_iterator_t = file_region_iterator<char>;

    fs::create_directories(TEST_FOLDER);
    { std::ofstream fos2(TEST_FOLDER "/data.xml"); }

    std::map<std::string, std::vector<int>> data, data2;
    for (int i = 0; i < 100; ++i) {
        std::string key = "key #" + boost::lexical_cast<std::string>(i);
        std::vector<int> val;
        val.reserve(i);
        for (int j = 0; j < i; j++) {
            val.push_back(j);
        }
        data[key] = std::move(val);
    }

    {
        file_write_iterator_t fout(TEST_FOLDER "/data.xml");
        serialization::ostream<file_write_iterator_t, char> fos(std::move(fout));
        boost::archive::xml_oarchive oa(fos);

        oa & boost::serialization::make_nvp("map", data);
    }

    file_iterator_t fin(TEST_FOLDER "/data.xml");
    serialization::istream<file_iterator_t, char> fis(std::move(fin));
    boost::archive::xml_iarchive ia(fis, boost::archive::no_tracking);
    
    ia & boost::serialization::make_nvp("map", data2);

    BOOST_CHECK(data == data2);
}
