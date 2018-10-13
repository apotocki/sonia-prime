//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#define TEST_FOLDER "aio_test"

#include <sstream>

#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/services.hpp"

using namespace sonia;

namespace {

shared_ptr<std::istream> get_configuration() {
    auto ss = sonia::make_shared<std::stringstream>();
    (*ss) <<
        "{"
        "   hosts: ["
        "       {"
        "           name: 'default',"
        "           services: []"
        "       }"
        "   ],"
        "   services: {"
        "       'aio.serv': {"
        "           factory: 'aio-factory',"
        "           layer: 16,"
        "           parameters: {"
        "               threads: 8"
        "           }"
        "       },"
        "       'scheduler.serv': {"
        "           factory: 'scheduler-factory',"
        "           layer: 16,"
        "           parameters: {"
        "               threads: 1,"
        "               fibers: 2"
        "           }"
        "       },"
        "       'aio-factory': {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'aio' }"
        "       },"
        "       'scheduler-factory': {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'scheduler' }"
        "       }"
        "   },"
        "   bundles: {"
        "       prime: {lib: 'sonia-prime'}"
        "   }"
        "}";
    return ss;
}

}

#include <vector>
#include "applied/scoped_services.hpp"
#include "sonia/io/file.hpp"

BOOST_AUTO_TEST_CASE (aio_test)
{
    fs::remove_all(TEST_FOLDER);

    try {
        scoped_services ss;
        services::load_configuration(*get_configuration());

        auto ff = services::locate<io::file_factory>("aio.serv");
        io::file f = ff->open_file("data/sparqldaf184.n3", io::file_open_mode::open, io::file_access_mode::read, io::file_bufferring_mode::not_buffered);
        BOOST_CHECK_EQUAL(f.size(), 392671779);
        std::vector<char> buff(65536 * 16);
        
        size_t cnt = 0;
        fiber([&cnt]() { for (size_t i = 0; i < 0xf0000; ++i, ++cnt); }).detach();
        
        BOOST_CHECK_EQUAL(0, cnt);
        size_t rsz = f.read(0, to_array_view(buff));
        BOOST_CHECK_EQUAL(0xf0000, cnt);
        BOOST_CHECK_EQUAL(rsz, buff.size());

        fs::create_directories(TEST_FOLDER);
        io::file wrf = ff->open_file(TEST_FOLDER "/tmp.data", io::file_open_mode::create, io::file_access_mode::write, io::file_bufferring_mode::not_buffered);

        fiber([&cnt]() { for (size_t i = 0; i < 0xf0000; ++i, ++cnt); }).detach();
        size_t wrsz = wrf.write(0, to_array_view(buff));
        BOOST_CHECK_EQUAL(wrsz, buff.size());
        BOOST_CHECK_EQUAL(0x1e0000, cnt);
    } catch (shutdown_exception const& e) {
        std::cout << e.what() << "\n";
    } catch (std::exception const& e) {
        std::cerr << e.what() << "\n";
        BOOST_CHECK(false);
    }
}
