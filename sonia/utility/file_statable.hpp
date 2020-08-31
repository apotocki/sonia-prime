//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_FILE_STATABLE_HPP
#define SONIA_UTILITY_FILE_STATABLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <filesystem>

#include <boost/variant.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/utility/persister.hpp"

namespace sonia::utility {

template <typename DerivedT>
class file_statable
{
public:
    using name_ref_t = boost::variant<std::string, std::string const&>;
    using path_ref_t = boost::variant<null_t, std::filesystem::path, std::filesystem::path const&>;

    //template <class ArchiveT> void serialize(ArchiveT & ar, const unsigned int = 0)
    //{
    //    ar & derived();
    //}
    
    path_ref_t get_state_path() const;
    path_ref_t get_state_dir() const { return null_t{}; }
    name_ref_t get_state_name() const;

    void open();
    void restore();
    void backup() const;
    void bootstrap() {};

    DerivedT & derived() { return static_cast<DerivedT&>(*this); }
    DerivedT const& derived() const { return static_cast<DerivedT const&>(*this); }

protected:
    shared_ptr<persister> state_persister_;
};

}

#endif // SONIA_UTILITY_FILE_STATABLE_HPP
