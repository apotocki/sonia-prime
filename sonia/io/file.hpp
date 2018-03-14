//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_FILE_HPP
#define SONIA_IO_FILE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/filesystem/path.hpp>

#include "sonia/cstdint.hpp"

namespace sonia { namespace io {

class file
{
    file(file const&) = delete;

public:
    file(boost::filesystem::path const& fp, bool create_if_not_exists);
    void open();

    uint64_t size() const;
    void set_size();

    void read(uint64_t fileoffset, void * dest, uint64_t count);
    void write(void const* src, uint64_t count, uint64_t fileoffset);
    
    void remove();

private:
    void * fh_;
};

}}
#endif // SONIA_IO_FILE_HPP
