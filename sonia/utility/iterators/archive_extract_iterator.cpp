//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "archive_extract_iterator.hpp"

namespace sonia::archive_detail {

archive_type get_archive_type(string_view str)
{
    if (str.size() <= 3) {
        char buff[16];
        char * buff_it = buff;
        for (char c : str) {
            if (c >= 'a' && c <= 'z') c -= ('a' - 'A');
            *buff_it++ = c;
        }
        *buff_it = 0;
        cstring_view ucasestr((char*)buff, buff_it);
        if (ucasestr == "TAR") return archive_type::TAR;
        if (ucasestr == "GZ") return archive_type::GZIP;
        if (ucasestr == "BZ2") return archive_type::BZIP2;
        if (ucasestr == "XZ") return archive_type::XZ;
        if (ucasestr == "TGZ") return archive_type::TGZ;
    }
    return archive_type::UNKNOWN;
}

std::pair<string_view, archive_type> split_name(string_view nm)
{
    auto rit = std::find(nm.rbegin(), nm.rend(), '.');
    string_view extstr(rit.base(), nm.end());
    string_view name_wo_ext(nm.begin(), rit.base() - (rit != nm.rend() ? 1 : 0));
    return { name_wo_ext, get_archive_type(extstr) };
}

namespace {

template <class T, typename ... ArgsT>
void replace(archive_iterator & r, ArgsT&& ... args)
{
    archive_iterator tmp(in_place_type<T>, std::forward<ArgsT>(args) ...);
    r = std::move(tmp);
}

}

bool extract_iterator_polymorpic_adapter_base::do_next(archive_iterator & ax)
{
    if (archive_type::UNDEFINED == type_) {
        if (!extraction_depth_) { type_ = archive_type::UNKNOWN; return !ax.empty(); }
        string_view name_wo_ext;
        std::tie(name_wo_ext, type_) = split_name(do_get_name());
        if (archive_type::UNKNOWN == type_) {
            return !ax.empty();
        }

        std::string name_wo_ext_str = to_string(name_wo_ext);
        switch (type_)
        {
        case archive_type::GZIP:
            replace<extract_iterator_polymorpic_adapter<buffering_mediator_iterator<inflate_iterator<archive_iterator>>>>(
                ax,
                std::move(name_wo_ext_str), buffsz_, extraction_depth_ - 1, in_place, buffsz_, std::move(ax), true
            );
            return false;
        case archive_type::BZIP2:
            replace<extract_iterator_polymorpic_adapter<buffering_mediator_iterator<bz2_decompress_iterator<archive_iterator>>>>(
                ax,
                std::move(name_wo_ext_str), buffsz_, extraction_depth_ - 1, in_place, buffsz_, std::move(ax)
            );
            return false;
        case archive_type::XZ:
            replace<extract_iterator_polymorpic_adapter<buffering_mediator_iterator<lzma_decompress_iterator<archive_iterator>>>>(
                ax,
                std::move(name_wo_ext_str), buffsz_, extraction_depth_ - 1, in_place, buffsz_, std::move(ax)
            );
            return false;
        case archive_type::TAR:
            replace<extract_iterator_polymorpic_adapter<tar_extract_iterator<archive_iterator>>>(
                ax,
                std::move(name_wo_ext_str), buffsz_, extraction_depth_ - 1, std::move(ax)
            );
            return false;
        case archive_type::TGZ:
        {
            archive_iterator tmp(
                in_place_type<extract_iterator_polymorpic_adapter<buffering_mediator_iterator<inflate_iterator<archive_iterator>>>>,
                "", buffsz_, extraction_depth_ - 1, in_place, buffsz_, std::move(ax), true
            );
            replace<extract_iterator_polymorpic_adapter<tar_extract_iterator<archive_iterator>>>(
                ax,
                std::move(name_wo_ext_str), buffsz_, extraction_depth_ - 1, std::move(tmp)
            );
            return false;
        }
        default:
            THROW_INTERNAL_ERROR("unexpected archive type: %1%"_fmt % (int)type_);
        }
    } else {
        this->finished = true;
        archive_iterator * pbs = ax.pbase();
        if (pbs) { // pop
            archive_iterator tmp(std::move(*pbs));
            ax = std::move(tmp);
        } else { // no more iterators on the stack
            return true;
            //ax.reset();
        }
        return false;
    }
}

}
