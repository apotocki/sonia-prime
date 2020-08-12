//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <map>
#include <string>
#include <fstream>
#include <filesystem>

#include <boost/test/unit_test.hpp>

namespace fs = std::filesystem;
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

    range_dereferencing_iterator it{file_write_iterator_t{TEST_FOLDER "/test.dat"}};

    std::string content = "this is a content";
    std::copy(content.begin(), content.end(), std::move(it)).flush(); // flush to limit file size
    BOOST_CHECK_EQUAL(content.size(), fs::file_size(TEST_FOLDER "/test.dat"));

    {
        std::ofstream f{TEST_FOLDER "/test1.dat"};
        file_write_iterator_t fout2{TEST_FOLDER "/test1.dat"};
        fout2.write(string_view(content));
        // no flush heare because of external data set
    }
    BOOST_CHECK_EQUAL(content.size(), fs::file_size(TEST_FOLDER "/test1.dat"));

    // write exactly 64k test
    std::vector<char> data;
    data.resize(65536, 0x22);
    {
        file_write_iterator_t it2{TEST_FOLDER "/test.dat"};
        array_view<char> buff = *it2;
        std::for_each(buff.begin(), buff.end(), [](char & c) { c = 0x22; });
        ++it2;
        it2.flush();
    }

}

#endif

#if 1
#if 1
BOOST_AUTO_TEST_CASE( tar_iterator_test )
{
    char const* path = std::getenv("SONIA_PRIME_HOME");
    BOOST_REQUIRE_MESSAGE(path, "SONIA_PRIME_HOME must be set");

    fs::path sonia_prime_home(path);

    using file_iterator_t = file_region_iterator<const char>;
    using tar_iterator_t = tar_extract_iterator<file_iterator_t>;

    tar_iterator_t tit{file_iterator_t{sonia_prime_home / "tests" / "data" / "archives" / "files.tar", 0, 65536}};

    std::map<std::string, std::string> content;
    while (tit.next()) {
        std::string name = tit.name();
        std::string val;
        do {
            array_view<const char> rng = *tit;
            val += std::string(rng.begin(), rng.end());
            ++tit;
        } while (!tit.empty());
        content[name] = val;
    }
    BOOST_CHECK(tit.empty());
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
    fs::path files_base_path = sonia_prime_home / "tests" / "data" / "archives";

    using file_iterator_t = file_region_iterator<const char>;

	//using inflate_iterator_t = inflate_iterator<file_iterator_t>;
	//using buffering_mediator_iterator_t = buffering_mediator_iterator<inflate_iterator_t>;
	//using tar_extract_iterator_t = tar_extract_iterator<buffering_mediator_iterator_t>;
	
	tar_extract_iterator tit{
		buffering_mediator_iterator{
        inflate_iterator{file_iterator_t{files_base_path / "files.tar.gz", 0, 65536}, true},
			65536
        }
    };

	std::map<std::string, std::string> content;
	while (tit.next()) {
		std::string name = tit.name();
		std::string val;
		do {
			array_view<const char> rng = *tit;
			val += std::string(rng.begin(), rng.end());
			++tit;
		} while (!tit.empty());
		content[name] = val;
	}
    BOOST_CHECK(tit.empty());
	BOOST_CHECK_EQUAL(content.size(), 2);
	BOOST_CHECK_EQUAL(content["file0.txt"], "file0\r\n");
	BOOST_CHECK_EQUAL(content["file1.txt"], "file1\r\n");

}
#endif

#if 1
template <
    template <class> class compress_iterator,
    template <class> class decompress_iterator,
    typename ... ArgsT
