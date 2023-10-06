//  Copyright (C) by Alexander A Pototskiy

#pragma once

#include <iterator>

#include "sonia/cstdint.hpp"
#include "sonia/utility/serialization/serialization.hpp"
#include "sonia/utility/serialization/integral.hpp"
#include "sonia/utility/serialization/span.hpp"

namespace sonia {

// https://docs.fileformat.com/compression/zip/
// https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT

template <std::random_access_iterator IteratorT>
class zipper
{
    using const_iterator = IteratorT;

    // End of Central Directory Record
    struct ecdrecord_type
    {
        static constexpr uint_least16_t min_size() { return 22; } // including signature

        uint_least16_t disknum; // Number of this disk
        uint_least16_t disknum_with_cd; // number of the disk with the start of the central directory
        uint_least16_t rec_count; // Number of central directory records on this disk
        uint_least16_t total_rec_count; // Total number of central directory records
        uint_least32_t size; // Size of central directory (bytes)
        uint_least32_t offset; // Offset of start of central directory, relative to start of archive
        uint_least16_t comment_length; // Comment length (n)

        template <typename ArchiveT>
        void archive(ArchiveT & arch)
        {
            using namespace sonia::serialization;
            arch & sized_integral<2>(disknum)
                 & sized_integral<2>(disknum_with_cd)
                 & sized_integral<2>(rec_count)
                 & sized_integral<2>(total_rec_count)
                 & sized_integral<4>(size)
                 & sized_integral<4>(offset)
                 & sized_integral<2>(comment_length);
        }
    };

    struct cd64locator_type
    {
        static constexpr uint_least16_t min_size() { return 20; }

        uint_least32_t disknum_with_cd; // number of the disk with the start of the zip64 end of central directory
        uint_least64_t offset; // relative offset of the zip64 end of central directory record
        uint_least32_t disk_count; // total number of disks

        template <typename ArchiveT>
        void archive(ArchiveT& arch)
        {
            using namespace sonia::serialization;
            arch & sized_integral<4>(disknum_with_cd)
                 & sized_integral<8>(offset)
                 & sized_integral<4>(disk_count);
        }
    };

    struct ecd64record_type
    {
        static constexpr uint_least16_t min_size() { return 56; } // including signature

        uint_least64_t size; // size of zip64 end of central directory record
        uint_least16_t version; // version made by
        uint_least16_t needed_version; // version needed to extract
        uint_least32_t disk_count; // number of this disk
        uint_least32_t disk_start; // number of the disk with the start of the central directory
        uint_least64_t disk_ecount; // total number of entries in the central directory on this disk
        uint_least64_t total_ecount; // total number of entries in the central directory
        uint_least64_t cdsize; // size of the central directory
        uint_least64_t offset; // offset of start of central directory with respect to the starting disk number
        // zip64 extensible data sector(variable size)

        template <typename ArchiveT>
        void archive(ArchiveT& arch)
        {
            using namespace sonia::serialization;
            arch & sized_integral<8>(size)
                 & sized_integral<2>(version)
                 & sized_integral<2>(needed_version)
                 & sized_integral<4>(disk_count)
                 & sized_integral<4>(disk_start)
                 & sized_integral<8>(disk_ecount)
                 & sized_integral<8>(total_ecount)
                 & sized_integral<8>(cdsize)
                 & sized_integral<8>(offset);
        }

        template <typename DecoderT>
        void decode(DecoderT& dec)
        {
            uint_least32_t signature;
            dec& sonia::serialization::sized_integral<4>(signature);
            if (0x06064b50 != signature) {
                throw std::runtime_error("wrong zip64 end of central directory record signature");
            }
            archive(dec);
        }
    };

    // Central Directory File Header
    struct cd_type
    {
        static constexpr uint_least32_t min_size() { return 46; }

        uint_least16_t version; // Version made by
        uint_least16_t needed_version; // Version needed to extract(minimum)
        uint_least16_t flag; // General purpose bit flag
        uint_least16_t method; // Compression method
        uint_least16_t time; // File last modification time
        uint_least16_t date; // File last modification date
        uint_least32_t crc32; // CRC-32
        uint_least32_t csize; // Compressed size
        uint_least32_t usize; // Uncompressed size
        uint_least16_t fnlength; // File name length (n)
        uint_least16_t elength; // Extra field length (m)
        uint_least16_t clength; // File comment length (k)
        uint_least16_t disk_start; // Disk number where file starts
        uint_least16_t iattrs; // Internal file attributes
        uint_least32_t eattrs; // External file attributes
        uint_least32_t offset; // Relative offset of local file header. This is the number of bytes between the start of the first disk on which the file occurs, and the start of the local file header. This allows software reading the central directory to locate the position of the file inside the ZIP file.

