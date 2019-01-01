//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <map>
#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
#define TEST_FOLDER "iterators_test"

#include "sonia/string.hpp"
#include "sonia/utility/iterators/file_region_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/utility/iterators/tar_iterator.hpp"
#include "sonia/utility/iterators/buffering_mediator_iterator.hpp"
#include "sonia/utility/iterators/inflate_iterator.hpp"
#include "sonia/utility/iterators/deflate_iterator.hpp"
#include "sonia/utility/iterators/bz2_decompress_iterator.hpp"
#include "sonia/utility/iterators/bz2_compress_iterator.hpp"

using namespace sonia;

#if 0
#if 1
BOOST_AUTO_TEST_CASE (file_region_iterator_test)
{
    fs::remove_all(TEST_FOLDER);

    using file_iterator_t = file_region_iterator<const char>;
    using file_write_iterator_t = file_region_iterator<char>;

    // create the file
    {
        fs::create_directories(TEST_FOLDER);
        std::ofstream f(TEST_FOLDER "/test.dat");
    }

    range_dereferencing_iterator it(file_write_iterator_t{TEST_FOLDER "/test.dat"});

    std::string content = "this is a content";
    std::copy(content.begin(), content.end(), std::move(it)).flush();
    BOOST_CHECK_EQUAL(content.size(), fs::file_size(TEST_FOLDER "/test.dat"));

    {
        std::ofstream f(TEST_FOLDER "/test1.dat");
        file_write_iterator_t fout2(TEST_FOLDER "/test1.dat");
        *fout2 = string_view(content);
        fout2.flush();
    }
    BOOST_CHECK_EQUAL(content.size(), fs::file_size(TEST_FOLDER "/test1.dat"));
}

#endif

