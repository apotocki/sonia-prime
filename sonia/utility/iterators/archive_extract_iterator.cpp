//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "archive_extract_iterator.hpp"

namespace sonia { namespace archive_detail {

archive_type get_archive_type(string_view str)
{
    if (str.size() > 3) return archive_type::UNKNOWN;

    char buff[8];
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
    if (ucasestr == "TGZ") return archive_type::TGZ;
    return archive_type::UNKNOWN;
}

std::pair<string_view, archive_type> split_name(string_view nm)
{
    auto rit = std::find(nm.rbegin(), nm.rend(), '.');
    string_view extstr(rit.base(), nm.end() - rit.base());
    string_view name_wo_ext(nm.begin(), rit.base() - (rit != nm.rend() ? 1 : 0));
    return { name_wo_ext, get_archive_type(extstr) };
}

bool extract_iterator_polymorpic_adapter_base::do_next(shared_ptr<archive_iterator_polymorphic> & ptr)
{
    if (archive_type::UNDEFINED == type_) {
        string_view name_wo_ext;
        std::tie(name_wo_ext, type_) = split_name(do_get_name());
        if (archive_type::UNKNOWN == type_) {
            return !ptr->empty();
        }

        std::string name_wo_ext_str = to_string(name_wo_ext);
        switch (type_)
        {
        case archive_type::GZIP:
            ptr = make_shared<extract_iterator_polymorpic_adapter<buffering_mediator_iterator<inflate_iterator<archive_iterator>>>>(
                std::move(name_wo_ext_str), buffsz_, buffering_mediator_iterator(inflate_iterator(archive_iterator(std::move(ptr)), true), buffsz_)
                );
            return false;
        case archive_type::BZIP2:
            ptr = make_shared<extract_iterator_polymorpic_adapter<buffering_mediator_iterator<bz2_decompress_iterator<archive_iterator>>>>(
                std::move(name_wo_ext_str), buffsz_, buffering_mediator_iterator(bz2_decompress_iterator(archive_iterator(std::move(ptr))), buffsz_)
                );
            return false;
        case archive_type::TAR:
            ptr = make_shared<extract_iterator_polymorpic_adapter<tar_extract_iterator<archive_iterator>>>(
                std::move(name_wo_ext_str), buffsz_, archive_iterator(std::move(ptr))
                );
            return false;
        case archive_type::TGZ:
        {
            auto tmpptr = make_shared<extract_iterator_polymorpic_adapter<buffering_mediator_iterator<inflate_iterator<archive_iterator>>>>(
                "", buffsz_, buffering_mediator_iterator(inflate_iterator(archive_iterator(std::move(ptr)), true), buffsz_)
                );
            ptr = make_shared<extract_iterator_polymorpic_adapter<tar_extract_iterator<archive_iterator>>>(
                std::move(name_wo_ext_str), buffsz_, archive_iterator(std::move(tmpptr))
                );
            return false;
        }
        default:
            BOOST_THROW_EXCEPTION(internal_error("unexpected archive type: %1%"_fmt % (int)type_));
        }
    } else {
        archive_iterator * pbs = ptr->pbase();
        if (pbs) { // pop
            ptr = std::move(pbs->impl.ptr);
        }
        else { // no more iterators on the stack
            ptr.reset();
        }
        return false;
    }
}

}}

