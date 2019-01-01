//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ARCHIVE_EXTRACT_ITERATOR_HPP
#define SONIA_UTILITY_ARCHIVE_EXTRACT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <list>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/variant.hpp>

#include "sonia/iterator_traits.hpp"
#include "sonia/utility/automatic_pointer.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"
#include "sonia/utility/iterators/buffering_mediator_iterator.hpp"
#include "sonia/utility/iterators/inflate_iterator.hpp"
#include "sonia/utility/iterators/bz2_decompress_iterator.hpp"
#include "sonia/utility/iterators/tar_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"

#ifndef DEFAULT_ARCHIVER_INTERNAL_BUFFER_SIZE
#   define DEFAULT_ARCHIVER_INTERNAL_BUFFER_SIZE 65536
#endif

namespace sonia {

enum class archive_type
{
    TAR,        // tar
    GZIP,       // gz
    BZIP2,      // bz2
    TGZ,        // gzipped tar
    UNKNOWN
};

inline archive_type get_archive_type(string_view str)
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

using archive_iterator_t = wrapper_iterator<
    automatic_pointer<shared_ptr<iterator_polymorphic<array_view<const char>>>>,
    array_view<const char>,
    forward_traversal_tag
>;

template <class IteratorT>
class archive_traverser_visitor 
    : public boost::static_visitor<archive_iterator_t>
{
    static_assert(is_same_v<iterator_value_t<IteratorT>, array_view<const char>>);

    using var_iterator = boost::variant<
        IteratorT,
        archive_iterator_t,
        tar_extract_iterator<IteratorT>,
        tar_extract_iterator<archive_iterator_t>
    >;

    template <class SomeIteratorT>
    using iterator_adapter_t = iterator_polymorpic_adapter<
        SomeIteratorT,
        forward_traversal_tag,
        array_view<const char>>;

    std::pair<string_view, archive_type> split_name(string_view nm)
    {
        auto rit = std::find(nm.rbegin(), nm.rend(), '.');
        string_view extstr(rit.base(), nm.end() - rit.base());
        string_view name_wo_ext(nm.begin(), rit.base() - (rit != nm.rend() ? 1 : 0));
        return {name_wo_ext, get_archive_type(extstr)};
    }

public:
    archive_traverser_visitor(string_view name, IteratorT && it, size_t internalbuffsz)
        : buffsz_(internalbuffsz)
    {
        stack_.push_back(std::move(it));
        //stack_.push_back(archive_iterator_t(in_place_type<iterator_adapter_t<IteratorT>>, std::move(it)));
        namearr_.push_back(std::pair(to_string(name), to_string(name)));
    }

    template <class SomeIteratorT>
    archive_iterator_t operator()(SomeIteratorT & it)
    {
        auto const& [fullname, localname] = namearr_.back();
        auto [name_wo_ext, ext_type] = split_name(localname);
        
        if (archive_type::UNKNOWN == ext_type) {
            if constexpr (is_same_v<SomeIteratorT, archive_iterator_t>) {
                return std::move(it);
            } else {
                return archive_iterator_t(in_place_type<iterator_adapter_t<IteratorT>>, std::move(it));
            }
        }

        namearr_.push_back(std::pair(fullname, to_string(name_wo_ext)));

        switch (ext_type)
        {
        case archive_type::GZIP:
            stack_.back() = archive_iterator_t(
                in_place_type<iterator_adapter_t<buffering_mediator_iterator<inflate_iterator<SomeIteratorT>>>>,
                inflate_iterator(std::move(it), true), buffsz_);
            break;
        case archive_type::BZIP2:
            stack_.back() = archive_iterator_t(
                in_place_type<iterator_adapter_t<buffering_mediator_iterator<bz2_decompress_iterator<SomeIteratorT>>>>,
                bz2_decompress_iterator(std::move(it)), buffsz_);
            break;
        case archive_type::TAR:
            stack_.back() = tar_extract_iterator(std::move(it));
            break;
        case archive_type::TGZ:
            stack_.back() = tar_extract_iterator(archive_iterator_t(
                in_place_type<iterator_adapter_t<buffering_mediator_iterator<inflate_iterator<SomeIteratorT>>>>,
                inflate_iterator(std::move(it), true), buffsz_));
            break;
        default:
            BOOST_THROW_EXCEPTION(internal_error("unexpected archive type: %1%"_fmt % (int)ext_type));
        }
        return archive_iterator_t{};
    }

    template <class SomeIteratorT>
    archive_iterator_t operator()(tar_extract_iterator<SomeIteratorT> & it)
    {
        if (it.next()) {
            auto const& [fullname, localname] = namearr_.back();
            namearr_.push_back(std::pair(fullname + "/" + it.current_name(), it.current_name()));
            stack_.push_back(archive_iterator_t(in_place_type<iterator_adapter_t<tar_extract_iterator<SomeIteratorT>>>, std::move(it)));
        }
        throw not_implemented_error();
    }

    bool next(archive_iterator_t & it)
    {
        while (!stack_.empty()) {
            if (it = boost::apply_visitor(*this, stack_.back()); !it.empty()) {
                stack_.pop_back();
                return true;
            }
        }
        return false;
    }

    std::string const& current_name() const
    {
        return namearr_.back().first;
    }

private:
    std::list<var_iterator> stack_;
    std::vector<std::pair<std::string, std::string>> namearr_; //{(full, handled)}
    size_t buffsz_;
};



template <class IteratorT>
class archive_extract_iterator
    : public boost::iterator_facade<
        archive_extract_iterator<IteratorT>,
        array_view<const char>,
        forward_traversal_tag,
        array_view<const char>>
{
    friend class boost::iterator_core_access;

    using archive_visitor_t = archive_traverser_visitor<IteratorT>;

    void increment()
    {
        ++it_;
    }

    array_view<const char> dereference() const
    {
        return *it_;
    }

public:
    archive_extract_iterator(string_view name, IteratorT it, size_t interal_buffsz = DEFAULT_ARCHIVER_INTERNAL_BUFFER_SIZE)
    {
        visitor_ = make_shared<archive_visitor_t>(name, std::move(it), interal_buffsz);
    }

    bool next()
    {
        return visitor_->next(it_);
    }

    std::string const& current_name() const
    {
        return visitor_->current_name();
    }

    bool empty() const
    {
        return !visitor_ || it_.empty();
    }

private:
    shared_ptr<archive_visitor_t> visitor_;
    archive_iterator_t it_;
};

}

#endif // SONIA_UTILITY_ARCHIVE_EXTRACT_ITERATOR_HPP
