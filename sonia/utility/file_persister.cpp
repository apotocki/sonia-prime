//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "file_persister.hpp"

#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#include "sonia/utility/iterators/file_region_iterator.hpp"

namespace sonia {

file_persister::file_persister(std::string fname)
    : fname_(std::move(fname))
{

}

file_persister::~file_persister()
{

}

bool file_persister::read(function<void(input_iterator)> const& ftor) const
{
    typedef iterator_polymorpic_impl_adapter<file_region_iterator<const uint8_t>, forward_traversal_tag> impl_t;

    if (fs::exists(fname_)) {
        ftor(input_iterator(in_place_type<impl_t>, fname_.c_str()));
    } else {
        std::string tmpfile = fname_ + ".tmp";
        if (!fs::exists(tmpfile)) return false;

        ftor(input_iterator(in_place_type<impl_t>, tmpfile.c_str()));
    }
    return true;
}

void file_persister::write(function<void(output_iterator)> const& ftor)
{
    typedef iterator_polymorpic_impl_adapter<file_region_iterator<uint8_t>, forward_traversal_tag, array_view<uint8_t>> impl_t;

    // firstly write into a temp file, then rename it

    std::string tmpfile = fname_ + ".tmp";
    if (fs::exists(tmpfile)) {
        fs::remove(tmpfile);
    }
    try {
        fs::path tmpfilefolder = fs::path(tmpfile).parent_path();
        if (!tmpfilefolder.empty() && !fs::exists(tmpfilefolder)) {
            fs::create_directories(tmpfilefolder);
        }
        {
            std::ofstream f(tmpfile.c_str());
        }

        ftor(output_iterator(in_place_type<impl_t>, tmpfile.c_str()));
    } catch (...) {
        fs::remove(tmpfile);
        throw;
    }
    if (fs::exists(fname_)) fs::remove(fname_);
    fs::rename(tmpfile, fname_);
}

}