#if 1
BOOST_AUTO_TEST_CASE( tar_iterator_test )
{
    char const* path = std::getenv("SONIA_PRIME_HOME");
    BOOST_REQUIRE_MESSAGE(path, "SONIA_PRIME_HOME must be set");

    fs::path sonia_prime_home(path);

    using file_iterator_t = file_region_iterator<const char>;
    using tar_iterator_t = tar_extract_iterator<file_iterator_t>;

    tar_iterator_t tit(file_iterator_t(sonia_prime_home / "tests" / "data" / "files.tar", 0, 65536));

    std::map<std::string, std::string> content;
    while (tit.next()) {
        std::string name = tit.current_name();
        std::string val;
        do {
            array_view<const char> rng = *tit;
            val += std::string(rng.begin(), rng.end());
            ++tit;
        } while (!tit.empty());
        content[name] = val;
    }
    BOOST_CHECK_EQUAL(content.size(), 2);
    BOOST_CHECK_EQUAL(content["file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(content["file1.txt"], "file1\r\n");
}

#endif

#if 1
BOOST_AUTO_TEST_CASE(gz_iterator_test)
{
    char const* path = std::getenv("SONIA_PRIME_HOME");
    BOOST_REQUIRE_MESSAGE(path, "SONIA_PRIME_HOME must be set");

    fs::path sonia_prime_home(path);
    fs::path files_base_path = sonia_prime_home / "tests" / "data";

    using file_iterator_t = file_region_iterator<const char>;

	//using inflate_iterator_t = inflate_iterator<file_iterator_t>;
	//using buffering_mediator_iterator_t = buffering_mediator_iterator<inflate_iterator_t>;
	//using tar_extract_iterator_t = tar_extract_iterator<buffering_mediator_iterator_t>;
	
	tar_extract_iterator tit(
		buffering_mediator_iterator(
			inflate_iterator(file_iterator_t(files_base_path / "files.tar.gz", 0, 65536), true),
			65536
		)
	);

	std::map<std::string, std::string> content;
	while (tit.next()) {
		std::string name = tit.current_file_name();
		std::string val;
		do {
			array_view<const char> rng = *tit;
			val += std::string(rng.begin(), rng.end());
			++tit;
		} while (!tit.empty());
		content[name] = val;
	}
	BOOST_CHECK_EQUAL(content.size(), 2);
	BOOST_CHECK_EQUAL(content["file0.txt"], "file0\r\n");
	BOOST_CHECK_EQUAL(content["file1.txt"], "file1\r\n");

}
#endif

template <
    template <class> class compress_iterator,
    template <class> class decompress_iterator,
    typename ... ArgsT
>
void archiver_test(const char* fname, size_t fsz, ArgsT && ... args)
{
    std::ofstream{fname};
    compress_iterator oit(file_region_iterator<char>(fname, 0, 65536), std::forward<ArgsT>(args) ...);

    for (size_t idx = 0; idx < fsz;) {
        char buff[40000];
        size_t sz = (std::min)(sizeof(buff), fsz - idx);
        for (size_t k = 0; k < sz; ++k) {
            buff[k] = (char)((idx + k) & 0xff);
        }
        *oit = array_view(buff, sz);
        ++oit;
        idx += sz;
    }
    oit.flush();

    buffering_mediator_iterator iit(
        decompress_iterator(file_region_iterator<const char>(fname, 0, 65536), std::forward<ArgsT>(args) ...),
        40017);

    size_t rfsz = 0;
    char checkv = 0;
    for (; !iit.empty(); ++iit) {
        array_view<const char> arr = *iit;
        for (char c : arr) {
            if (c != checkv++) {
                BOOST_CHECK_EQUAL(c, checkv - 1);
            }
        }
        //if ((rfsz / 100000) % 10 == 0) {
        //    std::cout << rfsz << "\n";
        //}
        rfsz += arr.size();
    }

    BOOST_CHECK_EQUAL(rfsz, fsz);
}

#if 1
BOOST_AUTO_TEST_CASE(gzip_iterators_test)
{
    //fs::remove_all(TEST_FOLDER);
    fs::create_directories(TEST_FOLDER);

    archiver_test<deflate_iterator, inflate_iterator>(TEST_FOLDER "/temp.gz", 1024 * 1024 * 128, true);
}
#endif

#if 1
BOOST_AUTO_TEST_CASE(bzip2_iterators_test)
{
    //fs::remove_all(TEST_FOLDER);
    fs::create_directories(TEST_FOLDER);

    archiver_test<bz2_compress_iterator, bz2_decompress_iterator>(TEST_FOLDER "/temp.bz2", 1024 * 1024 * 16);
}
#endif

#endif

#include "sonia/utility/iterators/archive_extract_iterator.hpp"
#if 1
namespace {

std::map<std::string, std::string> load_archive(fs::path const& p)
{
    std::map<std::string, std::string> result;
    archive_extract_iterator ait(to_string_view(p.leaf().string()), file_region_iterator<const char>(p, 0, 65536));
    
    while (ait.next()) {
        std::string name = ait.current_name();
        std::string val;

        do {
            array_view<const char> rng = *ait;
            val += std::string(rng.begin(), rng.end());
            ++ait;
        } while (!ait.empty());
        result[name] = val;
    }

    return std::move(result);
}

}

BOOST_AUTO_TEST_CASE (archive_extract_iterator_test)
{
    char const* path = std::getenv("SONIA_PRIME_HOME");
    BOOST_REQUIRE_MESSAGE(path, "SONIA_PRIME_HOME must be set");

    fs::path sonia_prime_home{path};

    std::map<std::string, std::string> cnt;
#if 0
    // just a file
    cnt = load_archive(sonia_prime_home / "tests" / "data" / "file0.txt");
    BOOST_CHECK_EQUAL(cnt.size(), 1);
    BOOST_CHECK_EQUAL(cnt["file0.txt"], "file0");

    // gzipped file
    cnt = load_archive(sonia_prime_home / "tests" / "data" / "file0.txt.gz");
    BOOST_CHECK_EQUAL(cnt.size(), 1);
    BOOST_CHECK_EQUAL(cnt["file0.txt"], "file0");

    // bzipp2ed file
    cnt = load_archive(sonia_prime_home / "tests" / "data" / "file0.txt.bz2");
    BOOST_CHECK_EQUAL(cnt.size(), 1);
    BOOST_CHECK_EQUAL(cnt["file0.txt"], "file0");
#endif
    // tar file
    cnt = load_archive(sonia_prime_home / "tests" / "data" / "files.tar");
    BOOST_CHECK_EQUAL(cnt.size(), 2);
    BOOST_CHECK_EQUAL(cnt["file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["file1.txt"], "file1\r\n");
}
#endif