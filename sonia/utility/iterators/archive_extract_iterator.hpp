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

namespace sonia { namespace archive_detail {

enum class archive_type
{
    UNDEFINED,
    TAR,        // tar
    GZIP,       // gz
    BZIP2,      // bz2
    TGZ,        // gzipped tar
    UNKNOWN
};

archive_type get_archive_type(string_view str);
std::pair<string_view, archive_type> split_name(string_view nm);

class archive_iterator;

class archive_iterator_polymorphic
    : public iterator_polymorphic<array_view<const char>>
{
public:
    virtual bool next(shared_ptr<archive_iterator_polymorphic> &) = 0;
    virtual std::string const& name() = 0;
    virtual archive_iterator * pbase() = 0;
};

class archive_iterator
    : public wrapper_iterator<
        automatic_pointer<shared_ptr<archive_iterator_polymorphic>>,
        array_view<const char>,
        std::forward_iterator_tag
    >
{
    using base_type = archive_iterator::wrapper_iterator_t;

public:
    using base_type::base_type;

    std::string const& name() const { return impl->name(); }

    bool next()
    {
        while (impl.ptr) {
            bool r = impl->next(impl.ptr);
            if (r || !impl.ptr) return r;
        }
        return false;
    }
};

class extract_iterator_polymorpic_adapter_base
{
protected:
    std::string full_name_;
    std::string part_name_;
    archive_type type_;
    size_t buffsz_;

public:
    extract_iterator_polymorpic_adapter_base(std::string full_name, std::string part_name, size_t buffsz)
        : full_name_(std::move(full_name))
        , part_name_(std::move(part_name))
        , type_(archive_type::UNDEFINED)
        , buffsz_(buffsz)
    {}

    extract_iterator_polymorpic_adapter_base(std::string name, size_t buffsz)
        : full_name_(std::move(name))
        , type_(archive_type::UNDEFINED)
        , buffsz_(buffsz)
    {}

    bool do_next(shared_ptr<archive_iterator_polymorphic> & ptr);

    std::string const& do_get_name() const
    {
        if (!part_name_.empty()) return part_name_;
        return full_name_;
    }
};

template <class IteratorT>
class extract_iterator_polymorpic_adapter
    : public iterator_polymorpic_adapter_base<
        IteratorT,
        forward_traversal_tag,
        archive_iterator_polymorphic>
    , extract_iterator_polymorpic_adapter_base
{
    using base_type = typename extract_iterator_polymorpic_adapter::adapter_base_t;

public:
    template <typename ... ArgsT>
    explicit extract_iterator_polymorpic_adapter(std::string fullname, size_t buffsz, ArgsT && ... args)
        : base_type(std::forward<ArgsT>(args)...)
        , extract_iterator_polymorpic_adapter_base(std::move(fullname), buffsz)
    {}

    template <typename ... ArgsT>
    explicit extract_iterator_polymorpic_adapter(std::string fullname, std::string partname, size_t buffsz, ArgsT && ... args)
        : base_type(std::forward<ArgsT>(args)...)
        , extract_iterator_polymorpic_adapter_base(std::move(fullname), std::move(partname), buffsz)
    {}

    array_view<const char> dereference() const override final { return *base_type::it_; }
    size_t get_sizeof() const override final { return sizeof(extract_iterator_polymorpic_adapter); }
    polymorphic_clonable * clone(void * address, size_t sz) const override final { return base_type::do_clone(this, address, sz); }
    polymorphic_movable * move(void * address, size_t sz) override final { return base_type::do_move(this, address, sz); }

    archive_iterator * pbase() override final
    {
        if constexpr (is_template_instance_v<tar_extract_iterator, IteratorT>) {
            return &base_type::it_.base();
        } else if constexpr (is_template_instance_v<buffering_mediator_iterator, IteratorT>) {
            // structure: buffering_mediator_iterator<some decompress iterator <archive_iterator>>
            return &base_type::it_.base().base();
        } else {
            return nullptr;
        }
    }

    bool next(shared_ptr<archive_iterator_polymorphic> & ptr) override final
    {
        if constexpr (is_template_instance_v<tar_extract_iterator, IteratorT>) {
            while (!base_type::it_.empty()) ++base_type::it_;

            bool res = base_type::it_.next();
            if (res) {
                part_name_ = full_name_ + '/' + base_type::it_.name();
                type_ = archive_type::UNDEFINED;
            } else {
                // skip possible odd data
                while (!base_type::it_.base().empty()) ++base_type::it_.base();
                // note: it_.base() is an archive_iterator
                ptr = std::move(base_type::it_.base().impl.ptr);
                return false;
            }
        }

        return do_next(ptr);
    }

    std::string const& name() override final
    {
        return do_get_name();
    }

};

} // sonia::archive_detail

using archive_iterator = archive_detail::archive_iterator;

template <class IteratorT>
archive_iterator make_archive_extract_iterator(std::string name, IteratorT && it, size_t interal_buffsz = DEFAULT_ARCHIVER_INTERNAL_BUFFER_SIZE)
{
    return archive_iterator(in_place_type<archive_detail::extract_iterator_polymorpic_adapter<remove_cvref_t<IteratorT>>>, std::move(name), interal_buffsz, std::forward<IteratorT>(it));
}

}

#endif // SONIA_UTILITY_ARCHIVE_EXTRACT_ITERATOR_HPP
