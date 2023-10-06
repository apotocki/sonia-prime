//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#define TEST_FOLDER "zip_test"

#include "applied/sonia_test.hpp"

//#include "sonia/utility/iterators/file_region_iterator.hpp"
//#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

#include "sonia/filesystem.hpp"
#include "sonia/utility/zip.hpp"

using namespace sonia;

/*
namespace sonia {

template <typename T>
struct range_dereferencing_iterator_equal<range_dereferencing_iterator<file_region_iterator<T>>>
{
    using iterator_t = range_dereferencing_iterator<file_region_iterator<T>>;
    bool operator()(iterator_t const& l, iterator_t const& r) const
    {
        if (l.empty()) return r.empty();
        if (r.empty()) return false;
        return l.offset() + l.base.offset() == r.offset() + r.base.offset();
    }
};

}
*/

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
namespace boost::interprocess::detail::ipcdetail {}
namespace ipc = boost::interprocess;

void zip_test()
{
    fs::remove_all(TEST_FOLDER);
    
    char const* path = std::getenv("TESTS_HOME");
    fs::path data_path{ (path ? fs::path(path) / "testdata" : fs::path("testdata")) / "archives" };
    
    fs::path file_path = data_path / u8"simple0.zip"sv;

    //file_path = "k:\\temp\\temp\\Cambridge1.zip";
    file_path = "s:\\kaggle\\stanford-ribonanza-rna-folding.zip";

    ipc::file_mapping fm = ipc::file_mapping(file_path.c_str(), ipc::read_only);
    ipc::offset_t file_size = 0u;
    {
        using namespace ipc::detail;
        using namespace ipc::ipcdetail;
        get_file_size(file_handle_from_mapping_handle(fm.get_mapping_handle()), file_size);
    }
    ipc::mapped_region mrgn(fm, fm.get_mode(), 0, file_size);
    std::span file{ reinterpret_cast<const char*>(mrgn.get_address()), mrgn.get_size() };

    //auto b = 
    //using file_rng_iterator_t = file_region_iterator<const char>;
    //using read_iterator_t = range_dereferencing_iterator<file_rng_iterator_t>;
    //read_iterator_t it{ file_rng_iterator_t{data_path / u8"simple0.zip"sv} };
    //read_iterator_t eit = it;
    //eit += eit.base.max_offset() - 1;

    zipper z(file.begin(), file.end());
    std::cout << "entry count: " << z.count() << "\n";
    z.sort_by_names();
    std::cout << "sorted\n";
    /*
    for (size_t i = 0; i < z.count(); ++i) {
        std::cout << z.name_at(i) << "\n";
    }
    */
}

void zip_test_registrar()
{
    register_test(BOOST_TEST_CASE(&zip_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(zip_test_registrar)
#endif