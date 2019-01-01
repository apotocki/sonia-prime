//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tar_iterator.hpp"

#include <cstring>

namespace sonia {

#define ASCII_TO_NUMBER(num) ((num)-48) //Converts an ascii digit to the corresponding number (assuming it is an ASCII digit)

uint64_t decode_octal(const char* data, size_t size)
{
    const unsigned char* current_ptr = (const unsigned char*)data + size;
    uint64_t sum = 0;
    uint64_t current_multiplier = 1;
    //Skip everything after the last NUL/space character
    //In some TAR archives the size field has non-trailing NULs/spaces, so this is neccessary
    const unsigned char* check_ptr = current_ptr; //This is used to check where the last NUL/space char is
    for (; check_ptr >= (const unsigned char*)data; --check_ptr) {
        if ((*check_ptr) == 0 || (*check_ptr) == ' ') {
            current_ptr = check_ptr - 1;
        }
    }
    for (; current_ptr >= (const unsigned char*)data; --current_ptr) {
        sum += ASCII_TO_NUMBER(*current_ptr) * current_multiplier;
        current_multiplier *= 8;
    }
    return sum;
}

bool tar_header::is_ustar() const
{
    return (std::memcmp("ustar", ustar_indicator, 5) == 0);
}

bool tar_header::check_checksum()
{
    // We need to set the checksum to zer
    char original_checksum[8];
    std::memcpy(original_checksum, checksum, 8);
    std::memset(checksum, ' ', 8);
    //Calculate the checksum -- both signed and unsigned
    int64_t unsigned_sum = 0;
    int64_t signed_sum = 0;
    for (int i = 0; i < sizeof (tar_header); ++i) {
        unsigned_sum += ((unsigned char*) this)[i];
        signed_sum += ((signed char*) this)[i];
    }
    //Copy back the checksum
    std::memcpy(checksum, original_checksum, 8);
    //Decode the original checksum
    uint64_t reference_checksum = decode_octal(original_checksum, 12);
    return (reference_checksum == unsigned_sum || reference_checksum == signed_sum);
}

bool tar_header::is_end() const
{
    return 512 == std::count((char*)this, (char*)this + 512, 0);
}

uint64_t tar_header::get_file_size() const
{
    return decode_octal(file_size, 12);
}

std::string tar_header::get_filename() const
{
    std::string fname;

    size_t prefixLength = strlen(filename_prefix);
    if (prefixLength > 0) {
        fname = std::string(filename_prefix, (std::min)((size_t)155, prefixLength));
        fname.push_back('/');
    }
    fname.insert(fname.end(), filename, filename + (std::min)((size_t)100, strlen(filename)));
    return std::move(fname);
}

}

