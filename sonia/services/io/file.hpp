//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/sal_types.hpp"

namespace sonia::io {

enum class file_open_mode
{
    create,
    create_or_open,
    open
};

enum class file_access_mode
{
    read,
    write
};

enum class file_bufferring_mode
{
    not_buffered,
    buffered
};

class file_service
{
public:
    virtual ~file_service() = default;

    virtual size_t file_read(sonia::sal::file_handle_type handle, uint64_t fileoffset, array_view<char> dest) = 0;
    virtual size_t file_write(sonia::sal::file_handle_type handle, uint64_t fileoffset, array_view<const char> src) = 0;
};

class file
{
    friend class file_access;

    file(file const&) = delete;
    file& operator=(file const&) = delete;

    file(shared_ptr<file_service> s, sonia::sal::file_handle_type handle, u8string_view path = u8string_view())
        : impl_(std::move(s)), path_(to_string(path)), fh_(handle)
    {}

public:
    file(file && rhs);
    ~file();

    file & operator= (file && rhs);

    void close() noexcept;

    uint64_t size() const;
    void set_size(uint64_t sz);

    size_t read(uint64_t fileoffset, array_view<char> dest) const;
    size_t write(uint64_t fileoffset, array_view<const char> src);
    
    void flush() const;

    void remove();

    cstring_view path() const;

private:
    shared_ptr<file_service> impl_;
    mutable std::u8string path_;
    sonia::sal::file_handle_type fh_;
};

class file_access
{
public:
    static file open_file(shared_ptr<file_service> impl, sonia::sal::file_handle_type handle, u8string_view path = u8string_view())
    {
        return file(std::move(impl), handle, path);
    }
};

class file_factory
{
public:
    virtual ~file_factory() = default;
    virtual file open_file(
        cstring_view path,
        file_open_mode fom = file_open_mode::open,
        file_access_mode fam = file_access_mode::read,
        file_bufferring_mode fbm = file_bufferring_mode::not_buffered) = 0;
};

}
