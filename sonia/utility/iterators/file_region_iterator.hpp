//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_FILE_REGION_ITERATOR_HPP
#define SONIA_UTILITY_FILE_REGION_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include "sonia/shared_ptr.hpp"
#include "sonia/array_view.hpp"
#include "sonia/type_traits.hpp"

namespace sonia {

class file_mapping_holder
{
    struct fm_cache {
        boost::interprocess::file_mapping fm;
        boost::interprocess::mode_t mode;
        uint64_t size;
        size_t region_size;
    };

public:
    file_mapping_holder(const char* filename, boost::interprocess::mode_t mode, size_t rsz);

    boost::interprocess::file_mapping & file_mapping() const { return fmc_->fm; }
    boost::interprocess::mode_t mode() const { return fmc_->mode; }
    uint64_t file_size() const { return fmc_->size; }
    uint64_t region_size() const { return fmc_->region_size; }

    void truncate(uint64_t sz);

private:
    shared_ptr<fm_cache> fmc_;
};

/**
    file_region_descriptor instancies share the same file_mapping instance
*/
class file_region_descriptor
    : private file_mapping_holder
    , public enable_shared_from_this<file_region_descriptor>
{
    uint64_t fileoffset_;
    boost::interprocess::mapped_region region_;
    weak_ptr<file_region_descriptor> previous_;
    shared_ptr<file_region_descriptor> next_;

public:
    file_region_descriptor(const char* filename, boost::interprocess::mode_t mode, uint64_t offset, size_t region_sz);
    file_region_descriptor(shared_ptr<file_region_descriptor> prev);

    file_region_descriptor(file_region_descriptor const&) = delete;
    file_region_descriptor& operator=(file_region_descriptor const&) = delete;

    ~file_region_descriptor();

    array_view<char> get() const;
    void update_region_size(size_t);

    shared_ptr<file_region_descriptor> get_next() const { return next_; }
    void set_next(shared_ptr<file_region_descriptor> next) { next_ = std::move(next); }

    shared_ptr<file_region_descriptor> get_previous() const { return previous_.lock(); }
    void set_previous(shared_ptr<file_region_descriptor> const& prev) { previous_ = prev; }

    void next_from(file_region_descriptor const& previous);
    void previous_from(file_region_descriptor const& next);

    bool empty() const;

private:
    boost::interprocess::mapped_region create_region(uint64_t offset, size_t region_size);
};

template <class FileRegionIteratorT, typename T>
class file_region_iterator_value_proxy
{
    typedef array_view<T> value_type;

public:
    explicit file_region_iterator_value_proxy(FileRegionIteratorT & it) : it_(it) {}
    file_region_iterator_value_proxy(file_region_iterator_value_proxy &&) = default;

    file_region_iterator_value_proxy(file_region_iterator_value_proxy const&) = delete;
    file_region_iterator_value_proxy & operator = (file_region_iterator_value_proxy const&) = delete;
    file_region_iterator_value_proxy & operator = (file_region_iterator_value_proxy &&) = delete;

    operator value_type() const { return it_.do_dereference(); }

    template <typename ArgT>
    file_region_iterator_value_proxy & operator = (ArgT && ref) { it_.set( std::forward<ArgT>(ref)); return *this; }

private:
    FileRegionIteratorT & it_;
};

class file_region_iterator_base
{
    friend class boost::iterator_core_access;

protected:
    bool equal(file_region_iterator_base const& rhs) const {
        if (empty()) return rhs.empty();
        if (rhs.empty()) return false;
        return region_ == rhs.region_;
    }

    void increment();
    void decrement();

    void set(array_view<const char> val);

public:
    bool empty() const { return !region_ || region_->empty(); }

protected:
    shared_ptr<file_region_descriptor> region_;
};

template <typename T>
class file_region_iterator 
    : public boost::iterator_facade<
        file_region_iterator<T>, array_view<T>, boost::bidirectional_traversal_tag,
        conditional_t<is_const_v<T>, array_view<T>, file_region_iterator_value_proxy<file_region_iterator<T>, T>>
    >, public file_region_iterator_base
{
    friend class boost::iterator_core_access;
    template <class, typename> friend class file_region_iterator_value_proxy;

    static const bool is_writable = !is_const_v<T>;

    decltype(auto) dereference() const {
        if constexpr (is_writable) {
            return file_region_iterator_value_proxy<file_region_iterator, T>(const_cast<file_region_iterator&>(*this));
        } else {
            return do_dereference();
        }
    }

    array_view<T> do_dereference() const {
        array_view<char> raw = region_->get();
        BOOST_ASSERT(0 == raw.size() % sizeof(T));
        return array_view<T>((T*)raw.begin(), raw.size() / sizeof(T));
    }

    void set(array_view<const T> val) {
        array_view<const char> src_raw((const char*)val.begin(), sizeof(T) * val.size());
        file_region_iterator_base::set(src_raw);
    }

public:
    explicit file_region_iterator(const char* name, uint64_t offset = 0, size_t least_region_sz = 1)
    {
        namespace ipc = boost::interprocess;

        region_ = make_shared<file_region_descriptor>(
            name,
            is_const_v<T> ? ipc::read_only : ipc::read_write,
            sizeof(T) * offset, sizeof(T) * least_region_sz);
    }
};

}

#endif // SONIA_UTILITY_FILE_REGION_ITERATOR_HPP
