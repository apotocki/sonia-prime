//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "file_statable.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/file_persister.hpp"

#ifndef SONIA_FILE_STATABLE_NVP_TAG_NAME
#   define SONIA_FILE_STATABLE_NVP_TAG_NAME "state"
#endif

namespace sonia::utility {
    
template <typename DerivedT>
void file_statable<DerivedT>::open()
{
    path_ref_t sp = derived().get_state_path();
    if (auto * psp = boost::get<std::filesystem::path>(&sp); psp) {
        state_persister_ = sonia::make_shared<file_persister>(*psp);
    } else if (auto * psp = boost::get<std::filesystem::path const&>(&sp); psp) {
        state_persister_ = sonia::make_shared<file_persister>(*psp);
    } else {
        THROW_INTERNAL_ERROR("no state path defined");
    }

    derived().restore();
}

template <typename DerivedT>
typename file_statable<DerivedT>::name_ref_t file_statable<DerivedT>::get_state_name() const
{
    return std::string{to_string(".%1%.state"_fmt % derived().get_name())};
}

template <typename DerivedT>
typename file_statable<DerivedT>::path_ref_t file_statable<DerivedT>::get_state_path() const
{
    name_ref_t snm = derived().get_state_name();
    std::string const* psnm = boost::get<std::string>(&snm);
    if (!psnm) {
        psnm = &boost::get<std::string const&>(snm);
    }
    std::filesystem::path path;
    path_ref_t dir = derived().get_state_dir();
    if (auto * pdir = boost::get<std::filesystem::path>(&dir); pdir) {
        return *pdir / *psnm;
    } else if (auto * pdir = boost::get<std::filesystem::path const&>(&dir); pdir) {
        return *pdir / *psnm;
    } else {
        return std::filesystem::path{*psnm};
    }
}

template <typename DerivedT>
void file_statable<DerivedT>::restore()
{
    if (!state_persister_->read_stream([this](std::istream & is) {
        boost::archive::xml_iarchive ia{is};
        ia >> boost::serialization::make_nvp(SONIA_FILE_STATABLE_NVP_TAG_NAME, derived());
    })) {
        derived().bootstrap();
    }
}

template <typename DerivedT>
void file_statable<DerivedT>::backup() const
{
    state_persister_->write_stream([this](std::ostream & os) {
        boost::archive::xml_oarchive oa{os};
        oa << boost::serialization::make_nvp(SONIA_FILE_STATABLE_NVP_TAG_NAME, const_cast<DerivedT&>(derived()));
    });
}

}