>
void archiver_test(const char* fname, size_t fsz, ArgsT && ... args)
{
    std::ofstream{fname};
    compress_iterator oit{file_region_iterator<char>{fname, 0, 65536}, std::forward<ArgsT>(args) ...};

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
    oit.close();

    buffering_mediator_iterator iit{
        decompress_iterator{file_region_iterator<const char>{fname, 0, 65536}, std::forward<ArgsT>(args) ...},
        40017};

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

BOOST_AUTO_TEST_CASE(gzip_iterators_test)
{
    //fs::remove_all(TEST_FOLDER);
    fs::create_directories(TEST_FOLDER);

    archiver_test<deflate_iterator, inflate_iterator>(TEST_FOLDER "/temp.gz", 1024 * 1024 * 128, true);
}

BOOST_AUTO_TEST_CASE(bzip2_iterators_test)
{
    //fs::remove_all(TEST_FOLDER);
    fs::create_directories(TEST_FOLDER);

    archiver_test<bz2_compress_iterator, bz2_decompress_iterator>(TEST_FOLDER "/temp.bz2", 1024 * 1024 * 16);
}
#endif

#include "sonia/utility/iterators/archive_extract_iterator.hpp"

namespace {

std::map<std::string, std::string> load_archive(fs::path const& p)
{
    std::map<std::string, std::string> result;
    
    archive_iterator ait = make_archive_extract_iterator(p.filename().string(), file_region_iterator<const char>(p, 0, 65536));
    
    while (ait.next()) {
        std::string name = ait.name();
        std::string val;

        do {
            array_view<const char> rng = *ait;
            val += std::string(rng.begin(), rng.end());
            ++ait;
        } while (!ait.empty());
        result[name] = val;
    }
    BOOST_CHECK (ait.empty());
    file_region_iterator<const char> bit = std::move(*reinterpret_cast<file_region_iterator<const char>*>(ait.get_base()));
    BOOST_CHECK (bit.empty());
    return std::move(result);
}

}

BOOST_AUTO_TEST_CASE (archive_extract_iterator_test)
{
    char const* path = std::getenv("SONIA_PRIME_HOME");
    BOOST_REQUIRE_MESSAGE(path, "SONIA_PRIME_HOME must be set");

    fs::path sonia_prime_home{ path };
    fs::path archive_test_home{ sonia_prime_home / "tests" / "data" / "archives"};
    std::map<std::string, std::string> cnt;

#if 1
    // just a file
    cnt = load_archive(archive_test_home / "file0.txt");
    BOOST_CHECK_EQUAL(cnt.size(), 1);
    BOOST_CHECK_EQUAL(cnt["file0.txt"], "file0");

    // gzipped file
    cnt = load_archive(archive_test_home / "file0.txt.gz");
    BOOST_CHECK_EQUAL(cnt.size(), 1);
    BOOST_CHECK_EQUAL(cnt["file0.txt"], "file0");

    // bzipp2ed file
    cnt = load_archive(archive_test_home / "file0.txt.bz2");
    BOOST_CHECK_EQUAL(cnt.size(), 1);
    BOOST_CHECK_EQUAL(cnt["file0.txt"], "file0");

    // tar file
    cnt = load_archive(archive_test_home / "files.tar");
    BOOST_CHECK_EQUAL(cnt.size(), 2);
    BOOST_CHECK_EQUAL(cnt["files/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["files/file1.txt"], "file1\r\n");

    // tgz file
    cnt = load_archive(archive_test_home / "files.tgz");
    BOOST_CHECK_EQUAL(cnt.size(), 2);
    BOOST_CHECK_EQUAL(cnt["files/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["files/file1.txt"], "file1\r\n");

    // gzipped tar file
    cnt = load_archive(archive_test_home / "files.tar.gz");
    BOOST_CHECK_EQUAL(cnt.size(), 2);
    BOOST_CHECK_EQUAL(cnt["files/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["files/file1.txt"], "file1\r\n");

    // bzipped2 tar file
    cnt = load_archive(archive_test_home / "files.tar.bz2");
    BOOST_CHECK_EQUAL(cnt.size(), 2);
    BOOST_CHECK_EQUAL(cnt["files/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["files/file1.txt"], "file1\r\n");

    cnt = load_archive(archive_test_home / "fileset0.tgz");
    BOOST_CHECK_EQUAL(cnt.size(), 5);
    BOOST_CHECK_EQUAL(cnt["fileset0/file0.txt"], "file0");
    BOOST_CHECK_EQUAL(cnt["fileset0/file1.txt"], "file1");
    BOOST_CHECK_EQUAL(cnt["fileset0/files2/file2.txt"], "file2");
    BOOST_CHECK_EQUAL(cnt["fileset0/files5/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset0/files5/file1.txt"], "file1\r\n");
#endif
#if 1
    cnt = load_archive(archive_test_home / "fileset1.tgz");
    BOOST_CHECK_EQUAL(cnt.size(), 7);
    BOOST_CHECK_EQUAL(cnt["fileset1/file0.txt"], "file0");
    BOOST_CHECK_EQUAL(cnt["fileset1/file1.txt"], "file1");
    BOOST_CHECK_EQUAL(cnt["fileset1/files2/file2.txt"], "file2");
    BOOST_CHECK_EQUAL(cnt["fileset1/files2/files/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset1/files2/files/file1.txt"], "file1\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset1/files5/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset1/files5/file1.txt"], "file1\r\n");
#endif
#if 1
    // gzipped tar files of different items
    cnt = load_archive(archive_test_home / "fileset.tgz");
    BOOST_CHECK_EQUAL(cnt.size(), 12);
    BOOST_CHECK_EQUAL(cnt["fileset/file0.txt"], "file0");
    BOOST_CHECK_EQUAL(cnt["fileset/file1.txt"], "file1");
    BOOST_CHECK_EQUAL(cnt["fileset/files5/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset/files5/file1.txt"], "file1\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/file2.txt"], "file2");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/file0.txt"], "file0");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/files/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/files/file1.txt"], "file1\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/files3/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/files3/file1.txt"], "file1\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/files4/file0.txt"], "file0\r\n");
    BOOST_CHECK_EQUAL(cnt["fileset/files2/files4/file1.txt"], "file1\r\n");
    
#endif
}
#endif
