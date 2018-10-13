//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_FILE_HPP
#define SONIA_IO_FILE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"

namespace sonia { namespace io {

enum class file_open_mode {
    create,
    create_or_open,
    open
};

enum class file_access_mode {
    read,
    write
};

enum class file_bufferring_mode {
    not_buffered,
    buffered
};

class file_service {
public:
    virtual ~file_service() {}

    virtual size_t file_read(void * handle, uint64_t fileoffset, array_view<char> dest) = 0;
    virtual size_t file_write(void * handle, uint64_t fileoffset, array_view<const char> src) = 0;
};

class file
{
    friend class file_access;

    file(file const&) = delete;
    file& operator=(file const&) = delete;

    file(shared_ptr<file_service> s, void * handle)
        : impl_(std::move(s)), fh_(handle)
    {}

public:
    file(file && f) : impl_(std::move(f.impl_)), fh_(f.fh_) {
        f.fh_ = nullptr;
    }

    file & operator= (file && f) {
        if (fh_ != f.fh_) {
            close();
            fh_ = f.fh_;
            impl_ = std::move(f.impl_);
            f.fh_ = nullptr;
        }
        return *this;
    }

    ~file();

    void close();

    uint64_t size() const;
    void set_size(uint64_t sz);

    size_t read(uint64_t fileoffset, array_view<char> dest);
    size_t write(uint64_t fileoffset, array_view<const char> src);
    
    void remove();

private:
    shared_ptr<file_service> impl_;
    void * fh_;
};

class file_access {
public:
    static file open_file(shared_ptr<file_service> impl, void * handle) {
        return file(std::move(impl), handle);
    }
};

class file_factory {
public:
    virtual ~file_factory() {}
    virtual file open_file(
        string_view path,
        file_open_mode fom = file_open_mode::open,
        file_access_mode fam = file_access_mode::read,
        file_bufferring_mode fbm = file_bufferring_mode::not_buffered) = 0;
};

}}

#endif // SONIA_IO_FILE_HPP
