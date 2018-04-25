//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "file_persister.hpp"

#include <fstream>

#include "sonia/utility/iterators/file_region_iterator.hpp"

namespace sonia {

file_persister::file_persister(std::string fname)
    : fname_(std::move(fname))
{
    std::ofstream f(fname_.c_str());
}

file_persister::~file_persister()
{

}

persister::input_iterator file_persister::reader() const
{
    typedef iterator_polymorpic_impl_adapter<file_region_iterator<const uint8_t>, forward_traversal_tag> impl_t;
    return input_iterator(in_place, in_place_type<impl_t>, fname_.c_str());
}

persister::output_iterator file_persister::writer()
{
    typedef iterator_polymorpic_impl_adapter<file_region_iterator<uint8_t>, forward_traversal_tag, array_view<uint8_t>> impl_t;
    return output_iterator(in_place, in_place_type<impl_t>, fname_.c_str());
}

}
