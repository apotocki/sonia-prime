//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "file_persister.hpp"

#include <fstream>

#include "sonia/utility/iterators/file_region_iterator.hpp"

namespace sonia {

file_persister::file_persister(fs::path fname)
    : fname_(std::move(fname))
{

}

file_persister::~file_persister()
{

}

bool file_persister::read(function<void(input_iterator)> const& ftor) const
{
    using impl_t = output_iterator_polymorpic_adapter<
        file_region_iterator<const char>,
        forward_traversal_tag,
        std::span<const char>,
        std::span<const char>
    >;

    if (fs::exists(fname_)) {
        ftor(input_iterator(in_place_type<impl_t>, fname_));
    } else {
        fs::path tmpfile = fname_.string() + ".tmp";
        if (!fs::exists(tmpfile)) return false;

        ftor(input_iterator(in_place_type<impl_t>, tmpfile));
    }
    return true;
}

void file_persister::write(function<void(output_iterator)> const& ftor)
{
    using impl_t = output_iterator_polymorpic_adapter<
        file_region_iterator<char>,
        forward_traversal_tag,
        std::span<char>,
        std::span<char>
    >;

    // firstly write into a temp file, then rename it

    fs::path tmpfile = fname_.string() + ".tmp";
    if (fs::exists(tmpfile)) {
        fs::remove(tmpfile);
    }
    try {
        fs::path tmpfilefolder = fs::path(tmpfile).parent_path();
        if (!tmpfilefolder.empty() && !fs::exists(tmpfilefolder)) {
            fs::create_directories(tmpfilefolder);
        }

        std::ofstream(tmpfile.c_str());

        ftor(output_iterator(in_place_type<impl_t>, tmpfile));
    } catch (...) {
        fs::remove(tmpfile);
        throw;
    }
    if (fs::exists(fname_)) fs::remove(fname_);
    fs::rename(tmpfile, fname_);
}

}
