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
#include <boost/intrusive_ptr.hpp>

#include "sonia/iterator_traits.hpp"
#include "sonia/utility/explicit_operator_bool.hpp"
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
    archive_iterator_polymorphic() : refs(1) {}
    archive_iterator_polymorphic(archive_iterator_polymorphic const&) : refs(1) {}

    virtual bool next(archive_iterator &) = 0;
    virtual std::string const& name() const = 0;
    virtual archive_iterator * pbase() = 0;

    friend void intrusive_ptr_add_ref(archive_iterator_polymorphic * p)
    {
        ++p->refs;
    }

    friend void intrusive_ptr_release(archive_iterator_polymorphic * p)
    {
        if (0 == --p->refs) {
            delete[] reinterpret_cast<char*>(p);
        }
    }

    unsigned int refs;
};

class archive_iterator_impl
{
public:
    archive_iterator_impl() {}

    template <class T, typename ... ArgsT>
    archive_iterator_impl(in_place_type_t<T>, ArgsT && ... args)
    {
        char * p = new char[sizeof(T)];
        try {
            new (p) T(std::forward<ArgsT>(args)...);
        } catch (...) {
            delete[] p;
            throw;
        }
        ptr.reset(reinterpret_cast<T*>(p), false);
    }

    archive_iterator_impl(archive_iterator_impl const&) = default;
    archive_iterator_impl(archive_iterator_impl &&) = default;
    archive_iterator_impl& operator= (archive_iterator_impl const&) = default;
    archive_iterator_impl& operator= (archive_iterator_impl &&) = default;
    
    void increment()
    {
        if (1 != ptr->refs) {
            size_t sz = ptr->get_sizeof();
            char * p = new char[sz];
            try {
                ptr.reset(static_cast<archive_iterator_polymorphic*>(ptr->clone(p, sz)), false);
            } catch (...) {
                delete[] p;
                throw;
            }
        }
        ptr->increment();
    }
    
    bool operator!() const { return !ptr; }
    BOOST_EXPLICIT_OPERATOR_BOOL();

    void reset() { ptr.reset(); }

    archive_iterator_polymorphic * operator-> () const { return ptr.get(); }
    boost::intrusive_ptr<archive_iterator_polymorphic> ptr;
};

class archive_iterator
    : public wrapper_iterator<
        archive_iterator_impl,
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
        while (impl) {
            bool r = impl->next(*this);
            if (r || !impl) return r;
        }
        return false;
    }

    archive_iterator * pbase() { return impl->pbase(); }

    void reset() { impl.reset(); }
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

    bool do_next(archive_iterator &);

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

    bool next(archive_iterator & ax) override final
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
                archive_iterator tmp(std::move(base_type::it_.base()));
                ax = std::move(tmp);
                return false;
            }
        }

        return do_next(ax);
    }

    std::string const& name() const override final
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