        template <typename ArchiveT>
        void archive(ArchiveT& arch)
        {
            using namespace sonia::serialization;
            arch & sized_integral<2>(version)
                 & sized_integral<2>(needed_version)
                 & sized_integral<2>(flag)
                 & sized_integral<2>(method)
                 & sized_integral<2>(time)
                 & sized_integral<2>(date)
                 & sized_integral<4>(crc32)
                 & sized_integral<4>(csize)
                 & sized_integral<4>(usize)
                 & sized_integral<2>(fnlength)
                 & sized_integral<2>(elength)
                 & sized_integral<2>(clength)
                 & sized_integral<2>(disk_start)
                 & sized_integral<2>(iattrs)
                 & sized_integral<4>(eattrs)
                 & sized_integral<4>(offset);
        }

        template <typename DecoderT>
        void decode(DecoderT& dec)
        {
            uint_least32_t signature;
            dec & sonia::serialization::sized_integral<4>(signature);
            if (0x02014b50 != signature) {
                throw std::runtime_error("wrong Central Directory File Header signature");
            }
            archive(dec);
        }
    };

public:
    // read archive
    zipper(const_iterator first, const_iterator last);

    void sort_by_names();

    size_t count() const { return entries_.size(); }
    std::string_view name_at(size_t pos) const
    {
        const char* b = names_.data() + entries_[pos].name_offset;
        return b;
    }

private:
    struct entry
    {
        uint64_t offset;
        uint32_t name_offset;
    };

    std::vector<entry> entries_;
    std::vector<char> names_;
};

template <std::random_access_iterator IteratorT>
zipper<IteratorT>::zipper(const_iterator b, const_iterator e)
{
    uint_least64_t total_sz = e - b;
    if (total_sz < ecdrecord_type::min_size()) {
        throw std::runtime_error("bad zip");
    }
    
    const_iterator it = e - 1;
    auto finder = [](char last_byte, uint_least32_t maxdistance, const_iterator & it, const_iterator end) -> char {
        for (uint_least32_t d = 0; it != end && d < maxdistance; ++d) {
            char c = *it;
            if (c == last_byte) {
                --it;
                if (it == end) break;
                char c1 = *it;
                if (c1 + 1 != c) continue;
                --it; ++d;
                if (it == end) break;
                if (*it != 0x4b) continue;
                --it; ++d;
                if (*it != 0x50) continue;
                it += 4;
                return c;
            }
            --it;
        }
        return 0;
    };

    // CD 0x06054b50
    char r = finder(0x6, 65535 + ecdrecord_type::min_size(), it, b);
    size_t tail_sz = e - it;
    if (!r || tail_sz < ecdrecord_type::min_size() - 4) {
        throw std::runtime_error("can't find ECD");
    }
    
    ecdrecord_type ecd;
    auto decoder = make_decoder<serialization::le_t>(it);
    ecd.archive(decoder);

    uint_least64_t cd_offset = ecd.offset;
    uint_least64_t cd_size = ecd.size;

    if (cd_offset == 0xffffffff) { // ZIP64
        it -= 4;
        // CD64 0x07064b50
        r = finder(0x7, 65535, it, b);
        if (!r || e - it - tail_sz < cd64locator_type::min_size()) {
            throw std::runtime_error("can't find zip64 end of central directory locator");
        }
        cd64locator_type cd64locator;
        decoder = make_decoder<serialization::le_t>(it);
        cd64locator.archive(decoder);

        uint_least64_t cd64l_offset = it - b - 4;
        if (cd64locator.offset + ecd64record_type::min_size() > cd64l_offset) {
            throw std::runtime_error("wrong zip64 end of central directory record offset");
        }
        ecd64record_type ecd64;
        decoder = make_decoder<serialization::le_t>(b + cd64locator.offset);
        ecd64.decode(decoder);

        cd_offset = ecd64.offset;
        cd_size = ecd64.cdsize;

        tail_sz = total_sz - cd64locator.offset;
    }
    if ((total_sz - tail_sz) < cd_offset + cd_size) {
        throw std::runtime_error("wrong CD offset");
    }
    
    cd_type cd;
    decoder = make_decoder<serialization::le_t>(b + cd_offset);

    while (cd_size) {
        if (cd_size < cd_type::min_size()) {
            throw std::runtime_error("wrong CD size");
        }
        cd.decode(decoder);
        cd_size -= cd_type::min_size();

        uint_least32_t extra_size = cd.fnlength;
        extra_size += cd.elength;
        extra_size += cd.clength;

        if (ecd.size < extra_size) {
            throw std::runtime_error("wrong CD size");
        }

        uint_least32_t namepos = integral_cast<uint_least32_t>(names_.size());
        names_.resize(namepos + cd.fnlength + 1); // 0 temintated string
        decoder& std::span<char>{names_.data() + namepos, cd.fnlength};
        names_.back() = 0;

        entries_.emplace_back(cd.offset, namepos);

        // read extra field
        decoder.iterator() += cd.elength;

        // read comment
        decoder.iterator() += cd.clength;

        cd_size -= extra_size;
    }


}

template <std::random_access_iterator IteratorT>
void zipper<IteratorT>::sort_by_names()
{
    const char* names = names_.data();
    std::sort(entries_.begin(), entries_.end(), [names](entry const& l, entry const& r) -> bool {
        return std::strcmp(names + l.name_offset, names + r.name_offset) < 0;
    });
}

}
