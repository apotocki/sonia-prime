//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_TAR_EXTRACT_ITERATOR_HPP
#define SONIA_UTILITY_TAR_EXTRACT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/optional.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/utility/iterators/range_reference.hpp"
#include "sonia/utility/algorithm/copy.hpp"

namespace sonia {

// tar things are based on https://techoverflow.net/2013/03/29/reading-tar-files-in-c/
struct tar_header
{
    char filename[100]; //NUL-terminated
    char mode[8];
    char uid[8];
    char gid[8];
    char file_size[12];
    char last_modification[12];
    char checksum[8];
    char type_flag; //Also called link indicator for none-UStar format
    char linked_file_name[100];

    //USTar-specific fields -- NUL-filled in non-USTAR version
    char ustar_indicator[6]; //"ustar" -- 6th character might be NUL but results show it doesn't have to
    char ustar_version[2]; //00
    char owner_user_name[32];
    char owner_group_name[32];
    char device_major_number[8];
    char device_minor_number[8];
    char filename_prefix[155];
    char padding[12]; //Nothing of interest, but relevant for checksum

    bool is_ustar() const;
    bool check_checksum();
    bool is_end() const;

    uint64_t get_file_size() const;

    std::string get_filename() const;
};

template <class IteratorT>
class tar_extract_iterator
    : public boost::iterator_facade<
        tar_extract_iterator<IteratorT>,
        range_reference_t<iterator_value_t<IteratorT>>,
        iterator_traversal_t<IteratorT>,
        range_reference_t<iterator_value_t<IteratorT>>>
    , range_dereferencing_iterator_state<IteratorT>
{
    friend class boost::iterator_core_access;
    using rng_state_t = range_dereferencing_iterator_state<IteratorT>;
    using reference_type = range_reference_t<iterator_value_t<IteratorT>>;

    std::string current_name_;
    uint64_t current_size_;
    uint16_t padding_;
    bool closed_;

    bool equal(tar_extract_iterator const& rhs) const
    {
        return std::get<0>(rng_state_t::get()) == std::get<0>(rhs.get());
    }

    reference_type dereference() const
    {
        auto & rng = rng_state_t::get();
        auto eit = std::get<1>(rng);
        ptrdiff_t sz = eit - std::get<0>(rng);
        if (((uint64_t)sz) > current_size_) {
            eit = std::get<0>(rng) + current_size_;
        }
        return range_reference<iterator_value_t<IteratorT>>::make(std::get<0>(rng), eit);
    }

    void increment()
    {
        auto & rng = rng_state_t::get();
        auto & bit = std::get<0>(rng);
        auto & eit = std::get<1>(rng);
        uint64_t sz = (uint64_t)(eit - bit);
        if (sz > current_size_) {
            bit += current_size_;
            for (;;) {
                sz = (uint64_t)(eit - bit);
                if (sz > padding_) {
                    bit += padding_;
                    closed_ = true;
                    return;
                }
                padding_ -= (uint16_t)sz;
                rng_state_t::increment();
                rng = rng_state_t::get();
            }
        } else {
            current_size_ -= sz;
            rng_state_t::increment();
        }
    }

public:
    using base_iterator_type = IteratorT;

    explicit tar_extract_iterator(IteratorT it)
        : rng_state_t(std::move(it)), closed_(true)
    {
        
    }

    std::string const& name() const { return current_name_; }
    
    uint64_t current_size() const { return current_size_; }

    bool empty() const { return closed_; }

    IteratorT & base() { return rng_state_t::base(); }
    IteratorT const& base() const { return rng_state_t::base(); }

    bool next()
    {
        process();
        return !closed_;
    }

    void load_header(tar_header &h)
    {
        char * h_it = reinterpret_cast<char*>(&h), *h_eit = h_it + sizeof(tar_header);
        for (;;) {
            if (rng_state_t::empty()) {
                throw exception("tar_extract_iterator : unexpected eof");
            }
            auto & st = rng_state_t::get();
            std::tie(std::get<0>(st), h_it) = copy_not_more(std::get<0>(st), std::get<1>(st), h_it, h_eit - h_it);
            if (h_it == h_eit) break;
            rng_state_t::increment();
        }
    }

    void process()
    {
        tar_header h;
        bool next_entry_has_long_name = false;
        std::string name;

        for (;;) {
            load_header(h);

            if (h.is_end()) {
                closed_ = true;
                return;
            }

            if (next_entry_has_long_name) {
                // Set the filename from the current header
                current_name_ = h.filename;
                next_entry_has_long_name = false;
                load_header(h);
            } else {
                current_name_ = h.get_filename();
            }

            if (h.type_flag == '0' || h.type_flag == 0) {
                current_size_ = h.get_file_size();
                padding_ = (512 - (current_size_ % 512)) % 512;
                closed_ = false;
                return;
            } else if (h.type_flag == '5') {
                // skip directory
            } else if (h.type_flag == 'L') {
                next_entry_has_long_name = true;
            } else {
                // Neither normal file nor directory (symlink etc.) -- currently ignored silently
                closed_ = true;
                throw exception("found unhandled TAR Entry type '%1%', for name:  %2%"_fmt % h.type_flag % name);
            }
        }
    }
};

}

#endif // SONIA_UTILITY_TAR_EXTRACT_ITERATOR_HPP
